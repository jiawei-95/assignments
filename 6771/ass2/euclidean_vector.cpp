#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <cassert>
#include <cmath>
#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

// construct i-dimensional EV with all magnitude 0.0
EuclideanVector::EuclideanVector(int i) noexcept
  : size_{i}, magnitudes_{std::make_unique<double[]>(size_)} {
  for (int j = 0; j < size_; ++j) {
    magnitudes_[j] = 0.0;
  }
}

// construct i-dimensional EV with all magnitude = magnitude
EuclideanVector::EuclideanVector(int i, double magnitude) noexcept
  : size_{i}, magnitudes_{std::make_unique<double[]>(size_)} {
  for (int j = 0; j < size_; ++j) {
    magnitudes_[j] = magnitude;
  }
}

// construct an EV from the given vector
EuclideanVector::EuclideanVector(std::vector<double>::const_iterator begin,
                                 std::vector<double>::const_iterator end) noexcept
  : size_{static_cast<int>(std::distance(begin, end))}, magnitudes_{
                                                            std::make_unique<double[]>(size_)} {
  for (int i = 0; i < size_; ++i, ++begin) {
    magnitudes_[i] = *begin;
  }
}

// copy constructor
EuclideanVector::EuclideanVector(const EuclideanVector& copy) noexcept
  : size_{copy.size_}, magnitudes_{std::make_unique<double[]>(size_)} {
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] = copy.magnitudes_[i];
  }
}

// move constructor
EuclideanVector::EuclideanVector(EuclideanVector&& tmp) noexcept
  : size_{tmp.size_}, magnitudes_{std::move(tmp.magnitudes_)} {
  tmp.size_ = 0;
}

// copy assignment
EuclideanVector& EuclideanVector::operator=(const EuclideanVector& copy) noexcept {
  EuclideanVector tmp{copy};
  std::swap(tmp, *this);
  return *this;
}

// move assignment
EuclideanVector& EuclideanVector::operator=(EuclideanVector&& tmp) noexcept {
  size_ = tmp.size_;
  // reset tmp
  tmp.size_ = 0;
  magnitudes_ = std::move(tmp.magnitudes_);
  return *this;
}

// return the ith magnitude. marked const for both const and non-const EV.
double EuclideanVector::operator[](int i) const noexcept {
  assert(i >= 0 && i < size_);
  return magnitudes_[i];
}

// return the ith magnitude reference for modifying ith magnitude in EV.
double& EuclideanVector::operator[](int i) noexcept {
  assert(i >= 0 && i < size_);
  return magnitudes_[i];
}

// operator+= on EV with the same dimension.
EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& right) {
  auto x = this->GetNumDimensions();
  auto y = right.GetNumDimensions();
  if (x != y) {
    std::string message = "Dimensions of LHS(" + std::to_string(x) + ") and RHS(" +
                          std::to_string(y) + ") do not match";
    throw EuclideanVectorError(message);
  }
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] += right.magnitudes_[i];
  }
  return *this;
}

// operator-= on EV with the same dimension.
EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& right) {
  auto x = this->GetNumDimensions();
  auto y = right.GetNumDimensions();
  if (x != y) {
    std::string message = "Dimensions of LHS(" + std::to_string(x) + ") and RHS(" +
                          std::to_string(y) + ") do not match";
    throw EuclideanVectorError(message);
  }
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] -= right.magnitudes_[i];
  }
  return *this;
}

// operator*= double for multiplying magnitudes
EuclideanVector& EuclideanVector::operator*=(double n) noexcept {
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] *= n;
  }
  return *this;
}

// operator/= double for dividing magnitudes
EuclideanVector& EuclideanVector::operator/=(double n) {
  if (n == 0) {
    throw EuclideanVectorError("Invalid vector division by 0");
  }
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] /= n;
  }
  return *this;
}

// convert to vector for static_cast<std::vector<double>>(ev) or std::vector<double>{ev}
// marked const for both const and non-const EV
EuclideanVector::operator std::vector<double>() const noexcept {
  std::vector<double> vec{};
  for (int i = 0; i < size_; ++i) {
    vec.push_back(magnitudes_[i]);
  }
  return vec;
}

// convert to list for static_cast<std::list<double>>(ev) or std::list<double>{ev}
// marked const for both const and non-const EV
EuclideanVector::operator std::list<double>() const noexcept {
  std::list<double> l{};
  for (int i = 0; i < size_; ++i) {
    l.push_back(magnitudes_[i]);
  }
  return l;
}

// return the ith magnitude, marked const for both const and non-const EV
double EuclideanVector::at(int i) const {
  if (i < 0 || i >= size_) {
    throw EuclideanVectorError("Index " + std::to_string(i) +
                               " is not valid for this EuclideanVector object");
  }
  return magnitudes_[i];
}

// return the ith magnitude reference for modifying the ith magnitude
double& EuclideanVector::at(int i) {
  if (i < 0 || i >= size_) {
    throw EuclideanVectorError("Index " + std::to_string(i) +
                               " is not valid for this EuclideanVector object");
  }
  return magnitudes_[i];
}

// return the vector norm, marked const for both const and non-const EV
double EuclideanVector::GetEuclideanNorm() const {
  if (this->GetNumDimensions() == 0) {
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a norm");
  }
  double norm = 0.0;
  for (int i = 0; i < size_; ++i) {
    norm += magnitudes_[i] * magnitudes_[i];
  }
  return sqrt(norm);
}

// return the unit vector of EV, marked const for both const and non-const EV
EuclideanVector EuclideanVector::CreateUnitVector() const {
  if (this->GetNumDimensions() == 0) {
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a unit vector");
  }
  if (this->GetEuclideanNorm() == 0) {
    throw EuclideanVectorError(
        "EuclideanVector with euclidean normal of 0 does not have a unit vector");
  }
  EuclideanVector ev{*this};
  double norm = ev.GetEuclideanNorm();
  for (int i = 0; i < ev.size_; ++i) {
    ev.magnitudes_[i] /= norm;
  }
  return ev;
}

// return true if two EV are both equal in size and magnitudes
bool operator==(const EuclideanVector& left, const EuclideanVector& right) noexcept {
  if (left.size_ != right.size_) {
    return false;
  }
  for (int i = 0; i < left.size_; ++i) {
    if (left.magnitudes_[i] != right.magnitudes_[i]) {
      return false;
    }
  }
  return true;
}

// return true if two EV are not equal in size or magnitudes
bool operator!=(const EuclideanVector& left, const EuclideanVector& right) noexcept {
  return !(left == right);
}

// add two EV
EuclideanVector operator+(const EuclideanVector& left, const EuclideanVector& right) {
  auto x = left.GetNumDimensions();
  auto y = right.GetNumDimensions();
  if (x != y) {
    std::string message = "Dimensions of LHS(" + std::to_string(x) + ") and RHS(" +
                          std::to_string(y) + ") do not match";
    throw EuclideanVectorError(message);
  }
  EuclideanVector ev{left};
  for (int i = 0; i < ev.GetNumDimensions(); ++i) {
    ev.magnitudes_[i] += right.magnitudes_[i];
  }
  return ev;
}

// subtract two EV
EuclideanVector operator-(const EuclideanVector& left, const EuclideanVector& right) {
  auto x = left.GetNumDimensions();
  auto y = right.GetNumDimensions();
  if (x != y) {
    std::string message = "Dimensions of LHS(" + std::to_string(x) + ") and RHS(" +
                          std::to_string(y) + ") do not match";
    throw EuclideanVectorError(message);
  }
  EuclideanVector ev{left};
  for (int i = 0; i < ev.GetNumDimensions(); ++i) {
    ev.magnitudes_[i] -= right.magnitudes_[i];
  }
  return ev;
}

// multiply two EV
double operator*(const EuclideanVector& left, const EuclideanVector& right) {
  auto x = left.GetNumDimensions();
  auto y = right.GetNumDimensions();
  if (x != y) {
    std::string message = "Dimensions of LHS(" + std::to_string(x) + ") and RHS(" +
                          std::to_string(y) + ") do not match";
    throw EuclideanVectorError(message);
  }
  double product = 0.0;
  for (int i = 0; i < left.size_; ++i) {
    product += (left.magnitudes_[i] * right.magnitudes_[i]);
  }
  return product;
}

// EV * double, multiply magnitudes in EV
EuclideanVector operator*(const EuclideanVector& left, double right) noexcept {
  EuclideanVector ev{left};
  for (int i = 0; i < ev.size_; ++i) {
    ev.magnitudes_[i] *= right;
  }
  return ev;
}

// double * EV, multiply magnitudes in EV
EuclideanVector operator*(double left, const EuclideanVector& right) noexcept {
  EuclideanVector ev{right};
  for (int i = 0; i < ev.size_; ++i) {
    ev.magnitudes_[i] *= left;
  }
  return ev;
}

// EV / double, divide magnitudes in EV
EuclideanVector operator/(const EuclideanVector& left, double right) {
  if (right == 0) {
    throw EuclideanVectorError("Invalid vector division by 0");
  }
  EuclideanVector ev{left};
  for (int i = 0; i < ev.size_; ++i) {
    ev.magnitudes_[i] /= right;
  }
  return ev;
}

// operator<< overloading, print EV in [1 2 3] format.
std::ostream& operator<<(std::ostream& os, const EuclideanVector& ev) noexcept {
  os << "[";
  for (int i = 0; i < ev.size_; ++i) {
    os << ev.magnitudes_[i];
    if (i != ev.size_ - 1) {
      os << " ";
    }
  }
  os << "]";
  return os;
}
