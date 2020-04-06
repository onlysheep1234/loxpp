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

	//eof
	Eof,
};


class token
{
public:
	token(token_type type, std::wstring lexeme, double lv, int line, int col) : type{ type }, lexeme{ lexeme }, line{ line }, col{ col }, val{lv}{};
	token(token_type type, std::wstring lexeme, std::wstring&& lv, int line, int col) : type{ type }, lexeme{ lexeme }, line{ line }, col{ col }, val{ lv }{};
	token(token_type type, std::wstring lexeme, int line, int col) : type{ type }, lexeme{ lexeme }, line{ line }, col{ col }, val{ nullptr }{};
	friend std::wostream& operator<< (std::wostream & os, const token & t);

private:
	token_type type;
	std::wstring lexeme;
	std::variant<std::wstring, double, void*> val;
	int line;
	int col;
};


//todo: use a more elegent method
std::wostream& operator<<(std::wostream& os, const token_type tt) {
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
    case token_type::Eof:
        os << "eof";
        break;
	default:
		break;
	}

	return os;
}

std::wostream& operator<<(std::wostream& os, const token& t) {
	return os << t.type << "," << t.lexeme << "," << t.line << "," << t.col;
};

std::unordered_map<std::wstring, token_type> keywords = {
        {L"and", token_type::AND},
        {L"or", token_type::OR},
        {L"false", token_type::FALSE},
        {L"true", token_type::TRUE},
        {L"if", token_type::IF},
        {L"else", token_type::ELSE},
        {L"func", token_type::FUN},
        {L"for", token_type::FOR},
        {L"nil", token_type::NIL},
        {L"while", token_type::WHILE},
        {L"return", token_type::RETURN},
        {L"super", token_type::SUPER},
        {L"this", token_type::THIS},
        {L"var", token_type::VAR},
        {L"print", token_type::PRINT},
};
