import scala.io.Source

// Чтение текста из файла
val source = Source.fromFile("D:\\TEXT_SCALA_DOP.txt")

// 1. Замена последних 3 букв каждого слова на "ing" 
val regex = "\\b(\\w{1,})\\b".r  
def replaceWithIng(m: scala.util.matching.Regex.Match): String = m.group(1).dropRight(3) + "ing"
val modifiedText = regex.replaceAllIn(text, replaceWithIng _)

// 2. Удаление всех гласных
// 1. Удаление гласных с использованием регулярных выражений
val regex = "[aeiouAEIOU]".r
val modifiedText1 = regex.replaceAllIn(text, "")

// 2. Удаление гласных с использованием функции
def removeVowels(input: String): String = { input.replaceAll("[aeiouAEIOU]", "") }

val modifiedText2 = removeVowels(text)
