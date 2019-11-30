#include <unordered_map>

template <typename T>
class LStack;

#define OPERATORS 9 // the number of operator
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; // the set of operator
const char pri[OPERATORS][OPERATORS] = { // operator precedence [stack top][current]
    /*               |----------------current operator----------------| */
    /*               +     -     *     /     ^     !     (     )     \0 */
    /* ----- + */   '>',  '>',  '<',  '<',  '<',  '<',  '<',  '>',  '>',
    /*   |   - */   '>',  '>',  '<',  '<',  '<',  '<',  '<',  '>',  '>',
    /*   |   * */   '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',
    /* stack / */   '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',
    /*   |   ^ */   '>',  '>',  '>',  '>',  '>',  '<',  '<',  '>',  '>',
    /*  top  ! */   '>',  '>',  '>',  '>',  '>',  '>',  ' ',  '>',  '>',
    /*   |   ( */   '<',  '<',  '<',  '<',  '<',  '<',  '<',  '=',  ' ',
    /*   |   ) */   ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',
    /* ---- \0 */   '<',  '<',  '<',  '<',  '<',  '<',  '<',  ' ',  '='
};

std::unordered_map<char, Operator> optrToEnumMap = {
   {'+', ADD},
   {'-', SUB},
   {'*', MUL},
   {'/', DIV},
   {'^', POW},
   {'!', FAC},
   {'(', L_P},
   {')', R_P},
   {'\0', EOE}
};

class StackAlgorithm
{
public:
    void convert(LStack<char>& s, int n , int base);

    bool parenthesisMatching(const char exp[] , int low, int hight);

    //evalute Infix expression, for +-*/!^(), and export reverse polish notation
    float evalute(char* S, std::string& RPN); 

    float readNumber(char* &s, std::string& RPN);
    char orderBetween(char stackTop, char current);
    float calculate(char optr, float opnd);
    float calculate(float opndLeft, char optr, float opndRight);

    float evaluteRPN(const std::string& RPN);
};
