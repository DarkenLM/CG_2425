#pragma once

/*
    In essence, fuck unique pointers, fuck static blocks, fuck class factories, fuck BaseGeometry,
    fuck me for every coming up with this shit, and fuck you for reading this.

    I'm hardcoding all this shit because if I have to debug another 12 hours of link-level abuse,
    I'll probably go even more insane than I already am.
*/

#include "common/geometry/cone.hpp"
#include "common/geometry/plane.hpp"
#include "common/geometry/sphere.hpp"
#include "common/geometry/cylinder.hpp"
#include "common/geometry/BaseGeometry.hpp"

BaseGeometry* createGeometryFromKind(BaseGeometryKind kind, std::vector<Point3D> vertices);