#pragma once

#include "utility.hpp"

class PrettyPrint{
private:

    std::string astToString(const ASTNode* node, bool isRoot = true);
    void findDeepestReducible(std::unique_ptr<ASTNode>& node, std::unique_ptr<ASTNode>** bestPtr, int depth, int& bestDepth);
    double applyOp(double a, double b, const std::string& op);

public:
      
    void print(std::unique_ptr<ASTNode> root);

};
