import sys
import numpy as np
import cv2 as cv
import os

# Функция для изменения размера изображения
def resize_image(image, max_width, max_height):
    # присваиваем высоту и ширину
    h, w = image.shape[:2]
    aspect_ratio = w / h
    if w > max_width or h > max_height:
        if aspect_ratio > 1:
            new_width = max_width
            new_height = int(max_width / aspect_ratio)
        else:
            new_height = max_height
            new_width = int(max_height * aspect_ratio)
        return cv.resize(image, (new_width, new_height))
    return image

if __name__ == '__main__':
    fn = '1.jpg'  # Путь к изображению (находится в вашей корневой папке)
    img = cv.imread(fn)

    if img is None:
        print(f"Не удалось загрузить изображение {fn}. Проверьте путь.")
        sys.exit(1)

    # Изменение размера изображения для удобства обработки
    max_width, max_height = 1200, 1000
    img = resize_image(img, max_width, max_height)

    # Преобразуем изображение в HSV для выделения цветов
    hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)

    # Определяем диапазон для зеленого цвета
    lower_green = np.array([35, 40, 40])
    upper_green = np.array([85, 255, 255])

    # Определяем диапазон для синего цвета
    lower_blue = np.array([100, 50, 50])
    upper_blue = np.array([130, 255, 255])

    # Создаем маски для зеленого и синего цветов
    green_mask = cv.inRange(hsv, lower_green, upper_green)
    blue_mask = cv.inRange(hsv, lower_blue, upper_blue)

    # Объединяем маски зеленого и синего цветов
    combined_mask = cv.bitwise_or(green_mask, blue_mask)

    # Находим контуры зеленых и синих объектов
    contours_green, _ = cv.findContours(green_mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    contours_blue, _ = cv.findContours(blue_mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    # Копируем исходное изображение для рисования контуров
    output_image = img.copy()

    # Рисуем зеленые контуры на изображении
    for contour in contours_green:
        if cv.contourArea(contour) > 100:  # Убираем слишком маленькие области
            cv.drawContours(output_image, [contour], -1, (0, 255, 0), 2)  # Зеленый контур

    # Рисуем синие контуры на изображении
    for contour in contours_blue:
        if cv.contourArea(contour) > 100:  # Убираем слишком маленькие области
            cv.drawContours(output_image, [contour], -1, (255, 0, 0), 2)  # Синий контур

    # Размываем все, что не относится к выделенным фигурам
    blurred_img = cv.GaussianBlur(img, (101, 101), 0)  # Чем больше значение, тем сильнее размытие
    # filter Gauss
    result_image = np.where(combined_mask[:, :, None] == 0, blurred_img, output_image)

    # Сохраняем результат
    base_name, _ = os.path.splitext(fn)

    # Показываем результат
    cv.imshow(f'{base_name}_new_filtred', result_image)
    cv.imwrite(f'{base_name}_new_filtred.jpg', result_image)

    cv.waitKey()
    cv.destroyAllWindows()

# BGR - OpenCV standart
