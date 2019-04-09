#include "_global.h"
#include "_defs.h"

#include "lexer.h"
#include "parser.h"

int main()
{
  print_defs();

  lexer lexer;
  auto tokens = lexer.scan("test.q");

  parser parser;
  parser.parse( std::move(tokens) );

  std::cin.ignore();
  return 0;
}