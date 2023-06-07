package lab_2;

import java.util.ArrayList;
import java.util.List;

public class Student {
    private firstNameStudent nameStudent;
    List<String> countStudent = new ArrayList<>();

    public Student(List<String> listStudent) {
        this.countStudent = listStudent;
    }

    public Student() {};

    public firstNameStudent getNameStudent(){
        return nameStudent;
    }

    public void setNameStudent(firstNameStudent nameStudent){
        this.nameStudent=nameStudent;
    }
    List<Integer> marks = new ArrayList<>();


    public void checkMarks() {
        System.out.println("Новый список оценок: " + marks);
    }

    public void listenLesson(List<String> present) {
        System.out.println("Лекция посещена! На ней были: " + present);
    }
}
