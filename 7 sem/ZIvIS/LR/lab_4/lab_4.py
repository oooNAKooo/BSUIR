from Crypto.Cipher import AES, DES, ARC2
from Crypto.Util.Padding import pad, unpad
from Crypto.Hash import SHA256
import base64
import os
import secrets


def generate_session_key(algorithm):
    """Генерация случайного сессионного ключа в зависимости от алгоритма."""
    try:
        if algorithm == 'AES':
            return secrets.token_bytes(16)  # 16 байт для AES-128
        elif algorithm == 'AES-256':
            return secrets.token_bytes(32)  # 32 байта для AES-256
        elif algorithm == 'DES':
            return secrets.token_bytes(8)  # 8 байт для DES
        elif algorithm == 'RC2':
            return secrets.token_bytes(16)  # 16 байт для RC2
        else:
            raise ValueError("Неизвестный алгоритм")
    except Exception as e:
        print(f"Ошибка при генерации сессионного ключа: {e}")


def create_fixed_key(passphrase, algorithm):
    """Создание фиксированного ключа из строки (пароля)."""
    try:
        # Преобразуем строку-пароль в байты
        passphrase_bytes = passphrase.encode('utf-8')

        # Создаём хэш SHA256
        hasher = SHA256.new()
        hasher.update(passphrase_bytes)

        # Определяем длину ключа для алгоритма
        if algorithm == 'AES' or algorithm == 'AES-256':
            return hasher.digest()[:32]  # 32 байта для AES-256
        elif algorithm == 'DES':
            return hasher.digest()[:8]  # 8 байт для DES
        elif algorithm == 'RC2':
            return hasher.digest()[:16]  # 16 байт для RC2
        else:
            raise ValueError("Неизвестный алгоритм")
    except Exception as e:
        print(f"Ошибка при создании фиксированного ключа: {e}")


def encrypt_file(file_path, session_key, algorithm):
    try:
        # Используем сессионный или фиксированный ключ для шифрования
        if algorithm == 'AES' or algorithm == 'AES-256':
            cipher = AES.new(session_key, AES.MODE_CBC)
            iv_length = AES.block_size
        elif algorithm == 'DES':
            cipher = DES.new(session_key, DES.MODE_CBC)
            iv_length = DES.block_size
        elif algorithm == 'RC2':
            cipher = ARC2.new(session_key, ARC2.MODE_CBC)
            iv_length = ARC2.block_size
        else:
            raise ValueError("Неизвестный алгоритм")

        iv = cipher.iv
        with open(file_path, 'rb') as f:
            plaintext = f.read()
        ciphertext = iv + cipher.encrypt(pad(plaintext, cipher.block_size))

        # Кодируем зашифрованные данные в Base64
        encoded_ciphertext = base64.b64encode(ciphertext)

        # Путь для сохранения зашифрованного файла с расширением .txt
        encrypted_file_path = 'D:/encrypted_test.txt'

        with open(encrypted_file_path, 'wb') as f:
            f.write(encoded_ciphertext)
        print(f"Файл {file_path} зашифрован в {encrypted_file_path}")

    except FileNotFoundError:
        print(f"Ошибка: Файл {file_path} не найден.")
    except Exception as e:
        print(f"Ошибка при шифровании файла: {e}")


def decrypt_file(algorithm):
    try:
        # Путь для чтения зашифрованного файла
        encrypted_file_path = 'D:/encrypted_test.txt'

        if not os.path.exists(encrypted_file_path):
            print(f"Файл {encrypted_file_path} не существует.")
            return

        with open(encrypted_file_path, 'rb') as f:
            encoded_ciphertext = f.read()

        # Декодируем данные из Base64
        ciphertext = base64.b64decode(encoded_ciphertext)

        # Запрашиваем строку или сессионный ключ
        key_choice = input("Введите тип ключа (session/fixed): ").lower()
        if key_choice == 'fixed':
            passphrase = input("Введите строку для фиксированного ключа: ")
            session_key = create_fixed_key(passphrase, algorithm)
        elif key_choice == 'session':
            session_key_path = 'D:/session_key.txt'
            if not os.path.exists(session_key_path):
                print(f"Файл с сессионным ключом {session_key_path} не существует.")
                return
            with open(session_key_path, 'r') as f:
                session_key = base64.b64decode(f.read().strip())
        else:
            print("Неизвестный выбор. Пожалуйста, используйте 'session' или 'fixed'.")
            return

        iv_length = 0
        if algorithm == 'AES' or algorithm == 'AES-256':
            iv_length = AES.block_size
            cipher = AES.new(session_key, AES.MODE_CBC, ciphertext[:iv_length])
        elif algorithm == 'DES':
            iv_length = DES.block_size
            cipher = DES.new(session_key, DES.MODE_CBC, ciphertext[:iv_length])
        elif algorithm == 'RC2':
            iv_length = ARC2.block_size
            cipher = ARC2.new(session_key, ARC2.MODE_CBC, ciphertext[:iv_length])
        else:
            raise ValueError("Неизвестный алгоритм")

        plaintext = unpad(cipher.decrypt(ciphertext[iv_length:]), cipher.block_size)

        # Путь для сохранения расшифрованного файла
        decrypted_file_path = 'D:/decrypted_test.txt'
        with open(decrypted_file_path, 'wb') as f:
            f.write(plaintext)
        print(f"Файл {encrypted_file_path} расшифрован в {decrypted_file_path}")

    except Exception as e:
        print(f"Ошибка при расшифровке файла: {e}")


if __name__ == "__main__":
    while True:
        try:
            algorithm = input("Выберите алгоритм (AES/AES-256/DES/RC2) или 'exit' для выхода: ").upper()
            if algorithm == 'EXIT':
                print("Выход из программы.")
                break

            operation = input("Введите тип операции (encrypt/decrypt): ").lower()
            if operation == "encrypt":
                key_choice = input("Выберите тип ключа (session/fixed): ").lower()
                if key_choice == "session":
                    session_key = generate_session_key(algorithm)
                    # Сохраняем сессионный ключ
                    session_key_path = 'D:/session_key.txt'
                    with open(session_key_path, 'w') as f:
                        f.write(base64.b64encode(session_key).decode('utf-8'))
                    print(f"Сессионный ключ сохранён в {session_key_path}")
                elif key_choice == "fixed":
                    passphrase = input("Введите строку для фиксированного ключа: ")
                    session_key = create_fixed_key(passphrase, algorithm)
                else:
                    print("Неизвестный тип ключа.")
                    continue

                file_path = input("Введите путь к файлу для шифрования: ")
                encrypt_file(file_path, session_key, algorithm)
            elif operation == "decrypt":
                decrypt_file(algorithm)
            else:
                print("Неизвестная операция. Пожалуйста, используйте 'encrypt' или 'decrypt'.")
        except Exception as e:
            print(f"Ошибка: {e}")
