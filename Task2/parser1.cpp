#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

enum TokenType {
    T_INT,T_ID,T_NUM,T_IF,
    T_ELSE,T_RETURN,T_ASSIGN,
    T_PLUS,T_MINUS,T_MUL,T_DIV,
    T_LPAREN,T_RPAREN,T_LBRACE,
    T_RBRACE,T_SEMICOLON,T_GT,T_EOF
};

struct Token {
    TokenType type;
    string value;
    int line;  // Add line number
};

class Parser {
private:
    vector<Token> tokens;
    size_t pos;
public:
    Parser(const vector<Token> &tokens) {
        this->tokens = tokens;
        this->pos = 0;
    }
    void parseProgram() {
        while (tokens[pos].type != T_EOF) 
            parseStatement();
        cout << "Parsing completed successfully! No Syntax Error" << endl; // Moved outside the loop
    }
    void parseStatement() {
        if (tokens[pos].type == T_INT) parseDeclaration();
        else if (tokens[pos].type == T_ID) parseAssignment();
        else if (tokens[pos].type == T_IF) parseIfStatement();
         else if (tokens[pos].type == T_RETURN) parseReturnStatement();
         else if (tokens[pos].type == T_LBRACE) parseBlock();
         else {
            cout << "Syntax error: unexpected token " << tokens[pos].value 
                 << " on line " << tokens[pos].line << endl;
            exit(1);
        }
    }
    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) parseStatement();
        expect(T_RBRACE);
    }
    void parseDeclaration() {
        expect(T_INT);
        expect(T_ID);
        expect(T_SEMICOLON);
    }
    void parseAssignment() {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }
    void parseIfStatement() {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE) {
            expect(T_ELSE);
            parseStatement();
        }
    }
    void parseReturnStatement() {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }
    void parseExpression() {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            parseExpression(); // After relational operator, parse the next expression
        }
    }
    void parseTerm() {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            pos++;
            parseFactor();
        }
    }
    void parseFactor() {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID) {
            pos++;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        } else {
            cout << "Syntax error: unexpected token " << tokens[pos].value 
                 << " on line " << tokens[pos].line << endl;
            exit(1);
        }
    }
    void expect(TokenType type) {
        if (tokens[pos].type == type) {
            pos++;
        } else {
            cout << "Syntax error: expected " << type << " but found " 
                 << tokens[pos].value << " on line " << tokens[pos].line << endl;
            exit(1);
        }
    }
};

class Lexer {
private:
    string src;
    size_t pos;

public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
    }

    string consumeNumber() {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        int line = 0; // Start with line number 1
        while (pos < src.size()) {
            char current = src[pos];
            if (isspace(current)) {
                if (current == '\n') line++; // Increment line number on newline
                pos++;
                continue;
            }
            if (isdigit(current)) {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            } else if (isalpha(current)) {
                string word = consumeWord();
                TokenType type;
                if (word == "int") type = T_INT;
                else if (word == "if") type = T_IF;
                else if (word == "else") type = T_ELSE;
                else if (word == "return") type = T_RETURN;
                else type = T_ID;
                tokens.push_back(Token{type, word, line});
                continue;
            }
            switch (current) {
                case '=': tokens.push_back(Token{T_ASSIGN, "=", line}); break;
                case '+': tokens.push_back(Token{T_PLUS, "+", line}); break;
                case '-': tokens.push_back(Token{T_MINUS, "-", line}); break;
                case '*': tokens.push_back(Token{T_MUL, "*", line}); break;
                case '/': tokens.push_back(Token{T_DIV, "/", line}); break;
                case '(': tokens.push_back(Token{T_LPAREN, "(", line}); break;
                case ')': tokens.push_back(Token{T_RPAREN, ")", line}); break;
                case '{': tokens.push_back(Token{T_LBRACE, "{", line}); break;
                case '}': tokens.push_back(Token{T_RBRACE, "}", line}); break;
                case ';': tokens.push_back(Token{T_SEMICOLON, ";", line}); break;
                case '>': tokens.push_back(Token{T_GT, ">", line}); break;
                default: cout << "Unexpected character: " << current << " on line " << line << endl; exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line}); // Add EOF token
        return tokens;
    }
};

int main() {
    string input = R"(
    int a;
    a = 5;
    int b;
    b = a + 10;
    if (b > 10) {
        return b;
    } else {
        return 0;
    })";
    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
