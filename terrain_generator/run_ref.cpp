#include "generator.h"

int main() {

    Terrain tr{100, 200,1, 2, 0.8, 4, 1, 12,
               5, 0.5, 0.4,
               100,0.6,0.5,1, 0.1};
//    tr.load_png("sourse.png");
    tr.generate();
    tr.export_png();
    return 0;
}