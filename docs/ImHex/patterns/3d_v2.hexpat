enum GeometryKind: u8 {
    _GEOMETRY_BASE,
    GEOMETRY_CONE,
    GEOMETRY_PLANE,
    GEOMETRY_BOX,
    GEOMETRY_SPHERE,
    GEOMETRY_CYLINDER,
    GEOMETRY_DONOT
};

struct Vertice {
    float x;
    float y;
    float z;
};

struct Trigon {
    Vertice vertices[3] [[inline]];
};

u8 version @ 0x0;
GeometryKind kind @ $;
u16 numTrigons @ $;

Trigon trigons[numTrigons] @ $;