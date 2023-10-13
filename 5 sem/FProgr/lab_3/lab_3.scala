object TextManipulation {
  def main(args: Array[String]): Unit = {
    val text1 = "Hello to everybody"
    val text2 = "When executing the exercise extract all extra words"
    val text3 = "A big round ball fall to the ground"
    val text4 = "A big round ball fall to the ground"
    val text5 = "Hello to everybody"
    val text6 = "Hello to everybody"
    val text7 = "Hello to everybody"
    val text8 = "Hello to everybody"

    // Задача 1: Заменить латинские буквы на русские или цифры
    val replacedText1 = text1.map {
      case c if c.isLetter && c <= 'z' => ('а' + (c - 'a')).toChar
      case c if c.isLetter && c <= 'Z' => ('А' + (c - 'A')).toChar
      case c => c
    }
    println(s"Задача 1: $replacedText1")

    // Задача 2: Найти слова, начинающиеся на 'ext'
    val extWords = "\\bext\\w*\\b".r.findAllIn(text2).toList
    println(s"Задача 2: $extWords")

    // Задача 3: Заменить 'the' на 'a'
    val replacedText3 = text3.replaceAll("the", "a")
    println(s"Задача 3: $replacedText3")

    // Задача 4: Записать слова в обратном порядке
    val reversedWords = text4.split("\\s+").reverse.mkString(" ")
    println(s"Задача 4: $reversedWords")

    // Задача 5: Убрать все гласные
    val withoutVowels = text5.replaceAll("[aeiouyAEIOU]", "")
    println(s"Задача 5: $withoutVowels")

    // Задача 6: Удвоить каждую букву в слове
    val doubledLetters = text6.split("\\s+").map(word => word.map(c => c.toString * 2).mkString(" ")).mkString(" ")
    println(s"Задача 6: $doubledLetters")

    // Задача 7: Удалить все вхождения буквы 'y'
    val withoutY = text7.replaceAll("y", "")
    println(s"Задача 7: $withoutY")

    // Задача 8: Вставить слова 'with heartness'
    val withHeartness = text8.replace("to", "with heartness to")
    println(s"Задача 8: $withHeartness")
  }
}
