#ifndef MARKOV_TEXT_GENERATOR
#define MARKOV_TEXT_GENERATOR
#include <map>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>

namespace Markov_generator
{

/**
 \brief The class storing the state for a Markov chain.
 */
class Position
{
public:
    std::string last_chars; ///< A string storing the most recent characters.
    std::vector<int> counters; ///< A vector storing additional state e.g. the length of the current word.
    Position();
    Position(std::string _last_chars, std::vector<int> _counters);

    bool operator <(const Position other) const;
};


/**
 \brief The class storing transfer rules between states.
 */
class TransferSettings
{
public:
    void (*outcome)(std::vector<int>&, char); ///< A pointer to the function modifying the counters based on the next char.
    TransferSettings();
    TransferSettings(void (*_outcome)(std::vector<int>&, char));
    
    /**
     \brief Calculate the next position based on the current position and the next character.
     \param [in] p      Current position.
     \param [in] c      The next char.
     \return The position after adding c.
     */
    Position transfer(Position p, char c);
};


/**
 * \brief The class which is used to generate text.
 */
class Generator
{
    TransferSettings settings; ///< The transfer rules.
    int last_chars_length; ///< How many most recent characters are being stored in positions.
    int counters_num; ///< How many counters are there in the vector in the positions.
    std::map<Position, std::map<char, int>> transfers; ///< Map storing the frequency of each char after a position.

public:
    Generator();

    /**
     \brief Add new transfer occurences.
     \param [in] s      The string, from which transfers are extracted.
     */
    void influence_transfers(std::string s);

    /**
     \brief A constructor.
     \param [in] source                 The vector of strings, on which the generator is based.
     \param [in] outcome                The pointer to the function governing the transfers.
     \param [in] _last_chars_length     How many most recent characters are being stored in positions.
     \param [in] _counters_num          How many counters are there in the vector in the positions.
    */
    Generator(std::vector<std::string>& source, void (*outcome)(std::vector<int>&, char), int _last_chars_length, int _counters_num);

    /**
     \brief A constructor.
     \param [in] source                 The istream storing the input, on which the generator is based.
     \param [in] outcome                The pointer to the function governing the transfers.
     \param [in] _last_chars_length     How many most recent characters are being stored in positions.
     \param [in] _counters_num          How many counters are there in the vector in the positions.
     \param [in] breaking_by_line       True, if the strings in different lines are considered to be separate. False, if the input has to be treated as one string.
    */
    Generator(std::istream& source, void (*outcome)(std::vector<int>&, char), int _last_chars_length, int _counters_num, bool breaking_by_line);

    /**
     \brief Get the next char with probability corresponding to the frequency of transfers found in the source strings.
     \param [in] p      The current position.
     \return The next char.
    */
    char get_next_char(Position p);

    /**
     \brief Generate a string.
     \param [in] max_length     The maximum length of the generated string.
     \return The generated string.
     */
    std::string generate(int max_length);

};

};

#endif // MARKOV_TEXT_GENERATOR
