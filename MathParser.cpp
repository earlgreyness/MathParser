#include "MathParser.h"
#include <cmath>
#include <cctype>  // isidgit
#include <cstdlib> // atof
#include <stack>
//#include <iostream>

//Static members===============================================================================
std::map<std::string, MathParser::Type> MathParser::st_functions;
const std::string MathParser::st_symbols = " 0123456789().+-/*^abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//Static methods===============================================================================
void MathParser::staticInit()
{
    if (st_functions.empty()) {
        st_functions["sqrt"]   = Sqrt;
        st_functions["sin"]    = Sin;
        st_functions["cos"]    = Cos;
        st_functions["tan"]    = Tan;
        st_functions["arcsin"] = ArcSin;
        st_functions["arccos"] = ArcCos;
        st_functions["arctan"] = ArcTan;
        st_functions["sinh"]   = Sinh;
        st_functions["cosh"]   = Cosh;
        st_functions["tanh"]   = Tanh;
        st_functions["exp"]    = Exp;
        st_functions["log"]    = Log;
        st_functions["ln"]     = Ln;
        st_functions["abs"]    = Abs;
        st_functions["sign"]   = Sign;
    }
}
bool MathParser::isValidBrackets(const std::string& expr)
{
    // (()(((())))()(()()())()(())) - верно
    int numLBr = 0;
    for (size_t i = 0, n = expr.size(); i < n; ++i) {
        if (expr[i] == '(') {
            ++numLBr;
        } else if (expr[i] == ')') {
            if (numLBr > 0) {
                --numLBr;
            } else {
                return false;
            }
        }
    }
    if (numLBr != 0) {
        return false;
    } else {
        return true;
    }
}
bool MathParser::isCorrect(const std::string& expr0)
{
    if (expr0.empty()) {
        return false;
    }
    // заменить pi и e на числа
    staticInit();
    std::string::size_type it = expr0.find_first_not_of(st_symbols);
    if (it != std::string::npos) {
        return false;
    }
    std::string expr = "@" + expr0 + "@"; // '@' специальный символ начала и конца строки
    deleteSpaces(expr);
    toLower(expr);
    if (!isValidBrackets(expr)) {
        return false;
    }
    addMultToCorrectExpr(expr);
    std::string temp;
    int i = 1;
    int sz = expr.size();
    while (i < sz) {
        // std::cout << expr[i] << " "; //отладочная печать
        if (expr[i] == '@') {
            ++i;
        }
        else if (expr[i] == '(') {
            if (expr[i - 1] != '@' &&
                expr[i - 1] != '(' &&
                expr[i - 1] != '+' &&
                expr[i - 1] != '-' &&
                expr[i - 1] != '*' &&
                expr[i - 1] != '/' &&
                expr[i - 1] != '^' &&
                !std::isalpha(expr[i - 1])) {
                return false;
            }
            if (expr[i + 1] != '-'         &&
                !std::isalpha(expr[i + 1]) &&
                !std::isdigit(expr[i + 1]) &&
                expr[i + 1] != '(') {
                return false;
            } else {
                ++i;
            }
        }
        else if (expr[i] == ')') {
            if (!std::isdigit(expr[i - 1]) && expr[i - 1] != ')' && expr[i - 1] != 'x') {
                return false;
            }
            if (expr[i + 1] != '@' &&
                expr[i + 1] != ')' &&
                expr[i + 1] != '+' &&
                expr[i + 1] != '-' &&
                expr[i + 1] != '*' &&
                expr[i + 1] != '/' &&
                expr[i + 1] != '^') {
                return false;
            } else {
                ++i;
            }
        }
        else if (expr[i] == '+' || expr[i] == '*' || expr[i] == '/' || expr[i] == '^') {
            if (!std::isdigit(expr[i - 1]) && expr[i - 1] != ')' && expr[i - 1] != 'x') {
                return false;
            }
            if (!std::isdigit(expr[i + 1]) && !std::isalpha(expr[i + 1]) && expr[i + 1] != '(' && expr[i + 1] != 'x') {
                return false;
            } else {
                ++i;
            }
        }
        else if (expr[i] == '-') {
            if (expr[i - 1] != '@' &&
                expr[i - 1] != '(' &&
                expr[i - 1] != ')' &&
                !std::isdigit(expr[i - 1]) &&
                expr[i - 1] != 'x') {
                return false;
            }
            if (expr[i + 1] != '(' &&
                !std::isalpha(expr[i + 1]) &&
                !std::isdigit(expr[i + 1])) {
                return false;
            } else {
                ++i;
            }
        }
        else if (std::isalpha(expr[i])) {
            if (expr[i - 1] != '@' &&
                expr[i - 1] != '(' &&
                expr[i - 1] != '+' &&
                expr[i - 1] != '-' &&
                expr[i - 1] != '*' &&
                expr[i - 1] != '/' &&
                expr[i - 1] != '^') {
                return false;
            }
            temp.clear();
            temp.push_back(expr[i++]);
            while (std::isalpha(expr[i])) {
                temp.push_back(expr[i++]);
            }
            if (temp == "x") {
                if (expr[i] != '@' &&
                    expr[i] != '+' &&
                    expr[i] != '-' &&
                    expr[i] != '*' &&
                    expr[i] != '/' &&
                    expr[i] != '^' &&
                    expr[i] != ')') {
                    return false;
                }
            } else {
                std::map<std::string, Type>::iterator it = st_functions.find(temp);
                if (it != st_functions.end()) {
                    if (expr[i] != '(') {
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
        }
        else if (std::isdigit(expr[i])) {
            if (expr[i - 1] != '@' &&
                expr[i - 1] != '(' &&
                expr[i - 1] != '+' &&
                expr[i - 1] != '-' &&
                expr[i - 1] != '*' &&
                expr[i - 1] != '/' &&
                expr[i - 1] != '^') {
                return false;
            }
            ++i;
            while (std::isdigit(expr[i])) {
                ++i;
            }
            if (expr[i] == '.') {
                if (!std::isdigit(expr[i + 1])) {
                    return false;
                }
                ++i;
                while (std::isdigit(expr[i])) {
                    ++i;
                }
            }
            if (expr[i] != '@' &&
                expr[i] != ')' &&
                expr[i] != '+' &&
                expr[i] != '-' &&
                expr[i] != '*' &&
                expr[i] != '/' &&
                expr[i] != '^') {
                return false;
            }
        }
        else if (expr[i] == '.') {
            return false;
        }
    }

    return true;
}
void MathParser::toLower(std::string& expr)
{
    for (std::string::size_type i = 0, n = expr.size(); i < n; ++i) {
        int ch = static_cast<int>(expr[i]);
        if (std::isupper(ch)) {
            expr[i] = static_cast<std::string::value_type>(std::tolower(ch));
        }
    }
}
void MathParser::deleteSpaces(std::string& expr)
{
    std::string::size_type it = expr.find(" ");
    while (it != std::string::npos) {
        expr.erase(it, 1);
        it = expr.find(" ");
    }
}
void MathParser::addMultToCorrectExpr(std::string& expr)
{
    /*"5sin(3x)"*/
    /*"7"*/
    std::string::size_type i = 0;
    while (i < expr.size() - 1) {
        if ( std::isdigit(static_cast<int>(expr[i])) ) {
            if ( std::isalpha(static_cast<int>(expr[i + 1])) || expr[i + 1] == '(' ) {
                expr.insert(i + 1, "*");
            }
        }
        ++i;
    }
}
//=============================================================================================
MathParser::MathParser()
{
    init("0");
}
MathParser::MathParser(const MathParser& parser)
{
    this->m_sizePostfix = parser.m_sizePostfix;
    this->m_maxSizePostfix = parser.m_maxSizePostfix;
    this->m_maxSizeOperand = parser.m_maxSizeOperand;
    this->m_postfix = new Lexeme[this->m_maxSizePostfix];
    for (size_t i = 0; i < this->m_sizePostfix; ++i) {
        this->m_postfix[i] = parser.m_postfix[i];
    }
    this->m_operand = new double[this->m_maxSizeOperand];
    this->m_success = parser.m_success;
}
MathParser::MathParser(const std::string& expr)
{
    init(expr);
}
MathParser::~MathParser()
{
    delete[] m_postfix;
    delete[] m_operand;
}
MathParser& MathParser::operator=(const MathParser& rhs)
{
    if (&rhs != this) {
        delete[] this->m_postfix;
        delete[] this->m_operand;
        this->m_sizePostfix = rhs.m_sizePostfix;
        this->m_maxSizePostfix = rhs.m_maxSizePostfix;
        this->m_maxSizeOperand = rhs.m_maxSizeOperand;
        this->m_postfix = new Lexeme[this->m_maxSizePostfix];
        for (size_t i = 0; i < this->m_sizePostfix; ++i) {
            this->m_postfix[i] = rhs.m_postfix[i];
        }
        this->m_operand = new double[this->m_maxSizeOperand];
        this->m_success = rhs.m_success;
    }
    return *this;
}
//=============================================================================================
bool MathParser::compile(const std::string& expr)
{
    if (!isCorrect(expr)) {
        compile("0"); // рекурсия!
        m_success = false;
    } else {
        std::vector<Lexeme> infix = stringToInfix(expr);
        infixToPostfix(infix);
        m_success = true;
    }
    return isSuccess();
}
double MathParser::evaluate(double variable) const
{
    // Ни в коем случае не делать их static!!! Они будут статическими членами всего класса, а не данного объекта!!!
    Lexeme* post = m_postfix; // так работает быстрее (почему-то...)
    double* oper = m_operand; // так работает быстрее (почему-то...)
    int j = 0; // бегунок стека операндов m_operand (oper)

    for (size_t i = 0; i < m_sizePostfix; ++i) {
        switch (post[i].type) {
        case Number:
            oper[j] = post[i].number;
            ++j;
            break;
        case Variable:
            oper[j] = variable;
            ++j;
            break;
        case Plus:
            oper[j - 2] = oper[j - 2] + oper[j - 1];
            --j;
            break;
        case Minus:
            oper[j - 2] = oper[j - 2] - oper[j - 1];
            --j;
            break;
        case Mult:
            oper[j - 2] = oper[j - 2] * oper[j - 1];
            --j;
            break;
        case Div:
            oper[j - 2] = oper[j - 2] / oper[j - 1];
            --j;
            break;
        case Pow:
            oper[j - 2] = std::pow(oper[j - 2], oper[j - 1]);
            --j;
            break;
        case Sqrt:
            oper[j - 1] = std::sqrt(oper[j - 1]);
            break;
        case Exp:
            oper[j - 1] = std::exp(oper[j - 1]);
            break;
        case Ln:
            oper[j - 1] = std::log(oper[j - 1]);
            break;
        case Log:
            oper[j - 1] = std::log10(oper[j - 1]);
            break;
        case Sin:
            oper[j - 1] = std::sin(oper[j - 1]);
            break;
        case Cos:
            oper[j - 1] = std::cos(oper[j - 1]);
            break;
        case Tan:
            oper[j - 1] = std::tan(oper[j - 1]);
            break;
        case ArcSin:
            oper[j - 1] = std::asin(oper[j - 1]);
            break;
        case ArcCos:
            oper[j - 1] = std::acos(oper[j - 1]);
            break;
        case ArcTan:
            oper[j - 1] = std::atan(oper[j - 1]);
            break;
        case Sinh:
            oper[j - 1] = std::sinh(oper[j - 1]);
            break;
        case Cosh:
            oper[j - 1] = std::cosh(oper[j - 1]);
            break;
        case Tanh:
            oper[j - 1] = std::tanh(oper[j - 1]);
            break;
        case UMinus:
            oper[j - 1] = -1 * oper[j - 1];
            break;
        case Abs:
            oper[j - 1] = std::abs(oper[j - 1]);
            break;
        case Sign:
            if (oper[j - 1] < 0) {
                oper[j - 1] = -1.0;
            } else if (oper[j - 1] > 0) {
                oper[j - 1] = 1.0;
            } else {
                oper[j - 1] = 0.0;
            }
            break;
        }
    }

    //m_ans = oper[j - 1];
    return oper[--j];
}
//=============================================================================================
void MathParser::init(const std::string& expr)
{
    m_sizePostfix = 0; // это делать необязательно
    m_maxSizePostfix = 256; // В случае необходимости увеличивается автоматически
    m_maxSizeOperand = 1024; // Максимальное число операндов в стеке операндов. Сделать побольше
    m_postfix = new Lexeme[m_maxSizePostfix];
    m_operand = new double[m_maxSizeOperand];
    m_success = false; // это делать необязательно

    staticInit(); // Инициализация статических переменных

    compile(expr);
}
std::vector<MathParser::Lexeme> MathParser::stringToInfix(const std::string& expr0)
{
    std::vector<Lexeme> infix;
    infix.reserve(1024);
    std::string expr = "@" + expr0 + "@";
    deleteSpaces(expr);
    toLower(expr);
    addMultToCorrectExpr(expr);
    std::string temp;

    size_t i = 1; // ведь нулевой это '@'
    while (i < expr.size()) {
        if (expr[i] == '@') {
            ++i;
        }
        else if (expr[i] == '(') {
            infix.push_back(Lexeme(0, LBracket));
            ++i;
        }
        else if (expr[i] == ')') {
            infix.push_back(Lexeme(0, RBracket));
            ++i;
        }
        else if (expr[i] == '+') {
            infix.push_back(Lexeme(10, Plus));
            ++i;
        }
        else if (expr[i] == '-') {
            if (expr[i - 1] == '@' || expr[i - 1] == '(') { // то есть если минус унарный
                infix.push_back(Lexeme(40, UMinus));
            } else {
                infix.push_back(Lexeme(10, Minus)); // минус обычный (бинарный)
            }
            ++i;
        }
        else if (expr[i] == '*') {
            infix.push_back(Lexeme(20, Mult));
            ++i;
        }
        else if (expr[i] == '/') {
            infix.push_back(Lexeme(20, Div));
            ++i;
        }
        else if (expr[i] == '^') {
            infix.push_back(Lexeme(30, Pow));
            ++i;
        }
        else if (std::isalpha(expr[i])) {
            temp.clear();
            temp.push_back(expr[i++]);
            while (std::isalpha(expr[i])) {
                temp.push_back(expr[i++]);
            }
            if (temp == std::string("x")) {
                infix.push_back(Lexeme(0, Variable));
            } else {
                std::map<std::string, Type>::iterator it = st_functions.find(temp);
                if (it != st_functions.end()) {
                    infix.push_back(Lexeme(50, (*it).second));
                }
            }
        }
        else if (std::isdigit(expr[i])) {
            temp.clear();
            temp.push_back(expr[i++]);
            while (std::isdigit(expr[i])) {
                temp.push_back(expr[i++]);
            }
            if (expr[i] == '.') {
                temp.push_back(expr[i++]);
                while (std::isdigit(expr[i])) {
                    temp.push_back(expr[i++]);
                }
            }
            infix.push_back(Lexeme(std::atof(temp.c_str()), Number));
        }
    }

    return infix;
}
void MathParser::infixToPostfix(const std::vector<Lexeme>& infix)
{
    if (m_maxSizePostfix < infix.size()) {
        delete[] m_postfix;
        m_maxSizePostfix = infix.size();
        m_postfix = new Lexeme[m_maxSizePostfix];
    }

    m_sizePostfix = 0;

    std::stack<Lexeme> stk;

    for (size_t i = 0, sizeInfix = infix.size(); i < sizeInfix; ++i) {
        Lexeme lex = infix[i];

        if (lex.type == Number || lex.type == Variable) {
            m_postfix[m_sizePostfix++] = lex;
        } else if (lex.type == LBracket) {
            stk.push(lex);
        } else if (lex.type == RBracket) {
            while (stk.top().type != LBracket) {
                m_postfix[m_sizePostfix++] = stk.top();
                stk.pop();
            }
            stk.pop();
        } else { // то есть если оператор
            while (true) {
                if (stk.empty()) {
                    stk.push(lex);
                    break;
                }
                else if (stk.top().type == LBracket) {
                    stk.push(lex);
                    break;
                }
                else if (lex.number > stk.top().number) { // Сравниваются приоритеты операторов
                    stk.push(lex);
                    break;
                }
                else {
                    m_postfix[m_sizePostfix++] = stk.top();
                    stk.pop();
                }
            }
        }
    }

    while (!stk.empty()) {
        m_postfix[m_sizePostfix++] = stk.top();
        stk.pop();
    }
}
//=============================================================================================
