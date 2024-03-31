usage:
you should include generator.h file, it contains Terrain class
    auto tr = Terrain(uint size, uint resolution, uint seed,
            double perlin_intensity = 1.2, double perlin_frequency = 4, double perlin_amplitude = 1,
            uint perlin_iterations = 10,
            uint hydraulic_erosion_iterations = 10, double hydraulic_erosion_sub_intensity = 0.5,
            double hydraulic_erosion_add_intensity = 0.4, uint max_height = MAX_HEIGHT,
            bool cuda = false);

    to generate new terrain use tr.generate();
    to get terrain 2d uint array use tr.get_map()-->uint** map

    to save map as png file use tr.export_png(string filename = "result.png")

    to preload hmap from png use tr.load_png(string filename)

required hfiles:
https://github.com/nothings/stb/blob/master/stb_image.h
https://github.com/nothings/stb/blob/master/stb_image_write.h
(this files is a public domain. idk if I need to provide attribution. i think not)