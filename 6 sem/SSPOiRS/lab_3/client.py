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

def connect_to_server(host, port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.settimeout(0.5)  # Увеличиваем тайм-аут
    try:
        client_socket.connect((host, port))
        print("Успешное подключение к серверу.")
        return client_socket
    except Exception as e:
        print("Не удалось подключиться к серверу:", e)
        return None


def handle_commands(client_socket):
    while True:
        command = input("Введите команду: ")
        if command.upper() == "EXIT":
            print("Завершение соединения.")
            client_socket.close()
            sys.exit()

        try:
            client_socket.send(command.encode("utf-8"))
            response = client_socket.recv(1024).decode("utf-8")

            if response == "File not found":
                print("Файл не найден на сервере.")
            elif response == "CONNECTED":
                print("Подключение успешно. Происходит скачивание файла.")
            elif response == "Файл отправлен":
                print("Сервер закончил отправку файла.")
            else:
                print(f"Ответ от сервера: {response}")

            if command.upper().startswith("DOWNLOAD"):
                filename = command.split()[1]
                receive_file(client_socket, filename)

        except Exception as e:
            print(f"Ошибка при выполнении команды: {e}")
            client_socket.close()
            break


if name == "main":
    host = "localhost"
    port = 8080

    client_socket = connect_to_server(host, port)

    if client_socket:
        handle_commands(client_socket)
