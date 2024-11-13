#include <iostream>
#include <vector>
#include <asio.hpp>
#include <chrono>
#include <thread>
#include <random>  // Для генерации случайных чисел

using asio::ip::tcp;
using namespace std;

// Размер матрицы и блока
const int MATRIX_SIZE = 2048;
const int BLOCK_SIZE = MATRIX_SIZE / 2;

// Функция для заполнения матрицы случайными числами
void fill_matrix_with_random_numbers(vector<vector<int>>& matrix) {
    random_device rd;  // Источник случайных чисел
    mt19937 gen(rd());  // Генератор случайных чисел
    uniform_int_distribution<> dis(0, 100);  // Диапазон случайных чисел от 0 до 100

    for (auto& row : matrix) {
        for (auto& element : row) {
            element = dis(gen);  // Заполняем элемент случайным числом
        }
    }
}

void multiply_block(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void send_block_blocking(tcp::socket& socket, const vector<vector<int>>& matrix, int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        asio::write(socket, asio::buffer(matrix[i].data(), MATRIX_SIZE * sizeof(int)));
    }
}

void receive_block_blocking(tcp::socket& socket, vector<vector<int>>& matrix, int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        asio::read(socket, asio::buffer(matrix[i].data(), MATRIX_SIZE * sizeof(int)));
    }
}

void send_block_nonblocking(tcp::socket& socket, const vector<vector<int>>& matrix, int start_row, int end_row, asio::io_context& io_context) {
    for (int i = start_row; i < end_row; ++i) {
        asio::post(io_context, [&socket, &matrix, i] {
            asio::async_write(socket, asio::buffer(matrix[i].data(), MATRIX_SIZE * sizeof(int)),
            [](const asio::error_code&, std::size_t) {});
            });
    }
}

void receive_block_nonblocking(tcp::socket& socket, vector<vector<int>>& matrix, int start_row, int end_row, asio::io_context& io_context) {
    for (int i = start_row; i < end_row; ++i) {
        asio::post(io_context, [&socket, &matrix, i] { // добавляем задачу
            asio::async_read(socket, asio::buffer(matrix[i].data(), MATRIX_SIZE * sizeof(int)),
            [](const asio::error_code&, std::size_t) {}); // обработчик звершения
            });
    }
}

int main() {
    setlocale(0, "");
    asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
    tcp::socket socket(io_context);

    cout << "Ожидание подключения клиента..." << endl;
    acceptor.accept(socket);
    cout << "Клиент подключен." << endl;

    // Инициализируем матрицы случайными числами
    vector<vector<int>> A(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> B(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> C(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    fill_matrix_with_random_numbers(A);
    fill_matrix_with_random_numbers(B);

    // Блокирующий режим
    cout << "Используется неблокирующий режим..." << endl;
    auto start_time = chrono::high_resolution_clock::now();

    send_block_blocking(socket, A, BLOCK_SIZE, MATRIX_SIZE);
    send_block_blocking(socket, B, BLOCK_SIZE, MATRIX_SIZE);

    multiply_block(A, B, C, 0, BLOCK_SIZE);

    vector<vector<int>> client_result(BLOCK_SIZE, vector<int>(MATRIX_SIZE, 0));
    receive_block_blocking(socket, client_result, 0, BLOCK_SIZE);

    for (int i = 0; i < BLOCK_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            C[BLOCK_SIZE + i][j] = client_result[i][j];
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    cout << "Время (неблокирующий режим): " << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << " ms" << endl;

    // Неблокирующий режим
    cout << "Используется блокирующий режим..." << endl;
    start_time = chrono::high_resolution_clock::now();

    send_block_nonblocking(socket, A, BLOCK_SIZE, MATRIX_SIZE, io_context);
    send_block_nonblocking(socket, B, BLOCK_SIZE, MATRIX_SIZE, io_context);
    io_context.run();

    multiply_block(A, B, C, 0, BLOCK_SIZE);

    receive_block_nonblocking(socket, client_result, 0, BLOCK_SIZE, io_context);
    io_context.run();

    end_time = chrono::high_resolution_clock::now();
    cout << "Время (блокирующий режим): " << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << " ms" << endl;

    return 0;
}
