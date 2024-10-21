#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
#include <iomanip>

using namespace std;

enum TokenType
{
    T_INT,
    T_FLOAT,
    T_STRING,
    T_BOOL,
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
    T_LT,
    T_EQ,
    T_NEQ,
    T_LOGICAL_AND,
    T_LOGICAL_OR,
    T_WHILE,
    T_FOR,
    T_EOF
};

struct Token
{
    TokenType type;
    string value;
    int line;
};

class Parser
{
private:
    vector<Token> tokens;
    size_t pos;
    unordered_map<string, string> symbolTable;

public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
            parseStatement();
        cout << "Parsing completed successfully! No Syntax Error" << endl;
        displaySymbolTable();
    }

    void parseStatement()
    {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_STRING ||
            tokens[pos].type == T_BOOL)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileLoop();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForLoop();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cout << "Syntax error: unexpected token " << tokens[pos].value
                 << " on line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
            parseStatement();
        expect(T_RBRACE);
    }

    void addToSymbolTable(string varName, string type)
    {
        symbolTable[varName] = type;
    }

    void parseDeclaration()
    {
        string dataType = tokens[pos].value; // Get the data type
        pos++;                               // Move to the next token

        if (tokens[pos].type == T_ID)
        {
            string varName = tokens[pos].value;
            pos++; // Move to the next token

            // Check for duplicate declaration
            if (symbolTable.find(varName) != symbolTable.end())
            {
                cout << "Error: Variable '" << varName
                     << "' is already declared on line " << tokens[pos].line << endl;
                exit(1);
            }

            // Add the variable to the symbol table
            addToSymbolTable(varName, dataType);

            // Check if there's an assignment during declaration
            if (tokens[pos].type == T_ASSIGN)
            {
                pos++;
                parseExpression(); // Handle assignment
            }

            expect(T_SEMICOLON); // Ensure semicolon is present
        }
        else
        {
            cout << "Syntax error: Expected variable name on line "
                 << tokens[pos].line << endl;
            exit(1);
        }
    }

    void displaySymbolTable()
    {
        cout << "\nSymbol Table:\n";
        cout << "-----------------------------------\n";
        cout << "| Variable Name |    Data Type   |\n";
        cout << "-----------------------------------\n";
        for (const auto &entry : symbolTable)
        {
            cout << "| " << setw(14) << left << entry.first
                 << "| " << setw(15) << left << entry.second << "|\n";
        }
        cout << "-----------------------------------\n";
    }

    void parseAssignment()
    {
        string varName = tokens[pos].value;
        pos++;

        if (symbolTable.find(varName) == symbolTable.end())
        {
            cout << "Error: Variable '" << varName
                 << "' is not declared on line " << tokens[pos].line << endl;
            exit(1);
        }

        expect(T_ASSIGN);
        parseExpression(); // Parse the right-hand side of the assignment
        expect(T_SEMICOLON);
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseWhileLoop()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
    }

    void parseForLoop()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        // For now, we'll only support a simple for loop structure: for (init; condition; increment)
        expect(T_ID); // Assume init is a variable declaration for simplicity
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
        parseExpression(); // condition
        expect(T_SEMICOLON);
        expect(T_ID); // Assume increment is a variable name
        expect(T_ASSIGN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            pos++;
            parseTerm();
        }
        // Handle logical expressions
        while (tokens[pos].type == T_LOGICAL_AND || tokens[pos].type == T_LOGICAL_OR)
        {
            pos++;
            parseTerm();
        }
    }

    void parseTerm()
    {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            pos++;
            parseFactor();
        }
    }

    void parseFactor()
    {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            pos++;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        }
        else
        {
            cout << "Syntax error: unexpected token " << tokens[pos].value
                 << " on line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: expected " << type << " but found "
                 << tokens[pos].value << " on line " << tokens[pos].line << endl;
            exit(1);
        }
    }
};

class Lexer
{
private:
    string src;
    size_t pos;

public:
    Lexer(const string &src) : src(src), pos(0) {}

    string consumeNumber()
    {
        size_t start = pos;
        bool hasDecimal = false;

        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.'))
        {
            if (src[pos] == '.')
            {
                if (hasDecimal)
                    break;
                hasDecimal = true;
            }
            pos++;
        }

        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;

        while (pos < src.size() && (isalnum(src[pos]) || src[pos] == '_'))
            pos++;

        return src.substr(start, pos - start);
    }

    string consumeString()
    {
        size_t start = ++pos; // Skip the opening quote (")
        while (pos < src.size() && src[pos] != '"')
        {
            if (src[pos] == '\\' && pos + 1 < src.size())
            {             // Handle escape sequences
                pos += 2; // Skip the escape character and the next character
                continue;
            }
            pos++; // Consume the character
        }
        return src.substr(start, pos++ - start - 1); // Skip the closing quote
    }

    Token nextToken()
    {
        while (pos < src.size() && isspace(src[pos]))
            pos++;

        if (pos == src.size())
            return {T_EOF, "", 0};

        char current = src[pos];

        if (isdigit(current))
            return {T_NUM, consumeNumber(), 1}; // Line number is hardcoded for simplicity
        else if (isalpha(current) || current == '_')
        {
            string word = consumeWord();
            if (word == "int")
                return {T_INT, word, 1};
            else if (word == "float")
                return {T_FLOAT, word, 1};
            else if (word == "string")
                return {T_STRING, word, 1};
            else if (word == "bool")
                return {T_BOOL, word, 1};
            else if (word == "if")
                return {T_IF, word, 1};
            else if (word == "else")
                return {T_ELSE, word, 1};
            else if (word == "return")
                return {T_RETURN, word, 1};
            else if (word == "while")
                return {T_WHILE, word, 1};
            else if (word == "for")
                return {T_FOR, word, 1};
            return {T_ID, word, 1};
        }
        else if (current == '"')
            return {T_STRING, consumeString(), 1};

        // Handle operators and punctuation
        switch (current)
        {
        case '=':
            return {T_ASSIGN, "=", 1};
        case '+':
            return {T_PLUS, "+", 1};
        case '-':
            return {T_MINUS, "-", 1};
        case '*':
            return {T_MUL, "*", 1};
        case '/':
            return {T_DIV, "/", 1};
        case '(':
            return {T_LPAREN, "(", 1};
        case ')':
            return {T_RPAREN, ")", 1};
        case '{':
            return {T_LBRACE, "{", 1};
        case '}':
            return {T_RBRACE, "}", 1};
        case ';':
            return {T_SEMICOLON, ";", 1};
        case '>':
            return {T_GT, ">", 1};
        case '<':
            return {T_LT, "<", 1};
        case '!':
            if (src[pos + 1] == '=')
            {
                pos += 2; // Skip '!='
                return {T_NEQ, "!=", 1};
            }
            break;
        case '&':
            if (src[pos + 1] == '&')
            {
                pos += 2; // Skip '&&'
                return {T_LOGICAL_AND, "&&", 1};
            }
            break;
        case '|':
            if (src[pos + 1] == '|')
            {
                pos += 2; // Skip '||'
                return {T_LOGICAL_OR, "||", 1};
            }
            break;
        case '\0':
            return {T_EOF, "", 1}; // End of file
        }

        cout << "Error: Unrecognized character '" << current << "' on line 1." << endl;
        exit(1);
    }

    vector<Token> tokenize()
    {
        vector<Token> tokenList;
        Token token;
        do
        {
            token = nextToken();
            tokenList.push_back(token);
        } while (token.type != T_EOF);
        return tokenList;
    }
};

int main()
{
    string sourceCode = R"(
    int x;
    float y; 
    string name; 
    x = 5; 
    y = 4.5; 
    name = \"test\"; 
    if (x > y) 
    { return; } 
    else { x = y; }
    )";
    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
