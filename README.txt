how to compile and execute the source cord

Qt 프로그램을 사용하여 project를 진행하였다.
이 소스 코드는 Calculator, postfix, inf_int 총 3개의 클래스로 이루어져 있습니다
첫번째 클래스는 GUI 로 인터페이스를 위해 만든 클래스입니다. 인터페이스 구성과 예외 입력 처리를 구현하였습니다.

두번째 클래스 postfix는 GUI를 통해 입력받은 infix 형태의 식을 postfix 형태의 식으로 변환합니다. 
그리고 이 postfix 식의 문자로 inf_int 객체를 생성하였습니다.

세번째 클래스는 inf_int , int와 double 과 같은 변수형은 입력 범위의 한계가 있기 때문에 입력범위를 벗어나는 큰 수를 계산할 수 없습니다. 
char 배열을 사용해서 문자 형식으로 다른 자료형의 범위를 넘게 숫자를 받을 수 있게 하였고, 연산자 오버로딩을 이용하여 문자의 사칙연산을 계산하는 함수를 구현했습니다. 

이렇게 GUI -> postfix -> inf_int를 거쳐 도출된 결과값을 다시 GUI 에 출력하도록 설계했습니다.

