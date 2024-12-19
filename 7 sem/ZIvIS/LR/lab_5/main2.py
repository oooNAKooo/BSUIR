from Crypto.Hash import HMAC, SHA256

def hmac_with_cryptoapi(message, secret):
    # Создание объекта HMAC с использованием SHA256
    hmac_obj = HMAC.new(secret.encode('utf-8'), msg=message.encode('utf-8'), digestmod=SHA256)

    # Получение хеша
    return hmac_obj.hexdigest()

message = input("Введите сообщение: ")
secret = input("Введите секретный ключ: ")
hmac_value = hmac_with_cryptoapi(message, secret)
print("Сообщение: " + message + "\nПосле хеширования: ", hmac_value)
