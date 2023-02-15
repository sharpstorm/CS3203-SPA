#include <string>
class SourceTestCases {
 public:
  string SourceTestCases::empty() {
    string procedure =
        "procedure empty {\n"
        "}";
    return procedure;
  }

  string SourceTestCases::simpleProgram() {
    string procedure =
        "procedure simple {\n"
        "read num1;\n"
        "num1 = num1 + 1;\n"
        "print num1;\n"
        "}";
    return procedure;
  }

  string SourceTestCases::oneLine() {
    string procedure =
        "procedure jammed {\n"
        "read num1;read num2;num3 = num1 + num2;print num3;\n"
        "}";
    return procedure;
  }

  string SourceTestCases::sameNames() {
    string procedure =
        "procedure procedure {\n"
        "read read;\n"
        "print read;\n"
        "}";
    return procedure;
  }

  string SourceTestCases::longNames() {
    string procedure =
        "procedure arbitrarilyLongProcedureName {\n"
        "read someVeryArbitrarilyLongVariableName;\n"
        "print someVeryArbitrarilyLongVariableName;\n"
        "}";
    return procedure;
  }

  string SourceTestCases::whitespace() {
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

  string SourceTestCases::testAssign() {
    string procedure =
        "procedure assign {\n"
        "num1 = 0;\n"
        "num2 = num1;\n"
        "num1 = num1;\n"
        "print num1;\n"
        "}";
    return procedure;
  }

  string SourceTestCases::testAverage() {
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

  string SourceTestCases::testOperaters() {
    string procedure =
        "procedure operators {\n"
        "num1 = 1 + 2 + 3 * 4 / 5;\n"
        "num2 = 5 * 5 - 5 / (4*3);\n"
        "num3 = num1+num2+1\n"
        "print num3;\n"
        "}";
    return procedure;
  }

  string SourceTestCases::callProcedures() {
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

  string SourceTestCases::simpleIf() {
    string procedure =
        "procedure if {\n"
        "read num;\n"
        "if (num==0) then {\n"
        "  print num;\n"
        "}}";
    return procedure;
  }

  string SourceTestCases::oneLineIf() {
    string procedure =
        "procedure longif {\n"
        "read num;\n"
        "if (num==0) then {  print num;}}";
    return procedure;
  }

  string SourceTestCases::simpleIfElse() {
    string procedure =
        "procedure ifElse {\n"
        "read num;\n"
        "if (num==0) then {\n"
        "  print num;\n"
        "} else {\n"
        "  print 0;\n"
        "}}";
    return procedure;
  }
};