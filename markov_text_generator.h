#ifndef MARKOV_TEXT_GENERATOR
#define MARKOV_TEXT_GENERATOR
#include <map>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>

namespace Markov_generator
{

class Position
{
public:
    std::string last_chars;
    std::vector<int> counters;
    Position();
    Position(std::string _last_chars, std::vector<int> _counters);

    bool operator <(const Position other) const;
};

class TransferSettings
{
public:
    void (*outcome)(std::vector<int>&, char);
    TransferSettings();
    TransferSettings(void (*_outcome)(std::vector<int>&, char));
    Position transfer(Position p, char c);
};

class Generator
{
public:
    TransferSettings settings;
    int last_chars_length;
    int counters_num;
    std::map<Position, std::map<char, int>> transfers;

    Generator();

    void influence_transfers(std::string s);

    Generator(std::vector<std::string>& source, void (*outcome)(std::vector<int>&, char), int _last_chars_length, int _counters_num);


    Generator(std::istream& source, void (*outcome)(std::vector<int>&, char), int _last_chars_length, int _counters_num, bool breaking_by_line);

    char get_next_char(Position p);

    std::string generate(int max_length);

};

};

#endif // MARKOV_TEXT_GENERATOR
