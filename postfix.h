#ifndef POSTFIX_H
#define POSTFIX_H

#include "inf_int.h"

class postfix
{
private:
    char* savepostfix[100];
    int postfixsize;
public:
    postfix(string input);
    inf_int computepostfix();
};

#endif POSTFIX_H
