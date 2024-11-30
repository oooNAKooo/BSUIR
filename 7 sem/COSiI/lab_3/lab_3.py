import cv2
import numpy as np
from sklearn.cluster import KMeans


def cart2pol(x, y):
    theta = np.arctan2(y, x)
    rho = np.hypot(x, y)
    return theta, rho


def pol2cart(theta, rho):
    x = rho * np.cos(theta)
    y = rho * np.sin(theta)
    return x, y


def func_to_rotate(contorous):
    result = []
    for cont in contorous:
        [vx, vy, x, y] = cv2.fitLine(cont, cv2.DIST_L2, 0, 0.01, 0.01)

        # Вектор, направленный вдоль оси Y
        y_axis = np.array([0, 1])  # единичный вектор по оси Y
        your_line = np.array([vx, vy])  # вектор линии

        # Нормализуем ваш вектор линии
        your_line_norm = your_line / np.linalg.norm(your_line)

        # Вычисляем скалярное произведение
        dot_product = np.dot(y_axis, your_line_norm)

        # Вычисляем угол в радианах
        angle_rad = np.arccos(dot_product)

        # Преобразуем угол в градусы
        angle_degree = np.degrees(angle_rad)
        # переносим в начало координат фигуру
        if angle_degree >= 150:
            angle_degree = 0

        M = cv2.moments(cont)
        if M['m00'] == 0:
            M['m00'] = 1

        cx = int(M['m10'] / M['m00'])
        cy = int(M['m01'] / M['m00'])

        cnt_norm = cont - [cx, cy]

        coordinates = cnt_norm[:, 0, :]
        xs, ys = coordinates[:, 0], coordinates[:, 1]
        thetas, rhos = cart2pol(xs, ys)

        thetas = np.rad2deg(thetas)
        thetas = (thetas + angle_degree) % 360
        thetas = np.deg2rad(thetas)

        xs, ys = pol2cart(thetas, rhos)

        cnt_norm[:, 0, 0] = xs
        cnt_norm[:, 0, 1] = ys

        cnt_rotated = cnt_norm + [cx, cy]
        cnt_rotated = cnt_rotated.astype(np.int32)
        result.append(cnt_rotated)

    return result


if __name__ == "__main__":
    picture = cv2.imread("clear.jpg")
    image = cv2.GaussianBlur(picture, (15, 15), 0)
    temp = cv2.GaussianBlur(image, (15, 15), 0)
    new_image = cv2.GaussianBlur(temp, (15, 15), 0)
    high_sym = np.array([220, 255, 255], np.uint8())
    low_sym = np.array([150, 160, 0], np.uint8())
    only_sym_hsv = cv2.inRange(new_image, low_sym, high_sym)
    cv2.imwrite("work_3.jpg", only_sym_hsv)

    contorous, _ = cv2.findContours(only_sym_hsv, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    print(len(contorous))

    cont_rotated = func_to_rotate(contorous)

    # Создаем черный фон такого же размера, как исходное изображение
    black_background = np.zeros_like(picture)

    # Рисуем зеленые контуры на черном фоне
    cv2.drawContours(black_background, cont_rotated, -1, (0, 255, 0), 4)

    # Сохраняем результат
    cv2.imwrite("working.jpg", black_background)
