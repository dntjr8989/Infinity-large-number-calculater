#define _CRT_SECURE_NO_WARNINGS
#include "postfix.h"
#include <map>

postfix::postfix(string input)
{
    int cnt;
    int i;

    map<char, int> ma;
    //기호별 우선순위
    ma['+'] = 0;
    ma['-'] = 0;
    ma['*'] = 1;
    ma['/'] = 1;
    ma['%'] = 1;
    ma['^'] = 2;
    ma['('] = -1;

    char stack[100];
    int top = -1;

    this->postfixsize = 0;
    for ( i = 0; i < 100; i++) this->savepostfix[i] = NULL;

    cnt = 0;
    for (i = 0; i < input.length(); i++)
    {
        if (input.at(i) == '+' || input.at(i) == '-' || input.at(i) == '*' || input.at(i) == '/' || input.at(i) == '%' || input.at(i) == '^' || input.at(i) == '(' || input.at(i) == ')' )
        {
            if (i != 0)
            {
                if (input.at(i - 1) >= 48 && input.at(i - 1) <= 57)
                {
                    this->savepostfix[this->postfixsize] = (char*)realloc(this->savepostfix[this->postfixsize], sizeof(char) * (cnt + 1));
                    this->savepostfix[this->postfixsize][cnt] = '\0';
                    this->postfixsize++;
                }
                //음수인 경우 체크
                if (input.at(i - 1) == '(' && input.at(i) == '-')
                {
                    this->savepostfix[this->postfixsize] = (char*)realloc(this->savepostfix[this->postfixsize], sizeof(char) * (cnt + 1));
                    this->savepostfix[this->postfixsize][cnt] = input.at(i); //'-'
                    cnt++;
                    continue;
                }
            }
            //'('
            if (input.at(i) == '(')
            {
                stack[top + 1] = '(';
                top++;
            }
            else if (input.at(i) == ')')
            {
                while (stack[top] != '(')
                {
                    this->savepostfix[this->postfixsize] = (char*)realloc(this->savepostfix[this->postfixsize], sizeof(char)*2);
                    this->savepostfix[this->postfixsize][0] = stack[top];
                    this->savepostfix[this->postfixsize][1] = '\0';
                    this->postfixsize++;
                    top--;
                }
                top--;
            }
            else
            {
                while (top >= 0 && ma[stack[top]] >= ma[input.at(i)])
                {
                    this->savepostfix[this->postfixsize] = (char*)realloc(this->savepostfix[this->postfixsize], sizeof(char) * 2);
                    this->savepostfix[this->postfixsize][0] = stack[top];
                    this->savepostfix[this->postfixsize][1] = '\0';
                    this->postfixsize++;
                    top--;
                }
                stack[top + 1] = input.at(i);
                top++;
            }
            cnt = 0;
        }
        else
        {
            this->savepostfix[this->postfixsize] = (char*)realloc(this->savepostfix[this->postfixsize], sizeof(char) * (cnt + 1));
            this->savepostfix[this->postfixsize][cnt] = input.at(i);
            cnt++;
        }
    }


    if (input.at(i - 1) >= 48 && input.at(i - 1) <= 57)
    {
        this->savepostfix[this->postfixsize] = (char*)realloc(this->savepostfix[this->postfixsize], sizeof(char)*(cnt+1));
        this->savepostfix[this->postfixsize][cnt] = '\0';
        this->postfixsize++;
    }

    while (top >= 0)
    {
        this->savepostfix[this->postfixsize] = (char*)realloc(this->savepostfix[this->postfixsize], sizeof(char) * 2);
        this->savepostfix[this->postfixsize][0] = stack[top];
        this->savepostfix[this->postfixsize][1] = '\0';
        this->postfixsize++;
        this->savepostfix[this->postfixsize] = (char*)malloc(sizeof(char));
        top--;
    }
}
inf_int postfix::computepostfix()
{
    inf_int stack[100];
    int top = -1;

    for (int i = 0; i < this->postfixsize; i++)
    {
        //연산자인 경우
        if (this->savepostfix[i][0] == '+' ||  (this->savepostfix[i][0] == '-' && (this->savepostfix[i][1] == '\0')) || this->savepostfix[i][0] == '*' || this->savepostfix[i][0] == '/' || this->savepostfix[i][0] == '%' || this->savepostfix[i][0] == '^')
        {
            //숫자 2개가 없는 경우(error사항)
            if (top < 1)
            {
                inf_int t("error");
                return t;
            }
            //숫자 2개 pop하기
            inf_int op2 = stack[top];
            inf_int op1 = stack[top - 1];
            top = top - 2;

            //pop한 숫자 2개 계산하기
            inf_int op3;

            //+인 경우
            if (this->savepostfix[i][0] == '+')
            {
                op3 = op1 + op2;
            }
            //-인 경우
            else if (this->savepostfix[i][0] == '-')
            {
                op3 = op1 - op2;
            }
            //*인 경우
            else if (this->savepostfix[i][0] == '*')
            {
                op3 = op1 * op2;
            }
            // /(나누기 -> 몫)인 경우
            else if (this->savepostfix[i][0] == '/')
            {
                op3 = op1 / op2;
            }
            // %(나누기 -> 나머지)인 경우
            else if (this->savepostfix[i][0] == '%')
            {
                op3 = op1 % op2;
            }
            // ^(제곱)인 경우
            else if (this->savepostfix[i][0] == '^')
            {
                //op2가 음수일 경우 (error사항)
                if (op2 < 0)
                {
                    inf_int t("error");
                    return t;
                }
                op3 = op1 ^ op2;
            }
            stack[top + 1] = op3;
            top++;
        }
        //연산자가 아닌 경우
        else
        {
            //괄호가 후위표기식에 있는 경우(error사항)
            if (this->savepostfix[i][0] == '(')
            {
                inf_int t("error");
                return t;
            }
            inf_int t(this->savepostfix[i]);
            stack[top + 1] = t;
            top++;
        }
    }
    return stack[top];
}
