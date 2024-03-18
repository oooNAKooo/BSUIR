from cmath import * # Импорт всех функций для работы с компл числами
import matplotlib.pyplot as plt # Создание графиков и диаграмм
import numpy as np

def inner_function(x):
    return np.cos(3 * x) + np.sin(2 * x) # Возврат сумму исходной ф-ции


def discretize(n):
    return [inner_function(i * 2 * np.pi / n) for i in range(n)] #


def dft(x):
    n = len(x) # Длина списка
    c = [0.0 + 0.0j] * n # Список компл чисел
    for k in range(n):
        for m in range(n):
            c[k] += x[m] * exp(-2j * np.pi * k * m / n) # Вычисление ДПФ (дискретка)
    return c


def idft(x):
    n = len(x) # Длина списка
    c = [0.0 + 0.0j] * n # Список компл чисел
    for k in range(n):
        for m in range(n):
            c[k] += x[m] * exp(2j * np.pi * k * m / n) # Вычисление ОПФ (обратки)
        c[k] *= 1 / n  # компенсация масштабирования - сигналы мб изменены
    return c


def fft(x):
    n = len(x) # Длина списка
    r = n // 2 # Точка разделения сигнала
    out = [0. + 0.0j] * n # Список компл чисел (все числа = 0) - чтобы было все корректно

    for i in range(r): # Делаем БПФ
        out[i] = x[i] + x[i + r]
        out[i + r] = (x[i] - x[i + r]) * exp(-2j * np.pi * i / n)

    if n > 2: # Рекурсия
        top_list = fft(out[:r]) # для верха
        bot_list = fft(out[r:]) # для низа

        for i in range(r): # Заполнения резов после рекурсии
            out[i * 2] = top_list[i]
            out[(i + 1) * 2 - 1] = bot_list[i]

    return out # Резы БПФ


def main():
    n = 8
    samples = discretize(n)

    # Преобразование Фурье (FFT)
    spectrum = fft(samples)
    magnitudes = np.abs(spectrum)
    phases = np.angle(spectrum)

    # Обратное преобразование Фурье (IFFT)
    inv_transform = np.real(idft(spectrum))

    # Создание первого окна с графиками из первого столбца
    fig1, axs1 = plt.subplots(5, 1, figsize=(10, 20))

    # Графики из первого столбца
    x_calc = np.linspace(0, 2 * np.pi, n, endpoint=False)
    y_calc = inner_function(x_calc)
    axs1[0].plot(x_calc, y_calc)
    axs1[0].set_title('График функции')

    real_part = [x.real for x in spectrum]
    axs1[1].stem(real_part)
    axs1[1].set_title('Действительная часть спектра')

    imaginary_part = [x.imag for x in spectrum]
    axs1[2].stem(imaginary_part)
    axs1[2].set_title('Мнимая часть спектра')

    axs1[3].plot(np.arange(n), np.sqrt(magnitudes), 'ro')  # Красные круглые маркеры
    axs1[3].set_title('Квадратный корень амплитуды спектра')

    # Добавление линии, параллельной оси X и проходящей через начало координат (0, 0)
    axs1[3].axhline(0, color='black', linestyle='--')

    plt.tight_layout()
    plt.show(block=False)

    axs1[4].plot(np.linspace(0, 2 * np.pi, n, endpoint=False), inv_transform, 'o-')
    axs1[4].set_title('Обратное преобразование Фурье (IFFT)')

    plt.tight_layout()
    plt.show(block=False)

    # Создание второго окна с графиками из второго столбца
    fig2, axs2 = plt.subplots(5, 1, figsize=(10, 20))

    # Графики из второго столбца
    x_np = np.linspace(0, 2 * np.pi, n, endpoint=False)
    y_np = inner_function(x_np)
    axs2[0].plot(x_np, y_np)
    axs2[0].set_title('График функции (NumPy)')

    real_part_np = np.real(spectrum)
    axs2[1].stem(real_part_np)
    axs2[1].set_title('Действительная часть спектра')

    imaginary_part_np = np.imag(spectrum)
    axs2[2].stem(imaginary_part_np)
    axs2[2].set_title('Мнимая часть спектра')

    axs2[3].plot(np.arange(n), np.sqrt(magnitudes), 'ro')  # Красные круглые маркеры
    axs2[3].set_title('Квадратный корень амплитуды спектра')

    # Добавление линии, параллельной оси X и проходящей через начало координат (0, 0)
    axs2[3].axhline(0, color='black', linestyle='--')

    inv_transform_np = np.real(np.fft.ifft(spectrum))
    axs2[4].plot(np.linspace(0, 2 * np.pi, n, endpoint=False), inv_transform_np)  # н = От 0 до  2*пи (не включая 2*пи)
    axs2[4].set_title('Обратное преобразование Фурье (IFFT)')

    plt.tight_layout()
    plt.show(block=False)

    plt.show()


if __name__ == "__main__":
    main()
