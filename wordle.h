#ifndef WORDLE_H
#define WORDLE_H

#ifndef RECCHECK
#include <set>
#include <string>
#include <vector>
#endif

/**
 * @brief Returns the list of all legal words in the provided dictionary
 *        that meet the criteria provided in the `in` and `floating`
 *        inputs.
 * 
 * @param [in] in - string of fixed, correct characters and `-`s to indicate
 *                  the length of string and locations that must be filled in
 * @param [in] floating  - Characters that must be used somewhere in the word
 * @param [in] dict - Dictionary of strings of legal words
 * @return std::set<std::string> - Set of all words that meet the criteria
 */
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict);
void permutation(const std::string& floating, std::set<std::string>& oldresult, size_t index);
void findRightPositionsHelper(const std::string& in, std::set< size_t>& right_positions, std::string& current, size_t  index);
void findRightPositions(const std::string& in, std::set< size_t>& right_positions,std::string& current);
void replace_dash(std::string& word_cpy, const std::string& perm,  size_t i);
#endif