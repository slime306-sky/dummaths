#pragma once

#include "utility.hpp"

class Laxer {
private:
    std::vector<Token> Tokens;
    const char* input;
    int pos;
    int size;

    std::unordered_map<char, TokenType> singleCharToken = {
        {'+', TokenType::PLUS},
        {'-', TokenType::MINUS},
        {'*', TokenType::STAR},
        {'/', TokenType::SLASH},
        
        {'=', TokenType::EQUAL_TO},
        
        {'^', TokenType::EXPONENT},
        {'%', TokenType::MODULO},
        {'!', TokenType::FACTORIAL},

        {'(', TokenType::OPEN_BRACKET},
        {')', TokenType::CLOSE_BRACKET},
    };
    char peek(int offset = 0);
    char consume(int offset = 1);
    double parseInteger();

public: 
    
    Laxer(const char* Input) : input(Input), pos(0), size(strlen(input)){
    }
    
    std::vector<Token> Tokenizer();

};
