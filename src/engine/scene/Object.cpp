#define _USE_MATH_DEFINES
#include "engine/scene/Object.hpp"

#include <math.h>

#include "common/debug.hpp"
#include "common/util/vectors.hpp"

ObjectTranslation::ObjectTranslation() : _dynamic(false), curve({}) {};

ObjectTranslation::ObjectTranslation(float x, float y, float z) : _dynamic(false), x(x), y(y), z(z), curve({}) {};

ObjectTranslation::ObjectTranslation(int time, bool align, std::vector<Point3D> points)
    : _dynamic(true),
      time(time),
      align(align),
      points(points),
      curve(points) {}

bool ObjectTranslation::isDynamic() const {
    return this->_dynamic;
}

Point3D ObjectTranslation::getPoint() const {
    return Point3D(this->x, this->y, this->z);
}

int ObjectTranslation::getTime() const {
    return this->time;
};
void ObjectTranslation::setTime(int time) {
    this->time = time;
};

bool ObjectTranslation::getAlign() const {
    return this->align;
};

void ObjectTranslation::setAlign(bool newAlignValue) {
    this->align = newAlignValue;
};

CatmullRom ObjectTranslation::getCurve() const {
    return this->curve;
};

void ObjectTranslation::setCurve(CatmullRom newCurve) {
    this->curve = newCurve;
};

Vector3<float> ObjectTranslation::getVector() const {
    return Vector3(this->x, this->y, this->z);
}

Point3D ObjectTranslation::getPointAtTime(float time) {
    int ind = std::min(
        static_cast<int>(this->points.size() - 1),
        std::max(
            0,
            static_cast<int>(std::floor((time * this->points.size()) / this->time))));

    return this->points[ind];
}

void ObjectTranslation::getInterpolatedPosition(float* pos, float* deriv, float elapsedTime) const {
    if (!isDynamic()) yeet "RIP";

    this->curve.getGlobalCatmullRomPoint(elapsedTime, pos, deriv);
}

void ObjectTranslation::getCurrentRotation(float* xAxis, float* yAxis, float* zAxis, float* deriv, float* m) {
    Vector3<float> front = {deriv[0], deriv[1], deriv[2]};
    front.normalize();

    Vector3<float> up = {0.0f, 1.0f, 0.0f};
    if (front.first < 0) {
        // TODO: U know
        //// up.second = -1.0f;
    }

    Vector3<float> right = front.cross(up);
    right.normalize();

    Vector3<float> top = right.cross(front);

    xAxis[0] = front.first;
    xAxis[1] = front.second;
    xAxis[2] = front.third;

    yAxis[0] = top.first;
    yAxis[1] = top.second;
    yAxis[2] = top.third;

    zAxis[0] = right.first;
    zAxis[1] = right.second;
    zAxis[2] = right.third;

    this->curve.buildRotMatrix(xAxis, yAxis, zAxis, m);
}

void ObjectTranslation::drawMyCordSystem(float* pos, float* xAxis, float* yAxis, float* zAxis) {
    glBegin(GL_LINES);
    // x in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(pos[0], pos[1], pos[2]);
    glVertex3f(pos[0] + xAxis[0], pos[1] + xAxis[1], pos[2] + xAxis[2]);

    // z in blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(pos[0], pos[1], pos[2]);
    glVertex3f(pos[0] + zAxis[0], pos[1] + zAxis[1], pos[2] + zAxis[2]);
    // y in green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(pos[0], pos[1], pos[2]);
    glVertex3f(pos[0] + yAxis[0], pos[1] + yAxis[1], pos[2] + yAxis[2]);
    // Changes color back to white
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
}

ObjectTranslation ObjectTranslation::operator+(const Vector3<float>& other) {
    if (!this->_dynamic) return ObjectTranslation(this->time, this->align, this->points);

    return ObjectTranslation(
        this->x + other.first,
        this->y + other.second,
        this->z + other.third);
}

ObjectTranslation ObjectTranslation::operator+=(const Vector3<float>& other) {
    if (!this->_dynamic) *this;

    this->x + other.first;
    this->y + other.second;
    this->z + other.third;

    return *this;
}

ObjectRotation::ObjectRotation() : _dynamic(false) {};

ObjectRotation::ObjectRotation(float angle, float x, float y, float z) : _dynamic(false), angle(angle), x(x), y(y), z(z) {};

ObjectRotation::ObjectRotation(int time, float x, float y, float z)
    : _dynamic(true), time(time), x(x), y(y), z(z) {};

bool ObjectRotation::isDynamic() const {
    return this->_dynamic;
}

float ObjectRotation::getAngle() const {
    return this->angle;
}

float ObjectRotation::getTime() const {
    return this->time;
}

void ObjectRotation::setAngle(float angle) {
    this->angle = angle;
}

void ObjectRotation::setTime(float time) {
    this->time = time;
}

Vector4<float> ObjectRotation::getVector4() const {
    return Vector4(this->angle, this->x, this->y, this->z);
}

float ObjectRotation::getRotationAngleAtTime(float time) {
    return (time * 360.0f) / this->time;
}

Vector4<float> ObjectRotation::getVector4AtTime(float time) {
    return Vector4(this->getRotationAngleAtTime(time), this->x, this->y, this->z);
}

ObjectMaterial::ObjectMaterial(
    Vector3<int> diffuse,
    Vector3<int> ambient,
    Vector3<int> specular,
    Vector3<int> emissive,
    int shininess) : diffuse(diffuse), ambient(ambient), specular(specular), emissive(emissive), shininess(shininess) {};

Vector3<int> ObjectMaterial::getDiffuse() { return this->diffuse; };

Vector3<int> ObjectMaterial::getAmbient() { return this->ambient; };

Vector3<int> ObjectMaterial::getSpecular() { return this->specular; };

Vector3<int> ObjectMaterial::getEmissive() { return this->emissive; };

int ObjectMaterial::getShininess() { return this->shininess; };