// ошибка в открытии libusb_open(device, &devHandle);
#include <iostream> // std
#include <libusb-1.0/libusb.h> // Подключение библиотеки libusb для работы с USB
#include <Cfgmgr32.h> //  конфигурация в винде. работа с дровами винды
#include <thread> // многозадачность
#include <chrono> // -//-
#include <dbt.h> // сообщения об изменении в системе (сообщения)
#include <setupapi.h> // работа с установленной инфой и дровах в системе
#include <initguid.h> // определение для юсб-устройств
#include <Usbiodef.h> // -//-

bool exitFlag = false; // Флаг для управления завершением программы

void listUSBDevices(libusb_context* ctx) {
    while (!exitFlag) { // В бесконечном цикле получаем список подключенных устройств
        libusb_device** deviceList; // Указатель на массив устройств
        ssize_t count = libusb_get_device_list(ctx, &deviceList); // Получаем список устройств
        if (count < 0) { // Проверяем на ошибку
            std::cerr << "Ошибка получения списка устройств: " << libusb_error_name(count) << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Подождать 3 секунды и продолжить
            continue;
        }
        system("cls"); // Очистить консоль
        std::cout << "Выберите устройство для безопасного извлечения (введите номер устройства, 0 для выхода)." << std::endl;
        std::cout << "Список подключенных USB-устройств:" << std::endl;

        for (ssize_t i = 0; i < count; ++i) { // Перебираем устройства в списке
            libusb_device* device = deviceList[i]; // Получаем указатель на устройство
            libusb_device_descriptor desc; // Структура для хранения дескриптора устройства
            int result = libusb_get_device_descriptor(device, &desc); // Получаем дескриптор устройства

            if (result == 0) { // Проверяем на успешное получение дескриптора
                std::cout << "[" << (i + 1) << "] " << "Adress: " << device << " Vendor ID: 0x" << std::hex << desc.idVendor << ", Product ID: 0x" << desc.idProduct << std::dec << std::endl;
                // Выводим информацию о устройстве: адрес, идентификатор производителя и продукта
            }
        }

        libusb_free_device_list(deviceList, 1); // Освобождаем список устройств
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Подождать 3 секунды
    }
}

libusb_device* findUSBDevice(libusb_context* ctx, int choice) {
    libusb_device** deviceList; // Указатель на массив устройств
    ssize_t count = libusb_get_device_list(ctx, &deviceList); // Получаем список устройств
    if (count < 0) { // Проверяем на ошибку
        std::cerr << "Ошибка получения списка устройств: " << libusb_error_name(count) << std::endl;
        return nullptr;
    }

    if (choice > 0 && choice <= count) { // Проверяем корректность выбора устройства
        libusb_device* selectedDevice = deviceList[choice - 1]; // Выбираем устройство
        libusb_free_device_list(deviceList, 1); // Освобождаем список устройств
        return selectedDevice;
    }
    else {
        std::cerr << "Некорректный выбор устройства." << std::endl;
        libusb_free_device_list(deviceList, 1); // Освобождаем список устройств
        return nullptr;
    }
}

bool safelyRemoveUSBDevice(libusb_device* device) {
    libusb_device_handle* devHandle;
    int result = libusb_open(device, &devHandle); // Открываем устройство
    if (result != 0) { // Проверяем на ошибку
        std::cerr << "Ошибка открытия устройства: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Получаем дескриптор интерфейса
    int interface = 0; // Выберите интерфейс, который вы хотите извлечь
    result = libusb_claim_interface(devHandle, interface); // Получаем доступ к интерфейсу
    if (result != 0) { // Проверяем на ошибку
        std::cerr << "Ошибка получения доступа к интерфейсу: " << libusb_error_name(result) << std::endl;
        libusb_close(devHandle); // Закрываем устройство
        return false;
    }

    // Вызываем отключение интерфейса
    result = libusb_release_interface(devHandle, interface); // Отключаем интерфейс
    if (result != 0) { // Проверяем на ошибку
        std::cerr << "Ошибка отключения интерфейса: " << libusb_error_name(result) << std::endl;
        libusb_close(devHandle); // Закрываем устройство
        return false;
    }

    libusb_close(devHandle); // Закрываем устройство

    return true;
}

int main() {
    setlocale(0, ""); // Устанавливаем локаль для корректного вывода

    libusb_context* ctx = nullptr;
    int result = libusb_init(&ctx); // Инициализируем библиотеку libusb
    if (result < 0) { // Проверяем на ошибку
        std::cerr << "Ошибка инициализации libusb: " << libusb_error_name(result) << std::endl;
        return 1;
    }

    std::thread deviceListThread(listUSBDevices, ctx); // Запускаем поток для отображения списка устройств
    system("cls");
    while (!exitFlag) {

        int choice = 0;
        std::cin >> choice;

        if (choice == 0) {
            exitFlag = true;
            break; // Пользователь выбрал выход
        }

        libusb_device* selectedDevice = findUSBDevice(ctx, choice);
        if (selectedDevice != nullptr) {
            if (safelyRemoveUSBDevice(selectedDevice)) {
                std::cout << "Устройство безопасно извлечено." << std::endl;
            }
            else {
                std::cerr << "Ошибка извлечения устройства." << std::endl;
            }
        }
    }

    deviceListThread.join(); // Дождитесь завершения потока
    libusb_exit(ctx); // Завершаем работу с libusb

    return 0;
}
