import socket
import os
import struct
from datetime import datetime
import select


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


def handle_client_connection(server_socket):
    # Список для отслеживания сокетов клиентов
    client_sockets = [server_socket]
    # Cписок сокетов, с которыми будет работать сервер

    while True:
        # Используем select для ожидания событий на сокетах
        readable, writable, _ = select.select(client_sockets, client_sockets, [])
        # 1 - список клиентов на чтение -> readable (recv())
        # 2 - список клиентов на запись -> writable (send())
        # 3 - список клиентов исключение (ошибки) -> _ (т.е. пропускаем их)

        # sock - список сокетов клиента
        for sock in readable:
            # если готов клиент
            if sock is server_socket:
                # Если сообщение на сервер, значит подключение
                # Принимаем новое соединение
                client_socket, address = server_socket.accept()
                # Метод accept() заблокан, пока не было нового соединения
                client_sockets.append(client_socket)
                # Метод append() для добавления элемента в конец списка
                print(f"Установлено соединение с {address}")
                client_socket.send("CONNECTED".encode('utf-8'))

            else:
                # Обрабатываем данные от клиента
                data = sock.recv(1024).decode('utf-8')
                if not data:
                    # Закрываем соединение и удаляем сокет из списка
                    print(f"Соединение с клиентом {sock.getpeername()} было разорвано.")
                    sock.close()
                    client_sockets.remove(sock)
                else:
                    print(f"Получено сообщение от клиента {sock.getpeername()}: {data}")
                    command, *args = data.split()
                    if command.upper() == "ECHO" or command.upper() == "echo" or command.upper() == "ЭХО" or command.upper() == "эхо":
                        response = " ".join(args)
                        sock.send(response.encode('utf-8'))
                    elif command.upper() == "TIME" or command.upper() == "time" or command.upper() == "ВРЕМЯ" or command.upper() == "время":
                        time_now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                        print(f"Время на сервере: {time_now}.")
                        sock.send(f"Время на сервере: {time_now}.".encode('utf-8'))
                    elif command.upper() in ["DOWNLOAD", "СКАЧАТЬ"]:
                        filename = " ".join(args)
                        if os.path.exists(filename):
                            send_file(sock, filename)
                            print("Загрузка файла завершена.")
                        else:
                            sock.send("File not found".encode('utf-8'))
                    elif command.upper() == "EXIT" or command.upper() == "EXIT" or command.upper() == "ВЫХОД" or command.upper() == "выход":
                        sock.send("Команда EXIT получена. Завершение соединения.".encode('utf-8'))


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
