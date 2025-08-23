#pragma once

#include "utility.hpp"

class Parser {
private: 
    std::vector<Token> Tokens;
    int pos = 0;

    Token peek(int offset = 0);
    Token consume();
    bool match(TokenType expected);
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePostfix();
    std::unique_ptr<ASTNode> parsePower();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseExpression();

public:
    Parser(std::vector<Token> toks): Tokens(move(toks)){}
    
    std::unique_ptr<ASTNode> parse();
    
};
