
std::set<std::list<std::string>> GetWordLadder(const std::string& start_word,
                                               const std::string& destination_word,
                                               std::unordered_set<std::string>& lexicon) {
  std::queue<std::pair<std::string, int>> q1, q2;
  std::unordered_set<std::string> visited_words_set_1, visited_words_set_2;
  const std::string ALPHABET{"abcdefghijklmnopqrstuvwxyz"};
  // manually fill in m2 to find one-hop solutions
  std::unordered_map<std::string, std::unordered_set<std::string>> m1,
      m2{{destination_word, std::unordered_set<std::string>{}}};
  std::set<std::list<std::string>> word_ladders;
  std::unordered_set<std::string> common_words_set;
  auto direction = true;
  // delete impossible words
  for (auto i = lexicon.cbegin(), last = lexicon.cend(); i != last;) {
    if ((*i).length() != start_word.length())
      i = lexicon.erase(i);
    else
      ++i;
  }
  auto current_distance_1 = 1, current_distance_2 = 1;
  auto distance = static_cast<int>(lexicon.size());
  // Bidirectional BFS
  q1.push(std::make_pair(start_word, 1));
  q2.push(std::make_pair(destination_word, 1));
  while (!q1.empty() && !q2.empty()) {
    std::pair<std::string, int> p;
    // forward
    if (direction) {
      p = q1.front();
      q1.pop();
      if (current_distance_1 < p.second) {
        // delete visited words
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
        // delete visited words
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
        // check if a solution happens
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
        // add pair(back : [front])
        if (direction) {
          const auto vec = m1.find(word);
          if (vec == m1.end())
            m1.emplace(word, std::unordered_set{copied_word});
          else
            vec->second.emplace(copied_word);
        } else {  // add pair(front : [back])
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
      w = save;
    }
  }
  // fill in the word ladders
  std::list<std::string> l;
  for (const auto& w : common_words_set) {
    l.emplace_back(w);
    GetPathBackward(m1, m2, l, word_ladders, start_word, destination_word, w);
    l.clear();
  }
  return word_ladders;
}

void GetPathBackward(const std::unordered_map<std::string,
                                              std::unordered_set<std::string>>& m1,
                     const std::unordered_map<std::string,
                                              std::unordered_set<std::string>>& m2,
                     std::list<std::string>& l,
                     std::set<std::list<std::string>>& word_ladders,
                     const std::string& start_word,
                     const std::string& destination_word,
                     const std::string& word) {
  if (word == start_word) {
    GetPathForward(m2, l, word_ladders, destination_word, l.back());
    return;
  }
  const auto i = m1.find(word);
  if (i != m1.end()) {
    for (const auto& p : i->second) {
      l.emplace_front(p);
      GetPathBackward(m1, m2, l, word_ladders, start_word, destination_word, p);
      l.pop_front();
    }
  }
}

void GetPathForward(const std::unordered_map<std::string,
                                             std::unordered_set<std::string>>& m2,
                    std::list<std::string>& l,
                    std::set<std::list<std::string>>& word_ladders,
                    const std::string& destination_word,
                    const std::string& word) {
  if (word == destination_word) {
    word_ladders.emplace(l);
    return;
  }
  const auto i = m2.find(word);
  if (i != m2.end()) {
    for (const auto& p : i->second) {
      l.emplace_back(p);
      GetPathForward(m2, l, word_ladders, destination_word, p);
      l.pop_back();
    }
  }
}
