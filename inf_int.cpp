#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"

/*

*/

inf_int::inf_int()
{
    this->digits = new char[2];   // 동적할당

    this->digits[0] = '0';      // default 값 0 설정
    this->digits[1] = '\0';
    this->length = 1;
    this->thesign = true;
}

inf_int::inf_int(int n) {
    char buf[100];

    if (n < 0) {      // 음수 처리
        this->thesign = false;
        n = -n;
    }
    else {
        this->thesign = true;
    }

    int i = 0;
    while (n > 0) {         // 숫자를 문자열로 변환하는 과정
        buf[i] = n % 10 + '0';

        n /= 10;
        i++;
    }

    if (i == 0) {   // 숫자의 절댓값이 0일 경우
        new (this) inf_int();   // 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨.
    }
    else {
        buf[i] = '\0';

        this->digits = new char[i + 1];
        this->length = i;
        strcpy(this->digits, buf);
    }
}

inf_int::inf_int(const char* str)
{
    //부호처리
    char* t = new char[strlen(str)+1];
    strcpy(t, str);
    if (t[0] == '-')
    {
        this->thesign = false;

        for (int i = 0; i < strlen(t); i++) {
            t[i] = t[i + 1];
        }
    }
    else
    {
        this->thesign = true;
    }
    // "100"이 들어왔다면 내부 표현에 맞게 "001"로 변환
    // ex) "-1053" -> thesign=false, digits="3501", len=4
    if (t[0] == '0') {

        new (this) inf_int();
    }
    else {

        char* rev = new char[strlen(t)+1];

        int b = 0;
        for (int i = strlen(t) - 1; i >= 0; i--) {

            rev[b] = t[i];
            b++;
        }
        rev[b] = '\0';
        this->digits = rev; //역순한 값
        this->length = strlen(rev);//길이
    }
    delete[] t;
}

inf_int::inf_int(const inf_int& a) {
    this->digits = new char[a.length + 1];

    strcpy(this->digits, a.digits);
    this->length = a.length;
    this->thesign = a.thesign;
}

inf_int::~inf_int() {
    delete digits;      // 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
    if (this->digits) {
        delete this->digits;      // 이미 문자열이 있을 경우 제거.
    }
    this->digits = new char[a.length + 1];

    strcpy(this->digits, a.digits);
    this->length = a.length;
    this->thesign = a.thesign;

    return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
    // we assume 0 is always positive.
    if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)   // 부호가 같고, 절댓값이 일치해야함.
        return true;
    return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
    return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
    // 절대값 비교
    bool t;
    inf_int c;
    c = a;
    t = c.compareabsolute(b);

    // 둘 다 양수일 경우 절댓값 비교한 것을 그대로 return
    if (a.thesign == true && b.thesign == true) return t;

    // 둘 다 음수일 경우 절댓값 비교의 것을 반전하여 return
    else if (a.thesign == false && b.thesign == false) return !t;

    // 부호가 다를 경우, a가 양수일 경우 b는 음수, a가 음수일 경우 b는 양수이기에 a의 부호진리값을 반환하면 됨
    else return a.thesign;
}

bool operator<(const inf_int& a, const inf_int& b)
{
    if (operator>(a, b) || operator==(a, b)) {
        return false;
    }
    else {
        return true;
    }
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
    inf_int c;
    unsigned int i;

    if (a.thesign == b.thesign) {   // 이항의 부호가 같을 경우 + 연산자로 연산
        for (i = 0; i < a.length; i++) {
            c.Add(a.digits[i], i + 1);
        }
        for (i = 0; i < b.length; i++) {
            c.Add(b.digits[i], i + 1);
        }

        c.thesign = a.thesign;

        return c;
    }
    else {   // 이항의 부호가 다를 경우 - 연산자로 연산
        c = b;
        c.thesign = a.thesign;

        return a - c;
    }
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
    inf_int c;
    unsigned int i;

    //부호가 같은 경우 ( 절대값을 뺀다. a-b에서 a가 절대값이 크냐 b가 크냐에 따라 부호 결정 )
    if (a.thesign == b.thesign)
    {
        c = a;
        if (c.compareabsolute(b) == true || operator==(c, b))
        {
            for (i = 0; i < b.length; i++)
            {
                c.Sub(b.digits[i], i + 1);
            }
            if (operator == (a,b))
            {
                c.thesign = true;
            }
        }
        else
        {
            c = b;
            for (i = 0; i < a.length; i++)
            {
                c.Sub(a.digits[i], i + 1);
            }
            c.thesign = !(c.thesign);
        }

        // 빼서 앞 자리가 0이 있는 경우 0삭제 ex) 200 - 199 = 1 경우, 배열에는 1 0 0 저장, 0 2개를 없애야함.
        for (i = c.length - 1; i >= 1; i--)
        {
            if (c.digits[i] != '0') break;
        }

        char* t = new char[strlen(c.digits)+1];
        strcpy(t, c.digits);

        c.digits = new char[i+2];
        for (int j = 0; j < i + 1; j++)
        {
            c.digits[j] = t[j];
        }
        c.digits[i + 1] = '\0';
        c.length = strlen(c.digits);
        delete[] t;

        return c;
    }
    //부호가 다른 경우 ( 절대값을 더한다. 부호는 앞 숫자의 부호 (a-b)경우 a의 부호 )
    else
    {
        for (i = 0; i < a.length; i++)
        {
            c.Add(a.digits[i], i + 1);
        }
        for (i = 0; i < b.length; i++)
        {
            c.Add(b.digits[i], i + 1);
        }
        c.thesign = a.thesign;

        return c;
    }
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
    inf_int c;
    int mul;
    int carry;
    int i, j;

    for ( i = 0; i < b.length; i++)
    {
        inf_int t;
        delete t.digits;
        t.digits = new char[i + a.length + 2];
        carry = 0;

        //0 추가하기
        for (j = 0; j < i; j++)
        {
            t.digits[j] = '0';
        }
      //
        for (j = 0; j < a.length; j++)
        {
            mul = carry + (a.digits[j] - '0') * (b.digits[i] - '0');
            t.digits[i + j] = mul % 10 + '0';
            carry = mul / 10;
        }
        if (carry > 0)
        {
            t.digits[i + j] = carry + '0';
            t.digits[i + j + 1] = '\0';
        }
        else
        {
            t.digits[i + j] = '\0';
        }

        t.length = strlen(t.digits);
        c = operator+(c, t);
    }

    // 곱한 결과 앞 자리가 0이 있는 경우 0삭제(0000으로 결과값이 나오면 0으로 바꿈)
    for (i = c.length - 1; i >= 1; i--)
    {
        if (c.digits[i] != '0') break;
    }

    char* t = new char[strlen(c.digits)+1];
    strcpy(t, c.digits);

    c.digits = new char[i+2];
    for (int j = 0; j < i + 1; j++)
    {
        c.digits[j] = t[j];
    }
    c.digits[i + 1] = '\0';
    c.length = strlen(c.digits);
    delete[] t;

    //a, b의 부호가 같거나 0이면 양수, 아니면 음수
    if (a.thesign == b.thesign || c == 0)
    {
        c.thesign = true;
    }
    else
    {
        c.thesign = false;
    }

    return c;
}

inf_int operator/(const inf_int& a, const inf_int& b)
{
    inf_int c;

    //절대값이 작은 수 / 절대값이 큰 수
    if (((inf_int)b).compareabsolute(a))
    {
        //a가 양수 (몫 : 0)
        if (a.thesign == true) return 0;
        //a가 음수이고 b가 양수 (몫 : -1)
        else if (a.thesign == false && b.thesign == true) return -1;
        //a가 음수이고 b가 음수 (몫 : 1)
        else return 1;
    }

    //절대값이 큰 수 / 절대값이 작은 수
    else
    {
        int i;
        inf_int at = a;
        inf_int bt;

        at.thesign = true;

        delete bt.digits;
        bt.digits = new char[at.length + 1];

        for (int j = 0; j < at.length - b.length; j++)
        {
            bt.digits[j] = '0';
        }
        for (int j = at.length - b.length; j <= at.length; j++)
        {
            bt.digits[j] = b.digits[j + b.length - at.length];
        }

        bt.length = strlen(bt.digits);

        delete c.digits;
        c.digits = new char[1];
        c.digits[0] = '\0';
        c.length = 0;
        int flag = 1;

        while (bt.length >= b.length)
        {
            if (at.compareabsolute(bt) == true || operator==(at, bt) == true)
            {
                for (i = 1; ; i++)
                {
                    at = operator-(at, bt);
                    if (at.thesign == false)
                    {
                        break;
                    }
                }
                at = operator+(at, bt);

                c.digits = (char*)realloc(c.digits, c.length + 2);
                for (int j = c.length; j >= 0; j--)
                {
                    c.digits[j + 1] = c.digits[j];
                }
                c.digits[0] = (i-1) + '0';

                flag = 0;
            }
            else
            {
                if (flag == 1)
                {
                    flag = 0;
                }
                else
                {
                    c.digits = (char*)realloc(c.digits, c.length + 2);
                    for (int j = c.length; j >= 0; j--)
                    {
                        c.digits[j + 1] = c.digits[j];
                    }
                    c.digits[0] = '0';
                }
            }

            c.length = strlen(c.digits);

            for (int j = 0; j < bt.length; j++)
            {
                bt.digits[j] = bt.digits[j + 1];
            }
            bt.length = strlen(bt.digits);
        }
        //양수 / 양수(몫 : +c)
        if (a.thesign == true && b.thesign == true) c.thesign = true;
        //양수 / 음수(몫 : -c)
        else if (a.thesign == true && b.thesign == false) c.thesign = false;
        //음수 / 양수(몫 : -c-1))
        else if (a.thesign == false && b.thesign == true)
        {
            c.thesign = false;
            if(at != 0)
            {
                c = c-1;
            }
        }
        //음수 / 음수(몫 : c+1)
        else
        {
            c.thesign = true;
            if(at != 0)
            {
                c = c + 1;
            }
        }
        return c;
    }
}

inf_int operator%(const inf_int& a, const inf_int& b)
{
    int i;
    inf_int at = a;
    inf_int bt;

    //a가 0일때
    if (a == 0) return 0;

    //절대값이 작은 수 / 절대값이 큰 수
    if (((inf_int)b).compareabsolute(a))
    {
        //a가 양수 (나머지 : a, at)
        if(a.thesign == true) return at;
        //a가 음수 (나머지 : |b|+a, |b|+at)
        else
        {
            bt = b;
            bt.thesign = true;
            return bt + at;
        }
    }

    //절대값이 큰 수 / 절대값이 작은 수
    else
    {
        at.thesign = true;

        delete bt.digits;
        bt.digits = new char[at.length + 1];

        for (int j = 0; j < at.length - b.length; j++)
        {
            bt.digits[j] = '0';
        }
        for (int j = at.length - b.length; j <= at.length; j++)
        {
            bt.digits[j] = b.digits[j + b.length - at.length];
        }

        bt.length = strlen(bt.digits);


        while (bt.length >= b.length)
        {
            if (at.compareabsolute(bt) == true || operator==(at, bt) == true)
            {
                for (i = 1; ; i++)
                {
                    at = operator-(at, bt);
                    if (at.thesign == false)
                    {
                        break;
                    }
                }
                at = operator+(at, bt);
            }

            for (int j = 0; j < bt.length; j++)
            {
                bt.digits[j] = bt.digits[j + 1];
            }
            bt.length = strlen(bt.digits);
        }
        //나머지가 0일경우
        if(at == 0) return 0;
        //양수 % 양수 (나머지 : at, a)
        else if (a.thesign == true && b.thesign == true) return at;
        //양수 % 음수 (나머지 : at, a)
        else if (a.thesign == true && b.thesign == false) return at;
        //음수 % 양수 (나머지 : b-at, b-a)
        else if (a.thesign == false && b.thesign == true)return b - at;
        //음수 % 음수 (나머지 : -(b+at), -(b+a))
        else return 0-(b + at);
    }
}

inf_int operator^(const inf_int& a, const inf_int& b)
{
    inf_int result = 1;
    inf_int t = b;

    while (t > 0)
    {
        result = result * a;
        t = t - 1;
    }
    return result;
}

ostream& operator<<(ostream& out, const inf_int& a)
{
    int i;

    if (a.thesign == false) {
        out << '-';
    }
    for (i = a.length - 1; i >= 0; i--) {
        out << a.digits[i];
    }
    return out;
}
//절대값 비교 함수
bool inf_int::compareabsolute(const inf_int& b)
{
    bool t;

    if (this->length > b.length)
    {
        t = true;
    }
    else if (this->length < b.length)
    {
        t = false;
    }
    else
    {
        t = false;
        for (int i = this->length; i >= 0; i--)
        {
            if (this->digits[i] > b.digits[i])
            {
                t = true;
                break;
            }
            else if (this->digits[i] < b.digits[i])
            {
                t = false;
                break;
            }
        }
    }
    return t;
}
void inf_int::Add(const char num, const unsigned int index)   // a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, Add(a, 2, 2)의 결과는 411
{
    if (this->length < index) {
        this->digits = (char*)realloc(this->digits, index + 1);

        if (this->digits == NULL) {      // 할당 실패 예외처리
            cout << "Memory reallocation failed, the program will terminate." << endl;

            exit(0);
        }

        this->length = index;               // 길이 지정
        this->digits[this->length] = '\0';   // 널문자 삽입
    }

    if (this->digits[index - 1] < '0') {   // 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
        this->digits[index - 1] = '0';
    }

    this->digits[index - 1] += num - '0';   // 값 연산


    if (this->digits[index - 1] > '9') {   // 자리올림이 발생할 경우
        this->digits[index - 1] -= 10;   // 현재 자릿수에서 (아스키값) 10을 빼고
        Add('1', index + 1);         // 윗자리에 1을 더한다
    }
}
void inf_int::Sub(const char num, const unsigned int index)
{
    if (this->length < index) {
        this->digits = (char*)realloc(this->digits, index + 1);

        if (this->digits == NULL) {      // 할당 실패 예외처리
            cout << "Memory reallocation failed, the program will terminate." << endl;

            exit(0);
        }

        this->length = index;               // 길이 지정
        this->digits[this->length] = '\0';   // 널문자 삽입
    }

    if (this->digits[index - 1] < '0') {   // 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
        this->digits[index - 1] = '0';
    }

    this->digits[index - 1] -= num - '0';   // 값 연산


    if (this->digits[index - 1] < '0') {   // 자리내림이 발생할 경우
        this->digits[index - 1] += 10;   // 현재 자릿수에서 (아스키값) 10을 더하고
        Sub('1', index+1);         // 윗자리에 1을 뺀다
    }
}

bool inf_int::get_thesign(){
    return this->thesign;
}
char* inf_int::get_digits(){
    return this->digits;
}
unsigned int inf_int:: get_length(){
    return this->length;
}
