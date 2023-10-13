object Main {
  def main(args: Array[String]): Unit = {
    // Задача 1: Удаление пробелов
    val textWithSpaces = "Это какой-то текст с пробелами между словами"
    val textWithoutSpaces = textWithSpaces.replaceAll(" ", "") // "\\s" = " "
    println(s"Задача 1: $textWithoutSpaces")

    // Задача 2: Добавление буквы 't'
    val text = "mother"
    val modifiedText = text.replace("t", "tttt")
    print("Задача 2: ")
    println(s"Исходное слово: $text")
    println(s"Модифицированное слово: $modifiedText")
  }
}
