struct Vertice {
    float x;
    float y;
    float z;
};

struct Trigon {
    Vertice vertices[3] [[inline]];
};

u8 version @ 0x0;
u16 numTrigons @ $;

Trigon trigons[numTrigons] @ $;