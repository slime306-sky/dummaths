#include "PrettyPrint.hpp"

std::string PrettyPrint::astToString(const ASTNode* node, bool isRoot){
    if(const NumberNode* n = dynamic_cast<const NumberNode*>(node)){
        std::ostringstream oss;
        oss << std::setprecision(12) << n->value;
        std::string s = oss.str();
        if(s.find('.') != std::string::npos){
            while(!s.empty() && s.back() == '0') s.pop_back();
            if(!s.empty() && s.back() == '.') s.pop_back(); 
        }
        return s;
    }
    if(const BinaryOperatorNode* b = dynamic_cast<const BinaryOperatorNode*>(node)){
        std::string left = astToString(b->left.get(), false);
        std::string right = astToString(b->right.get(), false);
        std::string expr = left + " " + b->op + " " + right;
        if(!isRoot) return "( " + expr + " )";
        return expr;
    }
    return "?";
}

void PrettyPrint::findDeepestReducible(std::unique_ptr<ASTNode>& node, std::unique_ptr<ASTNode>** bestPtr, int depth, int& bestDepth){
    if(!node) return;
    if(BinaryOperatorNode* b = dynamic_cast<BinaryOperatorNode*>(node.get())){
        bool leftIsNum = dynamic_cast<NumberNode*>(b->left.get()) != nullptr;
        bool rightIsNum = dynamic_cast<NumberNode*>(b->right.get()) != nullptr;
        if(leftIsNum && rightIsNum){
            if(depth > bestDepth){
                bestDepth = depth;
                *bestPtr = &node;
            }
        }
        // recurse left then right to prefer leftmost at same depth
        findDeepestReducible(b->left, bestPtr, depth+1, bestDepth);
        findDeepestReducible(b->right, bestPtr, depth+1, bestDepth);
    }
}

double PrettyPrint::applyOp(double a, double b, const std::string& op){
    if(op == "+") return a + b;
    else if(op == "-") return a - b;
    else if(op == "*") return a * b;
    else if(op == "/"){
        if(b == 0.0) throw std::runtime_error("Error evaluate op: Division by zero");
        return a / b;
    }
    else if(op == "^") return pow(a, b);
    else if(op == "%"){
        long long ia = static_cast<long long>(a);
        long long ib = static_cast<long long>(b);
        if(ib == 0 ) throw std::runtime_error("Error evaluate op: Modulo by zero");
        return ia % ib;
    }
    else if(op == "!"){
        if(a < 0 || floor(a) != 0) throw std::runtime_error("Error evaluate op: fact for only non-neg int");
        long long n = static_cast<long long>(a);
        long long res = 1;
        for(long long i = 2; i <= n; i++) res *= i;
        return res;
    }
    throw std::runtime_error("Error evaluate op: Unknown operator " + op);
}

void PrettyPrint::print(std::unique_ptr<ASTNode> root){
    // Print the original expression (nicely)
    std::cout << "\n" << astToString(root.get(), true) << "\n";

    // Rewriting loop
    int step = 1;
    while(dynamic_cast<NumberNode*>(root.get()) == nullptr){
        std::unique_ptr<ASTNode>* bestPtr = nullptr;
        int bestDepth = -1;
        findDeepestReducible(root, &bestPtr, 0, bestDepth);
        if(!bestPtr){
            std::cerr << "No reducible node found (malformed AST?)\n";
            return;
        }

        BinaryOperatorNode* bin = dynamic_cast<BinaryOperatorNode*>((*bestPtr).get());
        NumberNode* L = dynamic_cast<NumberNode*>(bin->left.get());
        NumberNode* R = dynamic_cast<NumberNode*>(bin->right.get());
        if(!L || !R){
            std::cerr << "Internal error: expected number children\n";
            return;
        }

        double res = applyOp(L->value, R->value, bin->op);

        *bestPtr = std::make_unique<NumberNode>(res);

        std::cout << "=> " << astToString(root.get(), true) << "\n";
    }

    // only after the loop is done
    NumberNode* finalN = dynamic_cast<NumberNode*>(root.get());
    std::cout << "\nFinal Answer: ";
    finalN->print();
    std::cout << "\n";

}

