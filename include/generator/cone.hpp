#ifndef CONE_H
#define CONE_H

#include <vector>
#include "include/generator/point.hpp"

void coneTriangleGenerator(int radius, int height, int slices, int stacks, std::vector<Point3D>& points);
int coneGenerator(int radius, int height, int slices, int stacks);

#endif