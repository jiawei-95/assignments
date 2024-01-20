#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void PrintWordLadders(const std::vector<std::vector<std::string>>&);
std::vector<std::vector<std::string>>
GetWordLadder(const std::string&, const std::string&, std::unordered_set<std::string>&);
void GetPathBackward(const std::unordered_map<std::string, std::unordered_set<std::string>>&,
                     const std::unordered_map<std::string, std::unordered_set<std::string>>&,
                     std::vector<std::string>&,
                     std::vector<std::vector<std::string>>&,
                     const std::string&,
                     const std::string&,
                     const std::string&);
void GetPathForward(const std::unordered_map<std::string, std::unordered_set<std::string>>&,
                    std::vector<std::string>&,
                    std::vector<std::vector<std::string>>&,
                    const std::string&,
                    const std::string&);
#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
