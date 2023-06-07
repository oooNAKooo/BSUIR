package lab_2;

import java.util.ArrayList;
import java.util.List;
public class Teacher {
    List<String> countTeacher=new ArrayList<>();
    public Teacher(List<String> listTeacher){
        this.countTeacher=listTeacher;
    }
    public void readLecture(String topic){
        System.out.println("Лекция по "+topic+" прочитана! ");
    };
    public void checkStudent(List<String> truants) { //truants==прогульщики
        if (truants.size() < 5) {
            int count = 5 - truants.size();
            System.out.println("У " + count + " человек будут проблемы со сдачей экзамена");
        } else
            System.out.println("Все на месте. Продолжаем лекцию.");
    }
    public void checkTeacher(){
        System.out.println(countTeacher);
        }

    };

