/*
  To test my work, I create some small lexicon which I can get the word ladder path by hand so that
  I can compare the return vector of the function with the expected vector. I test one-hop, two-hop,
  three-hop word ladders because it is easy to see if the result is correct and this can test if the
  bidirectional BFS can find the result when the solution is short. Although you will not assess
  some impossible word ladders, I test them like non-exist destination_word and different-length
  start_word and destination_word. I also create a lexicon to test the situation that there is no
  solution from start_word to destination_word. Things above are about the simplest situation, so I
  then do some more complex tests. I test if the function can find a longer word ladder and if it
  can solve multiple path problem. Finally, I test if the function can find a special situation
  where a word can appear in more than one path of word ladder(like aa->ab->cb, ac->ab->cb, "ab"
  appears twice). I think these are enough to test this function because I cover most possible
  situations during the test and these tests can find if there is a bug in the function.
*/
#include <string>
#include <unordered_set>
#include <vector>

#include "assignments/wl/word_ladder.h"
#include "catch.h"

SCENARIO("Simplest word: Words with one-character words") {
  GIVEN("An unordered set with some single-character words") {
    std::unordered_set<std::string> lexicon{"a", "b"};

    WHEN("Find one-hop word ladder") {
      auto t = GetWordLadder("a", "b", lexicon);
      THEN("There is a result") {
        REQUIRE(t.size() > 0);
        REQUIRE(t == std::vector<std::vector<std::string>>{{"a", "b"}});
      }
    }
    WHEN("Find a word not in the lexicon") {
      auto t = GetWordLadder("a", "e", lexicon);
      THEN("There is no result") { REQUIRE(t.size() == 0); }
    }
  }
}

SCENARIO("Words with only one-character and two-character words") {
  GIVEN("An unordered set with some words") {
    std::unordered_set<std::string> lexicon{"a", "ab", "ac", "bc", "bz", "dd"};
    WHEN("Find one-hop word ladder") {
      auto t = GetWordLadder("ab", "ac", lexicon);
      THEN("There is a result") {
        REQUIRE(t.size() == 1);
        REQUIRE(t == std::vector<std::vector<std::string>>{{"ab", "ac"}});
      }
    }
    WHEN("Find two-hop word ladder") {
      auto t = GetWordLadder("ab", "bc", lexicon);
      THEN("There is a possible result") {
        REQUIRE(t.size() == 1);
        REQUIRE(t == std::vector<std::vector<std::string>>{{"ab", "ac", "bc"}});
      }
    }
    WHEN("Find three-hop word ladder") {
      auto t = GetWordLadder("ab", "bz", lexicon);
      THEN("There is a possible solution") {
        REQUIRE(t.size() == 1);
        REQUIRE(t == std::vector<std::vector<std::string>>{{"ab", "ac", "bc", "bz"}});
      }
    }
    WHEN("Find word ladder when start_word and destination_word have different length") {
      auto t = GetWordLadder("a", "ab", lexicon);
      THEN("There is no result") { REQUIRE(t.size() == 0); }
    }
    WHEN("Try to find a non-exist solution") {
      auto t = GetWordLadder("ab", "dd", lexicon);
      THEN("There is no result") { REQUIRE(t.size() == 0); }
    }
  }
}

SCENARIO("Path may have longer answers") {
  GIVEN("An unordered set with some words") {
    std::unordered_set<std::string> lexicon{"aa", "ab", "bb", "bc", "cc", "cd", "dd", "de",
                                            "ee", "ef", "ff", "fg", "gg", "gh", "hh", "hi",
                                            "ii", "ij", "jj", "jk", "kk", "kl", "ll"};
    WHEN("Find word ladder from aa to ll") {
      auto t = GetWordLadder("aa", "ll", lexicon);
      THEN("There is a 22-hop solutions") {
        REQUIRE(t.size() == 1);
        REQUIRE(t == std::vector<std::vector<std::string>>{
                         {"aa", "ab", "bb", "bc", "cc", "cd", "dd", "de", "ee", "ef", "ff", "fg",
                          "gg", "gh", "hh", "hi", "ii", "ij", "jj", "jk", "kk", "kl", "ll"}});
      }
    }
  }
}

SCENARIO("Path may have multiple answers") {
  GIVEN("An unordered set with some words") {
    std::unordered_set<std::string> lexicon{"hit", "hot", "dot", "dog", "lot", "log", "cog"};
    WHEN("Find word ladder from hit to cog") {
      auto t = GetWordLadder("hit", "cog", lexicon);
      THEN("There are two 4-hop solutions") {
        REQUIRE(t.size() == 2);
        REQUIRE(t == std::vector<std::vector<std::string>>{{"hit", "hot", "dot", "dog", "cog"},
                                                           {"hit", "hot", "lot", "log", "cog"}});
      }
    }
  }
}

SCENARIO("A child may be generate by more than one parent more than once") {
  GIVEN("An unordered set with some words") {
    std::unordered_set<std::string> lexicon{"red", "rex", "ted", "tex", "tad", "tax"};
    WHEN("Find word ladder from red to tax") {
      auto t = GetWordLadder("red", "tax", lexicon);
      THEN("There are three solutions, word tex appears in the result twice") {
        REQUIRE(t.size() == 3);
        REQUIRE(t == std::vector<std::vector<std::string>>{{"red", "rex", "tex", "tax"},
                                                           {"red", "ted", "tad", "tax"},
                                                           {"red", "ted", "tex", "tax"}});
      }
    }
  }
}
