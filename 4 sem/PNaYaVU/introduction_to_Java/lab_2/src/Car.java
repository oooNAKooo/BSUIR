import java.util.Random;
public class Car extends Thread{
    private int speed;
    private int MAX_SPEED;
    private int ALLOWED_SPEED;
    private boolean violation;
    private static final Random random = new Random();
    Car(int MAX_SPEED,int ALLOWED_SPEED){
        this.MAX_SPEED=MAX_SPEED;
        this.ALLOWED_SPEED=ALLOWED_SPEED;
    }
    public int getSpeed() {
        return speed;
    }
    public boolean isViolation() {
        return violation;
    }

    @Override
    public void run() {
        speed = random.nextInt(MAX_SPEED);
        try {
            Thread.sleep(1000); // машина убирается с дороги через 1 секунду
        } catch (InterruptedException e) {
            e.printStackTrace(); // определение ошибки
        }
        if (speed > ALLOWED_SPEED) {
            violation = true;
            System.out.println("Машина со скоростью " + speed + " снята с дороги за нарушение.");
        } else {
            System.out.println("Машина со скоростью " + speed + " снята с дороги без нарушения.");
        }
    }
}
