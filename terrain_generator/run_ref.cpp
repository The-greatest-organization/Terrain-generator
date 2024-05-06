#include "generator.hpp"


int main() {
    TerrainParams params{1, 1, 100};
    params.smoothing_hydraulic_erosion_steps = 0;
    params.real_hydraulic_erosion_steps = 15;
    Terrain tr{params};
//    tr.params.resolution = 1;//edit params
    tr.generate();
//    tr.export_file("output", true);
    tr.export_png();
//    tr.import_file("output");
//    tr.export_png();
    return 0;
}