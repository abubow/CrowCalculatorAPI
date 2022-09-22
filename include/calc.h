#ifndef calc_h
#define calc_h
#define CROW_MAIN


#include<tuple>
#include<string>
#include<stack>
#include<cmath>
//#include "json.hpp"
#include "crow.h"

class Calc {
public:
    Calc() = default;
    ~Calc() = default;
    double evaluate(const std::string& equation) const;
    std::string jsonResult(double result) const;
};


bool signPriority(char sign1, char sign2){
    /* compares the priorty of two signs in respect to PEMDAS */
    /* returns true if sign1 has higher priority than sign2 */
    if(sign1 == '(' || sign1 == ')'){
        return false;
    }
    else if(sign2 == '(' || sign2 == ')'){
        return true;
    }
    else if(sign1 == '^'){
        return true;
    }
    else if(sign2 == '^'){
        return false;
    }
    else if(sign1 == '*' || sign1 == 'd'){
        return true;
    }
    else if(sign2 == '*' || sign2 == 'd'){
        return false;
    }
    return true;
}
bool validParantheses(const std::string& equation){
    /* checks if the expression is valid */
    /* returns true if valid */
    int open = 0;
    int close = 0;
    for(int i = 0; i < equation.length(); i++){
        if(equation[i] == '('){
            open++;
        }
        else if(equation[i] == ')'){
            close++;
        }
        if(close > open){
            return false;
        }
    }
    if(open != close){
        return false;
    }
    return true;
}
std::pair<bool, double> get_num_from_stack(const std::string& equation, int& i){
    /* gets the number from the stack */
    /* returns the number and the index of the last digit */
    double num = 0;
    bool dotOnce = false;
    double decimal = 1;
    while(i < equation.length() && (equation[i] >= '0' && equation[i] <= '9' || equation[i] == '.')){
        if(equation[i] == '.'){
            if(dotOnce){
                return std::make_pair(false, 0);
            }
            dotOnce = true;
        }
        else{
            if(dotOnce){
                decimal /= 10;
                num += decimal * (equation[i] - '0');
            }
            else{
                num *= 10;
                num += equation[i] - '0';
            }
        }
        i++;
    }
    i--;
    return std::make_pair(true, num);
}
bool check_alpha(const std::string& equation){
    /* checks if the expression contains any letters */
    /* returns true if it does */
    for(int i = 0; i < equation.length(); i++){
        if((equation[i] >= 'a' && equation[i] <= 'z') || (equation[i] >= 'A' && equation[i] <= 'Z')){
            if( equation[i] != 'd')
                return true;
        }
    }
    return false;
}
double Calc::evaluate(const std::string& equation) const{
    std::stack<double> numstack;
    std::stack<char> opstack;
    if (!validParantheses(equation) || check_alpha(equation)){
        throw std::invalid_argument("Invalid expression");
    }
    for (auto it = equation.begin(); it != equation.end(); ++it){
        if (isdigit(*it)){
            //using get num from stack to get the number
            int i = it - equation.begin();
            auto num = get_num_from_stack(equation, i);
            if(!num.first){
                throw std::invalid_argument("Invalid number");
            }
            numstack.push(num.second);
            it += i - (it - equation.begin());
        }
        else if (*it == '('){
            opstack.push(*it);
        }
        else if (*it == ')'){
            while (opstack.top() != '('){
                double num2 = numstack.top();
                numstack.pop();
                double num1 = numstack.top();
                numstack.pop();
                char op = opstack.top();
                opstack.pop();
                if (op == '+'){
                    numstack.push(num1 + num2);
                }
                else if (op == '-'){
                    numstack.push(num1 - num2);
                }
                else if (op == '*'){
                    numstack.push(num1 * num2);
                }
                else if (op == 'd'){
                    numstack.push(num1 / num2);
                }
                else if (op == '^'){
                    numstack.push(pow(num1, num2));
                }
            }
            opstack.pop();
        }
        else if (*it == '+' || *it == '-' || *it == '*' || *it == 'd' || *it == '^'){
            while (!opstack.empty() && signPriority(opstack.top(), *it)){
                double num2 = numstack.top();
                numstack.pop();
                double num1 = numstack.top();
                numstack.pop();
                char op = opstack.top();
                opstack.pop();
                if (op == '+'){
                    numstack.push(num1 + num2);
                }
                else if (op == '-'){
                    numstack.push(num1 - num2);
                }
                else if (op == '*'){
                    numstack.push(num1 * num2);
                }
                else if (op == 'd'){
                    numstack.push(num1 / num2);
                }
                else if (op == '^'){
                    numstack.push(pow(num1, num2));
                }
            }
            opstack.push(*it);
        }
        else if (*it == ' '){
            continue;
        }
        else{
            throw std::invalid_argument("Invalid character in equation");
        }
    }
    while (!opstack.empty()){
        double num2 = numstack.top();
        numstack.pop();
        double num1 = numstack.top();
        numstack.pop();
        char op = opstack.top();
        opstack.pop();
        if (op == '+'){
            numstack.push(num1 + num2);
        }
        else if (op == '-'){
            numstack.push(num1 - num2);
        }
        else if (op == '*'){
            numstack.push(num1 * num2);
        }
        else if (op == 'd'){
            numstack.push(num1 / num2);
        }
        else if (op == '^'){
            numstack.push(pow(num1, num2));
        }
    }
    return numstack.top();
}

std::string Calc::jsonResult(double result) const{
    std::string json = "{ result: \"";
    json += std::to_string(result);
    json += "\" }";
    return json;
}

#endif /* calc_h */
