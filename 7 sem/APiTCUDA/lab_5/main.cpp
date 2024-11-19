#include <iostream>
#include "median_filter.h"

int main() {
    unsigned char* input = nullptr;
    unsigned char* outputCPU = nullptr;
    unsigned char* outputGPU = nullptr;
    unsigned int width, height;

    if (!loadImageJPG("input.jpg", &input, &width, &height)) {
        return -1;
    }

    outputCPU = new unsigned char[width * height];
    outputGPU = new unsigned char[width * height];

    // CPU фильтрация
    medianFilterCPU(input, outputCPU, width, height);
    saveImageJPG("output_cpu.jpg", outputCPU, width, height);

    // GPU фильтрация (без shared memory)
    medianFilterGPU_NoShared(input, outputGPU, width, height);
    saveImageJPG("output_gpu_no_shared.jpg", outputGPU, width, height);

    delete[] input;
    delete[] outputCPU;
    delete[] outputGPU;

    return 0;
}
