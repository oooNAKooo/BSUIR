#include <iostream>
#include <vector>
#include <asio.hpp>

using asio::ip::tcp;
using namespace std;

const int MATRIX_SIZE = 2048;
const int BLOCK_SIZE = MATRIX_SIZE / 2;

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

void receive_block_blocking(tcp::socket& socket, vector<vector<int>>& matrix, int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        asio::read(socket, asio::buffer(matrix[i].data(), MATRIX_SIZE * sizeof(int)));
    }
}

void send_block_blocking(tcp::socket& socket, const vector<vector<int>>& matrix, int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        asio::write(socket, asio::buffer(matrix[i].data(), MATRIX_SIZE * sizeof(int)));
    }
}

void receive_block_nonblocking(tcp::socket& socket, vector<vector<int>>& matrix, int start_row, int end_row, asio::io_context& io_context) {
    for (int i = start_row; i < end_row; ++i) {
        asio::post(io_context, [&socket, &matrix, i] {
            asio::async_read(socket, asio::buffer(matrix[i].data(), MATRIX_SIZE * sizeof(int)),
                [](const asio::error_code&, std::size_t) {});
        });
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

int main() {
    asio::io_context io_context;
    tcp::socket socket(io_context);

    try {
        socket.connect(tcp::endpoint(asio::ip::address::from_string("сервер_IP"), 12345));

        vector<vector<int>> A(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
        vector<vector<int>> B(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
        vector<vector<int>> C(BLOCK_SIZE, vector<int>(MATRIX_SIZE, 0));

        // Блокирующий режим
        cout << "Используется блокирующий режим..." << endl;
        receive_block_blocking(socket, A, BLOCK_SIZE, MATRIX_SIZE);
        receive_block_blocking(socket, B, BLOCK_SIZE, MATRIX_SIZE);

        multiply_block(A, B, C, 0, BLOCK_SIZE);
        send_block_blocking(socket, C, 0, BLOCK_SIZE);

        // Неблокирующий режим
        cout << "Используется неблокирующий режим..." << endl;
        receive_block_nonblocking(socket, A, BLOCK_SIZE, MATRIX_SIZE, io_context);
        receive_block_nonblocking(socket, B, BLOCK_SIZE, MATRIX_SIZE, io_context);

        multiply_block(A, B, C, 0, BLOCK_SIZE);
        send_block_nonblocking(socket, C, 0, BLOCK_SIZE, io_context);

        io_context.run();
    } catch (std::exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
