import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
from scipy.fft import fft


def signall(x):
    return np.sin(x) + np.cos(4*x)
def filter_response(f):
    # Задаем частоту среза (где фильтр отклоняет сигнал)
    fc = 1  # Пример частоты среза

    # Вычисляем амплитуду передачи фильтра
    H = 1 / (1 + (f / fc)**2)

    return H

x = np.linspace(0, 10, 100)
y_signal = np.sin(x) + np.cos(4 * x)

# Преобразование Фурье исходного сигнала
y_fft = fft(y_signal)
# Получение амплитудного спектра
amplitude_spectrum = np.abs(y_fft)
# Частоты для графика
sampling_rate = 1 / (x[1] - x[0])
frequencies = np.fft.fftfreq(len(x), d=1/sampling_rate)


# Добавляем шумовую помеху (например, гауссовский белый шум)
noise = np.random.normal(0, 0.1, len(x))
y_noisy = y_signal + noise

y_fft_noise = fft(y_noisy)
# Получение амплитудного спектра
amplitude_spectrum_noise = np.abs(y_fft_noise)
# Частоты для графика
# sampling_rate = 1 / (x[1] - x[0])
frequencies_noise = np.fft.fftfreq(len(x), d=1/sampling_rate)


frequencies_filter = np.linspace(0, 10, 100)
filter_responses = filter_response(frequencies_filter)

filtered_signal = signall(frequencies_filter) * filter_responses


# Применяем КИХ фильтр с Режекторным окном Хэмминга
order = 51  # Порядок фильтра
cutoff_freq = 0.1  # Частота среза
nyquist = 0.5

fir_coeffs = signal.firwin(order, cutoff_freq, window='hamming', nyq = nyquist)  # Создаем фильтр
y_filtered_hamming = signal.lfilter(fir_coeffs, 1.0, y_noisy)  # Применяем фильтр

# Построение графиков
plt.figure(figsize=(15, 20))
plt.subplot(6, 1, 6)
plt.plot(frequencies_filter, filter_responses, label="АЧХ фильтра")
plt.plot(frequencies_noise, filtered_signal, label="Примененный фильтр")
plt.xlabel('Частота (Гц)')
plt.ylabel('Амплитуда')
plt.grid(True)
plt.legend()

plt.subplot(6,1,5)
plt.plot(frequencies_noise, amplitude_spectrum_noise, label='АЧХ шумного сигнала')

plt.xlabel('Частота (Гц)')
plt.ylabel('Амплитуда')
plt.grid(True)
plt.legend()

plt.subplot(6,1,4)
plt.plot(frequencies, amplitude_spectrum, label='АЧХ исходного сигнала')
plt.xlabel('Частота (Гц)')
plt.ylabel('Амплитуда')
plt.grid(True)
plt.legend()

plt.subplot(6, 1, 1)
plt.plot(x, y_signal, 'black', label='Исходный сигнал')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Сглаживание сигнала с помощью КИХ фильтра')
plt.legend()
plt.grid(True)
plt.subplot(6, 1, 2)
plt.plot(x, y_noisy, label='Сигнал с шумом')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.subplot(6, 1, 3)
plt.plot(x, y_filtered_hamming,'g--', label='Сглаженный сигнал (КИХ). Окно Хэмминга')
plt.xlabel('x')
plt.ylabel('y')

plt.legend()
plt.grid(True)
plt.show()

# Применяем БИХ фильтр с Режекторным окном Хэмминга
b, a = signal.butter(4, cutoff_freq, btype='low', analog=False)
zi = signal.lfilter_zi(b, a)
y_filtered_biquad_hamming, _ = signal.lfilter(b, a, y_noisy, zi=zi)

# b, a = signal.iirfilter(order, [0.02, 0.1], btype='band', ftype='butter', output='ba', fs=1000)  # Создаем фильтр
# y_filtered_biquad_hamming = signal.lfilter(b, a, y_noisy)  # Применяем фильтр

# Построение графиков
plt.figure(figsize=(10, 8))
plt.subplot(3, 1, 1)
plt.plot(x, y_signal,'black', label='Исходный сигнал')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.title('Сглаживание сигнала с помощью БИХ фильтра')
plt.subplot(3, 1, 2)
plt.plot(x, y_noisy, label='Сигнал с шумом')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.subplot(3, 1, 3)
plt.plot(x, y_filtered_biquad_hamming,'g--', label='Сглаженный сигнал (БИХ). Режекторный фильтр ')
plt.xlabel('x')
plt.ylabel('y')

plt.legend()
plt.grid(True)
plt.show()
