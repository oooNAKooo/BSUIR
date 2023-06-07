package lab_2;

import java.util.ArrayList;
import java.util.List;

public class JournalLine {
    List<String> nameStudent=new ArrayList<>();
    List<String> whoCame = new ArrayList<>();
    public JournalLine(List<String> nameStud, List<String> whoC)
    {
        this.nameStudent=nameStud;
        this.whoCame=whoC;
    }
    public JournalLine(){};

}
