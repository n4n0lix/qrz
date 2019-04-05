#include "lexer.h"

#define LEX_RULE(r,f) \
if (handle( r, [&](ldriver drv) -> token_type \
f \
)) continue;

string lexer::load_file(string path)
{
	std::ifstream file( path );

	if (file)
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();

		return buffer.str();
	}

	return "";
}

deque<token> lexer::scan(string path)
{
	_tokens = deque<token>();
  _driver = ldriver();
	_input = load_file(path);

  _driver.cur_file = path;

	// Fix the input so that regxps that match with an end \n work
	if (!str_ends_with(_input, "\n")) {
		_input.append("\n");
	}

	std::cout << "\t\tLEXER\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";

	while (_input.size() > 0) {
		bool matched = false;

		// linebreak & 1-line comment
    LEX_RULE("(#.*)?\n", {
      drv.cur_line_no++;
			drv.cur_line_pos = 0;
			return NONE;
    })

		// blank
		LEX_RULE("[ \r\t\v]+",  { return NONE; })

		// keywords
		LEX_RULE("(func)", { return FUNC;	})
    LEX_RULE("(extern)", { return EXTERN; })
    
		// assignment
    // LEX_RULE("[\=]", {	return token_type::ASSIGNMENT;	})

		// operators
		LEX_RULE("[\\+]", {	return OP_PLUS;	})
    LEX_RULE("[\\-]", {	return OP_MINUS; })
		LEX_RULE("[\\*]", {	return OP_MULTIPLY; })
    LEX_RULE("[\\/]", {	return OP_DIVIDE; })

		// misc
    LEX_RULE("[\\,]", {	return COMMA;	})
    LEX_RULE("[\\(]", {	return LEFT_PARENTHESIS; })
    LEX_RULE("[\\)]", {	return RIGHT_PARENTHESIS; })

		// integer
		LEX_RULE("[0-9][0-9_]*", { return INTEGER; })

		// identifier
		LEX_RULE("^[a-zA-Z_][a-zA-Z_0-9]*", {	return IDENTIFIER; })

		// eat garbage
		LEX_RULE(".", {	return NONE; })
	}

  std::cout << "Num tokens: " << _tokens.size() << "\n\n";
	for (auto token : _tokens) {
		std::cout << token.type_name() << "\n";
	}

	return std::move(_tokens);
}

bool lexer::handle(string pRegex, std::function<token_type(ldriver&)> func)
{
	// Handle matching
	std::regex regex("^" + pRegex);
	std::smatch smatch;
	if (!std::regex_search(_input, smatch, regex, std::regex_constants::match_continuous)) {
		return false;
	}

	auto match = _input.substr(0, smatch[0].length());

	// Create token
	token_type type = func(_driver);

  if (type != token_type::NONE) {
    token aToken;
    aToken.line_no = _driver.cur_line_no;
    aToken.line_pos = _driver.cur_line_pos;
    aToken.value = match;
    aToken.type = type;
    _tokens.push_back(aToken);
  }

	_driver.cur_line_pos += match.length();
	_input = _input.substr(match.length());
	
	return true;
}

bool lexer::str_ends_with(const string &str, const string &ending) {
	if (str.length() >= ending.length()) {
		return (0 == str.compare(str.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}