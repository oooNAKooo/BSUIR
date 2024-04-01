import socket
import os
import struct
from datetime import datetime
import pickle
import time  # Импортируем модуль для работы со временем

# Словарь для хранения состояния передачи файлов
transfer_state = {}


def send_file(sck: socket.socket, filename, client_address, file_position=0):
    filesize = os.path.getsize(filename)
    if file_position > 0:
        sck.sendto(struct.pack("<Q", filesize - file_position), client_address)
    else:
        sck.sendto(struct.pack("<Q", filesize), client_address)

    start_time = time.time()  # Запоминаем текущее время перед началом передачи файла

    with open(filename, "rb") as f:
        f.seek(file_position)
        remaining_bytes = filesize - file_position
        while remaining_bytes > 0:
            chunk_size = min(remaining_bytes, 2048)  # Изменим размер буфера на 4096 байт
            chunk = f.read(chunk_size)
            sck.sendto(chunk, client_address)
            remaining_bytes -= len(chunk)

    end_time = time.time()  # Запоминаем текущее время после завершения передачи файла
    transfer_time = end_time - start_time  # Вычисляем время передачи файла в секундах
    bitrate = filesize / transfer_time  # Вычисляем битрейт (скорость передачи файла в байтах в секунду)

    print(f"Загрузка файла завершена. Битрейт: {bitrate:.2f} байт/сек.")
    sck.sendto("Файл отправлен".encode('utf-8'), client_address)


# Функция для сохранения состояния передачи файлов
def save_transfer_state():
    with open('transfer_state.pickle', 'wb') as handle:
        pickle.dump(transfer_state, handle, protocol=pickle.HIGHEST_PROTOCOL)


# Загрузка состояния передачи файлов при запуске сервера
if os.path.exists('transfer_state.pickle'):
    with open('transfer_state.pickle', 'rb') as handle:
        transfer_state = pickle.load(handle)


# Создайте функцию для повторной отправки файла при подключении клиента
def resend_file_if_incomplete(client_socket, client_id):
    if client_id in transfer_state:
        filename, file_position = transfer_state[client_id]
        send_file(client_socket, filename, client_id, file_position)


def handle_client_connection(server_socket):
    while True:
        data, client_address = server_socket.recvfrom(1024)
        client_id = client_address
        print(f"Установлено соединение с {client_address}")
        server_socket.sendto("CONNECTED".encode('utf-8'), client_address)

        # Проверяем наличие недокачанного файла и отправляем его, если есть
        resend_file_if_incomplete(server_socket, client_id)

        while True:
            try:
                data = server_socket.recvfrom(1024)[0].decode('utf-8')
                if not data:
                    break
                print(f"Получено сообщение от клиента: {data}")
                command, *args = data.split()
                if command.upper() == "ECHO":
                    response = " ".join(args)
                    server_socket.sendto(response.encode('utf-8'), client_address)
                elif command.upper() == "TIME":
                    time_now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    print(f"Время на сервере: {time_now}.")
                    server_socket.sendto(f"Время на сервере: {time_now}.".encode('utf-8'), client_address)
                elif command.upper() in ["DOWNLOAD", "СКАЧАТЬ"]:
                    filename = " ".join(args)
                    print(f"Старт загрузки: ")
                    if os.path.exists(filename):
                        send_file(server_socket, filename, client_address)  # Передача client_address
                        # Обновляем состояние передачи файла
                        transfer_state[client_id] = (filename, os.path.getsize(filename))
                        save_transfer_state()  # Сохраняем состояние
                        print("Загрузка файла завершена.")
                    else:
                        server_socket.sendto("File not found".encode('utf-8'), client_address)
                elif command.upper() == "EXIT":
                    server_socket.sendto("Команда EXIT получена. Завершение соединения.".encode('utf-8'), client_address)
                    break
                else:
                    server_socket.sendto("Сообщение получено.".encode('utf-8'), client_address)
            except (ConnectionResetError, OSError) as e:
                print(f"Ошибка во время обработки запроса: {e}")
                break


if __name__ == "__main__":
    # Создаем сокет
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    host = ""  # Пустая строка означает, что мы принимаем соединения на всех доступных интерфейсах
    port = 8080
    server_socket.bind((host, port))
    print(f"Сервер запущен. Ожидание подключений на порту {port}...")
    # Обработка подключений клиентов
    handle_client_connection(server_socket)
