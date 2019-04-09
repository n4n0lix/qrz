#include "_global.h"
#include "_defs.h"

#include "lexer.h"
#include "parser.h"

#include "gtest/gtest.h"
#include "clara.h"

int main(int argc, char *argv[])
{
  bool test = false;

  auto cli
    = clara::Opt(test, "test")
        ["-t"]["--test"]
        ("Execute tests");

  auto result = cli.parse( clara::Args(argc, argv) );
  if (!result) {
    std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
    exit(1);
  }

  // TEST
  if (test) {
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cin.ignore();
    return result;
  }

  // COMPILE
  print_defs();

  lexer lexer;
  auto tokens = lexer.scan("test.q");

  parser parser;
  parser.parse(std::move(tokens));

  std::cin.ignore();
  return 0;
}