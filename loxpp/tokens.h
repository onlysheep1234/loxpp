#pragma once


enum class token_type
{
	//single character token
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	// One or two character tokens.                  
	BANG, BANG_EQUAL,  // !, !=
	EQUAL, EQUAL_EQUAL, // =, ==
	GREATER, GREATER_EQUAL, //>, >=
	LESS, LESS_EQUAL, //<, <=

	// Literals.                                     
	IDENTIFIER, STRING, NUMBER,

	// Keywords.   
	AND, OR, FALSE, TRUE,
	IF, ELSE, FUN, FOR, NIL, WHILE,
	RETURN, SUPER, THIS, VAR,
	PRINT,
};


class token
{
public:
	token(token_type type, std::string lexeme, double lv, int line, int col) : type{ type }, lexeme{ lexeme }, line{ line }, col{ col }, val{lv}{};
	token(token_type type, std::string lexeme, std::string lv, int line, int col) : type{ type }, lexeme{ lexeme }, line{ line }, col{ col }, val{ lv }{};
	token(token_type type, std::string lexeme, int line, int col) : type{ type }, lexeme{ lexeme }, line{ line }, col{ col }, val{ nullptr }{};
	friend std::ostream& operator<< (std::ostream & os, const token & t);

private:
	token_type type;
	std::string lexeme;
	std::variant<std::string, double, void*> val;
	int line;
	int col;
};


//todo: use a more elegent method
std::ostream& operator<<(std::ostream& os, const token_type tt) {
	switch (tt)
	{
	case token_type::LEFT_PAREN:
	case token_type::RIGHT_PAREN:
	case token_type::LEFT_BRACE:
	case token_type::RIGHT_BRACE:
	case token_type::COMMA:
	case token_type::DOT:
	case token_type::MINUS:
	case token_type::PLUS:
	case token_type::SEMICOLON:
	case token_type::SLASH:
	case token_type:: STAR:
		os << "single_char_token";
		break;
	case token_type::BANG:
	case token_type::BANG_EQUAL:// !, !=
	case token_type::EQUAL:
	case token_type::EQUAL_EQUAL:// =, ==
	case token_type::GREATER:
	case token_type::GREATER_EQUAL: //>, >=
	case token_type::LESS:
	case token_type::LESS_EQUAL: //<, <=
		os << "may_double_char_token";
		break;
	case token_type::IDENTIFIER:
	case token_type::STRING:
	case token_type::NUMBER:
		os << "literal";
		break;
	case token_type::AND:
	case token_type::OR:
	case token_type::FALSE:
	case token_type::TRUE:
	case token_type::IF:
	case token_type::ELSE:
	case token_type::FUN:
	case token_type::FOR:
	case token_type::NIL:
	case token_type::WHILE:
	case token_type::RETURN:
	case token_type::SUPER:
	case token_type::THIS:
	case token_type::VAR:
	case token_type::PRINT:
		os << "keyword";
		break;
	default:
		break;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const token& t) {
	return os << t.type << "," << t.lexeme << "," << t.line << "," << t.col;
};

