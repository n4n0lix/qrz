#include "_global.h"
#include "_defs.h"

#include "gtest/gtest.h"
#include "clara.h"

#include "lexer.h"
#include "parser.h"
#include "file_utils.h"


int exec_tests(int argc, char *argv[]);
int exec_compile(string filename);
int exec_interactive();

int main(int argc, char *argv[])
{
  string command = "compile";
  string file = "test.q";

  auto cli
    = clara::Opt(file, "file")
      ["-f"]["--file"]
      ("The file to compile")
    | clara::Arg(command, "command")
      ("the command to run (compile|interactive|test)");

  auto result = cli.parse( clara::Args(argc, argv) );
  if (!result) {
    std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
    exit(1);
  }

  //print_defs();

  // TEST
  if (command == "test") {
    return exec_tests(argc, argv);
  }

  // COMPILE
  else if (command == "compile") {
    return exec_compile( file );
  }

  // INTERACTIVE
  else if (command == "interactive") {
    return exec_interactive();
  }

  return 1;
}

int exec_tests(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  std::cin.ignore();
  return result;
}

int exec_compile(string filename) {
  auto input = file_utils::load_file( filename );
  auto tokens = Lexer::scan( input, filename );

  Parser parser;
  unique<ParserContext> context = std::make_unique<ParserContext>();
  parser.parse( std::move(tokens), context.get() );

  std::cin.ignore();
  return 0;
}

int exec_interactive() {
  unique<ParserContext> context = std::make_unique<ParserContext>();

  while (1) {
    string input;
    std::cout << "ready> ";
    std::getline( std::cin, input);

    if (input == "exit" || input == "quit") 
      return 0;

    auto tokens = Lexer::scan( input, "<interactive mode>" );
    Parser parser;
    parser.parse( std::move(tokens), context.get() );
  }
}