
std::set<std::list<std::string>> GetWordLadder(const std::string& startWord,
                                               const std::string& desWord,
                                               std::unordered_set<std::string>& lexicon) {
  std::queue<std::pair<std::string, int>> q;
  std::unordered_set<std::string> visitedWords;
  const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
  std::unordered_map<std::string, std::unordered_set<std::string>> m;
  // delete impossible words
  lexicon.erase(startWord);
  for (auto i = lexicon.begin(), last = lexicon.end(); i != last;) {
    if ((*i).length() != startWord.length()) {
      i = lexicon.erase(i);
    } else {
      ++i;
    }
  }
  std::pair<std::string, int> startPair(startWord, 1);
  int currentDistance = 1;
  auto distance = static_cast<int>(lexicon.size());
  // BFS
  q.push(startPair);
  while (!q.empty()) {
    auto p = q.front();
    q.pop();
    // visit next level
    if (currentDistance < p.second) {
      // delete visited words
      for (const auto& w : visitedWords) {
        lexicon.erase(w);
      }
      visitedWords.clear();
      currentDistance = p.second;
      if (distance < currentDistance) {
        break;
      }
    }
    auto word = p.first;
    const auto copiedWord = word;
    for (auto& w : word) {
      const char save = w;
      for (const auto& a : alphabet) {
        if (a == save)
          continue;
        w = a;
        // check legal word
        if (lexicon.find(word) == lexicon.end())
          continue;
        // add pair(parent->child)
        auto vec = m.find(word);
        if (vec == m.end())
          m.emplace(word, std::unordered_set{copiedWord});
        else
          vec->second.emplace(copiedWord);
        auto copiedPair = p;
        copiedPair.first = word;
        (copiedPair.second)++;
        visitedWords.emplace(word);
        if (word == desWord) {
          distance = currentDistance;
        } else {
          q.emplace(copiedPair);
        }
      }
      w = save;
    }
  }
  std::set<std::list<std::string>> wordLadders;
  std::list<std::string> l;
  l.emplace_back(desWord);
  GetPath(m, l, wordLadders, startWord, desWord);
  return wordLadders;
}

void GetPath(const std::unordered_map<std::string, std::unordered_set<std::string>>& m,
             std::list<std::string>& l,
             std::set<std::list<std::string>>& wordLadders,
             const std::string& startWord,
             const std::string& word) {
  if (word == startWord) {
    wordLadders.emplace(l);
    return;
  }
  const auto i = m.find(word);
  if (i != m.end()) {
    for (const auto& p : i->second) {
      l.emplace_front(p);
      GetPath(m, l, wordLadders, startWord, p);
      l.pop_front();
    }
  }
}

