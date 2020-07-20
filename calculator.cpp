#include "calculator.h"
#include "ui_calculator.h"
#include <qstring.h>

int calcVal = 0;     //첫 화면 숫자 0으로 고정
int num = 0;        //괄호 갯수 판단
QString resultVal;  //최종 결과 값
QString next_result; //=누른 다음 바로 연산자 눌렀을때 결과값 이어감

// 가장 마지막에 클릭된 버튼이 무엇인지 판단하기 위한 함수
bool divTrigger = false;
bool mulTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool modTrigger = false;
bool allTrigger = true; //연산자 연속 방지
bool zeroTrigger= true; //나누기 0, 및 숫자 000과 같은 문자열 방지
bool equalTrigger = false;


// 생성자
Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);

   // 디스플레이 화면에 0 넣기
    ui->Display->setText(QString::number(calcVal));

    //0~9까지의 버튼을 담기 위한 배열
    QPushButton *numButtons[10];

    //0~9까지의 숫자 버튼을 세팅
    for(int i = 0; i < 10 ;++i){
        QString butName = "Button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton*>(butName);
        connect(numButtons[i], SIGNAL(released()),this,
                SLOT(NumPressed()));
    }

    connect(ui->Add, SIGNAL(released()),this,
            SLOT(MathButtonPressed()));

    connect(ui->Subtract, SIGNAL(released()),this,
            SLOT(MathButtonPressed()));

    connect(ui->Multiply, SIGNAL(released()),this,
            SLOT(MathButtonPressed()));

    connect(ui->Divide, SIGNAL(released()),this,
            SLOT(MathButtonPressed()));

    connect(ui->Mod, SIGNAL(released()),this,
            SLOT(MathButtonPressed()));

    connect(ui->Equals, SIGNAL(released()),this,
            SLOT(EqualButton()));

    connect(ui->ChangeSign, SIGNAL(released()),this,
            SLOT(ChangeNumberSign()));

    connect(ui->Clear, SIGNAL(released()),this,
            SLOT(ClearButton()));

    connect(ui->Delete, SIGNAL(released()),this,
            SLOT(DeleteButton()));

    connect(ui->Parentheses, SIGNAL(released()),this,
            SLOT(ParenthesesButton()));

    connect(ui->Square, SIGNAL(released()),this,
            SLOT(SquareButton()));

    connect(ui->Nsquare, SIGNAL(released()),this,
            SLOT(NSquareButton()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){
    // sender는 눌린 버튼의 포인터 값을 반환한다.
    QPushButton *button = (QPushButton*)sender();
    allTrigger = false;

    // 버튼의 적혀있는 숫자를 가져온다
    QString butVal = button->text();

    // 디스플레이 화면의 숫자를 가져온다
    QString displayVal = ui->Display->toPlainText();
    QString temp = ui->Display->toPlainText();
    temp.chop(1);


    if(equalTrigger){
        if(displayVal.back()=='('){
            ui->Display->setText(displayVal+butVal);
        }else
        {
        ui->Display->setText(butVal);
        equalTrigger = false;
        }
    }

    // 첫 시작시 0을 입력하면 안됨
    else if(zeroTrigger){
        if(!QString::compare(displayVal, "0", Qt::CaseInsensitive))
        {
            ui->Display->setText(butVal);
        }
        else
        {
            // 나누기 0 은 안됨
            if((displayVal.back() == '%'|| displayVal.back()=='/')&& butVal =='0'){
                ui->Display->setText(displayVal);
            }
            // +,-,* 0은 가능 But 연산자 뒤 첫 숫자가 0이면 0으로 끝나야함
            else if(displayVal.back() =='0'&& (temp.back() == '+' || temp.back() == '-' || temp.back() == '*'||temp.back()=='^')){
                ui->Display->setText(displayVal);
                zeroTrigger = false;
            }
            else{
                QString newVal = displayVal + butVal;
                ui->Display->setText(displayVal+butVal);
            }

            // ) 다음에 숫자 바로 올때 예외처리
            if(displayVal.back()==')'){
                ui->Display->setText(displayVal+'*'+butVal);
            }
        }
    }

}

void Calculator::MathButtonPressed(){
    // 이전 클릭 된 버튼 값 전부 초기화
    divTrigger = false;
    mulTrigger = false;
    addTrigger = false;
    subTrigger = false;
    modTrigger = false;

    // 현재값 저장
    QString displayVal = ui->Display->toPlainText();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    if(equalTrigger){
        ui->Display->setText(next_result+button->text());
        equalTrigger = false;
        allTrigger = true;
        zeroTrigger = true;
    }else if(!allTrigger){

    if(QString::compare(butVal,"/",Qt::CaseInsensitive)==0){
        divTrigger = true;
    }else if(QString::compare(butVal,"*",Qt::CaseInsensitive)==0){
        mulTrigger = true;
    }else if(QString::compare(butVal,"+",Qt::CaseInsensitive)==0){
        addTrigger = true;
    }else if(QString::compare(butVal,"%",Qt::CaseSensitive)==0){
        modTrigger = true;
    }
    else{
        subTrigger = true;
    }

    ui->Display->setText(displayVal+button->text());
    allTrigger = true;
    zeroTrigger = true;
    }
}

void Calculator::EqualButton(){
    QString displayVal = ui->Display->toPlainText();    //화면 QString 읽어오기
    QString resultVal;
    resultVal.clear();
    QString rev;
    std::string equation = "0";                         //QString string으로 변환
    equation = displayVal.toUtf8().constData();
    equalTrigger = true;

    postfix result(equation);                               //해당 식 postfix 객체 생성 및 후위 연산자 재배열
    inf_int solution(result.computepostfix());              //정답 객체 생성
    if(solution.get_thesign()){
        rev = QString::fromStdString(solution.get_digits());

        for(int i = solution.get_length(); i > 0;i--){
            resultVal = resultVal + rev.right(1);
            rev.chop(1);
        }
        ui->Display->setText(displayVal+"\n\n\n="+resultVal);
    }else{
        resultVal = "(-";
        rev = QString::fromStdString(solution.get_digits());
        for(int i=solution.get_length(); i>0;i--){
            resultVal = resultVal + rev.right(1);
            rev.chop(1);
        }
        resultVal.append(")");
        ui->Display->setText(displayVal+"\n\n\n="+resultVal);

    }

    next_result = resultVal;
}

void Calculator::ChangeNumberSign(){
    QString displayVal = ui->Display->toPlainText();
    QString temp = displayVal;

    //첫 화면에서 괄호 입력할때
    if(!QString::compare(displayVal, "0", Qt::CaseInsensitive))
    {
        ui->Display->setText("(-");
        displayVal = ui->Display->toPlainText();
        num++;
    }
    // 연산자 다음으로 괄호 입력할때
    else if((temp.back() == '+' ||temp.back() == '-'||temp.back() == '*'
               ||temp.back() == '/'||temp.back() == '%'||temp.back() == '^'))
    {
        ui->Display->setText(displayVal+"(-");
        displayVal = ui->Display->toPlainText();
        num++;
    }
    else if(temp.back()=='(')
    {
        ui->Display->setText(displayVal+"-");
        displayVal = ui->Display->toPlainText();
    }
    else if(temp.back()==')')
    {
        ui->Display->setText(displayVal+"*(-");
        displayVal = ui->Display->toPlainText();
        num++;
    }
 }


void Calculator::ClearButton()
{
     ui->Display->setText("0");
     num = 0;
    next_result = '0';
     bool divTrigger = false;
     bool mulTrigger = false;
     bool addTrigger = false;
     bool subTrigger = false;
     bool modTrigger = false;
     bool allTrigger = true; //연산자 연속 방지
     bool zeroTrigger= true;
}

void Calculator::DeleteButton()
{
    QString delVal = ui->Display->toPlainText();

    if(delVal.length() == 1)
    {
        ui->Display->setText("0");
        num = 0;
        return;
    }

    /*if(resultVal.length()<=1 && equalTrigger)
    {
        ui->Display->setText("0");
        num = 0;
        return ;
    }else if(resultVal.length()>1 && equalTrigger)
    {
        resultVal.chop(1);
        ui->Display->setText(resultVal);

    }*/

    if(delVal.length() > 1){
        if(delVal.back()=='('){
            delVal.chop(1);
            num--;
        }else if(delVal.back( )==')'){
            delVal.chop(1);
            num++;
        }else{
            delVal.chop(1);
            }
    }
    ui->Display->setText(delVal);
    if(equalTrigger){
        next_result.chop(1);
        ui->Display->setText(next_result);
        equalTrigger = false;
    }
}

void Calculator:: ParenthesesButton()
{
    QString displayVal = ui->Display->toPlainText();

    if(equalTrigger){
        if(next_result.back()=='0'){
            ui->Display->setText("(");
            num++;
        }else
        {
        ui->Display->setText(next_result+"*(");
        num++;
        }
        equalTrigger = false;
        allTrigger = true;
        zeroTrigger = true;
    }
    else if(!QString::compare(displayVal, "0", Qt::CaseInsensitive))
    {
        ui->Display->setText("(");
        displayVal = ui->Display->toPlainText();
        num++;
    }else if((displayVal.back() == '+' ||displayVal.back() == '-'||displayVal.back() == '*'||displayVal.back() == '/'
              ||displayVal.back() == '%'||displayVal.back() =='('||displayVal.back()=='^')){
        ui->Display->setText(displayVal+'(');
        num++;
    }else if(num < 1){
        ui->Display->setText(displayVal+'*'+'(');
        num++;
    }else{
        ui->Display->setText(displayVal+')');
        num --;
    }
}

void Calculator::SquareButton()
{
    QString displayVal = ui->Display->toPlainText();

    if(equalTrigger)
    {
        ui->Display->setText(next_result+"^(2)");
        equalTrigger = false;
    }else if(!allTrigger)
    {
        ui->Display->setText(displayVal+"^(2)");

    }
}

void Calculator::NSquareButton()
{
    QString displayVal = ui->Display->toPlainText();

    if(equalTrigger)
    {
        ui->Display->setText(next_result+"^");
        equalTrigger = false;
    }else if(!allTrigger)
    {
        ui->Display->setText(displayVal+"^");
        allTrigger = true;
        zeroTrigger = true;
    }


}








