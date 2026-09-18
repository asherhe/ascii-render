#pragma once

#include <cmath>

struct Vec3 {
 public:
  double x, y, z;

  Vec3() : x(0), y(0), z(0) {}
  Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}
  Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

  Vec3 operator+(const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
  }
  Vec3 operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
  }

  Vec3 operator-() const { return Vec3(-x, -y, -z); }

  Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
  friend Vec3 operator*(double s, const Vec3& v) { return v * s; }

  Vec3 operator/(double s) const { return Vec3(x / s, y / s, z / s); }

  double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

  Vec3 cross(const Vec3& v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }

  double norm() const { return sqrt(x * x + y * y + z * z); }
  Vec3 normalized() const { return *this / norm(); }

  // component-wise multiplication
  Vec3 hadamard(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
};
