#include <iostream> // std
#include <filesystem> // работа с файловой системой
#include <stdlib.h> // работа с памятью
#include <sys/wait.h> // работа с процессами (функция ожидания)
#include <string> // строки
#include <sys/types.h> // работа с типами данных
#include <signal.h> // работа с сигналами
#include <limits.h> // работа с числовыми ограничениями
#include <sstream> // строковые потоки
#include <vector> // работа с динамическими массивами       
#include <unistd.h> // управление процессами и файловой системой
#include <sys/stat.h> // работа с информацией о файлах
#include <sys/mman.h> // отображение файлов в память
#include <sys/ipc.h> // функции создания и удаления ключей для обмена данных 
#include <sys/shm.h> // для обмена процессов данными через общую область памяти 

using namespace std; 

extern char** environ; // указатель на массив строк, содержащий все значения переменных окружения (текущие)

int shm_open(const char* name, int oflag, mode_t mode); //Выделение разделяемой памяти 
			// "/name", используемые флаги, определение прав доступа 
void exit_programm(int sig); // выход
int analysis_command(string command); // команды

void exit_programm(int sig)
{
  char ch;
  while(true)
  {
    rewind(stdin);
    cout << endl;
    cout << "Точно вы хотите завершить программу?(Y/N)" << endl ;
    ch = getchar();
    if(ch == 'N')
    {
      break;
    }
    else if(ch == 'Y')
    {
      cout << "Программа завершена с кодом: " << sig << endl;
      exit(sig); // завершение программы
    }
    else 
    {
      cout << "Неверный вариант!" << endl;
      continue;
    }
  }
}


int analysis_command(string command)
{
  int a; 
  if(command.length() == 1)
  {
    if(command == "+"){  a = 1;}
    else if(command == "-"){ a = 2;}
    else if(command == "l") {a = 3;}
    else if(command == "k") {a = 4;}
    else if(command == "s") {a = 5;}
    else if(command == "g") {a = 6;}
    else if(command == "q") {a = 10;}
    else {a = 100;}
  }
  else
  {
    if(command.substr(0,2) == "s<"){return 7;} // substr - обрезка строки (2 символа)
    else if (command.substr(0,2) == "g<"){return 8;}
    else if (command.substr(0,2) == "p<") {return 9;}
  }
  return a;
}

void information_programm()
{
  cout << "Список имеющихся комманд:" << endl
  << "[+]"<<"\t"<<"[-]"<<"\t"<<"[l]"<<"\t"<<"[k]"<<"\t"<<"[q]"<<endl
  << "[s]"<<"\t"<<"[g]"<<"\t"<<"[s<num]"<<"\t"<<"[g<num]"<<"\t"<<"[p<num]"<<endl;
}

int main(int argc, char* argv[])
{ 
  cout << "Программа запущена, создан родительский процесс!" << endl;
  information_programm();

  signal(SIGINT, exit_programm); // установка обработчика сигнала SIGINT на функцию exit_programm
  vector<int> counter; // объявление переменной вектора
  
  // shmid - индетификатор созданного сегмента 
  // shmget - для создания или получения доступа к сегменту раздел.пам.
  int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666); // 1 - нужно создать!, а не открыть
  								// 2 - определение размера сегмента
  							// 3 - определение прав доступа (666 - для всех)

  // shared_mem - указатель на область памяти, соответствующая созданному сегменту
  //shmat() присоединяет сегмент разделяемой памяти, к адресному пространству вызывающего процесса.
  int* shared_mem = (int*)(shmat(shmid, NULL, 0));  //shmid+сама выбирает обл.пам.+флаг по умолчанию 
  
  string command; // объявление переменной типа стринг
  while(true)
  {
    cout << "Введите команду для выбора действия: ";
    getline(cin, command, '\n');
    switch(analysis_command(command))
    {
      case 1:
      {
        string path = "/home/kali/OSISP/lab3/child";
        char* path_cstr = (char*)(path.c_str()); // преобразуем объект типа стринг в чар*
      pid_t child = fork(); // создание нового процесса, кот будет копией процесса
        		    // fork() возвращает значение типа pidt, кот. опред. в каком процессе выполняется код
        		    // если pidt == 0, значит процесс дочерний, если >0 - то родительский 
        if(child == 0)
        {
          // родительская область видимости не может присвоить значение другого процесса в родительскую переменную
          *shared_mem = getpid(); // присваиваем идентификатор текущего процесса для доступа к разделяемой памяти
          cout << "PID в разделяемой памяти [" << *shared_mem << "]" << endl;
          cout << "PID дочернего процесса добавлен в счетчик [" << getpid() << "]" << endl;
          char* argv[] = { path_cstr, NULL}; // создание массива указателей на строки argv (путь к файлу+NULL)
          execve(path_cstr, argv, NULL); // запуск новой программы 
          				 // (путь к файлу, массив строк, отстутствие переменных окружения)
        }
        
        // записываем значение из разделяемой памяти
        sleep(1); // Не лучший вариант использования, влияние на производительность
        cout << "Разделяемая память после выхода из области видимости -> " << *shared_mem << endl;
        counter.push_back(*shared_mem); // значение *shared_mem копируем в конец counter 
        break;
      }
      case 2:
      {
        cout << "Сработала команда вызова убийства последнего дочернего процесса!" << endl;
        if(counter.size() == 0 )
        {
          cout << "Последнего дочернего процесса не существует!" << endl;
        }
        else
        {
          int last_process = counter.at(counter.size()-1); // сохраняем последнее значение вектора 
        kill(last_process, SIGKILL); // остановка последнего процесса
        counter.erase(counter.begin() +counter.size() -1); // удаление последнего элемента вектора
        }
        break;
      }
      case 3:  
      {
        cout << "Вывод все процессов программы!" << endl;
        cout << "[0] -> Родительский процесс: [" << getpid() << "]" << endl;
        if(counter.size() == 0)
        {
          cout << "[-] -> дочерние процессы отсутствуют." << endl;
        }
        else 
        {
          int a = 1;
          for(auto i: counter)
          {
            cout <<"[" << a << "] -> Дочерний процесс: [" << i << "]" << endl;
            ++a; 
          }
        }
        break;
      }
      case 4:  
      {
        if(counter.size() == 0)
        {
          cout << "Нет дочерних процессов запущенных родительским процессом!" << endl;
        }
        else 
        {
          cout << "Режим удаления запущен!" << endl;
          for(int i = counter.size()-1; i > -1; --i)
          {
            int number = counter.at(i);
            kill(number, SIGKILL);
            counter.erase(counter.begin() + i); // удаление всех процессов
          }
          cout << "Удаление всех дочерних процессов завершено!" << endl;
        }
        break;
      }
      case 5:  
      {
        cout << "Включен запрет вывода статистики всем дочерним процессам!" << endl;
        if(counter.size() == 0)
        {
          cout << "Дочерних процессов не существует!" << endl;
        }
        else 
        {
          for(int i = 0; i < counter.size(); ++i)
          {
            int pid = counter.at(i); // сохранение индетификатора процесса
            kill(pid, SIGSTOP); // остановка этого сигнала
          }
        }
        cout << "Все дочерний процессы приостановлены!" << endl;
        break;
      }
      case 6:  
      {
        cout << "Подключение разешения на вывод дочерним процессам статистику!" << endl;
        if(counter.size() == 0)
        {
          cout << "Нет дочерних процесссов!" << endl;
        }
        else 
        {
          for(int i = 0; i < counter.size(); ++i )
          {
            int pid = counter.at(i); // сохранение нужного процесса
            kill(pid, SIGCONT); // разрешение на его продолжение
          }
          cout << "Активирование включено успешно!" << endl;
        }
        break;
      }
      case 7:  
      {
        // сначала мы присваиваем строку number_pid_str
        // потом с помощью stringstream конвентим в число 
        // это все мы делаем чтобы прочитало pid
        string number_pid_str;
        int number_pid;
        number_pid_str = command.substr(2,5); // начинаем со второго символа макс длина 5
        stringstream buffer; // преобразование строки в целое число
        buffer << number_pid_str; // помещаем значение
        buffer >> number_pid; // извлекаем значение
        if(counter.size() == 0)
        {
          cout << "Родительский процесс не запускал дочерние процессы!" << endl;
        }
        else
        {
	for(int i = 0; i < counter.size(); ++ i)
          {
            if(counter.at(i) == number_pid)//a t это функция которая берет и считывает определенный символ 
            {
              cout << "Поступил сигнал остановки процесса!" << endl;
              kill(number_pid, SIGSTOP); // остановка процесса
              
            }
          }
        }
        break;
      }
      case 8:  
      {
        string number_pid_str;
        int number_pid;
        number_pid_str = command.substr(2,5); // начинаем со второго символа макс длина 5
        stringstream buffer; // преобразование строки в целое число
        buffer << number_pid_str; // помещаем значение
        buffer >> number_pid; // извлекаем значение
        if(counter.size() == 0)
        {
          cout << "Родительский процесс не запускал дочерние процессы!" << endl;
        }
        else
        {
          for(int i = 0; i < counter.size(); ++ i) // проход по всем процессам
          {
            if(counter.at(i) == number_pid)
            {
              cout << "Поступил сигнал остановки процесса!" << endl;
              kill(number_pid, SIGCONT); // продолжение процесса
            }
          }
        }
        break;
      }
      case 9: 
      {
        if(counter.size() == 0)
        {
          cout << "Нет дочерних процессов, которые можно запретить!" << endl;
        }
        else
        {
        cout << "Основано на разъединени процессов!" << endl;
        for(int i = 0; i< counter.size(); ++i)
        {
          kill(counter.at(i),SIGSTOP); // остановка процесса
        }
        pid_t test = fork(); // создание процесса
        
        if(test == 0)
        {
          cout << "Номер процесса в случае сбоя: " << getpid() << endl;
          char t;
          alarm(5); // установка количества секунд, после которых пойдет сигнал SIGALRM
          t = getchar();
          if(pause() && t == NULL)
          {
            kill(getpid(), SIGKILL); // удаление процесса
          }
          else if( t == 'g')
          {
            stringstream buffer; // преобразование строки в целое число
            buffer << command.substr(2,5); // получаем значение (начинаем со второго символа (макс длина 5))
            int number_pid ;
            buffer >> number_pid; // передаем значение 
            kill(number_pid, SIGCONT); // продолжение процесса 
            kill(getpid(), SIGKILL); // удаление 
          }
        }
        else
        {
          int status;
          waitpid(test, &status, 0); // ожидание завершения процесса
          			     // процесс - тест
          			     // сохранение информации о его статусе
          			     // 0 || флаги
          for(int i = 0; i < counter.size(); ++i) //продолжение всех процессов
          {
            kill(counter.at(i), SIGCONT);
          }
          cout << "Процесс обработки завершился!" << endl;
        }
        }
        break;
      }
      case 10:  
      { 
        //Реализовать убийство всех дочерних процессов
        cout << "Программа успешно завершена c кодом 0!" << endl;
        int child_process;
        if(counter.size() == 0)
        {
          cout << "Дочерних процессов запущенных родительским процессом нет!" << endl;
        }
        else 
        {
          //Так же можно реализовать жесткое убийство и более мягкое!
          for(int i =0; i < counter.size(); ++i ) // отправка сигнала SIGTERM для всех процессов
          {
            kill(counter.at(i), SIGTERM); // жесткое убийство
          }
        }
        exit(0);
        break;
      }
      default:
      {
        cout << "Введена неверная комманда, повторить комманду!" << endl;
        break;
      }
    }
  }

  shmdt(shared_mem);                         // Отсоединение указателя shared_mem от раздел.пам.. 
  					     // Память остается выделенной!!!
  shmctl(shmid, IPC_RMID, NULL);             // Удаление разделяемой памяти вместе с shmid
  					     // Память не будет доступна для использования!!!
  return 0;
}


