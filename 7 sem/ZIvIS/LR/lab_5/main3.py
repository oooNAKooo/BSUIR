import os
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import serialization

VALUE = "QWERTY"  # Ожидаемое значение для проверки
PUBLIC_KEY_FILE = "dynamic_public_key.pem"
PRIVATE_KEY_FILE = "private_key.pem"


def generate_static_keys(private_key_file=PRIVATE_KEY_FILE, public_key_file=PUBLIC_KEY_FILE):
    """Генерация пары ключей RSA и сохранение их в файлы."""
    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
        backend=default_backend()
    )
    public_key = private_key.public_key()

    # Сохранение закрытого ключа
    with open(private_key_file, "wb") as f:
        f.write(private_key.private_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PrivateFormat.PKCS8,
            encryption_algorithm=serialization.NoEncryption()
        ))

    # Сохранение открытого ключа
    with open(public_key_file, "wb") as f:
        f.write(public_key.public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.SubjectPublicKeyInfo
        ))


def hash_file(file_name):
    """Возвращает хеш SHA-256 содержимого файла."""
    with open(file_name, "rb") as f:
        file_data = f.read()
    hasher = hashes.Hash(hashes.SHA256(), backend=default_backend())
    hasher.update(file_data)
    return hasher.finalize()


def sign_file(file_name, signature_file, private_key_file=PRIVATE_KEY_FILE):
    """Подписывает указанный файл и сохраняет подпись в файл."""
    try:
        with open(private_key_file, "rb") as f:
            private_key = serialization.load_pem_private_key(
                f.read(),
                password=None,
                backend=default_backend()
            )

        digest = hash_file(file_name)

        # Вывод хэша данных
        print(f"Хэш содержимого файла '{file_name}': {digest.hex()}")

        signature = private_key.sign(
            digest,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )

        with open(signature_file, "wb") as f:
            f.write(signature)

        print(f"Файл '{file_name}' успешно подписан. Подпись сохранена в '{signature_file}'.")

    except Exception as e:
        print(f"Ошибка при подписании файла: {e}")


def verify_signature(file_name, signature_file, public_key_file=PUBLIC_KEY_FILE):
    """Проверяет подпись указанного файла с использованием открытого ключа."""
    try:
        # Проверка содержимого файла
        with open(file_name, "r") as f:
            file_content = f.read().strip()

        if file_content != VALUE:
            print("Подпись недействительна: содержимое файла не соответствует ожидаемому.")
            return

        with open(public_key_file, "rb") as f:
            public_key = serialization.load_pem_public_key(
                f.read(),
                backend=default_backend()
            )

        digest = hash_file(file_name)

        # Вывод хэша данных
        print(f"Хэш содержимого файла '{file_name}' для проверки: {digest.hex()}")

        with open(signature_file, "rb") as f:
            signature = f.read()

        public_key.verify(
            signature,
            digest,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )

        print("Подпись действительна.")

    except Exception as e:
        print(f"Ошибка проверки подписи: {e}")


if __name__ == "__main__":
    # Генерация ключей
    if not os.path.exists(PUBLIC_KEY_FILE) or not os.path.exists(PRIVATE_KEY_FILE):
        generate_static_keys()

    file_name = "example.txt"
    signature_file = "signature.txt"

    # Создание тестового файла
    if not os.path.exists(file_name):
        with open(file_name, "w") as f:
            f.write(VALUE)

    # Подпись файла
    sign_file(file_name, signature_file)

    # Проверка подписи
    verify_signature(file_name, signature_file)
