object Main2 {

  private def sumList(): List[Int] = {
    println("Введите количество элементов списка:")
    val count = scala.io.StdIn.readInt() // Считать количество элементов
    var inputValues: List[Int] = Nil // создание пустого листа
    while (inputValues.length < count) {
      println(s"Введите элемент №${inputValues.length + 1}:")
      val inputValue = scala.io.StdIn.readInt() // Считать элемент с клавиатуры
      inputValues = inputValues :+ inputValue // Добавить элемент в список
    }
    if (inputValues.nonEmpty) {
      println("Весь список: " + inputValues)
      println("Сумма чисел не более 10: ")
      val sum = inputValues.filter(_<10).sum // фильтрация элементов <10
      println(sum)
      inputValues
    }
    else{
      println("Список пуст")
      Nil // возврат пустого списка
    }
  }
  
  private def sumThreeFirst():List[Int] = {
    var inputValues: List[Int] = Nil // создание пустого листа
    var count = 0
   do {
     println("Введите количество элементов списка:")
      count = scala.io.StdIn.readInt() // Считать количество элементов
     if(count <3) println("Для данного задания требуется 3 или более элемента!")
   }while(count<3)
    while (inputValues.length < count) {
      println(s"Введите элемент №${inputValues.length + 1}:")
      val inputValue = scala.io.StdIn.readInt() // Считать элемент с клавиатуры
      inputValues = inputValues :+ inputValue // Добавить элемент в список
    }
    if (inputValues.nonEmpty) {
      println("Весь список: " + inputValues)
      println("Сумма первых 3 элементов: ")
      val sum = inputValues.take(3).foldLeft(0)((ac_c, x)=>ac_c+x) // первые 3
      println(sum)
      inputValues
    }
    else {
      println("Список пуст")
      Nil // возврат пустого списка
    }
  }

  def main(args: Array[String]): Unit = {
    while (true) {
      print("Сделайте выбор: \n")
      println("1. Сумма чисел списка не более 10;")
      println("2. Сумма первых 3 элементов списка;")
      println("3. Поиск мин/макс элемента списка;")
      println("4. Проверка списка на возрастание;")
      println("5. Проверка на наличие одинаковых элементов в списке;")
      println("0. Выход.")
      var choose = ""
      choose = scala.io.StdIn.readLine()
      choose match {
        case "1" => sumList()
        case "2" => sumThreeFirst()
        //  case "3" => findMinMax()
        // case "4" => listUp()
        //case "5" => sameInList()
        case "0" => {println("Завершение программы.")
          System.exit(0)}
        case _ => println("Параметр не найден!")
      }
      var continueChoice = ""
      while (continueChoice != "1" && continueChoice != "0") {
        println("Желаете выполнить еще операцию? (1 - да, 0 - выход):")
        continueChoice = scala.io.StdIn.readLine()

        if (continueChoice != "1" && continueChoice != "0") {
          println("Некорректный выбор. Пожалуйста, введите 1 или 0.")
        }
      }
      if (continueChoice != "1") {
        println("Выход из программы.")
        System.exit(0)
      }
    }
  }
}
