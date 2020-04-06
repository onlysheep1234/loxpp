//
// Created by 黄耀东 on 2020/3/31.
//

#ifndef LOXPP_SCANNER_H
#define LOXPP_SCANNER_H

//TODO:add file path
struct err {
    int col;
    int line;
    std::wstring info;
    err(int col, int line, std::wstring&& info) : col{col}, line{line}, info{info}{}
};

std::wostream& operator<< (std::wostream& os, err& e){
    os << "line " << e.line << ", col " << e.col << " : " << e.info;
    return os;
}

class scanner {
public:
    explicit scanner(std::wistream& is);
    std::list<token>&& get_tokens();
    std::list<err>&& get_errors();
    wchar_t advance(std::wistream&, std::wstring& str);
    bool match(wchar_t c, std::wstring& line);
    void next_line(std::wistream&, std::wstring&);
//    std::wstring&& string(std::wstring& line, std::wistream&);
    static bool is_digit(wchar_t c);
    static bool is_alpha(wchar_t c);
    static bool is_blank(wchar_t c);
    void number(const std::wstring&, std::wistream& is);
    void identifier(const std::wstring&, std::wistream&);
    void string(std::wstring&, std::wistream&);

private:
    std::list<token> tokens;
    std::list<err> errs;
    int col;
    int line_no;
};

scanner::scanner(std::wistream &is) {
    wchar_t c = L'\n';
    col = 0;
    line_no = 1;
    std::wstring line;
    while (c != L'\0') {
        c = advance(is, line);
        switch (c) {
            //single character
            case L'(':
                tokens.emplace_back(token_type::LEFT_PAREN, L"(", col, line_no);
                break;
            case L')':
                tokens.emplace_back(token_type::RIGHT_PAREN, L")", col, line_no);
                break;
            case L'{':
                tokens.emplace_back(token_type::LEFT_BRACE, L"{", col, line_no);
                break;
            case L'}':
                tokens.emplace_back(token_type::RIGHT_BRACE, L"}", col, line_no);
                break;
            case L',':
                tokens.emplace_back(token_type::COMMA, L",", col, line_no);
                break;
            case L'.':
                tokens.emplace_back(token_type::DOT, L".", col, line_no);
                break;
            case L'-':
                tokens.emplace_back(token_type::MINUS, L"-", col, line_no);
                break;
            case L'+':
                tokens.emplace_back(token_type::PLUS, L"+", col, line_no);
                break;
            case L';':
                tokens.emplace_back(token_type::SEMICOLON, L";", col, line_no);
                break;
            case L'*':
                tokens.emplace_back(token_type::STAR, L"*", col, line_no);
                break;
                //two char
            case L'!':
                tokens.emplace_back(match(L'=', line) ? token_type::BANG_EQUAL : token_type::BANG, L"", col, line_no);
                break;
            case L'=':
                tokens.emplace_back(match(L'=', line) ? token_type::EQUAL_EQUAL : token_type::EQUAL, L"", col, line_no);
                break;
            case L'>':
                tokens.emplace_back(match(L'=', line) ? token_type::GREATER_EQUAL : token_type::GREATER, L"", col,
                                    line_no);
                break;
            case L'<':
                tokens.emplace_back(match(L'=', line) ? token_type::LESS_EQUAL : token_type::LESS, L"", col, line_no);
                break;
                //one line comment
            case L'/':
                if (match(L'/', line)) {
                    //just ignore comment
                    //TODO:add comment to tokens
                    try {
                        next_line(is, line);
                    }catch (std::exception& e){}
                } else {
                    tokens.emplace_back(token_type::SLASH, L"/", col, line_no);
                }
                break;
            case L'"': {
//                tokens.emplace_back(token_type::STRING, );
                string(line, is);
                break;
            }
                //nextline
            case L' ':
            case L'\t':
            case L'\r':
            case L'\n':
                break;
            default:
                if (is_digit(c)){
                    number(line, is);
                } else if (is_alpha(c)){
                    identifier(line, is);
                } else if (c == L'\0'){
                    break;
                }
                else {
                    errs.emplace_back(col, line_no, L"unexcept character");
                }
                break;
        }
    }
    this->tokens.emplace_back(token_type::Eof, L"eof", -1, -1);
}

wchar_t scanner::advance(std::wistream& is, std::wstring &str) {
    if (col >= str.size()){
        try {
            next_line(is, str);
            return L'\n';
        } catch (std::out_of_range& oe){
            return L'\0';
        }
    }
    return str[col++];
}

bool scanner::match(wchar_t c, std::wstring &line) {
    if (col+1 >= line.size()){
        return false;
    }

    if (line[col+1] != c){
        return false;
    }

    return ++col;
}

void scanner::string(std::wstring& line, std::wistream& is) {
    auto start = col-1;
    auto start_line = line_no;
    std::wstring s;
    for (auto c = advance(is, line); c != L'"'; c = advance(is, line)){
        if (c == L'\0'){
            errs.emplace_back(start-1, start_line, L"part string");
            return;
        }
        s+=c;
    }
    tokens.emplace_back(token_type::STRING, L"string_literal", std::move(s), start, start_line);
}

bool scanner::is_digit(wchar_t c) {
    return c >= L'0' && c <= L'9';
}


void scanner::number(const std::wstring &line, std::wistream &is) {
    std::wstring s;
    auto start = col-1, end = col;
    bool dot_exists = false;
    for (; end < line.size() && (is_digit(line[end]) || line[end] == L'.'); end++){
        if (line[end] == L'.') {
            if (dot_exists){
                errs.emplace_back(start, line_no, L"numbers error");
                col = end+1;
                return;
            }
            if (end+1 >= line.size() || !is_digit(line[end+1])){
                errs.emplace_back(start, line_no, L"numbers error");
                col = end+1;
                return;
            }
            dot_exists = true;
        }
    }

    col = end;
    auto lexeme = line.substr(start, col-start);
    std::wstringstream wss {lexeme};
    double d;
    wss >> d;
    tokens.emplace_back(token_type::NUMBER, lexeme, d, line_no, col);
}

bool scanner::is_alpha(wchar_t c) {
    return (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || c == L'_';
}

bool scanner::is_blank(wchar_t c) {
    return c == L' ' || c == L'\t' || c == L'\r' || c == L'\n';
}

void scanner::next_line(std::wistream &is, std::wstring &line) {
    if (is.eof()){
        throw std::out_of_range("istream out of range");
    }
    std::getline(is, line);
    line_no++;
    col  = 0;
}

//TODO: identifier corss line
void scanner::identifier(const std::wstring &line, std::wistream &) {
    int start = col-1, end = col;
    for (;is_alpha(line[end]) || is_digit(line[end]); end++){
    }

    col = end;
    auto name = line.substr(start, end-start);
    auto iter = keywords.find(name);
    if (iter != keywords.end()){
        tokens.emplace_back(iter->second, name, start, line_no);
    } else {
        tokens.emplace_back(token_type::IDENTIFIER, name, start, line_no);
    }
}

std::list<token>&& scanner::get_tokens() {
    return  std::move(tokens);
}

std::list<err> &&scanner::get_errors() {
    return std::move(errs);
}

#endif //LOXPP_SCANNER_H
