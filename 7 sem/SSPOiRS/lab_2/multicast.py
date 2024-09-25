import socket
import struct
import threading
import netifaces


# Получаем IP-адрес правильного интерфейса (например, 192.168.3.83)
def get_interface_ip():
    iface = netifaces.gateways()['default'][netifaces.AF_INET][1]  # Выбираем интерфейс с маршрутизатором по умолчанию
    ip = netifaces.ifaddresses(iface)[netifaces.AF_INET][0]['addr']
    return ip


# Отправляем сообщение в мультикастовом режиме
def send_multicast_message(message, multicast_group, port, interface_ip):
    # Создаем UDP-сокет
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

    # Устанавливаем TTL (Time to Live)
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)

    # Указываем интерфейс, через который будут отправляться пакеты
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_IF, socket.inet_aton(interface_ip))

    # Отправляем сообщение в мультикаст-группу
    sock.sendto(message.encode(), (multicast_group, port))
    sock.close()


# Присоединяемся к группе многоадресной передачи
def join_multicast_group(multicast_group, port, interface_ip):
    # Создаем UDP-сокет
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

    # Привязываем сокет к указанному порту на любом интерфейсе
    sock.bind(('', port))

    # Добавляем сокет в мультикаст-группу
    group = socket.inet_aton(multicast_group)
    mreq = struct.pack('4s4s', group, socket.inet_aton(interface_ip))  # Явно указываем IP интерфейса
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    return sock


# Прием сообщений
def receive_messages(sock):
    while True:
        data, addr = sock.recvfrom(1024)
        print(f"Received message: {data.decode()} from {addr}")


# Поток для приема сообщений
def start_receiving_thread(sock):
    receive_thread = threading.Thread(target=receive_messages, args=(sock,))
    receive_thread.daemon = True
    receive_thread.start()


# Основная программа
if __name__ == "__main__":
    # Мультикаст-группа и порт
    multicast_group = '224.1.1.1'
    port = 12345

    # Получаем IP правильного сетевого интерфейса
    interface_ip = get_interface_ip()
    print(f"Using interface IP: {interface_ip}")

    # Присоединяемся к группе многоадресной передачи
    multicast_sock = join_multicast_group(multicast_group, port, interface_ip)

    # Запускаем прием сообщений
    start_receiving_thread(multicast_sock)

    # Пример отправки сообщений в мультикаст
    while True:
        msg = input("Enter message to multicast: ")
        send_multicast_message(msg, multicast_group, port, interface_ip)
