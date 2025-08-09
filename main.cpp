#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <memory>

using namespace std;

enum class TokenType {
    SPACE,
    UNKNOWN,
    ENDOFTOKEN,

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
    string token;
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

    unordered_map<char, TokenType> singleCharToken = {
        {'+', TokenType::PLUS},
        {'-', TokenType::MINUS},
        {'*', TokenType::STAR},
        {'/', TokenType::SLASH},
        {'=', TokenType::EQUAL_TO},

        {'(', TokenType::OPEN_BRACKET},
        {')', TokenType::CLOSE_BRACKET},
    };

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

    
public: 
    
    Laxer(const char* Input) : input(Input){
    }

    int parseInteger(){
        int result = 0;
        while(isdigit(peek()))
            result = result * 10 + (consume() - '0');
        return result;
    }

    vector<Token> Tokenizer(){
        while(peek() != '\0'){
            char ch = peek();
            
            // for number
            if(isdigit(ch)){
                int _pos = pos;
                int value = parseInteger();
                Tokens.push_back({_pos, TokenType::NUMBER, to_string(value), value});
            }
            else if(singleCharToken.count(ch)){
                Tokens.push_back({pos, singleCharToken[ch], string(1, ch), 0});
                consume();
            }
            else if(isspace(ch)){
                Tokens.push_back({pos, TokenType::SPACE, "space", 0});
                consume();
            }
            else{
                Tokens.push_back({pos,TokenType::UNKNOWN,string(1,ch),0});
                cerr<<"Error : Unexpected token" << ch << " position " << pos <<endl;
                consume();
            }
        }
        Tokens.push_back({pos, TokenType::ENDOFTOKEN, "EOFT", 0});
        return Tokens;
    }
};

struct ASTNode {
    virtual ~ASTNode(){};
    virtual void print() const = 0;
};

struct NumberNode : ASTNode {
    int value;
    NumberNode(int val) : value(val) {};
    void print() const override { cout << value; }
};

struct BinaryOperatorNode : ASTNode{
    string op;
    unique_ptr<ASTNode> left;
    unique_ptr<ASTNode> right;

    BinaryOperatorNode(unique_ptr<ASTNode> l,string o,unique_ptr<ASTNode> r) : left(move(l)),op(move(o)),right(move(r)) {};

    void print() const override{
        cout<< "( "; 
        left->print();
        cout<< " " << op << " ";
        right->print();
        cout << " )";
    }
};

class Parser {
private: 
    unique_ptr<Laxer> laxer;
    vector<Token> Tokens;
    int pos = 0;


    Token peek(int offset = 0){
        if(pos + offset >= Tokens.size())
            return {pos, TokenType::ENDOFTOKEN, "EOFT", 0};
        return Tokens[pos + offset];
    }

    Token consume(){
        if(pos < Tokens.size()) pos++;
        return Tokens[pos - 1];
    }

    bool match(TokenType expected){
        if(peek().tokenType == expected){
            consume();
            return true;
        }
        return false;
    }
};

int main(){
    
            

    return 0;
}
