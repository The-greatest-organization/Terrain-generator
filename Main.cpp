#include "Application.h"
#include <memory>

class TerrainGeneratorApp : public TerrainGenerator::Application {
    virtual void onUpdate() override {

    }
};

int main(void) {
    auto terrainGeneratorApp = std::make_unique<TerrainGeneratorApp>();
    terrainGeneratorApp->start(200, 200, "aa");
}
