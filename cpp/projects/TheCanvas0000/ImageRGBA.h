#pragma once


#include <functional>

struct RGBA
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct ImageRGBA;

namespace ImageRGBA_
{
    ImageRGBA* create(int width, int height);
    ImageRGBA* load(const char* filename);
    void free_image(ImageRGBA* image);
    int get_width(const ImageRGBA& image);
    int get_height(const ImageRGBA& image);
    bool set_pixel(ImageRGBA& image, int x, int y, const RGBA rgba);
    bool add_to_pixel(ImageRGBA& image, int x, int y, const RGBA rgba);
    bool mix_with_pixel(ImageRGBA& image, int x, int y, const RGBA rgba, float mixture_factor);
    RGBA get_pixel(const ImageRGBA& image, int x, int y);
    void save_png(const ImageRGBA& image, const char* filename);
    void clear_with_color(ImageRGBA& image, RGBA color);
    void for_every_pixel(ImageRGBA& image, std::function<RGBA(int)> f);
    void for_every_pixel_UV(ImageRGBA& image, std::function<RGBA(RGBA, float u, float v)> f);
    bool are_images_same_size(const ImageRGBA& image_a, const ImageRGBA& image_b);
    ImageRGBA* create_same_size_image(const ImageRGBA& image);
    void for_each_pixel_pair(const ImageRGBA* image_a, const ImageRGBA* image_b, std::function<void(const RGBA&, const RGBA&)> f);
}


bool operator==(const RGBA& lhs, const RGBA& rhs);

namespace RGBA_
{
    void print(RGBA& rgba);
    RGBA mix(const RGBA& a, const RGBA& b, float factor);

    RGBA generate_random_color();
    
    float color_brightnes_0_to_1(const RGBA color);
    RGBA inverse_rgb(RGBA& color);

    RGBA color_black();
    RGBA color_white();
}



