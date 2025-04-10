#pragma once
#include <iostream>
#include "engine/glContext.hpp"

/**
 *  @class Point3D
 *  @brief Represents a 3D point with double-precision(float) coordinates (x,y,z).
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
    Point3D(double x, double y, double z);

    /**
     *  @brief Get the X-coordinate value.
     *  @return Current X-coordinate value as double.
     */
    double getX() const { return x; }

    /**
     *  @brief Get the Y-coordinate value.
     *  @return Current Y-coordinate value as double.
     */
    double getY() const { return y; }

    /**
     *  @brief Get the Z-coordinate value.
     *  @return Current Z-coordinate value as double.
     */
    double getZ() const { return z; }

    /**
     *  @brief Set the X-coordinate value.
     *  @param xVal New X-coordinate value.
     */
    void setX(double xVal);

    /**
     *  @brief Set the Y-coordinate value.
     *  @param yVal New Y-coordinate value.
     */
    void setY(double yVal);

    /**
     *  @brief Set the Z-coordinate value.
     *  @param zVal New Z-coordinate value.
     */
    void setZ(double zVal);

    /**
     *  @brief Set all coordinates simultaneously.
     *  @param xVal New X-coordinate value.
     *  @param yVal New Y-coordinate value.
     *  @param zVal New Z-coordinate value.
     */
    void set(double xVal, double yVal, double zVal);

    /**
     *  @brief Create a copy of this point.
     *  @return A new Point3D instance with identical coordinates.
     */
    Point3D copy();

    Vector3<float> toVector3() {
        return Vector3(this->x, this->y, this->z);
    }

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
     * @brief Stream insertion operator overload
     * @param os Output stream
     * @param point Point to format
     * @return Output stream with formatted point string "(x, y, z)"
     */
    friend std::ostream& operator<<(std::ostream& os, const Point3D& point);
};
