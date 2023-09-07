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

//нечетные
def substringFromSymbolsAtNotEvenPositions(str: String): String = 
{
  val result = new StringBuilder
  for (i <- str.indices if i % 2 != 1) 
  {
    result.append(str(i))
  }
  result.toString()
}

object Main234 
{
//Unit==void
  def main(args: Array[String]): Unit = 
  {
    print("Enter the number: ")
    var numberString = "" //ввод с клавы строки чисел
    var string_a = "" //1-число(четное)
    var string_b = "" //2-число(нечетное)
    var string_c = 0 //1*2=С
    var rez: Float = 0.0 //с/1000=рез
    var z = 0 // сумма всех С
    numberString = scala.io.StdIn.readLine() // ввод с консоли
    if((numberString.length() < 10)) 
    {
      println("Incorrent number")
      sys.exit(0)
    }
    for(i <- 1 to 5) //for(int i=1; i<5; i++)
    {
      string_a = substringFromSymbolsAtEvenPositions(numberString)
      string_b = substringFromSymbolsAtNotEvenPositions(numberString)
      string_c = string_a.toInt * string_b.toInt //стринг в инт 
      rez = string_c.toString().substring(0,3).toFloat / 1000
      z+=string_c
      println("a = " + string_a)
      println("b = " + string_b)
      println("c = " + string_c)
      println("rez = " + rez)
      println("z = " + z)
      numberString = numberString.substring(1,numberString.length()-1) 
      numberString = "0" + z //добавляем в начло, чтобы не было -
    }
  }
}

//