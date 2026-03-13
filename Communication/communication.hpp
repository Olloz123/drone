
enum packet_type {
    RAND_PACK,
    MOVEMENT_PACK
};

struct rand_pack {
    packet_type type;
    int x;
    int y;

    rand_pack() : type(RAND_PACK), x(0), y(0) {}
};

struct movement_pack {
    packet_type type;
    int x;
    int y;
    int z;

    movement_pack() : type(MOVEMENT_PACK), x(0), y(0), z(0) {}
};