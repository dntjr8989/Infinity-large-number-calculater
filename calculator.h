#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <string>
#include "postfix.h"

namespace Ui {
class Calculator;
}

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    std::string Q_to_String(QString result);
    ~Calculator();

private:
    Ui::Calculator *ui;


private slots:
    void NumPressed();
    void MathButtonPressed();
    void EqualButton();
    void ChangeNumberSign();
    void ClearButton();
    void DeleteButton();
    void SquareButton();
    void NSquareButton();
    void ParenthesesButton();
};

#endif // CALCULATOR_H
