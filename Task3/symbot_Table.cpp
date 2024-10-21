#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include <iomanip>

using namespace std;

enum TokenType {
    T_INT,
    T_FLOAT,
    T_DOUBLE,
    T_STRING,
    T_BOOL,
    T_CHAR,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_GT,
    T_EOF
};

struct Token {
    TokenType type;
    string value;
    int line;
};

class Parser {
private:
    vector<Token> tokens;
    size_t pos;
    unordered_map<string, string> symbolTable;

public:
    Parser(const vector<Token>& tokens) {
        this->tokens = tokens;
        this->pos = 0;
    }

    void parseProgram() {
        while (tokens[pos].type != T_EOF)
            parseStatement();
        cout << "Parsing completed successfully! No Syntax Error" << endl;
        displaySymbolTable();
    }

    void parseStatement() {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_STRING || 
            tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR)
            parseDeclaration();
        else if (tokens[pos].type == T_ID)
            parseAssignment();
        else if (tokens[pos].type == T_IF)
            parseIfStatement();
        else if (tokens[pos].type == T_RETURN)
            parseReturnStatement();
        else if (tokens[pos].type == T_LBRACE)
            parseBlock();
        else {
            cout << "Syntax error: unexpected token " << tokens[pos].value
                 << " on line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
            parseStatement();
        expect(T_RBRACE);
    }

    void addToSymbolTable(string varName, string type) {
        symbolTable[varName] = type;
    }

    void parseDeclaration() {
        string dataType = tokens[pos].value; // Get the data type
        pos++; // Move to the next token

        if (tokens[pos].type == T_ID) {
            string varName = tokens[pos].value;
            pos++; // Move to the next token

            // Check for duplicate declaration
            if (symbolTable.find(varName) != symbolTable.end()) {
                cout << "Error: Variable '" << varName
                     << "' is already declared on line " << tokens[pos].line << endl;
                exit(1);
            }

            // Add the variable to the symbol table
            addToSymbolTable(varName, dataType);

            // Check if there's an assignment during declaration
            if (tokens[pos].type == T_ASSIGN) {
                pos++;
                parseExpression(); // Handle assignment
            }

            expect(T_SEMICOLON); // Ensure semicolon is present
        } else {
            cout << "Syntax error: Expected variable name on line "
                 << tokens[pos].line << endl;
            exit(1);
        }
    }
    void displaySymbolTable() {
    cout << "\nSymbol Table:\n";
    cout << "-----------------------------------\n";
    cout << "| Variable Name |    Data Type   |\n";
    cout << "-----------------------------------\n";
    for (const auto& entry : symbolTable) {
        cout << "| " << setw(14) << left << entry.first 
             << "| " << setw(15) << left << entry.second << "|\n";
    }
    cout << "-----------------------------------\n";
}

    // void displaySymbolTable() {
    //     cout << "\nSymbol Table:\n";
    //     cout << "Variable Name\tData Type\n";
    //     for (const auto& entry : symbolTable) {
    //         cout << entry.first << "\t\t" << entry.second << endl;
    //     }
    // }

    void parseAssignment() {
        string varName = tokens[pos].value;
        pos++;

        if (symbolTable.find(varName) == symbolTable.end()) {
            cout << "Error: Variable '" << varName
                 << "' is not declared on line " << tokens[pos].line << endl;
            exit(1);
        }

        string expectedType = symbolTable[varName];
        expect(T_ASSIGN);
        parseExpression(); // Parse the right-hand side of the assignment

        // Type-check if necessary (this is a placeholder; further development needed)
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
        // For now, only parsing the first term
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            parseExpression();
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
    Lexer(const string &src) : src(src), pos(0) {}

    string consumeNumber() {
        size_t start = pos;
        bool hasDecimal = false;

        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.')) {
            if (src[pos] == '.') {
                if (hasDecimal)
                    break;
                hasDecimal = true;
            }
            pos++;
        }

        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;

        while (pos < src.size() && (isalnum(src[pos]) || src[pos] == '_'))
            pos++;

        return src.substr(start, pos - start);
    }

    string consumeString() {
        size_t start = ++pos; // Skip the opening quote (")
        while (pos < src.size() && src[pos] != '"') {
            if (src[pos] == '\\' && pos + 1 < src.size()) { // Handle escape sequences
                pos += 2; // Skip the escape character and the next character
                continue;
            }
            pos++; // Consume characters inside quotes
        }

        if (pos >= src.size() || src[pos] != '"') {
            cout << "Syntax error: Unterminated string literal" << endl;
            exit(1);
        }

        pos++; // Skip the closing quote (")
        return src.substr(start, pos - start - 1); // Return the string without quotes
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        int line = 1; // Start with line number 1

        while (pos < src.size()) {
            char current = src[pos];
            if (isspace(current)) {
                if (current == '\n')
                    line++; // Increment line number on newline
                pos++;
                continue;
            }
            if (isdigit(current)) {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            } else if (isalpha(current) || current == '_') {
                string word = consumeWord();
                TokenType type;

                if (word == "int") type = T_INT;
                else if (word == "float") type = T_FLOAT;
                else if (word == "double") type = T_DOUBLE;
                else if (word == "string") type = T_STRING;
                else if (word == "bool") type = T_BOOL;
                else if (word == "char") type = T_CHAR;
                else if (word == "if") type = T_IF;
                else if (word == "else") type = T_ELSE;
                else if (word == "return") type = T_RETURN;
                else type = T_ID; // Treat as an identifier

                tokens.push_back(Token{type, word, line});
                continue;
            }

            // Handle string literals
            if (current == '"') {
                tokens.push_back(Token{T_STRING, consumeString(), line});
                continue;
            }

            // Handle other single-character tokens
            switch (current) {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", line});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", line});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", line});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", line});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", line});
                break;
            default:
                cout << "Unexpected character: " << current << " on line " << line << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line}); // Add EOF token
        return tokens;
    }
};

void displaySymbolTable(const vector<Token>& tokens) {
    unordered_map<string, TokenType> symbolTable;

    for (size_t i = 0; i < tokens.size(); i++) {
        const auto& token = tokens[i];
        
        // Store variable declarations in the symbol table
        if (token.type == T_INT || token.type == T_FLOAT || token.type == T_STRING || token.type == T_BOOL) {
            // Check if the current token is a type declaration
            if (token.value == "int" || token.value == "float" || token.value == "string" || token.value == "bool") {
                // Make sure there is a next token and it is an identifier
                if (i + 1 < tokens.size() && tokens[i + 1].type == T_ID) {
                    string varName = tokens[i + 1].value; // Get the variable name
                    symbolTable[varName] = token.type; // Store the variable name and its type
                    i++; // Skip the next token as it's the variable name
                }
            }
        }
    }

    // Print the symbol table
    cout << "\nSymbol Table:\n";
    cout << "Variable Name\tData Type\n";
    for (const auto& entry : symbolTable) {
        cout << entry.first << "\t\t" << (entry.second == T_INT ? "int" :
                                              entry.second == T_FLOAT ? "float" :
                                              entry.second == T_STRING ? "string" :
                                              entry.second == T_BOOL ? "bool" : "unknown") << endl;
    }
}


int main() {
    string sourceCode = R"(
        int a;
        a = 5;
        float b;
        b = 10.5;
        string name;
        name = "John"; // Use double quotes for strings
        bool isActive;
        isActive = true;
        if (b > 10) {
            return b;
        } else {
            return 0;
        }
    )";

    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    cout << "Tokens:\n";
    for (const auto &token : tokens) {
        cout << "Type: " << token.type << ", Value: " << token.value << ", Line: " << token.line << endl;
    }

    // Display the symbol table
    displaySymbolTable(tokens);

    return 0;
}

// int main() {
    
//     string sourceCode = R"(
//     int a;
//     a = 5;
//     float b;
//     b = 10.5;
//     string name;
//     name = "John";
//     bool isActive;
//     isActive = true;
//     if (b > 10) {
//         return b;
//     } else {
//         return 0;
//     })";
//     // string sourceCode = R"(
//     //     int a = 10;
//     //     float b = 15.5;
//     //     if (a > b) {
//     //         return a;
//     //     } else {
//     //         return b;
//     //     }
//     // )";

//     Lexer lexer(sourceCode);
//     vector<Token> tokens = lexer.tokenize();

//     Parser parser(tokens);
//     parser.parseProgram();

//     return 0;
// }
