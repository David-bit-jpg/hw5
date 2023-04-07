#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <string>
#include <vector>
#endif

#include "wordle.h"
#include "dict-eng.h"

using namespace std;
void permutation(std::string& floating, std::set<std::string>& oldresult, size_t index)
{
   //cout<<"hello"<<endl;
    if (index == floating.length())
    {
      //cout<<"stop"<<endl;
        return;
    }
    else
    {
      //cout<<"I'm here"<<endl;
        char temp = floating[index];
        std::set<std::string> result;
        //cout<<temp<<endl;
        if (oldresult.empty())
        {
            result.insert(std::string(1, temp));
        }
        else
        {
          //cout<<"go for loop"<<endl;
            for (const std::string& x : oldresult)
            {
                for (size_t i = 0; i <= x.length(); i++)
                {
                    //cout<<x<<endl;
                    std::string y = x;
                    y.insert(i, 1, temp);
                    result.insert(y);
                }
            }
        }
        oldresult = result;
        permutation(floating, oldresult, index + 1);
    }
}

void findRightPositionsHelper(const std::string& in, std::set<size_t>& right_positions,  size_t index)
{
  if (index ==  in.length())
  {
    return;
  }
  else
  {
    if (in[index] != '-')
    {
      right_positions.insert(index);
    }
    findRightPositionsHelper(in, right_positions, index+1);
  }
}
void findRightPositions(const std::string& in, std::set< size_t>& right_positions)
{
  findRightPositionsHelper(in, right_positions, 0);
}
void replace_dash(std::string& word_cpy, const std::string& perm,  size_t i)
{
    if (i == perm.length()) 
    {
      return;
    }
    else 
    {
      if (perm[i] == '-') 
      {
        word_cpy[i] = '-';
      }
      replace_dash(word_cpy, perm, i+1);
    }
}

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{

    std::set<std::string> final_results;
    if(floating==""&&in!=""&&in.find('-')==string::npos)
    {
      final_results.insert(in);
      return final_results;
    }
    std::string current = "";
    if(floating!="")
    current = floating;
    size_t len = in.length();
    std::set<std::string> results;
    std::set<size_t> right_positions;
    std::string head = "";
    std::string tail = "";
    if(in!="")
    tail = in.substr(in.find('-'));

    //currently 2 for loops
    //current = floating+'-'s
    // for (auto element : right_positions) {
    //   std::cout << "right_positions: ";
    // std::cout << element << std::endl;
    // }
    // std::cout << "current: ";
    //  std::cout << current << std::endl;
      head = in.substr(0,in.find('-'));
      // cout<<"head: "<<head<<endl;
      // cout<<"tail: "<<tail<<endl;
      size_t len2 = tail.length();
      if(floating!="")
      current.append(len2- floating.length(),'-');
      else
      current.append(len2,'-');
      // cout<<"current big "<<current<<endl;
      findRightPositions(tail, right_positions);
      permutation(current, results,0);
      // cout<<"permutation done"<<endl;
    //     for (auto element : results) {
    //   std::cout << "permutations: ";
    // std::cout << element << std::endl;
    // }
    for (auto it = dict.begin(); it != dict.end(); ++it)//3
    {
      string word = *it;
      if(word.length() ==  len)//correct length
      {
        for (auto it2 = results.begin(); it2 != results.end(); ++it2)//4
        {
          std::string perm = *it2;
          for( size_t ind: right_positions)//5
          {
              if(perm[ind]=='-')
                {
                  perm[ind] = tail[ind];
                }
                //perm = ---+correct things+floating letters
                else
                {
                  results.erase(results.find(*it2));//not empty
                  break;
                }
          }
          //cout<<"perm = "<<perm<<endl;//still -
          std::string word_cpy = word;
            replace_dash(word_cpy,(head+perm),0);
            if((head+perm==word_cpy) && ((std::all_of(word.begin(), word.end(), [](char c){ return std::islower(c); }) )|| (std::all_of(word.begin(), word.end(), [](char c){ return std::isupper(c); }))))
            {
              final_results.insert(word);
            }
        }
      }
    }
    return final_results;
}
