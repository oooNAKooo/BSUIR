#pragma once

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int num_v = 1, num_i = 1, num_h_v = 1;

void info_frame();
void capture_video();
void capture_photo();
void capture_hidden_video();

int main()
{

    setlocale(0, "");
    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        cout << endl << "\tНе удалось открыть камеру." << endl;
        return -1;
    }

    int choice;

    while (true) {
        system("cls");
        cout << "\tВыберите действие:" << endl << endl;
        cout << "\t[1] Свойства камеры;" << endl;
        cout << "\t[2] Записать видео;" << endl;
        cout << "\t[3] Сделать фото;" << endl;
        cout << "\t[4] Скрытая видеосъемка;" << endl;
        cout << "\t[0] Выход." << endl;
        cout << endl << "\tВведите номер выбранного действия: ";
        cin >> choice;

        switch (choice) {
        case 1:
            info_frame();
            break;
        case 2:
            capture_video();
            break;
        case 3:
            capture_photo();
            break;
        case 4:
            capture_hidden_video();
            break;
        case 0:
            system("cls");
            cout << endl<<"\tДо скорой встречи!" << endl;
            return false;
        default:
            system("cls");
            cout << endl << "\tНеверный выбор." << endl << endl;
            system("pause");
            system("cls");
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
