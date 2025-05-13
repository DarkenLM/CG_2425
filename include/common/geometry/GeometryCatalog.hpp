#pragma once

/*
    In essence, fuck unique pointers, fuck static blocks, fuck class factories, fuck BaseGeometry,
    fuck me for ever coming up with that shit, and fuck you for reading this.

    I'm hardcoding all this shit because if I have to debug another 12 hours of link-level abuse,
    I'll probably go even more insane than I already am.
*/

#include "common/geometry/BaseGeometry.hpp"
#include "common/geometry/bezier.hpp"
#include "common/geometry/box.hpp"
#include "common/geometry/cone.hpp"
#include "common/geometry/cylinder.hpp"
#include "common/geometry/flatRing.hpp"
#include "common/geometry/icosphere.hpp"
#include "common/geometry/obj.hpp"
#include "common/geometry/plane.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/sphere.hpp"
#include "common/geometry/torus.hpp"

BaseGeometry* createGeometryFromKind(BaseGeometryKind kind, std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices);