#ifndef generator_h
#define generator_h

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
        std::mt19937 gen;
        std::uniform_int_distribution<type> range;
        std::uniform_int_distribution<type> range_interval;

    public:
        SimpleRandom(uint seed, type min, type max, type interval) {
            range = std::uniform_int_distribution<type>(min + interval, max);
            range_interval = std::uniform_int_distribution<type>(0, interval);
            gen = std::mt19937(seed);
        }

        type next() {
            return range(gen) - range_interval(gen);
        }
    };


    class PerlinNoise {
        struct vec2 {
            double x, y;
        };

        static vec2 pseudo_random_gradient_vector(int x, int y) {
            //TODO украл и мне не нравится, нужно переделать должна возвращать
            // случайный нормальный вектор, причем для одних и тех же x y один и тот же

            const unsigned w = 8 * sizeof(unsigned);
            const unsigned s = w / 2;
            unsigned a = x, b = y;
            a *= 3284157443;

            b ^= a << s | a >> (w - s);
            b *= 1911520717;

            a ^= b << s | b >> (w - s);
            a *= 2048419325;
            double rnd = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

            // Create the vector from the angle
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
        static double noise(double x, double y) {
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

    };

class Terrain {
    //all parameters list, will be changed
    uint seed;
    uint resolution;
    uint size;

    uint perlin_iterations;
    double perlin_intensity;
    double perlin_frequency;
    double perlin_amplitude;

    uint hydraulic_erosion_steps;//*resolution
    double hydraulic_erosion_sub_intensity = 0.5;
    double hydraulic_erosion_add_intensity = 0.4;

    bool cuda;
    static const uint MAX_HEIGHT = 4000; //метров
    uint max_height_points;

    uint hmap_size;
    uint **hmap = nullptr;

    template<typename type>
    type **new_map(uint map_size) {
        auto map = new type *[map_size];
        for (long i = 0; i <= map_size; i++) {
            map[i] = new type[map_size];
        }
        return (map);
    }

    void perlin_noise() {
        for (uint i = 0; i < hmap_size; i++) {
            for (uint j = 0; j < hmap_size; j++) {
                double dx = i, dy = j;
                dx /= resolution, dy /= resolution;
                double freq = perlin_frequency;
                double amp = perlin_amplitude;
                double val = 0;
                for (int k = 0; k < perlin_iterations; k++) {//as variable
                    val += PerlinNoise::noise(dx * freq / size + seed, dy * freq / size + seed) * amp;
                    freq *= 2;
                    amp /= 2;
                }
                val = ((val + 1) / 2);
                val = std::pow(val, perlin_intensity);
                if (val > 1)//this is bad
                    val = 1;
                hmap[i][j] += (uint) (val * max_height_points);
                if (hmap[i][j] > max_height_points) {
                    hmap[i][j] = max_height_points;
                }
            }
        }
    }

    void asteroids() {
        SimpleRandom<uint> rnd{seed, 0, size, 0};

    }

    void hydraulic_erosion() {
        char directions[9][2] = {{0,  0},
                                 {-1, 0},
                                 {-1, 1},
                                 {0,  1},
                                 {1,  1},
                                 {1,  0},
                                 {1,  -1},
                                 {0,  -1},
                                 {-1, -1}};
        for (uint step = 0; step < hydraulic_erosion_steps; step++) {
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
                    hmap[i][j] -= (uint) (diff * hydraulic_erosion_sub_intensity);
                    hmap[i + directions[lowest_direction][0]][j + directions[lowest_direction][1]] +=
                            (uint) (diff * hydraulic_erosion_add_intensity);
                }
            }
        }
    }

    void coloring() {

    }

public:
    Terrain(uint size, uint resolution, uint seed,
            double perlin_intensity = 1.2, double perlin_frequency = 4, double perlin_amplitude = 1,
            uint perlin_iterations = 10,
            uint hydraulic_erosion_iterations = 10, double hydraulic_erosion_sub_intensity = 0.5,
            double hydraulic_erosion_add_intensity = 0.4, uint max_height = MAX_HEIGHT,
            bool cuda = false) {

        this->resolution = resolution;
        this->size = size;
        this->seed = seed;

        this->perlin_iterations = perlin_iterations;
        this->perlin_intensity = 1 / perlin_intensity;
        this->perlin_frequency = perlin_frequency;
        this->perlin_amplitude = perlin_amplitude;

        this->hydraulic_erosion_steps = hydraulic_erosion_iterations * resolution;
        this->hydraulic_erosion_sub_intensity = hydraulic_erosion_sub_intensity;
        this->hydraulic_erosion_add_intensity = hydraulic_erosion_add_intensity;

        this->max_height_points = max_height * resolution;
        this->hmap_size = size * resolution;

        this->cuda = cuda;


    }

    void generate() {
        if (hmap == nullptr) {
            hmap = new_map<uint>(hmap_size);
        }
        perlin_noise();
        asteroids();
        hydraulic_erosion();
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
        uint k = max_height_points / CHAR_MAX;
        for (uint i = 0; i < hmap_size; i++) {
            for (uint j = 0; j < hmap_size; j++) {
                out[i * hmap_size + j] = (char) (hmap[i][j] / k);
//                std::cout << hmap[i][j] << ' ' << (int)out[i * hmap_size + j] << std::endl;
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

    uint** get_map(){
        return hmap;
    }
};