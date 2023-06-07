import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
public class Main {
    private static final Object lock = new Object(); // блокировка доступа к ресурсу (исп. внутри метода run)
    private static List<MyThread> threads = new ArrayList<>(); // лист потоков
    private static int count = 0; // для № потока
    private static Scanner in = new Scanner(System.in);//спец.класс для работы с вводом
    public static void main(String[] args) throws InterruptedException, IOException {
        String input;
        while(true){
            System.out.println("Введите '+' для добавления потока, '-' для удаления потока или 'q' для выхода:");
            input = in.nextLine(); // ввод строки
            for (int i = 0; i < input.length(); i++) {
                char c = input.charAt(i); // преобразование строки в char
                switch (c) {
                    case '+': {
                        if (threads.size() < 10) {
                            MyThread thread = new MyThread("Thread " + (count++));
                            threads.add(thread); // добавление в лист новый поток
                            thread.start(); // запуск потока
                            System.out.println("Добавлен новый поток: " + (thread.getName()));
                        } else {
                            System.out.println("Нельзя сразу добавить больше 10 потоков!");
                        }
                    }
                    break;
                    case '-': {
                        if (!threads.isEmpty()) { // проверка на пустоту
                            MyThread thread = threads.get(0); // выбор последнего элемента
                            thread.setShouldStop(true); // вызов метода
                            threads.remove(0); // удаление последнего
                            MyThread thread2 = new MyThread("Thread " + (--count)); // №потока--
                            System.out.println("Осталось потоков: " + threads.size()); // остаток потоков
                        } else {
                            System.out.println("Нет потоков для удаления!");
                        }
                    }
                    break;
                    case 'q': {
                        System.exit(0); // завершение программы
                    }
                    default: {
                        System.out.println("Некорректный ввод символа!");
                    }
                    break;
                }
            }
        }
    }
    private static class MyThread extends Thread {
        private String message; // строка, которую поток будет выводить в консоль
        private boolean shouldStop = false; // флаг, указывающий, должен ли поток остановиться после выполнения своей задачи
        public MyThread(String message) { // конструктор класса, принимает строку message и сохраняет ее в соответствующем поле
            this.message = message;
        }
        public void setShouldStop(boolean shouldStop) { // метод для установки значения флага shouldStop
            this.shouldStop = shouldStop;
        }

        @Override
        public void run() { // переопределение метода run() для выполнения потока
            while (!shouldStop) {  // synchronized (lock) работа только с одним потоком

                synchronized (lock) { // блокировка доступа к ресурсу (в данном случае - к консоли)
                    try {
                        for (int i = 0; i < message.length(); i++) { // цикл по символам сообщения
                            System.out.print(message.charAt(i)); // посимвольный вывод

                            Thread.sleep(100); // задержка на 500 миллисекунд

                        }
                        System.out.println();
                        if (shouldStop) {
                            threads.remove(this);
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
}