#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <optional>
#include <vector>

#include "common/geometry/point.hpp"
#include "engine/glContext.hpp"
#include "engine/scene/CatmullRomCurve.hpp"

#define _TRANSFORM_IF3(transformer, source) \
    if (source.has_value())                 \
    transformer(source.value().first, source.value().second, source.value().third)

#define _TRANSFORM_IF4(transformer, source) \
    if (source.has_value())                 \
    transformer(source.value().first, source.value().second, source.value().third, source.value().fourth)

typedef enum transform_type {
    TRANSFORM_RESET_ALL,
    TRANSFORM_TRANSLATE,
    TRANSFORM_TRANSLATE_RESET,
    TRANSFORM_ROTATE,
    TRANSFORM_ROTATE_RESET,
    TRANSFORM_SCALE,
    TRANSFORM_SCALE_RESET
} TransformType;

class ObjectTranslation {
   public:
    ObjectTranslation() : _dynamic(false), curve({}) {};
    ObjectTranslation(float x, float y, float z) : _dynamic(false), x(x), y(y), z(z), curve({}) {};
    ObjectTranslation(int time, bool align, std::vector<Point3D> points)
        : _dynamic(true),
          time(time),
          align(align),
          points(points),
          curve(points) {}

    bool isDynamic() const {
        return this->_dynamic;
    }

    Point3D getPoint() const {
        return Point3D(this->x, this->y, this->z);
    }

    Vector3<float> getVector() const {
        return Vector3(this->x, this->y, this->z);
    }

    Point3D getPointAtTime(float time) {
        int ind = std::min(
            static_cast<int>(this->points.size() - 1),
            std::max(
                0,
                static_cast<int>(std::floor((time * this->points.size()) / this->time))));

        return this->points[ind];
    }

    void getInterpolatedPosition(float* pos, float* deriv, float elapsedTime) const {
        if (!isDynamic()) yeet "RIP";

        this->curve.getGlobalCatmullRomPoint(elapsedTime, pos, deriv);
    }

    void getCurrentRotation(float* xAxis, float* yAxis, float* zAxis, float* deriv, float* m) {
        Vector3<float> front = {deriv[0], deriv[1], deriv[2]};
        front.normalize();

        Vector3<float> up = {0.0f, 1.0f, 0.0f};
        if (front.first < 0) {
            // up.second = -1.0f;
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

    void drawMyCordSystem(float* pos, float* xAxis, float* yAxis, float* zAxis) {
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

    ObjectTranslation operator+(const Vector3<float>& other) {
        if (!this->_dynamic) return ObjectTranslation(this->time, this->align, this->points);

        return ObjectTranslation(
            this->x + other.first,
            this->y + other.second,
            this->z + other.third);
    }

    ObjectTranslation operator+=(const Vector3<float>& other) {
        if (!this->_dynamic) *this;

        this->x + other.first;
        this->y + other.second;
        this->z + other.third;

        return *this;
    }

    // private:
    bool _dynamic;

    // Static
    float x, y, z;

    // Dynamic
    int time;
    bool align;
    std::vector<Point3D> points;
    CatmullRom curve;
};

class ObjectRotation {
   public:
    ObjectRotation() : _dynamic(false) {};
    ObjectRotation(float angle, float x, float y, float z) : _dynamic(false), angle(angle), x(x), y(y), z(z) {};
    ObjectRotation(int time, float x, float y, float z)
        : _dynamic(true), time(time), x(x), y(y), z(z) {};

    bool isDynamic() const {
        return this->_dynamic;
    }

    float getAngle() const {
        return this->angle;
    }

    float getTime() const {
        return this->time;
    }

    void setAngle(float angle) {
        this->angle = angle;
    }

    void setTime(float time) {
        this->time = time;
    }

    Vector4<float> getVector4() const {
        return Vector4(this->angle, this->x, this->y, this->z);
    }

    float getRotationAngleAtTime(float time) {
        return (time * 360.0f) / this->time;
    }

    Vector4<float> getVector4AtTime(float time) {
        return Vector4(this->getRotationAngleAtTime(time), this->x, this->y, this->z);
    }

   private:
    bool _dynamic;
    float x, y, z;

    // Static
    float angle;

    // Dynamic
    int time;
};

class ObjectMaterial {
   public:
    ObjectMaterial(
        Vector3<int> diffuse,
        Vector3<int> ambient,
        Vector3<int> specular,
        Vector3<int> emissive,
        int shininess) : diffuse(diffuse), ambient(ambient), specular(specular), emissive(emissive), shininess(shininess) {};

    Vector3<int> getDiffuse() { return this->diffuse; };
    Vector3<int> getAmbient() { return this->ambient; };
    Vector3<int> getSpecular() { return this->specular; };
    Vector3<int> getEmissive() { return this->emissive; };
    int getShininess() { return this->shininess; };

   private:
    Vector3<int> diffuse;
    Vector3<int> ambient;
    Vector3<int> specular;
    Vector3<int> emissive;
    int shininess;
};
class Object {
   public:
    virtual ~Object() = default;

    /**
     * Draws this object into the currently prepared environment.
     */
    virtual void render() = 0;

    /**
     * Steps the internal state forward one tick for this object.
     * Can be used to animate the object, or any other action that must persist a state between ticks
     * and be updated regularly.
     *
     * @param deltaTime The time between the current frame and the last frame, to normalize the simulation
     * independently of the FPS (kinda, it's a lot more complicated than I can currently understand).
     */
    virtual void update(float deltaTime) {};

    /**
     * Moves this object back to it's origin.
     *
     * **NOTE:** It does not necessarily reset the object back to the world's origin, but rather
     * relative to the object's referential.
     */
    virtual void resetPosition() {};

    /**
     * Moves this object to the absolute position represented by the coordinates passed as arguments.
     *
     * @param x The X position to move to.
     * @param y The Y position to move to.
     * @param z The Z position to move to.
     */
    virtual void setPosition(float x, float y, float z) {};

    /**
     * Moves this object to the absolute position represented by the vector passed as arguments.
     *
     * @param pos The position vector to move to.
     */
    virtual void setPosition(Vector3<float> pos) {};

    /**
     * Moves this object to the position represented by the coordinates passed as arguments, relative to the object's
     * current position. The values for each coordinate are added to the current object's coordinate values.
     *
     * @param x The X position to move to.
     * @param y The Y position to move to.
     * @param z The Z position to move to.
     */
    virtual void moveTo(float x, float y, float z) {};

    /**
     * Moves this object to the position represented by the position vector passed as argument, relative to the
     * object's current position. The values for each coordinate are added to the current object's coordinate values.
     *
     * @param pos The position vector to move to.
     */
    virtual void moveTo(Vector3<float> pos) {};

    /**
     * Rotates this object back to it's original rotation.
     *
     * **NOTE:** It does not necessarily reset the object back to it's original rotation angles relative to the world,
     * but rather relative to the object's referential.
     */
    virtual void resetRotation() {};

    /**
     * Rotates this object absolutely using the angles passed as arguments.
     *
     * @param roll The rotation angle along the X axis.
     * @param pitch The rotation angle along the Y axis.
     * @param yaw The rotation angle along the Z axis.
     */
    virtual void setRotation(float roll, float pitch, float yaw) {};

    /**
     * Rotates this object relative to the current rotation using the angles passed as arguments.
     * The values for each angle are added to the current object's angle values.
     *
     * @param roll The rotation angle along the X axis.
     * @param pitch The rotation angle along the Y axis.
     * @param yaw The rotation angle along the Z axis.
     */
    virtual void rotateTo(float roll, float pitch, float yaw) {};

    /**
     * Rotates this object along the axis characterized by the first three parameters by
     * the specified angle. The axis is a unit vector, and it's parameters must be normalized.
     *
     * @param axisX The X parameter of the axis vector.
     * @param axisY The Y parameter of the axis vector.
     * @param axisZ The Z parameter of the axis vector.
     */
    virtual void rotateAlong(float axisX, float axisY, float axisZ, float angle) {};

    /**
     * Rotates this object along the axis characterized by the first three parameters of the vector by
     * the fourth parameter of the vector. The first three parameters of the vector represent a unit
     * vector, and must be normalized.
     *
     * @param vec The (axisX, axisY, axisZ, angle) vector.
     */
    virtual void rotateAlong(Vector4<float> vec) {};

    /**
     * Scales this object back to it's original scale.
     *
     * **NOTE:** It does not necessarily reset the object back to it's true original scale, but rather
     * relative to the object's referential.
     */
    virtual void resetScale() {};

    /**
     * Scales this object to the absolute scale represented by the values passed as arguments.
     *
     * @param x The scale on the X axis.
     * @param y The scale on the Y axis.
     * @param z The scale on the Z axis.
     */
    virtual void setScale(float x, float y, float z) {};

    /**
     * Scales this object to the absolute scale represented by the vector passed as argument.
     *
     * @param scale The vector representing the scale on each axis.
     */
    virtual void setScale(Vector3<float> scale) {};

    /**
     * Scales this object relative to to the current scale using the values passed as arguments.
     *
     * @param x The scale on the X axis.
     * @param y The scale on the Y axis.
     * @param z The scale on the Z axis.
     */
    virtual void scaleTo(float x, float y, float z) {};

    /**
     * Scales this object relative to to the current scale using the vector passed as argument.
     *
     * @param scale The vector representing the scale on each axis.
     */
    virtual void scaleTo(Vector3<float> scale) {};

    virtual bool hasMaterial() {
        return this->material.has_value();
    }

    virtual void setMaterial(ObjectMaterial material) {
        this->material = material;
    }

    virtual ObjectMaterial getMaterial() {
        return this->material.value();
    }

   protected:
    // Transform
    // std::optional<Vector3<float>> translation;
    std::optional<ObjectTranslation> translation;
    // std::optional<Vector4<float>> rotation;
    std::optional<ObjectRotation> rotation;
    std::optional<Vector3<float>> scale;
    std::vector<TransformType> tfStack;

    std::optional<ObjectMaterial> material;

    virtual void setTfStack(std::vector<TransformType> tfStack) {
        this->tfStack = tfStack;
    }
};