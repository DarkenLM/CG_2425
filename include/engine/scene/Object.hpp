#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>
#include <optional>
#include <vector>

#include "common/geometry/point.hpp"
#include "common/util/vectors.hpp"
#include "engine/scene/CatmullRomCurve.hpp"

// ----- Macros -----
#define _TRANSFORM_IF3(transformer, source) \
    if (source.has_value())                 \
    transformer(source.value().first, source.value().second, source.value().third)

#define _TRANSFORM_IF4(transformer, source) \
    if (source.has_value())                 \
    transformer(source.value().first, source.value().second, source.value().third, source.value().fourth)

/**
 * @enum transform_type
 * @brief Possible transformation types of an Object.
 */
typedef enum transform_type {
    TRANSFORM_RESET_ALL,
    TRANSFORM_TRANSLATE,
    TRANSFORM_TRANSLATE_RESET,
    TRANSFORM_ROTATE,
    TRANSFORM_ROTATE_RESET,
    TRANSFORM_SCALE,
    TRANSFORM_SCALE_RESET
} TransformType;

/**
 * @class ObjectTranslation
 * @brief Handles both static and dynamic translations of an object in 3D space.
 */
class ObjectTranslation {
   public:
    /**
     * @brief Default constructor for a static translation.
     */
    ObjectTranslation();

    /**
     * @brief Creates a static translation.
     * @param x X-axis offset.
     * @param y Y-axis offset.
     * @param z Z-axis offset.
     */
    ObjectTranslation(float x, float y, float z);

    /**
     * @brief Creates a dynamic translation along a curve.
     * @param time Time it takes to traverse the path.
     * @param align Whether to align the object to the curve's direction.
     * @param points Control points that define the path.
     */
    ObjectTranslation(int time, bool align, std::vector<Point3D> points);

    bool isDynamic() const;   /**< Checks whether the translation is dynamic. */
    Point3D getPoint() const; /**< Returns the static translation point. */

    bool getShowCurve() const;         /**< Returns the boolean value associated with the curve being visible or not. */
    void setShowCurve(bool showCurve); /**< Sets the boolean value that defines if curve is visible or not. */

    // In ObjectTranslation class
    bool* getShowCurvePtr() {
        return &showCurve;
    }

    int getTime() const;    /**< Returns the total duration of the animation. */
    void setTime(int time); /**< Sets the duration of the animation. */

    bool getAlign() const;             /**< Checks whether the object is aligned to the curve. */
    void setAlign(bool newAlignValue); /**< Sets alignment with the curve. */

    CatmullRom getCurve() const;        /**< Returns the translation curve. */
    void setCurve(CatmullRom newCurve); /**< Sets a new translation curve. */

    Vector3<float> getVector() const; /**< Returns the translation as a vector. */

    /**
     * @brief Gets a point on the path at a specific time.
     * @param time Elapsed time.
     * @return Point3D at that moment.
     */
    Point3D getPointAtTime(float time);

    /**
     * @brief Gets interpolated position and derivative at a time along the curve.
     * @param pos Output: interpolated position.
     * @param deriv Output: tangent vector (derivative).
     * @param elapsedTime Time to sample from.
     */
    void getInterpolatedPosition(float* pos, float* deriv, float elapsedTime) const;

    /**
     * @brief Computes rotation axes from curve derivative.
     * @param xAxis Output: front axis.
     * @param yAxis Output: up axis.
     * @param zAxis Output: right axis.
     * @param deriv Derivative vector.
     * @param m Output: rotation matrix.
     */
    void getCurrentRotation(float* xAxis, float* yAxis, float* zAxis, float* deriv, float* m);

    /**
     * @brief Draws the local coordinate system at a position.
     * @param pos Position in space.
     * @param xAxis X-axis direction.
     * @param yAxis Y-axis direction.
     * @param zAxis Z-axis direction.
     */
    void drawMyCordSystem(float* pos, float* xAxis, float* yAxis, float* zAxis);

    /**
     * @brief Adds a vector to the translation.
     */
    ObjectTranslation operator+(const Vector3<float>& other);

    /**
     * @brief Adds and updates the current translation.
     */
    ObjectTranslation operator+=(const Vector3<float>& other);

   private:
    bool _dynamic;               /**< Indicates whether the translation is dynamic (animated) or static. */
    bool showCurve;              /**< Indicates whether the curve is visivel or not. */
    float x, y, z;               /**< Coordinates for static translation. */
    int time;                    /**< Duration (in seconds or frames) for dynamic translation along the curve. */
    bool align;                  /**< Indicates whether the object should align its orientation to the path curve. */
    std::vector<Point3D> points; /**< Control points used to define the Catmull-Rom spline curve. */
    CatmullRom curve;            /**< Interpolation curve used for dynamic translation. */
};

/**
 * @class ObjectRotation
 * @brief Represents a static or dynamic rotation transformation.
 */
class ObjectRotation {
   public:
    /**
     * @brief Default constructor. Initializes as static with default values.
     */
    ObjectRotation();

    /**
     * @brief Constructs a static rotation with a fixed angle and axis.
     * @param angle Rotation angle in degrees.
     * @param x X component of the rotation axis.
     * @param y Y component of the rotation axis.
     * @param z Z component of the rotation axis.
     */
    ObjectRotation(float angle, float x, float y, float z);

    /**
     * @brief Constructs a dynamic rotation over time along a given axis.
     * @param time Duration of the rotation.
     * @param x X component of the rotation axis.
     * @param y Y component of the rotation axis.
     * @param z Z component of the rotation axis.
     */
    ObjectRotation(int time, float x, float y, float z);

    /**
     * @brief Checks if the rotation is dynamic (time-based).
     * @return True if dynamic, false if static.
     */
    bool isDynamic() const;

    /**
     * @brief Gets the static rotation angle.
     * @return Rotation angle in degrees.
     */
    float getAngle() const;

    /**
     * @brief Gets the duration of the rotation if dynamic.
     * @return Time value.
     */
    float getTime() const;

    /**
     * @brief Sets the rotation angle.
     * @param angle New angle in degrees.
     */
    void setAngle(float angle);

    /**
     * @brief Sets the rotation duration (for dynamic rotations).
     * @param time New time value.
     */
    void setTime(float time);

    /**
     * @brief Gets the rotation as a 4D vector (angle + axis).
     * @return Vector4 containing angle and axis.
     */
    Vector4<float> getVector4() const;

    /**
     * @brief Computes the rotation angle at a given moment in time.
     * @param time Elapsed time.
     * @return Rotation angle corresponding to the given time.
     */
    float getRotationAngleAtTime(float time);

    /**
     * @brief Gets the rotation vector at a given time.
     * @param time Elapsed time.
     * @return Vector4 with interpolated rotation angle and axis.
     */
    Vector4<float> getVector4AtTime(float time);

   private:
    bool _dynamic; /**< Whether the rotation is dynamic (true) or static (false). */
    float x, y, z; /**< Components of the rotation axis. */
    float angle;   /**< rotation angle. */
    int time;      /**< Duration of the dynamic rotation (used when _dynamic is true). */
};

/**
 * @class ObjectMaterial
 * @brief Represents the material properties of a 3D object.
 *
 * This class stores the material attributes used for lighting and shading,
 * including diffuse, ambient, specular, emissive components, and shininess.
 */
class ObjectMaterial {
   public:
    /**
     * @brief Constructs a material with all its lighting components.
     * @param diffuse RGB diffuse component.
     * @param ambient RGB ambient component.
     * @param specular RGB specular component.
     * @param emissive RGB emissive component.
     * @param shininess Shininess coefficient used in specular lighting.
     */
    ObjectMaterial(Vector3<float> diffuse, Vector3<float> ambient, Vector3<float> specular, Vector3<float> emissive, float shininess);

    /**
     * @brief Gets the diffuse component of the material.
     * @return RGB vector representing diffuse color.
     */
    Vector3<float> getDiffuse();

    /**
     * @brief Gets the ambient component of the material.
     * @return RGB vector representing ambient color.
     */
    Vector3<float> getAmbient();

    /**
     * @brief Gets the specular component of the material.
     * @return RGB vector representing specular color.
     */
    Vector3<float> getSpecular();

    /**
     * @brief Gets the emissive component of the material.
     * @return RGB vector representing emissive color.
     */
    Vector3<float> getEmissive();

    /**
     * @brief Gets the shininess value of the material.
     * @return Integer representing the shininess coefficient.
     */
    float getShininess();

   private:
    Vector3<float> diffuse;  /**< Diffuse color component. */
    Vector3<float> ambient;  /**< Ambient color component. */
    Vector3<float> specular; /**< Specular color component. */
    Vector3<float> emissive; /**< Emissive color component. */
    float shininess;         /**< Shininess factor for specular reflection. */
};

/**
 * @class Object
 * @brief Abstract base class representing a renderable 3D object.
 *
 * Provides a common interface for all 3D objects, including transformation operations
 * such as translation, rotation, and scaling, as well as material management and rendering.
 */
class Object {
   public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Object() = default;

    /**
     * @brief Pure virtual method for rendering the object.
     * Must be implemented by all derived classes.
     */
    virtual void render() = 0;

    /**
     * @brief Updates the object's state based on elapsed time.
     * @param deltaTime Time in seconds since the last update.
     */
    virtual void update(float deltaTime) {};

    /** @brief Resets the object’s position to its default state. */
    virtual void resetPosition() {};

    /**
     * @brief Sets the position of the object using individual coordinates.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param z Z-coordinate.
     */
    virtual void setPosition(float x, float y, float z) {};

    /**
     * @brief Sets the position of the object using a vector.
     * @param pos A 3D vector specifying the position.
     */
    virtual void setPosition(Vector3<float> pos) {};

    /**
     * @brief Moves the object to a new position using individual coordinates.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param z Z-coordinate.
     */
    virtual void moveTo(float x, float y, float z) {};

    /**
     * @brief Moves the object to a new position using a vector.
     * @param pos A 3D vector specifying the destination position.
     */
    virtual void moveTo(Vector3<float> pos) {};

    /** @brief Resets the object’s rotation to its default orientation. */
    virtual void resetRotation() {};

    /**
     * @brief Sets the object's rotation using Euler angles.
     * @param roll Rotation around X-axis.
     * @param pitch Rotation around Y-axis.
     * @param yaw Rotation around Z-axis.
     */
    virtual void setRotation(float roll, float pitch, float yaw) {};

    /**
     * @brief Rotates the object to a specific orientation using Euler angles.
     * @param roll Rotation around X-axis.
     * @param pitch Rotation around Y-axis.
     * @param yaw Rotation around Z-axis.
     */
    virtual void rotateTo(float roll, float pitch, float yaw) {};

    /**
     * @brief Rotates the object around a given axis by a certain angle.
     * @param axisX X-component of the axis.
     * @param axisY Y-component of the axis.
     * @param axisZ Z-component of the axis.
     * @param angle Angle in degrees.
     */
    virtual void rotateAlong(float axisX, float axisY, float axisZ, float angle) {};

    /**
     * @brief Rotates the object using a 4D vector (angle + axis).
     * @param vec A 4D vector where the first element is the angle,
     *            and the rest define the rotation axis.
     */
    virtual void rotateAlong(Vector4<float> vec) {};

    /** @brief Resets the object's scale to its default size. */
    virtual void resetScale() {};

    /**
     * @brief Sets the scale of the object using individual scale factors.
     * @param x Scale along X-axis.
     * @param y Scale along Y-axis.
     * @param z Scale along Z-axis.
     */
    virtual void setScale(float x, float y, float z) {};

    /**
     * @brief Sets the scale of the object using a vector.
     * @param scale A 3D vector specifying the scale.
     */
    virtual void setScale(Vector3<float> scale) {};

    /**
     * @brief Scales the object to a new size using individual scale factors.
     * @param x New scale along X-axis.
     * @param y New scale along Y-axis.
     * @param z New scale along Z-axis.
     */
    virtual void scaleTo(float x, float y, float z) {};

    /**
     * @brief Scales the object to a new size using a vector.
     * @param scale A 3D vector specifying the new scale.
     */
    virtual void scaleTo(Vector3<float> scale) {};

    /**
     * @brief Checks if the object has a material assigned.
     * @return True if a material is present, false otherwise.
     */
    virtual bool hasMaterial() {
        return this->material.has_value();
    }

    /**
     * @brief Assigns a material to the object.
     * @param material The material to be assigned.
     */
    virtual void setMaterial(ObjectMaterial material) {
        this->material = material;
    }

    /**
     * @brief Retrieves the assigned material.
     * @return The current material.
     */
    virtual ObjectMaterial getMaterial() {
        return this->material.value();
    }

   protected:
    std::optional<ObjectTranslation> translation; /**< Optional translation component. */
    std::optional<ObjectRotation> rotation;       /**< Optional rotation component. */
    std::optional<Vector3<float>> scale;          /**< Optional scaling vector. */
    std::optional<ObjectMaterial> material;       /**< Optional material definition. */
    std::vector<TransformType> tfStack;           /**< Stack of transformation operations. */
};
