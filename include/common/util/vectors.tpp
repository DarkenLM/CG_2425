#pragma once

// ---- Vector ----
template <typename T>
Vector<T>::Vector(int d) : dim(d) {}

// ---- Vector3 ----
template <typename T>
Vector3<T>::Vector3() : Vector<T>(3), first(0), second(0), third(0) {}

template <typename T>
Vector3<T>::Vector3(T first, T second, T third) : Vector<T>(3) {
    this->first = first;
    this->second = second;
    this->third = third;
}

template <typename T>
Vector3<T> Vector3<T>::copy() {
    return Vector3(this->first, this->second, this->third);
}

template <typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) {
    return Vector3(
        this->first + other.first,
        this->second + other.second,
        this->third + other.third);
}

template <typename T>
Vector3<T> Vector3<T>::operator+=(const Vector3<T>& other) {
    this->first += other.first;
    this->second += other.second;
    this->third += other.third;
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator*(T scalar) const {
    return Vector3(
        this->first * scalar,
        this->second * scalar,
        this->third * scalar);
}

template <typename T>
T Vector3<T>::dot(const Vector3<T>& other) const {
    return this->first * other.first +
           this->second * other.second +
           this->third * other.third;
}

template <typename T>
Vector3<T> Vector3<T>::cross(const Vector3<T>& other) const {
    return Vector3<T>(
        this->second * other.third - this->third * other.second,
        this->third * other.first - this->first * other.third,
        this->first * other.second - this->second * other.first);
}

template <typename T>
float Vector3<T>::length() const {
    return std::sqrt(this->first * this->first +
                     this->second * this->second +
                     this->third * this->third);
}

template <typename T>
void Vector3<T>::normalize() {
    float len = length();
    if (len != 0) {
        this->first /= len;
        this->second /= len;
        this->third /= len;
    }
}

template <typename T>
Vector3<T> Vector3<T>::normalized() const {
    float len = length();
    if (len == 0) return *this;
    return Vector3<T>(this->first / len, this->second / len, this->third / len);
}

// You must define `Point3D` before this will compile
template <typename T>
Point3D Vector3<T>::toPoint3D() const {
    return Point3D(static_cast<float>(this->first),
                   static_cast<float>(this->second),
                   static_cast<float>(this->third));
}

// ---- Vector4 ----
template <typename T>
Vector4<T>::Vector4(T first, T second, T third, T fourth)
    : Vector3<T>(first, second, third) {
    this->fourth = fourth;
}

template <typename T>
Vector4<T> Vector4<T>::copy() {
    return Vector4(this->first, this->second, this->third, this->fourth);
}

template <typename T>
Vector4<T> Vector4<T>::operator+(const Vector4<T>& other) {
    return Vector4(
        this->first + other.first,
        this->second + other.second,
        this->third + other.third,
        this->fourth + other.fourth);
}

template <typename T>
Vector4<T> Vector4<T>::operator+=(const Vector4<T>& other) {
    this->first += other.first;
    this->second += other.second;
    this->third += other.third;
    this->fourth += other.fourth;
    return *this;
}

// ---- VertexKey ----
inline bool VertexKey::operator==(const VertexKey& other) const {
    return position == other.position &&
           normal.first == other.normal.first &&
           normal.second == other.normal.second &&
           normal.third == other.normal.third;
}

inline std::size_t std::hash<VertexKey>::operator()(const VertexKey& key) const {
    size_t h1 = hash<float>()(key.position.getX());
    size_t h2 = hash<float>()(key.position.getY());
    size_t h3 = hash<float>()(key.position.getZ());
    size_t h4 = hash<float>()(key.normal.first);
    size_t h5 = hash<float>()(key.normal.second);
    size_t h6 = hash<float>()(key.normal.third);
    return ((((((h1 ^ (h2 << 1)) ^ (h3 << 1)) ^ (h4 << 1)) ^ (h5 << 1)) ^ (h6 << 1)));
}
