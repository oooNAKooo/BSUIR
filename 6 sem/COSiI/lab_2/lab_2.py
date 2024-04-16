import numpy as np
import matplotlib.pyplot as plt

# Повторение процесса с использованием библиотеки
from scipy.signal import fftconvolve


# Определение функций
def y(x):
    return np.cos(3 * x)

def z(x):
    return np.sin(2 * x)

# Генерация значений x
x = np.linspace(0, 2 * np.pi, 100)

# Вычисление функций
y_values = y(x)
z_values = z(x)

# Построение графиков
plt.figure(figsize=(10, 8))

# Графики функций, рассчитанных вручную
plt.subplot(3, 3, 1)
plt.plot(x, y_values, label='y=cos(2x)')
plt.legend()
plt.title('График функции y=cos(2x)')

plt.subplot(3, 3, 2)
plt.plot(x, z_values, label='z=sin(5x)')
plt.legend()
plt.title('График функции z=sin(5x)')

# Циклическая свертка
convolution_cyclic = np.convolve(y_values, z_values, mode='same') / len(x)
plt.subplot(3, 3, 3)
plt.plot(x, convolution_cyclic, label='Циклическая свертка')
plt.legend()
plt.title('График циклической свертки')

# Линейная свертка
convolution_linear = np.convolve(y_values, z_values, mode='full') / len(x)
plt.subplot(3, 3, 4)
plt.plot(convolution_linear, label='Линейная свертка')
plt.legend()
plt.title('График линейной свертки')

# Корреляция
correlation = np.correlate(y_values, z_values, mode='full') / len(x)
plt.subplot(3, 3, 5)
plt.plot(correlation, label='Корреляция')
plt.legend()
plt.title('График корреляции')

# Преобразование Фурье
y_fft = np.fft.fft(y_values)
z_fft = np.fft.fft(z_values)
product = y_fft * z_fft
inverse_fft = np.fft.ifft(product)
plt.subplot(3, 3, 6)
plt.plot(inverse_fft, label='Обратное преобразование Фурье')
plt.legend()
plt.title('График обратного преобразования Фурье')


# Циклическая свертка с использованием библиотеки
convolution_cyclic_lib = fftconvolve(y_values, z_values, mode='same') / len(x)
plt.subplot(3, 3, 7)
plt.plot(x, convolution_cyclic_lib, label='Циклическая свертка (библиотека)')
plt.legend()
plt.title('График циклической свертки (библиотека)')

# Линейная свертка с использованием библиотеки
convolution_linear_lib = fftconvolve(y_values, z_values, mode='full') / len(x)
plt.subplot(3, 3, 8)
plt.plot(convolution_linear_lib, label='Линейная свертка (библиотека)')
plt.legend()
plt.title('График линейной свертки (библиотека)')

# Корреляция с использованием библиотеки
correlation_lib = np.correlate(y_values, z_values, mode='full') / len(x)
plt.subplot(3, 3, 9)
plt.plot(correlation_lib, label='Корреляция (библиотека)')
plt.legend()
plt.title('График корреляции (библиотека)')

plt.tight_layout()
plt.show()
