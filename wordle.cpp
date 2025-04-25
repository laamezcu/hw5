#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void getWords(string cur, size_t index, const string& in, string rFloating,
              const set<string>& dict, set<string>& result);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> result;
    getWords("", 0, in, floating, dict, result);
    return result;
}

// Define any helper functions here
void getWords(string cur, size_t index, const string& in, string rFloating,
              const set<string>& dict, set<string>& result)
{
    // Base case: entire word candidate
    if (index == in.length()) {
        // check if rFloat letters used in cur in dict
        if (rFloating.empty() && dict.find(cur) != dict.end()) {
            result.insert(cur);
        }
        return;
    }

    // Cur is a fixed letter
    if (in[index] != '-') {
        getWords(cur + in[index], index + 1, in, rFloating, dict, result);
        return;
    }

    for (size_t i = 0; i < rFloating.size(); ++i) {
        // place floating letter in cur
        char c = rFloating[i];
        string newRemaining = rFloating.substr(0, i) + rFloating.substr(i + 1);
        getWords(cur + c, index + 1, in, newRemaining, dict, result);
    }

    // Try non-floating letters if there are still letters to fill
    if (cur.size() + rFloating.size() < in.size()) {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (rFloating.find(c) != string::npos) {
                continue; // skip if c is in rFloating
            }
            getWords(cur + c, index + 1, in, rFloating, dict, result);
        }
    }

}