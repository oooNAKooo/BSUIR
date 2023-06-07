package lab_2;

import java.io.IOException; //отлов ошибок при вводе
import java.util.Arrays;
import java.util.Random;
import java.util.Scanner; //ввод с клавы
import java.util.List;
public class Main {
    private static Scanner in = new Scanner(System.in);//спец.класс для работы с вводом
    public static void main(String[] args) throws IOException, InterruptedException {
        {
            String teach, stud, les;
            String newTeach, newStud, newLes;
            String topic = "ОАиП";
            int i=0, j=0, k=0;
            int  mainSolution, solutionColledge, solutionStudent, solutionJournal, solutionJournalLine;
            int choice;
            boolean  x=true;

            Colledge BSUIR = new Colledge();
            Student aStudent = new Student(BSUIR.student);
            Teacher aTeacher = new Teacher(BSUIR.teacher);
            Lessons aTopic = new Lessons(BSUIR.lessons);
            Headman aLeader = new Headman();
            Journal aBook = new Journal();
            JournalLine aLine = new JournalLine();
            BSUIR.student.add("Петя Петров");
            BSUIR.teacher.add("Саша Глоба");
            BSUIR.lessons.add("ПНаЯВУ");

            while(x){
                System.out.println("Работа с:\n[1]Университет\n[2]Студент\n[3]Преподаватель\n[4]Лекция\n[5]Староста\n[0]Выход");

                mainSolution = in.nextInt();
                boolean u=true, v =true, w=true, y=true, z=true;

                switch (mainSolution) {
                    case 1:
                        while(y) {
                            System.out.println("[1]Добавить студента\n[2]Добавить преподавателя\n[3]Добавить лекцию\n[0]Назад");
                            solutionColledge = in.nextInt();

                            switch (solutionColledge) {
                                case 1:
                                    BSUIR.inviteStudent("noNameStudent_№" + i);
                                    i++;
                                    break;
                                case 2:
                                    BSUIR.inviteTeacher("noNameTeacher_№" + j);
                                    j++;
                                    break;
                                case 3:
                                    BSUIR.newLessons("noNameLesson_№" + k);
                                    k++;
                                    break;
                                case 0:
                                    y = false;
                                    break;
                                default:
                                    System.out.println("Выбран не верный пункт!");
                            }
                        }break;
                    case 2:
                        while(z){
                            System.out.println("[1]Добавить оценку\n[2]Посетить лекцию\n[0]Назад");
                            solutionStudent=in.nextInt();
                            switch(solutionStudent)
                            {
                                case 1:
                                    aStudent.marks.add(1+(int)(Math.random()*10));
                                    aStudent.checkMarks();
                                    break;
                                case 2:
                                    aStudent.listenLesson(BSUIR.student);
                                    break;
                                case 0:
                                    z = false;
                                    break;
                                default:
                                    System.out.println("Выбран не верный пункт!");
                            }
                        }break;
                    case 3:
                        while(w) {
                            System.out.println("[1]Прочитать лекцию\n[2]Отметить присутствующих\n[0]Назад");
                            solutionStudent = in.nextInt();
                            switch (solutionStudent) {
                                case 1:
                                    aTeacher.readLecture(topic);
                                    break;
                                case 2:
                                    aTeacher.checkStudent(BSUIR.student);
                                    break;
                                case 0:
                                    w = false;
                                    break;
                                default:
                                    System.out.println("Выбран не верный пункт!");
                            }
                        }break;
                    case 4:
                        aTopic.startLessons(BSUIR.lessons);
                        choice=in.nextInt();
                        topic = aTopic.decide(BSUIR.lessons, choice);
                        break;
                    case 5:
                        aLeader.checkGroup(BSUIR.student);
                        break;
                    case 6:
                        while(v){
                            System.out.println("[1]Отметить студента\n[2]Показать список группы\n[0]Назад");
                            solutionJournal = in.nextInt();
                            switch (solutionJournal) {
                                case 1:
                                    aTeacher.readLecture(topic);
                                    break;
                                case 2:
                                    aTeacher.checkStudent(BSUIR.student);
                                    break;
                                case 0:
                                    v = false;
                                    break;
                                default:
                                    System.out.println("Выбран не верный пункт!");
                            }
                        }break;
                    case 7:
                        while(u){
                            System.out.println("[1]Отметить отсутствие\n[2]Отметить присутствие\n[0]Назад");
                            solutionJournalLine = in.nextInt();
                            switch (solutionJournalLine) {
                                case 1:
                                    aBook.tagStudent(BSUIR.student);
                                    break;
                                case 2:
                                    aTeacher.checkStudent(BSUIR.student);
                                    break;
                                case 0:
                                    u = false;
                                    break;
                                default:
                                    System.out.println("Выбран не верный пункт!");
                            }
                        }break;
                    case 0:
                        x=false;
                        System.out.println("До свидания!");
                        break;
                    default:
                        System.out.println("Выбран не верный пункт!");
                }
            }
        }
    }
}