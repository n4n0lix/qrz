#include "gtest/gtest.h"

#include "integer_expr_ast.h"
#include "parser_context.h"

TEST(IntegerExprAst, valid_value_generates_a_llvm_value) {
  ParserContext driver;

  auto max_value = IntegerExprAST("9223372036854775807");
  auto min_value = IntegerExprAST("-9223372036854775808");
  auto zero = IntegerExprAST("0");
  auto negative_zero = IntegerExprAST("-0");

  EXPECT_NE( nullptr, max_value.generate_code(driver) );
  EXPECT_NE( nullptr, min_value.generate_code(driver) );
  EXPECT_NE( nullptr, zero.generate_code(driver) );
  EXPECT_NE( nullptr, negative_zero.generate_code(driver) );
}

TEST(IntegerExprAst, invalid_value_fails_to_generate_a_llvm_value) {
  ParserContext driver;

  auto too_big   = IntegerExprAST("9223372036854775808");
  auto too_small = IntegerExprAST("-9223372036854775809");

  EXPECT_EQ( nullptr, too_big.generate_code(driver) );
  EXPECT_EQ( nullptr, too_small.generate_code(driver) );
}

