#include <string>

using namespace std;

class SourceTestCases {
 public:
  // Simple programs for grammar testing
  string empty() {
    string procedure =
        "procedure empty {\n"
        "}";
    return procedure;
  }

  string simpleProgram() {
    string procedure =
        "procedure simple {\n"
        "read num1;\n"
        "num1 = num1 + 1;\n"
        "print num1;\n"
        "}";
    return procedure;
  }

  string oneLine() {
    string procedure =
        "procedure jammed {\n"
        "read num1;read num2;num3 = num1 + num2;print num3;\n"
        "}";
    return procedure;
  }

  string sameNames() {
    string procedure =
        "procedure procedure {\n"
        "read read;\n"
        "print read;\n"
        "}";
    return procedure;
  }

  string caseSensitive() {
    string procedure =
        "procedure if {\n"
        "read num;\n"
        "read Num;\n"
        "print num;\n"
        "print Num;\n"
        "}";
    return procedure;
  }

  string longNames() {
    string procedure =
        "procedure arbitrarilylongprocedurename {\n"
        "read someVeryArbitrarilyLongVariableName;\n"
        "print someVeryArbitrarilyLongVariableName;\n"
        "}";
    return procedure;
  }

  string whitespace() {
    string procedure =
        "procedure white{\n"
        "num1 =          1\n"
        "num2=2\n"
        "num3 =3\n"
        "newNum= num1+          num2+num3\n"
        "print newNum;"
        "}";
    return procedure;
  }

  string testAssign() {
    string procedure =
        "procedure assign {\n"
        "num1 = 0;\n"
        "num2 = num1;\n"
        "num1 = num1;\n"
        "print num1;\n"
        "}";
    return procedure;
  }

  string assignToSelf() {
    string procedure =
        "procedure if {\n"
        "read num;\n"
        "num = num / 2;\n"
        "print num;\n"
        "}";
    return procedure;
  }

  string testAverage() {  // from computeAverage in CS3203 website
    string procedure =
        "procedure average {\n"
        "read num1;\n"
        "read num2;\n"
        "read num3;\n"
        "sum = num1 + num2 + num3\n"
        "ave = sum \ 3\n"
        "print ave;\n"
        "}";
    return procedure;
  }

  string testOperaters() {
    string procedure =
        "procedure operators {\n"
        "num1 = 1 + 2 + 3 * 4 / 5;\n"
        "num2 = 5 * 5 - 5 / (4*3);\n"
        "num3 = num1+num2+1\n"
        "print num3;\n"
        "}";
    return procedure;
  }

  string callProcedures() {
    string procedures =
        "procedure progOne {\n"
        "call progTwo;\n"
        "}\n"
        "procedure progTwo {\n"
        "call progThree;\n"
        "print num;\n"
        "}\n"
        "procedure progThree {\n"
        "num = 1;\n"
        "}";
    return procedures;
  }

  string callCaseSensitive() {
    string procedures =
        "procedure main {\n"
        "call mAin;\n"
        "}\n"
        "procedure mAin {\n"
        "call mAIN;\n"
        "print num;\n"
        "}\n"
        "procedure mAIN {\n"
        "num = 1;\n"
        "}";
    return procedures;
  }

  // Programs for testing simple if/else cases
  string simpleIf() {
    string procedure =
        "procedure if {\n"
        "read num;\n"
        "if (num==0) then {\n"
        "  print num;\n"
        "} else {\n"
        "}}";
    return procedure;
  }

  string oneLineIf() {
    string procedure =
        "procedure longif {\n"
        "read num;\n"
        "if (num==0) then {  print num;} else {}}";
    return procedure;
  }

  string simpleIfElse() {
    string procedure =
        "procedure ifElse {\n"
        "read num;\n"
        "read num2;\n"
        "if (num==0) then {\n"
        "  print num;\n"
        "} else {\n"
        "  print num2;\n"
        "}}";
    return procedure;
  }

  string emptyIf() {
    string procedure =
        "procedure if {\n"
        "num = 0;\n"
        "if (num==0) then {\n"
        "} else {\n"
        "}}";
    return procedure;
  }

  string testConditional() {
    string procedure =
        "procedure if {\n"
        "read num1;\n"
        "read num2;\n"
        "read num3;\n"
        "if ((num1 == num2) || ((num2 == num3) && (num1 != num3)) then {\n"
        "  print num1;\n"
        "} else {\n"
        "}}";
    return procedure;
  }

  string testRelational() {
    string procedure =
        "procedure if {\n"
        "read num1;\n"
        "read num2;\n"
        "read num3;\n"
        "read num4;\n"
        "if ((num1 <= num4) && (num3 > num4)) then {\n"
        "  print num1;\n"
        "} else {\n"
        "}}";
    return procedure;
  }

  string chainedIf() {
    string procedure =
        "procedure chainedIf {\n"
        "read num;\n"
        "read num2\n"
        "if (num>0) then {\n"
        "  print num;\n"
        "  if (num>5) then {\n"
        "    print num2;\n"
        "    if ((num1+num2)<10) then {\n"
        "      num3 = num1+num2;\n"
        "      print num3;\n"
        "    } else {\n"
        "  } else {\n"
        "} else {\n"
        "}}}}";
    return procedure;
  }

  string chainedifElse() {
    string procedure =
        "procedure ifElse {\n"
        "read num;\n"
        "read num2;\n"
        "if (num!=0) then {\n"
        "  print num;\n"
        "  if (num == 1) then {\n"
        "    print num;\n"
        "   } else {\n"
        "} else {\n"
        "    if (num2 == 1) then {\n"
        "      print num2;\n"
        "     } else {}\n"
        "    print num2;\n"
        "}}";
    return procedure;
  }

  string callIf() {
    string procedure =
        "procedure procOne {\n"
        "read num;\n"
        "if (num>=0) then {\n"
        "  call procTwo;\n"
        "} else {\n"
        "  call procThree;\n"
        "}}\n"
        "procedure procTwo{ \n"
        "print num;\n"
        "}\n"
        "procedure procThree{ \n"
        "newNum = num + 1;\n"
        "print newNum;\n";
    "}" return procedure;
  }
  // Programs testing simple while cases

  string simpleWhile() {
    string procedure =
        "procedure main {\n"
        "num = 0;\n"
        "while (num <= 5) {\n"
        "  print num;\n"
        "  num = num + 1;\n"
        "}}";
    return procedure;
  }

  string whilewoutWhiteSpace() {
    string procedure =
        "procedure main {\n"
        "num = 0;\n"
        "while(num <= 5){\n"
        "print num;\n"
        "num =num+1;\n"
        "}}";
    return procedure;
  }

  string emptyWhile() {
    string procedure =
        "procedure main {\n"
        "num = 0;\n"
        "while (num != 0) {\n"
        "}}";
    return procedure;
  }

  string whileForTestingParent() {
    string procedure =
        "procedure main {\n"
        "num = 0;\n"
        "while (num <= 5) {\n"
        "  num = num + 1;\n"
        "}\n"
        "print num;\n"
        "}";
    return procedure;
  }

  string chainedWhile() {
    string procedure =
        "procedure main {\n"
        "num1 = 0;\n"
        "num2 = 5;\n"
        "while (num1 <= 5) {\n"
        "  while (num2 >= 0) {\n"
        "    num2 = num2 - 1;\n"
        "    print num2;\n"
        "  } num2 = 5;\n"
        "  num1 = num1 + 1;\n"
        "}}";
    return procedure;
  }

  string chainedWhileTwo() {
    string procedure =
        "procedure main {\n"
        "num1 = 0;\n"
        "num2 = 5;\n"
        "num3 = 0;\n"
        "while (num1 <= 5) {\n"
        "  while (num2 >= 0) {\n"
        "    while (num3 > 3) {\n"
        "      num3 = num3 + 1;\n"
        "        print num3;\n"
        "  } num3 = 0;\n"
        "  num2 = num2 - 1;\n"
        "  print num2;\n"
        "  } num2 = 5;\n"
        "  num1 = num1 + 1;\n"
        "}}";
    return procedure;
  }

  string callInWhile() {
    string procedure =
        "procedure main {\n"
        "num = 0;\n"
        "while (num != 5) {\n"
        "  call proc;\n"
        "}}\n"
        "procedure proc {\n"
        "print num;\n"
        "}" return procedure;
  }

  // programs with while and if/else

  string ifInWhile() {
    string procedure =
        "procedure main {\n"
        "read num;\n"
        "while (num >= 0) {\n"
        "  numCheck = num % 2;\n"
        "  if (numCheck == 0) then {\n"
        "    print num;\n"
        "  } else {\n"
        "  } num = num - 1;\n"
        "}}";
    return procedure;
  }

  string whileInIf() {
    string procedure =
        "procedure main {\n"
        "read num;\n"
        "if (num > 0) then {\n"
        "  while (num > 0) {\n"
        "    num = num - 1;\n"
        "  }\n"
        "} else {\n"
        "}}";
    return procedure;
  }

  string whileInIfElse() {
    string procedure =
        "procedure main {\n"
        "read num;\n"
        "if (num > 0) then {\n"
        "  while (num > 0) {\n"
        "    num = num - 1;\n"
        "  }\n"
        "} else {\n"
        "  while (num <= 0) {\n"
        "    num = num + 1;\n"
        "  }\n"
        "}}";
    return procedure;
  }

  string whileChainInIf() {
    string procedure =
        "procedure main {\n"
        "read num;\n"
        "read numOne;\n"
        "if (num > 0) then {\n"
        "  while (num > 0) {\n"
        "    while (numOne >= 0) {\n"
        "      numOne = numOne - num;\n
        "    } num = num - 1;\n"
        "  }\n"
        "} else {\n"
        "}}";
    return procedure;
  }
};
