#pragma once

#include <fstream>
#include "algorithm"
#include "random"
#include "iostream"
#include "cmath"
#include <cstring>
#include <climits>


template<typename type>
class SimpleRandom {
    std::uniform_int_distribution<type> range;
    std::uniform_int_distribution<type> range_interval;

public:
    std::mt19937 gen;

    SimpleRandom(int seed, type min, type max) {
        range = std::uniform_int_distribution<type>(min, max);
        range_interval = std::uniform_int_distribution<type>(0);
        gen = std::mt19937(seed);
    }

    type next() {
        return range(gen) - range_interval(gen);
    }

};


class Noise {
    struct vec2 {
        float x, y;

    };

    static vec2 pseudo_random_gradient_vector(int x, int y) {
        //TODO  I stole it and it's a little strange. redo
        // should return random normal vector, for the same x y the same

        const unsigned w = 8 * sizeof(unsigned);
        const unsigned s = w / 2;
        unsigned a = x, b = y;
        a *= 3284157443;

        b ^= a << s | a >> (w - s);
        b *= 1911520717;

        a ^= b << s | b >> (w - s);
        a *= 2048419325;
        float rnd = a * (3.14159265 / ~(~0u >> 1));

        vec2 v{std::sin(rnd), v.y = std::cos(rnd)};

        return v;
    }

    static float distance_gradient_dot(int ix, int iy, float x, float y) {
        vec2 gradient = pseudo_random_gradient_vector(ix, iy);

        //distance
        float dist_x = x - ix;
        float dist_y = y - iy;

        return dist_x * gradient.x + dist_y * gradient.y;

    }

    static float interpolate(float a, float b, float t) {
        t = t * t * (3.0f - 2.0f * t);//3rd degree smoothstep
        return a + t * (b - a);//TODO ?
    }


public:
    static float perlin_noise(float x, float y) {
        int top_left_x = (int) x;
        int top_left_y = (int) y;


        float top_left_value = distance_gradient_dot(top_left_x, top_left_y, x, y);
        float top_right_value = distance_gradient_dot(top_left_x + 1, top_left_y, x, y);

        float low_left_value = distance_gradient_dot(top_left_x, top_left_y + 1, x, y);
        float low_right_value = distance_gradient_dot(top_left_x + 1, top_left_y + 1, x, y);

        float top = interpolate(top_left_value, top_right_value, x - top_left_x);
        float low = interpolate(low_left_value, low_right_value, x - top_left_x);
        float res = interpolate(top, low, y - top_left_y);
        return res;
    }
};

struct TerrainParams {
    int seed;// [0:UINT_MAX]
    int resolution = 1;//limited by performance
    int size = 400;//limited by performance
    int max_height = 2000;//11000 below sea level + 8000 above sea level = 19000 - earth maximum


    int perlin_iterations = 12;//[0:20]
    float perlin_intensity = 0.9;//[0:1]
    float perlin_frequency = 4;//[0:5]
    float perlin_amplitude = 1;//[0:5]

    int smoothing_hydraulic_erosion_steps = 10;//[0:100]
    float smoothing_hydraulic_erosion_sub_intensity = 0.5;//[0:1]
    float smoothing_hydraulic_erosion_add_intensity = 0.4;//[0:smoothing_hydraulic_erosion_sub_intensity]

    int real_hydraulic_erosion_steps = 10;//[0:100]
    float real_hydraulic_erosion_intensity = 0.6;//precipitation per iteration, meters. [0:15] idk
    float real_hydraulic_erosion_fluidity_of_water = 0.5; //IDK how to call it better, lol. 0-1
    float real_hydraulic_erosion_soil_flowability = 1;// maybe [0:2], i will think about that one
    float real_hydraulic_erosion_non_evaporation_of_water = 0.5; //this one is funny too, also [0:1]

    bool cuda = false;

    bool operator==(TerrainParams &other) {
        return (memcmp(&seed, &other.seed, sizeof(TerrainParams)) == 0);
    }


    bool operator!=(TerrainParams &other) {
        return !this->operator==(other);
    }
};


class Terrain {
    struct TerrainParamsFreeze : TerrainParams {
        uint points_size = 0; //size*resolution

        TerrainParamsFreeze() : TerrainParams() {}

        explicit TerrainParamsFreeze(const TerrainParams &other) : TerrainParams::TerrainParams(other) {
            calculate();

        }

        void calculate() {
            max_height = max_height * resolution;
            points_size = size * resolution;

            perlin_intensity = 1 / perlin_intensity;

            smoothing_hydraulic_erosion_steps *= resolution;
            real_hydraulic_erosion_steps *= resolution;
            real_hydraulic_erosion_intensity *= resolution;
        }

        TerrainParamsFreeze &operator=(const TerrainParamsFreeze &other) = default;
    };

    const std::string FILE_HEADER = "TERRAIN";

    TerrainParamsFreeze frozen_params;

    uint **hmap = nullptr;

    template<typename type>
    type **new_map(uint map_size) {
        auto map = new type *[map_size];
        for (uint i = 0; i < map_size; i++) {
            map[i] = new type[map_size]{0};
        }
        return (map);
    }

    template<typename type>
    void delete_map(type **map, uint map_size) {
        if (map == nullptr) {
            return;
        }
        for (uint i = 0; i < map_size; i++) {
            delete[] map[i];
        }
        delete[] map;
    }

    //TODO beautify
    template<typename type>
    void noise_hmap(type **map, type max_height, uint iterations, float intensity, float in_freq, float in_amp,
                    uint map_seed) const {
        for (uint i = 0; i < frozen_params.points_size; i++) {
            for (uint j = 0; j < frozen_params.points_size; j++) {
                float x = i, y = j;
                x /= frozen_params.resolution, y /= frozen_params.resolution;
                float freq = in_freq;
                float amp = in_amp;
                float val = 0;
                for (int k = 0; k < iterations; k++) {//as variable
                    val += Noise::perlin_noise(x * freq / frozen_params.size + map_seed,
                                               y * freq / frozen_params.size + map_seed) * amp;
                    freq *= 2;
                    amp /= 2;
                }
                val = ((val + 1) / 2);
                val = std::pow(val, intensity);
                if (val > 1)//this is bad
                    val = 1;
                type res = (type) (val * max_height);
                if (res > max_height) {
                    res = max_height;
                }
                map[i][j] += res;


            }
        }
    }

    void asteroids() {
//Is this really necessary?
//I calculated asteroids size and it looks useless
    }

    void smoothing_hydraulic_erosion() {
        char directions[9][2] = {{0,  0},
                                 {-1, 0},
                                 {-1, 1},
                                 {0,  1},
                                 {1,  1},
                                 {1,  0},
                                 {1,  -1},
                                 {0,  -1},
                                 {-1, -1}};
        for (uint step = 0; step < frozen_params.smoothing_hydraulic_erosion_steps; step++) {
            for (uint i = 0; i < frozen_params.points_size; i++) {
                for (uint j = 0; j < frozen_params.points_size; j++) {
                    uint min = hmap[i][j];
                    char lowest_direction = 0;
                    for (char k = 0; k < 9; k++) {
                        uint x = i + directions[k][0];
                        uint y = j + directions[k][1];
                        if (x == UINT_MAX or x == frozen_params.points_size or y == UINT_MAX or
                            y == frozen_params.points_size) {
                            continue;
                        }
                        if (min > hmap[x][y]) {
                            min = hmap[x][y];
                            lowest_direction = k;
                        }
                    }

                    uint diff = hmap[i][j] - min;
                    hmap[i][j] -= (uint) (diff * frozen_params.smoothing_hydraulic_erosion_sub_intensity);
                    hmap[i + directions[lowest_direction][0]][j + directions[lowest_direction][1]] +=
                            (uint) (diff * frozen_params.smoothing_hydraulic_erosion_add_intensity);
                }
            }
        }
    }

    void real_hydraulic_erosion() {//TODO
        char directions[8][2] = {{-1, 0},
                                 {-1, 1},
                                 {0,  1},
                                 {1,  1},
                                 {1,  0},
                                 {1,  -1},
                                 {0,  -1},
                                 {-1, -1}};

        float m = frozen_params.real_hydraulic_erosion_fluidity_of_water;
        float n = frozen_params.real_hydraulic_erosion_soil_flowability;
        float k = pow(frozen_params.resolution, n) * pow(3.1415 / 2, n);

        SimpleRandom<int> rnd{frozen_params.seed, 1, 50000};
        uint water_seed = rnd.next();
        auto water = new_map<float>(frozen_params.points_size);
        for (uint step = 0; step < frozen_params.real_hydraulic_erosion_steps; step++) {
            noise_hmap<float>(water, frozen_params.real_hydraulic_erosion_intensity, 15, 1, 1, 1, water_seed);
            for (uint i = 0; i < frozen_params.points_size; i++) {
                for (uint j = 0; j < frozen_params.points_size; j++) {
                    std::shuffle(directions[0], directions[7], rnd.gen);//TODO i think it takes too long
                    for (char d = 0; d < 8; d++) {
                        uint x = i + directions[d][0];
                        uint y = j + directions[d][1];
                        if (x == UINT_MAX or x == frozen_params.points_size or y == UINT_MAX or
                            y == frozen_params.points_size or
                            hmap[i][j] <= hmap[x][y] or water[i][j] == 0) {
                            continue;
                        }//TODO Water should also flow away around the edges
                        uint diff = hmap[i][j] - hmap[x][y];
                        float slope = std::atan((float) diff);
                        float speed = std::pow(water[i][j], m) * std::pow(slope, n) / k;

                        //TODO now water carries too much
                        hmap[x][y] += (uint) (diff * speed * 0.1);
                        hmap[i][j] = std::max(hmap[i][j] - (uint) (diff * speed), hmap[x][y]);


                        diff = hmap[i][j] - hmap[x][y];
                        float all_water = water[x][y] + water[i][j];
                        if (all_water > diff) {
                            water[i][j] = (all_water - diff) / 2;
                            water[x][y] = diff + (all_water - diff) / 2;
                        } else {
                            water[x][y] = all_water;
                            water[i][j] = 0;
                        }
                        water[x][y] *= frozen_params.real_hydraulic_erosion_non_evaporation_of_water;
                        water[i][j] *= frozen_params.real_hydraulic_erosion_non_evaporation_of_water;
                    }


                }
            }
        }
        delete_map(water, frozen_params.points_size);
    }

    void coloring() {

    }

public:
    TerrainParams params;

    Terrain() = default;

    explicit Terrain(TerrainParams &in_params) {
        params = in_params;
    }

    ~Terrain() {
        delete_map(hmap, frozen_params.points_size);
    }

    void generate() {
        frozen_params = TerrainParamsFreeze(params);

        if (hmap == nullptr) {
            hmap = new_map<uint>(frozen_params.points_size);
        }
        noise_hmap<uint>(hmap, frozen_params.max_height, frozen_params.perlin_iterations,
                         frozen_params.perlin_intensity,
                         frozen_params.perlin_frequency,
                         frozen_params.perlin_amplitude,
                         frozen_params.seed);
        asteroids();
        real_hydraulic_erosion();
        smoothing_hydraulic_erosion();
        coloring();
    }

    void load_png(const std::string &filename);

    char export_file(const std::string &path,
                     bool force = false) {//status codes 0-OK, 1-BAD PATH, 2-NOT GENERATED, 3- NOT ACTUAL PARAMS, use force==true
        if (hmap == nullptr) {
            return 2;
        }
        if (static_cast<TerrainParams>(frozen_params) != params && !force) {
            return 3;
        }
        std::fstream file(path.c_str(), std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            return 1;
        }

        file << 'T';
        file.write(reinterpret_cast<const char *>(&frozen_params), sizeof(frozen_params));
        for (uint i = 0; i < frozen_params.points_size; i++) {
            file.write(reinterpret_cast<const char *>(hmap[i]), frozen_params.points_size * sizeof(uint));
        }
        file.close();
        return 0;
    }

    char import_file(const std::string &path) {//status codes 0-OK, 1-BAD PATH, 2-BAD FILE
        std::fstream file(path.c_str(), std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return 1;
        }
        char header;
        file >> header;

        if (header != 'T') {
            return 2;
        }

        delete_map(hmap, frozen_params.points_size);
        file.read(reinterpret_cast<char *>(&frozen_params), sizeof(frozen_params));
        frozen_params.calculate();
        hmap = new_map<uint>(frozen_params.points_size);
        for (uint i = 0; i < frozen_params.points_size; i++) {
            file.read(reinterpret_cast<char *>(hmap[i]), frozen_params.points_size * sizeof(uint));
        }
        return 0;
    }

    void export_png(const std::string &filename = "result.png");

    void cout_map() {//TODO fix before use
        float k = (float) frozen_params.max_height / 24;
        for (uint i = 0; i < frozen_params.points_size; i++) {
            for (uint j = 0; j < frozen_params.points_size; j++) {
                std::cout << (j == 0 ? "\n" : "") << "\033[38;5;" << 231 + (hmap[i][j] / k) << "m" << "█";

            }
        }
    }

    uint **get_map() {
        return hmap;
    }
};
