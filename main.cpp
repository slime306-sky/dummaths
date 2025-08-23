#include "Laxer.hpp"
#include "Parser.hpp"
#include "PrettyPrint.hpp"

using namespace std;

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