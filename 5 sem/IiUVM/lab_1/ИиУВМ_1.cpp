#include <iostream>
#include <thread>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <powrprof.h>

#pragma comment(lib, "PowrProf.lib")

// Глобальная переменная для хранения информации о состоянии энергопитания
SYSTEM_POWER_STATUS globalPowerStatus;

// Функция для вывода информации о состоянии энергопитания
void PrintPowerInfo() {
    while (true) {
        // Получение информации о состоянии энергопитания
        if (GetSystemPowerStatus(&globalPowerStatus)) {
            system("cls"); // Очищаем консоль перед выводом новой информации

            // Вывод информации о типе энергопитания (сеть или аккумулятор)
            std::cout << "Тип энергопитания: " << (globalPowerStatus.ACLineStatus ? "Подключено к сети" : "Работает от аккумулятора") << std::endl;

            // Вывод информации о типе батареи и уровне заряда (если есть батарея)
            if (globalPowerStatus.BatteryFlag != 255) {
                std::cout << "Тип батареи: ";
                if (globalPowerStatus.BatteryFlag & 8) {
                    std::cout << "Заряжается" << std::endl;
                }
                else if (globalPowerStatus.BatteryFlag & 1) {
                    std::cout << "Высокая степень заряда" << std::endl;
                }
                else if (globalPowerStatus.BatteryFlag & 2) {
                    std::cout << "Низкая степень заряда" << std::endl;
                }
                else if (globalPowerStatus.BatteryFlag & 4) {
                    std::cout << "Критическая степень заряда" << std::endl;
                }
                else if (globalPowerStatus.BatteryFlag & 128) {
                    std::cout << "Нет батареи" << std::endl;
                }
                else {
                    std::cout << "Неизвестное состояние" << std::endl;
                }

                std::cout << "Уровень заряда батареи: " << static_cast<int>(globalPowerStatus.BatteryLifePercent) << "%" << std::endl;
            }
            else {
                std::cout << "Батарея не обнаружена." << std::endl;
            }

            // Вывод информации о текущем режиме энергосбережения (балансировка или максимальная производительность)
            std::cout << "Текущий режим энергосбережения: " << (globalPowerStatus.SystemStatusFlag ? "Режим балансировки" : "Максимальная производительность") << std::endl;

            // Вывод информации о времени работы от аккумулятора (если не подключено к сети)
            if (!globalPowerStatus.ACLineStatus) {
                DWORD timeLeft = globalPowerStatus.BatteryLifeTime;
                std::cout << "Время работы аккумулятора: " << (globalPowerStatus.ACLineStatus ? "Подключено к питанию" : "Не подключено к питанию") << std::endl;

                if (!globalPowerStatus.ACLineStatus) {
                    DWORD timeLeftMinutes = timeLeft / 60; // Преобразование секунд в минуты
                    std::cout << "Оставшееся время работы от батареи: " << timeLeftMinutes << " минут" << std::endl;
                }
            }
        }
        else {
            std::cerr << "Не удалось получить информацию о состоянии энергопитания." << std::endl;
        }

        // Вывод меню действий
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Выберите действие:" << std::endl;
        std::cout << "1. Перейти в спящий режим" << std::endl;
        std::cout << "2. Перейти в режим гибернации" << std::endl;
        std::cout << "3. Выход" << std::endl;
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1:
            // Переходим в режим сна
            SetSuspendState(FALSE, TRUE, FALSE);
            break;
        case 2:
            // Переходим в режим гибернации
            SetSuspendState(TRUE, TRUE, FALSE);
            break;
        case 3:
            // Завершаем поток мониторинга и выходим
            return;
        default:
            system("cls");
            std::cerr << "Некорректный выбор. Попробуйте ещё раз." << std::endl;
            system("pause");
            system("cls");
        }

        // Пауза для обновления информации каждые 5 секунд
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    setlocale(0, "");

    // Создаем поток для мониторинга состояния энергопитания
    std::thread powerInfoThread(PrintPowerInfo);

    // Ожидаем завершения потока перед выходом
    powerInfoThread.join();

    return 0;
}