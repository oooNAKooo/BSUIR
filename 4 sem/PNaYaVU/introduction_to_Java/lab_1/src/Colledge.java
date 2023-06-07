package lab_2;

import java.util.ArrayList;
import java.util.List;
public class Colledge {
    public Colledge(){};
    List<String> student = new ArrayList<>();
    List<String> teacher = new ArrayList<>();
    List<String> lessons = new ArrayList<>();
    public void inviteStudent(String newStud){
        student.add(newStud);
        System.out.println("Новый список студентов: "+student );}
    public void inviteTeacher(String newTeach){
        teacher.add(newTeach);
        System.out.println("Новый список преподавателей: "+teacher );}
    public void newLessons(String newLes){
        lessons.add(newLes);
        System.out.println("Новый список лекций: "+lessons );}
}




