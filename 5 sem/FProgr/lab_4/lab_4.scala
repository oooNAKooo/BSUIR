import org.apache.spark.SparkConf
import org.apache.spark.SparkContext

    val text = sc.textFile("D:\\SCALA_TEXT.txt")

    val stopWords = Seq("the", "and", "in", "of", "to", "a", "for") // Пример списка stop-слов
    val filteredWords = text.flatMap(line => line.split("\\s")).filter(word => !stopWords.contains(word))
    println("1. Слова, исключая stop-слова:" + stopWords)
    filteredWords.collect().foreach(println)

    val wordsWithT = text.flatMap(line => line.split("\\s")).filter(word => word.contains("t"))
    println("2. Слова, содержащие букву 't':")
    wordsWithT.collect().foreach(println)

    val wordsEndingWithIng = text.flatMap(line => line.split("\\s")).filter(word => word.endsWith("ing"))
    println("3. Слова, заканчивающиеся на 'ing':")
    wordsEndingWithIng.collect().foreach(println)

    val wordsWithSecondLetterA = text.flatMap(line => line.split("\\s")).filter(word => word.length > 1 && word(1) == 'a')
    println("4. Слова, вторая буква которых 'a':")
    wordsWithSecondLetterA.collect().foreach(println)

    val wordsWithLastLetterS = text.flatMap(line => line.split("\\s")).filter(word => word.endsWith("s"))
    println("5. Слова, последняя буква которых 's':")
    wordsWithLastLetterS.collect().foreach(println)

    val everySecondWord = text.flatMap(line => line.split("\\s")).zipWithIndex.filter { case (_, index) => index % 2 == 1 }.map(_._1)
    println("6. Каждое второе слово:")
    everySecondWord.collect().foreach(println)

    sc.stop()
