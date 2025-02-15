#ifndef POINT3D_H
#define POINT3D_H

#include <cmath>
#include <iostream>

class Point3D {
   private:
    float x, y, z;

   public:
    Point3D();
    Point3D(double x, double y, double z);

    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double xVal);
    void setY(double yVal);
    void setZ(double zVal);

    bool operator==(const Point3D& other) const;
    bool operator!=(const Point3D& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Point3D& point);
};

#endif
