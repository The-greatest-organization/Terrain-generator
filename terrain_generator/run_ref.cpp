#include "generator.hpp"

int main() {
    TerrainParams params {1, 1, 100};
    Terrain tr{params};
    tr.params.resolution=10;//edit params
//    tr.load_png("sourse.png");
    tr.generate();
    tr.export_png();
    return 0;
}