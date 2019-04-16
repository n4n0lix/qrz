#include "lexer.h"

#define LEX_RULE(r,f) \
if (handle( r, [&](LexerContext drv) -> token_type \
f \
, ctx, input, tokens)) continue;

deque<token> Lexer::scan(string pInput, string location)
{
  auto tokens = deque<token>();
  auto ctx = LexerContext();
  ctx.cur_file = location;
  auto input = pInput;

  // Fix the input so that regxps that match with an end \n work
  if (!str_utils::ends_with(input, "\n")) {
    input.append("\n");
  }

  //std::cout << "\n\t\tLEXER\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";
  while (input.size() > 0) {
    bool matched = false;

    // linebreak & 1-line comment
    LEX_RULE("(#.*)?\n", {
      drv.cur_line_no++;
      drv.cur_line_pos = 0;
      return NONE;
    })

    // blank
    LEX_RULE("[ \r\t\v]+", { return NONE; })

    // keywords
    LEX_RULE("(func)", { return FUNC; })
    LEX_RULE("(extern)", { return EXTERN; })

    // assignment
    // LEX_RULE("[\=]", {	return token_type::ASSIGNMENT;	})

    // operators
    LEX_RULE("[\\+]", { return OP_PLUS; })
    LEX_RULE("[\\-]", { return OP_MINUS; })
    LEX_RULE("[\\*]", { return OP_MULTIPLY; })
    LEX_RULE("[\\/]", { return OP_DIVIDE; })

    // misc
    LEX_RULE("[\\,]", { return COMMA; })
    LEX_RULE("[\\(]", { return LEFT_PARENTHESIS; })
    LEX_RULE("[\\)]", { return RIGHT_PARENTHESIS; })

    // integer
    LEX_RULE("[0-9][0-9_]*", { return INTEGER; })

    // identifier
    LEX_RULE("^[a-zA-Z_][a-zA-Z_0-9]*", { return IDENTIFIER; })

    // eat garbage
    LEX_RULE(".", { return NONE; })
  }

  //std::cout << "Num tokens: " << tokens.size() << "\n\n";
  //for (auto token : tokens) {
  //  std::cout << token.type_name() << "\n";
  //}

  return std::move( tokens );
}

bool Lexer::handle(string pRegex, std::function<token_type(LexerContext&)> func, LexerContext& context, string& input, deque<token>& tokens)
{
	// Handle matching
	std::regex regex("^" + pRegex);
	std::smatch smatch;
	if (!std::regex_search(input, smatch, regex, std::regex_constants::match_continuous)) {
		return false;
	}

	auto match = input.substr(0, smatch[0].length());

	// Create token
	token_type type = func( context );

  if (type != token_type::NONE) {
    token aToken;
    aToken.line_no = context.cur_line_no;
    aToken.line_pos = context.cur_line_pos;
    aToken.value = match;
    aToken.type = type;
    tokens.push_back(aToken);
  }

  context.cur_line_pos += match.length();
	input = input.substr(match.length());
	
	return true;
}
