#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "ImageRGBA.h"

#include <string>
#include <iostream>

#include <cstdlib>

struct ImageRGBA
{
    int width, height;
    unsigned char* data;
};

namespace ImageRGBA_
{
    ImageRGBA* create(int width, int height)
    {
        ImageRGBA* image = new ImageRGBA();
        image->width = width;
        image->height = height;
        image->data = (unsigned char*)malloc(width * height * 4 * sizeof(unsigned char));

        clear_with_color(*image, { 0, 0, 0, 255 });

        return image;
    }

    ImageRGBA* load(const char* filename)
    {
        ImageRGBA* image = new ImageRGBA();
        image->width = 0;
        image->height = 0;
        free(image->data);

        int channels = 0;

        image->data = stbi_load(filename, &image->width, &image->height, &channels, 4);

        if (!image->data)
        {
            return nullptr;
        }

        return image;
    }

    void free_image(ImageRGBA* image)
    {
        if (image->data)
        {
            stbi_image_free(image->data);
        }
    }

    int get_width(const ImageRGBA& image)
    {
        return image.width;
    }

    int get_height(const ImageRGBA& image)
    {
        return image.height;
    }


    bool set_pixel(ImageRGBA& image, int x, int y, const RGBA rgba)
    {
        if (x < 0 || x >= image.width || y < 0 || y >= image.height)
        {
            return false;
        }


        {
            int index = (y * image.width + x) * 4;
            image.data[index + 0] = rgba.r;
            image.data[index + 1] = rgba.g;
            image.data[index + 2] = rgba.b;
            image.data[index + 3] = rgba.a;
        }
        

        return true;
    }

    bool add_to_pixel(ImageRGBA& image, int x, int y, const RGBA rgba)
    {
        if (x < 0 || x >= image.width || y < 0 || y >= image.height)
        {
            return false;
        }


        {
            int index = (y * image.width + x) * 4;
            image.data[index + 0] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(rgba.r) + static_cast<int>(image.data[index + 0]))));
            image.data[index + 1] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(rgba.g) + static_cast<int>(image.data[index + 1]))));
            image.data[index + 2] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(rgba.b) + static_cast<int>(image.data[index + 2]))));
            image.data[index + 3] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(rgba.a) + static_cast<int>(image.data[index + 3]))));
        }
        

        return true;
    }

    bool mix_with_pixel(ImageRGBA& image, int x, int y, const RGBA rgba, float mixture_factor)
    {
        if (x < 0 || x >= image.width || y < 0 || y >= image.height)
        {
            return false;
        }

        {
            int index = (y * image.width + x) * 4;
            float inverse_mixture_factor = 1.0 - mixture_factor;
            image.data[index + 0] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>( static_cast<float>(rgba.r) * mixture_factor + static_cast<float>(image.data[index + 0]) * inverse_mixture_factor))));
            image.data[index + 1] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>( static_cast<float>(rgba.g) * mixture_factor + static_cast<float>(image.data[index + 1]) * inverse_mixture_factor))));
            image.data[index + 2] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(static_cast<float>(rgba.b) * mixture_factor + static_cast<float>(image.data[index + 2]) * inverse_mixture_factor))));
            image.data[index + 3] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(static_cast<float>(rgba.a) * mixture_factor + static_cast<float>(image.data[index + 3]) * inverse_mixture_factor))));
        }
    }

    RGBA get_pixel(const ImageRGBA& image, int x, int y)
    {
        if (x < 0 || x >= image.width || y < 0 || y >= image.height)
        {
            return RGBA(0, 0, 0, 255);
        }

        int index = (y * image.width + x) * 4;
        return RGBA(image.data[index + 0], image.data[index + 1], image.data[index + 2], image.data[index + 3]);
    }

    void save_png(const ImageRGBA& image, const char* filename)
    {
        stbi_write_png(filename, image.width, image.height, 4, image.data, image.width * 4);
    }

    void clear_with_color(ImageRGBA& image, RGBA color)
    {
        int num = image.width * image.height;

        for (int i = 0; i < num; i++)
        {
            int index = i * 4;
            image.data[index + 0] = color.r;
            image.data[index + 1] = color.g;
            image.data[index + 2] = color.b;
            image.data[index + 3] = color.a;
        }
    }

    void for_every_pixel(ImageRGBA& image, std::function<RGBA(int)> f)
    {
        int num = image.width * image.height;

        for (int i = 0; i < num; i++)
        {
            RGBA color = f(i);
            int index = i * 4;
            image.data[index + 0] = color.r;
            image.data[index + 1] = color.g;
            image.data[index + 2] = color.b;
            image.data[index + 3] = color.a;
        }
    }

    void for_every_pixel_UV(ImageRGBA& image, std::function<RGBA(RGBA, float u, float v)> f)
    {
        int width = image.width;
        int height = image.height;

        float inv_width = 1.0f / static_cast<float>(width);
        float inv_height = 1.0f / static_cast<float>(height);

        for (int iy = 0; iy < height; iy++)
        {
            for (int ix = 0; ix < width; ix++)
            {
                int index = (iy * width + ix) * 4;

                const RGBA rgba_read
                (
                    image.data[index + 0],
                    image.data[index + 1],
                    image.data[index + 2],
                    image.data[index + 3]
                );


                {
                    const float u = ix * inv_width;
                    const float v = iy * inv_height;

                    const RGBA rgba_write = f(rgba_read, u, v);

                    image.data[index + 0] = rgba_write.r;
                    image.data[index + 1] = rgba_write.g;
                    image.data[index + 2] = rgba_write.b;
                    image.data[index + 3] = rgba_write.a;
                }
                

                
            }
        }
    }

    bool are_images_same_size(const ImageRGBA& image_a, const ImageRGBA& image_b)
    {
        return ImageRGBA_::get_width(image_a) == ImageRGBA_::get_width(image_b) &&
            ImageRGBA_::get_height(image_a) == ImageRGBA_::get_height(image_b);
    }

    ImageRGBA* create_same_size_image(const ImageRGBA& image)
    {
        return ImageRGBA_::create(ImageRGBA_::get_width(image), ImageRGBA_::get_height(image));
    }

    void for_each_pixel_pair(const ImageRGBA* image_a, const ImageRGBA* image_b, std::function<void(const RGBA&, const RGBA&)> f)
    {
        const int width = ImageRGBA_::get_width(*image_a);
        const int height = ImageRGBA_::get_height(*image_b);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                f(ImageRGBA_::get_pixel(*image_a, x, y), ImageRGBA_::get_pixel(*image_b, x, y));
            }
        }
    }
}

namespace RGBA_
{
    void print(RGBA& rgba)
    {
        std::cout << "[" << static_cast<int>(rgba.r) << ", " << static_cast<int>(rgba.g) << ", " << static_cast<int>(rgba.b) << ", " << static_cast<int>(rgba.a) << "]";
    }

    RGBA mix(const RGBA& a, const RGBA& b, float factor)
    {
        float inv_factor = 1.0 - factor;
        return RGBA
        (
            static_cast<float>(std::fmin(255.0, static_cast<float>(a.r) * factor + static_cast<float>(b.r) * inv_factor)),
            static_cast<float>(std::fmin(255.0, static_cast<float>(a.g) * factor + static_cast<float>(b.g) * inv_factor)),
            static_cast<float>(std::fmin(255.0, static_cast<float>(a.b) * factor + static_cast<float>(b.b) * inv_factor)),
            static_cast<float>(std::fmin(255.0, static_cast<float>(a.a) * factor + static_cast<float>(b.a) * inv_factor))
        );
    }

    RGBA generate_random_color()
    {
        auto generate_random_float_0_to_1 = []()
        {
                // Seed the random number generator with the current time
                // std::srand(static_cast<unsigned>(std::time(nullptr)));
                // be awere when you reset the seed value

                // Generate a random float between 0.0 and 1.0
                return static_cast<float>(std::rand()) / RAND_MAX;
        };

        return RGBA(
            static_cast<uint8_t>(generate_random_float_0_to_1() * 255.0),
            static_cast<uint8_t>(generate_random_float_0_to_1() * 255.0),
            static_cast<uint8_t>(generate_random_float_0_to_1() * 255.0),
            255
        );
    }

    float color_brightnes_0_to_1(const RGBA color)
    {
        return static_cast<float>(static_cast<int>(color.r) + static_cast<int>(color.g) + static_cast<int>(color.b)) / (255.0f * 3.0f);
    }

    RGBA inverse_rgb(RGBA& color)
    {
        return RGBA(255 - color.r, 255 - color.g, 255 - color.b, color.a);
    }

    RGBA color_black()
    {
        return RGBA(0, 0, 0, 255);
    }

    RGBA color_white()
    {
        return RGBA(255, 255, 255, 255);
    }
}


bool operator==(const RGBA& lhs, const RGBA& rhs)
{
    return
        lhs.r == rhs.r &&
        lhs.g == rhs.g &&
        lhs.b == rhs.b &&
        lhs.a == rhs.a;
}


//void Image::save(const std::string& filename) const {
//    std::string extension = filename.substr(filename.find_last_of(".") + 1);
//    
//    else if (extension == "jpg" || extension == "jpeg") {
//        stbi_write_jpg(filename.c_str(), width, height, channels, data, 100);
//    }
//    else if (extension == "bmp") {
//        stbi_write_bmp(filename.c_str(), width, height, channels, data);
//    }
//    else if (extension == "tga") {
//        stbi_write_tga(filename.c_str(), width, height, channels, data);
//    }
//    else {
//        throw std::runtime_error("Unsupported file format: " + extension);
//    }
//}