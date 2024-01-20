#include <iostream>
#include <string>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"

int main() {
  auto lexicon = GetLexicon("assignments/wl/words.txt");
  std::string start_word, destination_word;
  std::cout << "Enter start word (RETURN to quit): ";
  std::cin >> start_word;
  std::cout << "Enter destination word: ";
  std::cin >> destination_word;
  auto res = GetWordLadder(start_word, destination_word, lexicon);
  PrintWordLadders(res);
  return 0;
}
