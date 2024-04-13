#include "Application.h"

class TerrainGeneratorApp : public TerrainGenerator::Application {
    virtual void onUpdate() override {

    }
};


int main() {
    auto terrainGeneratorApp = std::make_unique<TerrainGeneratorApp>();
    terrainGeneratorApp->start(1024, 1024, "TerrainGenerator");
}
