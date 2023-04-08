#include "MyGrammarBaseListener.h"
#include "MyGrammarLexer.h"
#include "MyGrammarParser.h"
#include <ANTLRInputStream.h>
#include <antlr4-runtime.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int operation(int leftId, int rightId, MyGrammarParser::T1Context *sign) {
  if (sign->plus != nullptr) {
    int result = leftId + rightId;
    std::cout << "Operation:" << leftId << "+" << rightId << "=" << result
              << "\n";
    return result;
  } else if (sign->minus != nullptr) {
    int result = leftId - rightId;
    std::cout << "Operation:" << leftId << "-" << rightId << "=" << result
              << "\n";
    return result;
  }
  throw std::runtime_error("Ошибка в вычислении операции.");
}

bool comparision(int leftId, int rightId, MyGrammarParser::T2Context *sign) {
  if (sign->less != nullptr) {
    return leftId < rightId;
  } else if (sign->equal != nullptr) {
    return leftId == rightId;
  } else if (sign->not_equal != nullptr) {
    return leftId != rightId;
  } else if (sign->more != nullptr) {
    return leftId > rightId;
  }
  throw std::runtime_error("Ошибка в вычислении comparision.");
}

std::string comparision_to_string(MyGrammarParser::T2Context *sign) {
  if (sign->less != nullptr) {
    return "<";
  } else if (sign->equal != nullptr) {
    return "=";
  } else if (sign->not_equal != nullptr) {
    return "!=";
  } else if (sign->more != nullptr) {
    return ">";
  }
  throw std::runtime_error("Ошибка в вычислении comparision.");
}

int calc(std::vector<int> &values,
         std::vector<MyGrammarParser::T1Context *> &ops) {
  if (values.size() == 1) {
    return values[0];
  }
  int val = 0;
  val = operation(values[0], values[1], ops[0]);

  for (size_t i = 1; i < ops.size(); i++) {
    val = operation(val, values[i + 1], ops[i]);
  }
  return val;
}

class Listener : public MyGrammarBaseListener {
  bool finalResul;
  int leftValue = 0;
  int rightValue = 0;
  std::vector<int> rightValues, leftValues;
  std::vector<MyGrammarParser::T1Context *> rightOp, leftOp;
  MyGrammarParser::T2Context *sign;

  void exitP0(MyGrammarParser::P0Context *ctx) override {
    if (ctx->ID() == nullptr)
      throw std::runtime_error("Картина маслом.Приплыли\n");

    int tmp = std::stoi(ctx->ID()->toString());
    this->leftValues.insert(this->leftValues.begin(), tmp);

    this->rightValue = calc(this->rightValues, rightOp);
    this->leftValue = calc(this->leftValues, leftOp);

    this->finalResul =
        comparision(this->leftValue, this->rightValue, this->sign);
    std::cout << "\n\n"
              << leftValue << comparision_to_string(this->sign) << rightValue
              << "=" << finalResul << "\n\n";
  }

  void exitP1(MyGrammarParser::P1Context *ctx) override {
    if (ctx->ID() == nullptr)
      return;
    int tmp = std::stoi(ctx->ID()->toString());
    this->leftValues.insert(this->leftValues.begin(), tmp);
    this->leftOp.insert(this->leftOp.begin(), ctx->t1());
  }

  void exitP2(MyGrammarParser::P2Context *ctx) override {

    this->sign = ctx->t2();

    int tmp = std::stoi(ctx->ID()->toString());
    this->rightValues.insert(this->rightValues.begin(), tmp);
  }

  void exitP3(MyGrammarParser::P3Context *ctx) override {
    if (ctx->ID() == nullptr) {
      return;
    }
    int tmp = std::stoi(ctx->ID()->toString());
    this->rightValues.insert(this->rightValues.begin(), tmp);
    this->rightOp.insert(this->rightOp.begin(), ctx->t1());
  }
};

int main(int argc, const char *argv[]) {
  std::ifstream stream;
  if (argc > 1) {
    stream.open(argv[1]);
    std::cout << argv[1] << std::endl;
  } else {
    stream.open("../test");
  }
  antlr4::ANTLRInputStream input(stream);

  MyGrammarLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);

  tokens.fill();
  for (auto token : tokens.getTokens()) {
    std::cout << token->toString() << std::endl;
  }

  MyGrammarParser parser(&tokens);
  Listener listener;
  parser.addParseListener(&listener);

  antlr4::tree::ParseTree *tree = parser.p0();

  std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

  return 0;
}
