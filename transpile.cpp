#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cctype>
using namespace std;

map<string, int> variables;

class Parser {
    string s;
    size_t pos;

public:
    Parser(const string& input) : s(input), pos(0) {}

    // Skip whitespace
    void skipSpaces() {
        while (pos < s.size() && isspace(s[pos])) pos++;
    }

    // Parse full expression
    int parseExpr() {
        int result = parseTerm();
        while (true) {
            skipSpaces();
            if (pos < s.size() && s[pos] == '+') {
                pos++;
                result += parseTerm();
            }
            else if (pos < s.size() && s[pos] == '-') {
                pos++;
                result -= parseTerm();
            }
            else break;
        }
        return result;
    }

private:
    int parseTerm() {
        int result = parseFactor();
        while (true) {
            skipSpaces();
            if (pos < s.size() && s[pos] == '*') {
                pos++;
                result *= parseFactor();
            }
            else if (pos < s.size() && s[pos] == '/') {
                pos++;
                int denom = parseFactor();
                if (denom == 0) throw runtime_error("Division by zero");
                result /= denom;
            }
            else break;
        }
        return result;
    }

    int parseFactor() {
        skipSpaces();
        if (pos >= s.size()) throw runtime_error("Unexpected end of input");

        if (isdigit(s[pos])) {
            // Parse number
            int num = 0;
            while (pos < s.size() && isdigit(s[pos])) {
                num = num * 10 + (s[pos] - '0');
                pos++;
            }
            return num;
        }
        else if (isalpha(s[pos])) {
            // Parse identifier
            string var;
            while (pos < s.size() && isalnum(s[pos])) {
                var.push_back(s[pos]);
                pos++;
            }
            if (variables.count(var)) {
                return variables[var];
            }
            else {
                throw runtime_error("Undefined variable: " + var);
            }
        }
        else if (s[pos] == '(') {
            pos++;
            int val = parseExpr();
            skipSpaces();
            if (pos >= s.size() || s[pos] != ')') {
                throw runtime_error("Missing closing parenthesis");
            }
            pos++;
            return val;
        }
        else {
            throw runtime_error(string("Unexpected char: ") + s[pos]);
        }
    }
};

// Evaluate expression with parser
int evalExpr(const string& expr) {
    Parser parser(expr);
    return parser.parseExpr();
}

int main() {
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
            // let x = expr
            string var, eq;
            ss >> var >> eq;
            string rest;
            getline(ss, rest);
            variables[var] = evalExpr(rest);
        }
        else if (variables.count(keyword)) {
            // variable reassignment: x = expr
            string eq;
            ss >> eq;
            string rest;
            getline(ss, rest);
            if (eq != "=") {
                cout << "Syntax error: expected '='\n";
                continue;
            }
            variables[keyword] = evalExpr(rest);
        }
        else if (keyword == "shabd") {
            string rest;
            getline(ss, rest);
            try {
                cout << evalExpr(rest) << endl;
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        else {
            cout << "Unknown command.\n";
        }
    }
}
