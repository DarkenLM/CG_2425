#pragma once

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

template <typename T>
class Vector {
    public:
        int dim;

        Vector(int d): dim(d) {}
};

template <typename T>
class Vector3: public Vector<T> {
    public:
        T first, second, third;

        Vector3(T first, T second, T third): Vector<T>(3) {
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
                this->third + other.third
            );
        }

        Vector3<T> operator+=(const Vector3<T>& other) {
            this->first  += other.first;
            this->second += other.second;
            this->third  += other.third;

            return *this;
        }
};

template <typename T>
class Vector4: public Vector3<T> {
    public:
        T fourth;

        Vector4(T first, T second, T third, T fourth): Vector3<T>(first, second, third) {
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
                this->fourth + other.fourth
            );
        }

        Vector4<T> operator+=(const Vector4<T>& other) {
            this->first  += other.first;
            this->second += other.second;
            this->third  += other.third;
            this->fourth += other.fourth;
        }
};