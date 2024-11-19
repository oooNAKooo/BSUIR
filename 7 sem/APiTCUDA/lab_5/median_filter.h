#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#include <cuda_runtime.h>

// Загрузка и сохранение изображений
bool loadImageJPG(const char* filename, unsigned char** data, unsigned int* width, unsigned int* height);
bool saveImageJPG(const char* filename, unsigned char* data, unsigned int width, unsigned int height);

// Реализация медианного фильтра
void medianFilterCPU(const unsigned char* input, unsigned char* output, int width, int height);
void medianFilterGPU_NoShared(const unsigned char* input, unsigned char* output, int width, int height);
void medianFilterGPU_Shared(const unsigned char* input, unsigned char* output, int width, int height);

// Проверка ошибок CUDA
void checkCudaErrors(cudaError_t err);

#endif // MEDIAN_FILTER_H
