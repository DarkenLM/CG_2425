#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include <cmath>
#include <unordered_map>

#include "common/geometry/point.hpp"

template <typename T>
class Vector {
   public:
    int dim;
    Vector(int d);
};

template <typename T>
class Vector2 : public Vector<T> {
   public:
    T first, second;

    // Default constructor
    Vector2();

    // Constructor with values
    Vector2(T first, T second);

    // Copy constructor
    Vector2<T> copy();

    // Operator overloads
    Vector2<T> operator+(const Vector2<T>& other);
    Vector2<T> operator+=(const Vector2<T>& other);
    Vector2<T> operator*(T scalar) const;

    // Dot product
    T dot(const Vector2<T>& other) const;

    // Length and normalization
    float length() const;
    void normalize();
    Vector2<T> normalized() const;

    // Friend function for scalar multiplication
    friend Vector2<T> operator*(T scalar, const Vector2<T>& vec) {
        return vec * scalar;
    }
};

template <typename T>
class Vector3 : public Vector<T> {
   public:
    T first, second, third;

    Vector3();
    Vector3(T first, T second, T third);

    Vector3<T> copy();
    Vector3<T> operator+(const Vector3<T>& other);
    Vector3<T> operator+=(const Vector3<T>& other);
    Vector3<T> operator*(T scalar) const;
    T dot(const Vector3<T>& other) const;
    Vector3<T> cross(const Vector3<T>& other) const;

    float length() const;
    void normalize();
    Vector3<T> normalized() const;

    friend Vector3<T> operator*(T scalar, const Vector3<T>& vec) {
        return vec * scalar;
    }

    Vector3<T> operator-(const Vector3<T>& other) const;

    Vector3<T> operator-() const {
        return Vector3<T>(-this->first, -this->second, -this->third);
    }

    Point3D toPoint3D() const;
};

template <typename T>
class Vector4 : public Vector3<T> {
   public:
    T fourth;

    Vector4(T first, T second, T third, T fourth);

    Vector4<T> copy();
    Vector4<T> operator+(const Vector4<T>& other);
    Vector4<T> operator+=(const Vector4<T>& other);
};

struct VertexKey {
    Point3D position;

    bool operator==(const VertexKey& other) const;
};

namespace std {
template <>
struct hash<VertexKey> {
    std::size_t operator()(const VertexKey& key) const;
};
}  // namespace std

struct ExtendedVertexKey {
    Point3D position;
    Vector3<float> normal;

    bool operator==(const ExtendedVertexKey& other) const;
};

namespace std {
template <>
struct hash<ExtendedVertexKey>;
}

#include "common/util/vectors.tpp"