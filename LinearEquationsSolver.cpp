/**
Program that calculates linear equations
tested with examples:
	10+5*4+x=60
	12*3+5-x=1+x+1*2
	12*3+5-x=1+x+1*2
**/

#include <cstdlib>
#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <map>

using namespace std;

double StringToDouble ( char ch )
{
    stringstream ss;
    ss << ch;
    double result;
    return ss >> result ? result : 0;
}

string ToString( char ch )
{
    stringstream ss;
    ss << ch;
    string str;
    ss >> str;
    return str;
}

// Function devides the equation by the delimiter symbols
multimap<string, string> SplitEquation(string equation, vector<string> delimiters)
{
    //cout << "SplitEquation:: " << equation << " " << delimiters[0] << endl;
    multimap<string, string> parts;

    while(equation.length() > 0){
        //cout << equation << endl;
        string operation;
        int start_pos = 0;
        if( find( delimiters.begin(), delimiters.end(), ToString(equation[0]) ) != delimiters.end() ){
            start_pos = 1;
            operation = equation[0];
        }
        else{
            operation = "+";
        }

        int i = start_pos;
        for(; i < equation.length(); i++){
            if( find(delimiters.begin(), delimiters.end(), ToString(equation[i])) != delimiters.end() ){
                break;
            }
        }

        string equation_part = equation.substr(start_pos, i);
        parts.insert(make_pair(operation, equation_part));
        equation.erase(0, i);
    }

    //cout << "returns size : " << parts.size() << endl;
    return parts;
}

// Function takes already separated parts of the equation consisting only of digits and symbols like * un /
void CalculatePart(string operation, string sub_equation, bool after_equal_sign, double& x_sum, double& non_x_sum)
{
    //cout << "CalculatePart:: " << operation << " " << sub_equation << endl;
    bool is_x = (sub_equation.find("x") != string::npos);

    vector<double> stack;
    string last_op = "";
    for(int i = 0; i < sub_equation.length(); i++){

        if( isdigit(sub_equation[i]) ) {
            //cout << sub_equation[i] << endl;
            if( stack.empty() || (last_op != "" && stack.size() == 1) ){
                stack.push_back(0);
            }
            //cout << "size: " << stack.size() << endl;
            int cid = stack.size()-1;
            stack[cid] = stack[cid]*10 + StringToDouble(sub_equation[i]);
        }
        // save the variable so we can display the correctly formatted answer x = ...)
        else if( isalpha(sub_equation[i]) ) {
            // ignore the variable name
        }
        else if(sub_equation[i] == '*' || sub_equation[i] == '/'){
            last_op = ToString(sub_equation[i]);
            if(stack.size() == 2){
                stack[0] = ( last_op == "*" ? (stack[0]*stack[1]) : (stack[0]/stack[1]) );
                stack.pop_back();
                last_op = "";
            }

        }
    }

    if(stack.size() == 2){
        stack[0] = (last_op == "*" ? (stack[0]*stack[1]) : (stack[0]/stack[1]));
        //cout << stack[0] << last_op << stack[1];
        stack.pop_back();
    }

    if(is_x){
        if(stack.empty()){
            stack.push_back(1);
        }
        x_sum += (after_equal_sign ? -1 : 1) * (operation == "+" ? stack[0] : -1*stack[0]);
    }
    else{
        non_x_sum += (!after_equal_sign ? -1 : 1) * (operation == "+" ? stack[0] : -1*stack[0]);
    }

    //cout << "non_x: " << non_x_sum << " x_sum: " << x_sum << endl;
}

double FindRoot(double non_x_sum, double x_sum)
{
    double result = non_x_sum / x_sum;
    result = round(result * 10000) / 10000;
    return result;
}

int main(int argc, char** argv) {

    string equation;
    getline(cin, equation);

    // split equation into sections by "="
    vector<string> delimiters;
    delimiters.push_back("=");
    multimap<string, string> equation_sides = SplitEquation(equation, delimiters);
    delimiters.clear();
    delimiters.push_back("+");
    delimiters.push_back("-");

    double non_x_sum = 0;
    double x_sum = 0;
    bool after_equal_sign = false;

    for(multimap<string, string>::iterator it = equation_sides.begin(); it != equation_sides.end(); it++){
        multimap<string, string> parts = SplitEquation(it->second, delimiters);
        //cout << "parts: " << parts.size() << endl;
        for(multimap<string, string>::iterator it2 = parts.begin(); it2 != parts.end(); it2++){

            CalculatePart(it2->first, it2->second, after_equal_sign, x_sum, non_x_sum);

        }
        after_equal_sign = true;
    }

    double result = FindRoot(non_x_sum, x_sum);
    cout << "x = " << result;
    return 0;
}
