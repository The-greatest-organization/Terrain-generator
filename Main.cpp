#include <iostream>
#include <type_traits>
#include "Tiny3D/Component.hpp"

void foo(tiny3d::ObjectComponent* comp) {
    auto scale = reinterpret_cast<tiny3d::Scale2D*>(comp);
}

int main() {
    tiny3d::Scale2D* comp = new tiny3d::Scale2D(1, 2);
}
