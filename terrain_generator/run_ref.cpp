#include "generator.h"

int main() {

    Terrain tr{100, 1, 1, 0.8, 4, 1, 12,
               15, 0.5, 0.4};
//    tr.load_png("sourse.png");
    tr.generate();
    tr.export_png();
    return 0;
}