#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <cuda_runtime.h>
#include <thrust/device_vector.h>
#include <thrust/sort.h>

const int arraySize = 1000;  // Размер массива

// Функция для генерации случайных чисел
void generateRandomArray(float* array, int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1000.0);  // Диапазон чисел от 0 до 1000

    for (int i = 0; i < size; ++i) {
        array[i] = dis(gen);  // Генерация случайного числа
    }
}

// Сортировка на CPU
void sortArrayCPU(float* array, int size) {
    std::sort(array, array + size);  // Стандартная сортировка массива
}

// Функция для проверки результата сортировки
void printFirst10(float* array) {
    std::cout << "First 10 sorted numbers:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

// CUDA сортировка (на GPU без Thrust)
__global__ void sortArrayGPU(float* array, int size) {
    extern __shared__ float sharedArray[];

    int index = blockIdx.x * blockDim.x + threadIdx.x;

    if (index < size) {
        sharedArray[threadIdx.x] = array[index];
        __syncthreads();

        for (int step = 1; step < blockDim.x; step *= 2) {
            if (threadIdx.x % (2 * step) == 0) {
                if (threadIdx.x + step < blockDim.x) {
                    if (sharedArray[threadIdx.x] > sharedArray[threadIdx.x + step]) {
                        float temp = sharedArray[threadIdx.x];
                        sharedArray[threadIdx.x] = sharedArray[threadIdx.x + step];
                        sharedArray[threadIdx.x + step] = temp;
                    }
                }
            }
            __syncthreads();
        }

        array[index] = sharedArray[threadIdx.x];
    }
}

int main() {
    // Создание массива на CPU
    float* h_array = new float[arraySize];
    generateRandomArray(h_array, arraySize);  // Генерация случайных чисел на CPU

    // Сортировка на CPU
    auto start = std::chrono::high_resolution_clock::now();
    sortArrayCPU(h_array, arraySize);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cpu_duration = end - start;  // Время в миллисекундах
    std::cout << "CPU sort time: " << cpu_duration.count() << " ms" << std::endl;

    // Печать первых 10 чисел после сортировки на CPU
    printFirst10(h_array);

    // Сортировка на GPU с использованием Thrust
    thrust::device_vector<float> d_array(arraySize);
    thrust::copy(h_array, h_array + arraySize, d_array.begin());  // Копирование данных в память GPU

    start = std::chrono::high_resolution_clock::now();
    thrust::sort(d_array.begin(), d_array.end());  // Сортировка на GPU с использованием Thrust
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> gpu_thrust_duration = end - start;  // Время в миллисекундах
    std::cout << "GPU Thrust sort time: " << gpu_thrust_duration.count() << " ms" << std::endl;

    // Копирование отсортированных данных обратно на CPU
    thrust::copy(d_array.begin(), d_array.end(), h_array);

    // Печать первых 10 чисел после сортировки на GPU (Thrust)
    printFirst10(h_array);

    // Сортировка на GPU без использования Thrust
    float* d_array_gpu;
    cudaMalloc(&d_array_gpu, arraySize * sizeof(float));
    cudaMemcpy(d_array_gpu, h_array, arraySize * sizeof(float), cudaMemcpyHostToDevice);  // Копирование данных в память GPU

    start = std::chrono::high_resolution_clock::now();
    sortArrayGPU<<<(arraySize + 255) / 256, 256, 256 * sizeof(float)>>>(d_array_gpu, arraySize);  // Запуск сортировки на GPU
    cudaDeviceSynchronize();  // Ожидание завершения вычислений
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> gpu_manual_duration = end - start;  // Время в миллисекундах
    std::cout << "GPU (manual) sort time: " << gpu_manual_duration.count() << " ms" << std::endl;

    // Копирование отсортированных данных обратно на CPU
    cudaMemcpy(h_array, d_array_gpu, arraySize * sizeof(float), cudaMemcpyDeviceToHost);

    // Печать первых 10 чисел после сортировки на GPU (без Thrust)
    printFirst10(h_array);

    // Освобождение памяти
    delete[] h_array;
    cudaFree(d_array_gpu);

    return 0;
}
