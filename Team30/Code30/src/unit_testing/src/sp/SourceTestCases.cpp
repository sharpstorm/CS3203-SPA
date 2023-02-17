#include <string>

// Simple programs for grammar testing
class SourceTestCases {
 public:
  SourceTestCases() {}

  const std::string EMPTY =
      "procedure empty {"
      "}";

  const std::string SIMPLE_PROGRAM =
      "procedure simple {"
      "read num1;"
      "num1 = num1 + 1;"
      "print num1;"
      "}";

  const std::string ONE_LINE =
      "procedure jammed {"
      "read num1;read num2;num3 = num1 + num2;print num3;"
      "}";

  const std::string SAME_NAMES =
      "procedure procedure {"
      "read read;"
      "read print;"
      "print read;"
      "print print;"
      "}";

  const std::string CASE_SENSITIVE =
      "procedure if {"
      "read num;"
      "read Num;"
      "print num;"
      "print Num;"
      "}";

  const std::string LONG_NAMES =
      "procedure arbitrarilylongprocedurename {"
      "read someVeryArbitrarilyLongVariableName;"
      "print someVeryArbitrarilyLongVariableName;"
      "}";

  const std::string WHITESPACE =
      "procedure white{"
      "num1 =          1;"
      "num2=\n2;"
      "num3 =3;"
      "newNum= num1+          num2+num3;"
      "print \tnewNum;"
      "}";

  const std::string TEST_ASSIGN =
      "procedure assign {"
      "num1 = 0;"
      "num2 = num1;"
      "num1 = num1;"
      "print num1;"
      "}";

  const std::string ASSIGN_TO_SELF =
      "procedure if {"
      "read num;"
      "num = num / 2;"
      "print num;"
      "}";

  const std::string TEST_BASIC_PROCEDURE =  // from sample code 1 in CS3203
                                            // website
      "procedure average {"
      "read num1;"
      "read num2;"
      "read num3;"
      "sum = num1 + num2 + num3;"
      "ave = sum / 3;"
      "print ave;"
      "}";

  const std::string TEST_OPERATORS =
      "procedure operators {"
      "num1 = 1 + 2 + 3 * 4 / 5;"
      "num2 = 5 * 5 - 5 / (4*3);"
      "num3 = num1+num2+1;"
      "print num3;"
      "}";

  const std::string CALL_PROCEDURES =
      "procedure progOne {"
      "call progTwo;"
      "}"
      "procedure progTwo {"
      "call progThree;"
      "}"
      "procedure progThree {"
      "num = 1;"
      "print num;"
      "}";

  const std::string CALL_CASE_SENSITIVE =
      "procedure main {"
      "call mAin;"
      "}"
      "procedure mAin {"
      "call mAIN;"
      "print num;"
      "}"
      "procedure mAIN {"
      "num = 1;"
      "}";

  // Programs for testing simple if/else cases
  const std::string SIMPLE_IF =
      "procedure if {"
      "read num;"
      "if (num==0) then {"
      "  print num;"
      "} else {"
      "}}";

  const std::string ONE_LINE_IF_STMT =
      "procedure longif {"
      "read num;"
      "if (num==0) then {  print num;} else {}}";

  const std::string SIMPLE_IF_ELSE =
      "procedure ifElse {"
      "read num;"
      "read num2;"
      "if (num==0) then {"
      "  print num;"
      "} else {"
      "  print num2;"
      "}}";

  const std::string EMPTY_IF =
      "procedure if {"
      "num = 0;"
      "if (num==0) then {"
      "} else {"
      "}}";

  const std::string TEST_CONDITIONAL =
      "procedure if {"
      "read num1;"
      "read num2;"
      "read num3;"
      "if (((num1 == num2) || (num2 == num3)) && (num1 != num3)) then {"
      "  print num1;"
      "} else {"
      "}}";

  const std::string TEST_RELATIONAL =
      "procedure if {"
      "read num1;"
      "read num2;"
      "read num3;"
      "read num4;"
      "if ((num1 <= num4) && (num3 > num4)) then {"
      "  print num1;"
      "} else {"
      "}}";

  const std::string CHAINED_IF =
      "procedure chainedIf {"
      "read num;"
      "read num2;"
      "if (num>0) then {"
      "  print num;"
      "  if (num>5) then {"
      "    print num2;"
      "    if (num2<10) then {"
      "      num = num + 1;"
      "    } else {}"
      "  } else {}"
      "} else {}"
      "}";

  const std::string MULTIPLE_IF_ELSE =  // for follows relations
      "procedure ifElse {"
      "read num;"
      "read num2;"
      "num3 = num + num2;"
      "if (num==0) then {"
      "  print num;"
      "} else {"
      "  print num2;"
      "} if (num2==0) then {"
      "  print num2;"
      "} else {"
      "  print num3;"
      "} if (num3>5) then {"
      "  print num3;"
      "} else {"
      "  print num2;"
      "}}";

  const std::string CHAINED_IF_ELSE =
      "procedure ifElse {"
      "read num;"
      "read num2;"
      "if (num!=0) then {"
      "  print num;"
      "  if (num == 1) then {"
      "    print num;"
      "   } else {}"
      "} else {"
      "    if (num2 == 1) then {"
      "      print num2;"
      "     } else {}"
      "    print num2;"
      "}}";

  const std::string MULTI_CHAINED_IF_ELSE =
      "procedure ifElse {"
      "read num;"
      "read num2;"
      "if (num!=0) then {"
      "  print num;"
      "  if (num == 1) then {"
      "    print num;"
      "  } else {"
      "    if (num == 2) then {"
      "      print num;"
      "    } else {}"
      "}} else {"
      "    if (num2 == 1) then {"
      "      print num2;"
      "    } else {"
      "      if (num2 == 2) then {"
      "        print num2;"
      "       } else {}"
      "    } print num2;"
      "}}";

  const std::string CALL_IN_IF =
      "procedure procOne {"
      "read num;"
      "if (num>=0) then {"
      "  call procTwo;"
      "} else {"
      "  call procThree;"
      "}}"
      "procedure procTwo{ "
      "print num;"
      "}"
      "procedure procThree{ "
      "newNum = num + 1;"
      "print newNum;"
      "}";

  // Programs testing simple while cases

  const std::string SIMPLE_WHILE =
      "procedure main {"
      "num = 0;"
      "while (num <= 5) {"
      "  print num;"
      "  num = num + 1;"
      "}}";

  const std::string WHILE_WOUT_WHITESPACE =
      "procedure main {"
      "num = 0;"
      "while(num <= 5){"
      "print num;"
      "num =num+1;"
      "}}";

  const std::string MULTIPLE_WHILE =
      "procedure main {"
      "num = 0;"
      "while (num <= 5) {"
      "  print num;"
      "  num = num + 1;"
      "} while (num <= 10) {"
      "  print num;"
      "  num = num + 1;"
      "} while (num <= 15) {"
      "  print num;"
      "  num = num + 1;"
      "}}";

  const std::string EMPTY_WHILE =
      "procedure main {"
      "num = 0;"
      "while (num != 0) {"
      "}}";

  const std::string WHILE_INBETWEEN_STATEMENTS =
      "procedure main {"
      "num = 0;"
      "while (num <= 5) {"
      "  num = num + 1;"
      "}"
      "print num;"
      "}";

  const std::string CHAINED_WHILE =
      "procedure main {"
      "num1 = 0;"
      "num2 = 5;"
      "while (num1 <= 5) {"
      "  while (num2 >= 0) {"
      "    num2 = num2 - 1;"
      "    print num2;"
      "  } num2 = 5;"
      "  num1 = num1 + 1;"
      "}}";

  const std::string MULTI_CHAINED_WHILE =
      "procedure main {"
      "num1 = 0;"
      "num2 = 5;"
      "num3 = 0;"
      "while (num1 <= 5) {"
      "  while (num2 >= 0) {"
      "    while (num3 > 3) {"
      "      num3 = num3 - 1;"
      "        print num3;"
      "    } num3 = 0;"
      "    num2 = num2 - 1;"
      "    print num2;"
      "  } num2 = 5;"
      "  num1 = num1 + 1;"
      "}}";

  const std::string CALL_IN_WHILE =
      "procedure main {"
      "num = 0;"
      "while (num != 5) {"
      "  call proc;"
      "}}"
      "procedure proc {"
      "print num;"
      "}";

  // programs with while and if/else chains

  const std::string IF_IN_WHILE =
      "procedure main {"
      "read num;"
      "while (num >= 0) {"
      "  numCheck = num % 2;"
      "  if (numCheck == 0) then {"
      "    print num;"
      "  } else {"
      "  } num = num - 1;"
      "}}";

  const std::string WHILE_IN_IF =
      "procedure main {"
      "read num;"
      "if (num > 0) then {"
      "  while (num > 0) {"
      "    num = num - 1;"
      "  }"
      "} else {"
      "}}";

  const std::string WHILE_IN_IF_ELSE =
      "procedure main {"
      "read num;"
      "if (num > 0) then {"
      "  while (num > 0) {"
      "    num = num - 1;"
      "  }"
      "} else {"
      "  while (num <= 0) {"
      "    num = num + 1;"
      "  }"
      "}}";

  const std::string MULTIPLE_WHILE_IN_IF_ELSE =
      "procedure main {"
      "read num;"
      "if (num > 0) then {"
      "  while (num > 0) {"
      "    num = num - 1;"
      "  }"
      "} else {"
      "  while (num <= 0) {"
      "    num = num + 1;"
      "  }"
      "} num = 0;"
      "if (num > 0) then {"
      "  while (num > 0) {"
      "    num = num - 1;"
      "  }"
      "} else {"
      "  while (num <= 0) {"
      "    num = num + 1;"
      "  }"
      "}}";

  const std::string WHILE_CHAIN_IN_IF =
      "procedure main {"
      "read num;"
      "read numOne;"
      "if (num > 0) then {"
      "  while (num > 0) {"
      "    while (numOne >= 0) {"
      "      numOne = numOne - num;"
      "    } num = num - 1;"
      "  }"
      "} else {"
      "}}";

  const std::string WHILE_CHAIN_IN_ELSE =
      "procedure main {"
      "read num;"
      "read numOne;"
      "if (num < 0) then {"
      "} else {"
      "  while (num > 0) {"
      "    while (numOne >= 0) {"
      "      numOne = numOne - num;"
      "    } num = num - 1;"
      "  }"
      "}}";

  const std::string WHILE_CHAIN_IN_IF_ELSE =
      "procedure main {"
      "read num;"
      "read numOne;"
      "if (num > 0) then {"
      "  while (num > 0) {"
      "    while (numOne >= 0) {"
      "      numOne = numOne - num;"
      "    }} num = num - 1;"
      "} else {"
      "  while (num < 0) {"
      "    while (numOne <= 0) {"
      "      numOne = numOne + num;"
      "    } num = num + 1;"
      "  }"
      "}}";

  const std::string IF_ELSE_SINGLE_CALL_IN_WHILE =
      "procedure main {"
      "read num;"
      "while (num >= 0) {"
      "  numCheck = num % 2;"
      "  if (numCheck == 0) then {"
      "    call calls;"
      "  } else {"
      "  } num = num - 1;"
      "}}"
      "procedure calls {"
      "print num;"
      "}";

  const std::string MULTIPLE_CALLS_IN_WHILE =
      "procedure main {"
      "read num;"
      "while (num >= 0) {"
      "  call numCheck"
      "  if (numCheck == 0) then {"
      "    call prints;"
      "  } else {"
      "  } call decrements;"
      "}}"
      "procedure numCheck {"
      "numCheck = num % 2;"
      "}"
      "procedure decrements {"
      "num = num - 1;"
      "}"
      "procedure prints {"
      "print num;"
      "}";

  const std::string CHAINED_WHILE_WITH_IF_IN_BODY =
      "procedure main {"
      "num1 = 0;"
      "num2 = 5;"
      "while (num1 <= 5) {"
      "  while (num2 >= 0) {"
      "    if (num2 == 0) then {"
      "      print num1;"
      "    } else {"
      "    print num2;"
      "    } num2 = num2 - 1;"
      "  } num2 = 5;"
      "  num1 = num1 + 1;"
      "}}";

  const std::string MULTIPLE_IF_ELSE_WITH_WHILE_CHAIN =  // multi-layer
                                                         // stress test
      "procedure ifElse {"
      "read num1;"
      "read num2;"
      "num3 = num + num2;"
      "num4 = 10;"
      "if (num1 == 0) then {"
      "  print num1;"
      "} else {"
      "  print num2;"
      "} if (num2==0) then {"
      "  while (num1 <= 5) {"
      "    while (num2 >= 0) {"
      "      while (num4 > 3) {"
      "        num4 = num4 - 1;"
      "          print num4;"
      "      } num4 = 0;"
      "      num2 = num2 - 1;"
      "      print num2;"
      "    } num2 = 5;"
      "    num1 = num1 + 1;"
      "}} else {"
      "  print num3;"
      "} if (num3>5) then {"
      "  print num3;"
      "} else {"
      "  print num2;"
      "}}";

  // Complex programs (derived from samples in CS3203 website)

  const std::string COMPLEX_PROGRAM_ONE =  // from sample code 2
                                           // in CS3203 website
      "procedure printAscending {"
      "read num1;"
      "read num2;"
      "noSwap = 0;"

      "if (num1 > num2) then {"
      "temp = num1;"
      "num1 = num2;"
      "num2 = temp;"
      "}"
      "else {"
      "noSwap = 1;"
      "}"

      "print num1;"
      "print num2;"
      "}";

  const std::string COMPLEX_PROGRAM_TWO =  // from sample code 3
                                           // in CS3203 website
      "procedure sumDigits {"
      "read number;"
      "sum = 0;"

      "while (number > 0) {"
      "digit = number % 10;"
      "sum = sum + digit;"
      "number = number / 10;"
      "}"

      "print sum;"
      "}";

  const std::string COMPLEX_PROGRAM_THREE =  // from sample code 4
                                             // in CS3203 website
      "procedure main {"
      "flag = 0;"
      "call computeCentroid;"
      "call printResults;"
      "}"
      "procedure readPoint {"
      "read x;"
      "read y;"
      "}"
      "procedure printResults {"
      "print flag;"
      "print cenX;"
      "print cenY;"
      "print normSq;"
      "}"
      "procedure computeCentroid {"
      "count = 0;"
      "cenX = 0;"
      "cenY = 0;"
      "call readPoint;"
      "while ((x != 0) && (y != 0)) {"
      "count = count + 1;"
      "cenX = cenX + x;"
      "cenY = cenY + y;"
      "call readPoint;"
      "}"
      "if (count == 0) then { flag = 1; }"
      "else {"
      "cenX = cenX / count;"
      "cenY = cenY / count;"
      "}"
      "normSq = cenX * cenX + cenY * cenY;"
      "}";
};
