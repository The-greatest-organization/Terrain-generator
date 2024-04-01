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

class Terrain {
    uint seed;
    uint resolution;
    uint size;

    uint perlin_iterations;
    double perlin_intensity;
    double perlin_frequency;
    double perlin_amplitude;

    uint smoothing_hydraulic_erosion_steps;//*resolution
    double smoothing_hydraulic_erosion_sub_intensity;
    double smoothing_hydraulic_erosion_add_intensity;

    uint real_hydraulic_erosion_steps;//*resolution
    double real_hydraulic_erosion_intensity;//meters per iteration
    double real_hydraulic_erosion_fluidity_of_water; //IDK how to call it better, lol
    double real_hydraulic_erosion_soil_flowability;
    double real_hydraulic_erosion_non_evaporation_of_water; //this one is funny too


    bool cuda;
    static const uint MAX_HEIGHT = 4000; //meters
    uint max_height_points;

    uint hmap_size;
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

    template<typename type>
    void noise_hmap(type **map, type max_height, uint iterations, double intensity, double in_freq, double in_amp,
                    uint map_seed) const {
        for (uint i = 0; i < hmap_size; i++) {//TODO
            for (uint j = 0; j < hmap_size; j++) {
                double x = i, y = j;
                x /= resolution, y /= resolution;
                double freq = in_freq;
                double amp = in_amp;
                double val = 0;
                for (int k = 0; k < iterations; k++) {//as variable
                    val += Noise::perlin_noise(x * freq / size + map_seed, y * freq / size + map_seed) * amp;
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
        for (uint step = 0; step < smoothing_hydraulic_erosion_steps; step++) {
            for (uint i = 0; i < hmap_size; i++) {
                for (uint j = 0; j < hmap_size; j++) {
                    uint min = hmap[i][j];
                    char lowest_direction = 0;
                    for (char k = 0; k < 9; k++) {
                        uint x = i + directions[k][0];
                        uint y = j + directions[k][1];
                        if (x == UINT_MAX or x == hmap_size or y == UINT_MAX or y == hmap_size) {
                            continue;
                        }
                        if (min > hmap[x][y]) {
                            min = hmap[x][y];
                            lowest_direction = k;
                        }
                    }

                    uint diff = hmap[i][j] - min;
                    hmap[i][j] -= (uint) (diff * smoothing_hydraulic_erosion_sub_intensity);
                    hmap[i + directions[lowest_direction][0]][j + directions[lowest_direction][1]] +=
                            (uint) (diff * smoothing_hydraulic_erosion_add_intensity);
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

        double m = real_hydraulic_erosion_fluidity_of_water;
        double n = real_hydraulic_erosion_soil_flowability;
        double k = pow(resolution, n) * pow(3.1415 / 2, n);

        SimpleRandom<uint> rnd{seed, 0, 50000};
        uint water_seed = rnd.next();
        auto water = new_map<double>(hmap_size);
        for (uint step = 0; step < real_hydraulic_erosion_steps; step++) {
            noise_hmap<double>(water, real_hydraulic_erosion_intensity, 15, 1, 1, 1, water_seed);
            for (uint i = 0; i < hmap_size; i++) {
                for (uint j = 0; j < hmap_size; j++) {
                    std::shuffle(directions[0], directions[7], rnd.gen);//TODO i think it takes too long
                    for (char d = 0; d < 8; d++) {
                        uint x = i + directions[d][0];
                        uint y = j + directions[d][1];
                        if (x == UINT_MAX or x == hmap_size or y == UINT_MAX or y == hmap_size or
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
                        water[x][y] *= real_hydraulic_erosion_non_evaporation_of_water;
                        water[i][j] *= real_hydraulic_erosion_non_evaporation_of_water;
                    }


                }
            }
        }
        delete_map(water, hmap_size);
    }

    void coloring() {

    }

public:
    Terrain(uint size, uint max_height, uint seed, uint resolution = 10,
            double noise_intensity = 1.2, double noise_frequency = 4,
            double noise_amplitude = 1, uint noise_iterations = 10,

            uint smoothing_hydraulic_erosion_steps = 10, double smoothing_hydraulic_erosion_sub_intensity = 0.5,
            double smoothing_hydraulic_erosion_add_intensity = 0.4,

            uint real_hydraulic_erosion_steps = 0, double real_hydraulic_erosion_intensity = 1,
            double real_hydraulic_erosion_fluidity_of_water = 0.8, double real_hydraulic_erosion_soil_flowability = 2,
            double real_hydraulic_erosion_non_evaporation_of_water = 0.5,

            bool cuda = false) {

        this->resolution = resolution;
        this->size = size;
        this->seed = seed;

        this->perlin_iterations = noise_iterations;
        this->perlin_intensity = 1 / noise_intensity;
        this->perlin_frequency = noise_frequency;
        this->perlin_amplitude = noise_amplitude;

        this->smoothing_hydraulic_erosion_steps = smoothing_hydraulic_erosion_steps * resolution;
        this->smoothing_hydraulic_erosion_sub_intensity = smoothing_hydraulic_erosion_sub_intensity;
        this->smoothing_hydraulic_erosion_add_intensity = smoothing_hydraulic_erosion_add_intensity;

        this->real_hydraulic_erosion_steps = real_hydraulic_erosion_steps * resolution;
        this->real_hydraulic_erosion_intensity = real_hydraulic_erosion_intensity * resolution;
        this->real_hydraulic_erosion_fluidity_of_water = real_hydraulic_erosion_fluidity_of_water;
        this->real_hydraulic_erosion_soil_flowability = real_hydraulic_erosion_soil_flowability;
        this->real_hydraulic_erosion_non_evaporation_of_water = real_hydraulic_erosion_non_evaporation_of_water;


        this->max_height_points = max_height * resolution;
        this->hmap_size = size * resolution;

        this->cuda = cuda;


    }

    ~Terrain() {
        delete_map(hmap, hmap_size);
    }

    void generate() {
        if (hmap == nullptr) {
            hmap = new_map<uint>(hmap_size);
        }
        noise_hmap<uint>(hmap, max_height_points, perlin_iterations, perlin_intensity, perlin_frequency,
                         perlin_amplitude,
                         seed);
        asteroids();
        real_hydraulic_erosion();
        smoothing_hydraulic_erosion();
        coloring();
    }

    void load_png(const std::string &filename) {
        hmap = new_map<uint>(hmap_size);

        int width;
        int height;
        int comp;
        unsigned char *image = stbi_load(filename.c_str(), &width, &height, &comp, 1);
        if (height != width or width != hmap_size) {
            throw std::runtime_error("Wrong image size");
        }

        for (uint i = 0; i < width; i++) {
            for (uint j = 0; j < width; j++) {
                hmap[i][j] = image[i * hmap_size + j] / CHAR_MAX * max_height_points;
            }
        }
    }

    void export_png(const std::string &filename = "result.png") {
        if (hmap_size > INT_MAX) {
            throw std::range_error("Map size is too big");
        }
        auto out = new char[hmap_size * hmap_size];
        double k = (double) max_height_points / CHAR_MAX;
        for (uint i = 0; i < hmap_size; i++) {
            for (uint j = 0; j < hmap_size; j++) {
                out[i * hmap_size + j] = (char) (hmap[i][j] / k);
            }
        }

        stbi_write_png(filename.c_str(), (int) hmap_size, (int) hmap_size,
                       1, out, (int) hmap_size * 1);
    }

    void cout_map() {//TODO fix before use
        double k = (double) max_height_points / 24;
        for (uint i = 0; i < hmap_size; i++) {
            for (uint j = 0; j < hmap_size; j++) {
                std::cout << (j == 0 ? "\n" : "") << "\033[38;5;" << 231 + (hmap[i][j] / k) << "m" << "█";

            }
        }
    }

    uint **get_map() {
        return hmap;
    }
};