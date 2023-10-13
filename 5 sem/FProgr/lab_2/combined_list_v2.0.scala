object Main22 {
  def main(args: Array[String]): Unit = {
    def concatListsRecursively(list1: List[Int], list2: List[Int]): List[Int] = {
      def appendRecursively(lst: List[Int], acc: List[Int]): List[Int] = {
        if (lst.isEmpty) {
          acc
        } else {
          appendRecursively(lst.tail, acc :+ lst.head)
        }
      }

      var result = list2
      for (item <- list1) {
        result = appendRecursively(List(item), result)
      }
      result
    }

    def countElements(list: List[Int]): Int = {
      if (list.isEmpty) {
        0
      } else {
        1 + countElements(list.tail)
      }
    }

    val list1 = List(1, 2, 3)
    val list2 = List(4, 5, 6)

    if (list1.isEmpty && list2.isEmpty) {
      println("Оба списка пусты")
    } else if (list1.isEmpty) {
      println("Первый список пуст")
    } else if (list2.isEmpty) {
      println("Второй список пуст")
    } else {
      val concatenatedList = concatListsRecursively(list2, list1)

      println(s"Объединенный список: $concatenatedList")
      val elementCount = countElements(concatenatedList)
      println(s"Количество элементов в объединенном списке: $elementCount")
    }
  }
}
