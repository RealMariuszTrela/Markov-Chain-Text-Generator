#include <iostream>
#include <fstream>
#include "markov_text_generator.h"

void counter_for_towns(std::vector<int>& v, char c)
{
    int& letters = v[0];
    int& hyphens = v[1];
    int& words = v[2];
    if(c==' ')
    {
        letters = hyphens = 0;
        ++words;
    }
    else if(c=='-')
    {
        ++hyphens;
        letters  = 0;
    }
    else ++letters;
}
void counter_for_sherlock(std::vector<int>& v, char c)
{
    int& letters = v[0];
    int& inside_quote = v[1];
    if(c==' '||c=='\n')
    {
        letters = 0;
    }
    else ++letters;
    if(c=='"') inside_quote ^= 1;
}
int main()
{
    srand(time(NULL));
    std::ifstream towns_input("towns.txt"), sherlock_input("sherlock_holmes.txt");
    Markov_generator::Generator towns(towns_input, counter_for_towns, 3, 3, 1);
    Markov_generator::Generator sherlock(sherlock_input, counter_for_sherlock, 4, 2, 0);
    std::cout << "Towns\n";
    for(int i=0; i<10; ++i)
    {
        std::cout << towns.generate(100) << "\n";
    }
    std::cout << "\n\nSherlock:\n";
    std::cout << sherlock.generate(2000);
}
