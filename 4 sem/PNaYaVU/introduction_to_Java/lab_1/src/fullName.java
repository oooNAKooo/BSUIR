package lab_2;

import java.util.List;

public class fullName {
    private List<String>full;
    private List<String> first;
    private List<String>second;

    fullName(List<String> fn, List<String> sn) {
        this.first=fn;
        this.second=sn;
    }
    public String fullStudent(){
        full=first;
        return full.toString();
    }

    public String toString() {return toString();    }
}
