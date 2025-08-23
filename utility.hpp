#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cmath>

enum class TokenType {
    SPACE,
    UNKNOWN,
    ENDOFTOKEN,

    NUMBER,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    EXPONENT,
    MODULO,
    FACTORIAL,

    OPEN_BRACKET,
    CLOSE_BRACKET,
    EQUAL_TO,
};

inline const char* TokenToString(TokenType token){
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
    std::string token;
    double value;

    void printToken(){
        std::cout << position << ") " << TokenToString(tokenType) << " : " << token << std::endl;  
    }
};


// *****************************************************
// *                                                   *   
// *                    AST NODES                      *   
// *                                                   *   
// *****************************************************

struct ASTNode {
    virtual ~ASTNode(){};
    virtual void print() const = 0;
};

struct NumberNode : ASTNode {
    double value;
    NumberNode(double val) : value(val) {};
    void print() const override { std::cout << value; }
};

struct BinaryOperatorNode : ASTNode{
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOperatorNode(std::unique_ptr<ASTNode> l, std::string o, std::unique_ptr<ASTNode> r) : left(move(l)),op(move(o)),right(move(r)) {};

    void print() const override{
        std::cout<< "( "; 
        left->print();
        std::cout<< " " << op << " ";
        right->print();
        std::cout << " )";
    }
};
