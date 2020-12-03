/*
Process:
sin  = s
cos  = c
tg   = t
ctg  = g
exp  = n
sqrt = q
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
        str.erase(pos, 1);
        str.erase(pos + 1, 2);
    }

    pos = 0;
    while ((pos = str.find("pi", pos)) != -1)
        str.erase(pos + 1, 1);
}

bool is_number(char a)
{
    return (a == 'x' || a == 'p' || a == 'e' || (a >= '0' && a <= '9'));
}

bool is_pref_func(char a)
{
    return (a == 's' || a == 'c' || a == 't' || a == 'g' || a == 'n' || a == 'q');
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
    //if (a - std::round(a) < 0.00001 || std::round(a) - a > -0.00001) //abs doesn't work
    if (fabs(a - std::round(a)) < 0.00001)
        return std::round(a);
    else return a;
}

void calc (std::vector<float>& st, char oper)
{
    float b = pop (st), a;

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

        case 'q':
            st.push_back(own_round(std::sqrt(b)));
            break;

    }
}

int main ()
{
    while (true)
    {
        bool un_flag = true;
        bool is_x = false;
        std::vector <char> oper;
        std::vector <char> out;
        std::string str;
        std::cout << "> ";
        std::getline(std::cin, str);

        if (str == "quit" || str == "q")
            break;

        process(str);

        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == ' ');

            else if (is_number(str[i]))
            {
                if (un_flag && str[i-1] == '-')
                {
                        out.push_back('-');
                        out.push_back(str[i]);

                } else out.push_back(str[i]);

                if (str[i] == 'x')
                    is_x = true;

                if (str[i+1] == ')' || is_oper(str[i+1]) || i == str.size() - 1 || str[i+1] == ' ')
                    out.push_back(',');                     //ending of a number

                un_flag = false;

            } else if (is_pref_func(str[i]))
            {
                oper.push_back(str[i]);
                un_flag = false;

            } else if (str[i] == '.')
            {
                out.push_back(str[i]);
                un_flag = false;

            } else if (str[i] == '(')
            {
                oper.push_back(str[i]);
                un_flag = true;

            } else if (str[i] == ')')
            {
                while (oper.back() != '(')
                {
                    out.push_back(oper.back());
                    out.push_back(',');
                    oper.pop_back();
                }
                oper.pop_back();
                un_flag = false;

                if (is_pref_func(oper.back()))
                {
                    out.push_back(oper.back());
                    out.push_back(',');
                    oper.pop_back();
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

        if ( is_x ) // inserting of x
        {
            std::string x;
            std::cout << "Please, enter a value of x: ";
            std::cin >> x;

            auto it = out.begin();
            while (*it != 'x')
                ++it;

            out.erase(it);

            if (x == "pi")
            {
                x = "p";
            }

            for (int i = x.size() - 1; i >= 0; i--)
               out.insert(it, x[i]); 
        }

        for (auto x : out)
            std::cout << x;

        std::cout << std::endl;

        //Calculating
        
        std::vector <float> st;

        for (int i = 0; i < out.size(); i++)
        {
            if (is_number(out[i]) || (out[i] == '-' && (out[i-1] == ',' || i == 0) && out[i+1] != ','))
            {
                std::string num;

                int a = 0;
                while (out[i] != ',')
                {
                    num.push_back(out[i]);
                    a++; i++;
                }

                if (num == "p")
                    st.push_back(M_PI);
                else if (num == "e")
                    st.push_back(M_E);
                else
                    st.push_back(std::stof(num));

                num.clear();

            } else if (is_oper(out[i]))
                calc(st, out[i]);
            
            else if (is_pref_func(out[i]))
                calc(st, out[i]);
        }

        std::cout << "Answer: " << st[0] << std::endl;

        is_x = false;
        //out.clear();
        //oper.clear();
    }

    return 0;
}


