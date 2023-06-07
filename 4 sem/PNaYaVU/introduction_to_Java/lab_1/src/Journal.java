package lab_2;

import java.util.List;

public class Journal {
    public Journal(){};
    public void tagStudent(List<String> incStud){

        System.out.println("Список группы обновлен!");
    };
    public void listGroup(List<String> listGroup){
        System.out.println(listGroup);
    };
}
