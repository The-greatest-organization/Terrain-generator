#include "generator.hpp"


int main() {
    TerrainParams params{1, 1, 100};
    Terrain tr{};
//    tr.params.resolution = 1;//edit params
//    tr.generate();
//    tr.export_file("output", true);
//    tr.export_png();
    tr.import_file("output");
    tr.export_png();
    return 0;
}