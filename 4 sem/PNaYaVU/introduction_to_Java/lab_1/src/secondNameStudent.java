package lab_2;
public enum secondNameStudent {
    Smith("Смит"),
    Miller("Миллер"),
    Ivanov("Иванов"),
    Semenov("Семенов"),;

    private String translate;
    secondNameStudent(String translate) {
        this.translate = translate;
    }

    public String toString() {
        return translate;
    }
}