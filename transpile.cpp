#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

int main() {
    map<string, int> variables;
    string line;

    cout << "MohakLang REPL (type 'exit' to quit)\n";

    while (true) {
        cout << ">> ";
        getline(cin, line);
        if (line == "exit") break;

        stringstream ss(line);
        string keyword;
        ss >> keyword;

        if (keyword == "mano") {
            string var, eq;
            int val;
            ss >> var >> eq >> val;
            variables[var] = val;
        }
        else if (keyword == "shabd") {
            string expr;
            ss >> expr;
            if (variables.count(expr)) {
                cout << variables[expr] << endl;
            }
            else {
                cout << expr << endl;
            }
        }
        else {
            cout << "Unknown command.\n";
        }
    }
}
