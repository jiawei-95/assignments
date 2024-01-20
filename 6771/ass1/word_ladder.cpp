#include "assignments/wl/word_ladder.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// print out the all word ladders in the word_ladders_vector
void PrintWordLadders(const std::vector<std::vector<std::string>>& word_ladders_vector) {
  if (!word_ladders_vector.empty()) {
    std::cout << "Found ladder: ";
  } else {
    std::cout << "No ladder found.\n";
    return;
  }
  for (const auto& v : word_ladders_vector) {
    for (const auto& i : v) {
      std::cout << i << ' ';
    }
    std::cout << '\n';
  }
}

// use bidirectional BFS to find all word ladders from start_word to destination_word
// return an vector<vector<string >> object storing all found word ladders in lexicographic order
std::vector<std::vector<std::string>> GetWordLadder(const std::string& start_word,
                                                    const std::string& destination_word,
                                                    std::unordered_set<std::string>& lexicon) {
  // init variables
  std::queue<std::pair<std::string, int>> q1, q2;
  std::unordered_set<std::string> visited_words_set_1, visited_words_set_2;
  const std::string ALPHABET{"abcdefghijklmnopqrstuvwxyz"};
  // manually insert destination word to solve one-hop word ladders
  std::unordered_map<std::string, std::unordered_set<std::string>> m1,
      m2{{destination_word, std::unordered_set<std::string>{}}};
  std::vector<std::vector<std::string>> word_ladders_vector{};
  std::unordered_set<std::string> common_words_set;
  auto direction = true;
  // handle non-exist word ladders to avoid unnecessary search
  if (start_word.length() != destination_word.length())
    return word_ladders_vector;
  if (lexicon.find(start_word) == lexicon.end() || lexicon.find(destination_word) == lexicon.end())
    return word_ladders_vector;
  // delete words with different length
  for (auto i = lexicon.cbegin(), last = lexicon.cend(); i != last;) {
    if ((*i).length() != start_word.length())
      i = lexicon.erase(i);
    else
      ++i;
  }
  // Bidirectional BFS
  auto current_distance_1 = 1, current_distance_2 = 1;
  auto distance = static_cast<int>(lexicon.size());
  q1.push(std::make_pair(start_word, 1));
  q2.push(std::make_pair(destination_word, 1));
  while (!q1.empty() && !q2.empty()) {
    std::pair<std::string, int> p;  // p is a pair storing {word, level}
    // forward
    if (direction) {
      p = q1.front();
      q1.pop();
      if (current_distance_1 < p.second) {
        // delete visited words when searching next level
        for (const auto& w : visited_words_set_1)
          lexicon.erase(w);
        visited_words_set_1.clear();
        current_distance_1 = p.second;
        // change direction
        direction = !direction;
        q1.push(p);
        if (distance < current_distance_1 + current_distance_2)
          break;
        continue;
      }
    } else {  // backward
      p = q2.front();
      q2.pop();
      if (current_distance_2 < p.second) {
        // delete visited words when searching next level
        for (const auto& w : visited_words_set_2)
          lexicon.erase(w);
        visited_words_set_2.clear();
        current_distance_2 = p.second;
        // change direction
        direction = !direction;
        q2.push(p);
        if (distance < current_distance_1 + current_distance_2)
          break;
        continue;
      }
    }
    auto word = p.first;
    const auto copied_word = word;
    for (auto& w : word) {
      const char save = w;
      for (const auto& a : ALPHABET) {
        if (a == save)
          continue;
        w = a;
        // check if a common word appears
        // if yes, add to common_words_set and go to the next loop
        if (direction) {
          if (m2.find(word) != m2.end()) {
            distance = current_distance_1 + current_distance_2;
            common_words_set.emplace(word);
            // manually fill in the map
            const auto vec = m1.find(word);
            if (vec == m1.end())
              m1.emplace(word, std::unordered_set{copied_word});
            else
              vec->second.emplace(copied_word);
            continue;
          }
        } else {
          if (m1.find(word) != m1.end()) {
            distance = current_distance_1 + current_distance_2;
            common_words_set.emplace(word);
            const auto vec = m2.find(word);
            if (vec == m2.end())
              m2.emplace(word, std::unordered_set{copied_word});
            else
              vec->second.emplace(copied_word);
            continue;
          }
        }
        // check legal word
        if (lexicon.find(word) == lexicon.end())
          continue;
        if (direction) {  // add pair {back : [front]}
          const auto vec = m1.find(word);
          if (vec == m1.end())
            m1.emplace(word, std::unordered_set{copied_word});
          else
            vec->second.emplace(copied_word);
        } else {  // add pair {front : [back]}
          const auto vec = m2.find(word);
          if (vec == m2.end())
            m2.emplace(word, std::unordered_set{copied_word});
          else
            vec->second.emplace(copied_word);
        }
        // push next-level word in queue
        auto copied_pair = p;
        copied_pair.first = word;
        (copied_pair.second)++;
        if (direction) {
          visited_words_set_1.emplace(word);
          q1.push(copied_pair);
        } else {
          visited_words_set_2.emplace(word);
          q2.push(copied_pair);
        }
      }
      w = save;  // recover the word
    }
  }
  // fill in the word ladders
  std::vector<std::string> vec;
  for (const auto& w : common_words_set) {
    vec.emplace_back(w);
    GetPathBackward(m1, m2, vec, word_ladders_vector, start_word, destination_word, w);
    vec.clear();
  }
  std::sort(word_ladders_vector.begin(), word_ladders_vector.end());
  return word_ladders_vector;
}

// use DFS to find the word ladder path from the common word to the start_word
void GetPathBackward(const std::unordered_map<std::string, std::unordered_set<std::string>>& m1,
                     const std::unordered_map<std::string, std::unordered_set<std::string>>& m2,
                     std::vector<std::string>& vec,
                     std::vector<std::vector<std::string>>& word_ladders_vector,
                     const std::string& start_word,
                     const std::string& destination_word,
                     const std::string& word) {
  if (word == start_word) {
    std::vector<std::string> reversed_vector(vec.rbegin(), vec.rend());
    GetPathForward(m2, reversed_vector, word_ladders_vector, destination_word,
                   reversed_vector.back());
    return;
  }
  const auto i = m1.find(word);
  if (i != m1.end()) {
    for (const auto& p : i->second) {
      vec.emplace_back(p);
      GetPathBackward(m1, m2, vec, word_ladders_vector, start_word, destination_word, p);
      vec.pop_back();
    }
  }
}

// use DFS to find the word ladder path from the common word to the destination_word
void GetPathForward(const std::unordered_map<std::string, std::unordered_set<std::string>>& m2,
                    std::vector<std::string>& vec,
                    std::vector<std::vector<std::string>>& word_ladders_vector,
                    const std::string& destination_word,
                    const std::string& word) {
  if (word == destination_word) {
    word_ladders_vector.emplace_back(vec);
    return;
  }
  const auto i = m2.find(word);
  if (i != m2.end()) {
    for (const auto& p : i->second) {
      vec.emplace_back(p);
      GetPathForward(m2, vec, word_ladders_vector, destination_word, p);
      vec.pop_back();
    }
  }
}
