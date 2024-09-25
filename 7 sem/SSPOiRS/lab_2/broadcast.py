import socket
import struct
import netifaces
import threading


# Получаем параметры сети
def get_network_parameters():
    iface = netifaces.gateways()['default'][netifaces.AF_INET][1]
    ip = netifaces.ifaddresses(iface)[netifaces.AF_INET][0]['addr']
    netmask = netifaces.ifaddresses(iface)[netifaces.AF_INET][0]['netmask']
    broadcast = netifaces.ifaddresses(iface)[netifaces.AF_INET][0]['broadcast']
    return ip, netmask, broadcast


# Отправляем сообщение в широковещательном режиме
def send_broadcast_message(message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    broadcast_ip = get_network_parameters()[2]
    sock.sendto(message.encode(), (broadcast_ip, 12345))
    sock.close()


# Присоединяемся к группе многоадресной передачи
def join_multicast_group(multicast_group, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('', port))
    group = socket.inet_aton(multicast_group)
    mreq = struct.pack('4sL', group, socket.INADDR_ANY)
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
    return sock


# Выход из группы многоадресной передачи
def leave_multicast_group(sock, multicast_group):
    group = socket.inet_aton(multicast_group)
    mreq = struct.pack('4sL', group, socket.INADDR_ANY)
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)
    sock.close()


# Прием сообщений и игнорирование хостов
def receive_messages(sock, ignored_hosts):
    while True:
        data, addr = sock.recvfrom(1024)
        if addr[0] in ignored_hosts:
            print(f"Ignoring message from {addr}")
            continue
        print(f"Received message: {data.decode()} from {addr}")


# Поток для приема сообщений
def start_receiving_thread(sock, ignored_hosts):
    receive_thread = threading.Thread(target=receive_messages, args=(sock, ignored_hosts))
    receive_thread.daemon = True
    receive_thread.start()


# Основная программа
if __name__ == "__main__":
    ip, netmask, broadcast = get_network_parameters()
    print(f"IP: {ip}, Netmask: {netmask}, Broadcast: {broadcast}")

    # Игнорируем список хостов (например, '192.168.1.100')
    ignored_hosts = ['192.168.1.100']

    # Присоединяемся к группе многоадресной передачи
    multicast_group = '224.1.1.1'
    port = 12345
    multicast_sock = join_multicast_group(multicast_group, port)

    # Запускаем прием сообщений
    start_receiving_thread(multicast_sock, ignored_hosts)

    # Пример отправки сообщений
    while True:
        msg = input("Enter message to broadcast: ")
        send_broadcast_message(msg)
