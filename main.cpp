#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

enum class TokenType {
    NUMBER,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    OPEN_BRACKET,
    CLOSE_BRACKET,
    EQUAL_TO,
};

const char* TokenToString(TokenType token){
    switch (token){
        case TokenType::NUMBER: return "number";
    default:
        return "unknownToken";
    }
}

struct Token
{
    int position;
    TokenType tokenType;
    const char* token;
    int value;

    void printToken(){
        cout << position << ") " << TokenToString(tokenType) << " : " << token;  
    }
};



class Laxer {
private:
    vector<Token> Tokens;
    const char* input;
    int pos = 0;
    int size = strlen(input);

    char peek(int offset = 0){
        if((pos + offset) >= size){
            return '\0';
        }
        return input[pos + offset];
    }

    char consume(int offset = 1){
        if((pos + offset) >= size){
            cerr << "Error : Tried to consume charcter that didn't exits at " << 
                pos + offset << endl << " pos : " << pos << ", offset : " << offset << ", size : " << size;
            throw runtime_error("Lexer: consume out of bounds");
        }
        char character = input[pos + offset];
        pos += offset;
        return character;
    }

    Laxer(const char* Input) : input(Input){
    }

public: 
    

};



int main(){
    


    return 0;
}