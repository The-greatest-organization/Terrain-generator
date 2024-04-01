#include "generator.h"

int main() {

    Terrain tr{100, 200,1, 1, 0.9, 4, 1, 12,
               5, 0.5, 0.4,
               20,0.6,0.5,1, 0.5};
//    tr.load_png("sourse.png");
    tr.generate();
    tr.export_png();
    return 0;
}