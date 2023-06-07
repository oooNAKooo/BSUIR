import java.util.ArrayList;
import java.util.List;
public class Main {
    private static final int MAX_SPEED = 120; // максимальная скорость машины
    private static final int ALLOWED_SPEED = 80; // разрешенная скорость на дороге
    private static final int REPORT_INTERVAL = 50; // интервал вывода отчета
    private static final int NUM_THREADS = 10; // количество машин на дороге
    public static void main(String[] args) {
        List<Car> cars = new ArrayList<>();
        for (int i = 0; i < NUM_THREADS; i++) {
            Car car = new Car(MAX_SPEED, ALLOWED_SPEED);
            car.start();
            cars.add(car);
        }
        int totalViolations = 0;
        int maxSpeed = 0;
        for (int i = 0; i < cars.size(); i++) {
            Car car = cars.get(i);
            try {
                car.join(); // ждем завершения потока
                if (car.getSpeed() > maxSpeed) {
                    maxSpeed = car.getSpeed();
                }
                if (car.isViolation()) {
                    totalViolations++;
                }
            } catch (InterruptedException e) {
                e.printStackTrace(); // определение ошибки
            }
            if ((i + 1) % REPORT_INTERVAL == 0) {
                System.out.println("Отчёт полосы №" + ((i + 1) / REPORT_INTERVAL) +
                        ": максимальная скорость на полосе: " + maxSpeed + "; нарушений: " + totalViolations);
            }
        }
    }
}