#include "Laxer.hpp"

char Laxer::peek(int offset){
    if((pos + offset) >= size){
        return '\0';
    }
    return input[pos + offset];
}

char Laxer::consume(int offset){
    if((pos + offset - 1) >= size){
        std::cerr << "Error : Tried to consume charcter that didn't exits at " << 
            pos + offset << std::endl << " pos : " << pos << ", offset : " << offset << ", size : " << size << std::endl;
        throw std::runtime_error("Lexer: consume out of bounds");
    }
    char character = input[pos];
    pos += offset;
    return character;
}

double Laxer::parseInteger(){
    std::string numstr;
    bool hasDot = false;

    while(isdigit(peek()) || peek() == '.'){
        if(peek() == '.'){
            if(hasDot) break;
            hasDot = true;
        }
        numstr.push_back(consume());
    }
    return stod(numstr);
}

std::vector<Token> Laxer::Tokenizer(){
    while(peek() != '\0'){
        char ch = peek();
        
        // for number
        if(isdigit(ch) || ch == '.'){
            int _pos = pos;
            double value = parseInteger();
            Tokens.push_back({_pos, TokenType::NUMBER, std::to_string(value), value});
        }
        else if(singleCharToken.count(ch)){
            Tokens.push_back({pos, singleCharToken[ch], std::string(1, ch), 0});
            consume();
        }
        else if(isspace(ch)){
            // Tokens.push_back({pos, TokenType::SPACE, "space", 0});
            consume();
        }
        else{
            Tokens.push_back({pos, TokenType::UNKNOWN, std::string(1,ch), 0});
            std::cerr<<"Error : Unexpected token" << ch << " position " << pos << std::endl;
            consume();
        }
    }
    Tokens.push_back({pos, TokenType::ENDOFTOKEN, "EOFT", 0});
    return Tokens;
}