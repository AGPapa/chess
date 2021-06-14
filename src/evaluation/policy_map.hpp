#include <map>

#include "../chess/ply.cpp"

// TODO: complete policy_map
const std::map<Ply,int> policy_map = {
    { Ply("a1a2"), 1 },
    { Ply("a1a3"), 2 },
    { Ply("a1a4"), 3 },
    { Ply("a1a5"), 4 },
    { Ply("a1a6"), 5 },
    { Ply("a1a7"), 6 },
    { Ply("a1a8"), 7 },
    { Ply("a1b1"), 8 },
    { Ply("a1c1"), 9 },
    { Ply("a1d1"), 10 },
    { Ply("a1e1"), 11 },
    { Ply("a1f1"), 12 },
    { Ply("a1g1"), 13 },
    { Ply("a1h1"), 14 }
};