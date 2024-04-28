#ifndef generator_h
#define generator_h

#include "algorithm"
#include "random"
#include "iostream"
#include "cmath"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#endif

template<typename type>
class SimpleRandom {
    std::uniform_int_distribution<type> range;
    std::uniform_int_distribution<type> range_interval;

public:
    std::mt19937 gen;

    SimpleRandom(uint seed, type min, type max) {
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
        double x, y;

//        vec2 operator+(vec2 other) const {
//            return vec2{x + other.x, y + other.y};
//        }
//        vec2 operator*(double other) const{
//            return vec2{x*other, y*other};
//        }
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
        double rnd = a * (3.14159265 / ~(~0u >> 1));

        vec2 v{std::sin(rnd), v.y = std::cos(rnd)};

        return v;
    }

    static double distance_gradient_dot(int ix, int iy, double x, double y) {
        vec2 gradient = pseudo_random_gradient_vector(ix, iy);

        //distance
        double dist_x = x - ix;
        double dist_y = y - iy;

        return dist_x * gradient.x + dist_y * gradient.y;

    }

    static double interpolate(double a, double b, double t) {
        t = t * t * (3.0f - 2.0f * t);//3rd degree smoothstep
        return a + t * (b - a);//TODO ?
    }


public:
    static double perlin_noise(double x, double y) {
        int top_left_x = (int) x;
        int top_left_y = (int) y;


        double top_left_value = distance_gradient_dot(top_left_x, top_left_y, x, y);
        double top_right_value = distance_gradient_dot(top_left_x + 1, top_left_y, x, y);

        double low_left_value = distance_gradient_dot(top_left_x, top_left_y + 1, x, y);
        double low_right_value = distance_gradient_dot(top_left_x + 1, top_left_y + 1, x, y);

        double top = interpolate(top_left_value, top_right_value, x - top_left_x);
        double low = interpolate(low_left_value, low_right_value, x - top_left_x);
        double res = interpolate(top, low, y - top_left_y);
        return res;
    }

//    static double domain_distortion(double x, double y) {
//        double x1 = fbm(x, y);
//        double y1 = fbm(x + 5.2, y + 1.3);
//
//        double x2 = fbm(x + 4 * x1 + 1.7, x + 4 * y1 + 9.2);
//        double y2 = (x + 4 * x1 + 8.3, y + 4 * y1 + 2.8);
//
//        return fbm(x, y);
//    }

};


struct TerrainParams {
    uint seed;// [0:UINT_MAX]
    uint resolution;//limited by performance
    uint size;//limited by performance
    uint max_height = 2000;//11000 below sea level + 8000 above sea level = 19000 - earth maximum


    uint perlin_iterations = 12;//[0:20]
    double perlin_intensity = 0.9;//[0:1]
    double perlin_frequency = 4;//[0:5]
    double perlin_amplitude = 1;//[0:5]

    uint smoothing_hydraulic_erosion_steps = 10;//[0:100]
    double smoothing_hydraulic_erosion_sub_intensity = 0.5;//[0:1]
    double smoothing_hydraulic_erosion_add_intensity = 0.4;//[0:smoothing_hydraulic_erosion_sub_intensity]

    uint real_hydraulic_erosion_steps = 10;//[0:100]
    double real_hydraulic_erosion_intensity = 0.6;//precipitation per iteration, meters. [0:15] idk
    double real_hydraulic_erosion_fluidity_of_water = 0.5; //IDK how to call it better, lol. 0-1
    double real_hydraulic_erosion_soil_flowability = 1;// maybe [0:2], i will think about that one
    double real_hydraulic_erosion_non_evaporation_of_water = 0.5; //this one is funny too, also [0:1]

    bool cuda = false;

};


class Terrain {
    struct TerrainParamsFreeze : TerrainParams {
        uint points_size = 0; //size*resolution

        TerrainParamsFreeze() : TerrainParams() {}

        explicit TerrainParamsFreeze(const TerrainParams &other) : TerrainParams::TerrainParams(other) {
            max_height = max_height * resolution;
            points_size = size * resolution;

            perlin_intensity = 1 / perlin_intensity;

            smoothing_hydraulic_erosion_steps *= resolution;
            real_hydraulic_erosion_steps *= resolution;
            real_hydraulic_erosion_intensity *= resolution;
        }

        TerrainParamsFreeze &operator=(const TerrainParamsFreeze &other) = default;
    };

    TerrainParamsFreeze freezed_params;

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
        for (uint i = 0; i < map_size; i++) {
            delete[] map[i];
        }
        delete[] map;
    }

    //TODO beautify
    template<typename type>
    void noise_hmap(type **map, type max_height, uint iterations, double intensity, double in_freq, double in_amp,
                    uint map_seed) const {
        for (uint i = 0; i < freezed_params.points_size; i++) {//TODO
            for (uint j = 0; j < freezed_params.points_size; j++) {
                double x = i, y = j;
                x /= freezed_params.resolution, y /= freezed_params.resolution;
                double freq = in_freq;
                double amp = in_amp;
                double val = 0;
                for (int k = 0; k < iterations; k++) {//as variable
                    val += Noise::perlin_noise(x * freq / freezed_params.size + map_seed,
                                               y * freq / freezed_params.size + map_seed) * amp;
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
        for (uint step = 0; step < freezed_params.smoothing_hydraulic_erosion_steps; step++) {
            for (uint i = 0; i < freezed_params.points_size; i++) {
                for (uint j = 0; j < freezed_params.points_size; j++) {
                    uint min = hmap[i][j];
                    char lowest_direction = 0;
                    for (char k = 0; k < 9; k++) {
                        uint x = i + directions[k][0];
                        uint y = j + directions[k][1];
                        if (x == UINT_MAX or x == freezed_params.points_size or y == UINT_MAX or y == freezed_params.points_size) {
                            continue;
                        }
                        if (min > hmap[x][y]) {
                            min = hmap[x][y];
                            lowest_direction = k;
                        }
                    }

                    uint diff = hmap[i][j] - min;
                    hmap[i][j] -= (uint) (diff * freezed_params.smoothing_hydraulic_erosion_sub_intensity);
                    hmap[i + directions[lowest_direction][0]][j + directions[lowest_direction][1]] +=
                            (uint) (diff * freezed_params.smoothing_hydraulic_erosion_add_intensity);
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

        double m = freezed_params.real_hydraulic_erosion_fluidity_of_water;
        double n = freezed_params.real_hydraulic_erosion_soil_flowability;
        double k = pow(freezed_params.resolution, n) * pow(3.1415 / 2, n);

        SimpleRandom<uint> rnd{freezed_params.seed, 0, 50000};
        uint water_seed = rnd.next();
        auto water = new_map<double>(freezed_params.points_size);
        for (uint step = 0; step < freezed_params.real_hydraulic_erosion_steps; step++) {
            noise_hmap<double>(water, freezed_params.real_hydraulic_erosion_intensity, 15, 1, 1, 1, water_seed);
            for (uint i = 0; i < freezed_params.points_size; i++) {
                for (uint j = 0; j < freezed_params.points_size; j++) {
                    std::shuffle(directions[0], directions[7], rnd.gen);//TODO i think it takes too long
                    for (char d = 0; d < 8; d++) {
                        uint x = i + directions[d][0];
                        uint y = j + directions[d][1];
                        if (x == UINT_MAX or x == freezed_params.points_size or y == UINT_MAX or y == freezed_params.points_size or
                            hmap[i][j] <= hmap[x][y] or water[i][j] == 0) {
                            continue;
                        }//TODO Water should also flow away around the edges
                        uint diff = hmap[i][j] - hmap[x][y];
                        double slope = std::atan((double) diff);
                        double speed = std::pow(water[i][j], m) * std::pow(slope, n) / k;

                        //TODO now water carries too much
                        hmap[x][y] += (uint) (diff * speed * 0.1);
                        hmap[i][j] = std::max(hmap[i][j] - (uint) (diff * speed), hmap[x][y]);


                        diff = hmap[i][j] - hmap[x][y];
                        double all_water = water[x][y] + water[i][j];
                        if (all_water > diff) {
                            water[i][j] = (all_water - diff) / 2;
                            water[x][y] = diff + (all_water - diff) / 2;
                        } else {
                            water[x][y] = all_water;
                            water[i][j] = 0;
                        }
                        water[x][y] *= freezed_params.real_hydraulic_erosion_non_evaporation_of_water;
                        water[i][j] *= freezed_params.real_hydraulic_erosion_non_evaporation_of_water;
                    }


                }
            }
        }
        delete_map(water, freezed_params.points_size);
    }

    void coloring() {

    }

public:
    TerrainParams params;

    explicit Terrain(TerrainParams &in_params) {
        params = in_params;
    }

    ~Terrain() {
        delete_map(hmap, freezed_params.points_size);
    }

    void generate() {
        freezed_params = TerrainParamsFreeze(params);

        if (hmap == nullptr) {
            hmap = new_map<uint>(freezed_params.points_size);
        }
        noise_hmap<uint>(hmap, freezed_params.max_height, freezed_params.perlin_iterations,
                         freezed_params.perlin_intensity,
                         freezed_params.perlin_frequency,
                         freezed_params.perlin_amplitude,
                         freezed_params.seed);
        asteroids();
        real_hydraulic_erosion();
        smoothing_hydraulic_erosion();
        coloring();
    }

    void load_png(const std::string &filename) {
        hmap = new_map<uint>(freezed_params.points_size);

        int width;
        int height;
        int comp;
        unsigned char *image = stbi_load(filename.c_str(), &width, &height, &comp, 1);
        if (height != width or width != freezed_params.points_size) {
            throw std::runtime_error("Wrong image size");
        }

        for (uint i = 0; i < width; i++) {
            for (uint j = 0; j < width; j++) {
                hmap[i][j] = image[i * freezed_params.points_size + j] / CHAR_MAX * freezed_params.max_height;
            }
        }
    }

    char export_file(std::string path, bool force= false){//status codes 0-OK, 1-BAD PATH, 2-NOT GENERATED, 3- NOT ACTUAL PARAMS, use force==true

    }

    void export_png(const std::string &filename = "result.png") {
        if (freezed_params.points_size > INT_MAX) {
            throw std::range_error("Map size is too big");
        }
        auto out = new char[freezed_params.points_size * freezed_params.points_size];
        double k = (double) freezed_params.max_height / CHAR_MAX;
        for (uint i = 0; i < freezed_params.points_size; i++) {
            for (uint j = 0; j < freezed_params.points_size; j++) {
                out[i * freezed_params.points_size + j] = (char) (hmap[i][j] / k);
            }
        }

        stbi_write_png(filename.c_str(), (int) freezed_params.points_size, (int) freezed_params.points_size,
                       1, out, (int) freezed_params.points_size * 1);
    }

    void cout_map() {//TODO fix before use
        double k = (double) freezed_params.max_height / 24;
        for (uint i = 0; i < freezed_params.points_size; i++) {
            for (uint j = 0; j < freezed_params.points_size; j++) {
                std::cout << (j == 0 ? "\n" : "") << "\033[38;5;" << 231 + (hmap[i][j] / k) << "m" << "█";

            }
        }
    }

    uint **get_map() {
        return hmap;
    }
};
