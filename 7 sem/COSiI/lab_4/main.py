import cv2
import numpy as np
import tensorflow as tf
import os

def remove_background(image, lower_bound, upper_bound):
    """Удаление фона с использованием пороговых значений в HSV."""
    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv_image, lower_bound, upper_bound)

    # Морфологическая обработка маски для удаления шумов
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

    # Удаление фона
    no_background = cv2.bitwise_and(image, image, mask=mask)
    return no_background, mask

def rotate_and_extract_digits(no_background, mask):
    """Выделение и выравнивание цифр на основе контуров."""
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Фильтрация контуров по минимальной площади
    filtered_contours = [contour for contour in contours if cv2.contourArea(contour) > 0]

    if not filtered_contours:
        raise ValueError("Контуры не найдены. Проверьте параметры фильтрации или изображение.")

    digit_images = []
    areas = []

    for contour in filtered_contours:
        # Вычисление площади контура
        area = cv2.contourArea(contour)
        areas.append(area)

        # Минимальный прямоугольник, описывающий контур
        min_rect = cv2.minAreaRect(contour)
        angle = min_rect[2]
        size = min_rect[1]

        # Поворот, если необходимо
        if size[0] > size[1]:
            angle += 90
            size = (size[1], size[0])

        # Матрица поворота и выравнивание изображения
        rotation_matrix = cv2.getRotationMatrix2D(min_rect[0], angle, 1.0)
        rotated_image = cv2.warpAffine(no_background, rotation_matrix, (no_background.shape[1], no_background.shape[0]))

        # Вычисление координат вырезки
        center = tuple(map(int, min_rect[0]))
        width, height = map(int, size)
        x_start, y_start = center[0] - width // 2, center[1] - height // 2

        # Обрезка цифры
        digit_image = rotated_image[
            max(0, y_start): max(0, y_start + height),
            max(0, x_start): max(0, x_start + width)
        ]

        if digit_image.size > 0:
            # Добавление паддинга
            padded_digit_image = cv2.copyMakeBorder(
                digit_image, 40, 40, 40, 40, borderType=cv2.BORDER_CONSTANT, value=(0, 0, 0)
            )
            digit_images.append(padded_digit_image)

    return digit_images, areas

def predict_digits(digit_images, model):
    """Распознавание цифр с использованием модели TensorFlow."""
    predictions = []

    for i, digit in enumerate(digit_images):
        # Подготовка изображения для модели
        resized_digit = cv2.resize(digit, (28, 28))
        gray_digit = cv2.cvtColor(resized_digit, cv2.COLOR_BGR2GRAY)
        normalized_digit = gray_digit / 255.0
        digit_input = np.expand_dims(normalized_digit, axis=(0, -1))

        # Предсказание цифры
        prediction = model.predict(digit_input)
        predicted_digit = np.argmax(prediction)
        predictions.append(predicted_digit)

        # Сохранение изображения цифры с именем распознанной цифры
        output_path = f"digit_{predicted_digit}.png"  # Сохранение под именем распознанной цифры
        cv2.imwrite(output_path, digit)
        print(f"Сохранено изображение цифры: {output_path}")

    return predictions

def load_model(model_path):
    """Загрузка обученной модели TensorFlow."""
    return tf.keras.models.load_model(model_path)

def process_image(image_path, model):
    """Обработка изображения: удаление фона, выравнивание и распознавание цифр."""
    src_image = cv2.imread(image_path)

    if src_image is None:
        raise ValueError("Ошибка загрузки изображения. Проверьте путь к файлу.")

    # Параметры для выделения зеленого цвета
    lower_bound = np.array([40, 140, 140])
    upper_bound = np.array([90, 255, 255])

    # Удаление фона
    no_background, mask = remove_background(src_image, lower_bound, upper_bound)

    # Выделение и выравнивание цифр
    digit_images, areas = rotate_and_extract_digits(no_background, mask)

    # Распознавание цифр
    predicted_digits = predict_digits(digit_images, model)

    # Вывод результатов
    for digit, area in zip(predicted_digits, areas):
        print(f"Распознанная цифра: {digit}, Площадь(пиксели): {area:.2f}")

if __name__ == "__main__":
    input_image = "1.jpg"  # Входное изображение
    model_path = "model1.h5"  # Модель TensorFlow
    try:
        # Загрузка модели и обработка изображения
        model = load_model(model_path)
        process_image(input_image, model)
    except ValueError as e:
        print(f"Ошибка: {e}")
