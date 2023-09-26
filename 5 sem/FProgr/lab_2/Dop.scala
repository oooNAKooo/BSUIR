object Main2 {
  private def sumList(): List[Int] = {
    println("Введите количество элементов списка:")
    val count = scala.io.StdIn.readInt()
    def inputValuesRecursion(i: Int, acc: List[Int]): List[Int] = {
      if (i <= count) {
        println(s"Введите элемент №$i:")
        val inputValue = scala.io.StdIn.readInt()
        inputValuesRecursion(i + 1, inputValue :: acc)
      } else {
        acc.reverse
      }
    }
    val inputValues = inputValuesRecursion(1, Nil)
    if (inputValues.nonEmpty) {
      println("Весь список: " + inputValues)
      println("Сумма чисел не более 10: ")
      def sumRecursion(values: List[Int], acc: Int): Int = {
        values match {
          case Nil => acc
          case head :: tail =>
            if (head < 10) {
              sumRecursion(tail, acc + head)
            } else {
              sumRecursion(tail, acc)
            }
        }
      }
      val sum = sumRecursion(inputValues, 0)
      println(sum)
      inputValues
    } else {
      println("Список пуст")
      Nil
    }
  }

  private def sumThreeFirst(): List[Int] = {
    println("Введите количество элементов списка (не менее 3):")
    val count = scala.io.StdIn.readInt()
    if (count < 3) {
      println("Для данного задания требуется 3 или более элемента!")
      return Nil
    }
    val inputValues = (1 to count).map { i =>
      println(s"Введите элемент №$i:")
      scala.io.StdIn.readInt()
    }.toList
    if (inputValues.nonEmpty) {
      println("Весь список: " + inputValues)
      println("Сумма первых 3 элементов: ")
      var sum = 0
      for (i <- 0 until 3) {
        if (i < inputValues.length) {
          sum += inputValues(i)
        }
      }
      println(sum)
      inputValues
    } else {
      println("Список пуст")
      Nil
    }
  }

  private def findMaxOrMinIndex(): Unit = {
    println("Введите количество элементов списка:")
    val count = scala.io.StdIn.readInt()
    val inputValues = (1 to count).map { i =>
      println(s"Введите элемент №$i:")
      scala.io.StdIn.readInt()
    }.toList
    if (inputValues.nonEmpty) {
      println("Весь список: " + inputValues)
      println("Выберите операцию:")
      println("1. Поиск индекса максимального элемента")
      println("2. Поиск индекса минимального элемента")
      val choice = scala.io.StdIn.readInt()
      val result = choice match {
        case 1 =>
          val (maxElement, maxIndex) = findMax(inputValues, 0, 0, inputValues.head, 0)
          (maxElement, maxIndex + 1)
        case 2 =>
          val (minElement, minIndex) = findMin(inputValues, 0, 0, inputValues.head, 0)
          (minElement, minIndex + 1)
        case _ =>
          println("Некорректный выбор.")
          return
      }
      val (extremeValue, extremeIndex) = result
      println(s"Индекс искомого элемента ($extremeValue): $extremeIndex")
    } else {
      println("Список пуст")
    }
  }

  private def findMax(list: List[Int], currentIndex: Int, maxIndex: Int, currentMax: Int, globalMax: Int): (Int, Int) = {
    if (currentIndex < list.length) {
      val currentValue = list(currentIndex)
      if (currentValue > currentMax) {
        findMax(list, currentIndex + 1, currentIndex, currentValue, globalMax)
      } else {
        findMax(list, currentIndex + 1, maxIndex, currentMax, globalMax)
      }
    } else {
      if (currentMax > globalMax) {
        (currentMax, maxIndex)
      } else {
        (globalMax, maxIndex)
      }
    }
  }

  private def findMin(list: List[Int], currentIndex: Int, minIndex: Int, currentMin: Int, globalMin: Int): (Int, Int) = {
    if (currentIndex < list.length) {
      val currentValue = list(currentIndex)
      if (currentValue < currentMin) {
        findMin(list, currentIndex + 1, currentIndex, currentValue, globalMin)
      } else {
        findMin(list, currentIndex + 1, minIndex, currentMin, globalMin)
      }
    } else {
      if (currentMin < globalMin) {
        (currentMin, minIndex)
      } else {
        (globalMin, minIndex)
      }
    }
  }

  private def isSortedAsc(list: List[Int]): Boolean = {
    isSortedAscRecursive(list)
  }

  private def isSortedAscRecursive(list: List[Int], index: Int = 0): Boolean = {
    if (index < list.length - 1) {
      if (list(index) <= list(index + 1)) {
        isSortedAscRecursive(list, index + 1)
      } else {
        false
      }
    } else {
      true
    }
  }

  private def sortedList(): Unit = {
    println("Введите количество элементов списка:")
    val count = scala.io.StdIn.readInt()
    val inputValues = (1 to count).map { i =>
      println(s"Введите элемент №$i:")
      scala.io.StdIn.readInt()
    }.toList
    if (inputValues.nonEmpty) {
      println("Весь список: " + inputValues)
      val isSorted = isSortedAsc(inputValues)
      if (isSorted) {
        println("Список упорядочен по возрастанию")
      } else {
        println("Список не упорядочен по возрастанию")
      }
    } else {
      println("Список пуст")
    }
  }

  private def hasDuplicates(list: List[Int]): Boolean = {
    hasDuplicatesRecursive(list, Set.empty)
  }

  private def hasDuplicatesRecursive(list: List[Int], seenSet: Set[Int]): Boolean = {
    list match {
      case Nil => false
      case head :: tail =>
        if (seenSet.contains(head)) {
          true
        } else {
          hasDuplicatesRecursive(tail, seenSet + head)
        }
    }
  }

  private def sameInList(): Unit = {
    println("Введите количество элементов списка:")
    val count = scala.io.StdIn.readInt()
    val inputValues = (1 to count).map { i =>
      println(s"Введите элемент №$i:")
      scala.io.StdIn.readInt()
    }.toList

    if (inputValues.nonEmpty) {
      println("Весь список: " + inputValues)
      val hasDupe = hasDuplicates(inputValues)
      if (hasDupe) {
        println("В списке есть одинаковые элементы")
      } else {
        println("В списке нет одинаковых элементов")
      }
    } else {
      println("Список пуст")
    }
  }

  def main(args: Array[String]): Unit = {
    var continue = true
    while (continue) {
      print("Сделайте выбор: \n")
      println("1. Сумма чисел списка не более 10;")
      println("2. Сумма первых 3 элементов списка;")
      println("3. Поиск индекса максимального или минимального элемента списка;")
      println("4. Проверка списка на упорядоченность по возрастанию;")
      println("5. Проверка на наличие одинаковых элементов в списке;")
      println("0. Выход.")
      scala.io.StdIn.readLine() match {
        case "1" => sumList()
        case "2" => sumThreeFirst()
        case "3" => findMaxOrMinIndex()
        case "4" => sortedList()
        case "5" => sameInList()
        case "0" =>
          println("Завершение программы.")
          continue = false
        case _ => println("Параметр не найден!")
      }
      if (continue) {
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
          continue = false
        }
      }
    }
  }
}
