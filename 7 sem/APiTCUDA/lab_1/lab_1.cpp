#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono> // Библиотека для работы с временем
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

// Функция для сравнения двух матриц
template<typename T>
bool compare_matrices(const vector<vector<T>>& A, const vector<vector<T>>& B) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (A[i][j] != B[i][j]) {
                return false; // Нашли несовпадение
            }
        }
    }
    return true; // Все элементы совпадают
}

template<typename T>
void run_and_measure(const vector<vector<T>>& A, const vector<vector<T>>& B, vector<vector<T>>& C, const string& version) {
    auto start = chrono::high_resolution_clock::now(); // Старт замера времени
    if (version == "standard") {
        matrix_multiplication_standard(A, B, C);
    }
    else if (version == "blocked") {
        matrix_multiplication_blocked(A, B, C);
    }
    else if (version == "optimized") {
        matrix_multiplication_optimized(A, B, C);
    }
    auto end = chrono::high_resolution_clock::now(); // Окончание замера времени

    chrono::duration<double> elapsed = end - start; // Вычисление времени в секундах
    cout << "Execution time for " << version << " implementation: " << elapsed.count() << " seconds" << endl;
}

int main() {
    vector<vector<int>> A(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> B(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> C_standard(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<vector<int>> C_blocked(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<vector<int>> C_optimized(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));

    // Инициализация матриц
    initialize_matrix(A);
    initialize_matrix(B);

    // Выполнение умножений и замеры времени
    run_and_measure(A, B, C_standard, "standard");
    run_and_measure(A, B, C_blocked, "blocked");
    run_and_measure(A, B, C_optimized, "optimized");

    // Сравнение результатов
    if (compare_matrices(C_standard, C_blocked)) {
        cout << "Standard and Blocked results match!" << endl;
    }
    else {
        cout << "Standard and Blocked results do NOT match!" << endl;
    }

    if (compare_matrices(C_standard, C_optimized)) {
        cout << "Standard and Optimized results match!" << endl;
    }
    else {
        cout << "Standard and Optimized results do NOT match!" << endl;
    }

    return 0;
}
