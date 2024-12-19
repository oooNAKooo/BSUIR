from Crypto.Hash import SHA256
def hmac_sha256(message, secret):

    # Конкатенация
    message_bytes = message.encode('utf-8')
    secret_bytes = secret.encode('utf-8')
    combined = message_bytes + secret_bytes

    # Вычисление хеша
    hash_obj = SHA256.new(combined)
    return hash_obj.hexdigest()

message = input("Введите сообщение: ")
secret = input("Введите секретный ключ: ")

# Вычисление HMAC
hmac_value = hmac_sha256(message, secret)
print("Сообщение: " + message + "\nПосле хеширования: ", hmac_value)
