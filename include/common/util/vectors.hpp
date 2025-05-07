#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <unordered_map>

#include "common/geometry/point.hpp"

template <typename T>
class Vector {
   public:
    int dim;

    Vector(int d) : dim(d) {}
};

template <typename T>
class Vector3 : public Vector<T> {
   public:
    T first, second, third;

    Vector3() : Vector<T>(3), first(0), second(0), third(0) {};

    Vector3(T first, T second, T third) : Vector<T>(3) {
        this->first = first;
        this->second = second;
        this->third = third;
    };

    Vector3<T> copy() {
        return Vector3(this->first, this->second, this->third);
    }

    Vector3<T> operator+(const Vector3<T>& other) {
        return Vector3(
            this->first + other.first,
            this->second + other.second,
            this->third + other.third);
    }

    Vector3<T> operator+=(const Vector3<T>& other) {
        this->first += other.first;
        this->second += other.second;
        this->third += other.third;

        return *this;
    }

    Vector3<T> operator*(T scalar) const {
        return Vector3(
            this->first * scalar,
            this->second * scalar,
            this->third * scalar);
    }

    T dot(const Vector3<T>& other) const {
        return this->first * other.first +
               this->second * other.second +
               this->third * other.third;
    }

    Vector3<T> cross(const Vector3<T>& other) const {
        return Vector3<T>(
            this->second * other.third - this->third * other.second,
            this->third * other.first - this->first * other.third,
            this->first * other.second - this->second * other.first);
    }

    float length() const {
        return std::sqrt(first * first + second * second + third * third);
    }

    void normalize() {
        float len = length();
        if (len != 0) {
            first /= len;
            second /= len;
            third /= len;
        }
    }

    Vector3<T> normalized() const {
        float len = length();
        if (len == 0) return *this;  // return original if zero-length
        return Vector3<T>(first / len, second / len, third / len);
    }

    friend Vector3<T> operator*(T scalar, const Vector3<T>& vec) {
        return vec * scalar;
    }

    Point3D toPoint3D() const {
        return Point3D(static_cast<float>(first),
                       static_cast<float>(second),
                       static_cast<float>(third));
    }
};

template <typename T>
class Vector4 : public Vector3<T> {
   public:
    T fourth;

    Vector4(T first, T second, T third, T fourth) : Vector3<T>(first, second, third) {
        this->fourth = fourth;
    };

    Vector4<T> copy() {
        return Vector4(this->first, this->second, this->third, this->fourth);
    }

    Vector4<T> operator+(const Vector4<T>& other) {
        return Vector4(
            this->first + other.first,
            this->second + other.second,
            this->third + other.third,
            this->fourth + other.fourth);
    }

    Vector4<T> operator+=(const Vector4<T>& other) {
        this->first += other.first;
        this->second += other.second;
        this->third += other.third;
        this->fourth += other.fourth;
    }
};

// Struct for combined position + normal key
struct VertexKey {
    Point3D position;
    Vector3<float> normal;

    bool operator==(const VertexKey& other) const {
        return position == other.position &&
               normal.first == other.normal.first &&
               normal.second == other.normal.second &&
               normal.third == other.normal.third;
    }
};

// Custom hash function for VertexKey
namespace std {
template <>
struct hash<VertexKey> {
    std::size_t operator()(const VertexKey& key) const {
        size_t h1 = hash<float>()(key.position.getX());
        size_t h2 = hash<float>()(key.position.getY());
        size_t h3 = hash<float>()(key.position.getZ());
        size_t h4 = hash<float>()(key.normal.first);
        size_t h5 = hash<float>()(key.normal.second);
        size_t h6 = hash<float>()(key.normal.third);
        return ((((((h1 ^ (h2 << 1)) ^ (h3 << 1)) ^ (h4 << 1)) ^ (h5 << 1)) ^ (h6 << 1)));
    }
};
}  // namespace std