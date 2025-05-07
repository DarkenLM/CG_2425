#pragma once
#include <iostream>

/**
 *  @class Point3D
 *  @brief Represents a 3D point with float-precision (x, y, z) coordinates.
 *
 *  The Point3D class stores the coordinates of a point in 3D space. It provides
 *  methods for setting, getting, and manipulating the coordinates.
 */
class Point3D {
   private:
    float x, y, z;

   public:
    /**
     *  @brief Default constructor. Initializes coordinates to (0,0,0).
     */
    Point3D();

    /**
     *  @brief Parameterized constructor.
     *  @param x X-coordinate value.
     *  @param y Y-coordinate value.
     *  @param z Z-coordinate value.
     */
    Point3D(float x, float y, float z);

    /**
     *  @brief Get the X-coordinate value.
     *  @return Current X-coordinate value as float.
     */
    float getX() const { return x; }

    /**
     *  @brief Get the Y-coordinate value.
     *  @return Current Y-coordinate value as float.
     */
    float getY() const { return y; }

    /**
     *  @brief Get the Z-coordinate value.
     *  @return Current Z-coordinate value as float.
     */
    float getZ() const { return z; }

    /**
     *  @brief Set the X-coordinate value.
     *  @param xVal New X-coordinate value.
     */
    void setX(float xVal);

    /**
     *  @brief Set the Y-coordinate value.
     *  @param yVal New Y-coordinate value.
     */
    void setY(float yVal);

    /**
     *  @brief Set the Z-coordinate value.
     *  @param zVal New Z-coordinate value.
     */
    void setZ(float zVal);

    /**
     *  @brief Set all coordinates simultaneously.
     *  @param xVal New X-coordinate value.
     *  @param yVal New Y-coordinate value.
     *  @param zVal New Z-coordinate value.
     */
    void set(float xVal, float yVal, float zVal);

    /**
     *  @brief Create a copy of this point with a new Y value.
     *  @param y New Y-coordinate value.
     *  @return A new Point3D instance with the updated Y value and other coordinates unchanged.
     */
    Point3D withY(float y) const;

    /**
     *  @brief Create a copy of this point with a new X value.
     *  @param x New X-coordinate value.
     *  @return A new Point3D instance with the updated X value and other coordinates unchanged.
     */
    Point3D withX(float x) const;

    /**
     *  @brief Create a copy of this point with a new Z value.
     *  @param z New Z-coordinate value.
     *  @return A new Point3D instance with the updated Z value and other coordinates unchanged.
     */
    Point3D withZ(float z) const;

    /**
     *  @brief Create a copy of this point.
     *  @return A new Point3D instance with identical coordinates.
     */
    Point3D copy();

    /**
     * @brief Equality comparison operator
     * @param other Point to compare with
     * @return true if all coordinates are equal, false otherwise
     */
    bool operator==(const Point3D& other) const;

    /**
     * @brief Inequality comparison operator
     * @param other Point to compare with
     * @return true if any coordinate differs, false otherwise
     */
    bool operator!=(const Point3D& other) const;

    /**
     * @brief Addition operator.
     * @param a First Point to add.
     * @param b Second Point to add.
     * @return A new Point3D instance representing the sum of both points.
     */
    friend Point3D operator+(const Point3D& a, const Point3D& b);

    /**
     * @brief Subtraction operator.
     * @param other Point to subtract from the current point.
     * @return A new Point3D instance representing the difference.
     */
    Point3D operator-(const Point3D& other) const;

    /**
     * @brief Scalar multiplication operator (point * scalar).
     * @param point The point to multiply.
     * @param scalar The scalar value to multiply the point by.
     * @return A new Point3D instance with each coordinate multiplied by the scalar.
     */
    friend Point3D operator*(const Point3D& point, float scalar);

    /**
     * @brief Scalar multiplication operator (scalar * point).
     * @param scalar The scalar value to multiply.
     * @param point The point to multiply.
     * @return A new Point3D instance with each coordinate multiplied by the scalar.
     */
    friend Point3D operator*(float scalar, const Point3D& point);

    /**
     * @brief Stream insertion operator overload
     * @param os Output stream
     * @param point Point to format
     * @return Output stream with formatted point string "(x, y, z)"
     */
    friend std::ostream& operator<<(std::ostream& os, const Point3D& point);

    /**
     * @brief Return the raw coordinate data as a float array.
     * @return Pointer to the internal coordinate data (x, y, z).
     */
    float* data() { return &x; }
};