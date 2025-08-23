#include "Parser.hpp"

Token Parser::peek(int offset){
    if(pos + offset >= Tokens.size())
        return {pos, TokenType::ENDOFTOKEN, "EOFT", 0};
    return Tokens[pos + offset];
}

Token Parser::consume(){
    if(pos < Tokens.size()) pos++;
    return Tokens[pos - 1];
}

bool Parser::match(TokenType expected){
    if(peek().tokenType == expected){
        consume();
        return true;
    }
    return false;
}

std::unique_ptr<ASTNode> Parser::parsePrimary(){
    if(match(TokenType::NUMBER))
        return std::make_unique<NumberNode>(Tokens[pos-1].value); // match consumed token (pos++) that's why we did pos-1 to access the previous token's value
    
    else if (match(TokenType::OPEN_BRACKET)){
        auto expr = parseExpression();
        if(!match(TokenType::CLOSE_BRACKET)) throw std::runtime_error("Parser Error: Missing closing bracket\n");
        return expr;
    }
    throw std::runtime_error("Parser Error: Unknown Token in parsePrimery\n");
}

std::unique_ptr<ASTNode> Parser::parseUnary(){
    if(match(TokenType::PLUS))
        return parseUnary();
    else if (match(TokenType::MINUS))   {
        auto node = parseUnary();
        return std::make_unique<BinaryOperatorNode>(std::make_unique<NumberNode>(0), "-", move(node));
    }
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePostfix(){
    auto node = parseUnary();
    while(match(TokenType::FACTORIAL)){
        node = std::make_unique<BinaryOperatorNode>(move(node), "!", nullptr);
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parsePower(){
    auto node = parsePostfix();
    while(match(TokenType::EXPONENT)){
        auto right = parsePower();
        node = std::make_unique<BinaryOperatorNode>(move(node), "^", move(right));
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parseTerm(){
    auto node = parsePower();
    while (true){
        if(match(TokenType::STAR)){
            auto right = parsePower();
            node = std::make_unique<BinaryOperatorNode>(move(node), "*", move(right));
        }
        else if(match(TokenType::SLASH)){
            auto right = parsePower();
            node = std::make_unique<BinaryOperatorNode>(move(node), "/", move(right));
        }
        else if(match(TokenType::MODULO)){
            auto right = parsePower();
            node = std::make_unique<BinaryOperatorNode>(move(node), "%", move(right));
        }
        else break;
    }
    return node;         
}

std::unique_ptr<ASTNode> Parser::parseExpression(){
    auto node = parseTerm();
    while (true){
        if(match(TokenType::PLUS)){
            auto right = parseTerm();
            node = std::make_unique<BinaryOperatorNode>(move(node), "+", move(right));
        }
        else if(match(TokenType::MINUS)){
            auto right = parseTerm();
            node = std::make_unique<BinaryOperatorNode>(move(node), "-", move(right));
        }
        else break;
    }
    return node;         
}

std::unique_ptr<ASTNode> Parser::parse(){ 
    return parseExpression();
}