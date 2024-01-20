#ifndef ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
#define ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_

#include <exception>
#include <list>
#include <memory>
#include <string>
#include <vector>

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept { return what_.c_str(); }

 private:
  std::string what_;
};

class EuclideanVector {
 public:
  // constructors
  explicit EuclideanVector(int i = 1) noexcept;
  EuclideanVector(int, double) noexcept;
  EuclideanVector(std::vector<double>::const_iterator,
                  std::vector<double>::const_iterator) noexcept;
  EuclideanVector(const EuclideanVector&) noexcept;
  EuclideanVector(EuclideanVector&&) noexcept;

  // destructors
  ~EuclideanVector() noexcept = default;

  // operations
  EuclideanVector& operator=(const EuclideanVector&) noexcept;
  EuclideanVector& operator=(EuclideanVector&&) noexcept;
  double operator[](int i) const noexcept;
  double& operator[](int i) noexcept;
  EuclideanVector& operator+=(const EuclideanVector&);
  EuclideanVector& operator-=(const EuclideanVector&);
  EuclideanVector& operator*=(double n) noexcept;
  EuclideanVector& operator/=(double);
  explicit operator std::vector<double>() const noexcept;
  explicit operator std::list<double>() const noexcept;

  // member function
  double at(int) const;
  double& at(int);
  int GetNumDimensions() const noexcept { return size_; }
  double GetEuclideanNorm() const;
  EuclideanVector CreateUnitVector() const;

  // friends
  friend bool operator==(const EuclideanVector&, const EuclideanVector&) noexcept;
  friend bool operator!=(const EuclideanVector&, const EuclideanVector&) noexcept;
  friend EuclideanVector operator+(const EuclideanVector&, const EuclideanVector&);
  friend EuclideanVector operator-(const EuclideanVector&, const EuclideanVector&);
  friend double operator*(const EuclideanVector&, const EuclideanVector&);
  friend EuclideanVector operator*(const EuclideanVector&, double)noexcept;
  friend EuclideanVector operator*(double, const EuclideanVector&)noexcept;
  friend EuclideanVector operator/(const EuclideanVector&, double);
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& ev) noexcept;

 private:
  int size_;
  std::unique_ptr<double[]> magnitudes_;
};
#endif  // ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
