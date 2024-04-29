import socket
import os
import struct
import select
from datetime import datetime

# Функция для отправки файла
def send_file(sock, filename):
    try:
        filesize = os.path.getsize(filename)
        sock.send(struct.pack("<Q", filesize))  # Отправляем размер файла

        with open(filename, "rb") as f:
            remaining_bytes = filesize
            while remaining_bytes > 0:
                chunk_size = min(4096, remaining_bytes)  # Размер отправляемого чанка
                chunk = f.read(chunk_size)
                sock.send(chunk)  # Отправляем данные
                remaining_bytes -= len(chunk)  # Уменьшаем оставшийся размер

        sock.send("Файл отправлен".encode("utf-8"))  # Уведомление об успешной отправке

    except Exception as e:
        print(f"Ошибка при отправке файла: {e}")
        sock.send(f"Ошибка при отправке файла: {e}".encode("utf-8"))  # Сообщение об ошибке

# Функция для обработки соединений
def handle_connections(server_socket):
    client_sockets = [server_socket]  # Список отслеживаемых сокетов

    while True:
        readable, _, _ = select.select(client_sockets, [], [])  # Мультиплексирование сокетов

        for sock in readable:
            if sock is server_socket:  # Если активен основной сокет
                client_socket, address = server_socket.accept()
                client_sockets.append(client_socket)  # Добавляем в список
                client_socket.send("CONNECTED".encode("utf-8"))
                print(f"Новое соединение: {address}")

            else:  # Если активен клиентский сокет
                try:
                    data = sock.recv(1024)  # Принимаем команду
                    if not data:  # Если соединение разорвано
                        client_sockets.remove(sock)  # Удаляем сокет
                        sock.close()
                    else:
                        command, *args = data.decode("utf-8", errors='replace').split()  # Разделяем команду
                        if command.upper() == "DOWNLOAD":
                            filename = " ".join(args)  # Получаем имя файла
                            if os.path.exists(filename):
                                send_file(sock, filename)  # Отправляем файл
                            else:
                                sock.send("File not found".encode("utf-8"))  # Файл не найден
                        elif command.upper() == "ECHO":  # Команда эхо
                            response = " ".join(args)  # Сформируем ответ
                            sock.send(response.encode("utf-8"))  # Отправим ответ клиенту
                        elif command.upper() == "DATE":  # Команда дата
                            current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                            sock.send(current_time.encode("utf-8"))  # Отправим текущую дату
                        else:
                            sock.send("Неизвестная команда".encode("utf-8"))  # Некорректная команда
                except Exception as e:
                    print(f"Ошибка при обработке команды: {e}")
                    if sock in client_sockets:
                        client_sockets.remove(sock)  # Удаляем сокет при ошибке
                        sock.close()  # Закрываем соединение


# Запускаем сервер
if __name__ == "__main__":
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(("", 8080))  # Привязываем к порту 8080
    server_socket.listen(5)  # Максимум 5 подключений в очереди
    print("Сервер запущен на порту 8080.")

    handle_connections(server_socket)  # Запуск обработки соединений
