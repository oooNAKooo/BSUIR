import cv2 as cv
import numpy as np


# Функция для изменения размера изображения
def resize_image(image, max_width, max_height):
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


# Функция для обработки и описания фигур на изображении
def analyze_and_describe_shapes(image_path):
    # Загрузка изображения
    img = cv.imread(image_path)
    if img is None:
        print(f"Не удалось загрузить изображение {image_path}")
        return

    # Изменение размера изображения для удобства обработки
    max_width, max_height = 1200, 1000
    img = resize_image(img, max_width, max_height)

    # Преобразование изображения в цветовое пространство HSV
    hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)

    # Определение цветового диапазона для синих фигур
    lower_blue = np.array([100, 150, 50])
    upper_blue = np.array([140, 255, 255])
    mask = cv.inRange(hsv, lower_blue, upper_blue)

    # Применение эрозии и размыкания для разъединения фигур
    kernel = np.ones((3, 3), np.uint8)
    eroded = cv.erode(mask, kernel, iterations=2)
    opened = cv.morphologyEx(eroded, cv.MORPH_OPEN, kernel)
    dilated = cv.dilate(opened, kernel, iterations=3)

    # Поиск контуров
    contours, _ = cv.findContours(dilated, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    # Установка порога площади для крупных фигур
    min_area_threshold = 3000
    img_contours = img.copy()
    shape_descriptions = []

    for cnt in contours:
        area = cv.contourArea(cnt)
        if area > min_area_threshold:  # Фильтрация только крупных объектов
            # Определение формы фигуры
            approx = cv.approxPolyDP(cnt, 0.04 * cv.arcLength(cnt, True), True)
            if len(approx) == 4:
                shape = "Квадрат/Прямоугольник" if cv.isContourConvex(approx) else "Quadrilateral"
            elif len(approx) == 6:
                shape = "Шестиугольник"
            elif 7 <= len(approx) <= 5000:
                shape = "Многоугольник"
            else:
                shape = "Круг/Эллипс"  # Пример для округлых объектов

            # Найти центр контура (центроид)
            M = cv.moments(cnt)
            if M["m00"] != 0:
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])
            else:
                cx, cy = 0, 0

            # Добавление информации о фигуре
            shape_descriptions.append({
                "Shape": shape,
                "Area": area,
                "Center": (cx, cy)
            })

            # Отрисовка контуров и заполнение фигуры
            cv.drawContours(img_contours, [cnt], -1, (0, 0, 255), 2)  # Красный контур
            cv.drawContours(img, [cnt], -1, (0, 255, 255), -1)  # Заливка желтым

    # Вывод информации о каждой фигуре
    for i, desc in enumerate(shape_descriptions, start=1):
        print(f"Фигура {i}:")
        print(f"  Форма: {desc['Shape']}")
        print(f"  Площадь: {desc['Area']} пикселей")
        print(f"  Центр: {desc['Center']}")

    # Функция для отслеживания положения мыши
    def show_coordinates(event, x, y, flags, param):
        if event == cv.EVENT_MOUSEMOVE:  # Отслеживание движения мыши
            img_copy = final_result.copy()
            cv.putText(img_copy, f"Coordinates: ({x}, {y})", (10, 30), cv.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
            cv.imshow("Shapes Analyzed with Blurred Background", img_copy)

    # Смешивание фона с фигурами
    img_blurred = cv.GaussianBlur(img, (21, 21), 0)
    mask_colored_shapes = cv.bitwise_and(img, img, mask=dilated)
    mask_background = cv.bitwise_and(img_blurred, img_blurred, mask=cv.bitwise_not(dilated))
    final_result = cv.add(mask_colored_shapes, mask_background)

    # Показ окна с координатами курсора
    cv.namedWindow("Shapes Analyzed with Blurred Background")
    cv.setMouseCallback("Shapes Analyzed with Blurred Background", show_coordinates)

    cv.imshow("Shapes Analyzed with Blurred Background", final_result)
    cv.imwrite("shapes_described_with_blurred_background.jpg", final_result)
    cv.waitKey(0)
    cv.destroyAllWindows()


# Запуск функции с вашим изображением
analyze_and_describe_shapes('3.jpg')
