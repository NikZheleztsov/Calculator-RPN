/*
Process:
sin  = s
cos  = c
tg   = t
ctg  = g
exp  = n
sqrt = r
pi   = p
e    = e

Priority:
1. ^
2. * /
3. + -
4. ( )

*/

#define _USE_MATH_DEFINES //M_PI M_E

#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <string>

void help ()
{
    std::cout << "\nCalc - a command line calculator utility,\n";
    std::cout << "which supports brackets and such operations\n";
    std::cout << "as +, -, *, /, sin(), cos(), tg(), ctg(), exp(),\n";
    std::cout << "sqrt(), constants pi and 'e' and also a variable x\n";
    std::cout << "(one value, not an expression!) Please, pay attention\n";
    std::cout << "to the syntax of the program! Otherwise, program's behavior \n";
    std::cout << "is undefined) Enter 'q' in order to exit program\n\n";
}

void process (std::string& str)
{
    int pos = 0;
    while ((pos = str.find("sin", pos)) != -1)
        str.erase(pos + 1, 2);

    pos = 0;
    while ((pos = str.find("cos", pos)) != -1)
        str.erase(pos + 1, 2);

    pos = 0;
    while ((pos = str.find("ctg", pos)) != -1)
        str.erase(pos, 2);

    pos = 0;
    while ((pos = str.find("tg", pos)) != -1)
        str.erase(pos + 1, 1);
    
    pos = 0;
    while ((pos = str.find("exp", pos)) != -1)
    {
        str.erase(pos, 3);
        str.insert(pos, "n");
    }

    pos = 0;
    while ((pos = str.find("sqrt", pos)) != -1)
    {
        str.erase(pos, 2);
        str.erase(pos + 1, 1);
    }

    pos = 0;
    while ((pos = str.find("pi", pos)) != -1)
        str.erase(pos + 1, 1);

    pos = 0;
    while ((pos = str.find(' ', pos)) != -1)
        str.erase(pos, 1);
}

bool is_number(char a)
{
    return (a == 'x' || a == 'p' || a == 'e' || (a >= '0' && a <= '9'));
}

bool is_pref_func(char a)
{
    return (a == 's' || a == 'c' || a == 't' || a == 'g' || a == 'n' || a == 'r');
}

bool is_oper (char a)
{
    return (a == '+' || a == '-' || a == '*' || a == '/' || a == '^');
}

bool is_less_or_eq_prior (char a, char b) // a <= b
{
    int x, y;

    a == '^' ? x = 3 : a == '*' || a == '/' ? x = 2 : a == '+' || a == '-' ? x = 1 : x = 0;
    b == '^' ? y = 3 : b == '*' || b == '/' ? y = 2 : b == '+' || b == '-' ? y = 1 : y = 0;

    if ( x - y <= 0 )
        return 1;

    else return 0;
}

float pop (std::vector<float>& vec)
{
    float a = vec[vec.size() - 1];
    vec.pop_back();
    return a;
}

float own_round (float a)
{
    if (fabs(a - std::round(a)) < 0.00001)
        return std::round(a);
    else return a;
}

void calc (std::vector<float>& st, char oper)
{
    float a,b;

    if (st.size() == 0)
    {
        std::cout << "Error! Value not found\n";

    } else {

        b = pop (st), a;

        if (is_oper(oper))
            a = pop (st);

        switch (oper)
        {
            case '+':
                st.push_back(own_round(a + b));
                break;

            case '-':
                st.push_back(own_round(a - b));
                break;

            case '*':
                st.push_back(own_round(a * b));
                break;

            case '/':
                st.push_back(own_round(a/b));
                break;

            case '^':
                st.push_back(own_round(pow(a,b)));
                break;

            case 's':
                st.push_back(own_round(sin(b)));
                break;

            case 'c':
                st.push_back(own_round(cos(b)));
                break;

            case 't':
                st.push_back(own_round(tan(b)));
                break;

            case 'g': 
                st.push_back(own_round(pow(tan(b), -1)));
                break;

            case 'n': 
                st.push_back(own_round(exp(b)));
                break;

            case 'r':
                st.push_back(own_round(std::sqrt(b)));
                break;
        }

    }
}

int main ()
{
    std::cout << "Calc\nq - exit, h - help\n\n";
    std::string str;

    while (str != "q")
     {
        bool un_flag = true;
        int op_par = 0, cl_par = 0;
        std::vector <char> oper;
        std::vector <char> out;

        std::cout << "> ";
        std::getline(std::cin, str);

        if (str == "h" || str == "help")
            help();

        process(str);

        //Starting of coverting to reverse polish notation
        for (int i = 0; i < str.size(); i++)
        {
            if (is_number(str[i]))
            {
                if (un_flag && str[i-1] == '-')
                {
                        out.push_back('-');
                        out.push_back(str[i]);

                } else if (str[i] == 'x')
                {
                    std::string x;
                    std::cout << "Please, enter a value of x: ";
                    std::cin >> x;

                    int pos = 0; // spaces
                    while ((pos = x.find(' ', pos)) != -1)
                        x.erase(pos, 1);

                    if (x == "pi")
                        x = "p";
                   
                    std::cin.ignore(1000, '\n');
                    for (int j = 0; j < x.size(); j++)
                        out.push_back(x[j]);

                } else out.push_back(str[i]);

                if (str[i+1] == ')' || is_oper(str[i+1]) || i == str.size() - 1 || str[i+1] == ' ' || str[i+1] == '(' || is_pref_func(str[i+1]))
                    out.push_back(','); //ending of a number

                if ((str[i+1] == '(') || (str[i-1] == ')')) //*
                    oper.push_back('*');

                un_flag = false;

            } else if (is_pref_func(str[i]))
            {
                if (is_number(str[i-1]) || str[i-1] == ')') //*
                    oper.push_back('*');

                oper.push_back(str[i]);

                un_flag = false;

            } else if (str[i] == '.')
            {
                out.push_back(str[i]);
                un_flag = false;

            } else if (str[i] == '(')
            {
                if (str[i-1] == ')') // *
                    oper.push_back('*');

                oper.push_back(str[i]);
                un_flag = true;
                op_par++;

            } else if (str[i] == ')')
            {
                if (oper.size() != 0)
                {
                    while (oper.back() != '(')
                    {
                        out.push_back(oper.back());
                        out.push_back(',');
                        oper.pop_back();
                    }
                    oper.pop_back();
                    un_flag = false;
                    cl_par++;

                    if (is_pref_func(oper.back()))
                    {
                        out.push_back(oper.back());
                        out.push_back(',');
                        oper.pop_back();
                    }

                } else {
                    std::cout << "Error. Value not found\n";
                    break; // )))) не goto
                }

            } else if (is_oper(str[i]) && !un_flag)
            {
                if (oper.size() != 0)
                {
                    while (is_less_or_eq_prior(str[i], oper.back()))
                    {
                        out.push_back(oper.back());
                        out.push_back(',');
                        oper.pop_back();
                    }
                }
                oper.push_back(str[i]);
            }
        }   

        while (oper.size() != 0)
        {
            out.push_back(oper.back());
            out.push_back(',');
            oper.pop_back();
        }
        //Endfinr RPN

        if (op_par != cl_par)
        {
            std::cout << "Error! Missing parantheses\n";
            continue;
        }

        /*
        for (auto x : out)
            std::cout << x;

        std::cout << std::endl;
        */

        //Calculating
        
        std::vector <float> st;

        for (int i = 0; i < out.size(); i++)
        {
            if (is_number(out[i]) || (out[i] == '-' && (out[i-1] == ',' || i == 0) && out[i+1] != ','))
            {
                std::string num;

                int a = 0;
                while (out[i] != ',' && a < out.size())
                {
                    num.push_back(out[i]);
                    a++; i++;
                }

                if (num == "p")
                    st.push_back(M_PI);
                else if (num == "e")
                    st.push_back(M_E);
                else
                {
                    try {
                        st.push_back(std::stof(num));
                    } catch (...) {
                        std::cout << "Error in sytax\n";
                        break;
                    }
                }

                num.clear();

            } else if (is_oper(out[i]))
                calc(st, out[i]);
            
            else if (is_pref_func(out[i]))
                calc(st, out[i]);
        }

        if (st.size() > 0)
            std::cout << "-> " << st[0] << std::endl;

    } 

    return 0;
}


