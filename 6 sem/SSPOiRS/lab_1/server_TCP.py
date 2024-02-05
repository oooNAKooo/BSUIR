import socket
import os
import struct
from datetime import datetime
import pickle

# Словарь для хранения состояния передачи файлов
transfer_state = {}


def send_file(sck: socket.socket, filename, file_position=0):
    filesize = os.path.getsize(filename)
    if file_position > 0:
        sck.send(struct.pack("<Q", filesize - file_position))
    else:
        sck.send(struct.pack("<Q", filesize))

    with open(filename, "rb") as f:
        f.seek(file_position)
        remaining_bytes = filesize - file_position
        while remaining_bytes > 0:
            chunk_size = min(remaining_bytes, 1024)  # Максимальный размер чанка - 1024 байта
            chunk = f.read(chunk_size)
            bytes_sent = sck.send(chunk)
            remaining_bytes -= bytes_sent

    sck.send("Файл отправлен".encode('utf-8'))


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
        client_socket, address = server_socket.accept()
        client_id = address
        print(f"Установлено соединение с {address}")
        client_socket.sendall("CONNECTED".encode('utf-8'))

        # Проверяем наличие недокачанного файла и отправляем его, если есть
        resend_file_if_incomplete(client_socket, client_id)

        while True:
            try:
                data = client_socket.recv(1024).decode('utf-8')
                if not data:
                    break
                print(f"Получено сообщение от клиента: {data}")
                command, *args = data.split()
                if command.upper() == "ECHO" or command.upper() == "echo" or command.upper() == "ЭХО" or command.upper() == "эхо":
                    response = " ".join(args)
                    client_socket.sendall(response.encode('utf-8'))
                elif command.upper() == "TIME" or command.upper() == "time" or command.upper() == "ВРЕМЯ" or command.upper() == "время":
                    time_now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    print(f"Время на сервере: {time_now}.")
                    client_socket.sendall(f"Время на сервере: {time_now}.".encode('utf-8'))
                elif command.upper() in ["DOWNLOAD", "СКАЧАТЬ"]:
                    filename = " ".join(args)
                    if os.path.exists(filename):
                        send_file(client_socket, filename)
                        # Обновляем состояние передачи файла
                        transfer_state[client_id] = (filename, os.path.getsize(filename))
                        save_transfer_state()  # Сохраняем состояние
                        print("Загрузка файла завершена.")
                    else:
                        client_socket.sendall("File not found".encode('utf-8'))
                elif command.upper() == "EXIT" or command.upper() == "EXIT" or command.upper() == "ВЫХОД" or command.upper() == "выход":
                    client_socket.sendall("Команда EXIT получена. Завершение соединения.".encode('utf-8'))
                    break
                else:
                    client_socket.sendall("Сообщение получено.".encode('utf-8'))
            except ConnectionResetError:
                print("Соединение с клиентом было разорвано.")
                break
        client_socket.close()




if __name__ == "__main__":

    # Создаем сокет
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = ""  # Пустая строка означает, что мы принимаем соединения на всех доступных интерфейсах
    port = 8080
    server_socket.bind((host, port))
    server_socket.listen(5)
    print(f"Сервер запущен. Ожидание подключений на порту {port}...")
    # Обработка подключений клиентов
    handle_client_connection(server_socket)


    # Очистка незавершенных передач при остановке сервера
    def cleanup():
        for key, (filename, _) in transfer_state.items():
            os.remove(filename)
        os.remove('transfer_state.pickle')


    cleanup()

