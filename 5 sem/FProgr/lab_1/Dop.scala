//четные
def substringFromSymbolsAtEvenPositions(str: String): String = 
{
  val result = new StringBuilder // объект
  for (i <- str.indices if i % 2 == 1) 
  {
    result.append(str(i))
  }
  result.toString()
}

object Main22 {
def main(args: Array[String]): Unit = {
   print("Enter the number: ")
   var numberString = ""
   numberString = scala.io.StdIn.readLine() // ввод с консоли
   var string_a = substringFromSymbolsAtEvenPositions(numberString)
   var sum = 0
   string_a = substringFromSymbolsAtEvenPositions(numberString)
   for (digit <- string_a.toString) {
      sum += digit.asDigit
                             }

// Print the sum

   println(sum)
  }
}
