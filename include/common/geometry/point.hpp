#pragma once
#include <iostream>

class Point3D {
    private:
        float x, y, z;

    public:
        Point3D();
        Point3D(double x, double y, double z);

        // double getX() const;
        // double getY() const;
        // double getZ() const;

        // Separating the implementation makes G++ scream at me.
        double getX() const { return x; }
        double getY() const { return y; }
        double getZ() const { return z; }

        void setX(double xVal);
        void setY(double yVal);
        void setZ(double zVal);

        Point3D copy();

        bool operator==(const Point3D& other) const;
        bool operator!=(const Point3D& other) const;

        friend std::ostream& operator<<(std::ostream& os, const Point3D& point);
};
