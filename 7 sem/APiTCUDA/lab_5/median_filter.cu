#include "median_filter.h"
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cstdio>

// Загрузка изображения JPG
bool loadImageJPG(const char* filename, unsigned char** data, unsigned int* width, unsigned int* height) {
    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        fprintf(stderr, "Failed to load image: %s\n", filename);
        return false;
    }
    *width = image.cols;
    *height = image.rows;
    *data = new unsigned char[(*width) * (*height)];
    memcpy(*data, image.data, (*width) * (*height) * sizeof(unsigned char));
    printf("Image loaded: %s (%u x %u)\n", filename, *width, *height);
    return true;
}

// Сохранение изображения JPG
bool saveImageJPG(const char* filename, unsigned char* data, unsigned int width, unsigned int height) {
    cv::Mat image(height, width, CV_8UC1, data);
    if (!cv::imwrite(filename, image)) {
        fprintf(stderr, "Failed to save image: %s\n", filename);
        return false;
    }
    printf("Image saved: %s\n", filename);
    return true;
}

// Реализация медианного фильтра на CPU
void medianFilterCPU(const unsigned char* input, unsigned char* output, int width, int height) {
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            unsigned char window[9];
            int idx = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    window[idx++] = input[(y + dy) * width + (x + dx)];
                }
            }
            std::sort(window, window + 9);
            output[y * width + x] = window[4];
        }
    }
}

// Проверка ошибок CUDA
void checkCudaErrors(cudaError_t err) {
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
}

// Реализация медианного фильтра на GPU: без разделяемой памяти
__global__ void medianFilterKernel_NoShared(const unsigned char* input, unsigned char* output, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= 1 && x < width - 1 && y >= 1 && y < height - 1) {
        unsigned char window[9];
        int idx = 0;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                window[idx++] = input[(y + dy) * width + (x + dx)];
            }
        }
        for (int i = 0; i < 5; ++i) {
            for (int j = i + 1; j < 9; ++j) {
                if (window[i] > window[j]) {
                    unsigned char temp = window[i];
                    window[i] = window[j];
                    window[j] = temp;
                }
            }
        }
        output[y * width + x] = window[4];
    }
}

void medianFilterGPU_NoShared(const unsigned char* input, unsigned char* output, int width, int height) {
    unsigned char *d_input, *d_output;

    size_t size = width * height * sizeof(unsigned char);
    checkCudaErrors(cudaMalloc(&d_input, size));
    checkCudaErrors(cudaMalloc(&d_output, size));
    checkCudaErrors(cudaMemcpy(d_input, input, size, cudaMemcpyHostToDevice));

    dim3 threads(16, 16);
    dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);

    medianFilterKernel_NoShared<<<blocks, threads>>>(d_input, d_output, width, height);
    checkCudaErrors(cudaDeviceSynchronize());

    checkCudaErrors(cudaMemcpy(output, d_output, size, cudaMemcpyDeviceToHost));
    cudaFree(d_input);
    cudaFree(d_output);
}

// Реализация медианного фильтра на GPU: с разделяемой памятью
// Реализация аналогична, но с использованием __shared__ памяти.
