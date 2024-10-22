#include <iostream>
#include <cuda_runtime.h> //cuda api
#include <device_launch_parameters.h> // параметры ядра КУДА
#include <cstdlib> // рандом
#include <ctime> // генератор рандома
#include <chrono>  // Для замеров времени на CPU

#define BLOCK_SIZE 32 // размер блока (каждый блок содержит 32 потока, кот вып парал опер)

// Функция для проверки ошибок CUDA
void checkCudaError(cudaError_t result, const char* msg) {
    if (result != cudaSuccess) {
        std::cerr << "CUDA Error: " << msg << " - " << cudaGetErrorString(result) << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Инициализация матрицы случайными числами (0-99)
void initializeMatrix(int* matrix, int M, int N) {
    for (int i = 0; i < M * N; i++) {
        matrix[i] = rand() % 100;
    }
}

// CPU-реализация преобразования матрицы MxN с блоками 2x2 -> 1x4 (прямой порядок)
void transformCPU(const int* input, int* output, int M, int N) {
    int blockIdx = 0;  // Индекс блока 1x4 в выходной матрице
    for (int i = 0; i < M; i += 2) {
        for (int j = 0; j < N; j += 2) {
            // Копируем блок 2x2 в блок 1x4 (прямой порядок)
            output[blockIdx] = input[i * N + j];        // 1
            output[blockIdx + 1] = input[i * N + j + 1]; // 2
            output[blockIdx + 2] = input[(i + 1) * N + j]; // 3
            output[blockIdx + 3] = input[(i + 1) * N + j + 1]; // 4
            blockIdx += 4;  // Переходим к следующему блоку 1x4
        }
    }
}

// GPU-ядро для преобразования блоков 2x2 -> 1x4 (прямой порядок)
__global__ void transformGPU(const int* input, int* output, int M, int N) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    int totalBlocks = (M / 2) * (N / 2);  // Общее количество блоков 1x4

    // Проверка выхода за пределы массива
    if (idx >= totalBlocks) return;

    int i = (idx / (N / 2)) * 2;  // Нахождение координат исходного блока в матрице
    int j = (idx % (N / 2)) * 2;

    // Преобразование 2x2 в 1x4
    output[idx * 4] = input[i * N + j];        // 1
    output[idx * 4 + 1] = input[i * N + j + 1]; // 2
    output[idx * 4 + 2] = input[(i + 1) * N + j]; // 3
    output[idx * 4 + 3] = input[(i + 1) * N + j + 1]; // 4
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Приведение к unsigned int

    // Размеры исходной матрицы
    const int M = 2048;  // Количество строк
    const int N = 4096;  // Количество столбцов

    // Размеры выходной матрицы
    const int outputM = M / 2;
    const int outputN = (N / 2) * 4;

    // Выделение памяти динамически на CPU
    int* inputCPU = new int[M * N];
    int* outputCPU = new int[outputM * outputN];
    int* outputGPUCPU = new int[outputM * outputN]; // Массив для результата GPU
    int* inputGPU, * outputGPU;

    // Инициализация входной матрицы
    initializeMatrix(inputCPU, M, N);

    // Проверка доступной памяти на GPU
    size_t freeMem = 0, totalMem = 0;
    cudaMemGetInfo(&freeMem, &totalMem);
    std::cout << "Free GPU Memory: " << freeMem / (1024 * 1024) << " MB" << std::endl;
    std::cout << "Total GPU Memory: " << totalMem / (1024 * 1024) << " MB" << std::endl;

    // Замер времени выполнения на CPU
    auto startCPU = std::chrono::high_resolution_clock::now();
    transformCPU(inputCPU, outputCPU, M, N);
    auto endCPU = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationCPU = endCPU - startCPU;

    // Вывод времени выполнения CPU в миллисекундах
    std::cout << "Time CPU: " << durationCPU.count() << " ms" << std::endl;

    // Выделение памяти на GPU
    checkCudaError(cudaMalloc(&inputGPU, M * N * sizeof(int)), "Allocating inputGPU");
    checkCudaError(cudaMalloc(&outputGPU, outputM * outputN * sizeof(int)), "Allocating outputGPU");

    // Копирование данных на GPU (не учитываем в замере времени вычислений)
    checkCudaError(cudaMemcpy(inputGPU, inputCPU, M * N * sizeof(int), cudaMemcpyHostToDevice), "Copying to inputGPU");

    // Замер времени выполнения ядра на GPU
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    // Запуск ядра на GPU
    dim3 block(BLOCK_SIZE);
    dim3 grid((M * N / 4 + BLOCK_SIZE - 1) / BLOCK_SIZE);
    transformGPU << <grid, block >> > (inputGPU, outputGPU, M, N);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    // Измерение времени выполнения ядра (без учета копирования)
    float millisecondsGPU = 0;
    cudaEventElapsedTime(&millisecondsGPU, start, stop);

    // Копирование результата обратно на CPU (после замера времени)
    checkCudaError(cudaMemcpy(outputGPUCPU, outputGPU, outputM * outputN * sizeof(int), cudaMemcpyDeviceToHost), "Copying from outputGPU");

    // Вывод времени выполнения GPU (без времени на передачу данных)
    std::cout << "Time GPU (without memory transfer): " << millisecondsGPU << " ms" << std::endl;

    // Сравнение результатов CPU и GPU
    bool isEqual = true;
    for (int i = 0; i < outputM * outputN; i++) {
        if (outputCPU[i] != outputGPUCPU[i]) {
            isEqual = false;
            break;
        }
    }

    if (isEqual) {
        std::cout << "Results match!" << std::endl;
    }
    else {
        std::cout << "Results do not match!" << std::endl;
    }

    // Освобождение памяти
    cudaFree(inputGPU);
    cudaFree(outputGPU);
    delete[] inputCPU;
    delete[] outputCPU;
    delete[] outputGPUCPU;

    return 0;
}
