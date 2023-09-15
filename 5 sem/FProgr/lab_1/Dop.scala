def substringFromSymbolsAtEvenPositions(str: String): String = 
{
  val result = new StringBuilder
  for (i <- str.indices if (i+1) % 2 == 0) // счет цифр строки, стоящих на четной позиции в этой строке
  {
    result.append(str(i))
  }
  result.toString()
}

object Main22 {
def main(args: Array[String]): Unit = {
   print("Enter the number: ")
   var numberString = ""
   numberString = scala.io.StdIn.readLine()
   var string_a = substringFromSymbolsAtEvenPositions(numberString)
   var sum = 0
   for (digit <- string_a.toString) {
      sum += digit.asDigit
                             }
   println(sum)
  } 
}
