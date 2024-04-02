#include "Application.h"

class TerrainGeneratorApp : public TerrainGenerator::Application {
    virtual void onUpdate() override {

    }
};


int main(void) {
    auto terrainGeneratorApp = std::make_unique<TerrainGeneratorApp>();
    terrainGeneratorApp->start(1024, 1024, "aa");
}
