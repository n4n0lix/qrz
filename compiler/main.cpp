#include "lexer.h"
#include "parser.h"

int main()
{
  std::cout << "LUL" << std::endl;

  lexer lexer;
  auto tokens = lexer.scan("test.q");

  parser parser;
  parser.parse( std::move(tokens) );

  std::cin.ignore();
  return 0;
}