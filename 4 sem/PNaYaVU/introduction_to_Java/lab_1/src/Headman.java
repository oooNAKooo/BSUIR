package lab_2;

import java.util.List;
public class Headman extends Student {
    public void checkGroup(List<String> group)
    {
        if(group.size()<3)
        {
            int missing = 3- group.size();
            System.out.println("Список сделан. Отсутствует(-ют) "+missing+" человек(-а).");
        }
        else
            System.out.println("Список сделан. Все на месте.");
    }
}
