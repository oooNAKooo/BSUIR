import socket
import sys
from datetime import datetime

# Функция для приема файла от сервера
def receive_file(sck: socket.socket, filename):
    try:
        start_time = datetime.now()  # Запоминаем время начала передачи файла
        start_time_str = start_time.strftime("%H:%M:%S")
        print(f"Начало загрузки: {start_time_str}.")
        total_bytes = 0  # Общее количество переданных байтов
        with open(filename, 'wb') as file:
            while True:
                data = sck.recv(1024)
                if not data:
                    break
                file.write(data)
                total_bytes += len(data)  # Увеличиваем количество переданных байтов
                end_time = datetime.now()  # Запоминаем время завершения передачи файла

    except socket.timeout:
        print("Файл успешно скачан.")
        end_time_str = end_time.strftime("%H:%M:%S")
        print(f"Загрузка завершена в {end_time_str}.")
        transfer_time = (end_time - start_time).total_seconds()  # Вычисляем время передачи файла в секундах
        print(f"Время передачи файла (с): {transfer_time} сек.")
        print(f"Размер скачанного файла: {total_bytes} байт.")
        bitrate = total_bytes / transfer_time
        print(f"Битрейт передачи файла: {bitrate} байт/сек.")
    except Exception as e:
        print(f"Произошла ошибка при скачивании файла: {str(e)}.")
        if str(e) == "[WinError 10054] Удаленный хост принудительно разорвал существующее подключение":
            print("END_CONNECTION")
            sys.exit()

# Функция для подключения к серверу
def connect_to_server(host, port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.settimeout(0.5)  # Устанавливаем тайм-аут в 0.5 секунд
    try:
        client_socket.connect((host, port))
        confirmation = client_socket.recv(1024).decode('utf-8')
        if confirmation != "CONNECTED":
            print("Ошибка: Не удалось подключиться к серверу.")
            client_socket.close()
            return None
        else:
            print("Успешное подключение к серверу.")
            return client_socket
    except UnicodeDecodeError as e:
        print(f"Ошибка декодирования: {e}. Продолжаем работу без подтверждения.")
    except (ConnectionResetError, socket.error) as e:
        print("Произошел разрыв соединения:", e)
        return None

# Функция для обработки команд от пользователя
def handle_commands(client_socket):
    while True:
        message = input("Введите команду: ")
        if message.upper() == "EXIT" or message.upper() == "ВЫХОД":
            print("Завершение соединения.")
            client_socket.sendall(message.encode('utf-8'))
            sys.exit()
        try:
            client_socket.sendall(message.encode('utf-8'))
            data = client_socket.recv(1024)
            if data == b"File not found":
                print("Файл не найден на сервере.")
            else:
                split_message = message.split()
                if len(split_message) > 1 and (
                        split_message[0].upper() == "DOWNLOAD" or split_message[0].upper() == "СКАЧАТЬ"):
                    filename = split_message[1]
                    receive_file(client_socket, filename)
                else:
                    print(data.decode('utf-8'))  # Печатаем ответ сервера
        except socket.timeout:
            print("Произошел тайм-аут при приеме файла.")
        except ConnectionResetError as e:
            print("Соединение с сервером было разорвано.")
            break

if __name__ == "__main__":
    server_host = "192.168.123.83"  # IP-адрес сервера
    server_port = 8080

    print(f"Клиент запущен. Выбран IP-хоста {server_host}, выбран порт {server_port}. Ожидание подключения к серверу.")

    while True:
        client_socket = connect_to_server(server_host, server_port)
        if client_socket:
            handle_commands(client_socket)
            client_socket.close()
        choice = input("Хотите попробовать восстановить соединение? (да/нет): ").lower()
        if choice != "да":
            print("Завершение работы клиента.")
            break
