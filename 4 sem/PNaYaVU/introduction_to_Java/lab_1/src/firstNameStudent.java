package lab_2;
public enum firstNameStudent {
    Andrew("Андрей"),
    Nikita("Никита"),
    Vlad("Влад"),
    Anya("Аня"),;

    private String translate;
    firstNameStudent(String translate) {
        this.translate = translate;
    }

    public String toString() {
        return translate +' ';
    }
}