/**
 * 遍历中缀表达式的每个字符：
 * 初始化：创建一个空栈（存运算符），一个输出队列（存后缀表达式）
情况 1：遇到操作数（数字 / 字母）：直接加入输出队列；
情况 2：遇到左括号 (：直接入栈（左括号是 “优先级提升” 的标志）；
情况 3：遇到右括号 )：将栈顶运算符依次弹出并加入输出队列，直到遇到左括号 (，然后弹出左括号（不加入输出）；
情况 4：遇到运算符（+、-、*、/）：
循环判断：栈非空 且 栈顶不是左括号 且 栈顶运算符优先级 ≥ 当前运算符优先级 → 弹出栈顶运算符到输出队列；
循环结束后，将当前运算符入栈；
遍历结束后：将栈中剩余的所有运算符依次弹出，加入输出队列；
最终输出队列：即为后缀表达式（元素间用空格分隔）。
 */

#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

const char OPERATOR[] = {'+', '-', '*', '/', '(', ')'};
enum Precedence
{
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3,
    L_PAREN = 4,
    R_PAREN = 5
};
/**
 * @brief 获取运算符的优先级
 * @param [in] op 运算符
 * @return 优先级数值，数值越大优先级越高
 */
int getPrecedence(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return 0;
    }
}

/**
 * @brief 中缀表达式转后缀表达式
 * @param [in] s 中缀表达式字符串
 * @return 后缀表达式字符串
 */
string infixToPostfix(const string& s)
{
    if (s.empty())
    {
        return {};
    }

    stack<char> st;
    queue<char> que;

    for (auto c : s)
    {
        // 操作数直接加入输出队列
        if ('9' >= c && '0' <= c)
        {
            que.push(c);
        }
        // 左括号入栈
        else if (OPERATOR[L_PAREN] == c)
        {
            st.push(c);
        }
        // 运算符处理
        else if (OPERATOR[ADD] == c || OPERATOR[SUB] == c || OPERATOR[MUL] == c || OPERATOR[DIV] == c)
        {
            // 循环判断：栈非空 且 栈顶不是左括号 且 栈顶运算符优先级 ≥ 当前运算符优先级
            while (!st.empty() 
            && OPERATOR[L_PAREN] != st.top() 
            && getPrecedence(st.top()) >= getPrecedence(c))
            {
                // 弹出栈顶运算符到输出队列
                que.push(st.top());
                st.pop();
            }
            // 循环结束后，将当前运算符入栈
            st.push(c);
        }
        else if (OPERATOR[R_PAREN] == c)
        {
            // 将栈顶运算符依次弹出并加入输出队列，直到遇到左括号 (，
            while (!st.empty() && OPERATOR[L_PAREN] != st.top())
            {
                que.push(st.top());
                st.pop();
            }

            // 然后弹出左括号（不加入输出）
            if (!st.empty() && OPERATOR[L_PAREN] == st.top())
            {
                st.pop();
            }
        }
    }

    while (!st.empty())
    {
        que.push(st.top());
        st.pop();
    }

    string result;
    while (!que.empty())
    {
        result += que.front();
        que.pop();
    }
    return result;
}


/**
 * @brief 逆波兰表达式计算
 * @param [in] s 逆波兰表达式字符串
 * @return 计算结果
 */
int evaluatePostfix(const string& s)
{
    if (s.empty())
    {
        return 0;
    }

    stack<int> st;
    for (auto c : s)
    {
        // 操作数入栈
        if ('9' >= c && '0' <= c)
        {
            st.push(c - '0');
        }
        else
        {
            long long num1 = st.top();
            st.pop();
            long long num2 = st.top();
            st.pop();

            if (OPERATOR[ADD] == c)
            {
                st.push(num2 + num1);
            }
            else if (OPERATOR[SUB] == c)
            {
                st.push(num2 - num1);
            }
            else if (OPERATOR[MUL] == c)
            {
                st.push(num2 * num1);
            }
            else if (OPERATOR[DIV] == c)
            {
                st.push(num2 / num1);
            }
        }
    }
    return st.top();
}


/**
 * @brief 对于中缀表达式的特殊处理
 * @param [in] s 中缀表达式字符串
 * @param [in] begin 开始计算位置
 * @return 计算结果 {result, index}
 */
vector<int> calculateInfix(const string& s, size_t begin)
{
    stack<int> numStack;
    int num = 0;
    char sign = '+';
    size_t i = begin;
    for (i = begin; i < s.size(); ++i)
    {
        // 遍历字符，转换为数字
        if (isdigit(s[i]))
        {
            num = num*10 + (s[i] - '0');
            if (i != s.size() -1)
            {
                continue;
            }
        }
        // 若遇到括号
        else if ('(' == s[i])
        {
            vector<int> res = calculateInfix(s, i + 1); // 将括号中内容进行单独递归计算
            num = res[0];
            i = res[1];
            if (i != s.size() -1)
            {
                continue;
            }
        }
        // 对于符号，填充其对应内容
        switch(sign)
        {
            case '+':
                numStack.push(num);
                break;
            case '-':
                numStack.push(-num);
                break;
            case '*':
            {
                num = numStack.top() * num;
                numStack.pop();
                numStack.push(num);
                break;
            }
            case '/':
            {
                num = numStack.top() / num;
                numStack.pop();
                numStack.push(num);
                break;
            }
            default:
                break;
        }
        // 遇到右括号，结束递归
        if (')' == s[i])
        {
            break;
        }
        sign = s[i];
        num = 0;
    }

    int sum = 0;
    while (!numStack.empty())
    {
        sum += numStack.top();
        numStack.pop();
    }
    return {sum, i};
}



/**
 * @brief 主函数
 * @return int
 */
int main()
{
    string infix = "3+(2*2)-1";
    vector<int> res = calculateInfix(infix, 0);
    cout << "Calculated Result: " << res[0] << endl;
    string postfix = infixToPostfix(infix);
    cout << "Infix: " << infix << endl;


    cout << "Infix: " << infix << endl;
    cout << "Postfix: " << postfix << endl;
    int result = evaluatePostfix(postfix);
    cout << "Result: " << result << endl;
    return 0;
}