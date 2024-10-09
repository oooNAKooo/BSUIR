#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <immintrin.h> // Для векторизации AVX
#include <omp.h>       // Для многопоточности OpenMP

using namespace std;

const int L3_CACHE_SIZE = 16 * 1024 * 1024; // 16 МБ L3 кэша
const int MATRIX_SIZE = 2048; // Размер матриц N x N

template<typename T>
void initialize_matrix(vector<vector<T>>& mat) {
    for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat[0].size(); j++) {
            mat[i][j] = rand() % 10;
        }
    }
}

template<typename T>
void matrix_multiplication_standard(const vector<vector<T>>& A, const vector<vector<T>>& B, vector<vector<T>>& C) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            T sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

template<typename T>
void matrix_multiplication_blocked(const vector<vector<T>>& A, const vector<vector<T>>& B, vector<vector<T>>& C) {
    int blockSize = 1024;
    for (int i = 0; i < MATRIX_SIZE; i += blockSize) {
        for (int j = 0; j < MATRIX_SIZE; j += blockSize) {
            for (int k = 0; k < MATRIX_SIZE; k += blockSize) {
                for (int ii = i; ii < min(i + blockSize, MATRIX_SIZE); ii++) {
                    for (int jj = j; jj < min(j + blockSize, MATRIX_SIZE); jj++) {
                        T sum = 0;
                        for (int kk = k; kk < min(k + blockSize, MATRIX_SIZE); kk++) {
                            sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

template<typename T>
void matrix_multiplication_optimized(const vector<vector<T>>& A, const vector<vector<T>>& B, vector<vector<T>>& C) {
    int blockSize = 128;
    for (int i = 0; i < MATRIX_SIZE; i += blockSize) {
        for (int j = 0; j < MATRIX_SIZE; j += blockSize) {
            for (int k = 0; k < MATRIX_SIZE; k += blockSize) {
                for (int ii = i; ii < min(i + blockSize, MATRIX_SIZE); ii++) {
                    for (int jj = j; jj < min(j + blockSize, MATRIX_SIZE); jj++) {
                        T sum = 0;
                        for (int kk = k; kk < min(k + blockSize, MATRIX_SIZE); kk++) {
                            sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

void matrix_multiplication_vectorized(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    // Убедитесь, что C инициализирована нулями
    fill(C.begin(), C.end(), vector<int>(MATRIX_SIZE, 0));

    // Используем блочный размер
    int blockSize = 128; // Размер блока для векторизации
    for (int i = 0; i < MATRIX_SIZE; i += blockSize) {
        for (int j = 0; j < MATRIX_SIZE; j += blockSize) {
            for (int k = 0; k < MATRIX_SIZE; k += blockSize) {
                for (int ii = i; ii < min(i + blockSize, MATRIX_SIZE); ii++) {
                    for (int jj = j; jj < min(j + blockSize, MATRIX_SIZE); jj++) {
                        // Вектор для суммирования
                        __m256i sum_vec = _mm256_setzero_si256(); // Инициализация вектора суммы

                        // Переменная для хранения временной суммы
                        int sum = 0;

                        for (int kk = k; kk < min(k + blockSize, MATRIX_SIZE); kk += 8) {
                            // Загружаем 8 элементов из A и B
                            __m256i a_vec = _mm256_loadu_si256((__m256i*) & A[ii][kk]); // Загрузка 8 элементов из A
                            __m256i b_vec = _mm256_loadu_si256((__m256i*) & B[kk][jj]); // Загрузка 8 элементов из B

                            // Поэлементное умножение
                            __m256i mul_vec = _mm256_mullo_epi32(a_vec, b_vec);

                            // Суммирование результата
                            sum_vec = _mm256_add_epi32(sum_vec, mul_vec);
                        }

                        // Сохраняем результат
                        int sum_arr[8];
                        _mm256_storeu_si256((__m256i*)sum_arr, sum_vec); // Сохранение в массив

                        // Суммируем все элементы вектора
                        for (int z = 0; z < 8; ++z) {
                            sum += sum_arr[z];
                        }

                        // Добавляем итоговую сумму в C
                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }
}



// Реализация с OpenMP (многопоточность)
// Реализация с OpenMP и векторизацией (многопоточность + AVX)
void matrix_multiplication_openmp_vectorized(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int blockSize = 128;

    // Убедитесь, что C инициализирована нулями
    fill(C.begin(), C.end(), vector<int>(MATRIX_SIZE, 0));

#pragma omp parallel for collapse(2)
    for (int i = 0; i < MATRIX_SIZE; i += blockSize) {
        for (int j = 0; j < MATRIX_SIZE; j += blockSize) {
            for (int k = 0; k < MATRIX_SIZE; k += blockSize) {
                for (int ii = i; ii < min(i + blockSize, MATRIX_SIZE); ii++) {
                    for (int jj = j; jj < min(j + blockSize, MATRIX_SIZE); jj++) {
                        __m256i sum_vec = _mm256_setzero_si256(); // Инициализация вектора суммы
                        int sum = 0;

                        for (int kk = k; kk < min(k + blockSize, MATRIX_SIZE); kk += 8) {
                            // Загружаем 8 элементов из A и B
                            __m256i a_vec = _mm256_loadu_si256((__m256i*) & A[ii][kk]);  // Загрузка 8 элементов из A
                            __m256i b_vec = _mm256_loadu_si256((__m256i*) & B[kk][jj]);  // Загрузка 8 элементов из B

                            // Поэлементное умножение
                            __m256i mul_vec = _mm256_mullo_epi32(a_vec, b_vec);

                            // Суммирование результата
                            sum_vec = _mm256_add_epi32(sum_vec, mul_vec);
                        }

                        // Сохраняем результат
                        int sum_arr[8];
                        _mm256_storeu_si256((__m256i*)sum_arr, sum_vec);

                        // Суммируем все элементы вектора
                        for (int z = 0; z < 8; ++z) {
                            sum += sum_arr[z];
                        }

#pragma omp atomic
                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }
}




// Измерение времени и выполнение умножения
template<typename T>
void run_and_measure(const vector<vector<T>>& A, const vector<vector<T>>& B, vector<vector<T>>& C, const string& version) {
    auto start = chrono::high_resolution_clock::now();
    if (version == "standard") {
        matrix_multiplication_standard(A, B, C);
    }
    else if (version == "blocked") {
        matrix_multiplication_blocked(A, B, C);
    }
    else if (version == "optimized") {
        matrix_multiplication_optimized(A, B, C);
    }
    else if (version == "vectorized") {
        matrix_multiplication_vectorized(A, B, C);
    }
    else if (version == "openmp") {
        matrix_multiplication_openmp_vectorized(A, B, C);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Execution time for " << version << " implementation: " << elapsed.count() << " seconds" << endl;
}

// Функция для сравнения двух матриц
template<typename T>
bool compare_matrices(const vector<vector<T>>& A, const vector<vector<T>>& B) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (A[i][j] != B[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Сравнение и вывод результатов
template<typename T>
void compare_and_print_results(const vector<vector<T>>& standard, const vector<vector<T>>& other, const string& version) {
    if (compare_matrices(standard, other)) {
        cout << "Standard and " << version << " results match!" << endl;
    }
    else {
        cout << "Standard and " << version << " results DO NOT match!" << endl;
    }
}

int main() {
    vector<vector<int>> A(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> B(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> C_standard(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<vector<int>> C_blocked(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<vector<int>> C_optimized(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<vector<int>> C_vectorized(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<vector<int>> C_openmp(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));

    initialize_matrix(A);
    initialize_matrix(B);

    // Выполнение всех реализаций
    run_and_measure(A, B, C_standard, "standard");
    run_and_measure(A, B, C_blocked, "blocked");
    run_and_measure(A, B, C_optimized, "optimized");
    run_and_measure(A, B, C_vectorized, "vectorized");
    run_and_measure(A, B, C_openmp, "openmp");
    
    // Сравнение всех реализаций со стандартной
    compare_and_print_results(C_standard, C_blocked, "Blocked");
    compare_and_print_results(C_standard, C_optimized, "Optimized");
    compare_and_print_results(C_standard, C_vectorized, "Vectorized");
    compare_and_print_results(C_standard, C_openmp, "OpenMP");

    return 0;
}

