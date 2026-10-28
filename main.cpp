#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>

using namespace std;
// Функція розбиває вираз на токени
vector<string> toToken(const string &exp) {
    vector<string> tokens;
    int n = exp.length();
    int i = 0;
    while(i<n) {
        if (isspace(exp[i])) {
            i++;
            continue;
        }
        if (isdigit(exp[i])) {
            string numb = "";
            while (i<n && isdigit(exp[i])) {
                numb += exp[i];
                i++;
            }
            tokens.push_back(numb);
            continue;
        }
        if (exp.substr(i,3) == "sin") {
            tokens.push_back("sin");
            i += 3;
            continue;
        }
        if (exp.substr(i,3) == "cos") {
            tokens.push_back("cos");
            i += 3;
            continue;
        }
        if (exp.substr(i,4) == "ceil") {
            tokens.push_back("ceil");
            i += 4;
            continue;
        }
        if (exp.substr(i,4) == "tanh") {
            tokens.push_back("tanh");
            i += 4;
            continue;
        }
        string op(1,exp[i]);
        if (op == "+" || op == "-") {
            if (tokens.empty() || tokens.back() == "+" || tokens.back() == "-" || tokens.back() == "*" || tokens.back() == "/" || tokens.back() == "%" || tokens.back() == "(") {
                if (op == "+") {
                    op = "u+";
                }else {
                    op = "u-";
                }
            }
        }
        tokens.push_back(op);
        i++;
    }
    return tokens;
}
// Допоміжна функція, яка повертає пріоритетність операції
int getPriority(const string& op) {
    if (op == "u+" || op == "u-" || op == "sin" || op == "cos" || op == "ceil" || op == "tanh") return 4;
    if (op == "%") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}
// Функція, яка переводить вираз в обернену польску нотацію
vector<string> toOPN(vector<string> &tokens) {
    stack<string> ops;
    vector<string> ONP;
    for (int i = 0; i < tokens.size(); i++) {
        if (isdigit(tokens[i][0])) {
            ONP.push_back(tokens[i]);
            continue;
        }
        if (tokens[i] == "(") {
            ops.push(tokens[i]);
            continue;
        }
        if (tokens[i] == ")") {
            while (!ops.empty() && ops.top() != "(") {
                ONP.push_back(ops.top());
                ops.pop();
            }
            if (!ops.empty() && ops.top() == "(") {
                ops.pop();
            }
            continue;
        }
        while (!ops.empty() && getPriority(tokens[i]) <= getPriority(ops.top())) {
            ONP.push_back(ops.top());
            ops.pop();
        }
        ops.push(tokens[i]);
    }
    while (!ops.empty()) {
        ONP.push_back(ops.top());
        ops.pop();
    }
    return ONP;
}
// Функція, яка обчислює значення виразу в ОПН
double result (vector<string> &ONP){
    stack<double> calc;
    for (string &token : ONP) {
        if (isdigit(token[0])) {
            calc.push(stoi(token));
        }else if (!calc.empty()) {
            if (getPriority(token) == 4) {
                double a = calc.top(); calc.pop();
                if (token == "u+"){calc.push(a);}
                if (token == "u-"){calc.push(-a);}
                if (token == "sin"){calc.push(sin(a));}
                if (token == "cos"){calc.push(cos(a));}
                if (token == "ceil"){calc.push(ceil(a));}
                if (token == "tanh"){calc.push(tanh(a));}
            }else {
                double a = calc.top(); calc.pop();
                double b = calc.top(); calc.pop();
                switch (token[0]) {
                    case '+': {calc.push(b+a); break;}
                    case '-': {calc.push(b-a); break;}
                    case '*': {calc.push(b*a); break;}
                    case '/': {calc.push(b/a); break;}
                    case '%': {calc.push(static_cast<int>(b) % static_cast<int>(a)); break;}
                }
            }
        }

    }
    return calc.top();
}

int main() {
    cout << "Введіть вираз" << endl;
    string expression;
    getline(cin, expression);
    vector<string> tokens = toToken(expression);
    for (const string &token : tokens) {
        cout << token << "\t";
    }
    cout << "\n";
    vector<string> OPN = toOPN(tokens);
    for (const string &token : OPN) {
        cout << token << "\t";
    }
    cout << "\n" << result(OPN) << endl;
    return 0;
}