#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cmath>

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

    EXPONENT,
    MODULO,
    FACTORIAL,

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
    double value;

    void printToken(){
        cout << position << ") " << TokenToString(tokenType) << " : " << token;  
    }
};



class Laxer {
private:
    vector<Token> Tokens;
    const char* input;
    int pos;
    int size;

    unordered_map<char, TokenType> singleCharToken = {
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

    char peek(int offset = 0){
        if((pos + offset) >= size){
            return '\0';
        }
        return input[pos + offset];
    }

    char consume(int offset = 1){
        if((pos + offset - 1) >= size){
            cerr << "Error : Tried to consume charcter that didn't exits at " << 
                pos + offset << endl << " pos : " << pos << ", offset : " << offset << ", size : " << size << endl;
            throw runtime_error("Lexer: consume out of bounds");
        }
        char character = input[pos];
        pos += offset;
        return character;
    }

    double parseInteger(){
        string numstr;
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
    
public: 
    
    Laxer(const char* Input) : input(Input), pos(0), size(strlen(input)){
    }


    vector<Token> Tokenizer(){
        while(peek() != '\0'){
            char ch = peek();
            
            // for number
            if(isdigit(ch) || ch == '.'){
                int _pos = pos;
                double value = parseInteger();
                Tokens.push_back({_pos, TokenType::NUMBER, to_string(value), value});
            }
            else if(singleCharToken.count(ch)){
                Tokens.push_back({pos, singleCharToken[ch], string(1, ch), 0});
                consume();
            }
            else if(isspace(ch)){
                // Tokens.push_back({pos, TokenType::SPACE, "space", 0});
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
    double value;
    NumberNode(double val) : value(val) {};
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

    unique_ptr<ASTNode> parsePrimary(){
        if(match(TokenType::NUMBER))
            return make_unique<NumberNode>(Tokens[pos-1].value); // match consumed token (pos++) that's why we did pos-1 to access the previous token's value
        
        else if (match(TokenType::OPEN_BRACKET)){
            auto expr = parseExpression();
            if(!match(TokenType::CLOSE_BRACKET)) throw runtime_error("Parser Error: Missing closing bracket\n");
            return expr;
        }
        throw runtime_error("Parser Error: Unknown Token in parsePrimery\n");
    }

    unique_ptr<ASTNode> parseUnary(){
        if(match(TokenType::PLUS))
            return parseUnary();
        else if (match(TokenType::MINUS))   {
            auto node = parseUnary();
            return make_unique<BinaryOperatorNode>(make_unique<NumberNode>(0), "-", move(node));
        }
        return parsePrimary();
    }

    unique_ptr<ASTNode> parsePostfix(){
        auto node = parseUnary();
        while(match(TokenType::FACTORIAL)){
            node = make_unique<BinaryOperatorNode>(move(node), "!", nullptr);
        }
        return node;
    }

    unique_ptr<ASTNode> parsePower(){
        auto node = parsePostfix();
        while(match(TokenType::EXPONENT)){
            auto right = parsePower();
            node = make_unique<BinaryOperatorNode>(move(node), "^", move(right));
        }
        return node;
    }

    unique_ptr<ASTNode> parseTerm(){
        auto node = parsePower();
        while (true){
            if(match(TokenType::STAR)){
                auto right = parsePower();
                node = make_unique<BinaryOperatorNode>(move(node), "*", move(right));
            }
            else if(match(TokenType::SLASH)){
                auto right = parsePower();
                node = make_unique<BinaryOperatorNode>(move(node), "/", move(right));
            }
            else if(match(TokenType::MODULO)){
                auto right = parsePower();
                node = make_unique<BinaryOperatorNode>(move(node), "%", move(right));
            }
            else break;
        }
        return node;         
    }

    unique_ptr<ASTNode> parseExpression(){
        auto node = parseTerm();
        while (true){
            if(match(TokenType::PLUS)){
                auto right = parseTerm();
                node = make_unique<BinaryOperatorNode>(move(node), "+", move(right));
            }
            else if(match(TokenType::MINUS)){
                auto right = parseTerm();
                node = make_unique<BinaryOperatorNode>(move(node), "-", move(right));
            }
            else break;
        }
        return node;         
    }

public:
    Parser(vector<Token> toks): Tokens(move(toks)){}

    unique_ptr<ASTNode> parse(){ 
        return parseExpression();
    }
};
class PrettyPrint{
private:
    string astToString(const ASTNode* node, bool isRoot = true){
        if(const NumberNode* n = dynamic_cast<const NumberNode*>(node)){
            ostringstream oss;
            oss << setprecision(12) << n->value;
            string s = oss.str();
            if(s.find('.') != string::npos){
                while(!s.empty() && s.back() == '0') s.pop_back();
                if(!s.empty() && s.back() == '.') s.pop_back(); 
            }
            return s;
        }
        if(const BinaryOperatorNode* b = dynamic_cast<const BinaryOperatorNode*>(node)){
            string left = astToString(b->left.get(), false);
            string right = astToString(b->right.get(), false);
            string expr = left + " " + b->op + " " + right;
            if(!isRoot) return "( " + expr + " )";
            return expr;
        }
        return "?";
    }

    void findDeepestReducible(unique_ptr<ASTNode>& node, unique_ptr<ASTNode>** bestPtr, int depth, int& bestDepth){
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

    double applyOp(double a, double b, const string& op){
        if(op == "+") return a + b;
        else if(op == "-") return a - b;
        else if(op == "*") return a * b;
        else if(op == "/"){
            if(b == 0.0) throw runtime_error("Error evaluate op: Division by zero");
            return a / b;
        }
        else if(op == "^") return pow(a, b);
        else if(op == "%"){
            long long ia = static_cast<long long>(a);
            long long ib = static_cast<long long>(b);
            if(ib == 0 ) throw runtime_error("Error evaluate op: Modulo by zero");
            return ia % ib;
        }
        else if(op == "!"){
            if(a < 0 || floor(a) != 0) throw runtime_error("Error evaluate op: fact for only non-neg int");
            long long n = static_cast<long long>(a);
            long long res = 1;
            for(long long i = 2; i <= n; i++) res *= i;
            return res;
        }
        throw runtime_error("Error evaluate op: Unknown operator " + op);
    }

public:
      void print(unique_ptr<ASTNode> root){
        // Print the original expression (nicely)
        cout << "\n" << astToString(root.get(), true) << "\n";

        // Rewriting loop
        int step = 1;
        while(dynamic_cast<NumberNode*>(root.get()) == nullptr){
            unique_ptr<ASTNode>* bestPtr = nullptr;
            int bestDepth = -1;
            findDeepestReducible(root, &bestPtr, 0, bestDepth);
            if(!bestPtr){
                cerr << "No reducible node found (malformed AST?)\n";
                return;
            }

            // bestPtr points to a unique_ptr holding a BinaryOperatorNode whose children are both NumberNode
            BinaryOperatorNode* bin = dynamic_cast<BinaryOperatorNode*>((*bestPtr).get());
            NumberNode* L = dynamic_cast<NumberNode*>(bin->left.get());
            NumberNode* R = dynamic_cast<NumberNode*>(bin->right.get());
            if(!L || !R){
                cerr << "Internal error: expected number children\n";
                return;
            }

            double a = L->value;
            double b = R->value;
            double res;
            try {
                res = applyOp(a, b, bin->op);
            } catch(const std::exception& e){
                cerr << "Evaluation error: " << e.what() << endl;
                return;
            }

            // Replace the binary node with a NumberNode(result)
            *bestPtr = make_unique<NumberNode>(res);

            // print the rewritten expression
            cout << "=> " << astToString(root.get(), true) << "\n";
            //step++;
        }

        // final answer
        NumberNode* finalN = dynamic_cast<NumberNode*>(root.get());
        cout << "\nFinal Answer: ";
        finalN->print();
        cout << "\n";

    }

};

int main(){
    bool running = true;
    while(running){

        string input;
        cout << "Enter Expression or q to quit : ";
        getline(cin, input);
        if (input == "q") {running = false; break;}
        Laxer laxer(input.c_str());
        vector<Token> tokens = laxer.Tokenizer();
        
        Parser parser(tokens);
        unique_ptr<ASTNode> ast = parser.parse();
        
        cout << "Parsed expresstion (AST): " << endl;
        ast->print();
        cout << "\n\n";
        
        int step = 1;
        PrettyPrint prettyPrint;
        prettyPrint.print(move(ast));
    }

    return 0;
}
