#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstring>

typedef struct {
    int x;
    int y;
} pair;

pair p = {0, 0};

// чтобы собирать статистику
int stats[2];

// Определяем функцию для обработки сигнала таймера
void timer_handler() {
// Проверяем содержимое пары и увеличиваем соответствующую статистику
    if (stats[0] == stats[1]) {
        stats[0]++;
    } else if (stats[0] > stats[1]) {
        stats[1]++;
    } else {
        stats[0]++;
    }
}

// Определяем функцию для обработки сигнала подтверждения от C_k
void confirm_handler(int sig) {

    timer_handler();

    char ppid[10], pid[10], cnt1[10], cnt2[10], sig1[10];
    sprintf(ppid, "%d", getppid());
    sprintf(pid, "%d", getpid());
    sprintf(cnt1, "%d", stats[0]);
    sprintf(cnt2, "%d", stats[1]);
    sprintf(sig1, "%d", sig);
    
    // Процесс C_k запрашивает доступ к stdout у P и выполняет вывод после подтверждения
    
    std::cout << "Процесс C_k запрашивает доступ к стандартному выводу из P и выполняет вывод после подтверждения..." <<  std::endl;
    
// Функция fputs() записывает содержимое строки, на которую указывает sir, в заданный поток. Нуль в конце строки не записывается.
//В случае успеха функция fputs() возвращает последний записанный символ, а в случае неудачи EOF. Если поток открыт в текстовом режиме,
//могут иметь место некоторые преобразования сим­волов. Это значит, что строка может быть переписана в файл не «один к одному».
// Если же поток открыт в двоичном режиме, то преобразования символов нет и строка переписывается в файл «один к одному».

    //вывод при помощи fputs()

    std::cout << "Ppid : " << getppid() << std::endl;

    std::cout << "Pid : " << getpid() << std::endl;

    std::cout << "Stats[0] : " << fputc(cnt1[10], stdout) << std::endl;

    std::cout << "Stats[1] : " << fputc(cnt2[10], stdout) << std::endl; 

    std::cout << "Sig : " << fputc(sig, stdout) << std::endl;

    std::cout << "X : " << fputc(p.x, stdout) << std::endl;
    std::cout << "Y : " << fputc(p.y, stdout) << std::endl;
    fputc('\n', stdout);
    alarm(10);
    
    //stdout - стандартный поток ввода
    
    // Процесс C_k уведомляет P о завершении вывода
    std::cout << "Процесс C_k уведомляет P о завершении вывода..." <<  std::endl; 
}

int main() {

   // Настраиваем обработчики сигналов для SIGUSR1 и SIGUSR2
    signal(SIGALRM, confirm_handler);//ловит сигнал
  
    alarm(4);
    while(true){

       if(p.x == 0 && p.y == 0)
        {
            p.x = 1;
            p.y = 1;
        }
        else 
        {
            p.x = 0;
            p.y = 0;
        }
    
    }

     std::cout << "Вывод завершен." <<  std::endl;

    return 0;
}
