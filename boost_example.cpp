#include <iostream>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/arith2.hpp>
#include <functional>
#include <vector>
using namespace std;
using namespace boost::numeric;
using namespace interval_lib;

//знак полинома с коэфами в векторе Р размера sz в точке х
int sign_polynomial(double x, const vector<double>& P, int sz) {
    
    typedef interval<double> I_aux;

    I_aux::traits_type::rounding rnd;
    typedef unprotect<I_aux>::type I;

    I y = P[sz - 1];
    for (int i = sz - 2; i >= 0; i--)
        y = y * x + P[i];

    using namespace compare::certain;
    if (y > 0.) return 1;
    if (y < 0.) return -1;
    return 0;
}

ostream& operator<<(ostream& out, const interval<double> x) {
    out << "( " << x.lower() << " , " << x.upper() << " )\n";
    return out;
}


enum class Operations {
    ADD,
    SUB,
    MUL,
    DIV,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    EQUAL,
    OPS_COUNT,
};

void try_suspicious(const interval<double>& x, const interval<double>& y, Operations operation) {
    switch (operation) {
    case Operations::ADD: {
        try {
            cout << (x + y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE ADD\n";
        }
        break;
    }
    case Operations::SUB: {
        try {
            cout << (x - y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE SUB\n";
        }
        break;
    }
    case Operations::MUL: {
        try {
            cout << (x * y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE MUL\n";
        }
        break;
    }
    case Operations::DIV: {
        try {
            cout << (x / y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE DIV\n";
        }
        break;
    }
    case Operations::LESS: {
        try {
            cout << (x < y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE LESS\n";
        }
        break;
    }
    case Operations::LESS_EQUAL: {
        try {
            cout << (x <= y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE LESS_EQUAL\n";
        }
        break;
    }
    case Operations::GREATER: {
        try {
            cout << (x > y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE GREATER\n";
        }
        break;
    }
    case Operations::GREATER_EQUAL: {
        try {
            cout << (x >= y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE GREATER_EQUAL\n";
        }
        break;
    }
    case Operations::EQUAL: {
        try {
            cout << (x == y);
        }
        catch (exception& ex) {
            cout << ex.what() << " WHILE EQUAL\n";
        }
        break;
    }
    default: cout << "wrong operation or somthing else\n";
    }
}

void tests_arithmetics() {
    interval<double> x{ 12.3, 45.6 }, y{ 78.9, 86.5 };
    cout << (x + y) << (x - y) << (x * y) << (x / y)<<endl;
    x = { -12.3, 45.6 }, y = { 78.9, 86.5 };
    cout << (x + y) << (x - y) << (x * y) << (x / y) << endl;
    x = {  -45.6,-12.3 }, y = { 78.9, 86.5 };
    cout << (x + y) << (x - y) << (x * y) << (x / y) << endl;
    x = { -45.6,-12.3 }, y = { 0,0 };
    cout << (x + y) << (x - y) << (x * y);
    try_suspicious(x, y, Operations::DIV);
}

void tests_comparisons() {
    //тут все отрабатывает норм, сравнение непересекающихся интервалов определено в библиотеке
    interval<double> x = { 12.3, 45.6 }, y = { 78.9, 86.5 };
    try_suspicious(x, y, Operations::LESS);
    try_suspicious(x, y, Operations::LESS_EQUAL);
    try_suspicious(x, y, Operations::GREATER);
    try_suspicious(x, y, Operations::GREATER_EQUAL);
    try_suspicious(x, y, Operations::EQUAL);
    cout << "------------------------\n";

    //uncertain comparison для всех случаев, не работает для пересекающихся интервалов
    x = { 12.3, 45.6 }, y = { 12.3, 45.6 };
    try_suspicious(x, y, Operations::LESS);
    try_suspicious(x, y, Operations::LESS_EQUAL);
    try_suspicious(x, y, Operations::GREATER);
    try_suspicious(x, y, Operations::GREATER_EQUAL);
    try_suspicious(x, y, Operations::EQUAL);
    cout << "------------------------\n";
    //равенство не работает, ибо определено неверно в библиотеке (правый конец сравнивается с левым и наоборот - см. определение ниже)

/*
template<class T, class Policies> inline
bool interval<T, Policies>::operator== (const interval_holder& r) const
{
  if (!checking::is_empty(low, up)) {
    if (up == r.low && low == r.up) return true;
    else if (up < r.low || low > r.up) return false;
  }
  throw interval_lib::comparison_error();
}
*/
}
/*

БИБЛИОТЕЧНОЕ ОПРЕДЕЛЕНИЕ ДЛЯ <=, ОПРЕДЕЛЕНО ДЛЯ НЕПЕРЕСЕКАЮЩИХСЯ (ЗА ИСКЛЮЧЕНИЕМ ОДНОГО КОНЦА, ИНТЕРВАЛОВ)

template<class T, class Policies> inline
bool interval<T, Policies>::operator<= (const interval_holder& r) const
{
  if (!checking::is_empty(low, up)) {
    if (up <= r.low) return true;
    else if (low > r.up) return false;
  }
  throw interval_lib::comparison_error();
}

*/

void tests_funcs() {
    interval <double> x{ 0,1 }, y{ 16,36 }, z{ -16, 0 }, tt{ -25,-4 }, t{ -100,100 }, zero(0,0), four(4,4);
    cout << "squares: \n";
    cout << sqrt(x);
    cout << sqrt(y);
    cout << sqrt(z);
    cout << sqrt(t);
    try {//дропнет ошибку пустого интервала
        cout << sqrt(tt);
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
    }
    try {
        cout << sqrt(four);
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
    }
    try {
        cout << sqrt(zero);
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
    }

}

interval <double> foo(const vector <interval <double> >& x) {
    interval <double> y{ 1,16 }, p;
    interval<double> sum = 0;
    for (const auto& i : x) sum += i;
    return sum;
}

interval <double> doo(const interval <double>& x) {
    return sqrt(x);
}

void prnt_2(function <interval <double>(vector < interval <double> >)> func, const vector<interval <double>> & values) {
    cout << func(values) << endl;
}
void prnt_1(function <interval <double>( interval <double>)> func, const interval <double>& values) {
    cout << func(values) << endl;
}



int main()
{
    tests_arithmetics();
    tests_comparisons();
    prnt_2(foo, { {4,16}, {1,1}, {100,100} });

    interval <double> y{ 1,16 }, p = sqrt(y);
    cout << p;
    prnt_1(doo, y );
    tests_funcs();
    return 0;
}

