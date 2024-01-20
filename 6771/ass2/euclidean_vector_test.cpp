/*
  In this assignment, we need to implement many functions for an EuclideanVector class, so the test
  should be based on those functions. First, I test all constructors. Because operator[] and
  GetNumDimensions() are the simplest part (just return value with no exception) in all functions
  and it allows us to check private attributes, I primarily use these two methods to test
  constructors. Second, I test member methods. Because some methods can be called by both const and
  non-const EuclideanVector, tests on member methods should test both const and non-const EV object.
  Then, I test member operations. Similarly, this functions are within the class, so some of them
  can be called by const EV object which we should pay attention to. Finally, I test the friend
  operator overloading. Because this methods are not within the class, we do not have to test on
  both const and non-const EV object, but we need to consider some border tests like 0-dimensional
  EV.
  The test strategy is that we assume operator[] and GetNumDimensions() are correct and start
  out tests. After finishing the constructor tests, things becomes easier because we can test things
  based on things we have tested. Although we should not assume one function is correct, I think if
  the program passes all these tests, it should be correct because one test is dependent on result
  of other tests.
*/

#include <algorithm>
#include <list>
#include <utility>
#include <vector>

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"

// Tests on constructors
SCENARIO("Test on the first constructor") {
  GIVEN("an default constructor EuclideanVector(int i = 1)") {
    WHEN("try to construct by default constructor with default value") {
      EuclideanVector ev{};
      THEN("construct successfully") {
        REQUIRE(ev == EuclideanVector{1});
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 0.0);
      }
    }
    WHEN("try to construct const object by default constructor with default value") {
      const EuclideanVector ev{};
      THEN("construct successfully") {
        REQUIRE(ev == EuclideanVector{1});
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 0.0);
      }
    }
    WHEN("try to construct EV object by default constructor with number 5") {
      EuclideanVector ev{5};
      THEN("construct successfully") {
        REQUIRE(ev.GetNumDimensions() == 5);
        REQUIRE(std::vector<double>{ev[0], ev[1], ev[2], ev[3], ev[4]} ==
                std::vector<double>{0, 0, 0, 0, 0});
      }
    }
    WHEN("try to construct const EV object by default constructor with number 4") {
      const EuclideanVector ev{4};
      THEN("construct successfully") {
        REQUIRE(ev.GetNumDimensions() == 4);
        REQUIRE(std::vector<double>{ev[0], ev[1], ev[2], ev[3]} == std::vector<double>{0, 0, 0, 0});
      }
    }
    WHEN("try to construct 0-dimensional EV object by default constructor") {
      const EuclideanVector ev{0};
      THEN("construct successfully") { REQUIRE(ev.GetNumDimensions() == 0); }
    }
  }
}

SCENARIO("Test on the second constructor") {
  GIVEN("an API EuclideanVector(int, double)") {
    WHEN("construct a EV object with 0 dimensional") {
      EuclideanVector ev{0, 1};
      THEN("construct an 0-dimensional EV") { REQUIRE(ev.GetNumDimensions() == 0); }
    }
    WHEN("construct a const EV object with 1 dimensional") {
      const EuclideanVector ev{1, 2};
      THEN("size = 1 and magnitude = [2.0]") {
        REQUIRE(ev.GetNumDimensions() == 1);
        REQUIRE(ev[0] == 2.0);
      }
    }
    WHEN("construct an 4-dimensional EV") {
      EuclideanVector ev{4, 2.0};
      THEN("size = 4 and magnitude = [2.0, 2.0, 2.0, 2.0]") {
        REQUIRE(ev.GetNumDimensions() == 4);
        REQUIRE(std::vector{ev[0], ev[1], ev[2], ev[3]} == std::vector{2.0, 2.0, 2.0, 2.0});
      }
    }
  }
}

SCENARIO("Test on the third constructor") {
  GIVEN("a constructor EuclideanVector(std::vector<double>::const_iterator, "
        "std::vector<double>::const_iterator)") {
    std::vector<double> v_1{};
    std::vector<double> v_2{1, 2, 3};
    WHEN("construct by empty vector") {
      EuclideanVector ev{v_1.begin(), v_1.end()};
      THEN("construct a 0-dimensional EV") { REQUIRE(ev.GetNumDimensions() == 0); }
    }
    WHEN("construct const EV by non-empty vector") {
      const EuclideanVector ev{v_2.begin(), v_2.end()};
      THEN("size = 3, magnitude = [1.0, 2.0, 3.0]") {
        REQUIRE(ev.GetNumDimensions() == 3);
        REQUIRE(std::vector{ev[0], ev[1], ev[2]} == std::vector{1.0, 2.0, 3.0});
      }
    }
  }
}

SCENARIO("Test on copy constructor") {
  GIVEN("copy constructor EuclideanVector(const EuclideanVector&)") {
    WHEN("use copy constructor to construct EV") {
      EuclideanVector ev{2};
      EuclideanVector copy{ev};
      THEN("all attributes are same") {
        REQUIRE(copy == ev);
        REQUIRE(copy.GetNumDimensions() == ev.GetNumDimensions());
        REQUIRE(std::vector<double>{copy[0], copy[1]} == std::vector<double>{ev[0], ev[1]});
      }
    }
    WHEN("use copy constructor to construct const EV") {
      EuclideanVector ev{3, 2.0};
      EuclideanVector copy = ev;
      THEN("all attributes are same") {
        REQUIRE(ev == copy);
        REQUIRE(copy.GetNumDimensions() == ev.GetNumDimensions());
        REQUIRE(std::vector<double>{copy[0], copy[1], copy[2]} ==
                std::vector<double>{ev[0], ev[1], ev[2]});
      }
    }
    WHEN("copy an const EV") {
      std::vector<double> v{2, 5, 3};
      const EuclideanVector ev{v.begin(), v.end()};
      const EuclideanVector copy{ev};
      THEN("all attributes are same") {
        REQUIRE(ev == copy);
        REQUIRE(copy.GetNumDimensions() == ev.GetNumDimensions());
        REQUIRE(std::vector<double>{copy[0], copy[1], copy[2]} ==
                std::vector<double>{ev[0], ev[1], ev[2]});
      }
    }
  }
}

SCENARIO("Test on move constructor") {
  GIVEN("move constructor EuclideanVector(EuclideanVector&&)") {
    WHEN("move EV{int}") {
      EuclideanVector tmp{3};
      EuclideanVector ev{std::move(tmp)};
      THEN("successfully construct and the original one destroyed") {
        REQUIRE(ev.GetNumDimensions() == 3);
        REQUIRE(tmp.GetNumDimensions() == 0);
        REQUIRE(std::vector<double>{ev[0], ev[1], ev[2]} == std::vector{0.0, 0.0, 0.0});
      }
    }
    WHEN("move EV{int, double}") {
      EuclideanVector tmp{2, 2.0};
      EuclideanVector ev = std::move(tmp);
      THEN("successfully construct and the original one not destroyed because of const") {
        REQUIRE(ev.GetNumDimensions() == 2);
        REQUIRE(tmp.GetNumDimensions() == 0);
        REQUIRE(std::vector<double>{ev[0], ev[1]} == std::vector{2.0, 2.0});
      }
    }
    WHEN("move EV{begin, end} to const EV") {
      std::vector<double> v{1.0, 5.0, 4.0, 3.0};
      EuclideanVector tmp{v.begin(), v.end()};
      const EuclideanVector ev{std::move(tmp)};
      THEN("successfully construct and the original one destroyed") {
        REQUIRE(ev.GetNumDimensions() == 4);
        REQUIRE(tmp.GetNumDimensions() == 0);
        REQUIRE(std::vector<double>{ev[0], ev[1], ev[2], ev[3]} == v);
      }
    }
  }
}

// Tests on methods
SCENARIO("Test on at(int)") {
  GIVEN("an EV object and a const EV object") {
    std::vector v{1.0, 3.0, 4.0, 3.0};
    EuclideanVector ev{v.begin(), v.end()};
    const EuclideanVector cev{ev};
    WHEN("get value by at") {
      THEN("results are the same as the given vector") {
        REQUIRE(ev.at(0) == v.at(0));
        REQUIRE(ev.at(1) == v.at(1));
        REQUIRE(ev.at(2) == v.at(2));
        REQUIRE(ev.at(2) == v.at(2));
        REQUIRE(cev.at(0) == v.at(0));
        REQUIRE(cev.at(1) == v.at(1));
        REQUIRE(cev.at(2) == v.at(2));
      }
    }
    WHEN("change value by at") {
      ev.at(0) = 3.0;
      ev.at(1) = 4.0;
      ev.at(2) = 5.0;
      ev.at(3) = 6.0;
      THEN("magnitude changed") {
        REQUIRE(std::vector<double>{ev[0], ev[1], ev[2], ev[3]} ==
                std::vector<double>{3.0, 4.0, 5.0, 6.0});
      }
    }
    WHEN("try index out of range on const EV") {
      THEN("caught string") {
        REQUIRE_THROWS_WITH(cev.at(6), "Index 6 is not valid for this EuclideanVector object");
      }
    }
    WHEN("try index out of range on non-const EV") {
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev.at(6), "Index 6 is not valid for this EuclideanVector object");
      }
    }
  }
}

SCENARIO("Test on int GetNumDimensions()") {
  GIVEN("some EV objects") {
    EuclideanVector ev{2, 2.0};
    const EuclideanVector cev{3, 3.0};
    WHEN("get size") {
      auto size = ev.GetNumDimensions();
      auto size_const = cev.GetNumDimensions();
      THEN("size should be 2 and 3") {
        REQUIRE(size == 2);
        REQUIRE(size_const == 3);
      }
    }
    WHEN("copy assignment") {
      ev = cev;
      auto size = ev.GetNumDimensions();
      THEN("size changes") { REQUIRE(size == 3); }
    }
  }
}

SCENARIO("Test on double GetEuclideanNorm()") {
  GIVEN("some EV objects") {
    EuclideanVector ev{2};
    ev[0] = 3.0;
    ev[1] = 4.0;
    const EuclideanVector cev{1, 2};
    WHEN("get norm") {
      auto norm = ev.GetEuclideanNorm();
      auto norm_const = cev.GetEuclideanNorm();
      THEN("compare the norm to our expectation") {
        REQUIRE(norm == 5.0);
        REQUIRE(norm_const == 2);
      }
    }
    WHEN("modyfy things in magnitude") {
      ev[0] = 4;
      ev[1] = 3;
      auto norm = ev.GetEuclideanNorm();
      THEN("norm changes") { REQUIRE(norm == 5); }
    }
    WHEN("copy assignment") {
      ev = cev;
      auto norm = ev.GetEuclideanNorm();
      THEN("norm changes") { REQUIRE(norm == 2); }
    }
    WHEN("call this function on 0-dimensional EV") {
      ev = EuclideanVector{0};
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev.GetEuclideanNorm(),
                            "EuclideanVector with no dimensions does not have a norm");
      }
    }
  }
}

SCENARIO("Test on EuclideanVector CreateUnitVector()") {
  GIVEN("some EV objects") {
    EuclideanVector ev{2, 1};
    ev[0] = 3.0;
    ev[1] = 4.0;
    const EuclideanVector cev{1, 3};
    WHEN("create unit EV") {
      auto unit_ev = ev.CreateUnitVector();
      THEN("unit_ev should be the unit vector of ev") {
        REQUIRE(unit_ev.GetNumDimensions() == 2);
        REQUIRE(std::vector<double>{unit_ev[0], unit_ev[1]} == std::vector<double>{0.6, 0.8});
      }
    }
    WHEN("create unit const EV") {
      const EuclideanVector unit_cev = cev.CreateUnitVector();
      THEN("unit_cev should be the unit vector of cev") {
        REQUIRE(unit_cev.GetNumDimensions() == 1);
        REQUIRE(unit_cev[0] == 1.0);
      }
    }
    WHEN("call on 0-dimensional EV") {
      ev = EuclideanVector{0};
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev.CreateUnitVector(),
                            "EuclideanVector with no dimensions does not have a unit vector");
      }
    }
    WHEN("call on 0-norm EV") {
      ev = EuclideanVector{2};
      THEN("catch exception")
      REQUIRE_THROWS_WITH(ev.CreateUnitVector(),
                          "EuclideanVector with euclidean normal of 0 does not have a unit vector");
    }
  }
}

// Tests on member operations
SCENARIO("Test on copy assignment") {
  GIVEN("EV object") {
    EuclideanVector ev{};
    WHEN("try to assign an EV{5}") {
      EuclideanVector tmp{5};
      tmp[1] = 2.0;
      tmp[2] = 3.0;
      ev = tmp;
      THEN("original EV object become a copy") {
        REQUIRE(ev.GetNumDimensions() == 5);
        REQUIRE(ev == tmp);
        REQUIRE(std::vector<double>{ev[0], ev[1], ev[2], ev[3], ev[4]} ==
                std::vector<double>{0, 2.0, 3.0, 0, 0});
      }
    }
    WHEN("try to copy an 0-dimensional const EV") {
      const EuclideanVector tmp{0};
      ev = tmp;
      THEN("original EV object become the same as tmp") {
        REQUIRE(ev == tmp);
        REQUIRE(ev.GetNumDimensions() == 0);
      }
    }
  }
}

SCENARIO("Test on move assignment") {
  GIVEN("an EV object") {
    EuclideanVector tmp{0};
    EuclideanVector tmp_2{3};
    tmp_2.at(0) = 3.0;
    tmp_2.at(1) = 4.0;
    tmp_2.at(2) = 5.0;
    WHEN("try move assignment") {
      EuclideanVector ev{6};
      ev = std::move(tmp);
      THEN("move successfully and original one reset") {
        REQUIRE(ev.GetNumDimensions() == 0);
        REQUIRE(tmp.GetNumDimensions() == 0);
      }
    }
    WHEN("try move assignment on 3-dimensional EV") {
      EuclideanVector ev{0};
      ev = std::move(tmp_2);
      THEN("move successfully and original one reset") {
        REQUIRE(ev.GetNumDimensions() == 3);
        REQUIRE(tmp.GetNumDimensions() == 0);
        REQUIRE(std::vector<double>{ev[0], ev[1], ev[2]} == std::vector<double>{3.0, 4.0, 5.0});
      }
    }
  }
}

SCENARIO("Test on subscript operator[]") {
  GIVEN("an const and non-const EV constructed by vector") {
    std::vector<double> v{1, 2, 3};
    const EuclideanVector cev{v.begin(), v.end()};
    EuclideanVector ev{v.begin(), v.end()};
    WHEN("get value by [] on non-const EV") {
      THEN("equal with vector value")
      REQUIRE(ev[0] == 1.0);
      REQUIRE(ev[1] == 2.0);
      REQUIRE(ev[2] == 3.0);
    }
    WHEN("get value by [] on const EV") {
      THEN("equal with vector value") {
        REQUIRE(cev[0] == 1.0);
        REQUIRE(cev[1] == 2.0);
        REQUIRE(cev[2] == 3.0);
      }
    }
    WHEN("change value by []") {
      ev[0] = 2.0;
      ev[1] = 1.0;
      ev[2] = 3.0;
      THEN("value inside changed") {
        REQUIRE(std::vector<double>{ev} == std::vector{2.0, 1.0, 3.0});
        REQUIRE(ev[0] == 2.0);
        REQUIRE(ev[1] == 1.0);
        REQUIRE(ev[2] == 3.0);
      }
    }
  }
}

SCENARIO("Test on operator +=") {
  GIVEN("an EV object [1, 2, 3]") {
    EuclideanVector ev{3};
    ev[0] = 1;
    ev[1] = 2;
    ev[2] = 3;
    WHEN("call +=") {
      std::vector<double> v{2, 1, 0};
      ev += EuclideanVector{v.begin(), v.end()};
      THEN("ev changed") {
        REQUIRE(ev[0] == 3);
        REQUIRE(ev[1] == 3);
        REQUIRE(ev[2] == 3);
      }
    }
    WHEN("try += on different dimensional EV") {
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev += EuclideanVector{2},
                            "Dimensions of LHS(3) and RHS(2) do not match");
      }
    }
    WHEN("add two 0-dimensional EV") {
      EuclideanVector ev_1{0};
      EuclideanVector ev_2{0};
      ev_1 += ev_2;
      THEN("ev_1 is still 0-dimensional EV") { REQUIRE(ev_1.GetNumDimensions() == 0); }
    }
  }
}

SCENARIO("Test on operator -=") {
  GIVEN("an EV object [1, 2, 3]") {
    EuclideanVector ev{3};
    ev[0] = 1;
    ev[1] = 2;
    ev[2] = 3;
    WHEN("call -=") {
      std::vector<double> v{2, 1, 0};
      ev -= EuclideanVector{v.begin(), v.end()};
      THEN("ev changed") {
        REQUIRE(ev[0] == -1.0);
        REQUIRE(ev[1] == 1.0);
        REQUIRE(ev[2] == 3.0);
      }
    }
    WHEN("try -= on different dimensional EV") {
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev += EuclideanVector{2},
                            "Dimensions of LHS(3) and RHS(2) do not match");
      }
    }
    WHEN("minus two 0-dimensional EV") {
      EuclideanVector ev_1{0};
      EuclideanVector ev_2{0};
      ev_1 -= ev_2;
      THEN("ev_1 is still 0-dimensional EV") { REQUIRE(ev_1.GetNumDimensions() == 0); }
    }
  }
}

SCENARIO("Test on operator *=") {
  GIVEN("an EV object [1.0, 2.0, 3.0]") {
    EuclideanVector ev{3};
    ev[0] = 1.0;
    ev[1] = 2.0;
    ev[2] = 3.0;
    WHEN("ev *= 3") {
      ev *= 3;
      THEN("magnitude in ev changed") {
        REQUIRE(ev[0] == 3.0);
        REQUIRE(ev[1] == 6.0);
        REQUIRE(ev[2] == 9.0);
      }
    }
    WHEN("ev *= -3") {
      ev *= -3;
      THEN("magnitude in ev times -3") {
        REQUIRE(ev[0] == -3.0);
        REQUIRE(ev[1] == -6.0);
        REQUIRE(ev[2] == -9.0);
      }
    }
  }
}

SCENARIO("Test on operator /=") {
  GIVEN("an EV object [3.0, 6.0, 9.0]") {
    EuclideanVector ev{3};
    ev[0] = 3.0;
    ev[1] = 6.0;
    ev[2] = 9.0;
    WHEN("ev /= 3") {
      ev /= 3;
      THEN("magnitude in ev are divided by 3") {
        REQUIRE(ev[0] == 1.0);
        REQUIRE(ev[1] == 2.0);
        REQUIRE(ev[2] == 3.0);
      }
    }
    WHEN("ev /= -3") {
      ev /= -3;
      THEN("magnitude in ev are divided by -3") {
        REQUIRE(ev[0] == -1.0);
        REQUIRE(ev[1] == -2.0);
        REQUIRE(ev[2] == -3.0);
      }
    }
    WHEN("try ev /= 0") {
      THEN("catch exception") { REQUIRE_THROWS_WITH(ev /= 0, "Invalid vector division by 0"); }
    }
  }
}

SCENARIO("Test on vector conversion") {
  GIVEN("EV and const EV") {
    EuclideanVector ev_1{0};
    EuclideanVector ev_2{3};
    ev_2[0] = 2.0;
    ev_2[1] = 3.0;
    ev_2[2] = 4.0;
    const EuclideanVector cev{2, 2.0};
    WHEN("convert ev_1 (0-dimensional)") {
      std::vector<double> v{ev_1};
      THEN("v becomes {}") { REQUIRE(v == std::vector<double>{}); }
    }
    WHEN("convert ev_2") {
      std::vector<double> v{ev_2};
      THEN("v = {2.0, 3.0, 4.0}") { REQUIRE(v == std::vector<double>{2.0, 3.0, 4.0}); }
    }
    WHEN("convert const cev") {
      auto v = static_cast<std::vector<double>>(cev);
      THEN("v = {2.0, 2.0}") { REQUIRE(v == std::vector<double>{2.0, 2.0}); }
    }
  }
}

SCENARIO("Test on list conversion") {
  GIVEN("EV and const EV") {
    EuclideanVector ev_1{0};
    EuclideanVector ev_2{3};
    ev_2[0] = 2.0;
    ev_2[1] = 3.0;
    ev_2[2] = 4.0;
    const EuclideanVector cev{2, 2.0};
    WHEN("convert ev_1 (0-dimensional)") {
      std::list<double> l{ev_1};
      THEN("l becomes {}") { REQUIRE(l == std::list<double>{}); }
    }
    WHEN("convert ev_2") {
      std::list<double> l{ev_2};
      THEN("v = {2.0, 3.0, 4.0}") { REQUIRE(l == std::list<double>{2.0, 3.0, 4.0}); }
    }
    WHEN("convert const cev") {
      auto l = static_cast<std::list<double>>(cev);
      THEN("l = {2.0, 2.0}") { REQUIRE(l == std::list<double>{2.0, 2.0}); }
    }
  }
}

// Test on friend functions
SCENARIO("Test on operator ==") {
  GIVEN("EV and const EV") {
    EuclideanVector ev{2};
    const EuclideanVector cev{2};
    WHEN("compare two EV") {
      auto equal = (ev == cev);
      THEN("they should be equal") { REQUIRE(equal == true); }
    }
    WHEN("copy ev by copy constructor") {
      auto copy = EuclideanVector{ev};
      auto equal = (copy == ev);
      THEN("equal should be true") { REQUIRE(equal == true); }
    }
  }
}

SCENARIO("Test on operator !=") {
  GIVEN("EV and const EV") {
    EuclideanVector ev{2};
    const EuclideanVector cev{3};
    WHEN("compare two EV") {
      auto equal = (ev == cev);
      THEN("they should not be equal") { REQUIRE(equal == false); }
    }
    WHEN("copy ev by copy constructor and modify copied one") {
      auto copy = EuclideanVector{ev};
      copy[0] = 2.0;
      auto equal = (copy == ev);
      THEN("equal should be true") { REQUIRE(equal == false); }
    }
  }
}

SCENARIO("Test on operator +") {
  GIVEN("two EV object") {
    EuclideanVector ev_1{2};
    EuclideanVector ev_2{2};
    ev_1[0] = 1.0;
    ev_1[1] = 2.0;
    ev_2[0] = 2.0;
    ev_2[1] = 1.0;
    WHEN("add two EV") {
      auto ev_3 = ev_1 + ev_2;
      THEN("ev_3 should be [3.0, 3.0]") {
        REQUIRE(ev_3.GetNumDimensions() == 2);
        REQUIRE(ev_3[0] == 3.0);
        REQUIRE(ev_3[1] == 3.0);
      }
    }
    WHEN("add two 0-dimensional EV") {
      auto ev = EuclideanVector{0} + EuclideanVector{0};
      THEN("ev should be 0-dimensional") { REQUIRE(ev.GetNumDimensions() == 0); }
    }
    WHEN("try to add 2-dimensional and 0-dimensional EV") {
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev_1 + EuclideanVector{0},
                            "Dimensions of LHS(2) and RHS(0) do not match");
      }
    }
  }
}

SCENARIO("Test on operator -") {
  GIVEN("two EV object") {
    EuclideanVector ev_1{2};
    EuclideanVector ev_2{2};
    ev_1[0] = 1.0;
    ev_1[1] = 2.0;
    ev_2[0] = 2.0;
    ev_2[1] = 1.0;
    WHEN("subtract two EV") {
      auto ev_3 = ev_1 - ev_2;
      THEN("ev_3 should be [-1.0, 1.0]") {
        REQUIRE(ev_3.GetNumDimensions() == 2);
        REQUIRE(ev_3[0] == -1.0);
        REQUIRE(ev_3[1] == 1.0);
      }
    }
    WHEN("add two 0-dimensional EV") {
      auto ev = EuclideanVector{0} - EuclideanVector{0};
      THEN("ev should be 0-dimensional") { REQUIRE(ev.GetNumDimensions() == 0); }
    }
    WHEN("try to add 2-dimensional and 0-dimensional EV") {
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev_1 - EuclideanVector{0},
                            "Dimensions of LHS(2) and RHS(0) do not match");
      }
    }
  }
}

SCENARIO("Test on operator * (dot product)") {
  GIVEN("two EV object") {
    EuclideanVector ev_1{2};
    EuclideanVector ev_2{2};
    ev_1[0] = 1.0;
    ev_1[1] = 2.0;
    ev_2[0] = 2.0;
    ev_2[1] = 1.0;
    WHEN("multiply two EV") {
      auto product = ev_1 * ev_2;
      THEN("ev_3 should be 4.0") { REQUIRE(product == 4.0); }
    }
    WHEN("try to multiply 2-dimensional and 1-dimensional EV") {
      THEN("catch exception") {
        REQUIRE_THROWS_WITH(ev_1 * EuclideanVector{1},
                            "Dimensions of LHS(2) and RHS(1) do not match");
      }
    }
  }
}

SCENARIO("Test on operator *(EV * double or double * EV)") {
  GIVEN("EV object [1.0, 2.0, 3.0]") {
    EuclideanVector ev{3};
    ev[0] = 1.0;
    ev[1] = 2.0;
    ev[2] = 3.0;
    WHEN("try EV * double") {
      EuclideanVector ev_2 = ev * 3.0;
      THEN("check result becomes 3 times of the original") {
        REQUIRE(ev_2.GetNumDimensions() == 3);
        REQUIRE(ev_2[0] == 3.0);
        REQUIRE(ev_2[1] == 6.0);
        REQUIRE(ev_2[2] == 9.0);
      }
    }
    WHEN("try double * EV") {
      EuclideanVector ev_2 = 3.0 * ev;
      THEN("check result becomes 3 times of the original") {
        REQUIRE(ev_2.GetNumDimensions() == 3);
        REQUIRE(ev_2[0] == 3.0);
        REQUIRE(ev_2[1] == 6.0);
        REQUIRE(ev_2[2] == 9.0);
      }
    }
  }
}

SCENARIO("Test on operator / (EV / double)") {
  GIVEN("EV [3.0, 6.0, 9.0]") {
    EuclideanVector ev{3};
    ev[0] = 3.0;
    ev[1] = 6.0;
    ev[2] = 9.0;
    WHEN("divide by 3") {
      EuclideanVector ev_2 = ev / 3.0;
      THEN("ev_2 = [1.0, 2.0, 3.0]") {
        REQUIRE(ev_2.GetNumDimensions() == 3);
        REQUIRE(ev_2[0] == 1.0);
        REQUIRE(ev_2[1] == 2.0);
        REQUIRE(ev_2[2] == 3.0);
      }
    }
    WHEN("divide by 0") {
      THEN("catch exception") { REQUIRE_THROWS_WITH(ev / 0, "Invalid vector division by 0"); }
    }
  }
}

SCENARIO("Test on output stream") {
  GIVEN("some EV objects") {
    EuclideanVector ev_1{0};
    EuclideanVector ev_2{2};
    ev_2[0] = 1.0;
    ev_2[1] = 2.0;
    const EuclideanVector ev_3{3, 3.3};
    WHEN("print ev_1") {
      std::stringstream ss;
      ss << ev_1;
      THEN("we want result be []") { REQUIRE(ss.str() == "[]"); }
    }
    WHEN("print ev_2") {
      std::stringstream ss;
      ss << ev_2;
      THEN("we want result be [1 2]") { REQUIRE(ss.str() == "[1 2]"); }
    }
    WHEN("print ev_3") {
      std::stringstream ss;
      ss << ev_3;
      THEN("we want the result be [3.3 3.3 3.3]") { REQUIRE(ss.str() == "[3.3 3.3 3.3]"); }
    }
  }
}
