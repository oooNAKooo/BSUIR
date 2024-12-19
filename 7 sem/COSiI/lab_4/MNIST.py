import tensorflow as tf
from tensorflow.keras import layers, models
from tensorflow.keras.datasets import mnist
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import os

# Загрузка данных
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train = x_train[..., np.newaxis].astype('float32') / 255
x_test = x_test[..., np.newaxis].astype('float32') / 255

# Создание модели
def create_model():
    """Создает и возвращает сверточную нейронную сеть."""
    model = models.Sequential([
        layers.Conv2D(32, (3, 3), activation='relu', input_shape=(28, 28, 1)),
        layers.MaxPooling2D((2, 2)),
        layers.Conv2D(64, (3, 3), activation='relu'),
        layers.MaxPooling2D((2, 2)),
        layers.Flatten(),
        layers.Dense(64, activation='relu'),
        layers.Dense(10, activation='softmax')
    ])
    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])
    return model

# Коллбек для проверки предсказаний
class ManualCheckCallback(tf.keras.callbacks.Callback):
    def __init__(self):
        super().__init__()
        self.new_data = []

    def on_epoch_end(self, epoch, logs=None):
        """Выполняется после каждой эпохи."""
        image_path = input("\nВведите путь к изображению (или оставьте пустым для пропуска): ").strip()
        if not image_path:
            print("Пропуск проверки.")
            return

        if not os.path.exists(image_path):
            print(f"Файл {image_path} не найден. Пропуск проверки.")
            return

        try:
            # Загрузка и предобработка изображения
            img_array = self.preprocess_image(image_path)

            # Предсказание
            prediction = self.model.predict(img_array, verbose=0)
            predicted_digit = np.argmax(prediction)
            confidence = np.max(prediction) * 100

            # Визуализация результата
            self.visualize_prediction(img_array, predicted_digit, confidence)

            # Пользовательская проверка
            self.manual_verification(img_array, predicted_digit)
        except Exception as e:
            print(f"Ошибка: {e}. Пропуск проверки.")

    def preprocess_image(self, image_path):
        """Загружает и обрабатывает изображение для модели."""
        img = Image.open(image_path).convert('L')
        img = img.resize((28, 28))
        img_array = np.array(img).astype('float32') / 255
        return img_array[np.newaxis, ..., np.newaxis]

    def visualize_prediction(self, img_array, predicted_digit, confidence):
        """Отображает изображение и предсказание."""
        plt.imshow(img_array[0, ..., 0], cmap='gray')
        plt.title(f'Предсказание: {predicted_digit} (Вероятность: {confidence:.2f}%)')
        plt.axis('off')
        plt.show()

    def manual_verification(self, img_array, predicted_digit):
        """Ручная проверка и добавление в обучающую выборку."""
        correct_digit = input(f"Введите правильную цифру (0-9) или нажмите Enter, если {predicted_digit} верно: ").strip()
        if correct_digit == "":
            print("Предсказание подтверждено.")
            return

        if correct_digit.isdigit() and 0 <= int(correct_digit) <= 9:
            print("Неправильно. Добавляем изображение в обучающую выборку.")
            self.new_data.append((img_array[0], int(correct_digit)))

            # Переобучение, если накоплено достаточное количество новых данных
            if len(self.new_data) >= 1:  # Порог для переобучения
                self.retrain_model()

    def retrain_model(self):
        """Переобучение модели на новых данных."""
        print("Переобучение модели на новых данных...")
        x_new, y_new = zip(*self.new_data)
        x_new, y_new = np.array(x_new), np.array(y_new)
        self.model.fit(x_new, y_new, epochs=1, verbose=1)
        self.new_data.clear()

# Основной код
if __name__ == "__main__":
    model = create_model()
    manual_check_callback = ManualCheckCallback()

    # Обучение модели
    model.fit(x_train, y_train, epochs=20, callbacks=[manual_check_callback], validation_data=(x_test, y_test))

    # Сохранение модели
    model.save('model1.h5')
    print("Модель успешно сохранена как 'model1.h5'.")
