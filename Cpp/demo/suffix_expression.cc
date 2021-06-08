#define _GNU_SOURCE
#include <string>
#include <memory>
#include <stack>
#include <map>
#include <queue>
#include <algorithm>
#include <iostream> 
#include <unistd.h> 
 

using namespace std;

int getPriority(const char& ch) {
    if(ch == '+' || ch == '-') return 1;
    if(ch == '*' || ch == '/') return 2;
    return -1;
}

bool isDigit(char ch) {
	if (ch >= '0' && ch <= '9') {
		return true;
	}
	return false;
}

bool isOperator(string str) {
	if (str[0] >= '0' && str[0] <= '9') {
		return false;
	}
	return true;
}

int calculate(int left, int right, const string& op) {
	int result = 0;
    const char& ch = op[0];
	switch (ch)
	{
	case '+':
		result = left + right;
		break;
	case '-':
		result = left - right;
		break;
	case '*':
		result = left * right;
		break;
	case '/':
		result = left / right;
		break;
	}
	return result;
}


int getSuffix(const string& expression) {
    vector<string> suffix;
    stack<char> operators;

    size_t i = 0, len = expression.size();
	while(i < len) {
        char cur = expression[i];
        if(cur == ' ') {
            ++i;
            continue;
        }
		if(isDigit(cur)) {     // 数字直接入栈
            size_t start = i;
            while(isDigit(expression[i])) {
                ++i;
            }
            suffix.push_back(expression.substr(start, i - start));
        }
		else {
		    if(operators.empty() || cur == '(') {
		        operators.push(cur);
	        }
	        else {
		        if(cur != ')') {    // " + - * / "
			        while (!operators.empty() && getPriority(operators.top()) >= getPriority(cur)){
                        suffix.emplace_back(1, operators.top());
                        operators.pop();
			        }
			        operators.push(cur);
		        }
		        else { // 右括号：将栈中左括号之后入栈的运算符全部出栈到表达式结果，左括号出栈
                    while (operators.top() != '(') {
                        suffix.emplace_back(1, operators.top());
                        operators.pop();
                    }
			        operators.pop();
		        }
	        }
			++i;
		}
	}
    while(!operators.empty()) {
        suffix.emplace_back(1, operators.top());
	    operators.pop();
    }

    stack<int> nums;
    for(const auto& str : suffix) {
        if(isOperator(str)) {
            int num_1 = nums.top();
            nums.pop();
            int num_2 = nums.top();
            nums.pop();
        
            nums.push(calculate(num_2, num_1, str));
        }
        else {
            nums.push(atoi(str.c_str()));
        }
    }
    return nums.top();
}

int main() {
    char buffer[1024];   
    getcwd(buffer, sizeof(buffer)); 
    string path(buffer);
    size_t idx = path.find("Cpp");
    cout << path << ":" << idx  << endl;
    string test = "21+((42-2)*15+6)-18";
    cout << getSuffix(test) << endl;
}