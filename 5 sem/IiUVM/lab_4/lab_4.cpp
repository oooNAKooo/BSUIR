#include "main.h"
#include <iostream>

using namespace cv;
using namespace std;

void info_frame()
{
    int num_cameras = 10; // Допустим камер несколько.
    system("cls");
    cout << "Доступные камеры:" << endl;

    for (int i = 0; i < num_cameras; ++i)
    {
        VideoCapture cap(i);
        if (cap.isOpened())
        {
            cout << "Камера " << (i+1) << ": ";
            cout << "Ширина = " << cap.get(CAP_PROP_FRAME_WIDTH) << " "; // Ширина кадра.
            cout << "Высота = " << cap.get(CAP_PROP_FRAME_HEIGHT) << " "; // Высота кадра.
            cout << "FPS = " << cap.get(CAP_PROP_FPS) << endl; // gходу камера не предоставляет инфу о фпс.
            cap.release();
        }
    }
    system("pause");
}

void capture_video()
{
    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        cout << "Не удалось открыть камеру." << endl;
        return;
    }

    int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D'); // Кодек Xvid.
    // Кодек - алгоритм, который используется для сжатия видео данных
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    string folder_path = "C:/Ваш/путь/к/папке/Video/"; // Путь к папке с видео.
    string video_file_name = "test_" + to_string(num_v) + ".avi"; // Название файла.
    string full_video_name = folder_path + video_file_name; // Путь+имя_файла.
    VideoWriter video(full_video_name, fourcc, 10, Size(frame_width, frame_height)); // В объект заидываем следующие переменные.
    // путь к файлу, кодек, фпс, размер кадра в пикселях(ширина и высота кадра))
    num_v++;

    // Проверка, удалось ли создать объект для записи видео.
    if (!video.isOpened())
    {
        cout << "Не удалось создать файл для записи видео." << endl;
        return;
    }

    // Переменная для хранения кадра.
    Mat frame;
    // Mat - основной класс для хранения и обработки изображений и данных в библиотеке OpenCV
    // Mat - matrix
    while (true)
    {
        // Захватываем кадр с камеры.
        cap >> frame;

        // Проверка на ошибки при захвате.
        if (frame.empty())
        {
            cout << "Ошибка при захвате кадра." << endl;
            break;
        }

        // Записываем кадр в видеофайл.
        video.write(frame);

        // Отображаем кадр в окне с названием "Webcam".
        imshow("Webcam", frame);

        // Обработка клавиши "ctrl+z" для выхода из программы.
        if ((waitKey(1) & 0xFF) == 26)
            break;
    }

    // Для завершения записи и закрытия видеофайла.
    video.release(); // Освобождение ресурсов.
    system("cls");
    cout << endl << "\tФайл " << video_file_name << " с записью видео сохранен в в папке по следующему пути: " << endl << endl << "\t" << folder_path << endl << endl;
    system("pause");
}

void capture_photo()
{
    cv::VideoCapture cap(0); // Открытие камеры с индексом 0. 

    if (!cap.isOpened())
    {
        cout << "Не удалось открыть камеру." << endl;
        return;
    }

    Mat frame;

    cap >> frame;

    if (frame.empty())
    {
        cout << "Ошибка при захвате кадра." << endl;
        return;
    }
    string folder_path = "C:/Ваш/путь/к/папке/Photo/";  // Как рассказано ранее с видео.
    string image_file_name = "test_" + to_string(num_i) + ".jpg"; // Как рассказано ранее с видео.
    string full_image_name = folder_path + image_file_name; // Как рассказано ранее с видео.
    imwrite(full_image_name, frame); // Функция для сохранения изображения в файл.
    // (путь, кадр)
    num_i++;

    // После работы с веб-камерой, нужно закрыть все ранее вызванные окна.
    destroyAllWindows();
    system("cls");
    cout << endl << "\tФайл " << image_file_name << " с записью фото сохранен в папке по следующему пути: " << endl << endl << "\t" << folder_path << endl << endl;
    system("pause");
}

void capture_hidden_video()
{
    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        cout << "Не удалось открыть камеру." << endl;
        return;
    }

    int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D'); // Используйте другой кодек, если нужно.
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    string folder_path = "C:/Ваш/путь/к/папке/HiddenVideo/"; // Как рассказано ранее с видео.
    string hidden_video_file_name = "hidden_video_" + to_string(num_h_v) + ".avi"; // Как рассказано ранее с видео.
    string full_hidden_video_name = folder_path + hidden_video_file_name; // Как рассказано ранее с видео.
    VideoWriter video(full_hidden_video_name, fourcc, 30, Size(frame_width, frame_height)); // Как рассказано ранее с видео.
    num_h_v++;

    // Проверка, удалось ли создать объект для записи видео.
    if (!video.isOpened())
    {
        cout << "Не удалось создать файл для записи видео." << endl;
        return;
    }

    // Переменная для хранения кадра.
    Mat frame;

    chrono::steady_clock::time_point start_time = chrono::steady_clock::now(); // Сейчас.
    chrono::steady_clock::time_point end_time; // Конец.
    chrono::duration<double> elapsed_time; // Время между start_time и end_time.

    while (true)
    {
        // Захватываем кадр с камеры.
        cap >> frame;

        // Проверка на ошибки при захвате.
        if (frame.empty())
        {
            cout << "Ошибка при захвате кадра." << endl;
            break;
        }

        // Записываем кадр в видеофайл.
        video.write(frame);

        end_time = chrono::steady_clock::now();
        elapsed_time = chrono::duration_cast<chrono::seconds>(end_time - start_time); // На выходе 10 секунд - 1 секунда.

        if (elapsed_time.count() >= 5) // Если больше 5 секунд.
            break;
    }

    // Сказано ранее.
    video.release();
    system("cls");
    cout << endl << "\tСкрытая видеосъемка завершена. Файл " << hidden_video_file_name << " сохранен в папке по следующему пути: " << endl << endl << "\t" << folder_path << endl << endl;
    system("pause");
}
