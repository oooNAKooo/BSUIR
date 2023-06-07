package lab_2;

import java.util.ArrayList;
import java.util.List;
public class Lessons {
    List<String> countLessons=new ArrayList<>();
    public Lessons(List<String> listLessons){
        this.countLessons=listLessons;
    }
    public void startLessons(List<String> newLes){
        System.out.println(newLes);
        System.out.println("Выберите тему лекции: ");}
    public String decide(List<String> newLes, int choice){
        if(choice<=0||choice>newLes.size())
            System.out.println("Такой лекции нет!");
        else{
            choice--;
            System.out.println("Сегодня будет зачитана лекция по ["+newLes.get(choice)+"]");}
        return newLes.get(choice);
    }
}

