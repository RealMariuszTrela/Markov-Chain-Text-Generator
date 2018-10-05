#include "markov_text_generator.h"
#include <iostream>

using namespace Markov_generator;

Position::Position() {}

Position::Position(std::string _last_chars, std::vector<int> _counters):
    last_chars(_last_chars), counters(_counters) {}

bool Position::operator <(const Position other) const
{
    if(last_chars!=other.last_chars) return last_chars < other.last_chars;
    return counters < other.counters;
}


TransferSettings::TransferSettings() {}

TransferSettings::TransferSettings(void (*_outcome)(std::vector<int>&, char)):
    outcome(_outcome) {}

Position TransferSettings::transfer(Position p, char c)
{
    outcome(p.counters, c);
    p.last_chars.erase(0, 1);
    p.last_chars += c;
    return p;
}


Generator::Generator() {}

void Generator::influence_transfers(std::string s)
{
    Position p(std::string(last_chars_length, 0), std::vector<int>(counters_num, 0));
    for(int i=0; i<(int)s.size(); ++i)
    {
        ++transfers[p][s[i]];
        p = settings.transfer(p, s[i]);
    }
    ++transfers[p][0];
}

Generator::Generator(std::vector<std::string>& source, void (*outcome)(std::vector<int>&, char),
                    int _last_chars_length, int _counters_num):
        settings(outcome), last_chars_length(_last_chars_length), counters_num(_counters_num)
{
    for(std::string& s: source) influence_transfers(s);
}

Generator::Generator(std::istream& source, void (*outcome)(std::vector<int>&, char),
                    int _last_chars_length, int _counters_num, bool breaking_by_line):
    settings(outcome), last_chars_length(_last_chars_length), counters_num(_counters_num)
{
    if(breaking_by_line)
    {
        std::string s;
        while(std::getline(source, s))
        {
            influence_transfers(s);
        }
    }
    else
    {
        std::string s, whole_text;
        while(std::getline(source, s))
        {
            whole_text += s+'\n';
        }
        influence_transfers(whole_text);
    }
}

char Generator::get_next_char(Position p)
{
    int sum = 0;
    for(auto i: transfers[p]) sum += i.second;
    if(sum == 0) return 0;
    int val = rand()%sum;
    for(auto i: transfers[p])
    {
        if(i.second>val)
        {
            return i.first;
        }
        val -= i.second;
    }
    return 0; //shouldn't happen
}

std::string Generator::generate(int max_length)
{
    std::string res;
    Position p(std::string(last_chars_length, 0), std::vector<int>(counters_num, 0));
    while((int)res.size()<max_length)
    {
        char c = get_next_char(p);
        if(c == 0) break;
        res += c;
        p = settings.transfer(p, c);
    }
    return res;
}
