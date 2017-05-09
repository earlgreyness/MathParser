#ifndef MATHPARSER_H
#define MATHPARSER_H

#include <vector>
#include <string>
#include <map>

class MathParser {
private:
    enum Type {
        Number,   //done
        Variable, //done
        LBracket, //done
        RBracket, //done
        Plus,     //done
        Minus,    //done
        UMinus,   //done
        Mult,     //done
        Div,      //done
        Pow,      //done
        Sqrt,     //done
        Sin,      //done
        Cos,      //done
        Tan,      //done
        //Cot,
        ArcSin,   //done
        ArcCos,   //done
        ArcTan,   //done
        //ArcCot,
        Sinh,     //done
        Cosh,     //done
        Tanh,     //done
        //Coth,
        Exp,      //done
        Log,      //done
        Ln,       //done
        Abs,      //done
        Sign      //done
    };

    struct Lexeme {
        double number;
        Type   type;
        Lexeme(double n, Type t) : number(n), type(t) { }
        Lexeme() { }
    };

public:
    MathParser(                 );
    MathParser(const MathParser&);
    /*
    Мы не знаем, успешно ли произошел разбор строки,
    и, если не успешно, то evaluate() будет возвращать 0.0.
    Лучше будет убрать вовсе такой конструктор, однако, тогда мы лишимся возможности писать, например, так:
    std::cout << MathParser("2 + 2").evaluate();
    т.е. лишимся возможности получать результат через временные объекты
    */
    explicit MathParser(const std::string&);
    ~MathParser();
    MathParser& operator=(const MathParser&);

    bool compile(const std::string& expr);
    double evaluate(double variable = 0.0) const;
    bool isSuccess() const { return m_success; }

private:
    void init(const std::string& expr);
    static std::vector<Lexeme> stringToInfix(const std::string& expr);
    void infixToPostfix(const std::vector<Lexeme>& infix);

    Lexeme* m_postfix;
    double* m_operand;        // стек операндов, используется только в evaluate()
    size_t  m_sizePostfix;
    size_t  m_maxSizePostfix;
    size_t  m_maxSizeOperand;
    bool    m_success;        // был ли последний вызов compile() успешным

    // Проверяет, является ли строка корректной с точки зрения анализатора
    // (можно ли ее анализировать - разбирать на лексемы)
    static bool isCorrect(const std::string&);
    static void staticInit();
    static bool isValidBrackets(const std::string&);
    static void toLower(std::string&); // все буквы в нижний регистр
    static void deleteSpaces(std::string& expr);
    static void addMultToCorrectExpr(std::string& expr); // добавляет опущенные знаки умножения

    static std::map<std::string, Type> st_functions;
    static const std::string           st_symbols;
};

#endif // MATHPARSER_H
