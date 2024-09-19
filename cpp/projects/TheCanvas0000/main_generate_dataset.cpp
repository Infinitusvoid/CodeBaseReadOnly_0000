
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <fstream>
#include <numbers>
#include <cmath>
#include <filesystem>
#include <cstdint>

//#include "Rectangle2d.h"
namespace U
{
	namespace Rectangle2d_
	{
		struct Rectangle2d
		{
			float x_min;
			float x_max;
			float y_min;
			float y_max;
		};

		void create(Rectangle2d& rectangle, float position_x, float position_y, float width, float height);
		void calculate_coordinates(const Rectangle2d& rectangle, float x, float y, float& out_x, float& out_y);

		bool isRectangleInside(const Rectangle2d& inner, const Rectangle2d& outer);

		bool areRectanglesIntersecting(const Rectangle2d& rect1, const Rectangle2d& rect2, Rectangle2d& intersection);
		bool checkCollision(const Rectangle2d& rect1, const Rectangle2d& rect2, bool& out_side_x_min, bool& out_side_x_max, bool& out_side_y_min, bool& out_side_y_max);

		void scaleRectangle(Rectangle2d& rect, float scaleX, float scaleY);

		void translate(Rectangle2d& rect, float x, float y);

		float calculate_width(const Rectangle2d& rectangle);

		float calculate_height(const Rectangle2d& rectangle);

		void calculate_center(const Rectangle2d& rectangle, float& out_x, float& out_y);

		Rectangle2d generate_a_rectangle_inside_a_rectangle_with_the_procent_of_area(const Rectangle2d rectangle_outside, const float area_procent, const float offset_x_factor, const float offset_y_factor);

		void copy_values_from(const Rectangle2d& from, Rectangle2d& to);

		void calculate_the_rectangle_for_image_of_size(Rectangle2d& rectangle, const int width, const int height);

	}
}

namespace U
{
	namespace Rectangle2d_
	{
		void create(Rectangle2d& rectangle, float position_x, float position_y, float width, float height)
		{
			rectangle.x_min = position_x - width * 0.5f;
			rectangle.x_max = position_x + width * 0.5f;

			rectangle.y_min = position_y - width * 0.5f;
			rectangle.y_max = position_y + width * 0.5f;
		}

		void calculate_coordinates(const Rectangle2d& rectangle, float x, float y, float& out_x, float& out_y)
		{
			float rectangle_width = rectangle.x_max - rectangle.x_min;
			float rectangle_height = rectangle.y_max - rectangle.y_min;
			out_x = rectangle.x_min + x * rectangle_width;
			out_y = rectangle.y_min + y * rectangle_height;
		}

		bool isRectangleInside(const Rectangle2d& inner, const Rectangle2d& outer)
		{
			return (inner.x_min >= outer.x_min &&
				inner.x_max <= outer.x_max &&
				inner.y_min >= outer.y_min &&
				inner.y_max <= outer.y_max);
		}

		bool areRectanglesIntersecting(const Rectangle2d& rect1, const Rectangle2d& rect2, Rectangle2d& intersection)
		{
			// Calculate the intersection boundaries
			float x_min_intersection = std::max(rect1.x_min, rect2.x_min);
			float x_max_intersection = std::min(rect1.x_max, rect2.x_max);
			float y_min_intersection = std::max(rect1.y_min, rect2.y_min);
			float y_max_intersection = std::min(rect1.y_max, rect2.y_max);

			// Check if there is an intersection
			if (x_min_intersection < x_max_intersection && y_min_intersection < y_max_intersection) {
				// Set the intersection rectangle
				intersection.x_min = x_min_intersection;
				intersection.x_max = x_max_intersection;
				intersection.y_min = y_min_intersection;
				intersection.y_max = y_max_intersection;
				return true;
			}
			else {
				// No intersection
				return false;
			}
		}

		// Function to check for collision and return the sides of collision
		bool checkCollision(const Rectangle2d& rect1, const Rectangle2d& rect2, bool& out_side_x_min, bool& out_side_x_max, bool& out_side_y_min, bool& out_side_y_max)
		{
			out_side_x_min = false;
			out_side_x_max = false;
			out_side_y_min = false;
			out_side_y_max = false;

			// Check if there is a collision
			bool isColliding = !(rect1.x_max < rect2.x_min || rect1.x_min > rect2.x_max ||
				rect1.y_max < rect2.y_min || rect1.y_min > rect2.y_max);

			if (isColliding)
			{
				// Determine the sides of collision
				if (rect1.x_max > rect2.x_min && rect1.x_min < rect2.x_min)
				{
					// collisionSides.push_back("side_x_min");
					out_side_x_min = true;

				}
				if (rect1.x_min < rect2.x_max && rect1.x_max > rect2.x_max)
				{
					// collisionSides.push_back("side_x_max");
					out_side_x_max = true;
				}
				if (rect1.y_max > rect2.y_min && rect1.y_min < rect2.y_min)
				{
					// collisionSides.push_back("side_y_min");
					out_side_y_min = true;
				}
				if (rect1.y_min < rect2.y_max && rect1.y_max > rect2.y_max)
				{
					// collisionSides.push_back("side_y_max");
					out_side_y_max = true;
				}


				return true;
			}

			return false;
		}

		void scaleRectangle(Rectangle2d& rect, float scaleX, float scaleY)
		{
			float width = rect.x_max - rect.x_min;
			float height = rect.y_max - rect.y_min;

			float centerX = rect.x_min + width / 2.0f;
			float centerY = rect.y_min + height / 2.0f;

			float newWidth = width * scaleX;
			float newHeight = height * scaleY;

			rect.x_min = centerX - newWidth / 2.0f;
			rect.x_max = centerX + newWidth / 2.0f;
			rect.y_min = centerY - newHeight / 2.0f;
			rect.y_max = centerY + newHeight / 2.0f;
		}

		void translate(Rectangle2d& rect, float x, float y)
		{
			rect.x_min += x;
			rect.x_max += x;

			rect.y_min += y;
			rect.y_max += y;
		}

		float calculate_width(const Rectangle2d& rectangle)
		{
			return std::fabsf(rectangle.x_max - rectangle.x_min);
		}

		float calculate_height(const Rectangle2d& rectangle)
		{
			return std::fabsf(rectangle.y_max - rectangle.y_min);
		}

		void calculate_center(const Rectangle2d& rectangle, float& out_x, float& out_y)
		{
			out_x = (rectangle.x_max + rectangle.x_min) * 0.5f;
			out_y = (rectangle.y_max + rectangle.y_min) * 0.5f;
		}

		Rectangle2d generate_a_rectangle_inside_a_rectangle_with_the_procent_of_area(const Rectangle2d rectangle_outside, const float area_procent, const float offset_x_factor, const float offset_y_factor)
		{
			Rectangle2d_::Rectangle2d rectangle_output;

			float factor = 1.0 - area_procent;

			float width = calculate_width(rectangle_outside);
			float height = calculate_height(rectangle_outside);

			rectangle_output.x_min = rectangle_outside.x_min + width * factor;
			rectangle_output.x_max = rectangle_outside.x_max - width * factor;

			rectangle_output.y_min = rectangle_outside.y_min + height * factor;
			rectangle_output.y_max = rectangle_outside.y_max - height * factor;

			float offset_max_x = (rectangle_output.x_min - rectangle_outside.x_min);
			float offset_max_y = (rectangle_output.y_min - rectangle_outside.y_min);

			Rectangle2d_::translate(rectangle_output, offset_max_x * offset_x_factor, offset_max_y * offset_y_factor);


			return rectangle_output;
		}

		void copy_values_from(const Rectangle2d& from, Rectangle2d& to)
		{
			to.x_min = from.x_min;
			to.x_max = from.x_max;

			to.y_min = from.y_min;
			to.y_max = from.y_max;
		}

		void calculate_the_rectangle_for_image_of_size(Rectangle2d& rectangle, const int width, const int height)
		{
			if (width > height)
			{
				rectangle.x_min = 0.0f;
				rectangle.x_max = static_cast<float>(width) / static_cast<float>(height);
				rectangle.y_min = 0.0f;
				rectangle.y_max = 1.0f;
			}
			else
			{
				rectangle.x_min = 0.0f;
				rectangle.x_max = 1.0f;
				rectangle.y_min = 0.0f;
				rectangle.y_max = static_cast<float>(height) / static_cast<float>(width);
			}

		}
	}
}

// #include "Random.h"
namespace U
{
	namespace Random
	{
		float generate_random_float_0_to_1();
		float generate_random_float_minus_one_to_plus_one();
	}
}

namespace U
{
	namespace Random
	{
		//#include <cstdlib>
		//#include <ctime>

		float generate_random_float_0_to_1()
		{
			// Seed the random number generator with the current time
			//std::srand(static_cast<unsigned>(std::time(nullptr)));
			// be awere when you reset the seed value

			// Generate a random float between 0.0 and 1.0
			return static_cast<float>(std::rand()) / RAND_MAX;
		}

		float generate_random_float_minus_one_to_plus_one()
		{
			return (generate_random_float_0_to_1() - 0.5f) * 2.0f;
		}
	}
}

// #include "ImageRGBA.h"
namespace U
{

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



}

namespace U
{
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
/* stb_image - v2.27 - public domain image loader - http://nothings.org/stb
                                  no warranty implied; use at your own risk

   Do this:
      #define STB_IMAGE_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.

   // i.e. it should look like this:
   #include ...
   #include ...
   #include ...
   #define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"

   You can #define STBI_ASSERT(x) before the #include to avoid using assert.h.
   And #define STBI_MALLOC, STBI_REALLOC, and STBI_FREE to avoid using malloc,realloc,free


   QUICK NOTES:
      Primarily of interest to game developers and other people who can
          avoid problematic images and only need the trivial interface

      JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
      PNG 1/2/4/8/16-bit-per-channel

      TGA (not sure what subset, if a subset)
      BMP non-1bpp, non-RLE
      PSD (composited view only, no extra channels, 8/16 bit-per-channel)

      GIF (*comp always reports as 4-channel)
      HDR (radiance rgbE format)
      PIC (Softimage PIC)
      PNM (PPM and PGM binary only)

      Animated GIF still needs a proper API, but here's one way to do it:
          http://gist.github.com/urraka/685d9a6340b26b830d49

      - decode from memory or through FILE (define STBI_NO_STDIO to remove code)
      - decode from arbitrary I/O callbacks
      - SIMD acceleration on x86/x64 (SSE2) and ARM (NEON)

   Full documentation under "DOCUMENTATION" below.


LICENSE

  See end of file for license information.

RECENT REVISION HISTORY:

      2.27  (2021-07-11) document stbi_info better, 16-bit PNM support, bug fixes
      2.26  (2020-07-13) many minor fixes
      2.25  (2020-02-02) fix warnings
      2.24  (2020-02-02) fix warnings; thread-local failure_reason and flip_vertically
      2.23  (2019-08-11) fix clang static analysis warning
      2.22  (2019-03-04) gif fixes, fix warnings
      2.21  (2019-02-25) fix typo in comment
      2.20  (2019-02-07) support utf8 filenames in Windows; fix warnings and platform ifdefs
      2.19  (2018-02-11) fix warning
      2.18  (2018-01-30) fix warnings
      2.17  (2018-01-29) bugfix, 1-bit BMP, 16-bitness query, fix warnings
      2.16  (2017-07-23) all functions have 16-bit variants; optimizations; bugfixes
      2.15  (2017-03-18) fix png-1,2,4; all Imagenet JPGs; no runtime SSE detection on GCC
      2.14  (2017-03-03) remove deprecated STBI_JPEG_OLD; fixes for Imagenet JPGs
      2.13  (2016-12-04) experimental 16-bit API, only for PNG so far; fixes
      2.12  (2016-04-02) fix typo in 2.11 PSD fix that caused crashes
      2.11  (2016-04-02) 16-bit PNGS; enable SSE2 in non-gcc x64
                         RGB-format JPEG; remove white matting in PSD;
                         allocate large structures on the stack;
                         correct channel count for PNG & BMP
      2.10  (2016-01-22) avoid warning introduced in 2.09
      2.09  (2016-01-16) 16-bit TGA; comments in PNM files; STBI_REALLOC_SIZED

   See end of file for full revision history.


 ============================    Contributors    =========================

 Image formats                          Extensions, features
    Sean Barrett (jpeg, png, bmp)          Jetro Lauha (stbi_info)
    Nicolas Schulz (hdr, psd)              Martin "SpartanJ" Golini (stbi_info)
    Jonathan Dummer (tga)                  James "moose2000" Brown (iPhone PNG)
    Jean-Marc Lienher (gif)                Ben "Disch" Wenger (io callbacks)
    Tom Seddon (pic)                       Omar Cornut (1/2/4-bit PNG)
    Thatcher Ulrich (psd)                  Nicolas Guillemot (vertical flip)
    Ken Miller (pgm, ppm)                  Richard Mitton (16-bit PSD)
    github:urraka (animated gif)           Junggon Kim (PNM comments)
    Christopher Forseth (animated gif)     Daniel Gibson (16-bit TGA)
                                           socks-the-fox (16-bit PNG)
                                           Jeremy Sawicki (handle all ImageNet JPGs)
 Optimizations & bugfixes                  Mikhail Morozov (1-bit BMP)
    Fabian "ryg" Giesen                    Anael Seghezzi (is-16-bit query)
    Arseny Kapoulkine                      Simon Breuss (16-bit PNM)
    John-Mark Allen
    Carmelo J Fdez-Aguera

 Bug & warning fixes
    Marc LeBlanc            David Woo          Guillaume George     Martins Mozeiko
    Christpher Lloyd        Jerry Jansson      Joseph Thomson       Blazej Dariusz Roszkowski
    Phil Jordan                                Dave Moore           Roy Eltham
    Hayaki Saito            Nathan Reed        Won Chun
    Luke Graham             Johan Duparc       Nick Verigakis       the Horde3D community
    Thomas Ruf              Ronny Chevalier                         github:rlyeh
    Janez Zemva             John Bartholomew   Michal Cichon        github:romigrou
    Jonathan Blow           Ken Hamada         Tero Hanninen        github:svdijk
    Eugene Golushkov        Laurent Gomila     Cort Stratton        github:snagar
    Aruelien Pocheville     Sergio Gonzalez    Thibault Reuille     github:Zelex
    Cass Everitt            Ryamond Barbiero                        github:grim210
    Paul Du Bois            Engin Manap        Aldo Culquicondor    github:sammyhw
    Philipp Wiesemann       Dale Weiler        Oriol Ferrer Mesia   github:phprus
    Josh Tobin                                 Matthew Gregan       github:poppolopoppo
    Julian Raschke          Gregory Mullen     Christian Floisand   github:darealshinji
    Baldur Karlsson         Kevin Schmidt      JR Smith             github:Michaelangel007
                            Brad Weinberger    Matvey Cherevko      github:mosra
    Luca Sas                Alexander Veselov  Zack Middleton       [reserved]
    Ryan C. Gordon          [reserved]                              [reserved]
                     DO NOT ADD YOUR NAME HERE

                     Jacko Dirks

  To add your name to the credits, pick a random blank space in the middle and fill it.
  80% of merge conflicts on stb PRs are due to people adding their name at the end
  of the credits.
*/

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

// DOCUMENTATION
//
// Limitations:
//    - no 12-bit-per-channel JPEG
//    - no JPEGs with arithmetic coding
//    - GIF always returns *comp=4
//
// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data)
//
// Standard parameters:
//    int *x                 -- outputs image width in pixels
//    int *y                 -- outputs image height in pixels
//    int *channels_in_file  -- outputs # of image components in image file
//    int desired_channels   -- if non-zero, # of image components requested in result
//
// The return value from an image loader is an 'unsigned char *' which points
// to the pixel data, or NULL on an allocation failure or if the image is
// corrupt or invalid. The pixel data consists of *y scanlines of *x pixels,
// with each pixel consisting of N interleaved 8-bit components; the first
// pixel pointed to is top-left-most in the image. There is no padding between
// image scanlines or between pixels, regardless of format. The number of
// components N is 'desired_channels' if desired_channels is non-zero, or
// *channels_in_file otherwise. If desired_channels is non-zero,
// *channels_in_file has the number of components that _would_ have been
// output otherwise. E.g. if you set desired_channels to 4, you will always
// get RGBA output, but you can check *channels_in_file to see if it's trivially
// opaque because e.g. there were only 3 channels in the source image.
//
// An output image with N components has the following components interleaved
// in this order in each pixel:
//
//     N=#comp     components
//       1           grey
//       2           grey, alpha
//       3           red, green, blue
//       4           red, green, blue, alpha
//
// If image loading fails for any reason, the return value will be NULL,
// and *x, *y, *channels_in_file will be unchanged. The function
// stbi_failure_reason() can be queried for an extremely brief, end-user
// unfriendly explanation of why the load failed. Define STBI_NO_FAILURE_STRINGS
// to avoid compiling these strings at all, and STBI_FAILURE_USERMSG to get slightly
// more user-friendly ones.
//
// Paletted PNG, BMP, GIF, and PIC images are automatically depalettized.
//
// To query the width, height and component count of an image without having to
// decode the full file, you can use the stbi_info family of functions:
//
//   int x,y,n,ok;
//   ok = stbi_info(filename, &x, &y, &n);
//   // returns ok=1 and sets x, y, n if image is a supported format,
//   // 0 otherwise.
//
// Note that stb_image pervasively uses ints in its public API for sizes,
// including sizes of memory buffers. This is now part of the API and thus
// hard to change without causing breakage. As a result, the various image
// loaders all have certain limits on image size; these differ somewhat
// by format but generally boil down to either just under 2GB or just under
// 1GB. When the decoded image would be larger than this, stb_image decoding
// will fail.
//
// Additionally, stb_image will reject image files that have any of their
// dimensions set to a larger value than the configurable STBI_MAX_DIMENSIONS,
// which defaults to 2**24 = 16777216 pixels. Due to the above memory limit,
// the only way to have an image with such dimensions load correctly
// is for it to have a rather extreme aspect ratio. Either way, the
// assumption here is that such larger images are likely to be malformed
// or malicious. If you do need to load an image with individual dimensions
// larger than that, and it still fits in the overall size limit, you can
// #define STBI_MAX_DIMENSIONS on your own to be something larger.
//
// ===========================================================================
//
// UNICODE:
//
//   If compiling for Windows and you wish to use Unicode filenames, compile
//   with
//       #define STBI_WINDOWS_UTF8
//   and pass utf8-encoded filenames. Call stbi_convert_wchar_to_utf8 to convert
//   Windows wchar_t filenames to utf8.
//
// ===========================================================================
//
// Philosophy
//
// stb libraries are designed with the following priorities:
//
//    1. easy to use
//    2. easy to maintain
//    3. good performance
//
// Sometimes I let "good performance" creep up in priority over "easy to maintain",
// and for best performance I may provide less-easy-to-use APIs that give higher
// performance, in addition to the easy-to-use ones. Nevertheless, it's important
// to keep in mind that from the standpoint of you, a client of this library,
// all you care about is #1 and #3, and stb libraries DO NOT emphasize #3 above all.
//
// Some secondary priorities arise directly from the first two, some of which
// provide more explicit reasons why performance can't be emphasized.
//
//    - Portable ("ease of use")
//    - Small source code footprint ("easy to maintain")
//    - No dependencies ("ease of use")
//
// ===========================================================================
//
// I/O callbacks
//
// I/O callbacks allow you to read from arbitrary sources, like packaged
// files or some other source. Data read from callbacks are processed
// through a small internal buffer (currently 128 bytes) to try to reduce
// overhead.
//
// The three functions you must define are "read" (reads some bytes of data),
// "skip" (skips some bytes of data), "eof" (reports if the stream is at the end).
//
// ===========================================================================
//
// SIMD support
//
// The JPEG decoder will try to automatically use SIMD kernels on x86 when
// supported by the compiler. For ARM Neon support, you must explicitly
// request it.
//
// (The old do-it-yourself SIMD API is no longer supported in the current
// code.)
//
// On x86, SSE2 will automatically be used when available based on a run-time
// test; if not, the generic C versions are used as a fall-back. On ARM targets,
// the typical path is to have separate builds for NEON and non-NEON devices
// (at least this is true for iOS and Android). Therefore, the NEON support is
// toggled by a build flag: define STBI_NEON to get NEON loops.
//
// If for some reason you do not want to use any of SIMD code, or if
// you have issues compiling it, you can disable it entirely by
// defining STBI_NO_SIMD.
//
// ===========================================================================
//
// HDR image support   (disable by defining STBI_NO_HDR)
//
// stb_image supports loading HDR images in general, and currently the Radiance
// .HDR file format specifically. You can still load any file through the existing
// interface; if you attempt to load an HDR file, it will be automatically remapped
// to LDR, assuming gamma 2.2 and an arbitrary scale factor defaulting to 1;
// both of these constants can be reconfigured through this interface:
//
//     stbi_hdr_to_ldr_gamma(2.2f);
//     stbi_hdr_to_ldr_scale(1.0f);
//
// (note, do not use _inverse_ constants; stbi_image will invert them
// appropriately).
//
// Additionally, there is a new, parallel interface for loading files as
// (linear) floats to preserve the full dynamic range:
//
//    float *data = stbi_loadf(filename, &x, &y, &n, 0);
//
// If you load LDR images through this interface, those images will
// be promoted to floating point values, run through the inverse of
// constants corresponding to the above:
//
//     stbi_ldr_to_hdr_scale(1.0f);
//     stbi_ldr_to_hdr_gamma(2.2f);
//
// Finally, given a filename (or an open file or memory block--see header
// file for details) containing image data, you can query for the "most
// appropriate" interface to use (that is, whether the image is HDR or
// not), using:
//
//     stbi_is_hdr(char *filename);
//
// ===========================================================================
//
// iPhone PNG support:
//
// We optionally support converting iPhone-formatted PNGs (which store
// premultiplied BGRA) back to RGB, even though they're internally encoded
// differently. To enable this conversion, call
// stbi_convert_iphone_png_to_rgb(1).
//
// Call stbi_set_unpremultiply_on_load(1) as well to force a divide per
// pixel to remove any premultiplied alpha *only* if the image file explicitly
// says there's premultiplied data (currently only happens in iPhone images,
// and only if iPhone convert-to-rgb processing is on).
//
// ===========================================================================
//
// ADDITIONAL CONFIGURATION
//
//  - You can suppress implementation of any of the decoders to reduce
//    your code footprint by #defining one or more of the following
//    symbols before creating the implementation.
//
//        STBI_NO_JPEG
//        STBI_NO_PNG
//        STBI_NO_BMP
//        STBI_NO_PSD
//        STBI_NO_TGA
//        STBI_NO_GIF
//        STBI_NO_HDR
//        STBI_NO_PIC
//        STBI_NO_PNM   (.ppm and .pgm)
//
//  - You can request *only* certain decoders and suppress all other ones
//    (this will be more forward-compatible, as addition of new decoders
//    doesn't require you to disable them explicitly):
//
//        STBI_ONLY_JPEG
//        STBI_ONLY_PNG
//        STBI_ONLY_BMP
//        STBI_ONLY_PSD
//        STBI_ONLY_TGA
//        STBI_ONLY_GIF
//        STBI_ONLY_HDR
//        STBI_ONLY_PIC
//        STBI_ONLY_PNM   (.ppm and .pgm)
//
//   - If you use STBI_NO_PNG (or _ONLY_ without PNG), and you still
//     want the zlib decoder to be available, #define STBI_SUPPORT_ZLIB
//
//  - If you define STBI_MAX_DIMENSIONS, stb_image will reject images greater
//    than that size (in either width or height) without further processing.
//    This is to let programs in the wild set an upper bound to prevent
//    denial-of-service attacks on untrusted data, as one could generate a
//    valid image of gigantic dimensions and force stb_image to allocate a
//    huge block of memory and spend disproportionate time decoding it. By
//    default this is set to (1 << 24), which is 16777216, but that's still
//    very big.

#ifndef STBI_NO_STDIO
#include <stdio.h>
#endif // STBI_NO_STDIO

#define STBI_VERSION 1

    enum
    {
        STBI_default = 0, // only used for desired_channels

        STBI_grey = 1,
        STBI_grey_alpha = 2,
        STBI_rgb = 3,
        STBI_rgb_alpha = 4
    };

#include <stdlib.h>
    typedef unsigned char stbi_uc;
    typedef unsigned short stbi_us;

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef STBIDEF
#ifdef STB_IMAGE_STATIC
#define STBIDEF static
#else
#define STBIDEF extern
#endif
#endif

        //////////////////////////////////////////////////////////////////////////////
        //
        // PRIMARY API - works on images of any type
        //

        //
        // load image by filename, open file, or memory buffer
        //

        typedef struct
        {
            int      (*read)  (void* user, char* data, int size);   // fill 'data' with 'size' bytes.  return number of bytes actually read
            void     (*skip)  (void* user, int n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
            int      (*eof)   (void* user);                       // returns nonzero if we are at end of file/data
        } stbi_io_callbacks;

        ////////////////////////////////////
        //
        // 8-bits-per-channel interface
        //

        STBIDEF stbi_uc* stbi_load_from_memory(stbi_uc           const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels);
        STBIDEF stbi_uc* stbi_load_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);

#ifndef STBI_NO_STDIO
        STBIDEF stbi_uc* stbi_load(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
        STBIDEF stbi_uc* stbi_load_from_file(FILE* f, int* x, int* y, int* channels_in_file, int desired_channels);
        // for stbi_load_from_file, file pointer is left pointing immediately after image
#endif

#ifndef STBI_NO_GIF
        STBIDEF stbi_uc* stbi_load_gif_from_memory(stbi_uc const* buffer, int len, int** delays, int* x, int* y, int* z, int* comp, int req_comp);
#endif

#ifdef STBI_WINDOWS_UTF8
        STBIDEF int stbi_convert_wchar_to_utf8(char* buffer, size_t bufferlen, const wchar_t* input);
#endif

        ////////////////////////////////////
        //
        // 16-bits-per-channel interface
        //

        STBIDEF stbi_us* stbi_load_16_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels);
        STBIDEF stbi_us* stbi_load_16_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);

#ifndef STBI_NO_STDIO
        STBIDEF stbi_us* stbi_load_16(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
        STBIDEF stbi_us* stbi_load_from_file_16(FILE* f, int* x, int* y, int* channels_in_file, int desired_channels);
#endif

        ////////////////////////////////////
        //
        // float-per-channel interface
        //
#ifndef STBI_NO_LINEAR
        STBIDEF float* stbi_loadf_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels);
        STBIDEF float* stbi_loadf_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels);

#ifndef STBI_NO_STDIO
        STBIDEF float* stbi_loadf(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
        STBIDEF float* stbi_loadf_from_file(FILE* f, int* x, int* y, int* channels_in_file, int desired_channels);
#endif
#endif

#ifndef STBI_NO_HDR
        STBIDEF void   stbi_hdr_to_ldr_gamma(float gamma);
        STBIDEF void   stbi_hdr_to_ldr_scale(float scale);
#endif // STBI_NO_HDR

#ifndef STBI_NO_LINEAR
        STBIDEF void   stbi_ldr_to_hdr_gamma(float gamma);
        STBIDEF void   stbi_ldr_to_hdr_scale(float scale);
#endif // STBI_NO_LINEAR

        // stbi_is_hdr is always defined, but always returns false if STBI_NO_HDR
        STBIDEF int    stbi_is_hdr_from_callbacks(stbi_io_callbacks const* clbk, void* user);
        STBIDEF int    stbi_is_hdr_from_memory(stbi_uc const* buffer, int len);
#ifndef STBI_NO_STDIO
        STBIDEF int      stbi_is_hdr(char const* filename);
        STBIDEF int      stbi_is_hdr_from_file(FILE* f);
#endif // STBI_NO_STDIO


        // get a VERY brief reason for failure
        // on most compilers (and ALL modern mainstream compilers) this is threadsafe
        STBIDEF const char* stbi_failure_reason(void);

        // free the loaded image -- this is just free()
        STBIDEF void     stbi_image_free(void* retval_from_stbi_load);

        // get image dimensions & components without fully decoding
        STBIDEF int      stbi_info_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp);
        STBIDEF int      stbi_info_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* comp);
        STBIDEF int      stbi_is_16_bit_from_memory(stbi_uc const* buffer, int len);
        STBIDEF int      stbi_is_16_bit_from_callbacks(stbi_io_callbacks const* clbk, void* user);

#ifndef STBI_NO_STDIO
        STBIDEF int      stbi_info(char const* filename, int* x, int* y, int* comp);
        STBIDEF int      stbi_info_from_file(FILE* f, int* x, int* y, int* comp);
        STBIDEF int      stbi_is_16_bit(char const* filename);
        STBIDEF int      stbi_is_16_bit_from_file(FILE* f);
#endif



        // for image formats that explicitly notate that they have premultiplied alpha,
        // we just return the colors as stored in the file. set this flag to force
        // unpremultiplication. results are undefined if the unpremultiply overflow.
        STBIDEF void stbi_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply);

        // indicate whether we should process iphone images back to canonical format,
        // or just pass them through "as-is"
        STBIDEF void stbi_convert_iphone_png_to_rgb(int flag_true_if_should_convert);

        // flip the image vertically, so the first pixel in the output array is the bottom left
        STBIDEF void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);

        // as above, but only applies to images loaded on the thread that calls the function
        // this function is only available if your compiler supports thread-local variables;
        // calling it will fail to link if your compiler doesn't
        STBIDEF void stbi_set_unpremultiply_on_load_thread(int flag_true_if_should_unpremultiply);
        STBIDEF void stbi_convert_iphone_png_to_rgb_thread(int flag_true_if_should_convert);
        STBIDEF void stbi_set_flip_vertically_on_load_thread(int flag_true_if_should_flip);

        // ZLIB client - used by PNG, available for other purposes

        STBIDEF char* stbi_zlib_decode_malloc_guesssize(const char* buffer, int len, int initial_size, int* outlen);
        STBIDEF char* stbi_zlib_decode_malloc_guesssize_headerflag(const char* buffer, int len, int initial_size, int* outlen, int parse_header);
        STBIDEF char* stbi_zlib_decode_malloc(const char* buffer, int len, int* outlen);
        STBIDEF int   stbi_zlib_decode_buffer(char* obuffer, int olen, const char* ibuffer, int ilen);

        STBIDEF char* stbi_zlib_decode_noheader_malloc(const char* buffer, int len, int* outlen);
        STBIDEF int   stbi_zlib_decode_noheader_buffer(char* obuffer, int olen, const char* ibuffer, int ilen);


#ifdef __cplusplus
    }
#endif

    //
    //
    ////   end header file   /////////////////////////////////////////////////////
#endif // STBI_INCLUDE_STB_IMAGE_H

#ifdef STB_IMAGE_IMPLEMENTATION

#if defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
#ifndef STBI_ONLY_JPEG
#define STBI_NO_JPEG
#endif
#ifndef STBI_ONLY_PNG
#define STBI_NO_PNG
#endif
#ifndef STBI_ONLY_BMP
#define STBI_NO_BMP
#endif
#ifndef STBI_ONLY_PSD
#define STBI_NO_PSD
#endif
#ifndef STBI_ONLY_TGA
#define STBI_NO_TGA
#endif
#ifndef STBI_ONLY_GIF
#define STBI_NO_GIF
#endif
#ifndef STBI_ONLY_HDR
#define STBI_NO_HDR
#endif
#ifndef STBI_ONLY_PIC
#define STBI_NO_PIC
#endif
#ifndef STBI_ONLY_PNM
#define STBI_NO_PNM
#endif
#endif

#if defined(STBI_NO_PNG) && !defined(STBI_SUPPORT_ZLIB) && !defined(STBI_NO_ZLIB)
#define STBI_NO_ZLIB
#endif


#include <stdarg.h>
#include <stddef.h> // ptrdiff_t on osx
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#if !defined(STBI_NO_LINEAR) || !defined(STBI_NO_HDR)
#include <math.h>  // ldexp, pow
#endif

#ifndef STBI_NO_STDIO
#include <stdio.h>
#endif

#ifndef STBI_ASSERT
#include <assert.h>
#define STBI_ASSERT(x) assert(x)
#endif

#ifdef __cplusplus
#define STBI_EXTERN extern "C"
#else
#define STBI_EXTERN extern
#endif


#ifndef _MSC_VER
#ifdef __cplusplus
#define stbi_inline inline
#else
#define stbi_inline
#endif
#else
#define stbi_inline __forceinline
#endif

#ifndef STBI_NO_THREAD_LOCALS
#if defined(__cplusplus) &&  __cplusplus >= 201103L
#define STBI_THREAD_LOCAL       thread_local
#elif defined(__GNUC__) && __GNUC__ < 5
#define STBI_THREAD_LOCAL       __thread
#elif defined(_MSC_VER)
#define STBI_THREAD_LOCAL       __declspec(thread)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
#define STBI_THREAD_LOCAL       _Thread_local
#endif

#ifndef STBI_THREAD_LOCAL
#if defined(__GNUC__)
#define STBI_THREAD_LOCAL       __thread
#endif
#endif
#endif

#ifdef _MSC_VER
    typedef unsigned short stbi__uint16;
    typedef   signed short stbi__int16;
    typedef unsigned int   stbi__uint32;
    typedef   signed int   stbi__int32;
#else
#include <stdint.h>
    typedef uint16_t stbi__uint16;
    typedef int16_t  stbi__int16;
    typedef uint32_t stbi__uint32;
    typedef int32_t  stbi__int32;
#endif

    // should produce compiler error if size is wrong
    typedef unsigned char validate_uint32[sizeof(stbi__uint32) == 4 ? 1 : -1];

#ifdef _MSC_VER
#define STBI_NOTUSED(v)  (void)(v)
#else
#define STBI_NOTUSED(v)  (void)sizeof(v)
#endif

#ifdef _MSC_VER
#define STBI_HAS_LROTL
#endif

#ifdef STBI_HAS_LROTL
#define stbi_lrot(x,y)  _lrotl(x,y)
#else
#define stbi_lrot(x,y)  (((x) << (y)) | ((x) >> (-(y) & 31)))
#endif

#if defined(STBI_MALLOC) && defined(STBI_FREE) && (defined(STBI_REALLOC) || defined(STBI_REALLOC_SIZED))
    // ok
#elif !defined(STBI_MALLOC) && !defined(STBI_FREE) && !defined(STBI_REALLOC) && !defined(STBI_REALLOC_SIZED)
    // ok
#else
#error "Must define all or none of STBI_MALLOC, STBI_FREE, and STBI_REALLOC (or STBI_REALLOC_SIZED)."
#endif

#ifndef STBI_MALLOC
#define STBI_MALLOC(sz)           malloc(sz)
#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
#define STBI_FREE(p)              free(p)
#endif

#ifndef STBI_REALLOC_SIZED
#define STBI_REALLOC_SIZED(p,oldsz,newsz) STBI_REALLOC(p,newsz)
#endif

    // x86/x64 detection
#if defined(__x86_64__) || defined(_M_X64)
#define STBI__X64_TARGET
#elif defined(__i386) || defined(_M_IX86)
#define STBI__X86_TARGET
#endif

#if defined(__GNUC__) && defined(STBI__X86_TARGET) && !defined(__SSE2__) && !defined(STBI_NO_SIMD)
// gcc doesn't support sse2 intrinsics unless you compile with -msse2,
// which in turn means it gets to use SSE2 everywhere. This is unfortunate,
// but previous attempts to provide the SSE2 functions with runtime
// detection caused numerous issues. The way architecture extensions are
// exposed in GCC/Clang is, sadly, not really suited for one-file libs.
// New behavior: if compiled with -msse2, we use SSE2 without any
// detection; if not, we don't use it at all.
#define STBI_NO_SIMD
#endif

#if defined(__MINGW32__) && defined(STBI__X86_TARGET) && !defined(STBI_MINGW_ENABLE_SSE2) && !defined(STBI_NO_SIMD)
// Note that __MINGW32__ doesn't actually mean 32-bit, so we have to avoid STBI__X64_TARGET
//
// 32-bit MinGW wants ESP to be 16-byte aligned, but this is not in the
// Windows ABI and VC++ as well as Windows DLLs don't maintain that invariant.
// As a result, enabling SSE2 on 32-bit MinGW is dangerous when not
// simultaneously enabling "-mstackrealign".
//
// See https://github.com/nothings/stb/issues/81 for more information.
//
// So default to no SSE2 on 32-bit MinGW. If you've read this far and added
// -mstackrealign to your build settings, feel free to #define STBI_MINGW_ENABLE_SSE2.
#define STBI_NO_SIMD
#endif

#if !defined(STBI_NO_SIMD) && (defined(STBI__X86_TARGET) || defined(STBI__X64_TARGET))
#define STBI_SSE2
#include <emmintrin.h>

#ifdef _MSC_VER

#if _MSC_VER >= 1400  // not VC6
#include <intrin.h> // __cpuid
    static int stbi__cpuid3(void)
    {
        int info[4];
        __cpuid(info, 1);
        return info[3];
    }
#else
    static int stbi__cpuid3(void)
    {
        int res;
        __asm {
            mov  eax, 1
            cpuid
            mov  res, edx
        }
        return res;
    }
#endif

#define STBI_SIMD_ALIGN(type, name) __declspec(align(16)) type name

#if !defined(STBI_NO_JPEG) && defined(STBI_SSE2)
    static int stbi__sse2_available(void)
    {
        int info3 = stbi__cpuid3();
        return ((info3 >> 26) & 1) != 0;
    }
#endif

#else // assume GCC-style if not VC++
#define STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))

#if !defined(STBI_NO_JPEG) && defined(STBI_SSE2)
    static int stbi__sse2_available(void)
    {
        // If we're even attempting to compile this on GCC/Clang, that means
        // -msse2 is on, which means the compiler is allowed to use SSE2
        // instructions at will, and so are we.
        return 1;
    }
#endif

#endif
#endif

    // ARM NEON
#if defined(STBI_NO_SIMD) && defined(STBI_NEON)
#undef STBI_NEON
#endif

#ifdef STBI_NEON
#include <arm_neon.h>
#ifdef _MSC_VER
#define STBI_SIMD_ALIGN(type, name) __declspec(align(16)) type name
#else
#define STBI_SIMD_ALIGN(type, name) type name __attribute__((aligned(16)))
#endif
#endif

#ifndef STBI_SIMD_ALIGN
#define STBI_SIMD_ALIGN(type, name) type name
#endif

#ifndef STBI_MAX_DIMENSIONS
#define STBI_MAX_DIMENSIONS (1 << 24)
#endif

///////////////////////////////////////////////
//
//  stbi__context struct and start_xxx functions

// stbi__context structure is our basic context used by all images, so it
// contains all the IO context, plus some basic image information
    typedef struct
    {
        stbi__uint32 img_x, img_y;
        int img_n, img_out_n;

        stbi_io_callbacks io;
        void* io_user_data;

        int read_from_callbacks;
        int buflen;
        stbi_uc buffer_start[128];
        int callback_already_read;

        stbi_uc* img_buffer, * img_buffer_end;
        stbi_uc* img_buffer_original, * img_buffer_original_end;
    } stbi__context;


    static void stbi__refill_buffer(stbi__context* s);

    // initialize a memory-decode context
    static void stbi__start_mem(stbi__context* s, stbi_uc const* buffer, int len)
    {
        s->io.read = NULL;
        s->read_from_callbacks = 0;
        s->callback_already_read = 0;
        s->img_buffer = s->img_buffer_original = (stbi_uc*)buffer;
        s->img_buffer_end = s->img_buffer_original_end = (stbi_uc*)buffer + len;
    }

    // initialize a callback-based context
    static void stbi__start_callbacks(stbi__context* s, stbi_io_callbacks* c, void* user)
    {
        s->io = *c;
        s->io_user_data = user;
        s->buflen = sizeof(s->buffer_start);
        s->read_from_callbacks = 1;
        s->callback_already_read = 0;
        s->img_buffer = s->img_buffer_original = s->buffer_start;
        stbi__refill_buffer(s);
        s->img_buffer_original_end = s->img_buffer_end;
    }

#ifndef STBI_NO_STDIO

    static int stbi__stdio_read(void* user, char* data, int size)
    {
        return (int)fread(data, 1, size, (FILE*)user);
    }

    static void stbi__stdio_skip(void* user, int n)
    {
        int ch;
        fseek((FILE*)user, n, SEEK_CUR);
        ch = fgetc((FILE*)user);  /* have to read a byte to reset feof()'s flag */
        if (ch != EOF) {
            ungetc(ch, (FILE*)user);  /* push byte back onto stream if valid. */
        }
    }

    static int stbi__stdio_eof(void* user)
    {
        return feof((FILE*)user) || ferror((FILE*)user);
    }

    static stbi_io_callbacks stbi__stdio_callbacks =
    {
       stbi__stdio_read,
       stbi__stdio_skip,
       stbi__stdio_eof,
    };

    static void stbi__start_file(stbi__context* s, FILE* f)
    {
        stbi__start_callbacks(s, &stbi__stdio_callbacks, (void*)f);
    }

    //static void stop_file(stbi__context *s) { }

#endif // !STBI_NO_STDIO

    static void stbi__rewind(stbi__context* s)
    {
        // conceptually rewind SHOULD rewind to the beginning of the stream,
        // but we just rewind to the beginning of the initial buffer, because
        // we only use it after doing 'test', which only ever looks at at most 92 bytes
        s->img_buffer = s->img_buffer_original;
        s->img_buffer_end = s->img_buffer_original_end;
    }

    enum
    {
        STBI_ORDER_RGB,
        STBI_ORDER_BGR
    };

    typedef struct
    {
        int bits_per_channel;
        int num_channels;
        int channel_order;
    } stbi__result_info;

#ifndef STBI_NO_JPEG
    static int      stbi__jpeg_test(stbi__context* s);
    static void* stbi__jpeg_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static int      stbi__jpeg_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PNG
    static int      stbi__png_test(stbi__context* s);
    static void* stbi__png_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static int      stbi__png_info(stbi__context* s, int* x, int* y, int* comp);
    static int      stbi__png_is16(stbi__context* s);
#endif

#ifndef STBI_NO_BMP
    static int      stbi__bmp_test(stbi__context* s);
    static void* stbi__bmp_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static int      stbi__bmp_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_TGA
    static int      stbi__tga_test(stbi__context* s);
    static void* stbi__tga_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static int      stbi__tga_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PSD
    static int      stbi__psd_test(stbi__context* s);
    static void* stbi__psd_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri, int bpc);
    static int      stbi__psd_info(stbi__context* s, int* x, int* y, int* comp);
    static int      stbi__psd_is16(stbi__context* s);
#endif

#ifndef STBI_NO_HDR
    static int      stbi__hdr_test(stbi__context* s);
    static float* stbi__hdr_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static int      stbi__hdr_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PIC
    static int      stbi__pic_test(stbi__context* s);
    static void* stbi__pic_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static int      stbi__pic_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_GIF
    static int      stbi__gif_test(stbi__context* s);
    static void* stbi__gif_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static void* stbi__load_gif_main(stbi__context* s, int** delays, int* x, int* y, int* z, int* comp, int req_comp);
    static int      stbi__gif_info(stbi__context* s, int* x, int* y, int* comp);
#endif

#ifndef STBI_NO_PNM
    static int      stbi__pnm_test(stbi__context* s);
    static void* stbi__pnm_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri);
    static int      stbi__pnm_info(stbi__context* s, int* x, int* y, int* comp);
    static int      stbi__pnm_is16(stbi__context* s);
#endif

    static
#ifdef STBI_THREAD_LOCAL
        STBI_THREAD_LOCAL
#endif
        const char* stbi__g_failure_reason;

    STBIDEF const char* stbi_failure_reason(void)
    {
        return stbi__g_failure_reason;
    }

#ifndef STBI_NO_FAILURE_STRINGS
    static int stbi__err(const char* str)
    {
        stbi__g_failure_reason = str;
        return 0;
    }
#endif

    static void* stbi__malloc(size_t size)
    {
        return STBI_MALLOC(size);
    }

    // stb_image uses ints pervasively, including for offset calculations.
    // therefore the largest decoded image size we can support with the
    // current code, even on 64-bit targets, is INT_MAX. this is not a
    // significant limitation for the intended use case.
    //
    // we do, however, need to make sure our size calculations don't
    // overflow. hence a few helper functions for size calculations that
    // multiply integers together, making sure that they're non-negative
    // and no overflow occurs.

    // return 1 if the sum is valid, 0 on overflow.
    // negative terms are considered invalid.
    static int stbi__addsizes_valid(int a, int b)
    {
        if (b < 0) return 0;
        // now 0 <= b <= INT_MAX, hence also
        // 0 <= INT_MAX - b <= INTMAX.
        // And "a + b <= INT_MAX" (which might overflow) is the
        // same as a <= INT_MAX - b (no overflow)
        return a <= INT_MAX - b;
    }

    // returns 1 if the product is valid, 0 on overflow.
    // negative factors are considered invalid.
    static int stbi__mul2sizes_valid(int a, int b)
    {
        if (a < 0 || b < 0) return 0;
        if (b == 0) return 1; // mul-by-0 is always safe
        // portable way to check for no overflows in a*b
        return a <= INT_MAX / b;
    }

#if !defined(STBI_NO_JPEG) || !defined(STBI_NO_PNG) || !defined(STBI_NO_TGA) || !defined(STBI_NO_HDR)
    // returns 1 if "a*b + add" has no negative terms/factors and doesn't overflow
    static int stbi__mad2sizes_valid(int a, int b, int add)
    {
        return stbi__mul2sizes_valid(a, b) && stbi__addsizes_valid(a * b, add);
    }
#endif

    // returns 1 if "a*b*c + add" has no negative terms/factors and doesn't overflow
    static int stbi__mad3sizes_valid(int a, int b, int c, int add)
    {
        return stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a * b, c) &&
            stbi__addsizes_valid(a * b * c, add);
    }

    // returns 1 if "a*b*c*d + add" has no negative terms/factors and doesn't overflow
#if !defined(STBI_NO_LINEAR) || !defined(STBI_NO_HDR) || !defined(STBI_NO_PNM)
    static int stbi__mad4sizes_valid(int a, int b, int c, int d, int add)
    {
        return stbi__mul2sizes_valid(a, b) && stbi__mul2sizes_valid(a * b, c) &&
            stbi__mul2sizes_valid(a * b * c, d) && stbi__addsizes_valid(a * b * c * d, add);
    }
#endif

#if !defined(STBI_NO_JPEG) || !defined(STBI_NO_PNG) || !defined(STBI_NO_TGA) || !defined(STBI_NO_HDR)
    // mallocs with size overflow checking
    static void* stbi__malloc_mad2(int a, int b, int add)
    {
        if (!stbi__mad2sizes_valid(a, b, add)) return NULL;
        return stbi__malloc(a * b + add);
    }
#endif

    static void* stbi__malloc_mad3(int a, int b, int c, int add)
    {
        if (!stbi__mad3sizes_valid(a, b, c, add)) return NULL;
        return stbi__malloc(a * b * c + add);
    }

#if !defined(STBI_NO_LINEAR) || !defined(STBI_NO_HDR) || !defined(STBI_NO_PNM)
    static void* stbi__malloc_mad4(int a, int b, int c, int d, int add)
    {
        if (!stbi__mad4sizes_valid(a, b, c, d, add)) return NULL;
        return stbi__malloc(a * b * c * d + add);
    }
#endif

    // stbi__err - error
    // stbi__errpf - error returning pointer to float
    // stbi__errpuc - error returning pointer to unsigned char

#ifdef STBI_NO_FAILURE_STRINGS
#define stbi__err(x,y)  0
#elif defined(STBI_FAILURE_USERMSG)
#define stbi__err(x,y)  stbi__err(y)
#else
#define stbi__err(x,y)  stbi__err(x)
#endif

#define stbi__errpf(x,y)   ((float *)(size_t) (stbi__err(x,y)?NULL:NULL))
#define stbi__errpuc(x,y)  ((unsigned char *)(size_t) (stbi__err(x,y)?NULL:NULL))

    STBIDEF void stbi_image_free(void* retval_from_stbi_load)
    {
        STBI_FREE(retval_from_stbi_load);
    }

#ifndef STBI_NO_LINEAR
    static float* stbi__ldr_to_hdr(stbi_uc* data, int x, int y, int comp);
#endif

#ifndef STBI_NO_HDR
    static stbi_uc* stbi__hdr_to_ldr(float* data, int x, int y, int comp);
#endif

    static int stbi__vertically_flip_on_load_global = 0;

    STBIDEF void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip)
    {
        stbi__vertically_flip_on_load_global = flag_true_if_should_flip;
    }

#ifndef STBI_THREAD_LOCAL
#define stbi__vertically_flip_on_load  stbi__vertically_flip_on_load_global
#else
    static STBI_THREAD_LOCAL int stbi__vertically_flip_on_load_local, stbi__vertically_flip_on_load_set;

    STBIDEF void stbi_set_flip_vertically_on_load_thread(int flag_true_if_should_flip)
    {
        stbi__vertically_flip_on_load_local = flag_true_if_should_flip;
        stbi__vertically_flip_on_load_set = 1;
    }

#define stbi__vertically_flip_on_load  (stbi__vertically_flip_on_load_set       \
                                         ? stbi__vertically_flip_on_load_local  \
                                         : stbi__vertically_flip_on_load_global)
#endif // STBI_THREAD_LOCAL

    static void* stbi__load_main(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri, int bpc)
    {
        memset(ri, 0, sizeof(*ri)); // make sure it's initialized if we add new fields
        ri->bits_per_channel = 8; // default is 8 so most paths don't have to be changed
        ri->channel_order = STBI_ORDER_RGB; // all current input & output are this, but this is here so we can add BGR order
        ri->num_channels = 0;

        // test the formats with a very explicit header first (at least a FOURCC
        // or distinctive magic number first)
#ifndef STBI_NO_PNG
        if (stbi__png_test(s))  return stbi__png_load(s, x, y, comp, req_comp, ri);
#endif
#ifndef STBI_NO_BMP
        if (stbi__bmp_test(s))  return stbi__bmp_load(s, x, y, comp, req_comp, ri);
#endif
#ifndef STBI_NO_GIF
        if (stbi__gif_test(s))  return stbi__gif_load(s, x, y, comp, req_comp, ri);
#endif
#ifndef STBI_NO_PSD
        if (stbi__psd_test(s))  return stbi__psd_load(s, x, y, comp, req_comp, ri, bpc);
#else
        STBI_NOTUSED(bpc);
#endif
#ifndef STBI_NO_PIC
        if (stbi__pic_test(s))  return stbi__pic_load(s, x, y, comp, req_comp, ri);
#endif

        // then the formats that can end up attempting to load with just 1 or 2
        // bytes matching expectations; these are prone to false positives, so
        // try them later
#ifndef STBI_NO_JPEG
        if (stbi__jpeg_test(s)) return stbi__jpeg_load(s, x, y, comp, req_comp, ri);
#endif
#ifndef STBI_NO_PNM
        if (stbi__pnm_test(s))  return stbi__pnm_load(s, x, y, comp, req_comp, ri);
#endif

#ifndef STBI_NO_HDR
        if (stbi__hdr_test(s)) {
            float* hdr = stbi__hdr_load(s, x, y, comp, req_comp, ri);
            return stbi__hdr_to_ldr(hdr, *x, *y, req_comp ? req_comp : *comp);
        }
#endif

#ifndef STBI_NO_TGA
        // test tga last because it's a crappy test!
        if (stbi__tga_test(s))
            return stbi__tga_load(s, x, y, comp, req_comp, ri);
#endif

        return stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
    }

    static stbi_uc* stbi__convert_16_to_8(stbi__uint16* orig, int w, int h, int channels)
    {
        int i;
        int img_len = w * h * channels;
        stbi_uc* reduced;

        reduced = (stbi_uc*)stbi__malloc(img_len);
        if (reduced == NULL) return stbi__errpuc("outofmem", "Out of memory");

        for (i = 0; i < img_len; ++i)
            reduced[i] = (stbi_uc)((orig[i] >> 8) & 0xFF); // top half of each byte is sufficient approx of 16->8 bit scaling

        STBI_FREE(orig);
        return reduced;
    }

    static stbi__uint16* stbi__convert_8_to_16(stbi_uc* orig, int w, int h, int channels)
    {
        int i;
        int img_len = w * h * channels;
        stbi__uint16* enlarged;

        enlarged = (stbi__uint16*)stbi__malloc(img_len * 2);
        if (enlarged == NULL) return (stbi__uint16*)stbi__errpuc("outofmem", "Out of memory");

        for (i = 0; i < img_len; ++i)
            enlarged[i] = (stbi__uint16)((orig[i] << 8) + orig[i]); // replicate to high and low byte, maps 0->0, 255->0xffff

        STBI_FREE(orig);
        return enlarged;
    }

    static void stbi__vertical_flip(void* image, int w, int h, int bytes_per_pixel)
    {
        int row;
        size_t bytes_per_row = (size_t)w * bytes_per_pixel;
        stbi_uc temp[2048];
        stbi_uc* bytes = (stbi_uc*)image;

        for (row = 0; row < (h >> 1); row++) {
            stbi_uc* row0 = bytes + row * bytes_per_row;
            stbi_uc* row1 = bytes + (h - row - 1) * bytes_per_row;
            // swap row0 with row1
            size_t bytes_left = bytes_per_row;
            while (bytes_left) {
                size_t bytes_copy = (bytes_left < sizeof(temp)) ? bytes_left : sizeof(temp);
                memcpy(temp, row0, bytes_copy);
                memcpy(row0, row1, bytes_copy);
                memcpy(row1, temp, bytes_copy);
                row0 += bytes_copy;
                row1 += bytes_copy;
                bytes_left -= bytes_copy;
            }
        }
    }

#ifndef STBI_NO_GIF
    static void stbi__vertical_flip_slices(void* image, int w, int h, int z, int bytes_per_pixel)
    {
        int slice;
        int slice_size = w * h * bytes_per_pixel;

        stbi_uc* bytes = (stbi_uc*)image;
        for (slice = 0; slice < z; ++slice) {
            stbi__vertical_flip(bytes, w, h, bytes_per_pixel);
            bytes += slice_size;
        }
    }
#endif

    static unsigned char* stbi__load_and_postprocess_8bit(stbi__context* s, int* x, int* y, int* comp, int req_comp)
    {
        stbi__result_info ri;
        void* result = stbi__load_main(s, x, y, comp, req_comp, &ri, 8);

        if (result == NULL)
            return NULL;

        // it is the responsibility of the loaders to make sure we get either 8 or 16 bit.
        STBI_ASSERT(ri.bits_per_channel == 8 || ri.bits_per_channel == 16);

        if (ri.bits_per_channel != 8) {
            result = stbi__convert_16_to_8((stbi__uint16*)result, *x, *y, req_comp == 0 ? *comp : req_comp);
            ri.bits_per_channel = 8;
        }

        // @TODO: move stbi__convert_format to here

        if (stbi__vertically_flip_on_load) {
            int channels = req_comp ? req_comp : *comp;
            stbi__vertical_flip(result, *x, *y, channels * sizeof(stbi_uc));
        }

        return (unsigned char*)result;
    }

    static stbi__uint16* stbi__load_and_postprocess_16bit(stbi__context* s, int* x, int* y, int* comp, int req_comp)
    {
        stbi__result_info ri;
        void* result = stbi__load_main(s, x, y, comp, req_comp, &ri, 16);

        if (result == NULL)
            return NULL;

        // it is the responsibility of the loaders to make sure we get either 8 or 16 bit.
        STBI_ASSERT(ri.bits_per_channel == 8 || ri.bits_per_channel == 16);

        if (ri.bits_per_channel != 16) {
            result = stbi__convert_8_to_16((stbi_uc*)result, *x, *y, req_comp == 0 ? *comp : req_comp);
            ri.bits_per_channel = 16;
        }

        // @TODO: move stbi__convert_format16 to here
        // @TODO: special case RGB-to-Y (and RGBA-to-YA) for 8-bit-to-16-bit case to keep more precision

        if (stbi__vertically_flip_on_load) {
            int channels = req_comp ? req_comp : *comp;
            stbi__vertical_flip(result, *x, *y, channels * sizeof(stbi__uint16));
        }

        return (stbi__uint16*)result;
    }

#if !defined(STBI_NO_HDR) && !defined(STBI_NO_LINEAR)
    static void stbi__float_postprocess(float* result, int* x, int* y, int* comp, int req_comp)
    {
        if (stbi__vertically_flip_on_load && result != NULL) {
            int channels = req_comp ? req_comp : *comp;
            stbi__vertical_flip(result, *x, *y, channels * sizeof(float));
        }
    }
#endif

#ifndef STBI_NO_STDIO

#if defined(_WIN32) && defined(STBI_WINDOWS_UTF8)
    STBI_EXTERN __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int cp, unsigned long flags, const char* str, int cbmb, wchar_t* widestr, int cchwide);
    STBI_EXTERN __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, const wchar_t* widestr, int cchwide, char* str, int cbmb, const char* defchar, int* used_default);
#endif

#if defined(_WIN32) && defined(STBI_WINDOWS_UTF8)
    STBIDEF int stbi_convert_wchar_to_utf8(char* buffer, size_t bufferlen, const wchar_t* input)
    {
        return WideCharToMultiByte(65001 /* UTF8 */, 0, input, -1, buffer, (int)bufferlen, NULL, NULL);
    }
#endif

    static FILE* stbi__fopen(char const* filename, char const* mode)
    {
        FILE* f;
#if defined(_WIN32) && defined(STBI_WINDOWS_UTF8)
        wchar_t wMode[64];
        wchar_t wFilename[1024];
        if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, filename, -1, wFilename, sizeof(wFilename) / sizeof(*wFilename)))
            return 0;

        if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, mode, -1, wMode, sizeof(wMode) / sizeof(*wMode)))
            return 0;

#if defined(_MSC_VER) && _MSC_VER >= 1400
        if (0 != _wfopen_s(&f, wFilename, wMode))
            f = 0;
#else
        f = _wfopen(wFilename, wMode);
#endif

#elif defined(_MSC_VER) && _MSC_VER >= 1400
        if (0 != fopen_s(&f, filename, mode))
            f = 0;
#else
        f = fopen(filename, mode);
#endif
        return f;
    }


    STBIDEF stbi_uc* stbi_load(char const* filename, int* x, int* y, int* comp, int req_comp)
    {
        FILE* f = stbi__fopen(filename, "rb");
        unsigned char* result;
        if (!f) return stbi__errpuc("can't fopen", "Unable to open file");
        result = stbi_load_from_file(f, x, y, comp, req_comp);
        fclose(f);
        return result;
    }

    STBIDEF stbi_uc* stbi_load_from_file(FILE* f, int* x, int* y, int* comp, int req_comp)
    {
        unsigned char* result;
        stbi__context s;
        stbi__start_file(&s, f);
        result = stbi__load_and_postprocess_8bit(&s, x, y, comp, req_comp);
        if (result) {
            // need to 'unget' all the characters in the IO buffer
            fseek(f, -(int)(s.img_buffer_end - s.img_buffer), SEEK_CUR);
        }
        return result;
    }

    STBIDEF stbi__uint16* stbi_load_from_file_16(FILE* f, int* x, int* y, int* comp, int req_comp)
    {
        stbi__uint16* result;
        stbi__context s;
        stbi__start_file(&s, f);
        result = stbi__load_and_postprocess_16bit(&s, x, y, comp, req_comp);
        if (result) {
            // need to 'unget' all the characters in the IO buffer
            fseek(f, -(int)(s.img_buffer_end - s.img_buffer), SEEK_CUR);
        }
        return result;
    }

    STBIDEF stbi_us* stbi_load_16(char const* filename, int* x, int* y, int* comp, int req_comp)
    {
        FILE* f = stbi__fopen(filename, "rb");
        stbi__uint16* result;
        if (!f) return (stbi_us*)stbi__errpuc("can't fopen", "Unable to open file");
        result = stbi_load_from_file_16(f, x, y, comp, req_comp);
        fclose(f);
        return result;
    }


#endif //!STBI_NO_STDIO

    STBIDEF stbi_us* stbi_load_16_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* channels_in_file, int desired_channels)
    {
        stbi__context s;
        stbi__start_mem(&s, buffer, len);
        return stbi__load_and_postprocess_16bit(&s, x, y, channels_in_file, desired_channels);
    }

    STBIDEF stbi_us* stbi_load_16_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* channels_in_file, int desired_channels)
    {
        stbi__context s;
        stbi__start_callbacks(&s, (stbi_io_callbacks*)clbk, user);
        return stbi__load_and_postprocess_16bit(&s, x, y, channels_in_file, desired_channels);
    }

    STBIDEF stbi_uc* stbi_load_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp, int req_comp)
    {
        stbi__context s;
        stbi__start_mem(&s, buffer, len);
        return stbi__load_and_postprocess_8bit(&s, x, y, comp, req_comp);
    }

    STBIDEF stbi_uc* stbi_load_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* comp, int req_comp)
    {
        stbi__context s;
        stbi__start_callbacks(&s, (stbi_io_callbacks*)clbk, user);
        return stbi__load_and_postprocess_8bit(&s, x, y, comp, req_comp);
    }

#ifndef STBI_NO_GIF
    STBIDEF stbi_uc* stbi_load_gif_from_memory(stbi_uc const* buffer, int len, int** delays, int* x, int* y, int* z, int* comp, int req_comp)
    {
        unsigned char* result;
        stbi__context s;
        stbi__start_mem(&s, buffer, len);

        result = (unsigned char*)stbi__load_gif_main(&s, delays, x, y, z, comp, req_comp);
        if (stbi__vertically_flip_on_load) {
            stbi__vertical_flip_slices(result, *x, *y, *z, *comp);
        }

        return result;
    }
#endif

#ifndef STBI_NO_LINEAR
    static float* stbi__loadf_main(stbi__context* s, int* x, int* y, int* comp, int req_comp)
    {
        unsigned char* data;
#ifndef STBI_NO_HDR
        if (stbi__hdr_test(s)) {
            stbi__result_info ri;
            float* hdr_data = stbi__hdr_load(s, x, y, comp, req_comp, &ri);
            if (hdr_data)
                stbi__float_postprocess(hdr_data, x, y, comp, req_comp);
            return hdr_data;
        }
#endif
        data = stbi__load_and_postprocess_8bit(s, x, y, comp, req_comp);
        if (data)
            return stbi__ldr_to_hdr(data, *x, *y, req_comp ? req_comp : *comp);
        return stbi__errpf("unknown image type", "Image not of any known type, or corrupt");
    }

    STBIDEF float* stbi_loadf_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp, int req_comp)
    {
        stbi__context s;
        stbi__start_mem(&s, buffer, len);
        return stbi__loadf_main(&s, x, y, comp, req_comp);
    }

    STBIDEF float* stbi_loadf_from_callbacks(stbi_io_callbacks const* clbk, void* user, int* x, int* y, int* comp, int req_comp)
    {
        stbi__context s;
        stbi__start_callbacks(&s, (stbi_io_callbacks*)clbk, user);
        return stbi__loadf_main(&s, x, y, comp, req_comp);
    }

#ifndef STBI_NO_STDIO
    STBIDEF float* stbi_loadf(char const* filename, int* x, int* y, int* comp, int req_comp)
    {
        float* result;
        FILE* f = stbi__fopen(filename, "rb");
        if (!f) return stbi__errpf("can't fopen", "Unable to open file");
        result = stbi_loadf_from_file(f, x, y, comp, req_comp);
        fclose(f);
        return result;
    }

    STBIDEF float* stbi_loadf_from_file(FILE* f, int* x, int* y, int* comp, int req_comp)
    {
        stbi__context s;
        stbi__start_file(&s, f);
        return stbi__loadf_main(&s, x, y, comp, req_comp);
    }
#endif // !STBI_NO_STDIO

#endif // !STBI_NO_LINEAR

    // these is-hdr-or-not is defined independent of whether STBI_NO_LINEAR is
    // defined, for API simplicity; if STBI_NO_LINEAR is defined, it always
    // reports false!

    STBIDEF int stbi_is_hdr_from_memory(stbi_uc const* buffer, int len)
    {
#ifndef STBI_NO_HDR
        stbi__context s;
        stbi__start_mem(&s, buffer, len);
        return stbi__hdr_test(&s);
#else
        STBI_NOTUSED(buffer);
        STBI_NOTUSED(len);
        return 0;
#endif
    }

#ifndef STBI_NO_STDIO
    STBIDEF int      stbi_is_hdr(char const* filename)
    {
        FILE* f = stbi__fopen(filename, "rb");
        int result = 0;
        if (f) {
            result = stbi_is_hdr_from_file(f);
            fclose(f);
        }
        return result;
    }

    STBIDEF int stbi_is_hdr_from_file(FILE* f)
    {
#ifndef STBI_NO_HDR
        long pos = ftell(f);
        int res;
        stbi__context s;
        stbi__start_file(&s, f);
        res = stbi__hdr_test(&s);
        fseek(f, pos, SEEK_SET);
        return res;
#else
        STBI_NOTUSED(f);
        return 0;
#endif
    }
#endif // !STBI_NO_STDIO

    STBIDEF int      stbi_is_hdr_from_callbacks(stbi_io_callbacks const* clbk, void* user)
    {
#ifndef STBI_NO_HDR
        stbi__context s;
        stbi__start_callbacks(&s, (stbi_io_callbacks*)clbk, user);
        return stbi__hdr_test(&s);
#else
        STBI_NOTUSED(clbk);
        STBI_NOTUSED(user);
        return 0;
#endif
    }

#ifndef STBI_NO_LINEAR
    static float stbi__l2h_gamma = 2.2f, stbi__l2h_scale = 1.0f;

    STBIDEF void   stbi_ldr_to_hdr_gamma(float gamma) { stbi__l2h_gamma = gamma; }
    STBIDEF void   stbi_ldr_to_hdr_scale(float scale) { stbi__l2h_scale = scale; }
#endif

    static float stbi__h2l_gamma_i = 1.0f / 2.2f, stbi__h2l_scale_i = 1.0f;

    STBIDEF void   stbi_hdr_to_ldr_gamma(float gamma) { stbi__h2l_gamma_i = 1 / gamma; }
    STBIDEF void   stbi_hdr_to_ldr_scale(float scale) { stbi__h2l_scale_i = 1 / scale; }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Common code used by all image loaders
    //

    enum
    {
        STBI__SCAN_load = 0,
        STBI__SCAN_type,
        STBI__SCAN_header
    };

    static void stbi__refill_buffer(stbi__context* s)
    {
        int n = (s->io.read)(s->io_user_data, (char*)s->buffer_start, s->buflen);
        s->callback_already_read += (int)(s->img_buffer - s->img_buffer_original);
        if (n == 0) {
            // at end of file, treat same as if from memory, but need to handle case
            // where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file
            s->read_from_callbacks = 0;
            s->img_buffer = s->buffer_start;
            s->img_buffer_end = s->buffer_start + 1;
            *s->img_buffer = 0;
        }
        else {
            s->img_buffer = s->buffer_start;
            s->img_buffer_end = s->buffer_start + n;
        }
    }

    stbi_inline static stbi_uc stbi__get8(stbi__context* s)
    {
        if (s->img_buffer < s->img_buffer_end)
            return *s->img_buffer++;
        if (s->read_from_callbacks) {
            stbi__refill_buffer(s);
            return *s->img_buffer++;
        }
        return 0;
    }

#if defined(STBI_NO_JPEG) && defined(STBI_NO_HDR) && defined(STBI_NO_PIC) && defined(STBI_NO_PNM)
    // nothing
#else
    stbi_inline static int stbi__at_eof(stbi__context* s)
    {
        if (s->io.read) {
            if (!(s->io.eof)(s->io_user_data)) return 0;
            // if feof() is true, check if buffer = end
            // special case: we've only got the special 0 character at the end
            if (s->read_from_callbacks == 0) return 1;
        }

        return s->img_buffer >= s->img_buffer_end;
    }
#endif

#if defined(STBI_NO_JPEG) && defined(STBI_NO_PNG) && defined(STBI_NO_BMP) && defined(STBI_NO_PSD) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF) && defined(STBI_NO_PIC)
    // nothing
#else
    static void stbi__skip(stbi__context* s, int n)
    {
        if (n == 0) return;  // already there!
        if (n < 0) {
            s->img_buffer = s->img_buffer_end;
            return;
        }
        if (s->io.read) {
            int blen = (int)(s->img_buffer_end - s->img_buffer);
            if (blen < n) {
                s->img_buffer = s->img_buffer_end;
                (s->io.skip)(s->io_user_data, n - blen);
                return;
            }
        }
        s->img_buffer += n;
    }
#endif

#if defined(STBI_NO_PNG) && defined(STBI_NO_TGA) && defined(STBI_NO_HDR) && defined(STBI_NO_PNM)
    // nothing
#else
    static int stbi__getn(stbi__context* s, stbi_uc* buffer, int n)
    {
        if (s->io.read) {
            int blen = (int)(s->img_buffer_end - s->img_buffer);
            if (blen < n) {
                int res, count;

                memcpy(buffer, s->img_buffer, blen);

                count = (s->io.read)(s->io_user_data, (char*)buffer + blen, n - blen);
                res = (count == (n - blen));
                s->img_buffer = s->img_buffer_end;
                return res;
            }
        }

        if (s->img_buffer + n <= s->img_buffer_end) {
            memcpy(buffer, s->img_buffer, n);
            s->img_buffer += n;
            return 1;
        }
        else
            return 0;
    }
#endif

#if defined(STBI_NO_JPEG) && defined(STBI_NO_PNG) && defined(STBI_NO_PSD) && defined(STBI_NO_PIC)
    // nothing
#else
    static int stbi__get16be(stbi__context* s)
    {
        int z = stbi__get8(s);
        return (z << 8) + stbi__get8(s);
    }
#endif

#if defined(STBI_NO_PNG) && defined(STBI_NO_PSD) && defined(STBI_NO_PIC)
    // nothing
#else
    static stbi__uint32 stbi__get32be(stbi__context* s)
    {
        stbi__uint32 z = stbi__get16be(s);
        return (z << 16) + stbi__get16be(s);
    }
#endif

#if defined(STBI_NO_BMP) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF)
    // nothing
#else
    static int stbi__get16le(stbi__context* s)
    {
        int z = stbi__get8(s);
        return z + (stbi__get8(s) << 8);
    }
#endif

#ifndef STBI_NO_BMP
    static stbi__uint32 stbi__get32le(stbi__context* s)
    {
        stbi__uint32 z = stbi__get16le(s);
        z += (stbi__uint32)stbi__get16le(s) << 16;
        return z;
    }
#endif

#define STBI__BYTECAST(x)  ((stbi_uc) ((x) & 255))  // truncate int to byte without warnings

#if defined(STBI_NO_JPEG) && defined(STBI_NO_PNG) && defined(STBI_NO_BMP) && defined(STBI_NO_PSD) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF) && defined(STBI_NO_PIC) && defined(STBI_NO_PNM)
    // nothing
#else
    //////////////////////////////////////////////////////////////////////////////
    //
    //  generic converter from built-in img_n to req_comp
    //    individual types do this automatically as much as possible (e.g. jpeg
    //    does all cases internally since it needs to colorspace convert anyway,
    //    and it never has alpha, so very few cases ). png can automatically
    //    interleave an alpha=255 channel, but falls back to this for other cases
    //
    //  assume data buffer is malloced, so malloc a new one and free that one
    //  only failure mode is malloc failing

    static stbi_uc stbi__compute_y(int r, int g, int b)
    {
        return (stbi_uc)(((r * 77) + (g * 150) + (29 * b)) >> 8);
    }
#endif

#if defined(STBI_NO_PNG) && defined(STBI_NO_BMP) && defined(STBI_NO_PSD) && defined(STBI_NO_TGA) && defined(STBI_NO_GIF) && defined(STBI_NO_PIC) && defined(STBI_NO_PNM)
    // nothing
#else
    static unsigned char* stbi__convert_format(unsigned char* data, int img_n, int req_comp, unsigned int x, unsigned int y)
    {
        int i, j;
        unsigned char* good;

        if (req_comp == img_n) return data;
        STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

        good = (unsigned char*)stbi__malloc_mad3(req_comp, x, y, 0);
        if (good == NULL) {
            STBI_FREE(data);
            return stbi__errpuc("outofmem", "Out of memory");
        }

        for (j = 0; j < (int)y; ++j) {
            unsigned char* src = data + j * x * img_n;
            unsigned char* dest = good + j * x * req_comp;

#define STBI__COMBO(a,b)  ((a)*8+(b))
#define STBI__CASE(a,b)   case STBI__COMBO(a,b): for(i=x-1; i >= 0; --i, src += a, dest += b)
            // convert source image with img_n components to one with req_comp components;
            // avoid switch per pixel, so use switch per scanline and massive macros
            switch (STBI__COMBO(img_n, req_comp)) {
                STBI__CASE(1, 2) { dest[0] = src[0]; dest[1] = 255; } break;
                STBI__CASE(1, 3) { dest[0] = dest[1] = dest[2] = src[0]; } break;
                STBI__CASE(1, 4) { dest[0] = dest[1] = dest[2] = src[0]; dest[3] = 255; } break;
                STBI__CASE(2, 1) { dest[0] = src[0]; } break;
                STBI__CASE(2, 3) { dest[0] = dest[1] = dest[2] = src[0]; } break;
                STBI__CASE(2, 4) { dest[0] = dest[1] = dest[2] = src[0]; dest[3] = src[1]; } break;
                STBI__CASE(3, 4) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 255; } break;
                STBI__CASE(3, 1) { dest[0] = stbi__compute_y(src[0], src[1], src[2]); } break;
                STBI__CASE(3, 2) { dest[0] = stbi__compute_y(src[0], src[1], src[2]); dest[1] = 255; } break;
                STBI__CASE(4, 1) { dest[0] = stbi__compute_y(src[0], src[1], src[2]); } break;
                STBI__CASE(4, 2) { dest[0] = stbi__compute_y(src[0], src[1], src[2]); dest[1] = src[3]; } break;
                STBI__CASE(4, 3) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; } break;
            default: STBI_ASSERT(0); STBI_FREE(data); STBI_FREE(good); return stbi__errpuc("unsupported", "Unsupported format conversion");
            }
#undef STBI__CASE
        }

        STBI_FREE(data);
        return good;
    }
#endif

#if defined(STBI_NO_PNG) && defined(STBI_NO_PSD)
    // nothing
#else
    static stbi__uint16 stbi__compute_y_16(int r, int g, int b)
    {
        return (stbi__uint16)(((r * 77) + (g * 150) + (29 * b)) >> 8);
    }
#endif

#if defined(STBI_NO_PNG) && defined(STBI_NO_PSD)
    // nothing
#else
    static stbi__uint16* stbi__convert_format16(stbi__uint16* data, int img_n, int req_comp, unsigned int x, unsigned int y)
    {
        int i, j;
        stbi__uint16* good;

        if (req_comp == img_n) return data;
        STBI_ASSERT(req_comp >= 1 && req_comp <= 4);

        good = (stbi__uint16*)stbi__malloc(req_comp * x * y * 2);
        if (good == NULL) {
            STBI_FREE(data);
            return (stbi__uint16*)stbi__errpuc("outofmem", "Out of memory");
        }

        for (j = 0; j < (int)y; ++j) {
            stbi__uint16* src = data + j * x * img_n;
            stbi__uint16* dest = good + j * x * req_comp;

#define STBI__COMBO(a,b)  ((a)*8+(b))
#define STBI__CASE(a,b)   case STBI__COMBO(a,b): for(i=x-1; i >= 0; --i, src += a, dest += b)
            // convert source image with img_n components to one with req_comp components;
            // avoid switch per pixel, so use switch per scanline and massive macros
            switch (STBI__COMBO(img_n, req_comp)) {
                STBI__CASE(1, 2) { dest[0] = src[0]; dest[1] = 0xffff; } break;
                STBI__CASE(1, 3) { dest[0] = dest[1] = dest[2] = src[0]; } break;
                STBI__CASE(1, 4) { dest[0] = dest[1] = dest[2] = src[0]; dest[3] = 0xffff; } break;
                STBI__CASE(2, 1) { dest[0] = src[0]; } break;
                STBI__CASE(2, 3) { dest[0] = dest[1] = dest[2] = src[0]; } break;
                STBI__CASE(2, 4) { dest[0] = dest[1] = dest[2] = src[0]; dest[3] = src[1]; } break;
                STBI__CASE(3, 4) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = 0xffff; } break;
                STBI__CASE(3, 1) { dest[0] = stbi__compute_y_16(src[0], src[1], src[2]); } break;
                STBI__CASE(3, 2) { dest[0] = stbi__compute_y_16(src[0], src[1], src[2]); dest[1] = 0xffff; } break;
                STBI__CASE(4, 1) { dest[0] = stbi__compute_y_16(src[0], src[1], src[2]); } break;
                STBI__CASE(4, 2) { dest[0] = stbi__compute_y_16(src[0], src[1], src[2]); dest[1] = src[3]; } break;
                STBI__CASE(4, 3) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; } break;
            default: STBI_ASSERT(0); STBI_FREE(data); STBI_FREE(good); return (stbi__uint16*)stbi__errpuc("unsupported", "Unsupported format conversion");
            }
#undef STBI__CASE
        }

        STBI_FREE(data);
        return good;
    }
#endif

#ifndef STBI_NO_LINEAR
    static float* stbi__ldr_to_hdr(stbi_uc* data, int x, int y, int comp)
    {
        int i, k, n;
        float* output;
        if (!data) return NULL;
        output = (float*)stbi__malloc_mad4(x, y, comp, sizeof(float), 0);
        if (output == NULL) { STBI_FREE(data); return stbi__errpf("outofmem", "Out of memory"); }
        // compute number of non-alpha components
        if (comp & 1) n = comp; else n = comp - 1;
        for (i = 0; i < x * y; ++i) {
            for (k = 0; k < n; ++k) {
                output[i * comp + k] = (float)(pow(data[i * comp + k] / 255.0f, stbi__l2h_gamma) * stbi__l2h_scale);
            }
        }
        if (n < comp) {
            for (i = 0; i < x * y; ++i) {
                output[i * comp + n] = data[i * comp + n] / 255.0f;
            }
        }
        STBI_FREE(data);
        return output;
    }
#endif

#ifndef STBI_NO_HDR
#define stbi__float2int(x)   ((int) (x))
    static stbi_uc* stbi__hdr_to_ldr(float* data, int x, int y, int comp)
    {
        int i, k, n;
        stbi_uc* output;
        if (!data) return NULL;
        output = (stbi_uc*)stbi__malloc_mad3(x, y, comp, 0);
        if (output == NULL) { STBI_FREE(data); return stbi__errpuc("outofmem", "Out of memory"); }
        // compute number of non-alpha components
        if (comp & 1) n = comp; else n = comp - 1;
        for (i = 0; i < x * y; ++i) {
            for (k = 0; k < n; ++k) {
                float z = (float)pow(data[i * comp + k] * stbi__h2l_scale_i, stbi__h2l_gamma_i) * 255 + 0.5f;
                if (z < 0) z = 0;
                if (z > 255) z = 255;
                output[i * comp + k] = (stbi_uc)stbi__float2int(z);
            }
            if (k < comp) {
                float z = data[i * comp + k] * 255 + 0.5f;
                if (z < 0) z = 0;
                if (z > 255) z = 255;
                output[i * comp + k] = (stbi_uc)stbi__float2int(z);
            }
        }
        STBI_FREE(data);
        return output;
    }
#endif

    //////////////////////////////////////////////////////////////////////////////
    //
    //  "baseline" JPEG/JFIF decoder
    //
    //    simple implementation
    //      - doesn't support delayed output of y-dimension
    //      - simple interface (only one output format: 8-bit interleaved RGB)
    //      - doesn't try to recover corrupt jpegs
    //      - doesn't allow partial loading, loading multiple at once
    //      - still fast on x86 (copying globals into locals doesn't help x86)
    //      - allocates lots of intermediate memory (full size of all components)
    //        - non-interleaved case requires this anyway
    //        - allows good upsampling (see next)
    //    high-quality
    //      - upsampled channels are bilinearly interpolated, even across blocks
    //      - quality integer IDCT derived from IJG's 'slow'
    //    performance
    //      - fast huffman; reasonable integer IDCT
    //      - some SIMD kernels for common paths on targets with SSE2/NEON
    //      - uses a lot of intermediate memory, could cache poorly

#ifndef STBI_NO_JPEG

// huffman decoding acceleration
#define FAST_BITS   9  // larger handles more cases; smaller stomps less cache

    typedef struct
    {
        stbi_uc  fast[1 << FAST_BITS];
        // weirdly, repacking this into AoS is a 10% speed loss, instead of a win
        stbi__uint16 code[256];
        stbi_uc  values[256];
        stbi_uc  size[257];
        unsigned int maxcode[18];
        int    delta[17];   // old 'firstsymbol' - old 'firstcode'
    } stbi__huffman;

    typedef struct
    {
        stbi__context* s;
        stbi__huffman huff_dc[4];
        stbi__huffman huff_ac[4];
        stbi__uint16 dequant[4][64];
        stbi__int16 fast_ac[4][1 << FAST_BITS];

        // sizes for components, interleaved MCUs
        int img_h_max, img_v_max;
        int img_mcu_x, img_mcu_y;
        int img_mcu_w, img_mcu_h;

        // definition of jpeg image component
        struct
        {
            int id;
            int h, v;
            int tq;
            int hd, ha;
            int dc_pred;

            int x, y, w2, h2;
            stbi_uc* data;
            void* raw_data, * raw_coeff;
            stbi_uc* linebuf;
            short* coeff;   // progressive only
            int      coeff_w, coeff_h; // number of 8x8 coefficient blocks
        } img_comp[4];

        stbi__uint32   code_buffer; // jpeg entropy-coded buffer
        int            code_bits;   // number of valid bits
        unsigned char  marker;      // marker seen while filling entropy buffer
        int            nomore;      // flag if we saw a marker so must stop

        int            progressive;
        int            spec_start;
        int            spec_end;
        int            succ_high;
        int            succ_low;
        int            eob_run;
        int            jfif;
        int            app14_color_transform; // Adobe APP14 tag
        int            rgb;

        int scan_n, order[4];
        int restart_interval, todo;

        // kernels
        void (*idct_block_kernel)(stbi_uc* out, int out_stride, short data[64]);
        void (*YCbCr_to_RGB_kernel)(stbi_uc* out, const stbi_uc* y, const stbi_uc* pcb, const stbi_uc* pcr, int count, int step);
        stbi_uc* (*resample_row_hv_2_kernel)(stbi_uc* out, stbi_uc* in_near, stbi_uc* in_far, int w, int hs);
    } stbi__jpeg;

    static int stbi__build_huffman(stbi__huffman* h, int* count)
    {
        int i, j, k = 0;
        unsigned int code;
        // build size list for each symbol (from JPEG spec)
        for (i = 0; i < 16; ++i)
            for (j = 0; j < count[i]; ++j)
                h->size[k++] = (stbi_uc)(i + 1);
        h->size[k] = 0;

        // compute actual symbols (from jpeg spec)
        code = 0;
        k = 0;
        for (j = 1; j <= 16; ++j) {
            // compute delta to add to code to compute symbol id
            h->delta[j] = k - code;
            if (h->size[k] == j) {
                while (h->size[k] == j)
                    h->code[k++] = (stbi__uint16)(code++);
                if (code - 1 >= (1u << j)) return stbi__err("bad code lengths", "Corrupt JPEG");
            }
            // compute largest code + 1 for this size, preshifted as needed later
            h->maxcode[j] = code << (16 - j);
            code <<= 1;
        }
        h->maxcode[j] = 0xffffffff;

        // build non-spec acceleration table; 255 is flag for not-accelerated
        memset(h->fast, 255, 1 << FAST_BITS);
        for (i = 0; i < k; ++i) {
            int s = h->size[i];
            if (s <= FAST_BITS) {
                int c = h->code[i] << (FAST_BITS - s);
                int m = 1 << (FAST_BITS - s);
                for (j = 0; j < m; ++j) {
                    h->fast[c + j] = (stbi_uc)i;
                }
            }
        }
        return 1;
    }

    // build a table that decodes both magnitude and value of small ACs in
    // one go.
    static void stbi__build_fast_ac(stbi__int16* fast_ac, stbi__huffman* h)
    {
        int i;
        for (i = 0; i < (1 << FAST_BITS); ++i) {
            stbi_uc fast = h->fast[i];
            fast_ac[i] = 0;
            if (fast < 255) {
                int rs = h->values[fast];
                int run = (rs >> 4) & 15;
                int magbits = rs & 15;
                int len = h->size[fast];

                if (magbits && len + magbits <= FAST_BITS) {
                    // magnitude code followed by receive_extend code
                    int k = ((i << len) & ((1 << FAST_BITS) - 1)) >> (FAST_BITS - magbits);
                    int m = 1 << (magbits - 1);
                    if (k < m) k += (~0U << magbits) + 1;
                    // if the result is small enough, we can fit it in fast_ac table
                    if (k >= -128 && k <= 127)
                        fast_ac[i] = (stbi__int16)((k * 256) + (run * 16) + (len + magbits));
                }
            }
        }
    }

    static void stbi__grow_buffer_unsafe(stbi__jpeg* j)
    {
        do {
            unsigned int b = j->nomore ? 0 : stbi__get8(j->s);
            if (b == 0xff) {
                int c = stbi__get8(j->s);
                while (c == 0xff) c = stbi__get8(j->s); // consume fill bytes
                if (c != 0) {
                    j->marker = (unsigned char)c;
                    j->nomore = 1;
                    return;
                }
            }
            j->code_buffer |= b << (24 - j->code_bits);
            j->code_bits += 8;
        } while (j->code_bits <= 24);
    }

    // (1 << n) - 1
    static const stbi__uint32 stbi__bmask[17] = { 0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535 };

    // decode a jpeg huffman value from the bitstream
    stbi_inline static int stbi__jpeg_huff_decode(stbi__jpeg* j, stbi__huffman* h)
    {
        unsigned int temp;
        int c, k;

        if (j->code_bits < 16) stbi__grow_buffer_unsafe(j);

        // look at the top FAST_BITS and determine what symbol ID it is,
        // if the code is <= FAST_BITS
        c = (j->code_buffer >> (32 - FAST_BITS)) & ((1 << FAST_BITS) - 1);
        k = h->fast[c];
        if (k < 255) {
            int s = h->size[k];
            if (s > j->code_bits)
                return -1;
            j->code_buffer <<= s;
            j->code_bits -= s;
            return h->values[k];
        }

        // naive test is to shift the code_buffer down so k bits are
        // valid, then test against maxcode. To speed this up, we've
        // preshifted maxcode left so that it has (16-k) 0s at the
        // end; in other words, regardless of the number of bits, it
        // wants to be compared against something shifted to have 16;
        // that way we don't need to shift inside the loop.
        temp = j->code_buffer >> 16;
        for (k = FAST_BITS + 1; ; ++k)
            if (temp < h->maxcode[k])
                break;
        if (k == 17) {
            // error! code not found
            j->code_bits -= 16;
            return -1;
        }

        if (k > j->code_bits)
            return -1;

        // convert the huffman code to the symbol id
        c = ((j->code_buffer >> (32 - k)) & stbi__bmask[k]) + h->delta[k];
        STBI_ASSERT((((j->code_buffer) >> (32 - h->size[c])) & stbi__bmask[h->size[c]]) == h->code[c]);

        // convert the id to a symbol
        j->code_bits -= k;
        j->code_buffer <<= k;
        return h->values[c];
    }

    // bias[n] = (-1<<n) + 1
    static const int stbi__jbias[16] = { 0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767 };

    // combined JPEG 'receive' and JPEG 'extend', since baseline
    // always extends everything it receives.
    stbi_inline static int stbi__extend_receive(stbi__jpeg* j, int n)
    {
        unsigned int k;
        int sgn;
        if (j->code_bits < n) stbi__grow_buffer_unsafe(j);

        sgn = j->code_buffer >> 31; // sign bit always in MSB; 0 if MSB clear (positive), 1 if MSB set (negative)
        k = stbi_lrot(j->code_buffer, n);
        j->code_buffer = k & ~stbi__bmask[n];
        k &= stbi__bmask[n];
        j->code_bits -= n;
        return k + (stbi__jbias[n] & (sgn - 1));
    }

    // get some unsigned bits
    stbi_inline static int stbi__jpeg_get_bits(stbi__jpeg* j, int n)
    {
        unsigned int k;
        if (j->code_bits < n) stbi__grow_buffer_unsafe(j);
        k = stbi_lrot(j->code_buffer, n);
        j->code_buffer = k & ~stbi__bmask[n];
        k &= stbi__bmask[n];
        j->code_bits -= n;
        return k;
    }

    stbi_inline static int stbi__jpeg_get_bit(stbi__jpeg* j)
    {
        unsigned int k;
        if (j->code_bits < 1) stbi__grow_buffer_unsafe(j);
        k = j->code_buffer;
        j->code_buffer <<= 1;
        --j->code_bits;
        return k & 0x80000000;
    }

    // given a value that's at position X in the zigzag stream,
    // where does it appear in the 8x8 matrix coded as row-major?
    static const stbi_uc stbi__jpeg_dezigzag[64 + 15] =
    {
        0,  1,  8, 16,  9,  2,  3, 10,
       17, 24, 32, 25, 18, 11,  4,  5,
       12, 19, 26, 33, 40, 48, 41, 34,
       27, 20, 13,  6,  7, 14, 21, 28,
       35, 42, 49, 56, 57, 50, 43, 36,
       29, 22, 15, 23, 30, 37, 44, 51,
       58, 59, 52, 45, 38, 31, 39, 46,
       53, 60, 61, 54, 47, 55, 62, 63,
       // let corrupt input sample past end
       63, 63, 63, 63, 63, 63, 63, 63,
       63, 63, 63, 63, 63, 63, 63
    };

    // decode one 64-entry block--
    static int stbi__jpeg_decode_block(stbi__jpeg* j, short data[64], stbi__huffman* hdc, stbi__huffman* hac, stbi__int16* fac, int b, stbi__uint16* dequant)
    {
        int diff, dc, k;
        int t;

        if (j->code_bits < 16) stbi__grow_buffer_unsafe(j);
        t = stbi__jpeg_huff_decode(j, hdc);
        if (t < 0 || t > 15) return stbi__err("bad huffman code", "Corrupt JPEG");

        // 0 all the ac values now so we can do it 32-bits at a time
        memset(data, 0, 64 * sizeof(data[0]));

        diff = t ? stbi__extend_receive(j, t) : 0;
        dc = j->img_comp[b].dc_pred + diff;
        j->img_comp[b].dc_pred = dc;
        data[0] = (short)(dc * dequant[0]);

        // decode AC components, see JPEG spec
        k = 1;
        do {
            unsigned int zig;
            int c, r, s;
            if (j->code_bits < 16) stbi__grow_buffer_unsafe(j);
            c = (j->code_buffer >> (32 - FAST_BITS)) & ((1 << FAST_BITS) - 1);
            r = fac[c];
            if (r) { // fast-AC path
                k += (r >> 4) & 15; // run
                s = r & 15; // combined length
                j->code_buffer <<= s;
                j->code_bits -= s;
                // decode into unzigzag'd location
                zig = stbi__jpeg_dezigzag[k++];
                data[zig] = (short)((r >> 8) * dequant[zig]);
            }
            else {
                int rs = stbi__jpeg_huff_decode(j, hac);
                if (rs < 0) return stbi__err("bad huffman code", "Corrupt JPEG");
                s = rs & 15;
                r = rs >> 4;
                if (s == 0) {
                    if (rs != 0xf0) break; // end block
                    k += 16;
                }
                else {
                    k += r;
                    // decode into unzigzag'd location
                    zig = stbi__jpeg_dezigzag[k++];
                    data[zig] = (short)(stbi__extend_receive(j, s) * dequant[zig]);
                }
            }
        } while (k < 64);
        return 1;
    }

    static int stbi__jpeg_decode_block_prog_dc(stbi__jpeg* j, short data[64], stbi__huffman* hdc, int b)
    {
        int diff, dc;
        int t;
        if (j->spec_end != 0) return stbi__err("can't merge dc and ac", "Corrupt JPEG");

        if (j->code_bits < 16) stbi__grow_buffer_unsafe(j);

        if (j->succ_high == 0) {
            // first scan for DC coefficient, must be first
            memset(data, 0, 64 * sizeof(data[0])); // 0 all the ac values now
            t = stbi__jpeg_huff_decode(j, hdc);
            if (t < 0 || t > 15) return stbi__err("can't merge dc and ac", "Corrupt JPEG");
            diff = t ? stbi__extend_receive(j, t) : 0;

            dc = j->img_comp[b].dc_pred + diff;
            j->img_comp[b].dc_pred = dc;
            data[0] = (short)(dc * (1 << j->succ_low));
        }
        else {
            // refinement scan for DC coefficient
            if (stbi__jpeg_get_bit(j))
                data[0] += (short)(1 << j->succ_low);
        }
        return 1;
    }

    // @OPTIMIZE: store non-zigzagged during the decode passes,
    // and only de-zigzag when dequantizing
    static int stbi__jpeg_decode_block_prog_ac(stbi__jpeg* j, short data[64], stbi__huffman* hac, stbi__int16* fac)
    {
        int k;
        if (j->spec_start == 0) return stbi__err("can't merge dc and ac", "Corrupt JPEG");

        if (j->succ_high == 0) {
            int shift = j->succ_low;

            if (j->eob_run) {
                --j->eob_run;
                return 1;
            }

            k = j->spec_start;
            do {
                unsigned int zig;
                int c, r, s;
                if (j->code_bits < 16) stbi__grow_buffer_unsafe(j);
                c = (j->code_buffer >> (32 - FAST_BITS)) & ((1 << FAST_BITS) - 1);
                r = fac[c];
                if (r) { // fast-AC path
                    k += (r >> 4) & 15; // run
                    s = r & 15; // combined length
                    j->code_buffer <<= s;
                    j->code_bits -= s;
                    zig = stbi__jpeg_dezigzag[k++];
                    data[zig] = (short)((r >> 8) * (1 << shift));
                }
                else {
                    int rs = stbi__jpeg_huff_decode(j, hac);
                    if (rs < 0) return stbi__err("bad huffman code", "Corrupt JPEG");
                    s = rs & 15;
                    r = rs >> 4;
                    if (s == 0) {
                        if (r < 15) {
                            j->eob_run = (1 << r);
                            if (r)
                                j->eob_run += stbi__jpeg_get_bits(j, r);
                            --j->eob_run;
                            break;
                        }
                        k += 16;
                    }
                    else {
                        k += r;
                        zig = stbi__jpeg_dezigzag[k++];
                        data[zig] = (short)(stbi__extend_receive(j, s) * (1 << shift));
                    }
                }
            } while (k <= j->spec_end);
        }
        else {
            // refinement scan for these AC coefficients

            short bit = (short)(1 << j->succ_low);

            if (j->eob_run) {
                --j->eob_run;
                for (k = j->spec_start; k <= j->spec_end; ++k) {
                    short* p = &data[stbi__jpeg_dezigzag[k]];
                    if (*p != 0)
                        if (stbi__jpeg_get_bit(j))
                            if ((*p & bit) == 0) {
                                if (*p > 0)
                                    *p += bit;
                                else
                                    *p -= bit;
                            }
                }
            }
            else {
                k = j->spec_start;
                do {
                    int r, s;
                    int rs = stbi__jpeg_huff_decode(j, hac); // @OPTIMIZE see if we can use the fast path here, advance-by-r is so slow, eh
                    if (rs < 0) return stbi__err("bad huffman code", "Corrupt JPEG");
                    s = rs & 15;
                    r = rs >> 4;
                    if (s == 0) {
                        if (r < 15) {
                            j->eob_run = (1 << r) - 1;
                            if (r)
                                j->eob_run += stbi__jpeg_get_bits(j, r);
                            r = 64; // force end of block
                        }
                        else {
                            // r=15 s=0 should write 16 0s, so we just do
                            // a run of 15 0s and then write s (which is 0),
                            // so we don't have to do anything special here
                        }
                    }
                    else {
                        if (s != 1) return stbi__err("bad huffman code", "Corrupt JPEG");
                        // sign bit
                        if (stbi__jpeg_get_bit(j))
                            s = bit;
                        else
                            s = -bit;
                    }

                    // advance by r
                    while (k <= j->spec_end) {
                        short* p = &data[stbi__jpeg_dezigzag[k++]];
                        if (*p != 0) {
                            if (stbi__jpeg_get_bit(j))
                                if ((*p & bit) == 0) {
                                    if (*p > 0)
                                        *p += bit;
                                    else
                                        *p -= bit;
                                }
                        }
                        else {
                            if (r == 0) {
                                *p = (short)s;
                                break;
                            }
                            --r;
                        }
                    }
                } while (k <= j->spec_end);
            }
        }
        return 1;
    }

    // take a -128..127 value and stbi__clamp it and convert to 0..255
    stbi_inline static stbi_uc stbi__clamp(int x)
    {
        // trick to use a single test to catch both cases
        if ((unsigned int)x > 255) {
            if (x < 0) return 0;
            if (x > 255) return 255;
        }
        return (stbi_uc)x;
    }

#define stbi__f2f(x)  ((int) (((x) * 4096 + 0.5)))
#define stbi__fsh(x)  ((x) * 4096)

    // derived from jidctint -- DCT_ISLOW
#define STBI__IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7) \
   int t0,t1,t2,t3,p1,p2,p3,p4,p5,x0,x1,x2,x3; \
   p2 = s2;                                    \
   p3 = s6;                                    \
   p1 = (p2+p3) * stbi__f2f(0.5411961f);       \
   t2 = p1 + p3*stbi__f2f(-1.847759065f);      \
   t3 = p1 + p2*stbi__f2f( 0.765366865f);      \
   p2 = s0;                                    \
   p3 = s4;                                    \
   t0 = stbi__fsh(p2+p3);                      \
   t1 = stbi__fsh(p2-p3);                      \
   x0 = t0+t3;                                 \
   x3 = t0-t3;                                 \
   x1 = t1+t2;                                 \
   x2 = t1-t2;                                 \
   t0 = s7;                                    \
   t1 = s5;                                    \
   t2 = s3;                                    \
   t3 = s1;                                    \
   p3 = t0+t2;                                 \
   p4 = t1+t3;                                 \
   p1 = t0+t3;                                 \
   p2 = t1+t2;                                 \
   p5 = (p3+p4)*stbi__f2f( 1.175875602f);      \
   t0 = t0*stbi__f2f( 0.298631336f);           \
   t1 = t1*stbi__f2f( 2.053119869f);           \
   t2 = t2*stbi__f2f( 3.072711026f);           \
   t3 = t3*stbi__f2f( 1.501321110f);           \
   p1 = p5 + p1*stbi__f2f(-0.899976223f);      \
   p2 = p5 + p2*stbi__f2f(-2.562915447f);      \
   p3 = p3*stbi__f2f(-1.961570560f);           \
   p4 = p4*stbi__f2f(-0.390180644f);           \
   t3 += p1+p4;                                \
   t2 += p2+p3;                                \
   t1 += p2+p4;                                \
   t0 += p1+p3;

    static void stbi__idct_block(stbi_uc* out, int out_stride, short data[64])
    {
        int i, val[64], * v = val;
        stbi_uc* o;
        short* d = data;

        // columns
        for (i = 0; i < 8; ++i, ++d, ++v) {
            // if all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing
            if (d[8] == 0 && d[16] == 0 && d[24] == 0 && d[32] == 0
                && d[40] == 0 && d[48] == 0 && d[56] == 0) {
                //    no shortcut                 0     seconds
                //    (1|2|3|4|5|6|7)==0          0     seconds
                //    all separate               -0.047 seconds
                //    1 && 2|3 && 4|5 && 6|7:    -0.047 seconds
                int dcterm = d[0] * 4;
                v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
            }
            else {
                STBI__IDCT_1D(d[0], d[8], d[16], d[24], d[32], d[40], d[48], d[56])
                    // constants scaled things up by 1<<12; let's bring them back
                    // down, but keep 2 extra bits of precision
                    x0 += 512; x1 += 512; x2 += 512; x3 += 512;
                v[0] = (x0 + t3) >> 10;
                v[56] = (x0 - t3) >> 10;
                v[8] = (x1 + t2) >> 10;
                v[48] = (x1 - t2) >> 10;
                v[16] = (x2 + t1) >> 10;
                v[40] = (x2 - t1) >> 10;
                v[24] = (x3 + t0) >> 10;
                v[32] = (x3 - t0) >> 10;
            }
        }

        for (i = 0, v = val, o = out; i < 8; ++i, v += 8, o += out_stride) {
            // no fast case since the first 1D IDCT spread components out
            STBI__IDCT_1D(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7])
                // constants scaled things up by 1<<12, plus we had 1<<2 from first
                // loop, plus horizontal and vertical each scale by sqrt(8) so together
                // we've got an extra 1<<3, so 1<<17 total we need to remove.
                // so we want to round that, which means adding 0.5 * 1<<17,
                // aka 65536. Also, we'll end up with -128 to 127 that we want
                // to encode as 0..255 by adding 128, so we'll add that before the shift
                x0 += 65536 + (128 << 17);
            x1 += 65536 + (128 << 17);
            x2 += 65536 + (128 << 17);
            x3 += 65536 + (128 << 17);
            // tried computing the shifts into temps, or'ing the temps to see
            // if any were out of range, but that was slower
            o[0] = stbi__clamp((x0 + t3) >> 17);
            o[7] = stbi__clamp((x0 - t3) >> 17);
            o[1] = stbi__clamp((x1 + t2) >> 17);
            o[6] = stbi__clamp((x1 - t2) >> 17);
            o[2] = stbi__clamp((x2 + t1) >> 17);
            o[5] = stbi__clamp((x2 - t1) >> 17);
            o[3] = stbi__clamp((x3 + t0) >> 17);
            o[4] = stbi__clamp((x3 - t0) >> 17);
        }
    }

#ifdef STBI_SSE2
    // sse2 integer IDCT. not the fastest possible implementation but it
    // produces bit-identical results to the generic C version so it's
    // fully "transparent".
    static void stbi__idct_simd(stbi_uc* out, int out_stride, short data[64])
    {
        // This is constructed to match our regular (generic) integer IDCT exactly.
        __m128i row0, row1, row2, row3, row4, row5, row6, row7;
        __m128i tmp;

        // dot product constant: even elems=x, odd elems=y
#define dct_const(x,y)  _mm_setr_epi16((x),(y),(x),(y),(x),(y),(x),(y))

// out(0) = c0[even]*x + c0[odd]*y   (c0, x, y 16-bit, out 32-bit)
// out(1) = c1[even]*x + c1[odd]*y
#define dct_rot(out0,out1, x,y,c0,c1) \
      __m128i c0##lo = _mm_unpacklo_epi16((x),(y)); \
      __m128i c0##hi = _mm_unpackhi_epi16((x),(y)); \
      __m128i out0##_l = _mm_madd_epi16(c0##lo, c0); \
      __m128i out0##_h = _mm_madd_epi16(c0##hi, c0); \
      __m128i out1##_l = _mm_madd_epi16(c0##lo, c1); \
      __m128i out1##_h = _mm_madd_epi16(c0##hi, c1)

   // out = in << 12  (in 16-bit, out 32-bit)
#define dct_widen(out, in) \
      __m128i out##_l = _mm_srai_epi32(_mm_unpacklo_epi16(_mm_setzero_si128(), (in)), 4); \
      __m128i out##_h = _mm_srai_epi32(_mm_unpackhi_epi16(_mm_setzero_si128(), (in)), 4)

   // wide add
#define dct_wadd(out, a, b) \
      __m128i out##_l = _mm_add_epi32(a##_l, b##_l); \
      __m128i out##_h = _mm_add_epi32(a##_h, b##_h)

   // wide sub
#define dct_wsub(out, a, b) \
      __m128i out##_l = _mm_sub_epi32(a##_l, b##_l); \
      __m128i out##_h = _mm_sub_epi32(a##_h, b##_h)

   // butterfly a/b, add bias, then shift by "s" and pack
#define dct_bfly32o(out0, out1, a,b,bias,s) \
      { \
         __m128i abiased_l = _mm_add_epi32(a##_l, bias); \
         __m128i abiased_h = _mm_add_epi32(a##_h, bias); \
         dct_wadd(sum, abiased, b); \
         dct_wsub(dif, abiased, b); \
         out0 = _mm_packs_epi32(_mm_srai_epi32(sum_l, s), _mm_srai_epi32(sum_h, s)); \
         out1 = _mm_packs_epi32(_mm_srai_epi32(dif_l, s), _mm_srai_epi32(dif_h, s)); \
      }

   // 8-bit interleave step (for transposes)
#define dct_interleave8(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi8(a, b); \
      b = _mm_unpackhi_epi8(tmp, b)

   // 16-bit interleave step (for transposes)
#define dct_interleave16(a, b) \
      tmp = a; \
      a = _mm_unpacklo_epi16(a, b); \
      b = _mm_unpackhi_epi16(tmp, b)

#define dct_pass(bias,shift) \
      { \
         /* even part */ \
         dct_rot(t2e,t3e, row2,row6, rot0_0,rot0_1); \
         __m128i sum04 = _mm_add_epi16(row0, row4); \
         __m128i dif04 = _mm_sub_epi16(row0, row4); \
         dct_widen(t0e, sum04); \
         dct_widen(t1e, dif04); \
         dct_wadd(x0, t0e, t3e); \
         dct_wsub(x3, t0e, t3e); \
         dct_wadd(x1, t1e, t2e); \
         dct_wsub(x2, t1e, t2e); \
         /* odd part */ \
         dct_rot(y0o,y2o, row7,row3, rot2_0,rot2_1); \
         dct_rot(y1o,y3o, row5,row1, rot3_0,rot3_1); \
         __m128i sum17 = _mm_add_epi16(row1, row7); \
         __m128i sum35 = _mm_add_epi16(row3, row5); \
         dct_rot(y4o,y5o, sum17,sum35, rot1_0,rot1_1); \
         dct_wadd(x4, y0o, y4o); \
         dct_wadd(x5, y1o, y5o); \
         dct_wadd(x6, y2o, y5o); \
         dct_wadd(x7, y3o, y4o); \
         dct_bfly32o(row0,row7, x0,x7,bias,shift); \
         dct_bfly32o(row1,row6, x1,x6,bias,shift); \
         dct_bfly32o(row2,row5, x2,x5,bias,shift); \
         dct_bfly32o(row3,row4, x3,x4,bias,shift); \
      }

        __m128i rot0_0 = dct_const(stbi__f2f(0.5411961f), stbi__f2f(0.5411961f) + stbi__f2f(-1.847759065f));
        __m128i rot0_1 = dct_const(stbi__f2f(0.5411961f) + stbi__f2f(0.765366865f), stbi__f2f(0.5411961f));
        __m128i rot1_0 = dct_const(stbi__f2f(1.175875602f) + stbi__f2f(-0.899976223f), stbi__f2f(1.175875602f));
        __m128i rot1_1 = dct_const(stbi__f2f(1.175875602f), stbi__f2f(1.175875602f) + stbi__f2f(-2.562915447f));
        __m128i rot2_0 = dct_const(stbi__f2f(-1.961570560f) + stbi__f2f(0.298631336f), stbi__f2f(-1.961570560f));
        __m128i rot2_1 = dct_const(stbi__f2f(-1.961570560f), stbi__f2f(-1.961570560f) + stbi__f2f(3.072711026f));
        __m128i rot3_0 = dct_const(stbi__f2f(-0.390180644f) + stbi__f2f(2.053119869f), stbi__f2f(-0.390180644f));
        __m128i rot3_1 = dct_const(stbi__f2f(-0.390180644f), stbi__f2f(-0.390180644f) + stbi__f2f(1.501321110f));

        // rounding biases in column/row passes, see stbi__idct_block for explanation.
        __m128i bias_0 = _mm_set1_epi32(512);
        __m128i bias_1 = _mm_set1_epi32(65536 + (128 << 17));

        // load
        row0 = _mm_load_si128((const __m128i*) (data + 0 * 8));
        row1 = _mm_load_si128((const __m128i*) (data + 1 * 8));
        row2 = _mm_load_si128((const __m128i*) (data + 2 * 8));
        row3 = _mm_load_si128((const __m128i*) (data + 3 * 8));
        row4 = _mm_load_si128((const __m128i*) (data + 4 * 8));
        row5 = _mm_load_si128((const __m128i*) (data + 5 * 8));
        row6 = _mm_load_si128((const __m128i*) (data + 6 * 8));
        row7 = _mm_load_si128((const __m128i*) (data + 7 * 8));

        // column pass
        dct_pass(bias_0, 10);

        {
            // 16bit 8x8 transpose pass 1
            dct_interleave16(row0, row4);
            dct_interleave16(row1, row5);
            dct_interleave16(row2, row6);
            dct_interleave16(row3, row7);

            // transpose pass 2
            dct_interleave16(row0, row2);
            dct_interleave16(row1, row3);
            dct_interleave16(row4, row6);
            dct_interleave16(row5, row7);

            // transpose pass 3
            dct_interleave16(row0, row1);
            dct_interleave16(row2, row3);
            dct_interleave16(row4, row5);
            dct_interleave16(row6, row7);
        }

        // row pass
        dct_pass(bias_1, 17);

        {
            // pack
            __m128i p0 = _mm_packus_epi16(row0, row1); // a0a1a2a3...a7b0b1b2b3...b7
            __m128i p1 = _mm_packus_epi16(row2, row3);
            __m128i p2 = _mm_packus_epi16(row4, row5);
            __m128i p3 = _mm_packus_epi16(row6, row7);

            // 8bit 8x8 transpose pass 1
            dct_interleave8(p0, p2); // a0e0a1e1...
            dct_interleave8(p1, p3); // c0g0c1g1...

            // transpose pass 2
            dct_interleave8(p0, p1); // a0c0e0g0...
            dct_interleave8(p2, p3); // b0d0f0h0...

            // transpose pass 3
            dct_interleave8(p0, p2); // a0b0c0d0...
            dct_interleave8(p1, p3); // a4b4c4d4...

            // store
            _mm_storel_epi64((__m128i*) out, p0); out += out_stride;
            _mm_storel_epi64((__m128i*) out, _mm_shuffle_epi32(p0, 0x4e)); out += out_stride;
            _mm_storel_epi64((__m128i*) out, p2); out += out_stride;
            _mm_storel_epi64((__m128i*) out, _mm_shuffle_epi32(p2, 0x4e)); out += out_stride;
            _mm_storel_epi64((__m128i*) out, p1); out += out_stride;
            _mm_storel_epi64((__m128i*) out, _mm_shuffle_epi32(p1, 0x4e)); out += out_stride;
            _mm_storel_epi64((__m128i*) out, p3); out += out_stride;
            _mm_storel_epi64((__m128i*) out, _mm_shuffle_epi32(p3, 0x4e));
        }

#undef dct_const
#undef dct_rot
#undef dct_widen
#undef dct_wadd
#undef dct_wsub
#undef dct_bfly32o
#undef dct_interleave8
#undef dct_interleave16
#undef dct_pass
    }

#endif // STBI_SSE2

#ifdef STBI_NEON

    // NEON integer IDCT. should produce bit-identical
    // results to the generic C version.
    static void stbi__idct_simd(stbi_uc* out, int out_stride, short data[64])
    {
        int16x8_t row0, row1, row2, row3, row4, row5, row6, row7;

        int16x4_t rot0_0 = vdup_n_s16(stbi__f2f(0.5411961f));
        int16x4_t rot0_1 = vdup_n_s16(stbi__f2f(-1.847759065f));
        int16x4_t rot0_2 = vdup_n_s16(stbi__f2f(0.765366865f));
        int16x4_t rot1_0 = vdup_n_s16(stbi__f2f(1.175875602f));
        int16x4_t rot1_1 = vdup_n_s16(stbi__f2f(-0.899976223f));
        int16x4_t rot1_2 = vdup_n_s16(stbi__f2f(-2.562915447f));
        int16x4_t rot2_0 = vdup_n_s16(stbi__f2f(-1.961570560f));
        int16x4_t rot2_1 = vdup_n_s16(stbi__f2f(-0.390180644f));
        int16x4_t rot3_0 = vdup_n_s16(stbi__f2f(0.298631336f));
        int16x4_t rot3_1 = vdup_n_s16(stbi__f2f(2.053119869f));
        int16x4_t rot3_2 = vdup_n_s16(stbi__f2f(3.072711026f));
        int16x4_t rot3_3 = vdup_n_s16(stbi__f2f(1.501321110f));

#define dct_long_mul(out, inq, coeff) \
   int32x4_t out##_l = vmull_s16(vget_low_s16(inq), coeff); \
   int32x4_t out##_h = vmull_s16(vget_high_s16(inq), coeff)

#define dct_long_mac(out, acc, inq, coeff) \
   int32x4_t out##_l = vmlal_s16(acc##_l, vget_low_s16(inq), coeff); \
   int32x4_t out##_h = vmlal_s16(acc##_h, vget_high_s16(inq), coeff)

#define dct_widen(out, inq) \
   int32x4_t out##_l = vshll_n_s16(vget_low_s16(inq), 12); \
   int32x4_t out##_h = vshll_n_s16(vget_high_s16(inq), 12)

        // wide add
#define dct_wadd(out, a, b) \
   int32x4_t out##_l = vaddq_s32(a##_l, b##_l); \
   int32x4_t out##_h = vaddq_s32(a##_h, b##_h)

// wide sub
#define dct_wsub(out, a, b) \
   int32x4_t out##_l = vsubq_s32(a##_l, b##_l); \
   int32x4_t out##_h = vsubq_s32(a##_h, b##_h)

// butterfly a/b, then shift using "shiftop" by "s" and pack
#define dct_bfly32o(out0,out1, a,b,shiftop,s) \
   { \
      dct_wadd(sum, a, b); \
      dct_wsub(dif, a, b); \
      out0 = vcombine_s16(shiftop(sum_l, s), shiftop(sum_h, s)); \
      out1 = vcombine_s16(shiftop(dif_l, s), shiftop(dif_h, s)); \
   }

#define dct_pass(shiftop, shift) \
   { \
      /* even part */ \
      int16x8_t sum26 = vaddq_s16(row2, row6); \
      dct_long_mul(p1e, sum26, rot0_0); \
      dct_long_mac(t2e, p1e, row6, rot0_1); \
      dct_long_mac(t3e, p1e, row2, rot0_2); \
      int16x8_t sum04 = vaddq_s16(row0, row4); \
      int16x8_t dif04 = vsubq_s16(row0, row4); \
      dct_widen(t0e, sum04); \
      dct_widen(t1e, dif04); \
      dct_wadd(x0, t0e, t3e); \
      dct_wsub(x3, t0e, t3e); \
      dct_wadd(x1, t1e, t2e); \
      dct_wsub(x2, t1e, t2e); \
      /* odd part */ \
      int16x8_t sum15 = vaddq_s16(row1, row5); \
      int16x8_t sum17 = vaddq_s16(row1, row7); \
      int16x8_t sum35 = vaddq_s16(row3, row5); \
      int16x8_t sum37 = vaddq_s16(row3, row7); \
      int16x8_t sumodd = vaddq_s16(sum17, sum35); \
      dct_long_mul(p5o, sumodd, rot1_0); \
      dct_long_mac(p1o, p5o, sum17, rot1_1); \
      dct_long_mac(p2o, p5o, sum35, rot1_2); \
      dct_long_mul(p3o, sum37, rot2_0); \
      dct_long_mul(p4o, sum15, rot2_1); \
      dct_wadd(sump13o, p1o, p3o); \
      dct_wadd(sump24o, p2o, p4o); \
      dct_wadd(sump23o, p2o, p3o); \
      dct_wadd(sump14o, p1o, p4o); \
      dct_long_mac(x4, sump13o, row7, rot3_0); \
      dct_long_mac(x5, sump24o, row5, rot3_1); \
      dct_long_mac(x6, sump23o, row3, rot3_2); \
      dct_long_mac(x7, sump14o, row1, rot3_3); \
      dct_bfly32o(row0,row7, x0,x7,shiftop,shift); \
      dct_bfly32o(row1,row6, x1,x6,shiftop,shift); \
      dct_bfly32o(row2,row5, x2,x5,shiftop,shift); \
      dct_bfly32o(row3,row4, x3,x4,shiftop,shift); \
   }

   // load
        row0 = vld1q_s16(data + 0 * 8);
        row1 = vld1q_s16(data + 1 * 8);
        row2 = vld1q_s16(data + 2 * 8);
        row3 = vld1q_s16(data + 3 * 8);
        row4 = vld1q_s16(data + 4 * 8);
        row5 = vld1q_s16(data + 5 * 8);
        row6 = vld1q_s16(data + 6 * 8);
        row7 = vld1q_s16(data + 7 * 8);

        // add DC bias
        row0 = vaddq_s16(row0, vsetq_lane_s16(1024, vdupq_n_s16(0), 0));

        // column pass
        dct_pass(vrshrn_n_s32, 10);

        // 16bit 8x8 transpose
        {
            // these three map to a single VTRN.16, VTRN.32, and VSWP, respectively.
            // whether compilers actually get this is another story, sadly.
#define dct_trn16(x, y) { int16x8x2_t t = vtrnq_s16(x, y); x = t.val[0]; y = t.val[1]; }
#define dct_trn32(x, y) { int32x4x2_t t = vtrnq_s32(vreinterpretq_s32_s16(x), vreinterpretq_s32_s16(y)); x = vreinterpretq_s16_s32(t.val[0]); y = vreinterpretq_s16_s32(t.val[1]); }
#define dct_trn64(x, y) { int16x8_t x0 = x; int16x8_t y0 = y; x = vcombine_s16(vget_low_s16(x0), vget_low_s16(y0)); y = vcombine_s16(vget_high_s16(x0), vget_high_s16(y0)); }

      // pass 1
            dct_trn16(row0, row1); // a0b0a2b2a4b4a6b6
            dct_trn16(row2, row3);
            dct_trn16(row4, row5);
            dct_trn16(row6, row7);

            // pass 2
            dct_trn32(row0, row2); // a0b0c0d0a4b4c4d4
            dct_trn32(row1, row3);
            dct_trn32(row4, row6);
            dct_trn32(row5, row7);

            // pass 3
            dct_trn64(row0, row4); // a0b0c0d0e0f0g0h0
            dct_trn64(row1, row5);
            dct_trn64(row2, row6);
            dct_trn64(row3, row7);

#undef dct_trn16
#undef dct_trn32
#undef dct_trn64
        }

        // row pass
        // vrshrn_n_s32 only supports shifts up to 16, we need
        // 17. so do a non-rounding shift of 16 first then follow
        // up with a rounding shift by 1.
        dct_pass(vshrn_n_s32, 16);

        {
            // pack and round
            uint8x8_t p0 = vqrshrun_n_s16(row0, 1);
            uint8x8_t p1 = vqrshrun_n_s16(row1, 1);
            uint8x8_t p2 = vqrshrun_n_s16(row2, 1);
            uint8x8_t p3 = vqrshrun_n_s16(row3, 1);
            uint8x8_t p4 = vqrshrun_n_s16(row4, 1);
            uint8x8_t p5 = vqrshrun_n_s16(row5, 1);
            uint8x8_t p6 = vqrshrun_n_s16(row6, 1);
            uint8x8_t p7 = vqrshrun_n_s16(row7, 1);

            // again, these can translate into one instruction, but often don't.
#define dct_trn8_8(x, y) { uint8x8x2_t t = vtrn_u8(x, y); x = t.val[0]; y = t.val[1]; }
#define dct_trn8_16(x, y) { uint16x4x2_t t = vtrn_u16(vreinterpret_u16_u8(x), vreinterpret_u16_u8(y)); x = vreinterpret_u8_u16(t.val[0]); y = vreinterpret_u8_u16(t.val[1]); }
#define dct_trn8_32(x, y) { uint32x2x2_t t = vtrn_u32(vreinterpret_u32_u8(x), vreinterpret_u32_u8(y)); x = vreinterpret_u8_u32(t.val[0]); y = vreinterpret_u8_u32(t.val[1]); }

      // sadly can't use interleaved stores here since we only write
      // 8 bytes to each scan line!

      // 8x8 8-bit transpose pass 1
            dct_trn8_8(p0, p1);
            dct_trn8_8(p2, p3);
            dct_trn8_8(p4, p5);
            dct_trn8_8(p6, p7);

            // pass 2
            dct_trn8_16(p0, p2);
            dct_trn8_16(p1, p3);
            dct_trn8_16(p4, p6);
            dct_trn8_16(p5, p7);

            // pass 3
            dct_trn8_32(p0, p4);
            dct_trn8_32(p1, p5);
            dct_trn8_32(p2, p6);
            dct_trn8_32(p3, p7);

            // store
            vst1_u8(out, p0); out += out_stride;
            vst1_u8(out, p1); out += out_stride;
            vst1_u8(out, p2); out += out_stride;
            vst1_u8(out, p3); out += out_stride;
            vst1_u8(out, p4); out += out_stride;
            vst1_u8(out, p5); out += out_stride;
            vst1_u8(out, p6); out += out_stride;
            vst1_u8(out, p7);

#undef dct_trn8_8
#undef dct_trn8_16
#undef dct_trn8_32
        }

#undef dct_long_mul
#undef dct_long_mac
#undef dct_widen
#undef dct_wadd
#undef dct_wsub
#undef dct_bfly32o
#undef dct_pass
    }

#endif // STBI_NEON

#define STBI__MARKER_none  0xff
    // if there's a pending marker from the entropy stream, return that
    // otherwise, fetch from the stream and get a marker. if there's no
    // marker, return 0xff, which is never a valid marker value
    static stbi_uc stbi__get_marker(stbi__jpeg* j)
    {
        stbi_uc x;
        if (j->marker != STBI__MARKER_none) { x = j->marker; j->marker = STBI__MARKER_none; return x; }
        x = stbi__get8(j->s);
        if (x != 0xff) return STBI__MARKER_none;
        while (x == 0xff)
            x = stbi__get8(j->s); // consume repeated 0xff fill bytes
        return x;
    }

    // in each scan, we'll have scan_n components, and the order
    // of the components is specified by order[]
#define STBI__RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)

// after a restart interval, stbi__jpeg_reset the entropy decoder and
// the dc prediction
    static void stbi__jpeg_reset(stbi__jpeg* j)
    {
        j->code_bits = 0;
        j->code_buffer = 0;
        j->nomore = 0;
        j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = j->img_comp[3].dc_pred = 0;
        j->marker = STBI__MARKER_none;
        j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
        j->eob_run = 0;
        // no more than 1<<31 MCUs if no restart_interal? that's plenty safe,
        // since we don't even allow 1<<30 pixels
    }

    static int stbi__parse_entropy_coded_data(stbi__jpeg* z)
    {
        stbi__jpeg_reset(z);
        if (!z->progressive) {
            if (z->scan_n == 1) {
                int i, j;
                STBI_SIMD_ALIGN(short, data[64]);
                int n = z->order[0];
                // non-interleaved data, we just need to process one block at a time,
                // in trivial scanline order
                // number of blocks to do just depends on how many actual "pixels" this
                // component has, independent of interleaved MCU blocking and such
                int w = (z->img_comp[n].x + 7) >> 3;
                int h = (z->img_comp[n].y + 7) >> 3;
                for (j = 0; j < h; ++j) {
                    for (i = 0; i < w; ++i) {
                        int ha = z->img_comp[n].ha;
                        if (!stbi__jpeg_decode_block(z, data, z->huff_dc + z->img_comp[n].hd, z->huff_ac + ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) return 0;
                        z->idct_block_kernel(z->img_comp[n].data + z->img_comp[n].w2 * j * 8 + i * 8, z->img_comp[n].w2, data);
                        // every data block is an MCU, so countdown the restart interval
                        if (--z->todo <= 0) {
                            if (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                            // if it's NOT a restart, then just bail, so we get corrupt data
                            // rather than no data
                            if (!STBI__RESTART(z->marker)) return 1;
                            stbi__jpeg_reset(z);
                        }
                    }
                }
                return 1;
            }
            else { // interleaved
                int i, j, k, x, y;
                STBI_SIMD_ALIGN(short, data[64]);
                for (j = 0; j < z->img_mcu_y; ++j) {
                    for (i = 0; i < z->img_mcu_x; ++i) {
                        // scan an interleaved mcu... process scan_n components in order
                        for (k = 0; k < z->scan_n; ++k) {
                            int n = z->order[k];
                            // scan out an mcu's worth of this component; that's just determined
                            // by the basic H and V specified for the component
                            for (y = 0; y < z->img_comp[n].v; ++y) {
                                for (x = 0; x < z->img_comp[n].h; ++x) {
                                    int x2 = (i * z->img_comp[n].h + x) * 8;
                                    int y2 = (j * z->img_comp[n].v + y) * 8;
                                    int ha = z->img_comp[n].ha;
                                    if (!stbi__jpeg_decode_block(z, data, z->huff_dc + z->img_comp[n].hd, z->huff_ac + ha, z->fast_ac[ha], n, z->dequant[z->img_comp[n].tq])) return 0;
                                    z->idct_block_kernel(z->img_comp[n].data + z->img_comp[n].w2 * y2 + x2, z->img_comp[n].w2, data);
                                }
                            }
                        }
                        // after all interleaved components, that's an interleaved MCU,
                        // so now count down the restart interval
                        if (--z->todo <= 0) {
                            if (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                            if (!STBI__RESTART(z->marker)) return 1;
                            stbi__jpeg_reset(z);
                        }
                    }
                }
                return 1;
            }
        }
        else {
            if (z->scan_n == 1) {
                int i, j;
                int n = z->order[0];
                // non-interleaved data, we just need to process one block at a time,
                // in trivial scanline order
                // number of blocks to do just depends on how many actual "pixels" this
                // component has, independent of interleaved MCU blocking and such
                int w = (z->img_comp[n].x + 7) >> 3;
                int h = (z->img_comp[n].y + 7) >> 3;
                for (j = 0; j < h; ++j) {
                    for (i = 0; i < w; ++i) {
                        short* data = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
                        if (z->spec_start == 0) {
                            if (!stbi__jpeg_decode_block_prog_dc(z, data, &z->huff_dc[z->img_comp[n].hd], n))
                                return 0;
                        }
                        else {
                            int ha = z->img_comp[n].ha;
                            if (!stbi__jpeg_decode_block_prog_ac(z, data, &z->huff_ac[ha], z->fast_ac[ha]))
                                return 0;
                        }
                        // every data block is an MCU, so countdown the restart interval
                        if (--z->todo <= 0) {
                            if (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                            if (!STBI__RESTART(z->marker)) return 1;
                            stbi__jpeg_reset(z);
                        }
                    }
                }
                return 1;
            }
            else { // interleaved
                int i, j, k, x, y;
                for (j = 0; j < z->img_mcu_y; ++j) {
                    for (i = 0; i < z->img_mcu_x; ++i) {
                        // scan an interleaved mcu... process scan_n components in order
                        for (k = 0; k < z->scan_n; ++k) {
                            int n = z->order[k];
                            // scan out an mcu's worth of this component; that's just determined
                            // by the basic H and V specified for the component
                            for (y = 0; y < z->img_comp[n].v; ++y) {
                                for (x = 0; x < z->img_comp[n].h; ++x) {
                                    int x2 = (i * z->img_comp[n].h + x);
                                    int y2 = (j * z->img_comp[n].v + y);
                                    short* data = z->img_comp[n].coeff + 64 * (x2 + y2 * z->img_comp[n].coeff_w);
                                    if (!stbi__jpeg_decode_block_prog_dc(z, data, &z->huff_dc[z->img_comp[n].hd], n))
                                        return 0;
                                }
                            }
                        }
                        // after all interleaved components, that's an interleaved MCU,
                        // so now count down the restart interval
                        if (--z->todo <= 0) {
                            if (z->code_bits < 24) stbi__grow_buffer_unsafe(z);
                            if (!STBI__RESTART(z->marker)) return 1;
                            stbi__jpeg_reset(z);
                        }
                    }
                }
                return 1;
            }
        }
    }

    static void stbi__jpeg_dequantize(short* data, stbi__uint16* dequant)
    {
        int i;
        for (i = 0; i < 64; ++i)
            data[i] *= dequant[i];
    }

    static void stbi__jpeg_finish(stbi__jpeg* z)
    {
        if (z->progressive) {
            // dequantize and idct the data
            int i, j, n;
            for (n = 0; n < z->s->img_n; ++n) {
                int w = (z->img_comp[n].x + 7) >> 3;
                int h = (z->img_comp[n].y + 7) >> 3;
                for (j = 0; j < h; ++j) {
                    for (i = 0; i < w; ++i) {
                        short* data = z->img_comp[n].coeff + 64 * (i + j * z->img_comp[n].coeff_w);
                        stbi__jpeg_dequantize(data, z->dequant[z->img_comp[n].tq]);
                        z->idct_block_kernel(z->img_comp[n].data + z->img_comp[n].w2 * j * 8 + i * 8, z->img_comp[n].w2, data);
                    }
                }
            }
        }
    }

    static int stbi__process_marker(stbi__jpeg* z, int m)
    {
        int L;
        switch (m) {
        case STBI__MARKER_none: // no marker found
            return stbi__err("expected marker", "Corrupt JPEG");

        case 0xDD: // DRI - specify restart interval
            if (stbi__get16be(z->s) != 4) return stbi__err("bad DRI len", "Corrupt JPEG");
            z->restart_interval = stbi__get16be(z->s);
            return 1;

        case 0xDB: // DQT - define quantization table
            L = stbi__get16be(z->s) - 2;
            while (L > 0) {
                int q = stbi__get8(z->s);
                int p = q >> 4, sixteen = (p != 0);
                int t = q & 15, i;
                if (p != 0 && p != 1) return stbi__err("bad DQT type", "Corrupt JPEG");
                if (t > 3) return stbi__err("bad DQT table", "Corrupt JPEG");

                for (i = 0; i < 64; ++i)
                    z->dequant[t][stbi__jpeg_dezigzag[i]] = (stbi__uint16)(sixteen ? stbi__get16be(z->s) : stbi__get8(z->s));
                L -= (sixteen ? 129 : 65);
            }
            return L == 0;

        case 0xC4: // DHT - define huffman table
            L = stbi__get16be(z->s) - 2;
            while (L > 0) {
                stbi_uc* v;
                int sizes[16], i, n = 0;
                int q = stbi__get8(z->s);
                int tc = q >> 4;
                int th = q & 15;
                if (tc > 1 || th > 3) return stbi__err("bad DHT header", "Corrupt JPEG");
                for (i = 0; i < 16; ++i) {
                    sizes[i] = stbi__get8(z->s);
                    n += sizes[i];
                }
                L -= 17;
                if (tc == 0) {
                    if (!stbi__build_huffman(z->huff_dc + th, sizes)) return 0;
                    v = z->huff_dc[th].values;
                }
                else {
                    if (!stbi__build_huffman(z->huff_ac + th, sizes)) return 0;
                    v = z->huff_ac[th].values;
                }
                for (i = 0; i < n; ++i)
                    v[i] = stbi__get8(z->s);
                if (tc != 0)
                    stbi__build_fast_ac(z->fast_ac[th], z->huff_ac + th);
                L -= n;
            }
            return L == 0;
        }

        // check for comment block or APP blocks
        if ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
            L = stbi__get16be(z->s);
            if (L < 2) {
                if (m == 0xFE)
                    return stbi__err("bad COM len", "Corrupt JPEG");
                else
                    return stbi__err("bad APP len", "Corrupt JPEG");
            }
            L -= 2;

            if (m == 0xE0 && L >= 5) { // JFIF APP0 segment
                static const unsigned char tag[5] = { 'J','F','I','F','\0' };
                int ok = 1;
                int i;
                for (i = 0; i < 5; ++i)
                    if (stbi__get8(z->s) != tag[i])
                        ok = 0;
                L -= 5;
                if (ok)
                    z->jfif = 1;
            }
            else if (m == 0xEE && L >= 12) { // Adobe APP14 segment
                static const unsigned char tag[6] = { 'A','d','o','b','e','\0' };
                int ok = 1;
                int i;
                for (i = 0; i < 6; ++i)
                    if (stbi__get8(z->s) != tag[i])
                        ok = 0;
                L -= 6;
                if (ok) {
                    stbi__get8(z->s); // version
                    stbi__get16be(z->s); // flags0
                    stbi__get16be(z->s); // flags1
                    z->app14_color_transform = stbi__get8(z->s); // color transform
                    L -= 6;
                }
            }

            stbi__skip(z->s, L);
            return 1;
        }

        return stbi__err("unknown marker", "Corrupt JPEG");
    }

    // after we see SOS
    static int stbi__process_scan_header(stbi__jpeg* z)
    {
        int i;
        int Ls = stbi__get16be(z->s);
        z->scan_n = stbi__get8(z->s);
        if (z->scan_n < 1 || z->scan_n > 4 || z->scan_n > (int)z->s->img_n) return stbi__err("bad SOS component count", "Corrupt JPEG");
        if (Ls != 6 + 2 * z->scan_n) return stbi__err("bad SOS len", "Corrupt JPEG");
        for (i = 0; i < z->scan_n; ++i) {
            int id = stbi__get8(z->s), which;
            int q = stbi__get8(z->s);
            for (which = 0; which < z->s->img_n; ++which)
                if (z->img_comp[which].id == id)
                    break;
            if (which == z->s->img_n) return 0; // no match
            z->img_comp[which].hd = q >> 4;   if (z->img_comp[which].hd > 3) return stbi__err("bad DC huff", "Corrupt JPEG");
            z->img_comp[which].ha = q & 15;   if (z->img_comp[which].ha > 3) return stbi__err("bad AC huff", "Corrupt JPEG");
            z->order[i] = which;
        }

        {
            int aa;
            z->spec_start = stbi__get8(z->s);
            z->spec_end = stbi__get8(z->s); // should be 63, but might be 0
            aa = stbi__get8(z->s);
            z->succ_high = (aa >> 4);
            z->succ_low = (aa & 15);
            if (z->progressive) {
                if (z->spec_start > 63 || z->spec_end > 63 || z->spec_start > z->spec_end || z->succ_high > 13 || z->succ_low > 13)
                    return stbi__err("bad SOS", "Corrupt JPEG");
            }
            else {
                if (z->spec_start != 0) return stbi__err("bad SOS", "Corrupt JPEG");
                if (z->succ_high != 0 || z->succ_low != 0) return stbi__err("bad SOS", "Corrupt JPEG");
                z->spec_end = 63;
            }
        }

        return 1;
    }

    static int stbi__free_jpeg_components(stbi__jpeg* z, int ncomp, int why)
    {
        int i;
        for (i = 0; i < ncomp; ++i) {
            if (z->img_comp[i].raw_data) {
                STBI_FREE(z->img_comp[i].raw_data);
                z->img_comp[i].raw_data = NULL;
                z->img_comp[i].data = NULL;
            }
            if (z->img_comp[i].raw_coeff) {
                STBI_FREE(z->img_comp[i].raw_coeff);
                z->img_comp[i].raw_coeff = 0;
                z->img_comp[i].coeff = 0;
            }
            if (z->img_comp[i].linebuf) {
                STBI_FREE(z->img_comp[i].linebuf);
                z->img_comp[i].linebuf = NULL;
            }
        }
        return why;
    }

    static int stbi__process_frame_header(stbi__jpeg* z, int scan)
    {
        stbi__context* s = z->s;
        int Lf, p, i, q, h_max = 1, v_max = 1, c;
        Lf = stbi__get16be(s);         if (Lf < 11) return stbi__err("bad SOF len", "Corrupt JPEG"); // JPEG
        p = stbi__get8(s);            if (p != 8) return stbi__err("only 8-bit", "JPEG format not supported: 8-bit only"); // JPEG baseline
        s->img_y = stbi__get16be(s);   if (s->img_y == 0) return stbi__err("no header height", "JPEG format not supported: delayed height"); // Legal, but we don't handle it--but neither does IJG
        s->img_x = stbi__get16be(s);   if (s->img_x == 0) return stbi__err("0 width", "Corrupt JPEG"); // JPEG requires
        if (s->img_y > STBI_MAX_DIMENSIONS) return stbi__err("too large", "Very large image (corrupt?)");
        if (s->img_x > STBI_MAX_DIMENSIONS) return stbi__err("too large", "Very large image (corrupt?)");
        c = stbi__get8(s);
        if (c != 3 && c != 1 && c != 4) return stbi__err("bad component count", "Corrupt JPEG");
        s->img_n = c;
        for (i = 0; i < c; ++i) {
            z->img_comp[i].data = NULL;
            z->img_comp[i].linebuf = NULL;
        }

        if (Lf != 8 + 3 * s->img_n) return stbi__err("bad SOF len", "Corrupt JPEG");

        z->rgb = 0;
        for (i = 0; i < s->img_n; ++i) {
            static const unsigned char rgb[3] = { 'R', 'G', 'B' };
            z->img_comp[i].id = stbi__get8(s);
            if (s->img_n == 3 && z->img_comp[i].id == rgb[i])
                ++z->rgb;
            q = stbi__get8(s);
            z->img_comp[i].h = (q >> 4);  if (!z->img_comp[i].h || z->img_comp[i].h > 4) return stbi__err("bad H", "Corrupt JPEG");
            z->img_comp[i].v = q & 15;    if (!z->img_comp[i].v || z->img_comp[i].v > 4) return stbi__err("bad V", "Corrupt JPEG");
            z->img_comp[i].tq = stbi__get8(s);  if (z->img_comp[i].tq > 3) return stbi__err("bad TQ", "Corrupt JPEG");
        }

        if (scan != STBI__SCAN_load) return 1;

        if (!stbi__mad3sizes_valid(s->img_x, s->img_y, s->img_n, 0)) return stbi__err("too large", "Image too large to decode");

        for (i = 0; i < s->img_n; ++i) {
            if (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
            if (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
        }

        // check that plane subsampling factors are integer ratios; our resamplers can't deal with fractional ratios
        // and I've never seen a non-corrupted JPEG file actually use them
        for (i = 0; i < s->img_n; ++i) {
            if (h_max % z->img_comp[i].h != 0) return stbi__err("bad H", "Corrupt JPEG");
            if (v_max % z->img_comp[i].v != 0) return stbi__err("bad V", "Corrupt JPEG");
        }

        // compute interleaved mcu info
        z->img_h_max = h_max;
        z->img_v_max = v_max;
        z->img_mcu_w = h_max * 8;
        z->img_mcu_h = v_max * 8;
        // these sizes can't be more than 17 bits
        z->img_mcu_x = (s->img_x + z->img_mcu_w - 1) / z->img_mcu_w;
        z->img_mcu_y = (s->img_y + z->img_mcu_h - 1) / z->img_mcu_h;

        for (i = 0; i < s->img_n; ++i) {
            // number of effective pixels (e.g. for non-interleaved MCU)
            z->img_comp[i].x = (s->img_x * z->img_comp[i].h + h_max - 1) / h_max;
            z->img_comp[i].y = (s->img_y * z->img_comp[i].v + v_max - 1) / v_max;
            // to simplify generation, we'll allocate enough memory to decode
            // the bogus oversized data from using interleaved MCUs and their
            // big blocks (e.g. a 16x16 iMCU on an image of width 33); we won't
            // discard the extra data until colorspace conversion
            //
            // img_mcu_x, img_mcu_y: <=17 bits; comp[i].h and .v are <=4 (checked earlier)
            // so these muls can't overflow with 32-bit ints (which we require)
            z->img_comp[i].w2 = z->img_mcu_x * z->img_comp[i].h * 8;
            z->img_comp[i].h2 = z->img_mcu_y * z->img_comp[i].v * 8;
            z->img_comp[i].coeff = 0;
            z->img_comp[i].raw_coeff = 0;
            z->img_comp[i].linebuf = NULL;
            z->img_comp[i].raw_data = stbi__malloc_mad2(z->img_comp[i].w2, z->img_comp[i].h2, 15);
            if (z->img_comp[i].raw_data == NULL)
                return stbi__free_jpeg_components(z, i + 1, stbi__err("outofmem", "Out of memory"));
            // align blocks for idct using mmx/sse
            z->img_comp[i].data = (stbi_uc*)(((size_t)z->img_comp[i].raw_data + 15) & ~15);
            if (z->progressive) {
                // w2, h2 are multiples of 8 (see above)
                z->img_comp[i].coeff_w = z->img_comp[i].w2 / 8;
                z->img_comp[i].coeff_h = z->img_comp[i].h2 / 8;
                z->img_comp[i].raw_coeff = stbi__malloc_mad3(z->img_comp[i].w2, z->img_comp[i].h2, sizeof(short), 15);
                if (z->img_comp[i].raw_coeff == NULL)
                    return stbi__free_jpeg_components(z, i + 1, stbi__err("outofmem", "Out of memory"));
                z->img_comp[i].coeff = (short*)(((size_t)z->img_comp[i].raw_coeff + 15) & ~15);
            }
        }

        return 1;
    }

    // use comparisons since in some cases we handle more than one case (e.g. SOF)
#define stbi__DNL(x)         ((x) == 0xdc)
#define stbi__SOI(x)         ((x) == 0xd8)
#define stbi__EOI(x)         ((x) == 0xd9)
#define stbi__SOF(x)         ((x) == 0xc0 || (x) == 0xc1 || (x) == 0xc2)
#define stbi__SOS(x)         ((x) == 0xda)

#define stbi__SOF_progressive(x)   ((x) == 0xc2)

    static int stbi__decode_jpeg_header(stbi__jpeg* z, int scan)
    {
        int m;
        z->jfif = 0;
        z->app14_color_transform = -1; // valid values are 0,1,2
        z->marker = STBI__MARKER_none; // initialize cached marker to empty
        m = stbi__get_marker(z);
        if (!stbi__SOI(m)) return stbi__err("no SOI", "Corrupt JPEG");
        if (scan == STBI__SCAN_type) return 1;
        m = stbi__get_marker(z);
        while (!stbi__SOF(m)) {
            if (!stbi__process_marker(z, m)) return 0;
            m = stbi__get_marker(z);
            while (m == STBI__MARKER_none) {
                // some files have extra padding after their blocks, so ok, we'll scan
                if (stbi__at_eof(z->s)) return stbi__err("no SOF", "Corrupt JPEG");
                m = stbi__get_marker(z);
            }
        }
        z->progressive = stbi__SOF_progressive(m);
        if (!stbi__process_frame_header(z, scan)) return 0;
        return 1;
    }

    // decode image to YCbCr format
    static int stbi__decode_jpeg_image(stbi__jpeg* j)
    {
        int m;
        for (m = 0; m < 4; m++) {
            j->img_comp[m].raw_data = NULL;
            j->img_comp[m].raw_coeff = NULL;
        }
        j->restart_interval = 0;
        if (!stbi__decode_jpeg_header(j, STBI__SCAN_load)) return 0;
        m = stbi__get_marker(j);
        while (!stbi__EOI(m)) {
            if (stbi__SOS(m)) {
                if (!stbi__process_scan_header(j)) return 0;
                if (!stbi__parse_entropy_coded_data(j)) return 0;
                if (j->marker == STBI__MARKER_none) {
                    // handle 0s at the end of image data from IP Kamera 9060
                    while (!stbi__at_eof(j->s)) {
                        int x = stbi__get8(j->s);
                        if (x == 255) {
                            j->marker = stbi__get8(j->s);
                            break;
                        }
                    }
                    // if we reach eof without hitting a marker, stbi__get_marker() below will fail and we'll eventually return 0
                }
            }
            else if (stbi__DNL(m)) {
                int Ld = stbi__get16be(j->s);
                stbi__uint32 NL = stbi__get16be(j->s);
                if (Ld != 4) return stbi__err("bad DNL len", "Corrupt JPEG");
                if (NL != j->s->img_y) return stbi__err("bad DNL height", "Corrupt JPEG");
            }
            else {
                if (!stbi__process_marker(j, m)) return 0;
            }
            m = stbi__get_marker(j);
        }
        if (j->progressive)
            stbi__jpeg_finish(j);
        return 1;
    }

    // static jfif-centered resampling (across block boundaries)

    typedef stbi_uc* (*resample_row_func)(stbi_uc* out, stbi_uc* in0, stbi_uc* in1,
        int w, int hs);

#define stbi__div4(x) ((stbi_uc) ((x) >> 2))

    static stbi_uc* resample_row_1(stbi_uc* out, stbi_uc* in_near, stbi_uc* in_far, int w, int hs)
    {
        STBI_NOTUSED(out);
        STBI_NOTUSED(in_far);
        STBI_NOTUSED(w);
        STBI_NOTUSED(hs);
        return in_near;
    }

    static stbi_uc* stbi__resample_row_v_2(stbi_uc* out, stbi_uc* in_near, stbi_uc* in_far, int w, int hs)
    {
        // need to generate two samples vertically for every one in input
        int i;
        STBI_NOTUSED(hs);
        for (i = 0; i < w; ++i)
            out[i] = stbi__div4(3 * in_near[i] + in_far[i] + 2);
        return out;
    }

    static stbi_uc* stbi__resample_row_h_2(stbi_uc* out, stbi_uc* in_near, stbi_uc* in_far, int w, int hs)
    {
        // need to generate two samples horizontally for every one in input
        int i;
        stbi_uc* input = in_near;

        if (w == 1) {
            // if only one sample, can't do any interpolation
            out[0] = out[1] = input[0];
            return out;
        }

        out[0] = input[0];
        out[1] = stbi__div4(input[0] * 3 + input[1] + 2);
        for (i = 1; i < w - 1; ++i) {
            int n = 3 * input[i] + 2;
            out[i * 2 + 0] = stbi__div4(n + input[i - 1]);
            out[i * 2 + 1] = stbi__div4(n + input[i + 1]);
        }
        out[i * 2 + 0] = stbi__div4(input[w - 2] * 3 + input[w - 1] + 2);
        out[i * 2 + 1] = input[w - 1];

        STBI_NOTUSED(in_far);
        STBI_NOTUSED(hs);

        return out;
    }

#define stbi__div16(x) ((stbi_uc) ((x) >> 4))

    static stbi_uc* stbi__resample_row_hv_2(stbi_uc* out, stbi_uc* in_near, stbi_uc* in_far, int w, int hs)
    {
        // need to generate 2x2 samples for every one in input
        int i, t0, t1;
        if (w == 1) {
            out[0] = out[1] = stbi__div4(3 * in_near[0] + in_far[0] + 2);
            return out;
        }

        t1 = 3 * in_near[0] + in_far[0];
        out[0] = stbi__div4(t1 + 2);
        for (i = 1; i < w; ++i) {
            t0 = t1;
            t1 = 3 * in_near[i] + in_far[i];
            out[i * 2 - 1] = stbi__div16(3 * t0 + t1 + 8);
            out[i * 2] = stbi__div16(3 * t1 + t0 + 8);
        }
        out[w * 2 - 1] = stbi__div4(t1 + 2);

        STBI_NOTUSED(hs);

        return out;
    }

#if defined(STBI_SSE2) || defined(STBI_NEON)
    static stbi_uc* stbi__resample_row_hv_2_simd(stbi_uc* out, stbi_uc* in_near, stbi_uc* in_far, int w, int hs)
    {
        // need to generate 2x2 samples for every one in input
        int i = 0, t0, t1;

        if (w == 1) {
            out[0] = out[1] = stbi__div4(3 * in_near[0] + in_far[0] + 2);
            return out;
        }

        t1 = 3 * in_near[0] + in_far[0];
        // process groups of 8 pixels for as long as we can.
        // note we can't handle the last pixel in a row in this loop
        // because we need to handle the filter boundary conditions.
        for (; i < ((w - 1) & ~7); i += 8) {
#if defined(STBI_SSE2)
            // load and perform the vertical filtering pass
            // this uses 3*x + y = 4*x + (y - x)
            __m128i zero = _mm_setzero_si128();
            __m128i farb = _mm_loadl_epi64((__m128i*) (in_far + i));
            __m128i nearb = _mm_loadl_epi64((__m128i*) (in_near + i));
            __m128i farw = _mm_unpacklo_epi8(farb, zero);
            __m128i nearw = _mm_unpacklo_epi8(nearb, zero);
            __m128i diff = _mm_sub_epi16(farw, nearw);
            __m128i nears = _mm_slli_epi16(nearw, 2);
            __m128i curr = _mm_add_epi16(nears, diff); // current row

            // horizontal filter works the same based on shifted vers of current
            // row. "prev" is current row shifted right by 1 pixel; we need to
            // insert the previous pixel value (from t1).
            // "next" is current row shifted left by 1 pixel, with first pixel
            // of next block of 8 pixels added in.
            __m128i prv0 = _mm_slli_si128(curr, 2);
            __m128i nxt0 = _mm_srli_si128(curr, 2);
            __m128i prev = _mm_insert_epi16(prv0, t1, 0);
            __m128i next = _mm_insert_epi16(nxt0, 3 * in_near[i + 8] + in_far[i + 8], 7);

            // horizontal filter, polyphase implementation since it's convenient:
            // even pixels = 3*cur + prev = cur*4 + (prev - cur)
            // odd  pixels = 3*cur + next = cur*4 + (next - cur)
            // note the shared term.
            __m128i bias = _mm_set1_epi16(8);
            __m128i curs = _mm_slli_epi16(curr, 2);
            __m128i prvd = _mm_sub_epi16(prev, curr);
            __m128i nxtd = _mm_sub_epi16(next, curr);
            __m128i curb = _mm_add_epi16(curs, bias);
            __m128i even = _mm_add_epi16(prvd, curb);
            __m128i odd = _mm_add_epi16(nxtd, curb);

            // interleave even and odd pixels, then undo scaling.
            __m128i int0 = _mm_unpacklo_epi16(even, odd);
            __m128i int1 = _mm_unpackhi_epi16(even, odd);
            __m128i de0 = _mm_srli_epi16(int0, 4);
            __m128i de1 = _mm_srli_epi16(int1, 4);

            // pack and write output
            __m128i outv = _mm_packus_epi16(de0, de1);
            _mm_storeu_si128((__m128i*) (out + i * 2), outv);
#elif defined(STBI_NEON)
            // load and perform the vertical filtering pass
            // this uses 3*x + y = 4*x + (y - x)
            uint8x8_t farb = vld1_u8(in_far + i);
            uint8x8_t nearb = vld1_u8(in_near + i);
            int16x8_t diff = vreinterpretq_s16_u16(vsubl_u8(farb, nearb));
            int16x8_t nears = vreinterpretq_s16_u16(vshll_n_u8(nearb, 2));
            int16x8_t curr = vaddq_s16(nears, diff); // current row

            // horizontal filter works the same based on shifted vers of current
            // row. "prev" is current row shifted right by 1 pixel; we need to
            // insert the previous pixel value (from t1).
            // "next" is current row shifted left by 1 pixel, with first pixel
            // of next block of 8 pixels added in.
            int16x8_t prv0 = vextq_s16(curr, curr, 7);
            int16x8_t nxt0 = vextq_s16(curr, curr, 1);
            int16x8_t prev = vsetq_lane_s16(t1, prv0, 0);
            int16x8_t next = vsetq_lane_s16(3 * in_near[i + 8] + in_far[i + 8], nxt0, 7);

            // horizontal filter, polyphase implementation since it's convenient:
            // even pixels = 3*cur + prev = cur*4 + (prev - cur)
            // odd  pixels = 3*cur + next = cur*4 + (next - cur)
            // note the shared term.
            int16x8_t curs = vshlq_n_s16(curr, 2);
            int16x8_t prvd = vsubq_s16(prev, curr);
            int16x8_t nxtd = vsubq_s16(next, curr);
            int16x8_t even = vaddq_s16(curs, prvd);
            int16x8_t odd = vaddq_s16(curs, nxtd);

            // undo scaling and round, then store with even/odd phases interleaved
            uint8x8x2_t o;
            o.val[0] = vqrshrun_n_s16(even, 4);
            o.val[1] = vqrshrun_n_s16(odd, 4);
            vst2_u8(out + i * 2, o);
#endif

            // "previous" value for next iter
            t1 = 3 * in_near[i + 7] + in_far[i + 7];
        }

        t0 = t1;
        t1 = 3 * in_near[i] + in_far[i];
        out[i * 2] = stbi__div16(3 * t1 + t0 + 8);

        for (++i; i < w; ++i) {
            t0 = t1;
            t1 = 3 * in_near[i] + in_far[i];
            out[i * 2 - 1] = stbi__div16(3 * t0 + t1 + 8);
            out[i * 2] = stbi__div16(3 * t1 + t0 + 8);
        }
        out[w * 2 - 1] = stbi__div4(t1 + 2);

        STBI_NOTUSED(hs);

        return out;
    }
#endif

    static stbi_uc* stbi__resample_row_generic(stbi_uc* out, stbi_uc* in_near, stbi_uc* in_far, int w, int hs)
    {
        // resample with nearest-neighbor
        int i, j;
        STBI_NOTUSED(in_far);
        for (i = 0; i < w; ++i)
            for (j = 0; j < hs; ++j)
                out[i * hs + j] = in_near[i];
        return out;
    }

    // this is a reduced-precision calculation of YCbCr-to-RGB introduced
    // to make sure the code produces the same results in both SIMD and scalar
#define stbi__float2fixed(x)  (((int) ((x) * 4096.0f + 0.5f)) << 8)
    static void stbi__YCbCr_to_RGB_row(stbi_uc* out, const stbi_uc* y, const stbi_uc* pcb, const stbi_uc* pcr, int count, int step)
    {
        int i;
        for (i = 0; i < count; ++i) {
            int y_fixed = (y[i] << 20) + (1 << 19); // rounding
            int r, g, b;
            int cr = pcr[i] - 128;
            int cb = pcb[i] - 128;
            r = y_fixed + cr * stbi__float2fixed(1.40200f);
            g = y_fixed + (cr * -stbi__float2fixed(0.71414f)) + ((cb * -stbi__float2fixed(0.34414f)) & 0xffff0000);
            b = y_fixed + cb * stbi__float2fixed(1.77200f);
            r >>= 20;
            g >>= 20;
            b >>= 20;
            if ((unsigned)r > 255) { if (r < 0) r = 0; else r = 255; }
            if ((unsigned)g > 255) { if (g < 0) g = 0; else g = 255; }
            if ((unsigned)b > 255) { if (b < 0) b = 0; else b = 255; }
            out[0] = (stbi_uc)r;
            out[1] = (stbi_uc)g;
            out[2] = (stbi_uc)b;
            out[3] = 255;
            out += step;
        }
    }

#if defined(STBI_SSE2) || defined(STBI_NEON)
    static void stbi__YCbCr_to_RGB_simd(stbi_uc* out, stbi_uc const* y, stbi_uc const* pcb, stbi_uc const* pcr, int count, int step)
    {
        int i = 0;

#ifdef STBI_SSE2
        // step == 3 is pretty ugly on the final interleave, and i'm not convinced
        // it's useful in practice (you wouldn't use it for textures, for example).
        // so just accelerate step == 4 case.
        if (step == 4) {
            // this is a fairly straightforward implementation and not super-optimized.
            __m128i signflip = _mm_set1_epi8(-0x80);
            __m128i cr_const0 = _mm_set1_epi16((short)(1.40200f * 4096.0f + 0.5f));
            __m128i cr_const1 = _mm_set1_epi16(-(short)(0.71414f * 4096.0f + 0.5f));
            __m128i cb_const0 = _mm_set1_epi16(-(short)(0.34414f * 4096.0f + 0.5f));
            __m128i cb_const1 = _mm_set1_epi16((short)(1.77200f * 4096.0f + 0.5f));
            __m128i y_bias = _mm_set1_epi8((char)(unsigned char)128);
            __m128i xw = _mm_set1_epi16(255); // alpha channel

            for (; i + 7 < count; i += 8) {
                // load
                __m128i y_bytes = _mm_loadl_epi64((__m128i*) (y + i));
                __m128i cr_bytes = _mm_loadl_epi64((__m128i*) (pcr + i));
                __m128i cb_bytes = _mm_loadl_epi64((__m128i*) (pcb + i));
                __m128i cr_biased = _mm_xor_si128(cr_bytes, signflip); // -128
                __m128i cb_biased = _mm_xor_si128(cb_bytes, signflip); // -128

                // unpack to short (and left-shift cr, cb by 8)
                __m128i yw = _mm_unpacklo_epi8(y_bias, y_bytes);
                __m128i crw = _mm_unpacklo_epi8(_mm_setzero_si128(), cr_biased);
                __m128i cbw = _mm_unpacklo_epi8(_mm_setzero_si128(), cb_biased);

                // color transform
                __m128i yws = _mm_srli_epi16(yw, 4);
                __m128i cr0 = _mm_mulhi_epi16(cr_const0, crw);
                __m128i cb0 = _mm_mulhi_epi16(cb_const0, cbw);
                __m128i cb1 = _mm_mulhi_epi16(cbw, cb_const1);
                __m128i cr1 = _mm_mulhi_epi16(crw, cr_const1);
                __m128i rws = _mm_add_epi16(cr0, yws);
                __m128i gwt = _mm_add_epi16(cb0, yws);
                __m128i bws = _mm_add_epi16(yws, cb1);
                __m128i gws = _mm_add_epi16(gwt, cr1);

                // descale
                __m128i rw = _mm_srai_epi16(rws, 4);
                __m128i bw = _mm_srai_epi16(bws, 4);
                __m128i gw = _mm_srai_epi16(gws, 4);

                // back to byte, set up for transpose
                __m128i brb = _mm_packus_epi16(rw, bw);
                __m128i gxb = _mm_packus_epi16(gw, xw);

                // transpose to interleave channels
                __m128i t0 = _mm_unpacklo_epi8(brb, gxb);
                __m128i t1 = _mm_unpackhi_epi8(brb, gxb);
                __m128i o0 = _mm_unpacklo_epi16(t0, t1);
                __m128i o1 = _mm_unpackhi_epi16(t0, t1);

                // store
                _mm_storeu_si128((__m128i*) (out + 0), o0);
                _mm_storeu_si128((__m128i*) (out + 16), o1);
                out += 32;
            }
        }
#endif

#ifdef STBI_NEON
        // in this version, step=3 support would be easy to add. but is there demand?
        if (step == 4) {
            // this is a fairly straightforward implementation and not super-optimized.
            uint8x8_t signflip = vdup_n_u8(0x80);
            int16x8_t cr_const0 = vdupq_n_s16((short)(1.40200f * 4096.0f + 0.5f));
            int16x8_t cr_const1 = vdupq_n_s16(-(short)(0.71414f * 4096.0f + 0.5f));
            int16x8_t cb_const0 = vdupq_n_s16(-(short)(0.34414f * 4096.0f + 0.5f));
            int16x8_t cb_const1 = vdupq_n_s16((short)(1.77200f * 4096.0f + 0.5f));

            for (; i + 7 < count; i += 8) {
                // load
                uint8x8_t y_bytes = vld1_u8(y + i);
                uint8x8_t cr_bytes = vld1_u8(pcr + i);
                uint8x8_t cb_bytes = vld1_u8(pcb + i);
                int8x8_t cr_biased = vreinterpret_s8_u8(vsub_u8(cr_bytes, signflip));
                int8x8_t cb_biased = vreinterpret_s8_u8(vsub_u8(cb_bytes, signflip));

                // expand to s16
                int16x8_t yws = vreinterpretq_s16_u16(vshll_n_u8(y_bytes, 4));
                int16x8_t crw = vshll_n_s8(cr_biased, 7);
                int16x8_t cbw = vshll_n_s8(cb_biased, 7);

                // color transform
                int16x8_t cr0 = vqdmulhq_s16(crw, cr_const0);
                int16x8_t cb0 = vqdmulhq_s16(cbw, cb_const0);
                int16x8_t cr1 = vqdmulhq_s16(crw, cr_const1);
                int16x8_t cb1 = vqdmulhq_s16(cbw, cb_const1);
                int16x8_t rws = vaddq_s16(yws, cr0);
                int16x8_t gws = vaddq_s16(vaddq_s16(yws, cb0), cr1);
                int16x8_t bws = vaddq_s16(yws, cb1);

                // undo scaling, round, convert to byte
                uint8x8x4_t o;
                o.val[0] = vqrshrun_n_s16(rws, 4);
                o.val[1] = vqrshrun_n_s16(gws, 4);
                o.val[2] = vqrshrun_n_s16(bws, 4);
                o.val[3] = vdup_n_u8(255);

                // store, interleaving r/g/b/a
                vst4_u8(out, o);
                out += 8 * 4;
            }
        }
#endif

        for (; i < count; ++i) {
            int y_fixed = (y[i] << 20) + (1 << 19); // rounding
            int r, g, b;
            int cr = pcr[i] - 128;
            int cb = pcb[i] - 128;
            r = y_fixed + cr * stbi__float2fixed(1.40200f);
            g = y_fixed + cr * -stbi__float2fixed(0.71414f) + ((cb * -stbi__float2fixed(0.34414f)) & 0xffff0000);
            b = y_fixed + cb * stbi__float2fixed(1.77200f);
            r >>= 20;
            g >>= 20;
            b >>= 20;
            if ((unsigned)r > 255) { if (r < 0) r = 0; else r = 255; }
            if ((unsigned)g > 255) { if (g < 0) g = 0; else g = 255; }
            if ((unsigned)b > 255) { if (b < 0) b = 0; else b = 255; }
            out[0] = (stbi_uc)r;
            out[1] = (stbi_uc)g;
            out[2] = (stbi_uc)b;
            out[3] = 255;
            out += step;
        }
    }
#endif

    // set up the kernels
    static void stbi__setup_jpeg(stbi__jpeg* j)
    {
        j->idct_block_kernel = stbi__idct_block;
        j->YCbCr_to_RGB_kernel = stbi__YCbCr_to_RGB_row;
        j->resample_row_hv_2_kernel = stbi__resample_row_hv_2;

#ifdef STBI_SSE2
        if (stbi__sse2_available()) {
            j->idct_block_kernel = stbi__idct_simd;
            j->YCbCr_to_RGB_kernel = stbi__YCbCr_to_RGB_simd;
            j->resample_row_hv_2_kernel = stbi__resample_row_hv_2_simd;
        }
#endif

#ifdef STBI_NEON
        j->idct_block_kernel = stbi__idct_simd;
        j->YCbCr_to_RGB_kernel = stbi__YCbCr_to_RGB_simd;
        j->resample_row_hv_2_kernel = stbi__resample_row_hv_2_simd;
#endif
    }

    // clean up the temporary component buffers
    static void stbi__cleanup_jpeg(stbi__jpeg* j)
    {
        stbi__free_jpeg_components(j, j->s->img_n, 0);
    }

    typedef struct
    {
        resample_row_func resample;
        stbi_uc* line0, * line1;
        int hs, vs;   // expansion factor in each axis
        int w_lores; // horizontal pixels pre-expansion
        int ystep;   // how far through vertical expansion we are
        int ypos;    // which pre-expansion row we're on
    } stbi__resample;

    // fast 0..255 * 0..255 => 0..255 rounded multiplication
    static stbi_uc stbi__blinn_8x8(stbi_uc x, stbi_uc y)
    {
        unsigned int t = x * y + 128;
        return (stbi_uc)((t + (t >> 8)) >> 8);
    }

    static stbi_uc* load_jpeg_image(stbi__jpeg* z, int* out_x, int* out_y, int* comp, int req_comp)
    {
        int n, decode_n, is_rgb;
        z->s->img_n = 0; // make stbi__cleanup_jpeg safe

        // validate req_comp
        if (req_comp < 0 || req_comp > 4) return stbi__errpuc("bad req_comp", "Internal error");

        // load a jpeg image from whichever source, but leave in YCbCr format
        if (!stbi__decode_jpeg_image(z)) { stbi__cleanup_jpeg(z); return NULL; }

        // determine actual number of components to generate
        n = req_comp ? req_comp : z->s->img_n >= 3 ? 3 : 1;

        is_rgb = z->s->img_n == 3 && (z->rgb == 3 || (z->app14_color_transform == 0 && !z->jfif));

        if (z->s->img_n == 3 && n < 3 && !is_rgb)
            decode_n = 1;
        else
            decode_n = z->s->img_n;

        // nothing to do if no components requested; check this now to avoid
        // accessing uninitialized coutput[0] later
        if (decode_n <= 0) { stbi__cleanup_jpeg(z); return NULL; }

        // resample and color-convert
        {
            int k;
            unsigned int i, j;
            stbi_uc* output;
            stbi_uc* coutput[4] = { NULL, NULL, NULL, NULL };

            stbi__resample res_comp[4];

            for (k = 0; k < decode_n; ++k) {
                stbi__resample* r = &res_comp[k];

                // allocate line buffer big enough for upsampling off the edges
                // with upsample factor of 4
                z->img_comp[k].linebuf = (stbi_uc*)stbi__malloc(z->s->img_x + 3);
                if (!z->img_comp[k].linebuf) { stbi__cleanup_jpeg(z); return stbi__errpuc("outofmem", "Out of memory"); }

                r->hs = z->img_h_max / z->img_comp[k].h;
                r->vs = z->img_v_max / z->img_comp[k].v;
                r->ystep = r->vs >> 1;
                r->w_lores = (z->s->img_x + r->hs - 1) / r->hs;
                r->ypos = 0;
                r->line0 = r->line1 = z->img_comp[k].data;

                if (r->hs == 1 && r->vs == 1) r->resample = resample_row_1;
                else if (r->hs == 1 && r->vs == 2) r->resample = stbi__resample_row_v_2;
                else if (r->hs == 2 && r->vs == 1) r->resample = stbi__resample_row_h_2;
                else if (r->hs == 2 && r->vs == 2) r->resample = z->resample_row_hv_2_kernel;
                else                               r->resample = stbi__resample_row_generic;
            }

            // can't error after this so, this is safe
            output = (stbi_uc*)stbi__malloc_mad3(n, z->s->img_x, z->s->img_y, 1);
            if (!output) { stbi__cleanup_jpeg(z); return stbi__errpuc("outofmem", "Out of memory"); }

            // now go ahead and resample
            for (j = 0; j < z->s->img_y; ++j) {
                stbi_uc* out = output + n * z->s->img_x * j;
                for (k = 0; k < decode_n; ++k) {
                    stbi__resample* r = &res_comp[k];
                    int y_bot = r->ystep >= (r->vs >> 1);
                    coutput[k] = r->resample(z->img_comp[k].linebuf,
                        y_bot ? r->line1 : r->line0,
                        y_bot ? r->line0 : r->line1,
                        r->w_lores, r->hs);
                    if (++r->ystep >= r->vs) {
                        r->ystep = 0;
                        r->line0 = r->line1;
                        if (++r->ypos < z->img_comp[k].y)
                            r->line1 += z->img_comp[k].w2;
                    }
                }
                if (n >= 3) {
                    stbi_uc* y = coutput[0];
                    if (z->s->img_n == 3) {
                        if (is_rgb) {
                            for (i = 0; i < z->s->img_x; ++i) {
                                out[0] = y[i];
                                out[1] = coutput[1][i];
                                out[2] = coutput[2][i];
                                out[3] = 255;
                                out += n;
                            }
                        }
                        else {
                            z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
                        }
                    }
                    else if (z->s->img_n == 4) {
                        if (z->app14_color_transform == 0) { // CMYK
                            for (i = 0; i < z->s->img_x; ++i) {
                                stbi_uc m = coutput[3][i];
                                out[0] = stbi__blinn_8x8(coutput[0][i], m);
                                out[1] = stbi__blinn_8x8(coutput[1][i], m);
                                out[2] = stbi__blinn_8x8(coutput[2][i], m);
                                out[3] = 255;
                                out += n;
                            }
                        }
                        else if (z->app14_color_transform == 2) { // YCCK
                            z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
                            for (i = 0; i < z->s->img_x; ++i) {
                                stbi_uc m = coutput[3][i];
                                out[0] = stbi__blinn_8x8(255 - out[0], m);
                                out[1] = stbi__blinn_8x8(255 - out[1], m);
                                out[2] = stbi__blinn_8x8(255 - out[2], m);
                                out += n;
                            }
                        }
                        else { // YCbCr + alpha?  Ignore the fourth channel for now
                            z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
                        }
                    }
                    else
                        for (i = 0; i < z->s->img_x; ++i) {
                            out[0] = out[1] = out[2] = y[i];
                            out[3] = 255; // not used if n==3
                            out += n;
                        }
                }
                else {
                    if (is_rgb) {
                        if (n == 1)
                            for (i = 0; i < z->s->img_x; ++i)
                                *out++ = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
                        else {
                            for (i = 0; i < z->s->img_x; ++i, out += 2) {
                                out[0] = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
                                out[1] = 255;
                            }
                        }
                    }
                    else if (z->s->img_n == 4 && z->app14_color_transform == 0) {
                        for (i = 0; i < z->s->img_x; ++i) {
                            stbi_uc m = coutput[3][i];
                            stbi_uc r = stbi__blinn_8x8(coutput[0][i], m);
                            stbi_uc g = stbi__blinn_8x8(coutput[1][i], m);
                            stbi_uc b = stbi__blinn_8x8(coutput[2][i], m);
                            out[0] = stbi__compute_y(r, g, b);
                            out[1] = 255;
                            out += n;
                        }
                    }
                    else if (z->s->img_n == 4 && z->app14_color_transform == 2) {
                        for (i = 0; i < z->s->img_x; ++i) {
                            out[0] = stbi__blinn_8x8(255 - coutput[0][i], coutput[3][i]);
                            out[1] = 255;
                            out += n;
                        }
                    }
                    else {
                        stbi_uc* y = coutput[0];
                        if (n == 1)
                            for (i = 0; i < z->s->img_x; ++i) out[i] = y[i];
                        else
                            for (i = 0; i < z->s->img_x; ++i) { *out++ = y[i]; *out++ = 255; }
                    }
                }
            }
            stbi__cleanup_jpeg(z);
            *out_x = z->s->img_x;
            *out_y = z->s->img_y;
            if (comp) *comp = z->s->img_n >= 3 ? 3 : 1; // report original components, not output
            return output;
        }
    }

    static void* stbi__jpeg_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri)
    {
        unsigned char* result;
        stbi__jpeg* j = (stbi__jpeg*)stbi__malloc(sizeof(stbi__jpeg));
        if (!j) return stbi__errpuc("outofmem", "Out of memory");
        STBI_NOTUSED(ri);
        j->s = s;
        stbi__setup_jpeg(j);
        result = load_jpeg_image(j, x, y, comp, req_comp);
        STBI_FREE(j);
        return result;
    }

    static int stbi__jpeg_test(stbi__context* s)
    {
        int r;
        stbi__jpeg* j = (stbi__jpeg*)stbi__malloc(sizeof(stbi__jpeg));
        if (!j) return stbi__err("outofmem", "Out of memory");
        j->s = s;
        stbi__setup_jpeg(j);
        r = stbi__decode_jpeg_header(j, STBI__SCAN_type);
        stbi__rewind(s);
        STBI_FREE(j);
        return r;
    }

    static int stbi__jpeg_info_raw(stbi__jpeg* j, int* x, int* y, int* comp)
    {
        if (!stbi__decode_jpeg_header(j, STBI__SCAN_header)) {
            stbi__rewind(j->s);
            return 0;
        }
        if (x) *x = j->s->img_x;
        if (y) *y = j->s->img_y;
        if (comp) *comp = j->s->img_n >= 3 ? 3 : 1;
        return 1;
    }

    static int stbi__jpeg_info(stbi__context* s, int* x, int* y, int* comp)
    {
        int result;
        stbi__jpeg* j = (stbi__jpeg*)(stbi__malloc(sizeof(stbi__jpeg)));
        if (!j) return stbi__err("outofmem", "Out of memory");
        j->s = s;
        result = stbi__jpeg_info_raw(j, x, y, comp);
        STBI_FREE(j);
        return result;
    }
#endif

    // public domain zlib decode    v0.2  Sean Barrett 2006-11-18
    //    simple implementation
    //      - all input must be provided in an upfront buffer
    //      - all output is written to a single output buffer (can malloc/realloc)
    //    performance
    //      - fast huffman

#ifndef STBI_NO_ZLIB

// fast-way is faster to check than jpeg huffman, but slow way is slower
#define STBI__ZFAST_BITS  9 // accelerate all cases in default tables
#define STBI__ZFAST_MASK  ((1 << STBI__ZFAST_BITS) - 1)
#define STBI__ZNSYMS 288 // number of symbols in literal/length alphabet

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
    typedef struct
    {
        stbi__uint16 fast[1 << STBI__ZFAST_BITS];
        stbi__uint16 firstcode[16];
        int maxcode[17];
        stbi__uint16 firstsymbol[16];
        stbi_uc  size[STBI__ZNSYMS];
        stbi__uint16 value[STBI__ZNSYMS];
    } stbi__zhuffman;

    stbi_inline static int stbi__bitreverse16(int n)
    {
        n = ((n & 0xAAAA) >> 1) | ((n & 0x5555) << 1);
        n = ((n & 0xCCCC) >> 2) | ((n & 0x3333) << 2);
        n = ((n & 0xF0F0) >> 4) | ((n & 0x0F0F) << 4);
        n = ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
        return n;
    }

    stbi_inline static int stbi__bit_reverse(int v, int bits)
    {
        STBI_ASSERT(bits <= 16);
        // to bit reverse n bits, reverse 16 and shift
        // e.g. 11 bits, bit reverse and shift away 5
        return stbi__bitreverse16(v) >> (16 - bits);
    }

    static int stbi__zbuild_huffman(stbi__zhuffman* z, const stbi_uc* sizelist, int num)
    {
        int i, k = 0;
        int code, next_code[16], sizes[17];

        // DEFLATE spec for generating codes
        memset(sizes, 0, sizeof(sizes));
        memset(z->fast, 0, sizeof(z->fast));
        for (i = 0; i < num; ++i)
            ++sizes[sizelist[i]];
        sizes[0] = 0;
        for (i = 1; i < 16; ++i)
            if (sizes[i] > (1 << i))
                return stbi__err("bad sizes", "Corrupt PNG");
        code = 0;
        for (i = 1; i < 16; ++i) {
            next_code[i] = code;
            z->firstcode[i] = (stbi__uint16)code;
            z->firstsymbol[i] = (stbi__uint16)k;
            code = (code + sizes[i]);
            if (sizes[i])
                if (code - 1 >= (1 << i)) return stbi__err("bad codelengths", "Corrupt PNG");
            z->maxcode[i] = code << (16 - i); // preshift for inner loop
            code <<= 1;
            k += sizes[i];
        }
        z->maxcode[16] = 0x10000; // sentinel
        for (i = 0; i < num; ++i) {
            int s = sizelist[i];
            if (s) {
                int c = next_code[s] - z->firstcode[s] + z->firstsymbol[s];
                stbi__uint16 fastv = (stbi__uint16)((s << 9) | i);
                z->size[c] = (stbi_uc)s;
                z->value[c] = (stbi__uint16)i;
                if (s <= STBI__ZFAST_BITS) {
                    int j = stbi__bit_reverse(next_code[s], s);
                    while (j < (1 << STBI__ZFAST_BITS)) {
                        z->fast[j] = fastv;
                        j += (1 << s);
                    }
                }
                ++next_code[s];
            }
        }
        return 1;
    }

    // zlib-from-memory implementation for PNG reading
    //    because PNG allows splitting the zlib stream arbitrarily,
    //    and it's annoying structurally to have PNG call ZLIB call PNG,
    //    we require PNG read all the IDATs and combine them into a single
    //    memory buffer

    typedef struct
    {
        stbi_uc* zbuffer, * zbuffer_end;
        int num_bits;
        stbi__uint32 code_buffer;

        char* zout;
        char* zout_start;
        char* zout_end;
        int   z_expandable;

        stbi__zhuffman z_length, z_distance;
    } stbi__zbuf;

    stbi_inline static int stbi__zeof(stbi__zbuf* z)
    {
        return (z->zbuffer >= z->zbuffer_end);
    }

    stbi_inline static stbi_uc stbi__zget8(stbi__zbuf* z)
    {
        return stbi__zeof(z) ? 0 : *z->zbuffer++;
    }

    static void stbi__fill_bits(stbi__zbuf* z)
    {
        do {
            if (z->code_buffer >= (1U << z->num_bits)) {
                z->zbuffer = z->zbuffer_end;  /* treat this as EOF so we fail. */
                return;
            }
            z->code_buffer |= (unsigned int)stbi__zget8(z) << z->num_bits;
            z->num_bits += 8;
        } while (z->num_bits <= 24);
    }

    stbi_inline static unsigned int stbi__zreceive(stbi__zbuf* z, int n)
    {
        unsigned int k;
        if (z->num_bits < n) stbi__fill_bits(z);
        k = z->code_buffer & ((1 << n) - 1);
        z->code_buffer >>= n;
        z->num_bits -= n;
        return k;
    }

    static int stbi__zhuffman_decode_slowpath(stbi__zbuf* a, stbi__zhuffman* z)
    {
        int b, s, k;
        // not resolved by fast table, so compute it the slow way
        // use jpeg approach, which requires MSbits at top
        k = stbi__bit_reverse(a->code_buffer, 16);
        for (s = STBI__ZFAST_BITS + 1; ; ++s)
            if (k < z->maxcode[s])
                break;
        if (s >= 16) return -1; // invalid code!
        // code size is s, so:
        b = (k >> (16 - s)) - z->firstcode[s] + z->firstsymbol[s];
        if (b >= STBI__ZNSYMS) return -1; // some data was corrupt somewhere!
        if (z->size[b] != s) return -1;  // was originally an assert, but report failure instead.
        a->code_buffer >>= s;
        a->num_bits -= s;
        return z->value[b];
    }

    stbi_inline static int stbi__zhuffman_decode(stbi__zbuf* a, stbi__zhuffman* z)
    {
        int b, s;
        if (a->num_bits < 16) {
            if (stbi__zeof(a)) {
                return -1;   /* report error for unexpected end of data. */
            }
            stbi__fill_bits(a);
        }
        b = z->fast[a->code_buffer & STBI__ZFAST_MASK];
        if (b) {
            s = b >> 9;
            a->code_buffer >>= s;
            a->num_bits -= s;
            return b & 511;
        }
        return stbi__zhuffman_decode_slowpath(a, z);
    }

    static int stbi__zexpand(stbi__zbuf* z, char* zout, int n)  // need to make room for n bytes
    {
        char* q;
        unsigned int cur, limit, old_limit;
        z->zout = zout;
        if (!z->z_expandable) return stbi__err("output buffer limit", "Corrupt PNG");
        cur = (unsigned int)(z->zout - z->zout_start);
        limit = old_limit = (unsigned)(z->zout_end - z->zout_start);
        if (UINT_MAX - cur < (unsigned)n) return stbi__err("outofmem", "Out of memory");
        while (cur + n > limit) {
            if (limit > UINT_MAX / 2) return stbi__err("outofmem", "Out of memory");
            limit *= 2;
        }
        q = (char*)STBI_REALLOC_SIZED(z->zout_start, old_limit, limit);
        STBI_NOTUSED(old_limit);
        if (q == NULL) return stbi__err("outofmem", "Out of memory");
        z->zout_start = q;
        z->zout = q + cur;
        z->zout_end = q + limit;
        return 1;
    }

    static const int stbi__zlength_base[31] = {
       3,4,5,6,7,8,9,10,11,13,
       15,17,19,23,27,31,35,43,51,59,
       67,83,99,115,131,163,195,227,258,0,0 };

    static const int stbi__zlength_extra[31] =
    { 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };

    static const int stbi__zdist_base[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
    257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0 };

    static const int stbi__zdist_extra[32] =
    { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };

    static int stbi__parse_huffman_block(stbi__zbuf* a)
    {
        char* zout = a->zout;
        for (;;) {
            int z = stbi__zhuffman_decode(a, &a->z_length);
            if (z < 256) {
                if (z < 0) return stbi__err("bad huffman code", "Corrupt PNG"); // error in huffman codes
                if (zout >= a->zout_end) {
                    if (!stbi__zexpand(a, zout, 1)) return 0;
                    zout = a->zout;
                }
                *zout++ = (char)z;
            }
            else {
                stbi_uc* p;
                int len, dist;
                if (z == 256) {
                    a->zout = zout;
                    return 1;
                }
                z -= 257;
                len = stbi__zlength_base[z];
                if (stbi__zlength_extra[z]) len += stbi__zreceive(a, stbi__zlength_extra[z]);
                z = stbi__zhuffman_decode(a, &a->z_distance);
                if (z < 0) return stbi__err("bad huffman code", "Corrupt PNG");
                dist = stbi__zdist_base[z];
                if (stbi__zdist_extra[z]) dist += stbi__zreceive(a, stbi__zdist_extra[z]);
                if (zout - a->zout_start < dist) return stbi__err("bad dist", "Corrupt PNG");
                if (zout + len > a->zout_end) {
                    if (!stbi__zexpand(a, zout, len)) return 0;
                    zout = a->zout;
                }
                p = (stbi_uc*)(zout - dist);
                if (dist == 1) { // run of one byte; common in images.
                    stbi_uc v = *p;
                    if (len) { do *zout++ = v; while (--len); }
                }
                else {
                    if (len) { do *zout++ = *p++; while (--len); }
                }
            }
        }
    }

    static int stbi__compute_huffman_codes(stbi__zbuf* a)
    {
        static const stbi_uc length_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
        stbi__zhuffman z_codelength;
        stbi_uc lencodes[286 + 32 + 137];//padding for maximum single op
        stbi_uc codelength_sizes[19];
        int i, n;

        int hlit = stbi__zreceive(a, 5) + 257;
        int hdist = stbi__zreceive(a, 5) + 1;
        int hclen = stbi__zreceive(a, 4) + 4;
        int ntot = hlit + hdist;

        memset(codelength_sizes, 0, sizeof(codelength_sizes));
        for (i = 0; i < hclen; ++i) {
            int s = stbi__zreceive(a, 3);
            codelength_sizes[length_dezigzag[i]] = (stbi_uc)s;
        }
        if (!stbi__zbuild_huffman(&z_codelength, codelength_sizes, 19)) return 0;

        n = 0;
        while (n < ntot) {
            int c = stbi__zhuffman_decode(a, &z_codelength);
            if (c < 0 || c >= 19) return stbi__err("bad codelengths", "Corrupt PNG");
            if (c < 16)
                lencodes[n++] = (stbi_uc)c;
            else {
                stbi_uc fill = 0;
                if (c == 16) {
                    c = stbi__zreceive(a, 2) + 3;
                    if (n == 0) return stbi__err("bad codelengths", "Corrupt PNG");
                    fill = lencodes[n - 1];
                }
                else if (c == 17) {
                    c = stbi__zreceive(a, 3) + 3;
                }
                else if (c == 18) {
                    c = stbi__zreceive(a, 7) + 11;
                }
                else {
                    return stbi__err("bad codelengths", "Corrupt PNG");
                }
                if (ntot - n < c) return stbi__err("bad codelengths", "Corrupt PNG");
                memset(lencodes + n, fill, c);
                n += c;
            }
        }
        if (n != ntot) return stbi__err("bad codelengths", "Corrupt PNG");
        if (!stbi__zbuild_huffman(&a->z_length, lencodes, hlit)) return 0;
        if (!stbi__zbuild_huffman(&a->z_distance, lencodes + hlit, hdist)) return 0;
        return 1;
    }

    static int stbi__parse_uncompressed_block(stbi__zbuf* a)
    {
        stbi_uc header[4];
        int len, nlen, k;
        if (a->num_bits & 7)
            stbi__zreceive(a, a->num_bits & 7); // discard
        // drain the bit-packed data into header
        k = 0;
        while (a->num_bits > 0) {
            header[k++] = (stbi_uc)(a->code_buffer & 255); // suppress MSVC run-time check
            a->code_buffer >>= 8;
            a->num_bits -= 8;
        }
        if (a->num_bits < 0) return stbi__err("zlib corrupt", "Corrupt PNG");
        // now fill header the normal way
        while (k < 4)
            header[k++] = stbi__zget8(a);
        len = header[1] * 256 + header[0];
        nlen = header[3] * 256 + header[2];
        if (nlen != (len ^ 0xffff)) return stbi__err("zlib corrupt", "Corrupt PNG");
        if (a->zbuffer + len > a->zbuffer_end) return stbi__err("read past buffer", "Corrupt PNG");
        if (a->zout + len > a->zout_end)
            if (!stbi__zexpand(a, a->zout, len)) return 0;
        memcpy(a->zout, a->zbuffer, len);
        a->zbuffer += len;
        a->zout += len;
        return 1;
    }

    static int stbi__parse_zlib_header(stbi__zbuf* a)
    {
        int cmf = stbi__zget8(a);
        int cm = cmf & 15;
        /* int cinfo = cmf >> 4; */
        int flg = stbi__zget8(a);
        if (stbi__zeof(a)) return stbi__err("bad zlib header", "Corrupt PNG"); // zlib spec
        if ((cmf * 256 + flg) % 31 != 0) return stbi__err("bad zlib header", "Corrupt PNG"); // zlib spec
        if (flg & 32) return stbi__err("no preset dict", "Corrupt PNG"); // preset dictionary not allowed in png
        if (cm != 8) return stbi__err("bad compression", "Corrupt PNG"); // DEFLATE required for png
        // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
        return 1;
    }

    static const stbi_uc stbi__zdefault_length[STBI__ZNSYMS] =
    {
       8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
       8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
       8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
       8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
       8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
       9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
       9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
       9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
       7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8
    };
    static const stbi_uc stbi__zdefault_distance[32] =
    {
       5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
    };
    /*
    Init algorithm:
    {
       int i;   // use <= to match clearly with spec
       for (i=0; i <= 143; ++i)     stbi__zdefault_length[i]   = 8;
       for (   ; i <= 255; ++i)     stbi__zdefault_length[i]   = 9;
       for (   ; i <= 279; ++i)     stbi__zdefault_length[i]   = 7;
       for (   ; i <= 287; ++i)     stbi__zdefault_length[i]   = 8;

       for (i=0; i <=  31; ++i)     stbi__zdefault_distance[i] = 5;
    }
    */

    static int stbi__parse_zlib(stbi__zbuf* a, int parse_header)
    {
        int final, type;
        if (parse_header)
            if (!stbi__parse_zlib_header(a)) return 0;
        a->num_bits = 0;
        a->code_buffer = 0;
        do {
            final = stbi__zreceive(a, 1);
            type = stbi__zreceive(a, 2);
            if (type == 0) {
                if (!stbi__parse_uncompressed_block(a)) return 0;
            }
            else if (type == 3) {
                return 0;
            }
            else {
                if (type == 1) {
                    // use fixed code lengths
                    if (!stbi__zbuild_huffman(&a->z_length, stbi__zdefault_length, STBI__ZNSYMS)) return 0;
                    if (!stbi__zbuild_huffman(&a->z_distance, stbi__zdefault_distance, 32)) return 0;
                }
                else {
                    if (!stbi__compute_huffman_codes(a)) return 0;
                }
                if (!stbi__parse_huffman_block(a)) return 0;
            }
        } while (!final);
        return 1;
    }

    static int stbi__do_zlib(stbi__zbuf* a, char* obuf, int olen, int exp, int parse_header)
    {
        a->zout_start = obuf;
        a->zout = obuf;
        a->zout_end = obuf + olen;
        a->z_expandable = exp;

        return stbi__parse_zlib(a, parse_header);
    }

    STBIDEF char* stbi_zlib_decode_malloc_guesssize(const char* buffer, int len, int initial_size, int* outlen)
    {
        stbi__zbuf a;
        char* p = (char*)stbi__malloc(initial_size);
        if (p == NULL) return NULL;
        a.zbuffer = (stbi_uc*)buffer;
        a.zbuffer_end = (stbi_uc*)buffer + len;
        if (stbi__do_zlib(&a, p, initial_size, 1, 1)) {
            if (outlen) *outlen = (int)(a.zout - a.zout_start);
            return a.zout_start;
        }
        else {
            STBI_FREE(a.zout_start);
            return NULL;
        }
    }

    STBIDEF char* stbi_zlib_decode_malloc(char const* buffer, int len, int* outlen)
    {
        return stbi_zlib_decode_malloc_guesssize(buffer, len, 16384, outlen);
    }

    STBIDEF char* stbi_zlib_decode_malloc_guesssize_headerflag(const char* buffer, int len, int initial_size, int* outlen, int parse_header)
    {
        stbi__zbuf a;
        char* p = (char*)stbi__malloc(initial_size);
        if (p == NULL) return NULL;
        a.zbuffer = (stbi_uc*)buffer;
        a.zbuffer_end = (stbi_uc*)buffer + len;
        if (stbi__do_zlib(&a, p, initial_size, 1, parse_header)) {
            if (outlen) *outlen = (int)(a.zout - a.zout_start);
            return a.zout_start;
        }
        else {
            STBI_FREE(a.zout_start);
            return NULL;
        }
    }

    STBIDEF int stbi_zlib_decode_buffer(char* obuffer, int olen, char const* ibuffer, int ilen)
    {
        stbi__zbuf a;
        a.zbuffer = (stbi_uc*)ibuffer;
        a.zbuffer_end = (stbi_uc*)ibuffer + ilen;
        if (stbi__do_zlib(&a, obuffer, olen, 0, 1))
            return (int)(a.zout - a.zout_start);
        else
            return -1;
    }

    STBIDEF char* stbi_zlib_decode_noheader_malloc(char const* buffer, int len, int* outlen)
    {
        stbi__zbuf a;
        char* p = (char*)stbi__malloc(16384);
        if (p == NULL) return NULL;
        a.zbuffer = (stbi_uc*)buffer;
        a.zbuffer_end = (stbi_uc*)buffer + len;
        if (stbi__do_zlib(&a, p, 16384, 1, 0)) {
            if (outlen) *outlen = (int)(a.zout - a.zout_start);
            return a.zout_start;
        }
        else {
            STBI_FREE(a.zout_start);
            return NULL;
        }
    }

    STBIDEF int stbi_zlib_decode_noheader_buffer(char* obuffer, int olen, const char* ibuffer, int ilen)
    {
        stbi__zbuf a;
        a.zbuffer = (stbi_uc*)ibuffer;
        a.zbuffer_end = (stbi_uc*)ibuffer + ilen;
        if (stbi__do_zlib(&a, obuffer, olen, 0, 0))
            return (int)(a.zout - a.zout_start);
        else
            return -1;
    }
#endif

    // public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
    //    simple implementation
    //      - only 8-bit samples
    //      - no CRC checking
    //      - allocates lots of intermediate memory
    //        - avoids problem of streaming data between subsystems
    //        - avoids explicit window management
    //    performance
    //      - uses stb_zlib, a PD zlib implementation with fast huffman decoding

#ifndef STBI_NO_PNG
    typedef struct
    {
        stbi__uint32 length;
        stbi__uint32 type;
    } stbi__pngchunk;

    static stbi__pngchunk stbi__get_chunk_header(stbi__context* s)
    {
        stbi__pngchunk c;
        c.length = stbi__get32be(s);
        c.type = stbi__get32be(s);
        return c;
    }

    static int stbi__check_png_header(stbi__context* s)
    {
        static const stbi_uc png_sig[8] = { 137,80,78,71,13,10,26,10 };
        int i;
        for (i = 0; i < 8; ++i)
            if (stbi__get8(s) != png_sig[i]) return stbi__err("bad png sig", "Not a PNG");
        return 1;
    }

    typedef struct
    {
        stbi__context* s;
        stbi_uc* idata, * expanded, * out;
        int depth;
    } stbi__png;


    enum {
        STBI__F_none = 0,
        STBI__F_sub = 1,
        STBI__F_up = 2,
        STBI__F_avg = 3,
        STBI__F_paeth = 4,
        // synthetic filters used for first scanline to avoid needing a dummy row of 0s
        STBI__F_avg_first,
        STBI__F_paeth_first
    };

    static stbi_uc first_row_filter[5] =
    {
       STBI__F_none,
       STBI__F_sub,
       STBI__F_none,
       STBI__F_avg_first,
       STBI__F_paeth_first
    };

    static int stbi__paeth(int a, int b, int c)
    {
        int p = a + b - c;
        int pa = abs(p - a);
        int pb = abs(p - b);
        int pc = abs(p - c);
        if (pa <= pb && pa <= pc) return a;
        if (pb <= pc) return b;
        return c;
    }

    static const stbi_uc stbi__depth_scale_table[9] = { 0, 0xff, 0x55, 0, 0x11, 0,0,0, 0x01 };

    // create the png data from post-deflated data
    static int stbi__create_png_image_raw(stbi__png* a, stbi_uc* raw, stbi__uint32 raw_len, int out_n, stbi__uint32 x, stbi__uint32 y, int depth, int color)
    {
        int bytes = (depth == 16 ? 2 : 1);
        stbi__context* s = a->s;
        stbi__uint32 i, j, stride = x * out_n * bytes;
        stbi__uint32 img_len, img_width_bytes;
        int k;
        int img_n = s->img_n; // copy it into a local for later

        int output_bytes = out_n * bytes;
        int filter_bytes = img_n * bytes;
        int width = x;

        STBI_ASSERT(out_n == s->img_n || out_n == s->img_n + 1);
        a->out = (stbi_uc*)stbi__malloc_mad3(x, y, output_bytes, 0); // extra bytes to write off the end into
        if (!a->out) return stbi__err("outofmem", "Out of memory");

        if (!stbi__mad3sizes_valid(img_n, x, depth, 7)) return stbi__err("too large", "Corrupt PNG");
        img_width_bytes = (((img_n * x * depth) + 7) >> 3);
        img_len = (img_width_bytes + 1) * y;

        // we used to check for exact match between raw_len and img_len on non-interlaced PNGs,
        // but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),
        // so just check for raw_len < img_len always.
        if (raw_len < img_len) return stbi__err("not enough pixels", "Corrupt PNG");

        for (j = 0; j < y; ++j) {
            stbi_uc* cur = a->out + stride * j;
            stbi_uc* prior;
            int filter = *raw++;

            if (filter > 4)
                return stbi__err("invalid filter", "Corrupt PNG");

            if (depth < 8) {
                if (img_width_bytes > x) return stbi__err("invalid width", "Corrupt PNG");
                cur += x * out_n - img_width_bytes; // store output to the rightmost img_len bytes, so we can decode in place
                filter_bytes = 1;
                width = img_width_bytes;
            }
            prior = cur - stride; // bugfix: need to compute this after 'cur +=' computation above

            // if first row, use special filter that doesn't sample previous row
            if (j == 0) filter = first_row_filter[filter];

            // handle first byte explicitly
            for (k = 0; k < filter_bytes; ++k) {
                switch (filter) {
                case STBI__F_none: cur[k] = raw[k]; break;
                case STBI__F_sub: cur[k] = raw[k]; break;
                case STBI__F_up: cur[k] = STBI__BYTECAST(raw[k] + prior[k]); break;
                case STBI__F_avg: cur[k] = STBI__BYTECAST(raw[k] + (prior[k] >> 1)); break;
                case STBI__F_paeth: cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(0, prior[k], 0)); break;
                case STBI__F_avg_first: cur[k] = raw[k]; break;
                case STBI__F_paeth_first: cur[k] = raw[k]; break;
                }
            }

            if (depth == 8) {
                if (img_n != out_n)
                    cur[img_n] = 255; // first pixel
                raw += img_n;
                cur += out_n;
                prior += out_n;
            }
            else if (depth == 16) {
                if (img_n != out_n) {
                    cur[filter_bytes] = 255; // first pixel top byte
                    cur[filter_bytes + 1] = 255; // first pixel bottom byte
                }
                raw += filter_bytes;
                cur += output_bytes;
                prior += output_bytes;
            }
            else {
                raw += 1;
                cur += 1;
                prior += 1;
            }

            // this is a little gross, so that we don't switch per-pixel or per-component
            if (depth < 8 || img_n == out_n) {
                int nk = (width - 1) * filter_bytes;
#define STBI__CASE(f) \
             case f:     \
                for (k=0; k < nk; ++k)
                switch (filter) {
                    // "none" filter turns into a memcpy here; make that explicit.
                case STBI__F_none:         memcpy(cur, raw, nk); break;
                    STBI__CASE(STBI__F_sub) { cur[k] = STBI__BYTECAST(raw[k] + cur[k - filter_bytes]); } break;
                    STBI__CASE(STBI__F_up) { cur[k] = STBI__BYTECAST(raw[k] + prior[k]); } break;
                    STBI__CASE(STBI__F_avg) { cur[k] = STBI__BYTECAST(raw[k] + ((prior[k] + cur[k - filter_bytes]) >> 1)); } break;
                    STBI__CASE(STBI__F_paeth) { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - filter_bytes], prior[k], prior[k - filter_bytes])); } break;
                    STBI__CASE(STBI__F_avg_first) { cur[k] = STBI__BYTECAST(raw[k] + (cur[k - filter_bytes] >> 1)); } break;
                    STBI__CASE(STBI__F_paeth_first) { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - filter_bytes], 0, 0)); } break;
                }
#undef STBI__CASE
                raw += nk;
            }
            else {
                STBI_ASSERT(img_n + 1 == out_n);
#define STBI__CASE(f) \
             case f:     \
                for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) \
                   for (k=0; k < filter_bytes; ++k)
                switch (filter) {
                    STBI__CASE(STBI__F_none) { cur[k] = raw[k]; } break;
                    STBI__CASE(STBI__F_sub) { cur[k] = STBI__BYTECAST(raw[k] + cur[k - output_bytes]); } break;
                    STBI__CASE(STBI__F_up) { cur[k] = STBI__BYTECAST(raw[k] + prior[k]); } break;
                    STBI__CASE(STBI__F_avg) { cur[k] = STBI__BYTECAST(raw[k] + ((prior[k] + cur[k - output_bytes]) >> 1)); } break;
                    STBI__CASE(STBI__F_paeth) { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - output_bytes], prior[k], prior[k - output_bytes])); } break;
                    STBI__CASE(STBI__F_avg_first) { cur[k] = STBI__BYTECAST(raw[k] + (cur[k - output_bytes] >> 1)); } break;
                    STBI__CASE(STBI__F_paeth_first) { cur[k] = STBI__BYTECAST(raw[k] + stbi__paeth(cur[k - output_bytes], 0, 0)); } break;
                }
#undef STBI__CASE

                // the loop above sets the high byte of the pixels' alpha, but for
                // 16 bit png files we also need the low byte set. we'll do that here.
                if (depth == 16) {
                    cur = a->out + stride * j; // start at the beginning of the row again
                    for (i = 0; i < x; ++i, cur += output_bytes) {
                        cur[filter_bytes + 1] = 255;
                    }
                }
            }
        }

        // we make a separate pass to expand bits to pixels; for performance,
        // this could run two scanlines behind the above code, so it won't
        // intefere with filtering but will still be in the cache.
        if (depth < 8) {
            for (j = 0; j < y; ++j) {
                stbi_uc* cur = a->out + stride * j;
                stbi_uc* in = a->out + stride * j + x * out_n - img_width_bytes;
                // unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit
                // png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop
                stbi_uc scale = (color == 0) ? stbi__depth_scale_table[depth] : 1; // scale grayscale values to 0..255 range

                // note that the final byte might overshoot and write more data than desired.
                // we can allocate enough data that this never writes out of memory, but it
                // could also overwrite the next scanline. can it overwrite non-empty data
                // on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.
                // so we need to explicitly clamp the final ones

                if (depth == 4) {
                    for (k = x * img_n; k >= 2; k -= 2, ++in) {
                        *cur++ = scale * ((*in >> 4));
                        *cur++ = scale * ((*in) & 0x0f);
                    }
                    if (k > 0) *cur++ = scale * ((*in >> 4));
                }
                else if (depth == 2) {
                    for (k = x * img_n; k >= 4; k -= 4, ++in) {
                        *cur++ = scale * ((*in >> 6));
                        *cur++ = scale * ((*in >> 4) & 0x03);
                        *cur++ = scale * ((*in >> 2) & 0x03);
                        *cur++ = scale * ((*in) & 0x03);
                    }
                    if (k > 0) *cur++ = scale * ((*in >> 6));
                    if (k > 1) *cur++ = scale * ((*in >> 4) & 0x03);
                    if (k > 2) *cur++ = scale * ((*in >> 2) & 0x03);
                }
                else if (depth == 1) {
                    for (k = x * img_n; k >= 8; k -= 8, ++in) {
                        *cur++ = scale * ((*in >> 7));
                        *cur++ = scale * ((*in >> 6) & 0x01);
                        *cur++ = scale * ((*in >> 5) & 0x01);
                        *cur++ = scale * ((*in >> 4) & 0x01);
                        *cur++ = scale * ((*in >> 3) & 0x01);
                        *cur++ = scale * ((*in >> 2) & 0x01);
                        *cur++ = scale * ((*in >> 1) & 0x01);
                        *cur++ = scale * ((*in) & 0x01);
                    }
                    if (k > 0) *cur++ = scale * ((*in >> 7));
                    if (k > 1) *cur++ = scale * ((*in >> 6) & 0x01);
                    if (k > 2) *cur++ = scale * ((*in >> 5) & 0x01);
                    if (k > 3) *cur++ = scale * ((*in >> 4) & 0x01);
                    if (k > 4) *cur++ = scale * ((*in >> 3) & 0x01);
                    if (k > 5) *cur++ = scale * ((*in >> 2) & 0x01);
                    if (k > 6) *cur++ = scale * ((*in >> 1) & 0x01);
                }
                if (img_n != out_n) {
                    int q;
                    // insert alpha = 255
                    cur = a->out + stride * j;
                    if (img_n == 1) {
                        for (q = x - 1; q >= 0; --q) {
                            cur[q * 2 + 1] = 255;
                            cur[q * 2 + 0] = cur[q];
                        }
                    }
                    else {
                        STBI_ASSERT(img_n == 3);
                        for (q = x - 1; q >= 0; --q) {
                            cur[q * 4 + 3] = 255;
                            cur[q * 4 + 2] = cur[q * 3 + 2];
                            cur[q * 4 + 1] = cur[q * 3 + 1];
                            cur[q * 4 + 0] = cur[q * 3 + 0];
                        }
                    }
                }
            }
        }
        else if (depth == 16) {
            // force the image data from big-endian to platform-native.
            // this is done in a separate pass due to the decoding relying
            // on the data being untouched, but could probably be done
            // per-line during decode if care is taken.
            stbi_uc* cur = a->out;
            stbi__uint16* cur16 = (stbi__uint16*)cur;

            for (i = 0; i < x * y * out_n; ++i, cur16++, cur += 2) {
                *cur16 = (cur[0] << 8) | cur[1];
            }
        }

        return 1;
    }

    static int stbi__create_png_image(stbi__png* a, stbi_uc* image_data, stbi__uint32 image_data_len, int out_n, int depth, int color, int interlaced)
    {
        int bytes = (depth == 16 ? 2 : 1);
        int out_bytes = out_n * bytes;
        stbi_uc* final;
        int p;
        if (!interlaced)
            return stbi__create_png_image_raw(a, image_data, image_data_len, out_n, a->s->img_x, a->s->img_y, depth, color);

        // de-interlacing
        final = (stbi_uc*)stbi__malloc_mad3(a->s->img_x, a->s->img_y, out_bytes, 0);
        if (!final) return stbi__err("outofmem", "Out of memory");
        for (p = 0; p < 7; ++p) {
            int xorig[] = { 0,4,0,2,0,1,0 };
            int yorig[] = { 0,0,4,0,2,0,1 };
            int xspc[] = { 8,8,4,4,2,2,1 };
            int yspc[] = { 8,8,8,4,4,2,2 };
            int i, j, x, y;
            // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
            x = (a->s->img_x - xorig[p] + xspc[p] - 1) / xspc[p];
            y = (a->s->img_y - yorig[p] + yspc[p] - 1) / yspc[p];
            if (x && y) {
                stbi__uint32 img_len = ((((a->s->img_n * x * depth) + 7) >> 3) + 1) * y;
                if (!stbi__create_png_image_raw(a, image_data, image_data_len, out_n, x, y, depth, color)) {
                    STBI_FREE(final);
                    return 0;
                }
                for (j = 0; j < y; ++j) {
                    for (i = 0; i < x; ++i) {
                        int out_y = j * yspc[p] + yorig[p];
                        int out_x = i * xspc[p] + xorig[p];
                        memcpy(final + out_y * a->s->img_x * out_bytes + out_x * out_bytes,
                            a->out + (j * x + i) * out_bytes, out_bytes);
                    }
                }
                STBI_FREE(a->out);
                image_data += img_len;
                image_data_len -= img_len;
            }
        }
        a->out = final;

        return 1;
    }

    static int stbi__compute_transparency(stbi__png* z, stbi_uc tc[3], int out_n)
    {
        stbi__context* s = z->s;
        stbi__uint32 i, pixel_count = s->img_x * s->img_y;
        stbi_uc* p = z->out;

        // compute color-based transparency, assuming we've
        // already got 255 as the alpha value in the output
        STBI_ASSERT(out_n == 2 || out_n == 4);

        if (out_n == 2) {
            for (i = 0; i < pixel_count; ++i) {
                p[1] = (p[0] == tc[0] ? 0 : 255);
                p += 2;
            }
        }
        else {
            for (i = 0; i < pixel_count; ++i) {
                if (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
                    p[3] = 0;
                p += 4;
            }
        }
        return 1;
    }

    static int stbi__compute_transparency16(stbi__png* z, stbi__uint16 tc[3], int out_n)
    {
        stbi__context* s = z->s;
        stbi__uint32 i, pixel_count = s->img_x * s->img_y;
        stbi__uint16* p = (stbi__uint16*)z->out;

        // compute color-based transparency, assuming we've
        // already got 65535 as the alpha value in the output
        STBI_ASSERT(out_n == 2 || out_n == 4);

        if (out_n == 2) {
            for (i = 0; i < pixel_count; ++i) {
                p[1] = (p[0] == tc[0] ? 0 : 65535);
                p += 2;
            }
        }
        else {
            for (i = 0; i < pixel_count; ++i) {
                if (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
                    p[3] = 0;
                p += 4;
            }
        }
        return 1;
    }

    static int stbi__expand_png_palette(stbi__png* a, stbi_uc* palette, int len, int pal_img_n)
    {
        stbi__uint32 i, pixel_count = a->s->img_x * a->s->img_y;
        stbi_uc* p, * temp_out, * orig = a->out;

        p = (stbi_uc*)stbi__malloc_mad2(pixel_count, pal_img_n, 0);
        if (p == NULL) return stbi__err("outofmem", "Out of memory");

        // between here and free(out) below, exitting would leak
        temp_out = p;

        if (pal_img_n == 3) {
            for (i = 0; i < pixel_count; ++i) {
                int n = orig[i] * 4;
                p[0] = palette[n];
                p[1] = palette[n + 1];
                p[2] = palette[n + 2];
                p += 3;
            }
        }
        else {
            for (i = 0; i < pixel_count; ++i) {
                int n = orig[i] * 4;
                p[0] = palette[n];
                p[1] = palette[n + 1];
                p[2] = palette[n + 2];
                p[3] = palette[n + 3];
                p += 4;
            }
        }
        STBI_FREE(a->out);
        a->out = temp_out;

        STBI_NOTUSED(len);

        return 1;
    }

    static int stbi__unpremultiply_on_load_global = 0;
    static int stbi__de_iphone_flag_global = 0;

    STBIDEF void stbi_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply)
    {
        stbi__unpremultiply_on_load_global = flag_true_if_should_unpremultiply;
    }

    STBIDEF void stbi_convert_iphone_png_to_rgb(int flag_true_if_should_convert)
    {
        stbi__de_iphone_flag_global = flag_true_if_should_convert;
    }

#ifndef STBI_THREAD_LOCAL
#define stbi__unpremultiply_on_load  stbi__unpremultiply_on_load_global
#define stbi__de_iphone_flag  stbi__de_iphone_flag_global
#else
    static STBI_THREAD_LOCAL int stbi__unpremultiply_on_load_local, stbi__unpremultiply_on_load_set;
    static STBI_THREAD_LOCAL int stbi__de_iphone_flag_local, stbi__de_iphone_flag_set;

    STBIDEF void stbi__unpremultiply_on_load_thread(int flag_true_if_should_unpremultiply)
    {
        stbi__unpremultiply_on_load_local = flag_true_if_should_unpremultiply;
        stbi__unpremultiply_on_load_set = 1;
    }

    STBIDEF void stbi_convert_iphone_png_to_rgb_thread(int flag_true_if_should_convert)
    {
        stbi__de_iphone_flag_local = flag_true_if_should_convert;
        stbi__de_iphone_flag_set = 1;
    }

#define stbi__unpremultiply_on_load  (stbi__unpremultiply_on_load_set           \
                                       ? stbi__unpremultiply_on_load_local      \
                                       : stbi__unpremultiply_on_load_global)
#define stbi__de_iphone_flag  (stbi__de_iphone_flag_set                         \
                                ? stbi__de_iphone_flag_local                    \
                                : stbi__de_iphone_flag_global)
#endif // STBI_THREAD_LOCAL

    static void stbi__de_iphone(stbi__png* z)
    {
        stbi__context* s = z->s;
        stbi__uint32 i, pixel_count = s->img_x * s->img_y;
        stbi_uc* p = z->out;

        if (s->img_out_n == 3) {  // convert bgr to rgb
            for (i = 0; i < pixel_count; ++i) {
                stbi_uc t = p[0];
                p[0] = p[2];
                p[2] = t;
                p += 3;
            }
        }
        else {
            STBI_ASSERT(s->img_out_n == 4);
            if (stbi__unpremultiply_on_load) {
                // convert bgr to rgb and unpremultiply
                for (i = 0; i < pixel_count; ++i) {
                    stbi_uc a = p[3];
                    stbi_uc t = p[0];
                    if (a) {
                        stbi_uc half = a / 2;
                        p[0] = (p[2] * 255 + half) / a;
                        p[1] = (p[1] * 255 + half) / a;
                        p[2] = (t * 255 + half) / a;
                    }
                    else {
                        p[0] = p[2];
                        p[2] = t;
                    }
                    p += 4;
                }
            }
            else {
                // convert bgr to rgb
                for (i = 0; i < pixel_count; ++i) {
                    stbi_uc t = p[0];
                    p[0] = p[2];
                    p[2] = t;
                    p += 4;
                }
            }
        }
    }

#define STBI__PNG_TYPE(a,b,c,d)  (((unsigned) (a) << 24) + ((unsigned) (b) << 16) + ((unsigned) (c) << 8) + (unsigned) (d))

    static int stbi__parse_png_file(stbi__png* z, int scan, int req_comp)
    {
        stbi_uc palette[1024], pal_img_n = 0;
        stbi_uc has_trans = 0, tc[3] = { 0 };
        stbi__uint16 tc16[3];
        stbi__uint32 ioff = 0, idata_limit = 0, i, pal_len = 0;
        int first = 1, k, interlace = 0, color = 0, is_iphone = 0;
        stbi__context* s = z->s;

        z->expanded = NULL;
        z->idata = NULL;
        z->out = NULL;

        if (!stbi__check_png_header(s)) return 0;

        if (scan == STBI__SCAN_type) return 1;

        for (;;) {
            stbi__pngchunk c = stbi__get_chunk_header(s);
            switch (c.type) {
            case STBI__PNG_TYPE('C', 'g', 'B', 'I'):
                is_iphone = 1;
                stbi__skip(s, c.length);
                break;
            case STBI__PNG_TYPE('I', 'H', 'D', 'R'): {
                int comp, filter;
                if (!first) return stbi__err("multiple IHDR", "Corrupt PNG");
                first = 0;
                if (c.length != 13) return stbi__err("bad IHDR len", "Corrupt PNG");
                s->img_x = stbi__get32be(s);
                s->img_y = stbi__get32be(s);
                if (s->img_y > STBI_MAX_DIMENSIONS) return stbi__err("too large", "Very large image (corrupt?)");
                if (s->img_x > STBI_MAX_DIMENSIONS) return stbi__err("too large", "Very large image (corrupt?)");
                z->depth = stbi__get8(s);  if (z->depth != 1 && z->depth != 2 && z->depth != 4 && z->depth != 8 && z->depth != 16)  return stbi__err("1/2/4/8/16-bit only", "PNG not supported: 1/2/4/8/16-bit only");
                color = stbi__get8(s);  if (color > 6)         return stbi__err("bad ctype", "Corrupt PNG");
                if (color == 3 && z->depth == 16)                  return stbi__err("bad ctype", "Corrupt PNG");
                if (color == 3) pal_img_n = 3; else if (color & 1) return stbi__err("bad ctype", "Corrupt PNG");
                comp = stbi__get8(s);  if (comp) return stbi__err("bad comp method", "Corrupt PNG");
                filter = stbi__get8(s);  if (filter) return stbi__err("bad filter method", "Corrupt PNG");
                interlace = stbi__get8(s); if (interlace > 1) return stbi__err("bad interlace method", "Corrupt PNG");
                if (!s->img_x || !s->img_y) return stbi__err("0-pixel image", "Corrupt PNG");
                if (!pal_img_n) {
                    s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
                    if ((1 << 30) / s->img_x / s->img_n < s->img_y) return stbi__err("too large", "Image too large to decode");
                    if (scan == STBI__SCAN_header) return 1;
                }
                else {
                    // if paletted, then pal_n is our final components, and
                    // img_n is # components to decompress/filter.
                    s->img_n = 1;
                    if ((1 << 30) / s->img_x / 4 < s->img_y) return stbi__err("too large", "Corrupt PNG");
                    // if SCAN_header, have to scan to see if we have a tRNS
                }
                break;
            }

            case STBI__PNG_TYPE('P', 'L', 'T', 'E'): {
                if (first) return stbi__err("first not IHDR", "Corrupt PNG");
                if (c.length > 256 * 3) return stbi__err("invalid PLTE", "Corrupt PNG");
                pal_len = c.length / 3;
                if (pal_len * 3 != c.length) return stbi__err("invalid PLTE", "Corrupt PNG");
                for (i = 0; i < pal_len; ++i) {
                    palette[i * 4 + 0] = stbi__get8(s);
                    palette[i * 4 + 1] = stbi__get8(s);
                    palette[i * 4 + 2] = stbi__get8(s);
                    palette[i * 4 + 3] = 255;
                }
                break;
            }

            case STBI__PNG_TYPE('t', 'R', 'N', 'S'): {
                if (first) return stbi__err("first not IHDR", "Corrupt PNG");
                if (z->idata) return stbi__err("tRNS after IDAT", "Corrupt PNG");
                if (pal_img_n) {
                    if (scan == STBI__SCAN_header) { s->img_n = 4; return 1; }
                    if (pal_len == 0) return stbi__err("tRNS before PLTE", "Corrupt PNG");
                    if (c.length > pal_len) return stbi__err("bad tRNS len", "Corrupt PNG");
                    pal_img_n = 4;
                    for (i = 0; i < c.length; ++i)
                        palette[i * 4 + 3] = stbi__get8(s);
                }
                else {
                    if (!(s->img_n & 1)) return stbi__err("tRNS with alpha", "Corrupt PNG");
                    if (c.length != (stbi__uint32)s->img_n * 2) return stbi__err("bad tRNS len", "Corrupt PNG");
                    has_trans = 1;
                    if (z->depth == 16) {
                        for (k = 0; k < s->img_n; ++k) tc16[k] = (stbi__uint16)stbi__get16be(s); // copy the values as-is
                    }
                    else {
                        for (k = 0; k < s->img_n; ++k) tc[k] = (stbi_uc)(stbi__get16be(s) & 255) * stbi__depth_scale_table[z->depth]; // non 8-bit images will be larger
                    }
                }
                break;
            }

            case STBI__PNG_TYPE('I', 'D', 'A', 'T'): {
                if (first) return stbi__err("first not IHDR", "Corrupt PNG");
                if (pal_img_n && !pal_len) return stbi__err("no PLTE", "Corrupt PNG");
                if (scan == STBI__SCAN_header) { s->img_n = pal_img_n; return 1; }
                if ((int)(ioff + c.length) < (int)ioff) return 0;
                if (ioff + c.length > idata_limit) {
                    stbi__uint32 idata_limit_old = idata_limit;
                    stbi_uc* p;
                    if (idata_limit == 0) idata_limit = c.length > 4096 ? c.length : 4096;
                    while (ioff + c.length > idata_limit)
                        idata_limit *= 2;
                    STBI_NOTUSED(idata_limit_old);
                    p = (stbi_uc*)STBI_REALLOC_SIZED(z->idata, idata_limit_old, idata_limit); if (p == NULL) return stbi__err("outofmem", "Out of memory");
                    z->idata = p;
                }
                if (!stbi__getn(s, z->idata + ioff, c.length)) return stbi__err("outofdata", "Corrupt PNG");
                ioff += c.length;
                break;
            }

            case STBI__PNG_TYPE('I', 'E', 'N', 'D'): {
                stbi__uint32 raw_len, bpl;
                if (first) return stbi__err("first not IHDR", "Corrupt PNG");
                if (scan != STBI__SCAN_load) return 1;
                if (z->idata == NULL) return stbi__err("no IDAT", "Corrupt PNG");
                // initial guess for decoded data size to avoid unnecessary reallocs
                bpl = (s->img_x * z->depth + 7) / 8; // bytes per line, per component
                raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
                z->expanded = (stbi_uc*)stbi_zlib_decode_malloc_guesssize_headerflag((char*)z->idata, ioff, raw_len, (int*)&raw_len, !is_iphone);
                if (z->expanded == NULL) return 0; // zlib should set error
                STBI_FREE(z->idata); z->idata = NULL;
                if ((req_comp == s->img_n + 1 && req_comp != 3 && !pal_img_n) || has_trans)
                    s->img_out_n = s->img_n + 1;
                else
                    s->img_out_n = s->img_n;
                if (!stbi__create_png_image(z, z->expanded, raw_len, s->img_out_n, z->depth, color, interlace)) return 0;
                if (has_trans) {
                    if (z->depth == 16) {
                        if (!stbi__compute_transparency16(z, tc16, s->img_out_n)) return 0;
                    }
                    else {
                        if (!stbi__compute_transparency(z, tc, s->img_out_n)) return 0;
                    }
                }
                if (is_iphone && stbi__de_iphone_flag && s->img_out_n > 2)
                    stbi__de_iphone(z);
                if (pal_img_n) {
                    // pal_img_n == 3 or 4
                    s->img_n = pal_img_n; // record the actual colors we had
                    s->img_out_n = pal_img_n;
                    if (req_comp >= 3) s->img_out_n = req_comp;
                    if (!stbi__expand_png_palette(z, palette, pal_len, s->img_out_n))
                        return 0;
                }
                else if (has_trans) {
                    // non-paletted image with tRNS -> source image has (constant) alpha
                    ++s->img_n;
                }
                STBI_FREE(z->expanded); z->expanded = NULL;
                // end of PNG chunk, read and skip CRC
                stbi__get32be(s);
                return 1;
            }

            default:
                // if critical, fail
                if (first) return stbi__err("first not IHDR", "Corrupt PNG");
                if ((c.type & (1 << 29)) == 0) {
#ifndef STBI_NO_FAILURE_STRINGS
                    // not threadsafe
                    static char invalid_chunk[] = "XXXX PNG chunk not known";
                    invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
                    invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
                    invalid_chunk[2] = STBI__BYTECAST(c.type >> 8);
                    invalid_chunk[3] = STBI__BYTECAST(c.type >> 0);
#endif
                    return stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
                }
                stbi__skip(s, c.length);
                break;
            }
            // end of PNG chunk, read and skip CRC
            stbi__get32be(s);
        }
    }

    static void* stbi__do_png(stbi__png* p, int* x, int* y, int* n, int req_comp, stbi__result_info* ri)
    {
        void* result = NULL;
        if (req_comp < 0 || req_comp > 4) return stbi__errpuc("bad req_comp", "Internal error");
        if (stbi__parse_png_file(p, STBI__SCAN_load, req_comp)) {
            if (p->depth <= 8)
                ri->bits_per_channel = 8;
            else if (p->depth == 16)
                ri->bits_per_channel = 16;
            else
                return stbi__errpuc("bad bits_per_channel", "PNG not supported: unsupported color depth");
            result = p->out;
            p->out = NULL;
            if (req_comp && req_comp != p->s->img_out_n) {
                if (ri->bits_per_channel == 8)
                    result = stbi__convert_format((unsigned char*)result, p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y);
                else
                    result = stbi__convert_format16((stbi__uint16*)result, p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y);
                p->s->img_out_n = req_comp;
                if (result == NULL) return result;
            }
            *x = p->s->img_x;
            *y = p->s->img_y;
            if (n) *n = p->s->img_n;
        }
        STBI_FREE(p->out);      p->out = NULL;
        STBI_FREE(p->expanded); p->expanded = NULL;
        STBI_FREE(p->idata);    p->idata = NULL;

        return result;
    }

    static void* stbi__png_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri)
    {
        stbi__png p;
        p.s = s;
        return stbi__do_png(&p, x, y, comp, req_comp, ri);
    }

    static int stbi__png_test(stbi__context* s)
    {
        int r;
        r = stbi__check_png_header(s);
        stbi__rewind(s);
        return r;
    }

    static int stbi__png_info_raw(stbi__png* p, int* x, int* y, int* comp)
    {
        if (!stbi__parse_png_file(p, STBI__SCAN_header, 0)) {
            stbi__rewind(p->s);
            return 0;
        }
        if (x) *x = p->s->img_x;
        if (y) *y = p->s->img_y;
        if (comp) *comp = p->s->img_n;
        return 1;
    }

    static int stbi__png_info(stbi__context* s, int* x, int* y, int* comp)
    {
        stbi__png p;
        p.s = s;
        return stbi__png_info_raw(&p, x, y, comp);
    }

    static int stbi__png_is16(stbi__context* s)
    {
        stbi__png p;
        p.s = s;
        if (!stbi__png_info_raw(&p, NULL, NULL, NULL))
            return 0;
        if (p.depth != 16) {
            stbi__rewind(p.s);
            return 0;
        }
        return 1;
    }
#endif

    // Microsoft/Windows BMP image

#ifndef STBI_NO_BMP
    static int stbi__bmp_test_raw(stbi__context* s)
    {
        int r;
        int sz;
        if (stbi__get8(s) != 'B') return 0;
        if (stbi__get8(s) != 'M') return 0;
        stbi__get32le(s); // discard filesize
        stbi__get16le(s); // discard reserved
        stbi__get16le(s); // discard reserved
        stbi__get32le(s); // discard data offset
        sz = stbi__get32le(s);
        r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
        return r;
    }

    static int stbi__bmp_test(stbi__context* s)
    {
        int r = stbi__bmp_test_raw(s);
        stbi__rewind(s);
        return r;
    }


    // returns 0..31 for the highest set bit
    static int stbi__high_bit(unsigned int z)
    {
        int n = 0;
        if (z == 0) return -1;
        if (z >= 0x10000) { n += 16; z >>= 16; }
        if (z >= 0x00100) { n += 8; z >>= 8; }
        if (z >= 0x00010) { n += 4; z >>= 4; }
        if (z >= 0x00004) { n += 2; z >>= 2; }
        if (z >= 0x00002) { n += 1;/* >>=  1;*/ }
        return n;
    }

    static int stbi__bitcount(unsigned int a)
    {
        a = (a & 0x55555555) + ((a >> 1) & 0x55555555); // max 2
        a = (a & 0x33333333) + ((a >> 2) & 0x33333333); // max 4
        a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
        a = (a + (a >> 8)); // max 16 per 8 bits
        a = (a + (a >> 16)); // max 32 per 8 bits
        return a & 0xff;
    }

    // extract an arbitrarily-aligned N-bit value (N=bits)
    // from v, and then make it 8-bits long and fractionally
    // extend it to full full range.
    static int stbi__shiftsigned(unsigned int v, int shift, int bits)
    {
        static unsigned int mul_table[9] = {
           0,
           0xff/*0b11111111*/, 0x55/*0b01010101*/, 0x49/*0b01001001*/, 0x11/*0b00010001*/,
           0x21/*0b00100001*/, 0x41/*0b01000001*/, 0x81/*0b10000001*/, 0x01/*0b00000001*/,
        };
        static unsigned int shift_table[9] = {
           0, 0,0,1,0,2,4,6,0,
        };
        if (shift < 0)
            v <<= -shift;
        else
            v >>= shift;
        STBI_ASSERT(v < 256);
        v >>= (8 - bits);
        STBI_ASSERT(bits >= 0 && bits <= 8);
        return (int)((unsigned)v * mul_table[bits]) >> shift_table[bits];
    }

    typedef struct
    {
        int bpp, offset, hsz;
        unsigned int mr, mg, mb, ma, all_a;
        int extra_read;
    } stbi__bmp_data;

    static int stbi__bmp_set_mask_defaults(stbi__bmp_data* info, int compress)
    {
        // BI_BITFIELDS specifies masks explicitly, don't override
        if (compress == 3)
            return 1;

        if (compress == 0) {
            if (info->bpp == 16) {
                info->mr = 31u << 10;
                info->mg = 31u << 5;
                info->mb = 31u << 0;
            }
            else if (info->bpp == 32) {
                info->mr = 0xffu << 16;
                info->mg = 0xffu << 8;
                info->mb = 0xffu << 0;
                info->ma = 0xffu << 24;
                info->all_a = 0; // if all_a is 0 at end, then we loaded alpha channel but it was all 0
            }
            else {
                // otherwise, use defaults, which is all-0
                info->mr = info->mg = info->mb = info->ma = 0;
            }
            return 1;
        }
        return 0; // error
    }

    static void* stbi__bmp_parse_header(stbi__context* s, stbi__bmp_data* info)
    {
        int hsz;
        if (stbi__get8(s) != 'B' || stbi__get8(s) != 'M') return stbi__errpuc("not BMP", "Corrupt BMP");
        stbi__get32le(s); // discard filesize
        stbi__get16le(s); // discard reserved
        stbi__get16le(s); // discard reserved
        info->offset = stbi__get32le(s);
        info->hsz = hsz = stbi__get32le(s);
        info->mr = info->mg = info->mb = info->ma = 0;
        info->extra_read = 14;

        if (info->offset < 0) return stbi__errpuc("bad BMP", "bad BMP");

        if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) return stbi__errpuc("unknown BMP", "BMP type not supported: unknown");
        if (hsz == 12) {
            s->img_x = stbi__get16le(s);
            s->img_y = stbi__get16le(s);
        }
        else {
            s->img_x = stbi__get32le(s);
            s->img_y = stbi__get32le(s);
        }
        if (stbi__get16le(s) != 1) return stbi__errpuc("bad BMP", "bad BMP");
        info->bpp = stbi__get16le(s);
        if (hsz != 12) {
            int compress = stbi__get32le(s);
            if (compress == 1 || compress == 2) return stbi__errpuc("BMP RLE", "BMP type not supported: RLE");
            if (compress >= 4) return stbi__errpuc("BMP JPEG/PNG", "BMP type not supported: unsupported compression"); // this includes PNG/JPEG modes
            if (compress == 3 && info->bpp != 16 && info->bpp != 32) return stbi__errpuc("bad BMP", "bad BMP"); // bitfields requires 16 or 32 bits/pixel
            stbi__get32le(s); // discard sizeof
            stbi__get32le(s); // discard hres
            stbi__get32le(s); // discard vres
            stbi__get32le(s); // discard colorsused
            stbi__get32le(s); // discard max important
            if (hsz == 40 || hsz == 56) {
                if (hsz == 56) {
                    stbi__get32le(s);
                    stbi__get32le(s);
                    stbi__get32le(s);
                    stbi__get32le(s);
                }
                if (info->bpp == 16 || info->bpp == 32) {
                    if (compress == 0) {
                        stbi__bmp_set_mask_defaults(info, compress);
                    }
                    else if (compress == 3) {
                        info->mr = stbi__get32le(s);
                        info->mg = stbi__get32le(s);
                        info->mb = stbi__get32le(s);
                        info->extra_read += 12;
                        // not documented, but generated by photoshop and handled by mspaint
                        if (info->mr == info->mg && info->mg == info->mb) {
                            // ?!?!?
                            return stbi__errpuc("bad BMP", "bad BMP");
                        }
                    }
                    else
                        return stbi__errpuc("bad BMP", "bad BMP");
                }
            }
            else {
                // V4/V5 header
                int i;
                if (hsz != 108 && hsz != 124)
                    return stbi__errpuc("bad BMP", "bad BMP");
                info->mr = stbi__get32le(s);
                info->mg = stbi__get32le(s);
                info->mb = stbi__get32le(s);
                info->ma = stbi__get32le(s);
                if (compress != 3) // override mr/mg/mb unless in BI_BITFIELDS mode, as per docs
                    stbi__bmp_set_mask_defaults(info, compress);
                stbi__get32le(s); // discard color space
                for (i = 0; i < 12; ++i)
                    stbi__get32le(s); // discard color space parameters
                if (hsz == 124) {
                    stbi__get32le(s); // discard rendering intent
                    stbi__get32le(s); // discard offset of profile data
                    stbi__get32le(s); // discard size of profile data
                    stbi__get32le(s); // discard reserved
                }
            }
        }
        return (void*)1;
    }


    static void* stbi__bmp_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri)
    {
        stbi_uc* out;
        unsigned int mr = 0, mg = 0, mb = 0, ma = 0, all_a;
        stbi_uc pal[256][4];
        int psize = 0, i, j, width;
        int flip_vertically, pad, target;
        stbi__bmp_data info;
        STBI_NOTUSED(ri);

        info.all_a = 255;
        if (stbi__bmp_parse_header(s, &info) == NULL)
            return NULL; // error code already set

        flip_vertically = ((int)s->img_y) > 0;
        s->img_y = abs((int)s->img_y);

        if (s->img_y > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");
        if (s->img_x > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");

        mr = info.mr;
        mg = info.mg;
        mb = info.mb;
        ma = info.ma;
        all_a = info.all_a;

        if (info.hsz == 12) {
            if (info.bpp < 24)
                psize = (info.offset - info.extra_read - 24) / 3;
        }
        else {
            if (info.bpp < 16)
                psize = (info.offset - info.extra_read - info.hsz) >> 2;
        }
        if (psize == 0) {
            if (info.offset != s->callback_already_read + (s->img_buffer - s->img_buffer_original)) {
                return stbi__errpuc("bad offset", "Corrupt BMP");
            }
        }

        if (info.bpp == 24 && ma == 0xff000000)
            s->img_n = 3;
        else
            s->img_n = ma ? 4 : 3;
        if (req_comp && req_comp >= 3) // we can directly decode 3 or 4
            target = req_comp;
        else
            target = s->img_n; // if they want monochrome, we'll post-convert

        // sanity-check size
        if (!stbi__mad3sizes_valid(target, s->img_x, s->img_y, 0))
            return stbi__errpuc("too large", "Corrupt BMP");

        out = (stbi_uc*)stbi__malloc_mad3(target, s->img_x, s->img_y, 0);
        if (!out) return stbi__errpuc("outofmem", "Out of memory");
        if (info.bpp < 16) {
            int z = 0;
            if (psize == 0 || psize > 256) { STBI_FREE(out); return stbi__errpuc("invalid", "Corrupt BMP"); }
            for (i = 0; i < psize; ++i) {
                pal[i][2] = stbi__get8(s);
                pal[i][1] = stbi__get8(s);
                pal[i][0] = stbi__get8(s);
                if (info.hsz != 12) stbi__get8(s);
                pal[i][3] = 255;
            }
            stbi__skip(s, info.offset - info.extra_read - info.hsz - psize * (info.hsz == 12 ? 3 : 4));
            if (info.bpp == 1) width = (s->img_x + 7) >> 3;
            else if (info.bpp == 4) width = (s->img_x + 1) >> 1;
            else if (info.bpp == 8) width = s->img_x;
            else { STBI_FREE(out); return stbi__errpuc("bad bpp", "Corrupt BMP"); }
            pad = (-width) & 3;
            if (info.bpp == 1) {
                for (j = 0; j < (int)s->img_y; ++j) {
                    int bit_offset = 7, v = stbi__get8(s);
                    for (i = 0; i < (int)s->img_x; ++i) {
                        int color = (v >> bit_offset) & 0x1;
                        out[z++] = pal[color][0];
                        out[z++] = pal[color][1];
                        out[z++] = pal[color][2];
                        if (target == 4) out[z++] = 255;
                        if (i + 1 == (int)s->img_x) break;
                        if ((--bit_offset) < 0) {
                            bit_offset = 7;
                            v = stbi__get8(s);
                        }
                    }
                    stbi__skip(s, pad);
                }
            }
            else {
                for (j = 0; j < (int)s->img_y; ++j) {
                    for (i = 0; i < (int)s->img_x; i += 2) {
                        int v = stbi__get8(s), v2 = 0;
                        if (info.bpp == 4) {
                            v2 = v & 15;
                            v >>= 4;
                        }
                        out[z++] = pal[v][0];
                        out[z++] = pal[v][1];
                        out[z++] = pal[v][2];
                        if (target == 4) out[z++] = 255;
                        if (i + 1 == (int)s->img_x) break;
                        v = (info.bpp == 8) ? stbi__get8(s) : v2;
                        out[z++] = pal[v][0];
                        out[z++] = pal[v][1];
                        out[z++] = pal[v][2];
                        if (target == 4) out[z++] = 255;
                    }
                    stbi__skip(s, pad);
                }
            }
        }
        else {
            int rshift = 0, gshift = 0, bshift = 0, ashift = 0, rcount = 0, gcount = 0, bcount = 0, acount = 0;
            int z = 0;
            int easy = 0;
            stbi__skip(s, info.offset - info.extra_read - info.hsz);
            if (info.bpp == 24) width = 3 * s->img_x;
            else if (info.bpp == 16) width = 2 * s->img_x;
            else /* bpp = 32 and pad = 0 */ width = 0;
            pad = (-width) & 3;
            if (info.bpp == 24) {
                easy = 1;
            }
            else if (info.bpp == 32) {
                if (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
                    easy = 2;
            }
            if (!easy) {
                if (!mr || !mg || !mb) { STBI_FREE(out); return stbi__errpuc("bad masks", "Corrupt BMP"); }
                // right shift amt to put high bit in position #7
                rshift = stbi__high_bit(mr) - 7; rcount = stbi__bitcount(mr);
                gshift = stbi__high_bit(mg) - 7; gcount = stbi__bitcount(mg);
                bshift = stbi__high_bit(mb) - 7; bcount = stbi__bitcount(mb);
                ashift = stbi__high_bit(ma) - 7; acount = stbi__bitcount(ma);
                if (rcount > 8 || gcount > 8 || bcount > 8 || acount > 8) { STBI_FREE(out); return stbi__errpuc("bad masks", "Corrupt BMP"); }
            }
            for (j = 0; j < (int)s->img_y; ++j) {
                if (easy) {
                    for (i = 0; i < (int)s->img_x; ++i) {
                        unsigned char a;
                        out[z + 2] = stbi__get8(s);
                        out[z + 1] = stbi__get8(s);
                        out[z + 0] = stbi__get8(s);
                        z += 3;
                        a = (easy == 2 ? stbi__get8(s) : 255);
                        all_a |= a;
                        if (target == 4) out[z++] = a;
                    }
                }
                else {
                    int bpp = info.bpp;
                    for (i = 0; i < (int)s->img_x; ++i) {
                        stbi__uint32 v = (bpp == 16 ? (stbi__uint32)stbi__get16le(s) : stbi__get32le(s));
                        unsigned int a;
                        out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
                        out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
                        out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));
                        a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
                        all_a |= a;
                        if (target == 4) out[z++] = STBI__BYTECAST(a);
                    }
                }
                stbi__skip(s, pad);
            }
        }

        // if alpha channel is all 0s, replace with all 255s
        if (target == 4 && all_a == 0)
            for (i = 4 * s->img_x * s->img_y - 1; i >= 0; i -= 4)
                out[i] = 255;

        if (flip_vertically) {
            stbi_uc t;
            for (j = 0; j < (int)s->img_y >> 1; ++j) {
                stbi_uc* p1 = out + j * s->img_x * target;
                stbi_uc* p2 = out + (s->img_y - 1 - j) * s->img_x * target;
                for (i = 0; i < (int)s->img_x * target; ++i) {
                    t = p1[i]; p1[i] = p2[i]; p2[i] = t;
                }
            }
        }

        if (req_comp && req_comp != target) {
            out = stbi__convert_format(out, target, req_comp, s->img_x, s->img_y);
            if (out == NULL) return out; // stbi__convert_format frees input on failure
        }

        *x = s->img_x;
        *y = s->img_y;
        if (comp) *comp = s->img_n;
        return out;
    }
#endif

    // Targa Truevision - TGA
    // by Jonathan Dummer
#ifndef STBI_NO_TGA
// returns STBI_rgb or whatever, 0 on error
    static int stbi__tga_get_comp(int bits_per_pixel, int is_grey, int* is_rgb16)
    {
        // only RGB or RGBA (incl. 16bit) or grey allowed
        if (is_rgb16) *is_rgb16 = 0;
        switch (bits_per_pixel) {
        case 8:  return STBI_grey;
        case 16: if (is_grey) return STBI_grey_alpha;
            // fallthrough
        case 15: if (is_rgb16) *is_rgb16 = 1;
            return STBI_rgb;
        case 24: // fallthrough
        case 32: return bits_per_pixel / 8;
        default: return 0;
        }
    }

    static int stbi__tga_info(stbi__context* s, int* x, int* y, int* comp)
    {
        int tga_w, tga_h, tga_comp, tga_image_type, tga_bits_per_pixel, tga_colormap_bpp;
        int sz, tga_colormap_type;
        stbi__get8(s);                   // discard Offset
        tga_colormap_type = stbi__get8(s); // colormap type
        if (tga_colormap_type > 1) {
            stbi__rewind(s);
            return 0;      // only RGB or indexed allowed
        }
        tga_image_type = stbi__get8(s); // image type
        if (tga_colormap_type == 1) { // colormapped (paletted) image
            if (tga_image_type != 1 && tga_image_type != 9) {
                stbi__rewind(s);
                return 0;
            }
            stbi__skip(s, 4);       // skip index of first colormap entry and number of entries
            sz = stbi__get8(s);    //   check bits per palette color entry
            if ((sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32)) {
                stbi__rewind(s);
                return 0;
            }
            stbi__skip(s, 4);       // skip image x and y origin
            tga_colormap_bpp = sz;
        }
        else { // "normal" image w/o colormap - only RGB or grey allowed, +/- RLE
            if ((tga_image_type != 2) && (tga_image_type != 3) && (tga_image_type != 10) && (tga_image_type != 11)) {
                stbi__rewind(s);
                return 0; // only RGB or grey allowed, +/- RLE
            }
            stbi__skip(s, 9); // skip colormap specification and image x/y origin
            tga_colormap_bpp = 0;
        }
        tga_w = stbi__get16le(s);
        if (tga_w < 1) {
            stbi__rewind(s);
            return 0;   // test width
        }
        tga_h = stbi__get16le(s);
        if (tga_h < 1) {
            stbi__rewind(s);
            return 0;   // test height
        }
        tga_bits_per_pixel = stbi__get8(s); // bits per pixel
        stbi__get8(s); // ignore alpha bits
        if (tga_colormap_bpp != 0) {
            if ((tga_bits_per_pixel != 8) && (tga_bits_per_pixel != 16)) {
                // when using a colormap, tga_bits_per_pixel is the size of the indexes
                // I don't think anything but 8 or 16bit indexes makes sense
                stbi__rewind(s);
                return 0;
            }
            tga_comp = stbi__tga_get_comp(tga_colormap_bpp, 0, NULL);
        }
        else {
            tga_comp = stbi__tga_get_comp(tga_bits_per_pixel, (tga_image_type == 3) || (tga_image_type == 11), NULL);
        }
        if (!tga_comp) {
            stbi__rewind(s);
            return 0;
        }
        if (x) *x = tga_w;
        if (y) *y = tga_h;
        if (comp) *comp = tga_comp;
        return 1;                   // seems to have passed everything
    }

    static int stbi__tga_test(stbi__context* s)
    {
        int res = 0;
        int sz, tga_color_type;
        stbi__get8(s);      //   discard Offset
        tga_color_type = stbi__get8(s);   //   color type
        if (tga_color_type > 1) goto errorEnd;   //   only RGB or indexed allowed
        sz = stbi__get8(s);   //   image type
        if (tga_color_type == 1) { // colormapped (paletted) image
            if (sz != 1 && sz != 9) goto errorEnd; // colortype 1 demands image type 1 or 9
            stbi__skip(s, 4);       // skip index of first colormap entry and number of entries
            sz = stbi__get8(s);    //   check bits per palette color entry
            if ((sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32)) goto errorEnd;
            stbi__skip(s, 4);       // skip image x and y origin
        }
        else { // "normal" image w/o colormap
            if ((sz != 2) && (sz != 3) && (sz != 10) && (sz != 11)) goto errorEnd; // only RGB or grey allowed, +/- RLE
            stbi__skip(s, 9); // skip colormap specification and image x/y origin
        }
        if (stbi__get16le(s) < 1) goto errorEnd;      //   test width
        if (stbi__get16le(s) < 1) goto errorEnd;      //   test height
        sz = stbi__get8(s);   //   bits per pixel
        if ((tga_color_type == 1) && (sz != 8) && (sz != 16)) goto errorEnd; // for colormapped images, bpp is size of an index
        if ((sz != 8) && (sz != 15) && (sz != 16) && (sz != 24) && (sz != 32)) goto errorEnd;

        res = 1; // if we got this far, everything's good and we can return 1 instead of 0

    errorEnd:
        stbi__rewind(s);
        return res;
    }

    // read 16bit value and convert to 24bit RGB
    static void stbi__tga_read_rgb16(stbi__context* s, stbi_uc* out)
    {
        stbi__uint16 px = (stbi__uint16)stbi__get16le(s);
        stbi__uint16 fiveBitMask = 31;
        // we have 3 channels with 5bits each
        int r = (px >> 10) & fiveBitMask;
        int g = (px >> 5) & fiveBitMask;
        int b = px & fiveBitMask;
        // Note that this saves the data in RGB(A) order, so it doesn't need to be swapped later
        out[0] = (stbi_uc)((r * 255) / 31);
        out[1] = (stbi_uc)((g * 255) / 31);
        out[2] = (stbi_uc)((b * 255) / 31);

        // some people claim that the most significant bit might be used for alpha
        // (possibly if an alpha-bit is set in the "image descriptor byte")
        // but that only made 16bit test images completely translucent..
        // so let's treat all 15 and 16bit TGAs as RGB with no alpha.
    }

    static void* stbi__tga_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri)
    {
        //   read in the TGA header stuff
        int tga_offset = stbi__get8(s);
        int tga_indexed = stbi__get8(s);
        int tga_image_type = stbi__get8(s);
        int tga_is_RLE = 0;
        int tga_palette_start = stbi__get16le(s);
        int tga_palette_len = stbi__get16le(s);
        int tga_palette_bits = stbi__get8(s);
        int tga_x_origin = stbi__get16le(s);
        int tga_y_origin = stbi__get16le(s);
        int tga_width = stbi__get16le(s);
        int tga_height = stbi__get16le(s);
        int tga_bits_per_pixel = stbi__get8(s);
        int tga_comp, tga_rgb16 = 0;
        int tga_inverted = stbi__get8(s);
        // int tga_alpha_bits = tga_inverted & 15; // the 4 lowest bits - unused (useless?)
        //   image data
        unsigned char* tga_data;
        unsigned char* tga_palette = NULL;
        int i, j;
        unsigned char raw_data[4] = { 0 };
        int RLE_count = 0;
        int RLE_repeating = 0;
        int read_next_pixel = 1;
        STBI_NOTUSED(ri);
        STBI_NOTUSED(tga_x_origin); // @TODO
        STBI_NOTUSED(tga_y_origin); // @TODO

        if (tga_height > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");
        if (tga_width > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");

        //   do a tiny bit of precessing
        if (tga_image_type >= 8)
        {
            tga_image_type -= 8;
            tga_is_RLE = 1;
        }
        tga_inverted = 1 - ((tga_inverted >> 5) & 1);

        //   If I'm paletted, then I'll use the number of bits from the palette
        if (tga_indexed) tga_comp = stbi__tga_get_comp(tga_palette_bits, 0, &tga_rgb16);
        else tga_comp = stbi__tga_get_comp(tga_bits_per_pixel, (tga_image_type == 3), &tga_rgb16);

        if (!tga_comp) // shouldn't really happen, stbi__tga_test() should have ensured basic consistency
            return stbi__errpuc("bad format", "Can't find out TGA pixelformat");

        //   tga info
        *x = tga_width;
        *y = tga_height;
        if (comp) *comp = tga_comp;

        if (!stbi__mad3sizes_valid(tga_width, tga_height, tga_comp, 0))
            return stbi__errpuc("too large", "Corrupt TGA");

        tga_data = (unsigned char*)stbi__malloc_mad3(tga_width, tga_height, tga_comp, 0);
        if (!tga_data) return stbi__errpuc("outofmem", "Out of memory");

        // skip to the data's starting position (offset usually = 0)
        stbi__skip(s, tga_offset);

        if (!tga_indexed && !tga_is_RLE && !tga_rgb16) {
            for (i = 0; i < tga_height; ++i) {
                int row = tga_inverted ? tga_height - i - 1 : i;
                stbi_uc* tga_row = tga_data + row * tga_width * tga_comp;
                stbi__getn(s, tga_row, tga_width * tga_comp);
            }
        }
        else {
            //   do I need to load a palette?
            if (tga_indexed)
            {
                if (tga_palette_len == 0) {  /* you have to have at least one entry! */
                    STBI_FREE(tga_data);
                    return stbi__errpuc("bad palette", "Corrupt TGA");
                }

                //   any data to skip? (offset usually = 0)
                stbi__skip(s, tga_palette_start);
                //   load the palette
                tga_palette = (unsigned char*)stbi__malloc_mad2(tga_palette_len, tga_comp, 0);
                if (!tga_palette) {
                    STBI_FREE(tga_data);
                    return stbi__errpuc("outofmem", "Out of memory");
                }
                if (tga_rgb16) {
                    stbi_uc* pal_entry = tga_palette;
                    STBI_ASSERT(tga_comp == STBI_rgb);
                    for (i = 0; i < tga_palette_len; ++i) {
                        stbi__tga_read_rgb16(s, pal_entry);
                        pal_entry += tga_comp;
                    }
                }
                else if (!stbi__getn(s, tga_palette, tga_palette_len * tga_comp)) {
                    STBI_FREE(tga_data);
                    STBI_FREE(tga_palette);
                    return stbi__errpuc("bad palette", "Corrupt TGA");
                }
            }
            //   load the data
            for (i = 0; i < tga_width * tga_height; ++i)
            {
                //   if I'm in RLE mode, do I need to get a RLE stbi__pngchunk?
                if (tga_is_RLE)
                {
                    if (RLE_count == 0)
                    {
                        //   yep, get the next byte as a RLE command
                        int RLE_cmd = stbi__get8(s);
                        RLE_count = 1 + (RLE_cmd & 127);
                        RLE_repeating = RLE_cmd >> 7;
                        read_next_pixel = 1;
                    }
                    else if (!RLE_repeating)
                    {
                        read_next_pixel = 1;
                    }
                }
                else
                {
                    read_next_pixel = 1;
                }
                //   OK, if I need to read a pixel, do it now
                if (read_next_pixel)
                {
                    //   load however much data we did have
                    if (tga_indexed)
                    {
                        // read in index, then perform the lookup
                        int pal_idx = (tga_bits_per_pixel == 8) ? stbi__get8(s) : stbi__get16le(s);
                        if (pal_idx >= tga_palette_len) {
                            // invalid index
                            pal_idx = 0;
                        }
                        pal_idx *= tga_comp;
                        for (j = 0; j < tga_comp; ++j) {
                            raw_data[j] = tga_palette[pal_idx + j];
                        }
                    }
                    else if (tga_rgb16) {
                        STBI_ASSERT(tga_comp == STBI_rgb);
                        stbi__tga_read_rgb16(s, raw_data);
                    }
                    else {
                        //   read in the data raw
                        for (j = 0; j < tga_comp; ++j) {
                            raw_data[j] = stbi__get8(s);
                        }
                    }
                    //   clear the reading flag for the next pixel
                    read_next_pixel = 0;
                } // end of reading a pixel

                // copy data
                for (j = 0; j < tga_comp; ++j)
                    tga_data[i * tga_comp + j] = raw_data[j];

                //   in case we're in RLE mode, keep counting down
                --RLE_count;
            }
            //   do I need to invert the image?
            if (tga_inverted)
            {
                for (j = 0; j * 2 < tga_height; ++j)
                {
                    int index1 = j * tga_width * tga_comp;
                    int index2 = (tga_height - 1 - j) * tga_width * tga_comp;
                    for (i = tga_width * tga_comp; i > 0; --i)
                    {
                        unsigned char temp = tga_data[index1];
                        tga_data[index1] = tga_data[index2];
                        tga_data[index2] = temp;
                        ++index1;
                        ++index2;
                    }
                }
            }
            //   clear my palette, if I had one
            if (tga_palette != NULL)
            {
                STBI_FREE(tga_palette);
            }
        }

        // swap RGB - if the source data was RGB16, it already is in the right order
        if (tga_comp >= 3 && !tga_rgb16)
        {
            unsigned char* tga_pixel = tga_data;
            for (i = 0; i < tga_width * tga_height; ++i)
            {
                unsigned char temp = tga_pixel[0];
                tga_pixel[0] = tga_pixel[2];
                tga_pixel[2] = temp;
                tga_pixel += tga_comp;
            }
        }

        // convert to target component count
        if (req_comp && req_comp != tga_comp)
            tga_data = stbi__convert_format(tga_data, tga_comp, req_comp, tga_width, tga_height);

        //   the things I do to get rid of an error message, and yet keep
        //   Microsoft's C compilers happy... [8^(
        tga_palette_start = tga_palette_len = tga_palette_bits =
            tga_x_origin = tga_y_origin = 0;
        STBI_NOTUSED(tga_palette_start);
        //   OK, done
        return tga_data;
    }
#endif

    // *************************************************************************************************
    // Photoshop PSD loader -- PD by Thatcher Ulrich, integration by Nicolas Schulz, tweaked by STB

#ifndef STBI_NO_PSD
    static int stbi__psd_test(stbi__context* s)
    {
        int r = (stbi__get32be(s) == 0x38425053);
        stbi__rewind(s);
        return r;
    }

    static int stbi__psd_decode_rle(stbi__context* s, stbi_uc* p, int pixelCount)
    {
        int count, nleft, len;

        count = 0;
        while ((nleft = pixelCount - count) > 0) {
            len = stbi__get8(s);
            if (len == 128) {
                // No-op.
            }
            else if (len < 128) {
                // Copy next len+1 bytes literally.
                len++;
                if (len > nleft) return 0; // corrupt data
                count += len;
                while (len) {
                    *p = stbi__get8(s);
                    p += 4;
                    len--;
                }
            }
            else if (len > 128) {
                stbi_uc   val;
                // Next -len+1 bytes in the dest are replicated from next source byte.
                // (Interpret len as a negative 8-bit int.)
                len = 257 - len;
                if (len > nleft) return 0; // corrupt data
                val = stbi__get8(s);
                count += len;
                while (len) {
                    *p = val;
                    p += 4;
                    len--;
                }
            }
        }

        return 1;
    }

    static void* stbi__psd_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri, int bpc)
    {
        int pixelCount;
        int channelCount, compression;
        int channel, i;
        int bitdepth;
        int w, h;
        stbi_uc* out;
        STBI_NOTUSED(ri);

        // Check identifier
        if (stbi__get32be(s) != 0x38425053)   // "8BPS"
            return stbi__errpuc("not PSD", "Corrupt PSD image");

        // Check file type version.
        if (stbi__get16be(s) != 1)
            return stbi__errpuc("wrong version", "Unsupported version of PSD image");

        // Skip 6 reserved bytes.
        stbi__skip(s, 6);

        // Read the number of channels (R, G, B, A, etc).
        channelCount = stbi__get16be(s);
        if (channelCount < 0 || channelCount > 16)
            return stbi__errpuc("wrong channel count", "Unsupported number of channels in PSD image");

        // Read the rows and columns of the image.
        h = stbi__get32be(s);
        w = stbi__get32be(s);

        if (h > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");
        if (w > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");

        // Make sure the depth is 8 bits.
        bitdepth = stbi__get16be(s);
        if (bitdepth != 8 && bitdepth != 16)
            return stbi__errpuc("unsupported bit depth", "PSD bit depth is not 8 or 16 bit");

        // Make sure the color mode is RGB.
        // Valid options are:
        //   0: Bitmap
        //   1: Grayscale
        //   2: Indexed color
        //   3: RGB color
        //   4: CMYK color
        //   7: Multichannel
        //   8: Duotone
        //   9: Lab color
        if (stbi__get16be(s) != 3)
            return stbi__errpuc("wrong color format", "PSD is not in RGB color format");

        // Skip the Mode Data.  (It's the palette for indexed color; other info for other modes.)
        stbi__skip(s, stbi__get32be(s));

        // Skip the image resources.  (resolution, pen tool paths, etc)
        stbi__skip(s, stbi__get32be(s));

        // Skip the reserved data.
        stbi__skip(s, stbi__get32be(s));

        // Find out if the data is compressed.
        // Known values:
        //   0: no compression
        //   1: RLE compressed
        compression = stbi__get16be(s);
        if (compression > 1)
            return stbi__errpuc("bad compression", "PSD has an unknown compression format");

        // Check size
        if (!stbi__mad3sizes_valid(4, w, h, 0))
            return stbi__errpuc("too large", "Corrupt PSD");

        // Create the destination image.

        if (!compression && bitdepth == 16 && bpc == 16) {
            out = (stbi_uc*)stbi__malloc_mad3(8, w, h, 0);
            ri->bits_per_channel = 16;
        }
        else
            out = (stbi_uc*)stbi__malloc(4 * w * h);

        if (!out) return stbi__errpuc("outofmem", "Out of memory");
        pixelCount = w * h;

        // Initialize the data to zero.
        //memset( out, 0, pixelCount * 4 );

        // Finally, the image data.
        if (compression) {
            // RLE as used by .PSD and .TIFF
            // Loop until you get the number of unpacked bytes you are expecting:
            //     Read the next source byte into n.
            //     If n is between 0 and 127 inclusive, copy the next n+1 bytes literally.
            //     Else if n is between -127 and -1 inclusive, copy the next byte -n+1 times.
            //     Else if n is 128, noop.
            // Endloop

            // The RLE-compressed data is preceded by a 2-byte data count for each row in the data,
            // which we're going to just skip.
            stbi__skip(s, h * channelCount * 2);

            // Read the RLE data by channel.
            for (channel = 0; channel < 4; channel++) {
                stbi_uc* p;

                p = out + channel;
                if (channel >= channelCount) {
                    // Fill this channel with default data.
                    for (i = 0; i < pixelCount; i++, p += 4)
                        *p = (channel == 3 ? 255 : 0);
                }
                else {
                    // Read the RLE data.
                    if (!stbi__psd_decode_rle(s, p, pixelCount)) {
                        STBI_FREE(out);
                        return stbi__errpuc("corrupt", "bad RLE data");
                    }
                }
            }

        }
        else {
            // We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
            // where each channel consists of an 8-bit (or 16-bit) value for each pixel in the image.

            // Read the data by channel.
            for (channel = 0; channel < 4; channel++) {
                if (channel >= channelCount) {
                    // Fill this channel with default data.
                    if (bitdepth == 16 && bpc == 16) {
                        stbi__uint16* q = ((stbi__uint16*)out) + channel;
                        stbi__uint16 val = channel == 3 ? 65535 : 0;
                        for (i = 0; i < pixelCount; i++, q += 4)
                            *q = val;
                    }
                    else {
                        stbi_uc* p = out + channel;
                        stbi_uc val = channel == 3 ? 255 : 0;
                        for (i = 0; i < pixelCount; i++, p += 4)
                            *p = val;
                    }
                }
                else {
                    if (ri->bits_per_channel == 16) {    // output bpc
                        stbi__uint16* q = ((stbi__uint16*)out) + channel;
                        for (i = 0; i < pixelCount; i++, q += 4)
                            *q = (stbi__uint16)stbi__get16be(s);
                    }
                    else {
                        stbi_uc* p = out + channel;
                        if (bitdepth == 16) {  // input bpc
                            for (i = 0; i < pixelCount; i++, p += 4)
                                *p = (stbi_uc)(stbi__get16be(s) >> 8);
                        }
                        else {
                            for (i = 0; i < pixelCount; i++, p += 4)
                                *p = stbi__get8(s);
                        }
                    }
                }
            }
        }

        // remove weird white matte from PSD
        if (channelCount >= 4) {
            if (ri->bits_per_channel == 16) {
                for (i = 0; i < w * h; ++i) {
                    stbi__uint16* pixel = (stbi__uint16*)out + 4 * i;
                    if (pixel[3] != 0 && pixel[3] != 65535) {
                        float a = pixel[3] / 65535.0f;
                        float ra = 1.0f / a;
                        float inv_a = 65535.0f * (1 - ra);
                        pixel[0] = (stbi__uint16)(pixel[0] * ra + inv_a);
                        pixel[1] = (stbi__uint16)(pixel[1] * ra + inv_a);
                        pixel[2] = (stbi__uint16)(pixel[2] * ra + inv_a);
                    }
                }
            }
            else {
                for (i = 0; i < w * h; ++i) {
                    unsigned char* pixel = out + 4 * i;
                    if (pixel[3] != 0 && pixel[3] != 255) {
                        float a = pixel[3] / 255.0f;
                        float ra = 1.0f / a;
                        float inv_a = 255.0f * (1 - ra);
                        pixel[0] = (unsigned char)(pixel[0] * ra + inv_a);
                        pixel[1] = (unsigned char)(pixel[1] * ra + inv_a);
                        pixel[2] = (unsigned char)(pixel[2] * ra + inv_a);
                    }
                }
            }
        }

        // convert to desired output format
        if (req_comp && req_comp != 4) {
            if (ri->bits_per_channel == 16)
                out = (stbi_uc*)stbi__convert_format16((stbi__uint16*)out, 4, req_comp, w, h);
            else
                out = stbi__convert_format(out, 4, req_comp, w, h);
            if (out == NULL) return out; // stbi__convert_format frees input on failure
        }

        if (comp) *comp = 4;
        *y = h;
        *x = w;

        return out;
    }
#endif

    // *************************************************************************************************
    // Softimage PIC loader
    // by Tom Seddon
    //
    // See http://softimage.wiki.softimage.com/index.php/INFO:_PIC_file_format
    // See http://ozviz.wasp.uwa.edu.au/~pbourke/dataformats/softimagepic/

#ifndef STBI_NO_PIC
    static int stbi__pic_is4(stbi__context* s, const char* str)
    {
        int i;
        for (i = 0; i < 4; ++i)
            if (stbi__get8(s) != (stbi_uc)str[i])
                return 0;

        return 1;
    }

    static int stbi__pic_test_core(stbi__context* s)
    {
        int i;

        if (!stbi__pic_is4(s, "\x53\x80\xF6\x34"))
            return 0;

        for (i = 0; i < 84; ++i)
            stbi__get8(s);

        if (!stbi__pic_is4(s, "PICT"))
            return 0;

        return 1;
    }

    typedef struct
    {
        stbi_uc size, type, channel;
    } stbi__pic_packet;

    static stbi_uc* stbi__readval(stbi__context* s, int channel, stbi_uc* dest)
    {
        int mask = 0x80, i;

        for (i = 0; i < 4; ++i, mask >>= 1) {
            if (channel & mask) {
                if (stbi__at_eof(s)) return stbi__errpuc("bad file", "PIC file too short");
                dest[i] = stbi__get8(s);
            }
        }

        return dest;
    }

    static void stbi__copyval(int channel, stbi_uc* dest, const stbi_uc* src)
    {
        int mask = 0x80, i;

        for (i = 0; i < 4; ++i, mask >>= 1)
            if (channel & mask)
                dest[i] = src[i];
    }

    static stbi_uc* stbi__pic_load_core(stbi__context* s, int width, int height, int* comp, stbi_uc* result)
    {
        int act_comp = 0, num_packets = 0, y, chained;
        stbi__pic_packet packets[10];

        // this will (should...) cater for even some bizarre stuff like having data
         // for the same channel in multiple packets.
        do {
            stbi__pic_packet* packet;

            if (num_packets == sizeof(packets) / sizeof(packets[0]))
                return stbi__errpuc("bad format", "too many packets");

            packet = &packets[num_packets++];

            chained = stbi__get8(s);
            packet->size = stbi__get8(s);
            packet->type = stbi__get8(s);
            packet->channel = stbi__get8(s);

            act_comp |= packet->channel;

            if (stbi__at_eof(s))          return stbi__errpuc("bad file", "file too short (reading packets)");
            if (packet->size != 8)  return stbi__errpuc("bad format", "packet isn't 8bpp");
        } while (chained);

        *comp = (act_comp & 0x10 ? 4 : 3); // has alpha channel?

        for (y = 0; y < height; ++y) {
            int packet_idx;

            for (packet_idx = 0; packet_idx < num_packets; ++packet_idx) {
                stbi__pic_packet* packet = &packets[packet_idx];
                stbi_uc* dest = result + y * width * 4;

                switch (packet->type) {
                default:
                    return stbi__errpuc("bad format", "packet has bad compression type");

                case 0: {//uncompressed
                    int x;

                    for (x = 0; x < width; ++x, dest += 4)
                        if (!stbi__readval(s, packet->channel, dest))
                            return 0;
                    break;
                }

                case 1://Pure RLE
                {
                    int left = width, i;

                    while (left > 0) {
                        stbi_uc count, value[4];

                        count = stbi__get8(s);
                        if (stbi__at_eof(s))   return stbi__errpuc("bad file", "file too short (pure read count)");

                        if (count > left)
                            count = (stbi_uc)left;

                        if (!stbi__readval(s, packet->channel, value))  return 0;

                        for (i = 0; i < count; ++i, dest += 4)
                            stbi__copyval(packet->channel, dest, value);
                        left -= count;
                    }
                }
                break;

                case 2: {//Mixed RLE
                    int left = width;
                    while (left > 0) {
                        int count = stbi__get8(s), i;
                        if (stbi__at_eof(s))  return stbi__errpuc("bad file", "file too short (mixed read count)");

                        if (count >= 128) { // Repeated
                            stbi_uc value[4];

                            if (count == 128)
                                count = stbi__get16be(s);
                            else
                                count -= 127;
                            if (count > left)
                                return stbi__errpuc("bad file", "scanline overrun");

                            if (!stbi__readval(s, packet->channel, value))
                                return 0;

                            for (i = 0; i < count; ++i, dest += 4)
                                stbi__copyval(packet->channel, dest, value);
                        }
                        else { // Raw
                            ++count;
                            if (count > left) return stbi__errpuc("bad file", "scanline overrun");

                            for (i = 0; i < count; ++i, dest += 4)
                                if (!stbi__readval(s, packet->channel, dest))
                                    return 0;
                        }
                        left -= count;
                    }
                    break;
                }
                }
            }
        }

        return result;
    }

    static void* stbi__pic_load(stbi__context* s, int* px, int* py, int* comp, int req_comp, stbi__result_info* ri)
    {
        stbi_uc* result;
        int i, x, y, internal_comp;
        STBI_NOTUSED(ri);

        if (!comp) comp = &internal_comp;

        for (i = 0; i < 92; ++i)
            stbi__get8(s);

        x = stbi__get16be(s);
        y = stbi__get16be(s);

        if (y > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");
        if (x > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");

        if (stbi__at_eof(s))  return stbi__errpuc("bad file", "file too short (pic header)");
        if (!stbi__mad3sizes_valid(x, y, 4, 0)) return stbi__errpuc("too large", "PIC image too large to decode");

        stbi__get32be(s); //skip `ratio'
        stbi__get16be(s); //skip `fields'
        stbi__get16be(s); //skip `pad'

        // intermediate buffer is RGBA
        result = (stbi_uc*)stbi__malloc_mad3(x, y, 4, 0);
        if (!result) return stbi__errpuc("outofmem", "Out of memory");
        memset(result, 0xff, x * y * 4);

        if (!stbi__pic_load_core(s, x, y, comp, result)) {
            STBI_FREE(result);
            result = 0;
        }
        *px = x;
        *py = y;
        if (req_comp == 0) req_comp = *comp;
        result = stbi__convert_format(result, 4, req_comp, x, y);

        return result;
    }

    static int stbi__pic_test(stbi__context* s)
    {
        int r = stbi__pic_test_core(s);
        stbi__rewind(s);
        return r;
    }
#endif

    // *************************************************************************************************
    // GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb

#ifndef STBI_NO_GIF
    typedef struct
    {
        stbi__int16 prefix;
        stbi_uc first;
        stbi_uc suffix;
    } stbi__gif_lzw;

    typedef struct
    {
        int w, h;
        stbi_uc* out;                 // output buffer (always 4 components)
        stbi_uc* background;          // The current "background" as far as a gif is concerned
        stbi_uc* history;
        int flags, bgindex, ratio, transparent, eflags;
        stbi_uc  pal[256][4];
        stbi_uc lpal[256][4];
        stbi__gif_lzw codes[8192];
        stbi_uc* color_table;
        int parse, step;
        int lflags;
        int start_x, start_y;
        int max_x, max_y;
        int cur_x, cur_y;
        int line_size;
        int delay;
    } stbi__gif;

    static int stbi__gif_test_raw(stbi__context* s)
    {
        int sz;
        if (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8') return 0;
        sz = stbi__get8(s);
        if (sz != '9' && sz != '7') return 0;
        if (stbi__get8(s) != 'a') return 0;
        return 1;
    }

    static int stbi__gif_test(stbi__context* s)
    {
        int r = stbi__gif_test_raw(s);
        stbi__rewind(s);
        return r;
    }

    static void stbi__gif_parse_colortable(stbi__context* s, stbi_uc pal[256][4], int num_entries, int transp)
    {
        int i;
        for (i = 0; i < num_entries; ++i) {
            pal[i][2] = stbi__get8(s);
            pal[i][1] = stbi__get8(s);
            pal[i][0] = stbi__get8(s);
            pal[i][3] = transp == i ? 0 : 255;
        }
    }

    static int stbi__gif_header(stbi__context* s, stbi__gif* g, int* comp, int is_info)
    {
        stbi_uc version;
        if (stbi__get8(s) != 'G' || stbi__get8(s) != 'I' || stbi__get8(s) != 'F' || stbi__get8(s) != '8')
            return stbi__err("not GIF", "Corrupt GIF");

        version = stbi__get8(s);
        if (version != '7' && version != '9')    return stbi__err("not GIF", "Corrupt GIF");
        if (stbi__get8(s) != 'a')                return stbi__err("not GIF", "Corrupt GIF");

        stbi__g_failure_reason = "";
        g->w = stbi__get16le(s);
        g->h = stbi__get16le(s);
        g->flags = stbi__get8(s);
        g->bgindex = stbi__get8(s);
        g->ratio = stbi__get8(s);
        g->transparent = -1;

        if (g->w > STBI_MAX_DIMENSIONS) return stbi__err("too large", "Very large image (corrupt?)");
        if (g->h > STBI_MAX_DIMENSIONS) return stbi__err("too large", "Very large image (corrupt?)");

        if (comp != 0) *comp = 4;  // can't actually tell whether it's 3 or 4 until we parse the comments

        if (is_info) return 1;

        if (g->flags & 0x80)
            stbi__gif_parse_colortable(s, g->pal, 2 << (g->flags & 7), -1);

        return 1;
    }

    static int stbi__gif_info_raw(stbi__context* s, int* x, int* y, int* comp)
    {
        stbi__gif* g = (stbi__gif*)stbi__malloc(sizeof(stbi__gif));
        if (!g) return stbi__err("outofmem", "Out of memory");
        if (!stbi__gif_header(s, g, comp, 1)) {
            STBI_FREE(g);
            stbi__rewind(s);
            return 0;
        }
        if (x) *x = g->w;
        if (y) *y = g->h;
        STBI_FREE(g);
        return 1;
    }

    static void stbi__out_gif_code(stbi__gif* g, stbi__uint16 code)
    {
        stbi_uc* p, * c;
        int idx;

        // recurse to decode the prefixes, since the linked-list is backwards,
        // and working backwards through an interleaved image would be nasty
        if (g->codes[code].prefix >= 0)
            stbi__out_gif_code(g, g->codes[code].prefix);

        if (g->cur_y >= g->max_y) return;

        idx = g->cur_x + g->cur_y;
        p = &g->out[idx];
        g->history[idx / 4] = 1;

        c = &g->color_table[g->codes[code].suffix * 4];
        if (c[3] > 128) { // don't render transparent pixels;
            p[0] = c[2];
            p[1] = c[1];
            p[2] = c[0];
            p[3] = c[3];
        }
        g->cur_x += 4;

        if (g->cur_x >= g->max_x) {
            g->cur_x = g->start_x;
            g->cur_y += g->step;

            while (g->cur_y >= g->max_y && g->parse > 0) {
                g->step = (1 << g->parse) * g->line_size;
                g->cur_y = g->start_y + (g->step >> 1);
                --g->parse;
            }
        }
    }

    static stbi_uc* stbi__process_gif_raster(stbi__context* s, stbi__gif* g)
    {
        stbi_uc lzw_cs;
        stbi__int32 len, init_code;
        stbi__uint32 first;
        stbi__int32 codesize, codemask, avail, oldcode, bits, valid_bits, clear;
        stbi__gif_lzw* p;

        lzw_cs = stbi__get8(s);
        if (lzw_cs > 12) return NULL;
        clear = 1 << lzw_cs;
        first = 1;
        codesize = lzw_cs + 1;
        codemask = (1 << codesize) - 1;
        bits = 0;
        valid_bits = 0;
        for (init_code = 0; init_code < clear; init_code++) {
            g->codes[init_code].prefix = -1;
            g->codes[init_code].first = (stbi_uc)init_code;
            g->codes[init_code].suffix = (stbi_uc)init_code;
        }

        // support no starting clear code
        avail = clear + 2;
        oldcode = -1;

        len = 0;
        for (;;) {
            if (valid_bits < codesize) {
                if (len == 0) {
                    len = stbi__get8(s); // start new block
                    if (len == 0)
                        return g->out;
                }
                --len;
                bits |= (stbi__int32)stbi__get8(s) << valid_bits;
                valid_bits += 8;
            }
            else {
                stbi__int32 code = bits & codemask;
                bits >>= codesize;
                valid_bits -= codesize;
                // @OPTIMIZE: is there some way we can accelerate the non-clear path?
                if (code == clear) {  // clear code
                    codesize = lzw_cs + 1;
                    codemask = (1 << codesize) - 1;
                    avail = clear + 2;
                    oldcode = -1;
                    first = 0;
                }
                else if (code == clear + 1) { // end of stream code
                    stbi__skip(s, len);
                    while ((len = stbi__get8(s)) > 0)
                        stbi__skip(s, len);
                    return g->out;
                }
                else if (code <= avail) {
                    if (first) {
                        return stbi__errpuc("no clear code", "Corrupt GIF");
                    }

                    if (oldcode >= 0) {
                        p = &g->codes[avail++];
                        if (avail > 8192) {
                            return stbi__errpuc("too many codes", "Corrupt GIF");
                        }

                        p->prefix = (stbi__int16)oldcode;
                        p->first = g->codes[oldcode].first;
                        p->suffix = (code == avail) ? p->first : g->codes[code].first;
                    }
                    else if (code == avail)
                        return stbi__errpuc("illegal code in raster", "Corrupt GIF");

                    stbi__out_gif_code(g, (stbi__uint16)code);

                    if ((avail & codemask) == 0 && avail <= 0x0FFF) {
                        codesize++;
                        codemask = (1 << codesize) - 1;
                    }

                    oldcode = code;
                }
                else {
                    return stbi__errpuc("illegal code in raster", "Corrupt GIF");
                }
            }
        }
    }

    // this function is designed to support animated gifs, although stb_image doesn't support it
    // two back is the image from two frames ago, used for a very specific disposal format
    static stbi_uc* stbi__gif_load_next(stbi__context* s, stbi__gif* g, int* comp, int req_comp, stbi_uc* two_back)
    {
        int dispose;
        int first_frame;
        int pi;
        int pcount;
        STBI_NOTUSED(req_comp);

        // on first frame, any non-written pixels get the background colour (non-transparent)
        first_frame = 0;
        if (g->out == 0) {
            if (!stbi__gif_header(s, g, comp, 0)) return 0; // stbi__g_failure_reason set by stbi__gif_header
            if (!stbi__mad3sizes_valid(4, g->w, g->h, 0))
                return stbi__errpuc("too large", "GIF image is too large");
            pcount = g->w * g->h;
            g->out = (stbi_uc*)stbi__malloc(4 * pcount);
            g->background = (stbi_uc*)stbi__malloc(4 * pcount);
            g->history = (stbi_uc*)stbi__malloc(pcount);
            if (!g->out || !g->background || !g->history)
                return stbi__errpuc("outofmem", "Out of memory");

            // image is treated as "transparent" at the start - ie, nothing overwrites the current background;
            // background colour is only used for pixels that are not rendered first frame, after that "background"
            // color refers to the color that was there the previous frame.
            memset(g->out, 0x00, 4 * pcount);
            memset(g->background, 0x00, 4 * pcount); // state of the background (starts transparent)
            memset(g->history, 0x00, pcount);        // pixels that were affected previous frame
            first_frame = 1;
        }
        else {
            // second frame - how do we dispose of the previous one?
            dispose = (g->eflags & 0x1C) >> 2;
            pcount = g->w * g->h;

            if ((dispose == 3) && (two_back == 0)) {
                dispose = 2; // if I don't have an image to revert back to, default to the old background
            }

            if (dispose == 3) { // use previous graphic
                for (pi = 0; pi < pcount; ++pi) {
                    if (g->history[pi]) {
                        memcpy(&g->out[pi * 4], &two_back[pi * 4], 4);
                    }
                }
            }
            else if (dispose == 2) {
                // restore what was changed last frame to background before that frame;
                for (pi = 0; pi < pcount; ++pi) {
                    if (g->history[pi]) {
                        memcpy(&g->out[pi * 4], &g->background[pi * 4], 4);
                    }
                }
            }
            else {
                // This is a non-disposal case eithe way, so just
                // leave the pixels as is, and they will become the new background
                // 1: do not dispose
                // 0:  not specified.
            }

            // background is what out is after the undoing of the previou frame;
            memcpy(g->background, g->out, 4 * g->w * g->h);
        }

        // clear my history;
        memset(g->history, 0x00, g->w * g->h);        // pixels that were affected previous frame

        for (;;) {
            int tag = stbi__get8(s);
            switch (tag) {
            case 0x2C: /* Image Descriptor */
            {
                stbi__int32 x, y, w, h;
                stbi_uc* o;

                x = stbi__get16le(s);
                y = stbi__get16le(s);
                w = stbi__get16le(s);
                h = stbi__get16le(s);
                if (((x + w) > (g->w)) || ((y + h) > (g->h)))
                    return stbi__errpuc("bad Image Descriptor", "Corrupt GIF");

                g->line_size = g->w * 4;
                g->start_x = x * 4;
                g->start_y = y * g->line_size;
                g->max_x = g->start_x + w * 4;
                g->max_y = g->start_y + h * g->line_size;
                g->cur_x = g->start_x;
                g->cur_y = g->start_y;

                // if the width of the specified rectangle is 0, that means
                // we may not see *any* pixels or the image is malformed;
                // to make sure this is caught, move the current y down to
                // max_y (which is what out_gif_code checks).
                if (w == 0)
                    g->cur_y = g->max_y;

                g->lflags = stbi__get8(s);

                if (g->lflags & 0x40) {
                    g->step = 8 * g->line_size; // first interlaced spacing
                    g->parse = 3;
                }
                else {
                    g->step = g->line_size;
                    g->parse = 0;
                }

                if (g->lflags & 0x80) {
                    stbi__gif_parse_colortable(s, g->lpal, 2 << (g->lflags & 7), g->eflags & 0x01 ? g->transparent : -1);
                    g->color_table = (stbi_uc*)g->lpal;
                }
                else if (g->flags & 0x80) {
                    g->color_table = (stbi_uc*)g->pal;
                }
                else
                    return stbi__errpuc("missing color table", "Corrupt GIF");

                o = stbi__process_gif_raster(s, g);
                if (!o) return NULL;

                // if this was the first frame,
                pcount = g->w * g->h;
                if (first_frame && (g->bgindex > 0)) {
                    // if first frame, any pixel not drawn to gets the background color
                    for (pi = 0; pi < pcount; ++pi) {
                        if (g->history[pi] == 0) {
                            g->pal[g->bgindex][3] = 255; // just in case it was made transparent, undo that; It will be reset next frame if need be;
                            memcpy(&g->out[pi * 4], &g->pal[g->bgindex], 4);
                        }
                    }
                }

                return o;
            }

            case 0x21: // Comment Extension.
            {
                int len;
                int ext = stbi__get8(s);
                if (ext == 0xF9) { // Graphic Control Extension.
                    len = stbi__get8(s);
                    if (len == 4) {
                        g->eflags = stbi__get8(s);
                        g->delay = 10 * stbi__get16le(s); // delay - 1/100th of a second, saving as 1/1000ths.

                        // unset old transparent
                        if (g->transparent >= 0) {
                            g->pal[g->transparent][3] = 255;
                        }
                        if (g->eflags & 0x01) {
                            g->transparent = stbi__get8(s);
                            if (g->transparent >= 0) {
                                g->pal[g->transparent][3] = 0;
                            }
                        }
                        else {
                            // don't need transparent
                            stbi__skip(s, 1);
                            g->transparent = -1;
                        }
                    }
                    else {
                        stbi__skip(s, len);
                        break;
                    }
                }
                while ((len = stbi__get8(s)) != 0) {
                    stbi__skip(s, len);
                }
                break;
            }

            case 0x3B: // gif stream termination code
                return (stbi_uc*)s; // using '1' causes warning on some compilers

            default:
                return stbi__errpuc("unknown code", "Corrupt GIF");
            }
        }
    }

    static void* stbi__load_gif_main_outofmem(stbi__gif* g, stbi_uc* out, int** delays)
    {
        STBI_FREE(g->out);
        STBI_FREE(g->history);
        STBI_FREE(g->background);

        if (out) STBI_FREE(out);
        if (delays && *delays) STBI_FREE(*delays);
        return stbi__errpuc("outofmem", "Out of memory");
    }

    static void* stbi__load_gif_main(stbi__context* s, int** delays, int* x, int* y, int* z, int* comp, int req_comp)
    {
        if (stbi__gif_test(s)) {
            int layers = 0;
            stbi_uc* u = 0;
            stbi_uc* out = 0;
            stbi_uc* two_back = 0;
            stbi__gif g;
            int stride;
            int out_size = 0;
            int delays_size = 0;

            STBI_NOTUSED(out_size);
            STBI_NOTUSED(delays_size);

            memset(&g, 0, sizeof(g));
            if (delays) {
                *delays = 0;
            }

            do {
                u = stbi__gif_load_next(s, &g, comp, req_comp, two_back);
                if (u == (stbi_uc*)s) u = 0;  // end of animated gif marker

                if (u) {
                    *x = g.w;
                    *y = g.h;
                    ++layers;
                    stride = g.w * g.h * 4;

                    if (out) {
                        void* tmp = (stbi_uc*)STBI_REALLOC_SIZED(out, out_size, layers * stride);
                        if (!tmp)
                            return stbi__load_gif_main_outofmem(&g, out, delays);
                        else {
                            out = (stbi_uc*)tmp;
                            out_size = layers * stride;
                        }

                        if (delays) {
                            int* new_delays = (int*)STBI_REALLOC_SIZED(*delays, delays_size, sizeof(int) * layers);
                            if (!new_delays)
                                return stbi__load_gif_main_outofmem(&g, out, delays);
                            *delays = new_delays;
                            delays_size = layers * sizeof(int);
                        }
                    }
                    else {
                        out = (stbi_uc*)stbi__malloc(layers * stride);
                        if (!out)
                            return stbi__load_gif_main_outofmem(&g, out, delays);
                        out_size = layers * stride;
                        if (delays) {
                            *delays = (int*)stbi__malloc(layers * sizeof(int));
                            if (!*delays)
                                return stbi__load_gif_main_outofmem(&g, out, delays);
                            delays_size = layers * sizeof(int);
                        }
                    }
                    memcpy(out + ((layers - 1) * stride), u, stride);
                    if (layers >= 2) {
                        two_back = out - 2 * stride;
                    }

                    if (delays) {
                        (*delays)[layers - 1U] = g.delay;
                    }
                }
            } while (u != 0);

            // free temp buffer;
            STBI_FREE(g.out);
            STBI_FREE(g.history);
            STBI_FREE(g.background);

            // do the final conversion after loading everything;
            if (req_comp && req_comp != 4)
                out = stbi__convert_format(out, 4, req_comp, layers * g.w, g.h);

            *z = layers;
            return out;
        }
        else {
            return stbi__errpuc("not GIF", "Image was not as a gif type.");
        }
    }

    static void* stbi__gif_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri)
    {
        stbi_uc* u = 0;
        stbi__gif g;
        memset(&g, 0, sizeof(g));
        STBI_NOTUSED(ri);

        u = stbi__gif_load_next(s, &g, comp, req_comp, 0);
        if (u == (stbi_uc*)s) u = 0;  // end of animated gif marker
        if (u) {
            *x = g.w;
            *y = g.h;

            // moved conversion to after successful load so that the same
            // can be done for multiple frames.
            if (req_comp && req_comp != 4)
                u = stbi__convert_format(u, 4, req_comp, g.w, g.h);
        }
        else if (g.out) {
            // if there was an error and we allocated an image buffer, free it!
            STBI_FREE(g.out);
        }

        // free buffers needed for multiple frame loading;
        STBI_FREE(g.history);
        STBI_FREE(g.background);

        return u;
    }

    static int stbi__gif_info(stbi__context* s, int* x, int* y, int* comp)
    {
        return stbi__gif_info_raw(s, x, y, comp);
    }
#endif

    // *************************************************************************************************
    // Radiance RGBE HDR loader
    // originally by Nicolas Schulz
#ifndef STBI_NO_HDR
    static int stbi__hdr_test_core(stbi__context* s, const char* signature)
    {
        int i;
        for (i = 0; signature[i]; ++i)
            if (stbi__get8(s) != signature[i])
                return 0;
        stbi__rewind(s);
        return 1;
    }

    static int stbi__hdr_test(stbi__context* s)
    {
        int r = stbi__hdr_test_core(s, "#?RADIANCE\n");
        stbi__rewind(s);
        if (!r) {
            r = stbi__hdr_test_core(s, "#?RGBE\n");
            stbi__rewind(s);
        }
        return r;
    }

#define STBI__HDR_BUFLEN  1024
    static char* stbi__hdr_gettoken(stbi__context* z, char* buffer)
    {
        int len = 0;
        char c = '\0';

        c = (char)stbi__get8(z);

        while (!stbi__at_eof(z) && c != '\n') {
            buffer[len++] = c;
            if (len == STBI__HDR_BUFLEN - 1) {
                // flush to end of line
                while (!stbi__at_eof(z) && stbi__get8(z) != '\n')
                    ;
                break;
            }
            c = (char)stbi__get8(z);
        }

        buffer[len] = 0;
        return buffer;
    }

    static void stbi__hdr_convert(float* output, stbi_uc* input, int req_comp)
    {
        if (input[3] != 0) {
            float f1;
            // Exponent
            f1 = (float)ldexp(1.0f, input[3] - (int)(128 + 8));
            if (req_comp <= 2)
                output[0] = (input[0] + input[1] + input[2]) * f1 / 3;
            else {
                output[0] = input[0] * f1;
                output[1] = input[1] * f1;
                output[2] = input[2] * f1;
            }
            if (req_comp == 2) output[1] = 1;
            if (req_comp == 4) output[3] = 1;
        }
        else {
            switch (req_comp) {
            case 4: output[3] = 1; /* fallthrough */
            case 3: output[0] = output[1] = output[2] = 0;
                break;
            case 2: output[1] = 1; /* fallthrough */
            case 1: output[0] = 0;
                break;
            }
        }
    }

    static float* stbi__hdr_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri)
    {
        char buffer[STBI__HDR_BUFLEN];
        char* token;
        int valid = 0;
        int width, height;
        stbi_uc* scanline;
        float* hdr_data;
        int len;
        unsigned char count, value;
        int i, j, k, c1, c2, z;
        const char* headerToken;
        STBI_NOTUSED(ri);

        // Check identifier
        headerToken = stbi__hdr_gettoken(s, buffer);
        if (strcmp(headerToken, "#?RADIANCE") != 0 && strcmp(headerToken, "#?RGBE") != 0)
            return stbi__errpf("not HDR", "Corrupt HDR image");

        // Parse header
        for (;;) {
            token = stbi__hdr_gettoken(s, buffer);
            if (token[0] == 0) break;
            if (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
        }

        if (!valid)    return stbi__errpf("unsupported format", "Unsupported HDR format");

        // Parse width and height
        // can't use sscanf() if we're not using stdio!
        token = stbi__hdr_gettoken(s, buffer);
        if (strncmp(token, "-Y ", 3))  return stbi__errpf("unsupported data layout", "Unsupported HDR format");
        token += 3;
        height = (int)strtol(token, &token, 10);
        while (*token == ' ') ++token;
        if (strncmp(token, "+X ", 3))  return stbi__errpf("unsupported data layout", "Unsupported HDR format");
        token += 3;
        width = (int)strtol(token, NULL, 10);

        if (height > STBI_MAX_DIMENSIONS) return stbi__errpf("too large", "Very large image (corrupt?)");
        if (width > STBI_MAX_DIMENSIONS) return stbi__errpf("too large", "Very large image (corrupt?)");

        *x = width;
        *y = height;

        if (comp) *comp = 3;
        if (req_comp == 0) req_comp = 3;

        if (!stbi__mad4sizes_valid(width, height, req_comp, sizeof(float), 0))
            return stbi__errpf("too large", "HDR image is too large");

        // Read data
        hdr_data = (float*)stbi__malloc_mad4(width, height, req_comp, sizeof(float), 0);
        if (!hdr_data)
            return stbi__errpf("outofmem", "Out of memory");

        // Load image data
        // image data is stored as some number of sca
        if (width < 8 || width >= 32768) {
            // Read flat data
            for (j = 0; j < height; ++j) {
                for (i = 0; i < width; ++i) {
                    stbi_uc rgbe[4];
                main_decode_loop:
                    stbi__getn(s, rgbe, 4);
                    stbi__hdr_convert(hdr_data + j * width * req_comp + i * req_comp, rgbe, req_comp);
                }
            }
        }
        else {
            // Read RLE-encoded data
            scanline = NULL;

            for (j = 0; j < height; ++j) {
                c1 = stbi__get8(s);
                c2 = stbi__get8(s);
                len = stbi__get8(s);
                if (c1 != 2 || c2 != 2 || (len & 0x80)) {
                    // not run-length encoded, so we have to actually use THIS data as a decoded
                    // pixel (note this can't be a valid pixel--one of RGB must be >= 128)
                    stbi_uc rgbe[4];
                    rgbe[0] = (stbi_uc)c1;
                    rgbe[1] = (stbi_uc)c2;
                    rgbe[2] = (stbi_uc)len;
                    rgbe[3] = (stbi_uc)stbi__get8(s);
                    stbi__hdr_convert(hdr_data, rgbe, req_comp);
                    i = 1;
                    j = 0;
                    STBI_FREE(scanline);
                    goto main_decode_loop; // yes, this makes no sense
                }
                len <<= 8;
                len |= stbi__get8(s);
                if (len != width) { STBI_FREE(hdr_data); STBI_FREE(scanline); return stbi__errpf("invalid decoded scanline length", "corrupt HDR"); }
                if (scanline == NULL) {
                    scanline = (stbi_uc*)stbi__malloc_mad2(width, 4, 0);
                    if (!scanline) {
                        STBI_FREE(hdr_data);
                        return stbi__errpf("outofmem", "Out of memory");
                    }
                }

                for (k = 0; k < 4; ++k) {
                    int nleft;
                    i = 0;
                    while ((nleft = width - i) > 0) {
                        count = stbi__get8(s);
                        if (count > 128) {
                            // Run
                            value = stbi__get8(s);
                            count -= 128;
                            if (count > nleft) { STBI_FREE(hdr_data); STBI_FREE(scanline); return stbi__errpf("corrupt", "bad RLE data in HDR"); }
                            for (z = 0; z < count; ++z)
                                scanline[i++ * 4 + k] = value;
                        }
                        else {
                            // Dump
                            if (count > nleft) { STBI_FREE(hdr_data); STBI_FREE(scanline); return stbi__errpf("corrupt", "bad RLE data in HDR"); }
                            for (z = 0; z < count; ++z)
                                scanline[i++ * 4 + k] = stbi__get8(s);
                        }
                    }
                }
                for (i = 0; i < width; ++i)
                    stbi__hdr_convert(hdr_data + (j * width + i) * req_comp, scanline + i * 4, req_comp);
            }
            if (scanline)
                STBI_FREE(scanline);
        }

        return hdr_data;
    }

    static int stbi__hdr_info(stbi__context* s, int* x, int* y, int* comp)
    {
        char buffer[STBI__HDR_BUFLEN];
        char* token;
        int valid = 0;
        int dummy;

        if (!x) x = &dummy;
        if (!y) y = &dummy;
        if (!comp) comp = &dummy;

        if (stbi__hdr_test(s) == 0) {
            stbi__rewind(s);
            return 0;
        }

        for (;;) {
            token = stbi__hdr_gettoken(s, buffer);
            if (token[0] == 0) break;
            if (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
        }

        if (!valid) {
            stbi__rewind(s);
            return 0;
        }
        token = stbi__hdr_gettoken(s, buffer);
        if (strncmp(token, "-Y ", 3)) {
            stbi__rewind(s);
            return 0;
        }
        token += 3;
        *y = (int)strtol(token, &token, 10);
        while (*token == ' ') ++token;
        if (strncmp(token, "+X ", 3)) {
            stbi__rewind(s);
            return 0;
        }
        token += 3;
        *x = (int)strtol(token, NULL, 10);
        *comp = 3;
        return 1;
    }
#endif // STBI_NO_HDR

#ifndef STBI_NO_BMP
    static int stbi__bmp_info(stbi__context* s, int* x, int* y, int* comp)
    {
        void* p;
        stbi__bmp_data info;

        info.all_a = 255;
        p = stbi__bmp_parse_header(s, &info);
        if (p == NULL) {
            stbi__rewind(s);
            return 0;
        }
        if (x) *x = s->img_x;
        if (y) *y = s->img_y;
        if (comp) {
            if (info.bpp == 24 && info.ma == 0xff000000)
                *comp = 3;
            else
                *comp = info.ma ? 4 : 3;
        }
        return 1;
    }
#endif

#ifndef STBI_NO_PSD
    static int stbi__psd_info(stbi__context* s, int* x, int* y, int* comp)
    {
        int channelCount, dummy, depth;
        if (!x) x = &dummy;
        if (!y) y = &dummy;
        if (!comp) comp = &dummy;
        if (stbi__get32be(s) != 0x38425053) {
            stbi__rewind(s);
            return 0;
        }
        if (stbi__get16be(s) != 1) {
            stbi__rewind(s);
            return 0;
        }
        stbi__skip(s, 6);
        channelCount = stbi__get16be(s);
        if (channelCount < 0 || channelCount > 16) {
            stbi__rewind(s);
            return 0;
        }
        *y = stbi__get32be(s);
        *x = stbi__get32be(s);
        depth = stbi__get16be(s);
        if (depth != 8 && depth != 16) {
            stbi__rewind(s);
            return 0;
        }
        if (stbi__get16be(s) != 3) {
            stbi__rewind(s);
            return 0;
        }
        *comp = 4;
        return 1;
    }

    static int stbi__psd_is16(stbi__context* s)
    {
        int channelCount, depth;
        if (stbi__get32be(s) != 0x38425053) {
            stbi__rewind(s);
            return 0;
        }
        if (stbi__get16be(s) != 1) {
            stbi__rewind(s);
            return 0;
        }
        stbi__skip(s, 6);
        channelCount = stbi__get16be(s);
        if (channelCount < 0 || channelCount > 16) {
            stbi__rewind(s);
            return 0;
        }
        STBI_NOTUSED(stbi__get32be(s));
        STBI_NOTUSED(stbi__get32be(s));
        depth = stbi__get16be(s);
        if (depth != 16) {
            stbi__rewind(s);
            return 0;
        }
        return 1;
    }
#endif

#ifndef STBI_NO_PIC
    static int stbi__pic_info(stbi__context* s, int* x, int* y, int* comp)
    {
        int act_comp = 0, num_packets = 0, chained, dummy;
        stbi__pic_packet packets[10];

        if (!x) x = &dummy;
        if (!y) y = &dummy;
        if (!comp) comp = &dummy;

        if (!stbi__pic_is4(s, "\x53\x80\xF6\x34")) {
            stbi__rewind(s);
            return 0;
        }

        stbi__skip(s, 88);

        *x = stbi__get16be(s);
        *y = stbi__get16be(s);
        if (stbi__at_eof(s)) {
            stbi__rewind(s);
            return 0;
        }
        if ((*x) != 0 && (1 << 28) / (*x) < (*y)) {
            stbi__rewind(s);
            return 0;
        }

        stbi__skip(s, 8);

        do {
            stbi__pic_packet* packet;

            if (num_packets == sizeof(packets) / sizeof(packets[0]))
                return 0;

            packet = &packets[num_packets++];
            chained = stbi__get8(s);
            packet->size = stbi__get8(s);
            packet->type = stbi__get8(s);
            packet->channel = stbi__get8(s);
            act_comp |= packet->channel;

            if (stbi__at_eof(s)) {
                stbi__rewind(s);
                return 0;
            }
            if (packet->size != 8) {
                stbi__rewind(s);
                return 0;
            }
        } while (chained);

        *comp = (act_comp & 0x10 ? 4 : 3);

        return 1;
    }
#endif

    // *************************************************************************************************
    // Portable Gray Map and Portable Pixel Map loader
    // by Ken Miller
    //
    // PGM: http://netpbm.sourceforge.net/doc/pgm.html
    // PPM: http://netpbm.sourceforge.net/doc/ppm.html
    //
    // Known limitations:
    //    Does not support comments in the header section
    //    Does not support ASCII image data (formats P2 and P3)

#ifndef STBI_NO_PNM

    static int      stbi__pnm_test(stbi__context* s)
    {
        char p, t;
        p = (char)stbi__get8(s);
        t = (char)stbi__get8(s);
        if (p != 'P' || (t != '5' && t != '6')) {
            stbi__rewind(s);
            return 0;
        }
        return 1;
    }

    static void* stbi__pnm_load(stbi__context* s, int* x, int* y, int* comp, int req_comp, stbi__result_info* ri)
    {
        stbi_uc* out;
        STBI_NOTUSED(ri);

        ri->bits_per_channel = stbi__pnm_info(s, (int*)&s->img_x, (int*)&s->img_y, (int*)&s->img_n);
        if (ri->bits_per_channel == 0)
            return 0;

        if (s->img_y > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");
        if (s->img_x > STBI_MAX_DIMENSIONS) return stbi__errpuc("too large", "Very large image (corrupt?)");

        *x = s->img_x;
        *y = s->img_y;
        if (comp) *comp = s->img_n;

        if (!stbi__mad4sizes_valid(s->img_n, s->img_x, s->img_y, ri->bits_per_channel / 8, 0))
            return stbi__errpuc("too large", "PNM too large");

        out = (stbi_uc*)stbi__malloc_mad4(s->img_n, s->img_x, s->img_y, ri->bits_per_channel / 8, 0);
        if (!out) return stbi__errpuc("outofmem", "Out of memory");
        stbi__getn(s, out, s->img_n * s->img_x * s->img_y * (ri->bits_per_channel / 8));

        if (req_comp && req_comp != s->img_n) {
            out = stbi__convert_format(out, s->img_n, req_comp, s->img_x, s->img_y);
            if (out == NULL) return out; // stbi__convert_format frees input on failure
        }
        return out;
    }

    static int      stbi__pnm_isspace(char c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
    }

    static void     stbi__pnm_skip_whitespace(stbi__context* s, char* c)
    {
        for (;;) {
            while (!stbi__at_eof(s) && stbi__pnm_isspace(*c))
                *c = (char)stbi__get8(s);

            if (stbi__at_eof(s) || *c != '#')
                break;

            while (!stbi__at_eof(s) && *c != '\n' && *c != '\r')
                *c = (char)stbi__get8(s);
        }
    }

    static int      stbi__pnm_isdigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    static int      stbi__pnm_getinteger(stbi__context* s, char* c)
    {
        int value = 0;

        while (!stbi__at_eof(s) && stbi__pnm_isdigit(*c)) {
            value = value * 10 + (*c - '0');
            *c = (char)stbi__get8(s);
        }

        return value;
    }

    static int      stbi__pnm_info(stbi__context* s, int* x, int* y, int* comp)
    {
        int maxv, dummy;
        char c, p, t;

        if (!x) x = &dummy;
        if (!y) y = &dummy;
        if (!comp) comp = &dummy;

        stbi__rewind(s);

        // Get identifier
        p = (char)stbi__get8(s);
        t = (char)stbi__get8(s);
        if (p != 'P' || (t != '5' && t != '6')) {
            stbi__rewind(s);
            return 0;
        }

        *comp = (t == '6') ? 3 : 1;  // '5' is 1-component .pgm; '6' is 3-component .ppm

        c = (char)stbi__get8(s);
        stbi__pnm_skip_whitespace(s, &c);

        *x = stbi__pnm_getinteger(s, &c); // read width
        stbi__pnm_skip_whitespace(s, &c);

        *y = stbi__pnm_getinteger(s, &c); // read height
        stbi__pnm_skip_whitespace(s, &c);

        maxv = stbi__pnm_getinteger(s, &c);  // read max value
        if (maxv > 65535)
            return stbi__err("max value > 65535", "PPM image supports only 8-bit and 16-bit images");
        else if (maxv > 255)
            return 16;
        else
            return 8;
    }

    static int stbi__pnm_is16(stbi__context* s)
    {
        if (stbi__pnm_info(s, NULL, NULL, NULL) == 16)
            return 1;
        return 0;
    }
#endif

    static int stbi__info_main(stbi__context* s, int* x, int* y, int* comp)
    {
#ifndef STBI_NO_JPEG
        if (stbi__jpeg_info(s, x, y, comp)) return 1;
#endif

#ifndef STBI_NO_PNG
        if (stbi__png_info(s, x, y, comp))  return 1;
#endif

#ifndef STBI_NO_GIF
        if (stbi__gif_info(s, x, y, comp))  return 1;
#endif

#ifndef STBI_NO_BMP
        if (stbi__bmp_info(s, x, y, comp))  return 1;
#endif

#ifndef STBI_NO_PSD
        if (stbi__psd_info(s, x, y, comp))  return 1;
#endif

#ifndef STBI_NO_PIC
        if (stbi__pic_info(s, x, y, comp))  return 1;
#endif

#ifndef STBI_NO_PNM
        if (stbi__pnm_info(s, x, y, comp))  return 1;
#endif

#ifndef STBI_NO_HDR
        if (stbi__hdr_info(s, x, y, comp))  return 1;
#endif

        // test tga last because it's a crappy test!
#ifndef STBI_NO_TGA
        if (stbi__tga_info(s, x, y, comp))
            return 1;
#endif
        return stbi__err("unknown image type", "Image not of any known type, or corrupt");
    }

    static int stbi__is_16_main(stbi__context* s)
    {
#ifndef STBI_NO_PNG
        if (stbi__png_is16(s))  return 1;
#endif

#ifndef STBI_NO_PSD
        if (stbi__psd_is16(s))  return 1;
#endif

#ifndef STBI_NO_PNM
        if (stbi__pnm_is16(s))  return 1;
#endif
        return 0;
    }

#ifndef STBI_NO_STDIO
    STBIDEF int stbi_info(char const* filename, int* x, int* y, int* comp)
    {
        FILE* f = stbi__fopen(filename, "rb");
        int result;
        if (!f) return stbi__err("can't fopen", "Unable to open file");
        result = stbi_info_from_file(f, x, y, comp);
        fclose(f);
        return result;
    }

    STBIDEF int stbi_info_from_file(FILE* f, int* x, int* y, int* comp)
    {
        int r;
        stbi__context s;
        long pos = ftell(f);
        stbi__start_file(&s, f);
        r = stbi__info_main(&s, x, y, comp);
        fseek(f, pos, SEEK_SET);
        return r;
    }

    STBIDEF int stbi_is_16_bit(char const* filename)
    {
        FILE* f = stbi__fopen(filename, "rb");
        int result;
        if (!f) return stbi__err("can't fopen", "Unable to open file");
        result = stbi_is_16_bit_from_file(f);
        fclose(f);
        return result;
    }

    STBIDEF int stbi_is_16_bit_from_file(FILE* f)
    {
        int r;
        stbi__context s;
        long pos = ftell(f);
        stbi__start_file(&s, f);
        r = stbi__is_16_main(&s);
        fseek(f, pos, SEEK_SET);
        return r;
    }
#endif // !STBI_NO_STDIO

    STBIDEF int stbi_info_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp)
    {
        stbi__context s;
        stbi__start_mem(&s, buffer, len);
        return stbi__info_main(&s, x, y, comp);
    }

    STBIDEF int stbi_info_from_callbacks(stbi_io_callbacks const* c, void* user, int* x, int* y, int* comp)
    {
        stbi__context s;
        stbi__start_callbacks(&s, (stbi_io_callbacks*)c, user);
        return stbi__info_main(&s, x, y, comp);
    }

    STBIDEF int stbi_is_16_bit_from_memory(stbi_uc const* buffer, int len)
    {
        stbi__context s;
        stbi__start_mem(&s, buffer, len);
        return stbi__is_16_main(&s);
    }

    STBIDEF int stbi_is_16_bit_from_callbacks(stbi_io_callbacks const* c, void* user)
    {
        stbi__context s;
        stbi__start_callbacks(&s, (stbi_io_callbacks*)c, user);
        return stbi__is_16_main(&s);
    }

#endif // STB_IMAGE_IMPLEMENTATION

    /*
       revision history:
          2.20  (2019-02-07) support utf8 filenames in Windows; fix warnings and platform ifdefs
          2.19  (2018-02-11) fix warning
          2.18  (2018-01-30) fix warnings
          2.17  (2018-01-29) change sbti__shiftsigned to avoid clang -O2 bug
                             1-bit BMP
                             *_is_16_bit api
                             avoid warnings
          2.16  (2017-07-23) all functions have 16-bit variants;
                             STBI_NO_STDIO works again;
                             compilation fixes;
                             fix rounding in unpremultiply;
                             optimize vertical flip;
                             disable raw_len validation;
                             documentation fixes
          2.15  (2017-03-18) fix png-1,2,4 bug; now all Imagenet JPGs decode;
                             warning fixes; disable run-time SSE detection on gcc;
                             uniform handling of optional "return" values;
                             thread-safe initialization of zlib tables
          2.14  (2017-03-03) remove deprecated STBI_JPEG_OLD; fixes for Imagenet JPGs
          2.13  (2016-11-29) add 16-bit API, only supported for PNG right now
          2.12  (2016-04-02) fix typo in 2.11 PSD fix that caused crashes
          2.11  (2016-04-02) allocate large structures on the stack
                             remove white matting for transparent PSD
                             fix reported channel count for PNG & BMP
                             re-enable SSE2 in non-gcc 64-bit
                             support RGB-formatted JPEG
                             read 16-bit PNGs (only as 8-bit)
          2.10  (2016-01-22) avoid warning introduced in 2.09 by STBI_REALLOC_SIZED
          2.09  (2016-01-16) allow comments in PNM files
                             16-bit-per-pixel TGA (not bit-per-component)
                             info() for TGA could break due to .hdr handling
                             info() for BMP to shares code instead of sloppy parse
                             can use STBI_REALLOC_SIZED if allocator doesn't support realloc
                             code cleanup
          2.08  (2015-09-13) fix to 2.07 cleanup, reading RGB PSD as RGBA
          2.07  (2015-09-13) fix compiler warnings
                             partial animated GIF support
                             limited 16-bpc PSD support
                             #ifdef unused functions
                             bug with < 92 byte PIC,PNM,HDR,TGA
          2.06  (2015-04-19) fix bug where PSD returns wrong '*comp' value
          2.05  (2015-04-19) fix bug in progressive JPEG handling, fix warning
          2.04  (2015-04-15) try to re-enable SIMD on MinGW 64-bit
          2.03  (2015-04-12) extra corruption checking (mmozeiko)
                             stbi_set_flip_vertically_on_load (nguillemot)
                             fix NEON support; fix mingw support
          2.02  (2015-01-19) fix incorrect assert, fix warning
          2.01  (2015-01-17) fix various warnings; suppress SIMD on gcc 32-bit without -msse2
          2.00b (2014-12-25) fix STBI_MALLOC in progressive JPEG
          2.00  (2014-12-25) optimize JPG, including x86 SSE2 & NEON SIMD (ryg)
                             progressive JPEG (stb)
                             PGM/PPM support (Ken Miller)
                             STBI_MALLOC,STBI_REALLOC,STBI_FREE
                             GIF bugfix -- seemingly never worked
                             STBI_NO_*, STBI_ONLY_*
          1.48  (2014-12-14) fix incorrectly-named assert()
          1.47  (2014-12-14) 1/2/4-bit PNG support, both direct and paletted (Omar Cornut & stb)
                             optimize PNG (ryg)
                             fix bug in interlaced PNG with user-specified channel count (stb)
          1.46  (2014-08-26)
                  fix broken tRNS chunk (colorkey-style transparency) in non-paletted PNG
          1.45  (2014-08-16)
                  fix MSVC-ARM internal compiler error by wrapping malloc
          1.44  (2014-08-07)
                  various warning fixes from Ronny Chevalier
          1.43  (2014-07-15)
                  fix MSVC-only compiler problem in code changed in 1.42
          1.42  (2014-07-09)
                  don't define _CRT_SECURE_NO_WARNINGS (affects user code)
                  fixes to stbi__cleanup_jpeg path
                  added STBI_ASSERT to avoid requiring assert.h
          1.41  (2014-06-25)
                  fix search&replace from 1.36 that messed up comments/error messages
          1.40  (2014-06-22)
                  fix gcc struct-initialization warning
          1.39  (2014-06-15)
                  fix to TGA optimization when req_comp != number of components in TGA;
                  fix to GIF loading because BMP wasn't rewinding (whoops, no GIFs in my test suite)
                  add support for BMP version 5 (more ignored fields)
          1.38  (2014-06-06)
                  suppress MSVC warnings on integer casts truncating values
                  fix accidental rename of 'skip' field of I/O
          1.37  (2014-06-04)
                  remove duplicate typedef
          1.36  (2014-06-03)
                  convert to header file single-file library
                  if de-iphone isn't set, load iphone images color-swapped instead of returning NULL
          1.35  (2014-05-27)
                  various warnings
                  fix broken STBI_SIMD path
                  fix bug where stbi_load_from_file no longer left file pointer in correct place
                  fix broken non-easy path for 32-bit BMP (possibly never used)
                  TGA optimization by Arseny Kapoulkine
          1.34  (unknown)
                  use STBI_NOTUSED in stbi__resample_row_generic(), fix one more leak in tga failure case
          1.33  (2011-07-14)
                  make stbi_is_hdr work in STBI_NO_HDR (as specified), minor compiler-friendly improvements
          1.32  (2011-07-13)
                  support for "info" function for all supported filetypes (SpartanJ)
          1.31  (2011-06-20)
                  a few more leak fixes, bug in PNG handling (SpartanJ)
          1.30  (2011-06-11)
                  added ability to load files via callbacks to accomidate custom input streams (Ben Wenger)
                  removed deprecated format-specific test/load functions
                  removed support for installable file formats (stbi_loader) -- would have been broken for IO callbacks anyway
                  error cases in bmp and tga give messages and don't leak (Raymond Barbiero, grisha)
                  fix inefficiency in decoding 32-bit BMP (David Woo)
          1.29  (2010-08-16)
                  various warning fixes from Aurelien Pocheville
          1.28  (2010-08-01)
                  fix bug in GIF palette transparency (SpartanJ)
          1.27  (2010-08-01)
                  cast-to-stbi_uc to fix warnings
          1.26  (2010-07-24)
                  fix bug in file buffering for PNG reported by SpartanJ
          1.25  (2010-07-17)
                  refix trans_data warning (Won Chun)
          1.24  (2010-07-12)
                  perf improvements reading from files on platforms with lock-heavy fgetc()
                  minor perf improvements for jpeg
                  deprecated type-specific functions so we'll get feedback if they're needed
                  attempt to fix trans_data warning (Won Chun)
          1.23    fixed bug in iPhone support
          1.22  (2010-07-10)
                  removed image *writing* support
                  stbi_info support from Jetro Lauha
                  GIF support from Jean-Marc Lienher
                  iPhone PNG-extensions from James Brown
                  warning-fixes from Nicolas Schulz and Janez Zemva (i.stbi__err. Janez (U+017D)emva)
          1.21    fix use of 'stbi_uc' in header (reported by jon blow)
          1.20    added support for Softimage PIC, by Tom Seddon
          1.19    bug in interlaced PNG corruption check (found by ryg)
          1.18  (2008-08-02)
                  fix a threading bug (local mutable static)
          1.17    support interlaced PNG
          1.16    major bugfix - stbi__convert_format converted one too many pixels
          1.15    initialize some fields for thread safety
          1.14    fix threadsafe conversion bug
                  header-file-only version (#define STBI_HEADER_FILE_ONLY before including)
          1.13    threadsafe
          1.12    const qualifiers in the API
          1.11    Support installable IDCT, colorspace conversion routines
          1.10    Fixes for 64-bit (don't use "unsigned long")
                  optimized upsampling by Fabian "ryg" Giesen
          1.09    Fix format-conversion for PSD code (bad global variables!)
          1.08    Thatcher Ulrich's PSD code integrated by Nicolas Schulz
          1.07    attempt to fix C++ warning/errors again
          1.06    attempt to fix C++ warning/errors again
          1.05    fix TGA loading to return correct *comp and use good luminance calc
          1.04    default float alpha is 1, not 255; use 'void *' for stbi_image_free
          1.03    bugfixes to STBI_NO_STDIO, STBI_NO_HDR
          1.02    support for (subset of) HDR files, float interface for preferred access to them
          1.01    fix bug: possible bug in handling right-side up bmps... not sure
                  fix bug: the stbi__bmp_load() and stbi__tga_load() functions didn't work at all
          1.00    interface to zlib that skips zlib header
          0.99    correct handling of alpha in palette
          0.98    TGA loader by lonesock; dynamically add loaders (untested)
          0.97    jpeg errors on too large a file; also catch another malloc failure
          0.96    fix detection of invalid v value - particleman@mollyrocket forum
          0.95    during header scan, seek to markers in case of padding
          0.94    STBI_NO_STDIO to disable stdio usage; rename all #defines the same
          0.93    handle jpegtran output; verbose errors
          0.92    read 4,8,16,24,32-bit BMP files of several formats
          0.91    output 24-bit Windows 3.0 BMP files
          0.90    fix a few more warnings; bump version number to approach 1.0
          0.61    bugfixes due to Marc LeBlanc, Christopher Lloyd
          0.60    fix compiling as c++
          0.59    fix warnings: merge Dave Moore's -Wall fixes
          0.58    fix bug: zlib uncompressed mode len/nlen was wrong endian
          0.57    fix bug: jpg last huffman symbol before marker was >9 bits but less than 16 available
          0.56    fix bug: zlib uncompressed mode len vs. nlen
          0.55    fix bug: restart_interval not initialized to 0
          0.54    allow NULL for 'int *comp'
          0.53    fix bug in png 3->4; speedup png decoding
          0.52    png handles req_comp=3,4 directly; minor cleanup; jpeg comments
          0.51    obey req_comp requests, 1-component jpegs return as 1-component,
                  on 'test' only check type, not whether we support this variant
          0.50  (2006-11-19)
                  first released version
    */


    /*
    ------------------------------------------------------------------------------
    This software is available under 2 licenses -- choose whichever you prefer.
    ------------------------------------------------------------------------------
    ALTERNATIVE A - MIT License
    Copyright (c) 2017 Sean Barrett
    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in
    the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is furnished to do
    so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
    ------------------------------------------------------------------------------
    ALTERNATIVE B - Public Domain (www.unlicense.org)
    This is free and unencumbered software released into the public domain.
    Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
    software, either in source code form or as a compiled binary, for any purpose,
    commercial or non-commercial, and by any means.
    In jurisdictions that recognize copyright laws, the author or authors of this
    software dedicate any and all copyright interest in the software to the public
    domain. We make this dedication for the benefit of the public at large and to
    the detriment of our heirs and successors. We intend this dedication to be an
    overt act of relinquishment in perpetuity of all present and future rights to
    this software under copyright law.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    ------------------------------------------------------------------------------
    */

#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include "stb_image_resize.h"
/* stb_image_resize - v0.97 - public domain image resizing
   by Jorge L Rodriguez (@VinoBS) - 2014
   http://github.com/nothings/stb

   Written with emphasis on usability, portability, and efficiency. (No
   SIMD or threads, so it be easily outperformed by libs that use those.)
   Only scaling and translation is supported, no rotations or shears.
   Easy API downsamples w/Mitchell filter, upsamples w/cubic interpolation.

   COMPILING & LINKING
      In one C/C++ file that #includes this file, do this:
         #define STB_IMAGE_RESIZE_IMPLEMENTATION
      before the #include. That will create the implementation in that file.

   QUICKSTART
      stbir_resize_uint8(      input_pixels , in_w , in_h , 0,
                               output_pixels, out_w, out_h, 0, num_channels)
      stbir_resize_float(...)
      stbir_resize_uint8_srgb( input_pixels , in_w , in_h , 0,
                               output_pixels, out_w, out_h, 0,
                               num_channels , alpha_chan  , 0)
      stbir_resize_uint8_srgb_edgemode(
                               input_pixels , in_w , in_h , 0,
                               output_pixels, out_w, out_h, 0,
                               num_channels , alpha_chan  , 0, STBIR_EDGE_CLAMP)
                                                            // WRAP/REFLECT/ZERO

   FULL API
      See the "header file" section of the source for API documentation.

   ADDITIONAL DOCUMENTATION

      SRGB & FLOATING POINT REPRESENTATION
         The sRGB functions presume IEEE floating point. If you do not have
         IEEE floating point, define STBIR_NON_IEEE_FLOAT. This will use
         a slower implementation.

      MEMORY ALLOCATION
         The resize functions here perform a single memory allocation using
         malloc. To control the memory allocation, before the #include that
         triggers the implementation, do:

            #define STBIR_MALLOC(size,context) ...
            #define STBIR_FREE(ptr,context)   ...

         Each resize function makes exactly one call to malloc/free, so to use
         temp memory, store the temp memory in the context and return that.

      ASSERT
         Define STBIR_ASSERT(boolval) to override assert() and not use assert.h

      OPTIMIZATION
         Define STBIR_SATURATE_INT to compute clamp values in-range using
         integer operations instead of float operations. This may be faster
         on some platforms.

      DEFAULT FILTERS
         For functions which don't provide explicit control over what filters
         to use, you can change the compile-time defaults with

            #define STBIR_DEFAULT_FILTER_UPSAMPLE     STBIR_FILTER_something
            #define STBIR_DEFAULT_FILTER_DOWNSAMPLE   STBIR_FILTER_something

         See stbir_filter in the header-file section for the list of filters.

      NEW FILTERS
         A number of 1D filter kernels are used. For a list of
         supported filters see the stbir_filter enum. To add a new filter,
         write a filter function and add it to stbir__filter_info_table.

      PROGRESS
         For interactive use with slow resize operations, you can install
         a progress-report callback:

            #define STBIR_PROGRESS_REPORT(val)   some_func(val)

         The parameter val is a float which goes from 0 to 1 as progress is made.

         For example:

            static void my_progress_report(float progress);
            #define STBIR_PROGRESS_REPORT(val) my_progress_report(val)

            #define STB_IMAGE_RESIZE_IMPLEMENTATION
            #include "stb_image_resize.h"

            static void my_progress_report(float progress)
            {
               printf("Progress: %f%%\n", progress*100);
            }

      MAX CHANNELS
         If your image has more than 64 channels, define STBIR_MAX_CHANNELS
         to the max you'll have.

      ALPHA CHANNEL
         Most of the resizing functions provide the ability to control how
         the alpha channel of an image is processed. The important things
         to know about this:

         1. The best mathematically-behaved version of alpha to use is
         called "premultiplied alpha", in which the other color channels
         have had the alpha value multiplied in. If you use premultiplied
         alpha, linear filtering (such as image resampling done by this
         library, or performed in texture units on GPUs) does the "right
         thing". While premultiplied alpha is standard in the movie CGI
         industry, it is still uncommon in the videogame/real-time world.

         If you linearly filter non-premultiplied alpha, strange effects
         occur. (For example, the 50/50 average of 99% transparent bright green
         and 1% transparent black produces 50% transparent dark green when
         non-premultiplied, whereas premultiplied it produces 50%
         transparent near-black. The former introduces green energy
         that doesn't exist in the source image.)

         2. Artists should not edit premultiplied-alpha images; artists
         want non-premultiplied alpha images. Thus, art tools generally output
         non-premultiplied alpha images.

         3. You will get best results in most cases by converting images
         to premultiplied alpha before processing them mathematically.

         4. If you pass the flag STBIR_FLAG_ALPHA_PREMULTIPLIED, the
         resizer does not do anything special for the alpha channel;
         it is resampled identically to other channels. This produces
         the correct results for premultiplied-alpha images, but produces
         less-than-ideal results for non-premultiplied-alpha images.

         5. If you do not pass the flag STBIR_FLAG_ALPHA_PREMULTIPLIED,
         then the resizer weights the contribution of input pixels
         based on their alpha values, or, equivalently, it multiplies
         the alpha value into the color channels, resamples, then divides
         by the resultant alpha value. Input pixels which have alpha=0 do
         not contribute at all to output pixels unless _all_ of the input
         pixels affecting that output pixel have alpha=0, in which case
         the result for that pixel is the same as it would be without
         STBIR_FLAG_ALPHA_PREMULTIPLIED. However, this is only true for
         input images in integer formats. For input images in float format,
         input pixels with alpha=0 have no effect, and output pixels
         which have alpha=0 will be 0 in all channels. (For float images,
         you can manually achieve the same result by adding a tiny epsilon
         value to the alpha channel of every image, and then subtracting
         or clamping it at the end.)

         6. You can suppress the behavior described in #5 and make
         all-0-alpha pixels have 0 in all channels by #defining
         STBIR_NO_ALPHA_EPSILON.

         7. You can separately control whether the alpha channel is
         interpreted as linear or affected by the colorspace. By default
         it is linear; you almost never want to apply the colorspace.
         (For example, graphics hardware does not apply sRGB conversion
         to the alpha channel.)

   CONTRIBUTORS
      Jorge L Rodriguez: Implementation
      Sean Barrett: API design, optimizations
      Aras Pranckevicius: bugfix
      Nathan Reed: warning fixes

   REVISIONS
      0.97 (2020-02-02) fixed warning
      0.96 (2019-03-04) fixed warnings
      0.95 (2017-07-23) fixed warnings
      0.94 (2017-03-18) fixed warnings
      0.93 (2017-03-03) fixed bug with certain combinations of heights
      0.92 (2017-01-02) fix integer overflow on large (>2GB) images
      0.91 (2016-04-02) fix warnings; fix handling of subpixel regions
      0.90 (2014-09-17) first released version

   LICENSE
     See end of file for license information.

   TODO
      Don't decode all of the image data when only processing a partial tile
      Don't use full-width decode buffers when only processing a partial tile
      When processing wide images, break processing into tiles so data fits in L1 cache
      Installable filters?
      Resize that respects alpha test coverage
         (Reference code: FloatImage::alphaTestCoverage and FloatImage::scaleAlphaToCoverage:
         https://code.google.com/p/nvidia-texture-tools/source/browse/trunk/src/nvimage/FloatImage.cpp )
*/

#ifndef STBIR_INCLUDE_STB_IMAGE_RESIZE_H
#define STBIR_INCLUDE_STB_IMAGE_RESIZE_H

#ifdef _MSC_VER
typedef unsigned char  stbir_uint8;
typedef unsigned short stbir_uint16;
typedef unsigned int   stbir_uint32;
#else
#include <stdint.h>
typedef uint8_t  stbir_uint8;
typedef uint16_t stbir_uint16;
typedef uint32_t stbir_uint32;
#endif

#ifndef STBIRDEF
#ifdef STB_IMAGE_RESIZE_STATIC
#define STBIRDEF static
#else
#ifdef __cplusplus
#define STBIRDEF extern "C"
#else
#define STBIRDEF extern
#endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Easy-to-use API:
//
//     * "input pixels" points to an array of image data with 'num_channels' channels (e.g. RGB=3, RGBA=4)
//     * input_w is input image width (x-axis), input_h is input image height (y-axis)
//     * stride is the offset between successive rows of image data in memory, in bytes. you can
//       specify 0 to mean packed continuously in memory
//     * alpha channel is treated identically to other channels.
//     * colorspace is linear or sRGB as specified by function name
//     * returned result is 1 for success or 0 in case of an error.
//       #define STBIR_ASSERT() to trigger an assert on parameter validation errors.
//     * Memory required grows approximately linearly with input and output size, but with
//       discontinuities at input_w == output_w and input_h == output_h.
//     * These functions use a "default" resampling filter defined at compile time. To change the filter,
//       you can change the compile-time defaults by #defining STBIR_DEFAULT_FILTER_UPSAMPLE
//       and STBIR_DEFAULT_FILTER_DOWNSAMPLE, or you can use the medium-complexity API.

STBIRDEF int stbir_resize_uint8(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels);

STBIRDEF int stbir_resize_float(const float* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    float* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels);


// The following functions interpret image data as gamma-corrected sRGB.
// Specify STBIR_ALPHA_CHANNEL_NONE if you have no alpha channel,
// or otherwise provide the index of the alpha channel. Flags value
// of 0 will probably do the right thing if you're not sure what
// the flags mean.

#define STBIR_ALPHA_CHANNEL_NONE       -1

// Set this flag if your texture has premultiplied alpha. Otherwise, stbir will
// use alpha-weighted resampling (effectively premultiplying, resampling,
// then unpremultiplying).
#define STBIR_FLAG_ALPHA_PREMULTIPLIED    (1 << 0)
// The specified alpha channel should be handled as gamma-corrected value even
// when doing sRGB operations.
#define STBIR_FLAG_ALPHA_USES_COLORSPACE  (1 << 1)

STBIRDEF int stbir_resize_uint8_srgb(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags);


typedef enum
{
    STBIR_EDGE_CLAMP = 1,
    STBIR_EDGE_REFLECT = 2,
    STBIR_EDGE_WRAP = 3,
    STBIR_EDGE_ZERO = 4,
} stbir_edge;

// This function adds the ability to specify how requests to sample off the edge of the image are handled.
STBIRDEF int stbir_resize_uint8_srgb_edgemode(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode);

//////////////////////////////////////////////////////////////////////////////
//
// Medium-complexity API
//
// This extends the easy-to-use API as follows:
//
//     * Alpha-channel can be processed separately
//       * If alpha_channel is not STBIR_ALPHA_CHANNEL_NONE
//         * Alpha channel will not be gamma corrected (unless flags&STBIR_FLAG_GAMMA_CORRECT)
//         * Filters will be weighted by alpha channel (unless flags&STBIR_FLAG_ALPHA_PREMULTIPLIED)
//     * Filter can be selected explicitly
//     * uint16 image type
//     * sRGB colorspace available for all types
//     * context parameter for passing to STBIR_MALLOC

typedef enum
{
    STBIR_FILTER_DEFAULT = 0,  // use same filter type that easy-to-use API chooses
    STBIR_FILTER_BOX = 1,  // A trapezoid w/1-pixel wide ramps, same result as box for integer scale ratios
    STBIR_FILTER_TRIANGLE = 2,  // On upsampling, produces same results as bilinear texture filtering
    STBIR_FILTER_CUBICBSPLINE = 3,  // The cubic b-spline (aka Mitchell-Netrevalli with B=1,C=0), gaussian-esque
    STBIR_FILTER_CATMULLROM = 4,  // An interpolating cubic spline
    STBIR_FILTER_MITCHELL = 5,  // Mitchell-Netrevalli filter with B=1/3, C=1/3
} stbir_filter;

typedef enum
{
    STBIR_COLORSPACE_LINEAR,
    STBIR_COLORSPACE_SRGB,

    STBIR_MAX_COLORSPACES,
} stbir_colorspace;

// The following functions are all identical except for the type of the image data

STBIRDEF int stbir_resize_uint8_generic(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space,
    void* alloc_context);

STBIRDEF int stbir_resize_uint16_generic(const stbir_uint16* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    stbir_uint16* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space,
    void* alloc_context);

STBIRDEF int stbir_resize_float_generic(const float* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    float* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space,
    void* alloc_context);



//////////////////////////////////////////////////////////////////////////////
//
// Full-complexity API
//
// This extends the medium API as follows:
//
//       * uint32 image type
//     * not typesafe
//     * separate filter types for each axis
//     * separate edge modes for each axis
//     * can specify scale explicitly for subpixel correctness
//     * can specify image source tile using texture coordinates

typedef enum
{
    STBIR_TYPE_UINT8,
    STBIR_TYPE_UINT16,
    STBIR_TYPE_UINT32,
    STBIR_TYPE_FLOAT,

    STBIR_MAX_TYPES
} stbir_datatype;

STBIRDEF int stbir_resize(const void* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    void* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    stbir_datatype datatype,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_mode_horizontal, stbir_edge edge_mode_vertical,
    stbir_filter filter_horizontal, stbir_filter filter_vertical,
    stbir_colorspace space, void* alloc_context);

STBIRDEF int stbir_resize_subpixel(const void* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    void* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    stbir_datatype datatype,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_mode_horizontal, stbir_edge edge_mode_vertical,
    stbir_filter filter_horizontal, stbir_filter filter_vertical,
    stbir_colorspace space, void* alloc_context,
    float x_scale, float y_scale,
    float x_offset, float y_offset);

STBIRDEF int stbir_resize_region(const void* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    void* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    stbir_datatype datatype,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_mode_horizontal, stbir_edge edge_mode_vertical,
    stbir_filter filter_horizontal, stbir_filter filter_vertical,
    stbir_colorspace space, void* alloc_context,
    float s0, float t0, float s1, float t1);
// (s0, t0) & (s1, t1) are the top-left and bottom right corner (uv addressing style: [0, 1]x[0, 1]) of a region of the input image to use.

//
//
////   end header file   /////////////////////////////////////////////////////
#endif // STBIR_INCLUDE_STB_IMAGE_RESIZE_H





#ifdef STB_IMAGE_RESIZE_IMPLEMENTATION

#ifndef STBIR_ASSERT
#include <assert.h>
#define STBIR_ASSERT(x) assert(x)
#endif

// For memset
#include <string.h>

#include <math.h>

#ifndef STBIR_MALLOC
#include <stdlib.h>
// use comma operator to evaluate c, to avoid "unused parameter" warnings
#define STBIR_MALLOC(size,c) ((void)(c), malloc(size))
#define STBIR_FREE(ptr,c)    ((void)(c), free(ptr))
#endif

#ifndef _MSC_VER
#ifdef __cplusplus
#define stbir__inline inline
#else
#define stbir__inline
#endif
#else
#define stbir__inline __forceinline
#endif


// should produce compiler error if size is wrong
typedef unsigned char stbir__validate_uint32[sizeof(stbir_uint32) == 4 ? 1 : -1];

#ifdef _MSC_VER
#define STBIR__NOTUSED(v)  (void)(v)
#else
#define STBIR__NOTUSED(v)  (void)sizeof(v)
#endif

#define STBIR__ARRAY_SIZE(a) (sizeof((a))/sizeof((a)[0]))

#ifndef STBIR_DEFAULT_FILTER_UPSAMPLE
#define STBIR_DEFAULT_FILTER_UPSAMPLE    STBIR_FILTER_CATMULLROM
#endif

#ifndef STBIR_DEFAULT_FILTER_DOWNSAMPLE
#define STBIR_DEFAULT_FILTER_DOWNSAMPLE  STBIR_FILTER_MITCHELL
#endif

#ifndef STBIR_PROGRESS_REPORT
#define STBIR_PROGRESS_REPORT(float_0_to_1)
#endif

#ifndef STBIR_MAX_CHANNELS
#define STBIR_MAX_CHANNELS 64
#endif

#if STBIR_MAX_CHANNELS > 65536
#error "Too many channels; STBIR_MAX_CHANNELS must be no more than 65536."
// because we store the indices in 16-bit variables
#endif

// This value is added to alpha just before premultiplication to avoid
// zeroing out color values. It is equivalent to 2^-80. If you don't want
// that behavior (it may interfere if you have floating point images with
// very small alpha values) then you can define STBIR_NO_ALPHA_EPSILON to
// disable it.
#ifndef STBIR_ALPHA_EPSILON
#define STBIR_ALPHA_EPSILON ((float)1 / (1 << 20) / (1 << 20) / (1 << 20) / (1 << 20))
#endif



#ifdef _MSC_VER
#define STBIR__UNUSED_PARAM(v)  (void)(v)
#else
#define STBIR__UNUSED_PARAM(v)  (void)sizeof(v)
#endif

// must match stbir_datatype
static unsigned char stbir__type_size[] = {
    1, // STBIR_TYPE_UINT8
    2, // STBIR_TYPE_UINT16
    4, // STBIR_TYPE_UINT32
    4, // STBIR_TYPE_FLOAT
};

// Kernel function centered at 0
typedef float (stbir__kernel_fn)(float x, float scale);
typedef float (stbir__support_fn)(float scale);

typedef struct
{
    stbir__kernel_fn* kernel;
    stbir__support_fn* support;
} stbir__filter_info;

// When upsampling, the contributors are which source pixels contribute.
// When downsampling, the contributors are which destination pixels are contributed to.
typedef struct
{
    int n0; // First contributing pixel
    int n1; // Last contributing pixel
} stbir__contributors;

typedef struct
{
    const void* input_data;
    int input_w;
    int input_h;
    int input_stride_bytes;

    void* output_data;
    int output_w;
    int output_h;
    int output_stride_bytes;

    float s0, t0, s1, t1;

    float horizontal_shift; // Units: output pixels
    float vertical_shift;   // Units: output pixels
    float horizontal_scale;
    float vertical_scale;

    int channels;
    int alpha_channel;
    stbir_uint32 flags;
    stbir_datatype type;
    stbir_filter horizontal_filter;
    stbir_filter vertical_filter;
    stbir_edge edge_horizontal;
    stbir_edge edge_vertical;
    stbir_colorspace colorspace;

    stbir__contributors* horizontal_contributors;
    float* horizontal_coefficients;

    stbir__contributors* vertical_contributors;
    float* vertical_coefficients;

    int decode_buffer_pixels;
    float* decode_buffer;

    float* horizontal_buffer;

    // cache these because ceil/floor are inexplicably showing up in profile
    int horizontal_coefficient_width;
    int vertical_coefficient_width;
    int horizontal_filter_pixel_width;
    int vertical_filter_pixel_width;
    int horizontal_filter_pixel_margin;
    int vertical_filter_pixel_margin;
    int horizontal_num_contributors;
    int vertical_num_contributors;

    int ring_buffer_length_bytes;   // The length of an individual entry in the ring buffer. The total number of ring buffers is stbir__get_filter_pixel_width(filter)
    int ring_buffer_num_entries;    // Total number of entries in the ring buffer.
    int ring_buffer_first_scanline;
    int ring_buffer_last_scanline;
    int ring_buffer_begin_index;    // first_scanline is at this index in the ring buffer
    float* ring_buffer;

    float* encode_buffer; // A temporary buffer to store floats so we don't lose precision while we do multiply-adds.

    int horizontal_contributors_size;
    int horizontal_coefficients_size;
    int vertical_contributors_size;
    int vertical_coefficients_size;
    int decode_buffer_size;
    int horizontal_buffer_size;
    int ring_buffer_size;
    int encode_buffer_size;
} stbir__info;


static const float stbir__max_uint8_as_float = 255.0f;
static const float stbir__max_uint16_as_float = 65535.0f;
static const double stbir__max_uint32_as_float = 4294967295.0;


static stbir__inline int stbir__min(int a, int b)
{
    return a < b ? a : b;
}

static stbir__inline float stbir__saturate(float x)
{
    if (x < 0)
        return 0;

    if (x > 1)
        return 1;

    return x;
}

#ifdef STBIR_SATURATE_INT
static stbir__inline stbir_uint8 stbir__saturate8(int x)
{
    if ((unsigned int)x <= 255)
        return x;

    if (x < 0)
        return 0;

    return 255;
}

static stbir__inline stbir_uint16 stbir__saturate16(int x)
{
    if ((unsigned int)x <= 65535)
        return x;

    if (x < 0)
        return 0;

    return 65535;
}
#endif

static float stbir__srgb_uchar_to_linear_float[256] = {
    0.000000f, 0.000304f, 0.000607f, 0.000911f, 0.001214f, 0.001518f, 0.001821f, 0.002125f, 0.002428f, 0.002732f, 0.003035f,
    0.003347f, 0.003677f, 0.004025f, 0.004391f, 0.004777f, 0.005182f, 0.005605f, 0.006049f, 0.006512f, 0.006995f, 0.007499f,
    0.008023f, 0.008568f, 0.009134f, 0.009721f, 0.010330f, 0.010960f, 0.011612f, 0.012286f, 0.012983f, 0.013702f, 0.014444f,
    0.015209f, 0.015996f, 0.016807f, 0.017642f, 0.018500f, 0.019382f, 0.020289f, 0.021219f, 0.022174f, 0.023153f, 0.024158f,
    0.025187f, 0.026241f, 0.027321f, 0.028426f, 0.029557f, 0.030713f, 0.031896f, 0.033105f, 0.034340f, 0.035601f, 0.036889f,
    0.038204f, 0.039546f, 0.040915f, 0.042311f, 0.043735f, 0.045186f, 0.046665f, 0.048172f, 0.049707f, 0.051269f, 0.052861f,
    0.054480f, 0.056128f, 0.057805f, 0.059511f, 0.061246f, 0.063010f, 0.064803f, 0.066626f, 0.068478f, 0.070360f, 0.072272f,
    0.074214f, 0.076185f, 0.078187f, 0.080220f, 0.082283f, 0.084376f, 0.086500f, 0.088656f, 0.090842f, 0.093059f, 0.095307f,
    0.097587f, 0.099899f, 0.102242f, 0.104616f, 0.107023f, 0.109462f, 0.111932f, 0.114435f, 0.116971f, 0.119538f, 0.122139f,
    0.124772f, 0.127438f, 0.130136f, 0.132868f, 0.135633f, 0.138432f, 0.141263f, 0.144128f, 0.147027f, 0.149960f, 0.152926f,
    0.155926f, 0.158961f, 0.162029f, 0.165132f, 0.168269f, 0.171441f, 0.174647f, 0.177888f, 0.181164f, 0.184475f, 0.187821f,
    0.191202f, 0.194618f, 0.198069f, 0.201556f, 0.205079f, 0.208637f, 0.212231f, 0.215861f, 0.219526f, 0.223228f, 0.226966f,
    0.230740f, 0.234551f, 0.238398f, 0.242281f, 0.246201f, 0.250158f, 0.254152f, 0.258183f, 0.262251f, 0.266356f, 0.270498f,
    0.274677f, 0.278894f, 0.283149f, 0.287441f, 0.291771f, 0.296138f, 0.300544f, 0.304987f, 0.309469f, 0.313989f, 0.318547f,
    0.323143f, 0.327778f, 0.332452f, 0.337164f, 0.341914f, 0.346704f, 0.351533f, 0.356400f, 0.361307f, 0.366253f, 0.371238f,
    0.376262f, 0.381326f, 0.386430f, 0.391573f, 0.396755f, 0.401978f, 0.407240f, 0.412543f, 0.417885f, 0.423268f, 0.428691f,
    0.434154f, 0.439657f, 0.445201f, 0.450786f, 0.456411f, 0.462077f, 0.467784f, 0.473532f, 0.479320f, 0.485150f, 0.491021f,
    0.496933f, 0.502887f, 0.508881f, 0.514918f, 0.520996f, 0.527115f, 0.533276f, 0.539480f, 0.545725f, 0.552011f, 0.558340f,
    0.564712f, 0.571125f, 0.577581f, 0.584078f, 0.590619f, 0.597202f, 0.603827f, 0.610496f, 0.617207f, 0.623960f, 0.630757f,
    0.637597f, 0.644480f, 0.651406f, 0.658375f, 0.665387f, 0.672443f, 0.679543f, 0.686685f, 0.693872f, 0.701102f, 0.708376f,
    0.715694f, 0.723055f, 0.730461f, 0.737911f, 0.745404f, 0.752942f, 0.760525f, 0.768151f, 0.775822f, 0.783538f, 0.791298f,
    0.799103f, 0.806952f, 0.814847f, 0.822786f, 0.830770f, 0.838799f, 0.846873f, 0.854993f, 0.863157f, 0.871367f, 0.879622f,
    0.887923f, 0.896269f, 0.904661f, 0.913099f, 0.921582f, 0.930111f, 0.938686f, 0.947307f, 0.955974f, 0.964686f, 0.973445f,
    0.982251f, 0.991102f, 1.0f
};

static float stbir__srgb_to_linear(float f)
{
    if (f <= 0.04045f)
        return f / 12.92f;
    else
        return (float)pow((f + 0.055f) / 1.055f, 2.4f);
}

static float stbir__linear_to_srgb(float f)
{
    if (f <= 0.0031308f)
        return f * 12.92f;
    else
        return 1.055f * (float)pow(f, 1 / 2.4f) - 0.055f;
}

#ifndef STBIR_NON_IEEE_FLOAT
// From https://gist.github.com/rygorous/2203834

typedef union
{
    stbir_uint32 u;
    float f;
} stbir__FP32;

static const stbir_uint32 fp32_to_srgb8_tab4[104] = {
    0x0073000d, 0x007a000d, 0x0080000d, 0x0087000d, 0x008d000d, 0x0094000d, 0x009a000d, 0x00a1000d,
    0x00a7001a, 0x00b4001a, 0x00c1001a, 0x00ce001a, 0x00da001a, 0x00e7001a, 0x00f4001a, 0x0101001a,
    0x010e0033, 0x01280033, 0x01410033, 0x015b0033, 0x01750033, 0x018f0033, 0x01a80033, 0x01c20033,
    0x01dc0067, 0x020f0067, 0x02430067, 0x02760067, 0x02aa0067, 0x02dd0067, 0x03110067, 0x03440067,
    0x037800ce, 0x03df00ce, 0x044600ce, 0x04ad00ce, 0x051400ce, 0x057b00c5, 0x05dd00bc, 0x063b00b5,
    0x06970158, 0x07420142, 0x07e30130, 0x087b0120, 0x090b0112, 0x09940106, 0x0a1700fc, 0x0a9500f2,
    0x0b0f01cb, 0x0bf401ae, 0x0ccb0195, 0x0d950180, 0x0e56016e, 0x0f0d015e, 0x0fbc0150, 0x10630143,
    0x11070264, 0x1238023e, 0x1357021d, 0x14660201, 0x156601e9, 0x165a01d3, 0x174401c0, 0x182401af,
    0x18fe0331, 0x1a9602fe, 0x1c1502d2, 0x1d7e02ad, 0x1ed4028d, 0x201a0270, 0x21520256, 0x227d0240,
    0x239f0443, 0x25c003fe, 0x27bf03c4, 0x29a10392, 0x2b6a0367, 0x2d1d0341, 0x2ebe031f, 0x304d0300,
    0x31d105b0, 0x34a80555, 0x37520507, 0x39d504c5, 0x3c37048b, 0x3e7c0458, 0x40a8042a, 0x42bd0401,
    0x44c20798, 0x488e071e, 0x4c1c06b6, 0x4f76065d, 0x52a50610, 0x55ac05cc, 0x5892058f, 0x5b590559,
    0x5e0c0a23, 0x631c0980, 0x67db08f6, 0x6c55087f, 0x70940818, 0x74a007bd, 0x787d076c, 0x7c330723,
};

static stbir_uint8 stbir__linear_to_srgb_uchar(float in)
{
    static const stbir__FP32 almostone = { 0x3f7fffff }; // 1-eps
    static const stbir__FP32 minval = { (127 - 13) << 23 };
    stbir_uint32 tab, bias, scale, t;
    stbir__FP32 f;

    // Clamp to [2^(-13), 1-eps]; these two values map to 0 and 1, respectively.
    // The tests are carefully written so that NaNs map to 0, same as in the reference
    // implementation.
    if (!(in > minval.f)) // written this way to catch NaNs
        in = minval.f;
    if (in > almostone.f)
        in = almostone.f;

    // Do the table lookup and unpack bias, scale
    f.f = in;
    tab = fp32_to_srgb8_tab4[(f.u - minval.u) >> 20];
    bias = (tab >> 16) << 9;
    scale = tab & 0xffff;

    // Grab next-highest mantissa bits and perform linear interpolation
    t = (f.u >> 12) & 0xff;
    return (unsigned char)((bias + scale * t) >> 16);
}

#else
// sRGB transition values, scaled by 1<<28
static int stbir__srgb_offset_to_linear_scaled[256] =
{
            0,     40738,    122216,    203693,    285170,    366648,    448125,    529603,
       611080,    692557,    774035,    855852,    942009,   1033024,   1128971,   1229926,
      1335959,   1447142,   1563542,   1685229,   1812268,   1944725,   2082664,   2226148,
      2375238,   2529996,   2690481,   2856753,   3028870,   3206888,   3390865,   3580856,
      3776916,   3979100,   4187460,   4402049,   4622919,   4850123,   5083710,   5323731,
      5570236,   5823273,   6082892,   6349140,   6622065,   6901714,   7188133,   7481369,
      7781466,   8088471,   8402427,   8723380,   9051372,   9386448,   9728650,  10078021,
     10434603,  10798439,  11169569,  11548036,  11933879,  12327139,  12727857,  13136073,
     13551826,  13975156,  14406100,  14844697,  15290987,  15745007,  16206795,  16676389,
     17153826,  17639142,  18132374,  18633560,  19142734,  19659934,  20185196,  20718552,
     21260042,  21809696,  22367554,  22933648,  23508010,  24090680,  24681686,  25281066,
     25888850,  26505076,  27129772,  27762974,  28404716,  29055026,  29713942,  30381490,
     31057708,  31742624,  32436272,  33138682,  33849884,  34569912,  35298800,  36036568,
     36783260,  37538896,  38303512,  39077136,  39859796,  40651528,  41452360,  42262316,
     43081432,  43909732,  44747252,  45594016,  46450052,  47315392,  48190064,  49074096,
     49967516,  50870356,  51782636,  52704392,  53635648,  54576432,  55526772,  56486700,
     57456236,  58435408,  59424248,  60422780,  61431036,  62449032,  63476804,  64514376,
     65561776,  66619028,  67686160,  68763192,  69850160,  70947088,  72053992,  73170912,
     74297864,  75434880,  76581976,  77739184,  78906536,  80084040,  81271736,  82469648,
     83677792,  84896192,  86124888,  87363888,  88613232,  89872928,  91143016,  92423512,
     93714432,  95015816,  96327688,  97650056,  98982952, 100326408, 101680440, 103045072,
    104420320, 105806224, 107202800, 108610064, 110028048, 111456776, 112896264, 114346544,
    115807632, 117279552, 118762328, 120255976, 121760536, 123276016, 124802440, 126339832,
    127888216, 129447616, 131018048, 132599544, 134192112, 135795792, 137410592, 139036528,
    140673648, 142321952, 143981456, 145652208, 147334208, 149027488, 150732064, 152447968,
    154175200, 155913792, 157663776, 159425168, 161197984, 162982240, 164777968, 166585184,
    168403904, 170234160, 172075968, 173929344, 175794320, 177670896, 179559120, 181458992,
    183370528, 185293776, 187228736, 189175424, 191133888, 193104112, 195086128, 197079968,
    199085648, 201103184, 203132592, 205173888, 207227120, 209292272, 211369392, 213458480,
    215559568, 217672656, 219797792, 221934976, 224084240, 226245600, 228419056, 230604656,
    232802400, 235012320, 237234432, 239468736, 241715280, 243974080, 246245120, 248528464,
    250824112, 253132064, 255452368, 257785040, 260130080, 262487520, 264857376, 267239664,
};

static stbir_uint8 stbir__linear_to_srgb_uchar(float f)
{
    int x = (int)(f * (1 << 28)); // has headroom so you don't need to clamp
    int v = 0;
    int i;

    // Refine the guess with a short binary search.
    i = v + 128; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;
    i = v + 64; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;
    i = v + 32; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;
    i = v + 16; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;
    i = v + 8; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;
    i = v + 4; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;
    i = v + 2; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;
    i = v + 1; if (x >= stbir__srgb_offset_to_linear_scaled[i]) v = i;

    return (stbir_uint8)v;
}
#endif

static float stbir__filter_trapezoid(float x, float scale)
{
    float halfscale = scale / 2;
    float t = 0.5f + halfscale;
    STBIR_ASSERT(scale <= 1);

    x = (float)fabs(x);

    if (x >= t)
        return 0;
    else
    {
        float r = 0.5f - halfscale;
        if (x <= r)
            return 1;
        else
            return (t - x) / scale;
    }
}

static float stbir__support_trapezoid(float scale)
{
    STBIR_ASSERT(scale <= 1);
    return 0.5f + scale / 2;
}

static float stbir__filter_triangle(float x, float s)
{
    STBIR__UNUSED_PARAM(s);

    x = (float)fabs(x);

    if (x <= 1.0f)
        return 1 - x;
    else
        return 0;
}

static float stbir__filter_cubic(float x, float s)
{
    STBIR__UNUSED_PARAM(s);

    x = (float)fabs(x);

    if (x < 1.0f)
        return (4 + x * x * (3 * x - 6)) / 6;
    else if (x < 2.0f)
        return (8 + x * (-12 + x * (6 - x))) / 6;

    return (0.0f);
}

static float stbir__filter_catmullrom(float x, float s)
{
    STBIR__UNUSED_PARAM(s);

    x = (float)fabs(x);

    if (x < 1.0f)
        return 1 - x * x * (2.5f - 1.5f * x);
    else if (x < 2.0f)
        return 2 - x * (4 + x * (0.5f * x - 2.5f));

    return (0.0f);
}

static float stbir__filter_mitchell(float x, float s)
{
    STBIR__UNUSED_PARAM(s);

    x = (float)fabs(x);

    if (x < 1.0f)
        return (16 + x * x * (21 * x - 36)) / 18;
    else if (x < 2.0f)
        return (32 + x * (-60 + x * (36 - 7 * x))) / 18;

    return (0.0f);
}

static float stbir__support_zero(float s)
{
    STBIR__UNUSED_PARAM(s);
    return 0;
}

static float stbir__support_one(float s)
{
    STBIR__UNUSED_PARAM(s);
    return 1;
}

static float stbir__support_two(float s)
{
    STBIR__UNUSED_PARAM(s);
    return 2;
}

static stbir__filter_info stbir__filter_info_table[] = {
        { NULL,                     stbir__support_zero },
        { stbir__filter_trapezoid,  stbir__support_trapezoid },
        { stbir__filter_triangle,   stbir__support_one },
        { stbir__filter_cubic,      stbir__support_two },
        { stbir__filter_catmullrom, stbir__support_two },
        { stbir__filter_mitchell,   stbir__support_two },
};

stbir__inline static int stbir__use_upsampling(float ratio)
{
    return ratio > 1;
}

stbir__inline static int stbir__use_width_upsampling(stbir__info* stbir_info)
{
    return stbir__use_upsampling(stbir_info->horizontal_scale);
}

stbir__inline static int stbir__use_height_upsampling(stbir__info* stbir_info)
{
    return stbir__use_upsampling(stbir_info->vertical_scale);
}

// This is the maximum number of input samples that can affect an output sample
// with the given filter
static int stbir__get_filter_pixel_width(stbir_filter filter, float scale)
{
    STBIR_ASSERT(filter != 0);
    STBIR_ASSERT(filter < STBIR__ARRAY_SIZE(stbir__filter_info_table));

    if (stbir__use_upsampling(scale))
        return (int)ceil(stbir__filter_info_table[filter].support(1 / scale) * 2);
    else
        return (int)ceil(stbir__filter_info_table[filter].support(scale) * 2 / scale);
}

// This is how much to expand buffers to account for filters seeking outside
// the image boundaries.
static int stbir__get_filter_pixel_margin(stbir_filter filter, float scale)
{
    return stbir__get_filter_pixel_width(filter, scale) / 2;
}

static int stbir__get_coefficient_width(stbir_filter filter, float scale)
{
    if (stbir__use_upsampling(scale))
        return (int)ceil(stbir__filter_info_table[filter].support(1 / scale) * 2);
    else
        return (int)ceil(stbir__filter_info_table[filter].support(scale) * 2);
}

static int stbir__get_contributors(float scale, stbir_filter filter, int input_size, int output_size)
{
    if (stbir__use_upsampling(scale))
        return output_size;
    else
        return (input_size + stbir__get_filter_pixel_margin(filter, scale) * 2);
}

static int stbir__get_total_horizontal_coefficients(stbir__info* info)
{
    return info->horizontal_num_contributors
        * stbir__get_coefficient_width(info->horizontal_filter, info->horizontal_scale);
}

static int stbir__get_total_vertical_coefficients(stbir__info* info)
{
    return info->vertical_num_contributors
        * stbir__get_coefficient_width(info->vertical_filter, info->vertical_scale);
}

static stbir__contributors* stbir__get_contributor(stbir__contributors* contributors, int n)
{
    return &contributors[n];
}

// For perf reasons this code is duplicated in stbir__resample_horizontal_upsample/downsample,
// if you change it here change it there too.
static float* stbir__get_coefficient(float* coefficients, stbir_filter filter, float scale, int n, int c)
{
    int width = stbir__get_coefficient_width(filter, scale);
    return &coefficients[width * n + c];
}

static int stbir__edge_wrap_slow(stbir_edge edge, int n, int max)
{
    switch (edge)
    {
    case STBIR_EDGE_ZERO:
        return 0; // we'll decode the wrong pixel here, and then overwrite with 0s later

    case STBIR_EDGE_CLAMP:
        if (n < 0)
            return 0;

        if (n >= max)
            return max - 1;

        return n; // NOTREACHED

    case STBIR_EDGE_REFLECT:
    {
        if (n < 0)
        {
            if (n < max)
                return -n;
            else
                return max - 1;
        }

        if (n >= max)
        {
            int max2 = max * 2;
            if (n >= max2)
                return 0;
            else
                return max2 - n - 1;
        }

        return n; // NOTREACHED
    }

    case STBIR_EDGE_WRAP:
        if (n >= 0)
            return (n % max);
        else
        {
            int m = (-n) % max;

            if (m != 0)
                m = max - m;

            return (m);
        }
        // NOTREACHED

    default:
        STBIR_ASSERT(!"Unimplemented edge type");
        return 0;
    }
}

stbir__inline static int stbir__edge_wrap(stbir_edge edge, int n, int max)
{
    // avoid per-pixel switch
    if (n >= 0 && n < max)
        return n;
    return stbir__edge_wrap_slow(edge, n, max);
}

// What input pixels contribute to this output pixel?
static void stbir__calculate_sample_range_upsample(int n, float out_filter_radius, float scale_ratio, float out_shift, int* in_first_pixel, int* in_last_pixel, float* in_center_of_out)
{
    float out_pixel_center = (float)n + 0.5f;
    float out_pixel_influence_lowerbound = out_pixel_center - out_filter_radius;
    float out_pixel_influence_upperbound = out_pixel_center + out_filter_radius;

    float in_pixel_influence_lowerbound = (out_pixel_influence_lowerbound + out_shift) / scale_ratio;
    float in_pixel_influence_upperbound = (out_pixel_influence_upperbound + out_shift) / scale_ratio;

    *in_center_of_out = (out_pixel_center + out_shift) / scale_ratio;
    *in_first_pixel = (int)(floor(in_pixel_influence_lowerbound + 0.5));
    *in_last_pixel = (int)(floor(in_pixel_influence_upperbound - 0.5));
}

// What output pixels does this input pixel contribute to?
static void stbir__calculate_sample_range_downsample(int n, float in_pixels_radius, float scale_ratio, float out_shift, int* out_first_pixel, int* out_last_pixel, float* out_center_of_in)
{
    float in_pixel_center = (float)n + 0.5f;
    float in_pixel_influence_lowerbound = in_pixel_center - in_pixels_radius;
    float in_pixel_influence_upperbound = in_pixel_center + in_pixels_radius;

    float out_pixel_influence_lowerbound = in_pixel_influence_lowerbound * scale_ratio - out_shift;
    float out_pixel_influence_upperbound = in_pixel_influence_upperbound * scale_ratio - out_shift;

    *out_center_of_in = in_pixel_center * scale_ratio - out_shift;
    *out_first_pixel = (int)(floor(out_pixel_influence_lowerbound + 0.5));
    *out_last_pixel = (int)(floor(out_pixel_influence_upperbound - 0.5));
}

static void stbir__calculate_coefficients_upsample(stbir_filter filter, float scale, int in_first_pixel, int in_last_pixel, float in_center_of_out, stbir__contributors* contributor, float* coefficient_group)
{
    int i;
    float total_filter = 0;
    float filter_scale;

    STBIR_ASSERT(in_last_pixel - in_first_pixel <= (int)ceil(stbir__filter_info_table[filter].support(1 / scale) * 2)); // Taken directly from stbir__get_coefficient_width() which we can't call because we don't know if we're horizontal or vertical.

    contributor->n0 = in_first_pixel;
    contributor->n1 = in_last_pixel;

    STBIR_ASSERT(contributor->n1 >= contributor->n0);

    for (i = 0; i <= in_last_pixel - in_first_pixel; i++)
    {
        float in_pixel_center = (float)(i + in_first_pixel) + 0.5f;
        coefficient_group[i] = stbir__filter_info_table[filter].kernel(in_center_of_out - in_pixel_center, 1 / scale);

        // If the coefficient is zero, skip it. (Don't do the <0 check here, we want the influence of those outside pixels.)
        if (i == 0 && !coefficient_group[i])
        {
            contributor->n0 = ++in_first_pixel;
            i--;
            continue;
        }

        total_filter += coefficient_group[i];
    }

    // NOTE(fg): Not actually true in general, nor is there any reason to expect it should be.
    // It would be true in exact math but is at best approximately true in floating-point math,
    // and it would not make sense to try and put actual bounds on this here because it depends
    // on the image aspect ratio which can get pretty extreme.
    //STBIR_ASSERT(stbir__filter_info_table[filter].kernel((float)(in_last_pixel + 1) + 0.5f - in_center_of_out, 1/scale) == 0);

    STBIR_ASSERT(total_filter > 0.9);
    STBIR_ASSERT(total_filter < 1.1f); // Make sure it's not way off.

    // Make sure the sum of all coefficients is 1.
    filter_scale = 1 / total_filter;

    for (i = 0; i <= in_last_pixel - in_first_pixel; i++)
        coefficient_group[i] *= filter_scale;

    for (i = in_last_pixel - in_first_pixel; i >= 0; i--)
    {
        if (coefficient_group[i])
            break;

        // This line has no weight. We can skip it.
        contributor->n1 = contributor->n0 + i - 1;
    }
}

static void stbir__calculate_coefficients_downsample(stbir_filter filter, float scale_ratio, int out_first_pixel, int out_last_pixel, float out_center_of_in, stbir__contributors* contributor, float* coefficient_group)
{
    int i;

    STBIR_ASSERT(out_last_pixel - out_first_pixel <= (int)ceil(stbir__filter_info_table[filter].support(scale_ratio) * 2)); // Taken directly from stbir__get_coefficient_width() which we can't call because we don't know if we're horizontal or vertical.

    contributor->n0 = out_first_pixel;
    contributor->n1 = out_last_pixel;

    STBIR_ASSERT(contributor->n1 >= contributor->n0);

    for (i = 0; i <= out_last_pixel - out_first_pixel; i++)
    {
        float out_pixel_center = (float)(i + out_first_pixel) + 0.5f;
        float x = out_pixel_center - out_center_of_in;
        coefficient_group[i] = stbir__filter_info_table[filter].kernel(x, scale_ratio) * scale_ratio;
    }

    // NOTE(fg): Not actually true in general, nor is there any reason to expect it should be.
    // It would be true in exact math but is at best approximately true in floating-point math,
    // and it would not make sense to try and put actual bounds on this here because it depends
    // on the image aspect ratio which can get pretty extreme.
    //STBIR_ASSERT(stbir__filter_info_table[filter].kernel((float)(out_last_pixel + 1) + 0.5f - out_center_of_in, scale_ratio) == 0);

    for (i = out_last_pixel - out_first_pixel; i >= 0; i--)
    {
        if (coefficient_group[i])
            break;

        // This line has no weight. We can skip it.
        contributor->n1 = contributor->n0 + i - 1;
    }
}

static void stbir__normalize_downsample_coefficients(stbir__contributors* contributors, float* coefficients, stbir_filter filter, float scale_ratio, int input_size, int output_size)
{
    int num_contributors = stbir__get_contributors(scale_ratio, filter, input_size, output_size);
    int num_coefficients = stbir__get_coefficient_width(filter, scale_ratio);
    int i, j;
    int skip;

    for (i = 0; i < output_size; i++)
    {
        float scale;
        float total = 0;

        for (j = 0; j < num_contributors; j++)
        {
            if (i >= contributors[j].n0 && i <= contributors[j].n1)
            {
                float coefficient = *stbir__get_coefficient(coefficients, filter, scale_ratio, j, i - contributors[j].n0);
                total += coefficient;
            }
            else if (i < contributors[j].n0)
                break;
        }

        STBIR_ASSERT(total > 0.9f);
        STBIR_ASSERT(total < 1.1f);

        scale = 1 / total;

        for (j = 0; j < num_contributors; j++)
        {
            if (i >= contributors[j].n0 && i <= contributors[j].n1)
                *stbir__get_coefficient(coefficients, filter, scale_ratio, j, i - contributors[j].n0) *= scale;
            else if (i < contributors[j].n0)
                break;
        }
    }

    // Optimize: Skip zero coefficients and contributions outside of image bounds.
    // Do this after normalizing because normalization depends on the n0/n1 values.
    for (j = 0; j < num_contributors; j++)
    {
        int range, max, width;

        skip = 0;
        while (*stbir__get_coefficient(coefficients, filter, scale_ratio, j, skip) == 0)
            skip++;

        contributors[j].n0 += skip;

        while (contributors[j].n0 < 0)
        {
            contributors[j].n0++;
            skip++;
        }

        range = contributors[j].n1 - contributors[j].n0 + 1;
        max = stbir__min(num_coefficients, range);

        width = stbir__get_coefficient_width(filter, scale_ratio);
        for (i = 0; i < max; i++)
        {
            if (i + skip >= width)
                break;

            *stbir__get_coefficient(coefficients, filter, scale_ratio, j, i) = *stbir__get_coefficient(coefficients, filter, scale_ratio, j, i + skip);
        }

        continue;
    }

    // Using min to avoid writing into invalid pixels.
    for (i = 0; i < num_contributors; i++)
        contributors[i].n1 = stbir__min(contributors[i].n1, output_size - 1);
}

// Each scan line uses the same kernel values so we should calculate the kernel
// values once and then we can use them for every scan line.
static void stbir__calculate_filters(stbir__contributors* contributors, float* coefficients, stbir_filter filter, float scale_ratio, float shift, int input_size, int output_size)
{
    int n;
    int total_contributors = stbir__get_contributors(scale_ratio, filter, input_size, output_size);

    if (stbir__use_upsampling(scale_ratio))
    {
        float out_pixels_radius = stbir__filter_info_table[filter].support(1 / scale_ratio) * scale_ratio;

        // Looping through out pixels
        for (n = 0; n < total_contributors; n++)
        {
            float in_center_of_out; // Center of the current out pixel in the in pixel space
            int in_first_pixel, in_last_pixel;

            stbir__calculate_sample_range_upsample(n, out_pixels_radius, scale_ratio, shift, &in_first_pixel, &in_last_pixel, &in_center_of_out);

            stbir__calculate_coefficients_upsample(filter, scale_ratio, in_first_pixel, in_last_pixel, in_center_of_out, stbir__get_contributor(contributors, n), stbir__get_coefficient(coefficients, filter, scale_ratio, n, 0));
        }
    }
    else
    {
        float in_pixels_radius = stbir__filter_info_table[filter].support(scale_ratio) / scale_ratio;

        // Looping through in pixels
        for (n = 0; n < total_contributors; n++)
        {
            float out_center_of_in; // Center of the current out pixel in the in pixel space
            int out_first_pixel, out_last_pixel;
            int n_adjusted = n - stbir__get_filter_pixel_margin(filter, scale_ratio);

            stbir__calculate_sample_range_downsample(n_adjusted, in_pixels_radius, scale_ratio, shift, &out_first_pixel, &out_last_pixel, &out_center_of_in);

            stbir__calculate_coefficients_downsample(filter, scale_ratio, out_first_pixel, out_last_pixel, out_center_of_in, stbir__get_contributor(contributors, n), stbir__get_coefficient(coefficients, filter, scale_ratio, n, 0));
        }

        stbir__normalize_downsample_coefficients(contributors, coefficients, filter, scale_ratio, input_size, output_size);
    }
}

static float* stbir__get_decode_buffer(stbir__info* stbir_info)
{
    // The 0 index of the decode buffer starts after the margin. This makes
    // it okay to use negative indexes on the decode buffer.
    return &stbir_info->decode_buffer[stbir_info->horizontal_filter_pixel_margin * stbir_info->channels];
}

#define STBIR__DECODE(type, colorspace) ((int)(type) * (STBIR_MAX_COLORSPACES) + (int)(colorspace))

static void stbir__decode_scanline(stbir__info* stbir_info, int n)
{
    int c;
    int channels = stbir_info->channels;
    int alpha_channel = stbir_info->alpha_channel;
    int type = stbir_info->type;
    int colorspace = stbir_info->colorspace;
    int input_w = stbir_info->input_w;
    size_t input_stride_bytes = stbir_info->input_stride_bytes;
    float* decode_buffer = stbir__get_decode_buffer(stbir_info);
    stbir_edge edge_horizontal = stbir_info->edge_horizontal;
    stbir_edge edge_vertical = stbir_info->edge_vertical;
    size_t in_buffer_row_offset = stbir__edge_wrap(edge_vertical, n, stbir_info->input_h) * input_stride_bytes;
    const void* input_data = (char*)stbir_info->input_data + in_buffer_row_offset;
    int max_x = input_w + stbir_info->horizontal_filter_pixel_margin;
    int decode = STBIR__DECODE(type, colorspace);

    int x = -stbir_info->horizontal_filter_pixel_margin;

    // special handling for STBIR_EDGE_ZERO because it needs to return an item that doesn't appear in the input,
    // and we want to avoid paying overhead on every pixel if not STBIR_EDGE_ZERO
    if (edge_vertical == STBIR_EDGE_ZERO && (n < 0 || n >= stbir_info->input_h))
    {
        for (; x < max_x; x++)
            for (c = 0; c < channels; c++)
                decode_buffer[x * channels + c] = 0;
        return;
    }

    switch (decode)
    {
    case STBIR__DECODE(STBIR_TYPE_UINT8, STBIR_COLORSPACE_LINEAR):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = ((float)((const unsigned char*)input_data)[input_pixel_index + c]) / stbir__max_uint8_as_float;
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT8, STBIR_COLORSPACE_SRGB):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = stbir__srgb_uchar_to_linear_float[((const unsigned char*)input_data)[input_pixel_index + c]];

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                decode_buffer[decode_pixel_index + alpha_channel] = ((float)((const unsigned char*)input_data)[input_pixel_index + alpha_channel]) / stbir__max_uint8_as_float;
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT16, STBIR_COLORSPACE_LINEAR):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = ((float)((const unsigned short*)input_data)[input_pixel_index + c]) / stbir__max_uint16_as_float;
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT16, STBIR_COLORSPACE_SRGB):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = stbir__srgb_to_linear(((float)((const unsigned short*)input_data)[input_pixel_index + c]) / stbir__max_uint16_as_float);

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                decode_buffer[decode_pixel_index + alpha_channel] = ((float)((const unsigned short*)input_data)[input_pixel_index + alpha_channel]) / stbir__max_uint16_as_float;
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT32, STBIR_COLORSPACE_LINEAR):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = (float)(((double)((const unsigned int*)input_data)[input_pixel_index + c]) / stbir__max_uint32_as_float);
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT32, STBIR_COLORSPACE_SRGB):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = stbir__srgb_to_linear((float)(((double)((const unsigned int*)input_data)[input_pixel_index + c]) / stbir__max_uint32_as_float));

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                decode_buffer[decode_pixel_index + alpha_channel] = (float)(((double)((const unsigned int*)input_data)[input_pixel_index + alpha_channel]) / stbir__max_uint32_as_float);
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_FLOAT, STBIR_COLORSPACE_LINEAR):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = ((const float*)input_data)[input_pixel_index + c];
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_FLOAT, STBIR_COLORSPACE_SRGB):
        for (; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;
            int input_pixel_index = stbir__edge_wrap(edge_horizontal, x, input_w) * channels;
            for (c = 0; c < channels; c++)
                decode_buffer[decode_pixel_index + c] = stbir__srgb_to_linear(((const float*)input_data)[input_pixel_index + c]);

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                decode_buffer[decode_pixel_index + alpha_channel] = ((const float*)input_data)[input_pixel_index + alpha_channel];
        }

        break;

    default:
        STBIR_ASSERT(!"Unknown type/colorspace/channels combination.");
        break;
    }

    if (!(stbir_info->flags & STBIR_FLAG_ALPHA_PREMULTIPLIED))
    {
        for (x = -stbir_info->horizontal_filter_pixel_margin; x < max_x; x++)
        {
            int decode_pixel_index = x * channels;

            // If the alpha value is 0 it will clobber the color values. Make sure it's not.
            float alpha = decode_buffer[decode_pixel_index + alpha_channel];
#ifndef STBIR_NO_ALPHA_EPSILON
            if (stbir_info->type != STBIR_TYPE_FLOAT) {
                alpha += STBIR_ALPHA_EPSILON;
                decode_buffer[decode_pixel_index + alpha_channel] = alpha;
            }
#endif
            for (c = 0; c < channels; c++)
            {
                if (c == alpha_channel)
                    continue;

                decode_buffer[decode_pixel_index + c] *= alpha;
            }
        }
    }

    if (edge_horizontal == STBIR_EDGE_ZERO)
    {
        for (x = -stbir_info->horizontal_filter_pixel_margin; x < 0; x++)
        {
            for (c = 0; c < channels; c++)
                decode_buffer[x * channels + c] = 0;
        }
        for (x = input_w; x < max_x; x++)
        {
            for (c = 0; c < channels; c++)
                decode_buffer[x * channels + c] = 0;
        }
    }
}

static float* stbir__get_ring_buffer_entry(float* ring_buffer, int index, int ring_buffer_length)
{
    return &ring_buffer[index * ring_buffer_length];
}

static float* stbir__add_empty_ring_buffer_entry(stbir__info* stbir_info, int n)
{
    int ring_buffer_index;
    float* ring_buffer;

    stbir_info->ring_buffer_last_scanline = n;

    if (stbir_info->ring_buffer_begin_index < 0)
    {
        ring_buffer_index = stbir_info->ring_buffer_begin_index = 0;
        stbir_info->ring_buffer_first_scanline = n;
    }
    else
    {
        ring_buffer_index = (stbir_info->ring_buffer_begin_index + (stbir_info->ring_buffer_last_scanline - stbir_info->ring_buffer_first_scanline)) % stbir_info->ring_buffer_num_entries;
        STBIR_ASSERT(ring_buffer_index != stbir_info->ring_buffer_begin_index);
    }

    ring_buffer = stbir__get_ring_buffer_entry(stbir_info->ring_buffer, ring_buffer_index, stbir_info->ring_buffer_length_bytes / sizeof(float));
    memset(ring_buffer, 0, stbir_info->ring_buffer_length_bytes);

    return ring_buffer;
}


static void stbir__resample_horizontal_upsample(stbir__info* stbir_info, float* output_buffer)
{
    int x, k;
    int output_w = stbir_info->output_w;
    int channels = stbir_info->channels;
    float* decode_buffer = stbir__get_decode_buffer(stbir_info);
    stbir__contributors* horizontal_contributors = stbir_info->horizontal_contributors;
    float* horizontal_coefficients = stbir_info->horizontal_coefficients;
    int coefficient_width = stbir_info->horizontal_coefficient_width;

    for (x = 0; x < output_w; x++)
    {
        int n0 = horizontal_contributors[x].n0;
        int n1 = horizontal_contributors[x].n1;

        int out_pixel_index = x * channels;
        int coefficient_group = coefficient_width * x;
        int coefficient_counter = 0;

        STBIR_ASSERT(n1 >= n0);
        STBIR_ASSERT(n0 >= -stbir_info->horizontal_filter_pixel_margin);
        STBIR_ASSERT(n1 >= -stbir_info->horizontal_filter_pixel_margin);
        STBIR_ASSERT(n0 < stbir_info->input_w + stbir_info->horizontal_filter_pixel_margin);
        STBIR_ASSERT(n1 < stbir_info->input_w + stbir_info->horizontal_filter_pixel_margin);

        switch (channels) {
        case 1:
            for (k = n0; k <= n1; k++)
            {
                int in_pixel_index = k * 1;
                float coefficient = horizontal_coefficients[coefficient_group + coefficient_counter++];
                STBIR_ASSERT(coefficient != 0);
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
            }
            break;
        case 2:
            for (k = n0; k <= n1; k++)
            {
                int in_pixel_index = k * 2;
                float coefficient = horizontal_coefficients[coefficient_group + coefficient_counter++];
                STBIR_ASSERT(coefficient != 0);
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
                output_buffer[out_pixel_index + 1] += decode_buffer[in_pixel_index + 1] * coefficient;
            }
            break;
        case 3:
            for (k = n0; k <= n1; k++)
            {
                int in_pixel_index = k * 3;
                float coefficient = horizontal_coefficients[coefficient_group + coefficient_counter++];
                STBIR_ASSERT(coefficient != 0);
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
                output_buffer[out_pixel_index + 1] += decode_buffer[in_pixel_index + 1] * coefficient;
                output_buffer[out_pixel_index + 2] += decode_buffer[in_pixel_index + 2] * coefficient;
            }
            break;
        case 4:
            for (k = n0; k <= n1; k++)
            {
                int in_pixel_index = k * 4;
                float coefficient = horizontal_coefficients[coefficient_group + coefficient_counter++];
                STBIR_ASSERT(coefficient != 0);
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
                output_buffer[out_pixel_index + 1] += decode_buffer[in_pixel_index + 1] * coefficient;
                output_buffer[out_pixel_index + 2] += decode_buffer[in_pixel_index + 2] * coefficient;
                output_buffer[out_pixel_index + 3] += decode_buffer[in_pixel_index + 3] * coefficient;
            }
            break;
        default:
            for (k = n0; k <= n1; k++)
            {
                int in_pixel_index = k * channels;
                float coefficient = horizontal_coefficients[coefficient_group + coefficient_counter++];
                int c;
                STBIR_ASSERT(coefficient != 0);
                for (c = 0; c < channels; c++)
                    output_buffer[out_pixel_index + c] += decode_buffer[in_pixel_index + c] * coefficient;
            }
            break;
        }
    }
}

static void stbir__resample_horizontal_downsample(stbir__info* stbir_info, float* output_buffer)
{
    int x, k;
    int input_w = stbir_info->input_w;
    int channels = stbir_info->channels;
    float* decode_buffer = stbir__get_decode_buffer(stbir_info);
    stbir__contributors* horizontal_contributors = stbir_info->horizontal_contributors;
    float* horizontal_coefficients = stbir_info->horizontal_coefficients;
    int coefficient_width = stbir_info->horizontal_coefficient_width;
    int filter_pixel_margin = stbir_info->horizontal_filter_pixel_margin;
    int max_x = input_w + filter_pixel_margin * 2;

    STBIR_ASSERT(!stbir__use_width_upsampling(stbir_info));

    switch (channels) {
    case 1:
        for (x = 0; x < max_x; x++)
        {
            int n0 = horizontal_contributors[x].n0;
            int n1 = horizontal_contributors[x].n1;

            int in_x = x - filter_pixel_margin;
            int in_pixel_index = in_x * 1;
            int max_n = n1;
            int coefficient_group = coefficient_width * x;

            for (k = n0; k <= max_n; k++)
            {
                int out_pixel_index = k * 1;
                float coefficient = horizontal_coefficients[coefficient_group + k - n0];
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
            }
        }
        break;

    case 2:
        for (x = 0; x < max_x; x++)
        {
            int n0 = horizontal_contributors[x].n0;
            int n1 = horizontal_contributors[x].n1;

            int in_x = x - filter_pixel_margin;
            int in_pixel_index = in_x * 2;
            int max_n = n1;
            int coefficient_group = coefficient_width * x;

            for (k = n0; k <= max_n; k++)
            {
                int out_pixel_index = k * 2;
                float coefficient = horizontal_coefficients[coefficient_group + k - n0];
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
                output_buffer[out_pixel_index + 1] += decode_buffer[in_pixel_index + 1] * coefficient;
            }
        }
        break;

    case 3:
        for (x = 0; x < max_x; x++)
        {
            int n0 = horizontal_contributors[x].n0;
            int n1 = horizontal_contributors[x].n1;

            int in_x = x - filter_pixel_margin;
            int in_pixel_index = in_x * 3;
            int max_n = n1;
            int coefficient_group = coefficient_width * x;

            for (k = n0; k <= max_n; k++)
            {
                int out_pixel_index = k * 3;
                float coefficient = horizontal_coefficients[coefficient_group + k - n0];
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
                output_buffer[out_pixel_index + 1] += decode_buffer[in_pixel_index + 1] * coefficient;
                output_buffer[out_pixel_index + 2] += decode_buffer[in_pixel_index + 2] * coefficient;
            }
        }
        break;

    case 4:
        for (x = 0; x < max_x; x++)
        {
            int n0 = horizontal_contributors[x].n0;
            int n1 = horizontal_contributors[x].n1;

            int in_x = x - filter_pixel_margin;
            int in_pixel_index = in_x * 4;
            int max_n = n1;
            int coefficient_group = coefficient_width * x;

            for (k = n0; k <= max_n; k++)
            {
                int out_pixel_index = k * 4;
                float coefficient = horizontal_coefficients[coefficient_group + k - n0];
                output_buffer[out_pixel_index + 0] += decode_buffer[in_pixel_index + 0] * coefficient;
                output_buffer[out_pixel_index + 1] += decode_buffer[in_pixel_index + 1] * coefficient;
                output_buffer[out_pixel_index + 2] += decode_buffer[in_pixel_index + 2] * coefficient;
                output_buffer[out_pixel_index + 3] += decode_buffer[in_pixel_index + 3] * coefficient;
            }
        }
        break;

    default:
        for (x = 0; x < max_x; x++)
        {
            int n0 = horizontal_contributors[x].n0;
            int n1 = horizontal_contributors[x].n1;

            int in_x = x - filter_pixel_margin;
            int in_pixel_index = in_x * channels;
            int max_n = n1;
            int coefficient_group = coefficient_width * x;

            for (k = n0; k <= max_n; k++)
            {
                int c;
                int out_pixel_index = k * channels;
                float coefficient = horizontal_coefficients[coefficient_group + k - n0];
                for (c = 0; c < channels; c++)
                    output_buffer[out_pixel_index + c] += decode_buffer[in_pixel_index + c] * coefficient;
            }
        }
        break;
    }
}

static void stbir__decode_and_resample_upsample(stbir__info* stbir_info, int n)
{
    // Decode the nth scanline from the source image into the decode buffer.
    stbir__decode_scanline(stbir_info, n);

    // Now resample it into the ring buffer.
    if (stbir__use_width_upsampling(stbir_info))
        stbir__resample_horizontal_upsample(stbir_info, stbir__add_empty_ring_buffer_entry(stbir_info, n));
    else
        stbir__resample_horizontal_downsample(stbir_info, stbir__add_empty_ring_buffer_entry(stbir_info, n));

    // Now it's sitting in the ring buffer ready to be used as source for the vertical sampling.
}

static void stbir__decode_and_resample_downsample(stbir__info* stbir_info, int n)
{
    // Decode the nth scanline from the source image into the decode buffer.
    stbir__decode_scanline(stbir_info, n);

    memset(stbir_info->horizontal_buffer, 0, stbir_info->output_w * stbir_info->channels * sizeof(float));

    // Now resample it into the horizontal buffer.
    if (stbir__use_width_upsampling(stbir_info))
        stbir__resample_horizontal_upsample(stbir_info, stbir_info->horizontal_buffer);
    else
        stbir__resample_horizontal_downsample(stbir_info, stbir_info->horizontal_buffer);

    // Now it's sitting in the horizontal buffer ready to be distributed into the ring buffers.
}

// Get the specified scan line from the ring buffer.
static float* stbir__get_ring_buffer_scanline(int get_scanline, float* ring_buffer, int begin_index, int first_scanline, int ring_buffer_num_entries, int ring_buffer_length)
{
    int ring_buffer_index = (begin_index + (get_scanline - first_scanline)) % ring_buffer_num_entries;
    return stbir__get_ring_buffer_entry(ring_buffer, ring_buffer_index, ring_buffer_length);
}


static void stbir__encode_scanline(stbir__info* stbir_info, int num_pixels, void* output_buffer, float* encode_buffer, int channels, int alpha_channel, int decode)
{
    int x;
    int n;
    int num_nonalpha;
    stbir_uint16 nonalpha[STBIR_MAX_CHANNELS];

    if (!(stbir_info->flags & STBIR_FLAG_ALPHA_PREMULTIPLIED))
    {
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            float alpha = encode_buffer[pixel_index + alpha_channel];
            float reciprocal_alpha = alpha ? 1.0f / alpha : 0;

            // unrolling this produced a 1% slowdown upscaling a large RGBA linear-space image on my machine - stb
            for (n = 0; n < channels; n++)
                if (n != alpha_channel)
                    encode_buffer[pixel_index + n] *= reciprocal_alpha;

            // We added in a small epsilon to prevent the color channel from being deleted with zero alpha.
            // Because we only add it for integer types, it will automatically be discarded on integer
            // conversion, so we don't need to subtract it back out (which would be problematic for
            // numeric precision reasons).
        }
    }

    // build a table of all channels that need colorspace correction, so
    // we don't perform colorspace correction on channels that don't need it.
    for (x = 0, num_nonalpha = 0; x < channels; ++x)
    {
        if (x != alpha_channel || (stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
        {
            nonalpha[num_nonalpha++] = (stbir_uint16)x;
        }
    }

#define STBIR__ROUND_INT(f)    ((int)          ((f)+0.5))
#define STBIR__ROUND_UINT(f)   ((stbir_uint32) ((f)+0.5))

#ifdef STBIR__SATURATE_INT
#define STBIR__ENCODE_LINEAR8(f)   stbir__saturate8 (STBIR__ROUND_INT((f) * stbir__max_uint8_as_float ))
#define STBIR__ENCODE_LINEAR16(f)  stbir__saturate16(STBIR__ROUND_INT((f) * stbir__max_uint16_as_float))
#else
#define STBIR__ENCODE_LINEAR8(f)   (unsigned char ) STBIR__ROUND_INT(stbir__saturate(f) * stbir__max_uint8_as_float )
#define STBIR__ENCODE_LINEAR16(f)  (unsigned short) STBIR__ROUND_INT(stbir__saturate(f) * stbir__max_uint16_as_float)
#endif

    switch (decode)
    {
    case STBIR__DECODE(STBIR_TYPE_UINT8, STBIR_COLORSPACE_LINEAR):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < channels; n++)
            {
                int index = pixel_index + n;
                ((unsigned char*)output_buffer)[index] = STBIR__ENCODE_LINEAR8(encode_buffer[index]);
            }
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT8, STBIR_COLORSPACE_SRGB):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < num_nonalpha; n++)
            {
                int index = pixel_index + nonalpha[n];
                ((unsigned char*)output_buffer)[index] = stbir__linear_to_srgb_uchar(encode_buffer[index]);
            }

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                ((unsigned char*)output_buffer)[pixel_index + alpha_channel] = STBIR__ENCODE_LINEAR8(encode_buffer[pixel_index + alpha_channel]);
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT16, STBIR_COLORSPACE_LINEAR):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < channels; n++)
            {
                int index = pixel_index + n;
                ((unsigned short*)output_buffer)[index] = STBIR__ENCODE_LINEAR16(encode_buffer[index]);
            }
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT16, STBIR_COLORSPACE_SRGB):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < num_nonalpha; n++)
            {
                int index = pixel_index + nonalpha[n];
                ((unsigned short*)output_buffer)[index] = (unsigned short)STBIR__ROUND_INT(stbir__linear_to_srgb(stbir__saturate(encode_buffer[index])) * stbir__max_uint16_as_float);
            }

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                ((unsigned short*)output_buffer)[pixel_index + alpha_channel] = STBIR__ENCODE_LINEAR16(encode_buffer[pixel_index + alpha_channel]);
        }

        break;

    case STBIR__DECODE(STBIR_TYPE_UINT32, STBIR_COLORSPACE_LINEAR):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < channels; n++)
            {
                int index = pixel_index + n;
                ((unsigned int*)output_buffer)[index] = (unsigned int)STBIR__ROUND_UINT(((double)stbir__saturate(encode_buffer[index])) * stbir__max_uint32_as_float);
            }
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_UINT32, STBIR_COLORSPACE_SRGB):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < num_nonalpha; n++)
            {
                int index = pixel_index + nonalpha[n];
                ((unsigned int*)output_buffer)[index] = (unsigned int)STBIR__ROUND_UINT(((double)stbir__linear_to_srgb(stbir__saturate(encode_buffer[index]))) * stbir__max_uint32_as_float);
            }

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                ((unsigned int*)output_buffer)[pixel_index + alpha_channel] = (unsigned int)STBIR__ROUND_INT(((double)stbir__saturate(encode_buffer[pixel_index + alpha_channel])) * stbir__max_uint32_as_float);
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_FLOAT, STBIR_COLORSPACE_LINEAR):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < channels; n++)
            {
                int index = pixel_index + n;
                ((float*)output_buffer)[index] = encode_buffer[index];
            }
        }
        break;

    case STBIR__DECODE(STBIR_TYPE_FLOAT, STBIR_COLORSPACE_SRGB):
        for (x = 0; x < num_pixels; ++x)
        {
            int pixel_index = x * channels;

            for (n = 0; n < num_nonalpha; n++)
            {
                int index = pixel_index + nonalpha[n];
                ((float*)output_buffer)[index] = stbir__linear_to_srgb(encode_buffer[index]);
            }

            if (!(stbir_info->flags & STBIR_FLAG_ALPHA_USES_COLORSPACE))
                ((float*)output_buffer)[pixel_index + alpha_channel] = encode_buffer[pixel_index + alpha_channel];
        }
        break;

    default:
        STBIR_ASSERT(!"Unknown type/colorspace/channels combination.");
        break;
    }
}

static void stbir__resample_vertical_upsample(stbir__info* stbir_info, int n)
{
    int x, k;
    int output_w = stbir_info->output_w;
    stbir__contributors* vertical_contributors = stbir_info->vertical_contributors;
    float* vertical_coefficients = stbir_info->vertical_coefficients;
    int channels = stbir_info->channels;
    int alpha_channel = stbir_info->alpha_channel;
    int type = stbir_info->type;
    int colorspace = stbir_info->colorspace;
    int ring_buffer_entries = stbir_info->ring_buffer_num_entries;
    void* output_data = stbir_info->output_data;
    float* encode_buffer = stbir_info->encode_buffer;
    int decode = STBIR__DECODE(type, colorspace);
    int coefficient_width = stbir_info->vertical_coefficient_width;
    int coefficient_counter;
    int contributor = n;

    float* ring_buffer = stbir_info->ring_buffer;
    int ring_buffer_begin_index = stbir_info->ring_buffer_begin_index;
    int ring_buffer_first_scanline = stbir_info->ring_buffer_first_scanline;
    int ring_buffer_length = stbir_info->ring_buffer_length_bytes / sizeof(float);

    int n0, n1, output_row_start;
    int coefficient_group = coefficient_width * contributor;

    n0 = vertical_contributors[contributor].n0;
    n1 = vertical_contributors[contributor].n1;

    output_row_start = n * stbir_info->output_stride_bytes;

    STBIR_ASSERT(stbir__use_height_upsampling(stbir_info));

    memset(encode_buffer, 0, output_w * sizeof(float) * channels);

    // I tried reblocking this for better cache usage of encode_buffer
    // (using x_outer, k, x_inner), but it lost speed. -- stb

    coefficient_counter = 0;
    switch (channels) {
    case 1:
        for (k = n0; k <= n1; k++)
        {
            int coefficient_index = coefficient_counter++;
            float* ring_buffer_entry = stbir__get_ring_buffer_scanline(k, ring_buffer, ring_buffer_begin_index, ring_buffer_first_scanline, ring_buffer_entries, ring_buffer_length);
            float coefficient = vertical_coefficients[coefficient_group + coefficient_index];
            for (x = 0; x < output_w; ++x)
            {
                int in_pixel_index = x * 1;
                encode_buffer[in_pixel_index + 0] += ring_buffer_entry[in_pixel_index + 0] * coefficient;
            }
        }
        break;
    case 2:
        for (k = n0; k <= n1; k++)
        {
            int coefficient_index = coefficient_counter++;
            float* ring_buffer_entry = stbir__get_ring_buffer_scanline(k, ring_buffer, ring_buffer_begin_index, ring_buffer_first_scanline, ring_buffer_entries, ring_buffer_length);
            float coefficient = vertical_coefficients[coefficient_group + coefficient_index];
            for (x = 0; x < output_w; ++x)
            {
                int in_pixel_index = x * 2;
                encode_buffer[in_pixel_index + 0] += ring_buffer_entry[in_pixel_index + 0] * coefficient;
                encode_buffer[in_pixel_index + 1] += ring_buffer_entry[in_pixel_index + 1] * coefficient;
            }
        }
        break;
    case 3:
        for (k = n0; k <= n1; k++)
        {
            int coefficient_index = coefficient_counter++;
            float* ring_buffer_entry = stbir__get_ring_buffer_scanline(k, ring_buffer, ring_buffer_begin_index, ring_buffer_first_scanline, ring_buffer_entries, ring_buffer_length);
            float coefficient = vertical_coefficients[coefficient_group + coefficient_index];
            for (x = 0; x < output_w; ++x)
            {
                int in_pixel_index = x * 3;
                encode_buffer[in_pixel_index + 0] += ring_buffer_entry[in_pixel_index + 0] * coefficient;
                encode_buffer[in_pixel_index + 1] += ring_buffer_entry[in_pixel_index + 1] * coefficient;
                encode_buffer[in_pixel_index + 2] += ring_buffer_entry[in_pixel_index + 2] * coefficient;
            }
        }
        break;
    case 4:
        for (k = n0; k <= n1; k++)
        {
            int coefficient_index = coefficient_counter++;
            float* ring_buffer_entry = stbir__get_ring_buffer_scanline(k, ring_buffer, ring_buffer_begin_index, ring_buffer_first_scanline, ring_buffer_entries, ring_buffer_length);
            float coefficient = vertical_coefficients[coefficient_group + coefficient_index];
            for (x = 0; x < output_w; ++x)
            {
                int in_pixel_index = x * 4;
                encode_buffer[in_pixel_index + 0] += ring_buffer_entry[in_pixel_index + 0] * coefficient;
                encode_buffer[in_pixel_index + 1] += ring_buffer_entry[in_pixel_index + 1] * coefficient;
                encode_buffer[in_pixel_index + 2] += ring_buffer_entry[in_pixel_index + 2] * coefficient;
                encode_buffer[in_pixel_index + 3] += ring_buffer_entry[in_pixel_index + 3] * coefficient;
            }
        }
        break;
    default:
        for (k = n0; k <= n1; k++)
        {
            int coefficient_index = coefficient_counter++;
            float* ring_buffer_entry = stbir__get_ring_buffer_scanline(k, ring_buffer, ring_buffer_begin_index, ring_buffer_first_scanline, ring_buffer_entries, ring_buffer_length);
            float coefficient = vertical_coefficients[coefficient_group + coefficient_index];
            for (x = 0; x < output_w; ++x)
            {
                int in_pixel_index = x * channels;
                int c;
                for (c = 0; c < channels; c++)
                    encode_buffer[in_pixel_index + c] += ring_buffer_entry[in_pixel_index + c] * coefficient;
            }
        }
        break;
    }
    stbir__encode_scanline(stbir_info, output_w, (char*)output_data + output_row_start, encode_buffer, channels, alpha_channel, decode);
}

static void stbir__resample_vertical_downsample(stbir__info* stbir_info, int n)
{
    int x, k;
    int output_w = stbir_info->output_w;
    stbir__contributors* vertical_contributors = stbir_info->vertical_contributors;
    float* vertical_coefficients = stbir_info->vertical_coefficients;
    int channels = stbir_info->channels;
    int ring_buffer_entries = stbir_info->ring_buffer_num_entries;
    float* horizontal_buffer = stbir_info->horizontal_buffer;
    int coefficient_width = stbir_info->vertical_coefficient_width;
    int contributor = n + stbir_info->vertical_filter_pixel_margin;

    float* ring_buffer = stbir_info->ring_buffer;
    int ring_buffer_begin_index = stbir_info->ring_buffer_begin_index;
    int ring_buffer_first_scanline = stbir_info->ring_buffer_first_scanline;
    int ring_buffer_length = stbir_info->ring_buffer_length_bytes / sizeof(float);
    int n0, n1;

    n0 = vertical_contributors[contributor].n0;
    n1 = vertical_contributors[contributor].n1;

    STBIR_ASSERT(!stbir__use_height_upsampling(stbir_info));

    for (k = n0; k <= n1; k++)
    {
        int coefficient_index = k - n0;
        int coefficient_group = coefficient_width * contributor;
        float coefficient = vertical_coefficients[coefficient_group + coefficient_index];

        float* ring_buffer_entry = stbir__get_ring_buffer_scanline(k, ring_buffer, ring_buffer_begin_index, ring_buffer_first_scanline, ring_buffer_entries, ring_buffer_length);

        switch (channels) {
        case 1:
            for (x = 0; x < output_w; x++)
            {
                int in_pixel_index = x * 1;
                ring_buffer_entry[in_pixel_index + 0] += horizontal_buffer[in_pixel_index + 0] * coefficient;
            }
            break;
        case 2:
            for (x = 0; x < output_w; x++)
            {
                int in_pixel_index = x * 2;
                ring_buffer_entry[in_pixel_index + 0] += horizontal_buffer[in_pixel_index + 0] * coefficient;
                ring_buffer_entry[in_pixel_index + 1] += horizontal_buffer[in_pixel_index + 1] * coefficient;
            }
            break;
        case 3:
            for (x = 0; x < output_w; x++)
            {
                int in_pixel_index = x * 3;
                ring_buffer_entry[in_pixel_index + 0] += horizontal_buffer[in_pixel_index + 0] * coefficient;
                ring_buffer_entry[in_pixel_index + 1] += horizontal_buffer[in_pixel_index + 1] * coefficient;
                ring_buffer_entry[in_pixel_index + 2] += horizontal_buffer[in_pixel_index + 2] * coefficient;
            }
            break;
        case 4:
            for (x = 0; x < output_w; x++)
            {
                int in_pixel_index = x * 4;
                ring_buffer_entry[in_pixel_index + 0] += horizontal_buffer[in_pixel_index + 0] * coefficient;
                ring_buffer_entry[in_pixel_index + 1] += horizontal_buffer[in_pixel_index + 1] * coefficient;
                ring_buffer_entry[in_pixel_index + 2] += horizontal_buffer[in_pixel_index + 2] * coefficient;
                ring_buffer_entry[in_pixel_index + 3] += horizontal_buffer[in_pixel_index + 3] * coefficient;
            }
            break;
        default:
            for (x = 0; x < output_w; x++)
            {
                int in_pixel_index = x * channels;

                int c;
                for (c = 0; c < channels; c++)
                    ring_buffer_entry[in_pixel_index + c] += horizontal_buffer[in_pixel_index + c] * coefficient;
            }
            break;
        }
    }
}

static void stbir__buffer_loop_upsample(stbir__info* stbir_info)
{
    int y;
    float scale_ratio = stbir_info->vertical_scale;
    float out_scanlines_radius = stbir__filter_info_table[stbir_info->vertical_filter].support(1 / scale_ratio) * scale_ratio;

    STBIR_ASSERT(stbir__use_height_upsampling(stbir_info));

    for (y = 0; y < stbir_info->output_h; y++)
    {
        float in_center_of_out = 0; // Center of the current out scanline in the in scanline space
        int in_first_scanline = 0, in_last_scanline = 0;

        stbir__calculate_sample_range_upsample(y, out_scanlines_radius, scale_ratio, stbir_info->vertical_shift, &in_first_scanline, &in_last_scanline, &in_center_of_out);

        STBIR_ASSERT(in_last_scanline - in_first_scanline + 1 <= stbir_info->ring_buffer_num_entries);

        if (stbir_info->ring_buffer_begin_index >= 0)
        {
            // Get rid of whatever we don't need anymore.
            while (in_first_scanline > stbir_info->ring_buffer_first_scanline)
            {
                if (stbir_info->ring_buffer_first_scanline == stbir_info->ring_buffer_last_scanline)
                {
                    // We just popped the last scanline off the ring buffer.
                    // Reset it to the empty state.
                    stbir_info->ring_buffer_begin_index = -1;
                    stbir_info->ring_buffer_first_scanline = 0;
                    stbir_info->ring_buffer_last_scanline = 0;
                    break;
                }
                else
                {
                    stbir_info->ring_buffer_first_scanline++;
                    stbir_info->ring_buffer_begin_index = (stbir_info->ring_buffer_begin_index + 1) % stbir_info->ring_buffer_num_entries;
                }
            }
        }

        // Load in new ones.
        if (stbir_info->ring_buffer_begin_index < 0)
            stbir__decode_and_resample_upsample(stbir_info, in_first_scanline);

        while (in_last_scanline > stbir_info->ring_buffer_last_scanline)
            stbir__decode_and_resample_upsample(stbir_info, stbir_info->ring_buffer_last_scanline + 1);

        // Now all buffers should be ready to write a row of vertical sampling.
        stbir__resample_vertical_upsample(stbir_info, y);

        STBIR_PROGRESS_REPORT((float)y / stbir_info->output_h);
    }
}

static void stbir__empty_ring_buffer(stbir__info* stbir_info, int first_necessary_scanline)
{
    int output_stride_bytes = stbir_info->output_stride_bytes;
    int channels = stbir_info->channels;
    int alpha_channel = stbir_info->alpha_channel;
    int type = stbir_info->type;
    int colorspace = stbir_info->colorspace;
    int output_w = stbir_info->output_w;
    void* output_data = stbir_info->output_data;
    int decode = STBIR__DECODE(type, colorspace);

    float* ring_buffer = stbir_info->ring_buffer;
    int ring_buffer_length = stbir_info->ring_buffer_length_bytes / sizeof(float);

    if (stbir_info->ring_buffer_begin_index >= 0)
    {
        // Get rid of whatever we don't need anymore.
        while (first_necessary_scanline > stbir_info->ring_buffer_first_scanline)
        {
            if (stbir_info->ring_buffer_first_scanline >= 0 && stbir_info->ring_buffer_first_scanline < stbir_info->output_h)
            {
                int output_row_start = stbir_info->ring_buffer_first_scanline * output_stride_bytes;
                float* ring_buffer_entry = stbir__get_ring_buffer_entry(ring_buffer, stbir_info->ring_buffer_begin_index, ring_buffer_length);
                stbir__encode_scanline(stbir_info, output_w, (char*)output_data + output_row_start, ring_buffer_entry, channels, alpha_channel, decode);
                STBIR_PROGRESS_REPORT((float)stbir_info->ring_buffer_first_scanline / stbir_info->output_h);
            }

            if (stbir_info->ring_buffer_first_scanline == stbir_info->ring_buffer_last_scanline)
            {
                // We just popped the last scanline off the ring buffer.
                // Reset it to the empty state.
                stbir_info->ring_buffer_begin_index = -1;
                stbir_info->ring_buffer_first_scanline = 0;
                stbir_info->ring_buffer_last_scanline = 0;
                break;
            }
            else
            {
                stbir_info->ring_buffer_first_scanline++;
                stbir_info->ring_buffer_begin_index = (stbir_info->ring_buffer_begin_index + 1) % stbir_info->ring_buffer_num_entries;
            }
        }
    }
}

static void stbir__buffer_loop_downsample(stbir__info* stbir_info)
{
    int y;
    float scale_ratio = stbir_info->vertical_scale;
    int output_h = stbir_info->output_h;
    float in_pixels_radius = stbir__filter_info_table[stbir_info->vertical_filter].support(scale_ratio) / scale_ratio;
    int pixel_margin = stbir_info->vertical_filter_pixel_margin;
    int max_y = stbir_info->input_h + pixel_margin;

    STBIR_ASSERT(!stbir__use_height_upsampling(stbir_info));

    for (y = -pixel_margin; y < max_y; y++)
    {
        float out_center_of_in; // Center of the current out scanline in the in scanline space
        int out_first_scanline, out_last_scanline;

        stbir__calculate_sample_range_downsample(y, in_pixels_radius, scale_ratio, stbir_info->vertical_shift, &out_first_scanline, &out_last_scanline, &out_center_of_in);

        STBIR_ASSERT(out_last_scanline - out_first_scanline + 1 <= stbir_info->ring_buffer_num_entries);

        if (out_last_scanline < 0 || out_first_scanline >= output_h)
            continue;

        stbir__empty_ring_buffer(stbir_info, out_first_scanline);

        stbir__decode_and_resample_downsample(stbir_info, y);

        // Load in new ones.
        if (stbir_info->ring_buffer_begin_index < 0)
            stbir__add_empty_ring_buffer_entry(stbir_info, out_first_scanline);

        while (out_last_scanline > stbir_info->ring_buffer_last_scanline)
            stbir__add_empty_ring_buffer_entry(stbir_info, stbir_info->ring_buffer_last_scanline + 1);

        // Now the horizontal buffer is ready to write to all ring buffer rows.
        stbir__resample_vertical_downsample(stbir_info, y);
    }

    stbir__empty_ring_buffer(stbir_info, stbir_info->output_h);
}

static void stbir__setup(stbir__info* info, int input_w, int input_h, int output_w, int output_h, int channels)
{
    info->input_w = input_w;
    info->input_h = input_h;
    info->output_w = output_w;
    info->output_h = output_h;
    info->channels = channels;
}

static void stbir__calculate_transform(stbir__info* info, float s0, float t0, float s1, float t1, float* transform)
{
    info->s0 = s0;
    info->t0 = t0;
    info->s1 = s1;
    info->t1 = t1;

    if (transform)
    {
        info->horizontal_scale = transform[0];
        info->vertical_scale = transform[1];
        info->horizontal_shift = transform[2];
        info->vertical_shift = transform[3];
    }
    else
    {
        info->horizontal_scale = ((float)info->output_w / info->input_w) / (s1 - s0);
        info->vertical_scale = ((float)info->output_h / info->input_h) / (t1 - t0);

        info->horizontal_shift = s0 * info->output_w / (s1 - s0);
        info->vertical_shift = t0 * info->output_h / (t1 - t0);
    }
}

static void stbir__choose_filter(stbir__info* info, stbir_filter h_filter, stbir_filter v_filter)
{
    if (h_filter == 0)
        h_filter = stbir__use_upsampling(info->horizontal_scale) ? STBIR_DEFAULT_FILTER_UPSAMPLE : STBIR_DEFAULT_FILTER_DOWNSAMPLE;
    if (v_filter == 0)
        v_filter = stbir__use_upsampling(info->vertical_scale) ? STBIR_DEFAULT_FILTER_UPSAMPLE : STBIR_DEFAULT_FILTER_DOWNSAMPLE;
    info->horizontal_filter = h_filter;
    info->vertical_filter = v_filter;
}

static stbir_uint32 stbir__calculate_memory(stbir__info* info)
{
    int pixel_margin = stbir__get_filter_pixel_margin(info->horizontal_filter, info->horizontal_scale);
    int filter_height = stbir__get_filter_pixel_width(info->vertical_filter, info->vertical_scale);

    info->horizontal_num_contributors = stbir__get_contributors(info->horizontal_scale, info->horizontal_filter, info->input_w, info->output_w);
    info->vertical_num_contributors = stbir__get_contributors(info->vertical_scale, info->vertical_filter, info->input_h, info->output_h);

    // One extra entry because floating point precision problems sometimes cause an extra to be necessary.
    info->ring_buffer_num_entries = filter_height + 1;

    info->horizontal_contributors_size = info->horizontal_num_contributors * sizeof(stbir__contributors);
    info->horizontal_coefficients_size = stbir__get_total_horizontal_coefficients(info) * sizeof(float);
    info->vertical_contributors_size = info->vertical_num_contributors * sizeof(stbir__contributors);
    info->vertical_coefficients_size = stbir__get_total_vertical_coefficients(info) * sizeof(float);
    info->decode_buffer_size = (info->input_w + pixel_margin * 2) * info->channels * sizeof(float);
    info->horizontal_buffer_size = info->output_w * info->channels * sizeof(float);
    info->ring_buffer_size = info->output_w * info->channels * info->ring_buffer_num_entries * sizeof(float);
    info->encode_buffer_size = info->output_w * info->channels * sizeof(float);

    STBIR_ASSERT(info->horizontal_filter != 0);
    STBIR_ASSERT(info->horizontal_filter < STBIR__ARRAY_SIZE(stbir__filter_info_table)); // this now happens too late
    STBIR_ASSERT(info->vertical_filter != 0);
    STBIR_ASSERT(info->vertical_filter < STBIR__ARRAY_SIZE(stbir__filter_info_table)); // this now happens too late

    if (stbir__use_height_upsampling(info))
        // The horizontal buffer is for when we're downsampling the height and we
        // can't output the result of sampling the decode buffer directly into the
        // ring buffers.
        info->horizontal_buffer_size = 0;
    else
        // The encode buffer is to retain precision in the height upsampling method
        // and isn't used when height downsampling.
        info->encode_buffer_size = 0;

    return info->horizontal_contributors_size + info->horizontal_coefficients_size
        + info->vertical_contributors_size + info->vertical_coefficients_size
        + info->decode_buffer_size + info->horizontal_buffer_size
        + info->ring_buffer_size + info->encode_buffer_size;
}

static int stbir__resize_allocated(stbir__info* info,
    const void* input_data, int input_stride_in_bytes,
    void* output_data, int output_stride_in_bytes,
    int alpha_channel, stbir_uint32 flags, stbir_datatype type,
    stbir_edge edge_horizontal, stbir_edge edge_vertical, stbir_colorspace colorspace,
    void* tempmem, size_t tempmem_size_in_bytes)
{
    size_t memory_required = stbir__calculate_memory(info);

    int width_stride_input = input_stride_in_bytes ? input_stride_in_bytes : info->channels * info->input_w * stbir__type_size[type];
    int width_stride_output = output_stride_in_bytes ? output_stride_in_bytes : info->channels * info->output_w * stbir__type_size[type];

#ifdef STBIR_DEBUG_OVERWRITE_TEST
#define OVERWRITE_ARRAY_SIZE 8
    unsigned char overwrite_output_before_pre[OVERWRITE_ARRAY_SIZE];
    unsigned char overwrite_tempmem_before_pre[OVERWRITE_ARRAY_SIZE];
    unsigned char overwrite_output_after_pre[OVERWRITE_ARRAY_SIZE];
    unsigned char overwrite_tempmem_after_pre[OVERWRITE_ARRAY_SIZE];

    size_t begin_forbidden = width_stride_output * (info->output_h - 1) + info->output_w * info->channels * stbir__type_size[type];
    memcpy(overwrite_output_before_pre, &((unsigned char*)output_data)[-OVERWRITE_ARRAY_SIZE], OVERWRITE_ARRAY_SIZE);
    memcpy(overwrite_output_after_pre, &((unsigned char*)output_data)[begin_forbidden], OVERWRITE_ARRAY_SIZE);
    memcpy(overwrite_tempmem_before_pre, &((unsigned char*)tempmem)[-OVERWRITE_ARRAY_SIZE], OVERWRITE_ARRAY_SIZE);
    memcpy(overwrite_tempmem_after_pre, &((unsigned char*)tempmem)[tempmem_size_in_bytes], OVERWRITE_ARRAY_SIZE);
#endif

    STBIR_ASSERT(info->channels >= 0);
    STBIR_ASSERT(info->channels <= STBIR_MAX_CHANNELS);

    if (info->channels < 0 || info->channels > STBIR_MAX_CHANNELS)
        return 0;

    STBIR_ASSERT(info->horizontal_filter < STBIR__ARRAY_SIZE(stbir__filter_info_table));
    STBIR_ASSERT(info->vertical_filter < STBIR__ARRAY_SIZE(stbir__filter_info_table));

    if (info->horizontal_filter >= STBIR__ARRAY_SIZE(stbir__filter_info_table))
        return 0;
    if (info->vertical_filter >= STBIR__ARRAY_SIZE(stbir__filter_info_table))
        return 0;

    if (alpha_channel < 0)
        flags |= STBIR_FLAG_ALPHA_USES_COLORSPACE | STBIR_FLAG_ALPHA_PREMULTIPLIED;

    if (!(flags & STBIR_FLAG_ALPHA_USES_COLORSPACE) || !(flags & STBIR_FLAG_ALPHA_PREMULTIPLIED)) {
        STBIR_ASSERT(alpha_channel >= 0 && alpha_channel < info->channels);
    }

    if (alpha_channel >= info->channels)
        return 0;

    STBIR_ASSERT(tempmem);

    if (!tempmem)
        return 0;

    STBIR_ASSERT(tempmem_size_in_bytes >= memory_required);

    if (tempmem_size_in_bytes < memory_required)
        return 0;

    memset(tempmem, 0, tempmem_size_in_bytes);

    info->input_data = input_data;
    info->input_stride_bytes = width_stride_input;

    info->output_data = output_data;
    info->output_stride_bytes = width_stride_output;

    info->alpha_channel = alpha_channel;
    info->flags = flags;
    info->type = type;
    info->edge_horizontal = edge_horizontal;
    info->edge_vertical = edge_vertical;
    info->colorspace = colorspace;

    info->horizontal_coefficient_width = stbir__get_coefficient_width(info->horizontal_filter, info->horizontal_scale);
    info->vertical_coefficient_width = stbir__get_coefficient_width(info->vertical_filter, info->vertical_scale);
    info->horizontal_filter_pixel_width = stbir__get_filter_pixel_width(info->horizontal_filter, info->horizontal_scale);
    info->vertical_filter_pixel_width = stbir__get_filter_pixel_width(info->vertical_filter, info->vertical_scale);
    info->horizontal_filter_pixel_margin = stbir__get_filter_pixel_margin(info->horizontal_filter, info->horizontal_scale);
    info->vertical_filter_pixel_margin = stbir__get_filter_pixel_margin(info->vertical_filter, info->vertical_scale);

    info->ring_buffer_length_bytes = info->output_w * info->channels * sizeof(float);
    info->decode_buffer_pixels = info->input_w + info->horizontal_filter_pixel_margin * 2;

#define STBIR__NEXT_MEMPTR(current, newtype) (newtype*)(((unsigned char*)current) + current##_size)

    info->horizontal_contributors = (stbir__contributors*)tempmem;
    info->horizontal_coefficients = STBIR__NEXT_MEMPTR(info->horizontal_contributors, float);
    info->vertical_contributors = STBIR__NEXT_MEMPTR(info->horizontal_coefficients, stbir__contributors);
    info->vertical_coefficients = STBIR__NEXT_MEMPTR(info->vertical_contributors, float);
    info->decode_buffer = STBIR__NEXT_MEMPTR(info->vertical_coefficients, float);

    if (stbir__use_height_upsampling(info))
    {
        info->horizontal_buffer = NULL;
        info->ring_buffer = STBIR__NEXT_MEMPTR(info->decode_buffer, float);
        info->encode_buffer = STBIR__NEXT_MEMPTR(info->ring_buffer, float);

        STBIR_ASSERT((size_t)STBIR__NEXT_MEMPTR(info->encode_buffer, unsigned char) == (size_t)tempmem + tempmem_size_in_bytes);
    }
    else
    {
        info->horizontal_buffer = STBIR__NEXT_MEMPTR(info->decode_buffer, float);
        info->ring_buffer = STBIR__NEXT_MEMPTR(info->horizontal_buffer, float);
        info->encode_buffer = NULL;

        STBIR_ASSERT((size_t)STBIR__NEXT_MEMPTR(info->ring_buffer, unsigned char) == (size_t)tempmem + tempmem_size_in_bytes);
    }

#undef STBIR__NEXT_MEMPTR

    // This signals that the ring buffer is empty
    info->ring_buffer_begin_index = -1;

    stbir__calculate_filters(info->horizontal_contributors, info->horizontal_coefficients, info->horizontal_filter, info->horizontal_scale, info->horizontal_shift, info->input_w, info->output_w);
    stbir__calculate_filters(info->vertical_contributors, info->vertical_coefficients, info->vertical_filter, info->vertical_scale, info->vertical_shift, info->input_h, info->output_h);

    STBIR_PROGRESS_REPORT(0);

    if (stbir__use_height_upsampling(info))
        stbir__buffer_loop_upsample(info);
    else
        stbir__buffer_loop_downsample(info);

    STBIR_PROGRESS_REPORT(1);

#ifdef STBIR_DEBUG_OVERWRITE_TEST
    STBIR_ASSERT(memcmp(overwrite_output_before_pre, &((unsigned char*)output_data)[-OVERWRITE_ARRAY_SIZE], OVERWRITE_ARRAY_SIZE) == 0);
    STBIR_ASSERT(memcmp(overwrite_output_after_pre, &((unsigned char*)output_data)[begin_forbidden], OVERWRITE_ARRAY_SIZE) == 0);
    STBIR_ASSERT(memcmp(overwrite_tempmem_before_pre, &((unsigned char*)tempmem)[-OVERWRITE_ARRAY_SIZE], OVERWRITE_ARRAY_SIZE) == 0);
    STBIR_ASSERT(memcmp(overwrite_tempmem_after_pre, &((unsigned char*)tempmem)[tempmem_size_in_bytes], OVERWRITE_ARRAY_SIZE) == 0);
#endif

    return 1;
}


static int stbir__resize_arbitrary(
    void* alloc_context,
    const void* input_data, int input_w, int input_h, int input_stride_in_bytes,
    void* output_data, int output_w, int output_h, int output_stride_in_bytes,
    float s0, float t0, float s1, float t1, float* transform,
    int channels, int alpha_channel, stbir_uint32 flags, stbir_datatype type,
    stbir_filter h_filter, stbir_filter v_filter,
    stbir_edge edge_horizontal, stbir_edge edge_vertical, stbir_colorspace colorspace)
{
    stbir__info info;
    int result;
    size_t memory_required;
    void* extra_memory;

    stbir__setup(&info, input_w, input_h, output_w, output_h, channels);
    stbir__calculate_transform(&info, s0, t0, s1, t1, transform);
    stbir__choose_filter(&info, h_filter, v_filter);
    memory_required = stbir__calculate_memory(&info);
    extra_memory = STBIR_MALLOC(memory_required, alloc_context);

    if (!extra_memory)
        return 0;

    result = stbir__resize_allocated(&info, input_data, input_stride_in_bytes,
        output_data, output_stride_in_bytes,
        alpha_channel, flags, type,
        edge_horizontal, edge_vertical,
        colorspace, extra_memory, memory_required);

    STBIR_FREE(extra_memory, alloc_context);

    return result;
}

STBIRDEF int stbir_resize_uint8(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels)
{
    return stbir__resize_arbitrary(NULL, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, -1, 0, STBIR_TYPE_UINT8, STBIR_FILTER_DEFAULT, STBIR_FILTER_DEFAULT,
        STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP, STBIR_COLORSPACE_LINEAR);
}

STBIRDEF int stbir_resize_float(const float* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    float* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels)
{
    return stbir__resize_arbitrary(NULL, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, -1, 0, STBIR_TYPE_FLOAT, STBIR_FILTER_DEFAULT, STBIR_FILTER_DEFAULT,
        STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP, STBIR_COLORSPACE_LINEAR);
}

STBIRDEF int stbir_resize_uint8_srgb(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags)
{
    return stbir__resize_arbitrary(NULL, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, alpha_channel, flags, STBIR_TYPE_UINT8, STBIR_FILTER_DEFAULT, STBIR_FILTER_DEFAULT,
        STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP, STBIR_COLORSPACE_SRGB);
}

STBIRDEF int stbir_resize_uint8_srgb_edgemode(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode)
{
    return stbir__resize_arbitrary(NULL, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, alpha_channel, flags, STBIR_TYPE_UINT8, STBIR_FILTER_DEFAULT, STBIR_FILTER_DEFAULT,
        edge_wrap_mode, edge_wrap_mode, STBIR_COLORSPACE_SRGB);
}

STBIRDEF int stbir_resize_uint8_generic(const unsigned char* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    unsigned char* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space,
    void* alloc_context)
{
    return stbir__resize_arbitrary(alloc_context, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, alpha_channel, flags, STBIR_TYPE_UINT8, filter, filter,
        edge_wrap_mode, edge_wrap_mode, space);
}

STBIRDEF int stbir_resize_uint16_generic(const stbir_uint16* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    stbir_uint16* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space,
    void* alloc_context)
{
    return stbir__resize_arbitrary(alloc_context, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, alpha_channel, flags, STBIR_TYPE_UINT16, filter, filter,
        edge_wrap_mode, edge_wrap_mode, space);
}


STBIRDEF int stbir_resize_float_generic(const float* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    float* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space,
    void* alloc_context)
{
    return stbir__resize_arbitrary(alloc_context, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, alpha_channel, flags, STBIR_TYPE_FLOAT, filter, filter,
        edge_wrap_mode, edge_wrap_mode, space);
}


STBIRDEF int stbir_resize(const void* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    void* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    stbir_datatype datatype,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_mode_horizontal, stbir_edge edge_mode_vertical,
    stbir_filter filter_horizontal, stbir_filter filter_vertical,
    stbir_colorspace space, void* alloc_context)
{
    return stbir__resize_arbitrary(alloc_context, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, NULL, num_channels, alpha_channel, flags, datatype, filter_horizontal, filter_vertical,
        edge_mode_horizontal, edge_mode_vertical, space);
}


STBIRDEF int stbir_resize_subpixel(const void* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    void* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    stbir_datatype datatype,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_mode_horizontal, stbir_edge edge_mode_vertical,
    stbir_filter filter_horizontal, stbir_filter filter_vertical,
    stbir_colorspace space, void* alloc_context,
    float x_scale, float y_scale,
    float x_offset, float y_offset)
{
    float transform[4];
    transform[0] = x_scale;
    transform[1] = y_scale;
    transform[2] = x_offset;
    transform[3] = y_offset;
    return stbir__resize_arbitrary(alloc_context, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        0, 0, 1, 1, transform, num_channels, alpha_channel, flags, datatype, filter_horizontal, filter_vertical,
        edge_mode_horizontal, edge_mode_vertical, space);
}

STBIRDEF int stbir_resize_region(const void* input_pixels, int input_w, int input_h, int input_stride_in_bytes,
    void* output_pixels, int output_w, int output_h, int output_stride_in_bytes,
    stbir_datatype datatype,
    int num_channels, int alpha_channel, int flags,
    stbir_edge edge_mode_horizontal, stbir_edge edge_mode_vertical,
    stbir_filter filter_horizontal, stbir_filter filter_vertical,
    stbir_colorspace space, void* alloc_context,
    float s0, float t0, float s1, float t1)
{
    return stbir__resize_arbitrary(alloc_context, input_pixels, input_w, input_h, input_stride_in_bytes,
        output_pixels, output_w, output_h, output_stride_in_bytes,
        s0, t0, s1, t1, NULL, num_channels, alpha_channel, flags, datatype, filter_horizontal, filter_vertical,
        edge_mode_horizontal, edge_mode_vertical, space);
}

#endif // STB_IMAGE_RESIZE_IMPLEMENTATION

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
/* stb_image_write - v1.16 - public domain - http://nothings.org/stb
   writes out PNG/BMP/TGA/JPEG/HDR images to C stdio - Sean Barrett 2010-2015
                                     no warranty implied; use at your own risk

   Before #including,

       #define STB_IMAGE_WRITE_IMPLEMENTATION

   in the file that you want to have the implementation.

   Will probably not work correctly with strict-aliasing optimizations.

ABOUT:

   This header file is a library for writing images to C stdio or a callback.

   The PNG output is not optimal; it is 20-50% larger than the file
   written by a decent optimizing implementation; though providing a custom
   zlib compress function (see STBIW_ZLIB_COMPRESS) can mitigate that.
   This library is designed for source code compactness and simplicity,
   not optimal image file size or run-time performance.

BUILDING:

   You can #define STBIW_ASSERT(x) before the #include to avoid using assert.h.
   You can #define STBIW_MALLOC(), STBIW_REALLOC(), and STBIW_FREE() to replace
   malloc,realloc,free.
   You can #define STBIW_MEMMOVE() to replace memmove()
   You can #define STBIW_ZLIB_COMPRESS to use a custom zlib-style compress function
   for PNG compression (instead of the builtin one), it must have the following signature:
   unsigned char * my_compress(unsigned char *data, int data_len, int *out_len, int quality);
   The returned data will be freed with STBIW_FREE() (free() by default),
   so it must be heap allocated with STBIW_MALLOC() (malloc() by default),

UNICODE:

   If compiling for Windows and you wish to use Unicode filenames, compile
   with
       #define STBIW_WINDOWS_UTF8
   and pass utf8-encoded filenames. Call stbiw_convert_wchar_to_utf8 to convert
   Windows wchar_t filenames to utf8.

USAGE:

   There are five functions, one for each image file format:

     int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
     int stbi_write_bmp(char const *filename, int w, int h, int comp, const void *data);
     int stbi_write_tga(char const *filename, int w, int h, int comp, const void *data);
     int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality);
     int stbi_write_hdr(char const *filename, int w, int h, int comp, const float *data);

     void stbi_flip_vertically_on_write(int flag); // flag is non-zero to flip data vertically

   There are also five equivalent functions that use an arbitrary write function. You are
   expected to open/close your file-equivalent before and after calling these:

     int stbi_write_png_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data, int stride_in_bytes);
     int stbi_write_bmp_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data);
     int stbi_write_tga_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data);
     int stbi_write_hdr_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const float *data);
     int stbi_write_jpg_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const void *data, int quality);

   where the callback is:
      void stbi_write_func(void *context, void *data, int size);

   You can configure it with these global variables:
      int stbi_write_tga_with_rle;             // defaults to true; set to 0 to disable RLE
      int stbi_write_png_compression_level;    // defaults to 8; set to higher for more compression
      int stbi_write_force_png_filter;         // defaults to -1; set to 0..5 to force a filter mode


   You can define STBI_WRITE_NO_STDIO to disable the file variant of these
   functions, so the library will not use stdio.h at all. However, this will
   also disable HDR writing, because it requires stdio for formatted output.

   Each function returns 0 on failure and non-0 on success.

   The functions create an image file defined by the parameters. The image
   is a rectangle of pixels stored from left-to-right, top-to-bottom.
   Each pixel contains 'comp' channels of data stored interleaved with 8-bits
   per channel, in the following order: 1=Y, 2=YA, 3=RGB, 4=RGBA. (Y is
   monochrome color.) The rectangle is 'w' pixels wide and 'h' pixels tall.
   The *data pointer points to the first byte of the top-left-most pixel.
   For PNG, "stride_in_bytes" is the distance in bytes from the first byte of
   a row of pixels to the first byte of the next row of pixels.

   PNG creates output files with the same number of components as the input.
   The BMP format expands Y to RGB in the file format and does not
   output alpha.

   PNG supports writing rectangles of data even when the bytes storing rows of
   data are not consecutive in memory (e.g. sub-rectangles of a larger image),
   by supplying the stride between the beginning of adjacent rows. The other
   formats do not. (Thus you cannot write a native-format BMP through the BMP
   writer, both because it is in BGR order and because it may have padding
   at the end of the line.)

   PNG allows you to set the deflate compression level by setting the global
   variable 'stbi_write_png_compression_level' (it defaults to 8).

   HDR expects linear float data. Since the format is always 32-bit rgb(e)
   data, alpha (if provided) is discarded, and for monochrome data it is
   replicated across all three channels.

   TGA supports RLE or non-RLE compressed data. To use non-RLE-compressed
   data, set the global variable 'stbi_write_tga_with_rle' to 0.

   JPEG does ignore alpha channels in input data; quality is between 1 and 100.
   Higher quality looks better but results in a bigger image.
   JPEG baseline (no JPEG progressive).

CREDITS:


   Sean Barrett           -    PNG/BMP/TGA
   Baldur Karlsson        -    HDR
   Jean-Sebastien Guay    -    TGA monochrome
   Tim Kelsey             -    misc enhancements
   Alan Hickman           -    TGA RLE
   Emmanuel Julien        -    initial file IO callback implementation
   Jon Olick              -    original jo_jpeg.cpp code
   Daniel Gibson          -    integrate JPEG, allow external zlib
   Aarni Koskela          -    allow choosing PNG filter

   bugfixes:
      github:Chribba
      Guillaume Chereau
      github:jry2
      github:romigrou
      Sergio Gonzalez
      Jonas Karlsson
      Filip Wasil
      Thatcher Ulrich
      github:poppolopoppo
      Patrick Boettcher
      github:xeekworx
      Cap Petschulat
      Simon Rodriguez
      Ivan Tikhonov
      github:ignotion
      Adam Schackart
      Andrew Kensler

LICENSE

  See end of file for license information.

*/

#ifndef INCLUDE_STB_IMAGE_WRITE_H
#define INCLUDE_STB_IMAGE_WRITE_H

#include <stdlib.h>

// if STB_IMAGE_WRITE_STATIC causes problems, try defining STBIWDEF to 'inline' or 'static inline'
#ifndef STBIWDEF
#ifdef STB_IMAGE_WRITE_STATIC
#define STBIWDEF  static
#else
#ifdef __cplusplus
#define STBIWDEF  extern "C"
#else
#define STBIWDEF  extern
#endif
#endif
#endif

#ifndef STB_IMAGE_WRITE_STATIC  // C++ forbids static forward declarations
STBIWDEF int stbi_write_tga_with_rle;
STBIWDEF int stbi_write_png_compression_level;
STBIWDEF int stbi_write_force_png_filter;
#endif

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_png(char const* filename, int w, int h, int comp, const void* data, int stride_in_bytes);
STBIWDEF int stbi_write_bmp(char const* filename, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_tga(char const* filename, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_hdr(char const* filename, int w, int h, int comp, const float* data);
STBIWDEF int stbi_write_jpg(char const* filename, int x, int y, int comp, const void* data, int quality);

#ifdef STBIW_WINDOWS_UTF8
STBIWDEF int stbiw_convert_wchar_to_utf8(char* buffer, size_t bufferlen, const wchar_t* input);
#endif
#endif

typedef void stbi_write_func(void* context, void* data, int size);

STBIWDEF int stbi_write_png_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const void* data, int stride_in_bytes);
STBIWDEF int stbi_write_bmp_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_tga_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_hdr_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const float* data);
STBIWDEF int stbi_write_jpg_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int quality);

STBIWDEF void stbi_flip_vertically_on_write(int flip_boolean);

#endif//INCLUDE_STB_IMAGE_WRITE_H

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION

#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#endif

#ifndef STBI_WRITE_NO_STDIO
#include <stdio.h>
#endif // STBI_WRITE_NO_STDIO

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(STBIW_MALLOC) && defined(STBIW_FREE) && (defined(STBIW_REALLOC) || defined(STBIW_REALLOC_SIZED))
// ok
#elif !defined(STBIW_MALLOC) && !defined(STBIW_FREE) && !defined(STBIW_REALLOC) && !defined(STBIW_REALLOC_SIZED)
// ok
#else
#error "Must define all or none of STBIW_MALLOC, STBIW_FREE, and STBIW_REALLOC (or STBIW_REALLOC_SIZED)."
#endif

#ifndef STBIW_MALLOC
#define STBIW_MALLOC(sz)        malloc(sz)
#define STBIW_REALLOC(p,newsz)  realloc(p,newsz)
#define STBIW_FREE(p)           free(p)
#endif

#ifndef STBIW_REALLOC_SIZED
#define STBIW_REALLOC_SIZED(p,oldsz,newsz) STBIW_REALLOC(p,newsz)
#endif


#ifndef STBIW_MEMMOVE
#define STBIW_MEMMOVE(a,b,sz) memmove(a,b,sz)
#endif


#ifndef STBIW_ASSERT
#include <assert.h>
#define STBIW_ASSERT(x) assert(x)
#endif

#define STBIW_UCHAR(x) (unsigned char) ((x) & 0xff)

#ifdef STB_IMAGE_WRITE_STATIC
static int stbi_write_png_compression_level = 8;
static int stbi_write_tga_with_rle = 1;
static int stbi_write_force_png_filter = -1;
#else
int stbi_write_png_compression_level = 8;
int stbi_write_tga_with_rle = 1;
int stbi_write_force_png_filter = -1;
#endif

static int stbi__flip_vertically_on_write = 0;

STBIWDEF void stbi_flip_vertically_on_write(int flag)
{
    stbi__flip_vertically_on_write = flag;
}

typedef struct
{
    stbi_write_func* func;
    void* context;
    unsigned char buffer[64];
    int buf_used;
} stbi__write_context;

// initialize a callback-based context
static void stbi__start_write_callbacks(stbi__write_context* s, stbi_write_func* c, void* context)
{
    s->func = c;
    s->context = context;
}

#ifndef STBI_WRITE_NO_STDIO

static void stbi__stdio_write(void* context, void* data, int size)
{
    fwrite(data, 1, size, (FILE*)context);
}

#if defined(_WIN32) && defined(STBIW_WINDOWS_UTF8)
#ifdef __cplusplus
#define STBIW_EXTERN extern "C"
#else
#define STBIW_EXTERN extern
#endif
STBIW_EXTERN __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int cp, unsigned long flags, const char* str, int cbmb, wchar_t* widestr, int cchwide);
STBIW_EXTERN __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, const wchar_t* widestr, int cchwide, char* str, int cbmb, const char* defchar, int* used_default);

STBIWDEF int stbiw_convert_wchar_to_utf8(char* buffer, size_t bufferlen, const wchar_t* input)
{
    return WideCharToMultiByte(65001 /* UTF8 */, 0, input, -1, buffer, (int)bufferlen, NULL, NULL);
}
#endif

static FILE* stbiw__fopen(char const* filename, char const* mode)
{
    FILE* f;
#if defined(_WIN32) && defined(STBIW_WINDOWS_UTF8)
    wchar_t wMode[64];
    wchar_t wFilename[1024];
    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, filename, -1, wFilename, sizeof(wFilename) / sizeof(*wFilename)))
        return 0;

    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, mode, -1, wMode, sizeof(wMode) / sizeof(*wMode)))
        return 0;

#if defined(_MSC_VER) && _MSC_VER >= 1400
    if (0 != _wfopen_s(&f, wFilename, wMode))
        f = 0;
#else
    f = _wfopen(wFilename, wMode);
#endif

#elif defined(_MSC_VER) && _MSC_VER >= 1400
    if (0 != fopen_s(&f, filename, mode))
        f = 0;
#else
    f = fopen(filename, mode);
#endif
    return f;
}

static int stbi__start_write_file(stbi__write_context* s, const char* filename)
{
    FILE* f = stbiw__fopen(filename, "wb");
    stbi__start_write_callbacks(s, stbi__stdio_write, (void*)f);
    return f != NULL;
}

static void stbi__end_write_file(stbi__write_context* s)
{
    fclose((FILE*)s->context);
}

#endif // !STBI_WRITE_NO_STDIO

typedef unsigned int stbiw_uint32;
typedef int stb_image_write_test[sizeof(stbiw_uint32) == 4 ? 1 : -1];

static void stbiw__writefv(stbi__write_context* s, const char* fmt, va_list v)
{
    while (*fmt) {
        switch (*fmt++) {
        case ' ': break;
        case '1': { unsigned char x = STBIW_UCHAR(va_arg(v, int));
            s->func(s->context, &x, 1);
            break; }
        case '2': { int x = va_arg(v, int);
            unsigned char b[2];
            b[0] = STBIW_UCHAR(x);
            b[1] = STBIW_UCHAR(x >> 8);
            s->func(s->context, b, 2);
            break; }
        case '4': { stbiw_uint32 x = va_arg(v, int);
            unsigned char b[4];
            b[0] = STBIW_UCHAR(x);
            b[1] = STBIW_UCHAR(x >> 8);
            b[2] = STBIW_UCHAR(x >> 16);
            b[3] = STBIW_UCHAR(x >> 24);
            s->func(s->context, b, 4);
            break; }
        default:
            STBIW_ASSERT(0);
            return;
        }
    }
}

static void stbiw__writef(stbi__write_context* s, const char* fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    stbiw__writefv(s, fmt, v);
    va_end(v);
}

static void stbiw__write_flush(stbi__write_context* s)
{
    if (s->buf_used) {
        s->func(s->context, &s->buffer, s->buf_used);
        s->buf_used = 0;
    }
}

static void stbiw__putc(stbi__write_context* s, unsigned char c)
{
    s->func(s->context, &c, 1);
}

static void stbiw__write1(stbi__write_context* s, unsigned char a)
{
    if ((size_t)s->buf_used + 1 > sizeof(s->buffer))
        stbiw__write_flush(s);
    s->buffer[s->buf_used++] = a;
}

static void stbiw__write3(stbi__write_context* s, unsigned char a, unsigned char b, unsigned char c)
{
    int n;
    if ((size_t)s->buf_used + 3 > sizeof(s->buffer))
        stbiw__write_flush(s);
    n = s->buf_used;
    s->buf_used = n + 3;
    s->buffer[n + 0] = a;
    s->buffer[n + 1] = b;
    s->buffer[n + 2] = c;
}

static void stbiw__write_pixel(stbi__write_context* s, int rgb_dir, int comp, int write_alpha, int expand_mono, unsigned char* d)
{
    unsigned char bg[3] = { 255, 0, 255 }, px[3];
    int k;

    if (write_alpha < 0)
        stbiw__write1(s, d[comp - 1]);

    switch (comp) {
    case 2: // 2 pixels = mono + alpha, alpha is written separately, so same as 1-channel case
    case 1:
        if (expand_mono)
            stbiw__write3(s, d[0], d[0], d[0]); // monochrome bmp
        else
            stbiw__write1(s, d[0]);  // monochrome TGA
        break;
    case 4:
        if (!write_alpha) {
            // composite against pink background
            for (k = 0; k < 3; ++k)
                px[k] = bg[k] + ((d[k] - bg[k]) * d[3]) / 255;
            stbiw__write3(s, px[1 - rgb_dir], px[1], px[1 + rgb_dir]);
            break;
        }
        /* FALLTHROUGH */
    case 3:
        stbiw__write3(s, d[1 - rgb_dir], d[1], d[1 + rgb_dir]);
        break;
    }
    if (write_alpha > 0)
        stbiw__write1(s, d[comp - 1]);
}

static void stbiw__write_pixels(stbi__write_context* s, int rgb_dir, int vdir, int x, int y, int comp, void* data, int write_alpha, int scanline_pad, int expand_mono)
{
    stbiw_uint32 zero = 0;
    int i, j, j_end;

    if (y <= 0)
        return;

    if (stbi__flip_vertically_on_write)
        vdir *= -1;

    if (vdir < 0) {
        j_end = -1; j = y - 1;
    }
    else {
        j_end = y; j = 0;
    }

    for (; j != j_end; j += vdir) {
        for (i = 0; i < x; ++i) {
            unsigned char* d = (unsigned char*)data + (j * x + i) * comp;
            stbiw__write_pixel(s, rgb_dir, comp, write_alpha, expand_mono, d);
        }
        stbiw__write_flush(s);
        s->func(s->context, &zero, scanline_pad);
    }
}

static int stbiw__outfile(stbi__write_context* s, int rgb_dir, int vdir, int x, int y, int comp, int expand_mono, void* data, int alpha, int pad, const char* fmt, ...)
{
    if (y < 0 || x < 0) {
        return 0;
    }
    else {
        va_list v;
        va_start(v, fmt);
        stbiw__writefv(s, fmt, v);
        va_end(v);
        stbiw__write_pixels(s, rgb_dir, vdir, x, y, comp, data, alpha, pad, expand_mono);
        return 1;
    }
}

static int stbi_write_bmp_core(stbi__write_context* s, int x, int y, int comp, const void* data)
{
    if (comp != 4) {
        // write RGB bitmap
        int pad = (-x * 3) & 3;
        return stbiw__outfile(s, -1, -1, x, y, comp, 1, (void*)data, 0, pad,
            "11 4 22 4" "4 44 22 444444",
            'B', 'M', 14 + 40 + (x * 3 + pad) * y, 0, 0, 14 + 40,  // file header
            40, x, y, 1, 24, 0, 0, 0, 0, 0, 0);             // bitmap header
    }
    else {
        // RGBA bitmaps need a v4 header
        // use BI_BITFIELDS mode with 32bpp and alpha mask
        // (straight BI_RGB with alpha mask doesn't work in most readers)
        return stbiw__outfile(s, -1, -1, x, y, comp, 1, (void*)data, 1, 0,
            "11 4 22 4" "4 44 22 444444 4444 4 444 444 444 444",
            'B', 'M', 14 + 108 + x * y * 4, 0, 0, 14 + 108, // file header
            108, x, y, 1, 32, 3, 0, 0, 0, 0, 0, 0xff0000, 0xff00, 0xff, 0xff000000u, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // bitmap V4 header
    }
}

STBIWDEF int stbi_write_bmp_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    stbi__start_write_callbacks(&s, func, context);
    return stbi_write_bmp_core(&s, x, y, comp, data);
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_bmp(char const* filename, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    if (stbi__start_write_file(&s, filename)) {
        int r = stbi_write_bmp_core(&s, x, y, comp, data);
        stbi__end_write_file(&s);
        return r;
    }
    else
        return 0;
}
#endif //!STBI_WRITE_NO_STDIO

static int stbi_write_tga_core(stbi__write_context* s, int x, int y, int comp, void* data)
{
    int has_alpha = (comp == 2 || comp == 4);
    int colorbytes = has_alpha ? comp - 1 : comp;
    int format = colorbytes < 2 ? 3 : 2; // 3 color channels (RGB/RGBA) = 2, 1 color channel (Y/YA) = 3

    if (y < 0 || x < 0)
        return 0;

    if (!stbi_write_tga_with_rle) {
        return stbiw__outfile(s, -1, -1, x, y, comp, 0, (void*)data, has_alpha, 0,
            "111 221 2222 11", 0, 0, format, 0, 0, 0, 0, 0, x, y, (colorbytes + has_alpha) * 8, has_alpha * 8);
    }
    else {
        int i, j, k;
        int jend, jdir;

        stbiw__writef(s, "111 221 2222 11", 0, 0, format + 8, 0, 0, 0, 0, 0, x, y, (colorbytes + has_alpha) * 8, has_alpha * 8);

        if (stbi__flip_vertically_on_write) {
            j = 0;
            jend = y;
            jdir = 1;
        }
        else {
            j = y - 1;
            jend = -1;
            jdir = -1;
        }
        for (; j != jend; j += jdir) {
            unsigned char* row = (unsigned char*)data + j * x * comp;
            int len;

            for (i = 0; i < x; i += len) {
                unsigned char* begin = row + i * comp;
                int diff = 1;
                len = 1;

                if (i < x - 1) {
                    ++len;
                    diff = memcmp(begin, row + (i + 1) * comp, comp);
                    if (diff) {
                        const unsigned char* prev = begin;
                        for (k = i + 2; k < x && len < 128; ++k) {
                            if (memcmp(prev, row + k * comp, comp)) {
                                prev += comp;
                                ++len;
                            }
                            else {
                                --len;
                                break;
                            }
                        }
                    }
                    else {
                        for (k = i + 2; k < x && len < 128; ++k) {
                            if (!memcmp(begin, row + k * comp, comp)) {
                                ++len;
                            }
                            else {
                                break;
                            }
                        }
                    }
                }

                if (diff) {
                    unsigned char header = STBIW_UCHAR(len - 1);
                    stbiw__write1(s, header);
                    for (k = 0; k < len; ++k) {
                        stbiw__write_pixel(s, -1, comp, has_alpha, 0, begin + k * comp);
                    }
                }
                else {
                    unsigned char header = STBIW_UCHAR(len - 129);
                    stbiw__write1(s, header);
                    stbiw__write_pixel(s, -1, comp, has_alpha, 0, begin);
                }
            }
        }
        stbiw__write_flush(s);
    }
    return 1;
}

STBIWDEF int stbi_write_tga_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    stbi__start_write_callbacks(&s, func, context);
    return stbi_write_tga_core(&s, x, y, comp, (void*)data);
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_tga(char const* filename, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    if (stbi__start_write_file(&s, filename)) {
        int r = stbi_write_tga_core(&s, x, y, comp, (void*)data);
        stbi__end_write_file(&s);
        return r;
    }
    else
        return 0;
}
#endif

// *************************************************************************************************
// Radiance RGBE HDR writer
// by Baldur Karlsson

#define stbiw__max(a, b)  ((a) > (b) ? (a) : (b))

#ifndef STBI_WRITE_NO_STDIO

static void stbiw__linear_to_rgbe(unsigned char* rgbe, float* linear)
{
    int exponent;
    float maxcomp = stbiw__max(linear[0], stbiw__max(linear[1], linear[2]));

    if (maxcomp < 1e-32f) {
        rgbe[0] = rgbe[1] = rgbe[2] = rgbe[3] = 0;
    }
    else {
        float normalize = (float)frexp(maxcomp, &exponent) * 256.0f / maxcomp;

        rgbe[0] = (unsigned char)(linear[0] * normalize);
        rgbe[1] = (unsigned char)(linear[1] * normalize);
        rgbe[2] = (unsigned char)(linear[2] * normalize);
        rgbe[3] = (unsigned char)(exponent + 128);
    }
}

static void stbiw__write_run_data(stbi__write_context* s, int length, unsigned char databyte)
{
    unsigned char lengthbyte = STBIW_UCHAR(length + 128);
    STBIW_ASSERT(length + 128 <= 255);
    s->func(s->context, &lengthbyte, 1);
    s->func(s->context, &databyte, 1);
}

static void stbiw__write_dump_data(stbi__write_context* s, int length, unsigned char* data)
{
    unsigned char lengthbyte = STBIW_UCHAR(length);
    STBIW_ASSERT(length <= 128); // inconsistent with spec but consistent with official code
    s->func(s->context, &lengthbyte, 1);
    s->func(s->context, data, length);
}

static void stbiw__write_hdr_scanline(stbi__write_context* s, int width, int ncomp, unsigned char* scratch, float* scanline)
{
    unsigned char scanlineheader[4] = { 2, 2, 0, 0 };
    unsigned char rgbe[4];
    float linear[3];
    int x;

    scanlineheader[2] = (width & 0xff00) >> 8;
    scanlineheader[3] = (width & 0x00ff);

    /* skip RLE for images too small or large */
    if (width < 8 || width >= 32768) {
        for (x = 0; x < width; x++) {
            switch (ncomp) {
            case 4: /* fallthrough */
            case 3: linear[2] = scanline[x * ncomp + 2];
                linear[1] = scanline[x * ncomp + 1];
                linear[0] = scanline[x * ncomp + 0];
                break;
            default:
                linear[0] = linear[1] = linear[2] = scanline[x * ncomp + 0];
                break;
            }
            stbiw__linear_to_rgbe(rgbe, linear);
            s->func(s->context, rgbe, 4);
        }
    }
    else {
        int c, r;
        /* encode into scratch buffer */
        for (x = 0; x < width; x++) {
            switch (ncomp) {
            case 4: /* fallthrough */
            case 3: linear[2] = scanline[x * ncomp + 2];
                linear[1] = scanline[x * ncomp + 1];
                linear[0] = scanline[x * ncomp + 0];
                break;
            default:
                linear[0] = linear[1] = linear[2] = scanline[x * ncomp + 0];
                break;
            }
            stbiw__linear_to_rgbe(rgbe, linear);
            scratch[x + width * 0] = rgbe[0];
            scratch[x + width * 1] = rgbe[1];
            scratch[x + width * 2] = rgbe[2];
            scratch[x + width * 3] = rgbe[3];
        }

        s->func(s->context, scanlineheader, 4);

        /* RLE each component separately */
        for (c = 0; c < 4; c++) {
            unsigned char* comp = &scratch[width * c];

            x = 0;
            while (x < width) {
                // find first run
                r = x;
                while (r + 2 < width) {
                    if (comp[r] == comp[r + 1] && comp[r] == comp[r + 2])
                        break;
                    ++r;
                }
                if (r + 2 >= width)
                    r = width;
                // dump up to first run
                while (x < r) {
                    int len = r - x;
                    if (len > 128) len = 128;
                    stbiw__write_dump_data(s, len, &comp[x]);
                    x += len;
                }
                // if there's a run, output it
                if (r + 2 < width) { // same test as what we break out of in search loop, so only true if we break'd
                    // find next byte after run
                    while (r < width && comp[r] == comp[x])
                        ++r;
                    // output run up to r
                    while (x < r) {
                        int len = r - x;
                        if (len > 127) len = 127;
                        stbiw__write_run_data(s, len, comp[x]);
                        x += len;
                    }
                }
            }
        }
    }
}

/*
static int stbi_write_hdr_core(stbi__write_context *s, int x, int y, int comp, float *data)
{
   if (y <= 0 || x <= 0 || data == NULL)
      return 0;
   else {
      // Each component is stored separately. Allocate scratch space for full output scanline.
      unsigned char *scratch = (unsigned char *) STBIW_MALLOC(x*4);
      int i, len;
      char buffer[128];
      char header[] = "#?RADIANCE\n# Written by stb_image_write.h\nFORMAT=32-bit_rle_rgbe\n";
      s->func(s->context, header, sizeof(header)-1);

#ifdef __STDC_LIB_EXT1__
      len = sprintf_s(buffer, sizeof(buffer), "EXPOSURE=          1.0000000000000\n\n-Y %d +X %d\n", y, x);
#else
      len = sprintf(buffer, "EXPOSURE=          1.0000000000000\n\n-Y %d +X %d\n", y, x);
#endif
      s->func(s->context, buffer, len);

      for(i=0; i < y; i++)
         stbiw__write_hdr_scanline(s, x, comp, scratch, data + comp*x*(stbi__flip_vertically_on_write ? y-1-i : i));
      STBIW_FREE(scratch);
      return 1;
   }
}
*/

/*
STBIWDEF int stbi_write_hdr_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const float *data)
{
   stbi__write_context s = { 0 };
   stbi__start_write_callbacks(&s, func, context);
   return stbi_write_hdr_core(&s, x, y, comp, (float *) data);
}
*/

/*
STBIWDEF int stbi_write_hdr(char const *filename, int x, int y, int comp, const float *data)
{
   stbi__write_context s = { 0 };
   if (stbi__start_write_file(&s,filename)) {
      int r = stbi_write_hdr_core(&s, x, y, comp, (float *) data);
      stbi__end_write_file(&s);
      return r;
   } else
      return 0;
}
*/
#endif // STBI_WRITE_NO_STDIO


//////////////////////////////////////////////////////////////////////////////
//
// PNG writer
//

#ifndef STBIW_ZLIB_COMPRESS
// stretchy buffer; stbiw__sbpush() == vector<>::push_back() -- stbiw__sbcount() == vector<>::size()
#define stbiw__sbraw(a) ((int *) (void *) (a) - 2)
#define stbiw__sbm(a)   stbiw__sbraw(a)[0]
#define stbiw__sbn(a)   stbiw__sbraw(a)[1]

#define stbiw__sbneedgrow(a,n)  ((a)==0 || stbiw__sbn(a)+n >= stbiw__sbm(a))
#define stbiw__sbmaybegrow(a,n) (stbiw__sbneedgrow(a,(n)) ? stbiw__sbgrow(a,n) : 0)
#define stbiw__sbgrow(a,n)  stbiw__sbgrowf((void **) &(a), (n), sizeof(*(a)))

#define stbiw__sbpush(a, v)      (stbiw__sbmaybegrow(a,1), (a)[stbiw__sbn(a)++] = (v))
#define stbiw__sbcount(a)        ((a) ? stbiw__sbn(a) : 0)
#define stbiw__sbfree(a)         ((a) ? STBIW_FREE(stbiw__sbraw(a)),0 : 0)

static void* stbiw__sbgrowf(void** arr, int increment, int itemsize)
{
    int m = *arr ? 2 * stbiw__sbm(*arr) + increment : increment + 1;
    void* p = STBIW_REALLOC_SIZED(*arr ? stbiw__sbraw(*arr) : 0, *arr ? (stbiw__sbm(*arr) * itemsize + sizeof(int) * 2) : 0, itemsize * m + sizeof(int) * 2);
    STBIW_ASSERT(p);
    if (p) {
        if (!*arr) ((int*)p)[1] = 0;
        *arr = (void*)((int*)p + 2);
        stbiw__sbm(*arr) = m;
    }
    return *arr;
}

static unsigned char* stbiw__zlib_flushf(unsigned char* data, unsigned int* bitbuffer, int* bitcount)
{
    while (*bitcount >= 8) {
        stbiw__sbpush(data, STBIW_UCHAR(*bitbuffer));
        *bitbuffer >>= 8;
        *bitcount -= 8;
    }
    return data;
}

static int stbiw__zlib_bitrev(int code, int codebits)
{
    int res = 0;
    while (codebits--) {
        res = (res << 1) | (code & 1);
        code >>= 1;
    }
    return res;
}

static unsigned int stbiw__zlib_countm(unsigned char* a, unsigned char* b, int limit)
{
    int i;
    for (i = 0; i < limit && i < 258; ++i)
        if (a[i] != b[i]) break;
    return i;
}

static unsigned int stbiw__zhash(unsigned char* data)
{
    stbiw_uint32 hash = data[0] + (data[1] << 8) + (data[2] << 16);
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
}

#define stbiw__zlib_flush() (out = stbiw__zlib_flushf(out, &bitbuf, &bitcount))
#define stbiw__zlib_add(code,codebits) \
      (bitbuf |= (code) << bitcount, bitcount += (codebits), stbiw__zlib_flush())
#define stbiw__zlib_huffa(b,c)  stbiw__zlib_add(stbiw__zlib_bitrev(b,c),c)
// default huffman tables
#define stbiw__zlib_huff1(n)  stbiw__zlib_huffa(0x30 + (n), 8)
#define stbiw__zlib_huff2(n)  stbiw__zlib_huffa(0x190 + (n)-144, 9)
#define stbiw__zlib_huff3(n)  stbiw__zlib_huffa(0 + (n)-256,7)
#define stbiw__zlib_huff4(n)  stbiw__zlib_huffa(0xc0 + (n)-280,8)
#define stbiw__zlib_huff(n)  ((n) <= 143 ? stbiw__zlib_huff1(n) : (n) <= 255 ? stbiw__zlib_huff2(n) : (n) <= 279 ? stbiw__zlib_huff3(n) : stbiw__zlib_huff4(n))
#define stbiw__zlib_huffb(n) ((n) <= 143 ? stbiw__zlib_huff1(n) : stbiw__zlib_huff2(n))

#define stbiw__ZHASH   16384

#endif // STBIW_ZLIB_COMPRESS

STBIWDEF unsigned char* stbi_zlib_compress(unsigned char* data, int data_len, int* out_len, int quality)
{
#ifdef STBIW_ZLIB_COMPRESS
    // user provided a zlib compress implementation, use that
    return STBIW_ZLIB_COMPRESS(data, data_len, out_len, quality);
#else // use builtin
    static unsigned short lengthc[] = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258, 259 };
    static unsigned char  lengtheb[] = { 0,0,0,0,0,0,0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,  4,  5,  5,  5,  5,  0 };
    static unsigned short distc[] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577, 32768 };
    static unsigned char  disteb[] = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
    unsigned int bitbuf = 0;
    int i, j, bitcount = 0;
    unsigned char* out = NULL;
    unsigned char*** hash_table = (unsigned char***)STBIW_MALLOC(stbiw__ZHASH * sizeof(unsigned char**));
    if (hash_table == NULL)
        return NULL;
    if (quality < 5) quality = 5;

    stbiw__sbpush(out, 0x78);   // DEFLATE 32K window
    stbiw__sbpush(out, 0x5e);   // FLEVEL = 1
    stbiw__zlib_add(1, 1);  // BFINAL = 1
    stbiw__zlib_add(1, 2);  // BTYPE = 1 -- fixed huffman

    for (i = 0; i < stbiw__ZHASH; ++i)
        hash_table[i] = NULL;

    i = 0;
    while (i < data_len - 3) {
        // hash next 3 bytes of data to be compressed
        int h = stbiw__zhash(data + i) & (stbiw__ZHASH - 1), best = 3;
        unsigned char* bestloc = 0;
        unsigned char** hlist = hash_table[h];
        int n = stbiw__sbcount(hlist);
        for (j = 0; j < n; ++j) {
            if (hlist[j] - data > i - 32768) { // if entry lies within window
                int d = stbiw__zlib_countm(hlist[j], data + i, data_len - i);
                if (d >= best) { best = d; bestloc = hlist[j]; }
            }
        }
        // when hash table entry is too long, delete half the entries
        if (hash_table[h] && stbiw__sbn(hash_table[h]) == 2 * quality) {
            STBIW_MEMMOVE(hash_table[h], hash_table[h] + quality, sizeof(hash_table[h][0]) * quality);
            stbiw__sbn(hash_table[h]) = quality;
        }
        stbiw__sbpush(hash_table[h], data + i);

        if (bestloc) {
            // "lazy matching" - check match at *next* byte, and if it's better, do cur byte as literal
            h = stbiw__zhash(data + i + 1) & (stbiw__ZHASH - 1);
            hlist = hash_table[h];
            n = stbiw__sbcount(hlist);
            for (j = 0; j < n; ++j) {
                if (hlist[j] - data > i - 32767) {
                    int e = stbiw__zlib_countm(hlist[j], data + i + 1, data_len - i - 1);
                    if (e > best) { // if next match is better, bail on current match
                        bestloc = NULL;
                        break;
                    }
                }
            }
        }

        if (bestloc) {
            int d = (int)(data + i - bestloc); // distance back
            STBIW_ASSERT(d <= 32767 && best <= 258);
            for (j = 0; best > lengthc[j + 1] - 1; ++j);
            stbiw__zlib_huff(j + 257);
            if (lengtheb[j]) stbiw__zlib_add(best - lengthc[j], lengtheb[j]);
            for (j = 0; d > distc[j + 1] - 1; ++j);
            stbiw__zlib_add(stbiw__zlib_bitrev(j, 5), 5);
            if (disteb[j]) stbiw__zlib_add(d - distc[j], disteb[j]);
            i += best;
        }
        else {
            stbiw__zlib_huffb(data[i]);
            ++i;
        }
    }
    // write out final bytes
    for (; i < data_len; ++i)
        stbiw__zlib_huffb(data[i]);
    stbiw__zlib_huff(256); // end of block
    // pad with 0 bits to byte boundary
    while (bitcount)
        stbiw__zlib_add(0, 1);

    for (i = 0; i < stbiw__ZHASH; ++i)
        (void)stbiw__sbfree(hash_table[i]);
    STBIW_FREE(hash_table);

    // store uncompressed instead if compression was worse
    if (stbiw__sbn(out) > data_len + 2 + ((data_len + 32766) / 32767) * 5) {
        stbiw__sbn(out) = 2;  // truncate to DEFLATE 32K window and FLEVEL = 1
        for (j = 0; j < data_len;) {
            int blocklen = data_len - j;
            if (blocklen > 32767) blocklen = 32767;
            stbiw__sbpush(out, data_len - j == blocklen); // BFINAL = ?, BTYPE = 0 -- no compression
            stbiw__sbpush(out, STBIW_UCHAR(blocklen)); // LEN
            stbiw__sbpush(out, STBIW_UCHAR(blocklen >> 8));
            stbiw__sbpush(out, STBIW_UCHAR(~blocklen)); // NLEN
            stbiw__sbpush(out, STBIW_UCHAR(~blocklen >> 8));
            memcpy(out + stbiw__sbn(out), data + j, blocklen);
            stbiw__sbn(out) += blocklen;
            j += blocklen;
        }
    }

    {
        // compute adler32 on input
        unsigned int s1 = 1, s2 = 0;
        int blocklen = (int)(data_len % 5552);
        j = 0;
        while (j < data_len) {
            for (i = 0; i < blocklen; ++i) { s1 += data[j + i]; s2 += s1; }
            s1 %= 65521; s2 %= 65521;
            j += blocklen;
            blocklen = 5552;
        }
        stbiw__sbpush(out, STBIW_UCHAR(s2 >> 8));
        stbiw__sbpush(out, STBIW_UCHAR(s2));
        stbiw__sbpush(out, STBIW_UCHAR(s1 >> 8));
        stbiw__sbpush(out, STBIW_UCHAR(s1));
    }
    *out_len = stbiw__sbn(out);
    // make returned pointer freeable
    STBIW_MEMMOVE(stbiw__sbraw(out), out, *out_len);
    return (unsigned char*)stbiw__sbraw(out);
#endif // STBIW_ZLIB_COMPRESS
}

static unsigned int stbiw__crc32(unsigned char* buffer, int len)
{
#ifdef STBIW_CRC32
    return STBIW_CRC32(buffer, len);
#else
    static unsigned int crc_table[256] =
    {
       0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
       0x0eDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
       0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
       0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
       0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
       0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
       0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
       0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
       0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
       0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
       0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
       0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
       0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
       0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
       0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
       0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
       0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
       0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
       0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
       0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
       0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
       0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
       0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
       0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
       0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
       0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
       0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
       0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
       0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
       0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
       0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
       0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };

    unsigned int crc = ~0u;
    int i;
    for (i = 0; i < len; ++i)
        crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
    return ~crc;
#endif
}

#define stbiw__wpng4(o,a,b,c,d) ((o)[0]=STBIW_UCHAR(a),(o)[1]=STBIW_UCHAR(b),(o)[2]=STBIW_UCHAR(c),(o)[3]=STBIW_UCHAR(d),(o)+=4)
#define stbiw__wp32(data,v) stbiw__wpng4(data, (v)>>24,(v)>>16,(v)>>8,(v));
#define stbiw__wptag(data,s) stbiw__wpng4(data, s[0],s[1],s[2],s[3])

static void stbiw__wpcrc(unsigned char** data, int len)
{
    unsigned int crc = stbiw__crc32(*data - len - 4, len + 4);
    stbiw__wp32(*data, crc);
}

static unsigned char stbiw__paeth(int a, int b, int c)
{
    int p = a + b - c, pa = abs(p - a), pb = abs(p - b), pc = abs(p - c);
    if (pa <= pb && pa <= pc) return STBIW_UCHAR(a);
    if (pb <= pc) return STBIW_UCHAR(b);
    return STBIW_UCHAR(c);
}

// @OPTIMIZE: provide an option that always forces left-predict or paeth predict
static void stbiw__encode_png_line(unsigned char* pixels, int stride_bytes, int width, int height, int y, int n, int filter_type, signed char* line_buffer)
{
    static int mapping[] = { 0,1,2,3,4 };
    static int firstmap[] = { 0,1,0,5,6 };
    int* mymap = (y != 0) ? mapping : firstmap;
    int i;
    int type = mymap[filter_type];
    unsigned char* z = pixels + stride_bytes * (stbi__flip_vertically_on_write ? height - 1 - y : y);
    int signed_stride = stbi__flip_vertically_on_write ? -stride_bytes : stride_bytes;

    if (type == 0) {
        memcpy(line_buffer, z, width * n);
        return;
    }

    // first loop isn't optimized since it's just one pixel
    for (i = 0; i < n; ++i) {
        switch (type) {
        case 1: line_buffer[i] = z[i]; break;
        case 2: line_buffer[i] = z[i] - z[i - signed_stride]; break;
        case 3: line_buffer[i] = z[i] - (z[i - signed_stride] >> 1); break;
        case 4: line_buffer[i] = (signed char)(z[i] - stbiw__paeth(0, z[i - signed_stride], 0)); break;
        case 5: line_buffer[i] = z[i]; break;
        case 6: line_buffer[i] = z[i]; break;
        }
    }
    switch (type) {
    case 1: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - z[i - n]; break;
    case 2: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - z[i - signed_stride]; break;
    case 3: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - ((z[i - n] + z[i - signed_stride]) >> 1); break;
    case 4: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i - n], z[i - signed_stride], z[i - signed_stride - n]); break;
    case 5: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - (z[i - n] >> 1); break;
    case 6: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i - n], 0, 0); break;
    }
}

STBIWDEF unsigned char* stbi_write_png_to_mem(const unsigned char* pixels, int stride_bytes, int x, int y, int n, int* out_len)
{
    int force_filter = stbi_write_force_png_filter;
    int ctype[5] = { -1, 0, 4, 2, 6 };
    unsigned char sig[8] = { 137,80,78,71,13,10,26,10 };
    unsigned char* out, * o, * filt, * zlib;
    signed char* line_buffer;
    int j, zlen;

    if (stride_bytes == 0)
        stride_bytes = x * n;

    if (force_filter >= 5) {
        force_filter = -1;
    }

    filt = (unsigned char*)STBIW_MALLOC((x * n + 1) * y); if (!filt) return 0;
    line_buffer = (signed char*)STBIW_MALLOC(x * n); if (!line_buffer) { STBIW_FREE(filt); return 0; }
    for (j = 0; j < y; ++j) {
        int filter_type;
        if (force_filter > -1) {
            filter_type = force_filter;
            stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, force_filter, line_buffer);
        }
        else { // Estimate the best filter by running through all of them:
            int best_filter = 0, best_filter_val = 0x7fffffff, est, i;
            for (filter_type = 0; filter_type < 5; filter_type++) {
                stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, filter_type, line_buffer);

                // Estimate the entropy of the line using this filter; the less, the better.
                est = 0;
                for (i = 0; i < x * n; ++i) {
                    est += abs((signed char)line_buffer[i]);
                }
                if (est < best_filter_val) {
                    best_filter_val = est;
                    best_filter = filter_type;
                }
            }
            if (filter_type != best_filter) {  // If the last iteration already got us the best filter, don't redo it
                stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, best_filter, line_buffer);
                filter_type = best_filter;
            }
        }
        // when we get here, filter_type contains the filter type, and line_buffer contains the data
        filt[j * (x * n + 1)] = (unsigned char)filter_type;
        STBIW_MEMMOVE(filt + j * (x * n + 1) + 1, line_buffer, x * n);
    }
    STBIW_FREE(line_buffer);
    zlib = stbi_zlib_compress(filt, y * (x * n + 1), &zlen, stbi_write_png_compression_level);
    STBIW_FREE(filt);
    if (!zlib) return 0;

    // each tag requires 12 bytes of overhead
    out = (unsigned char*)STBIW_MALLOC(8 + 12 + 13 + 12 + zlen + 12);
    if (!out) return 0;
    *out_len = 8 + 12 + 13 + 12 + zlen + 12;

    o = out;
    STBIW_MEMMOVE(o, sig, 8); o += 8;
    stbiw__wp32(o, 13); // header length
    stbiw__wptag(o, "IHDR");
    stbiw__wp32(o, x);
    stbiw__wp32(o, y);
    *o++ = 8;
    *o++ = STBIW_UCHAR(ctype[n]);
    *o++ = 0;
    *o++ = 0;
    *o++ = 0;
    stbiw__wpcrc(&o, 13);

    stbiw__wp32(o, zlen);
    stbiw__wptag(o, "IDAT");
    STBIW_MEMMOVE(o, zlib, zlen);
    o += zlen;
    STBIW_FREE(zlib);
    stbiw__wpcrc(&o, zlen);

    stbiw__wp32(o, 0);
    stbiw__wptag(o, "IEND");
    stbiw__wpcrc(&o, 0);

    STBIW_ASSERT(o == out + *out_len);

    return out;
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_png(char const* filename, int x, int y, int comp, const void* data, int stride_bytes)
{
    FILE* f;
    int len;
    unsigned char* png = stbi_write_png_to_mem((const unsigned char*)data, stride_bytes, x, y, comp, &len);
    if (png == NULL) return 0;

    f = stbiw__fopen(filename, "wb");
    if (!f) { STBIW_FREE(png); return 0; }
    fwrite(png, 1, len, f);
    fclose(f);
    STBIW_FREE(png);
    return 1;
}
#endif

STBIWDEF int stbi_write_png_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int stride_bytes)
{
    int len;
    unsigned char* png = stbi_write_png_to_mem((const unsigned char*)data, stride_bytes, x, y, comp, &len);
    if (png == NULL) return 0;
    func(context, png, len);
    STBIW_FREE(png);
    return 1;
}


/* ***************************************************************************
 *
 * JPEG writer
 *
 * This is based on Jon Olick's jo_jpeg.cpp:
 * public domain Simple, Minimalistic JPEG writer - http://www.jonolick.com/code.html
 */

static const unsigned char stbiw__jpg_ZigZag[] = { 0,1,5,6,14,15,27,28,2,4,7,13,16,26,29,42,3,8,12,17,25,30,41,43,9,11,18,
      24,31,40,44,53,10,19,23,32,39,45,52,54,20,22,33,38,46,51,55,60,21,34,37,47,50,56,59,61,35,36,48,49,57,58,62,63 };

static void stbiw__jpg_writeBits(stbi__write_context* s, int* bitBufP, int* bitCntP, const unsigned short* bs) {
    int bitBuf = *bitBufP, bitCnt = *bitCntP;
    bitCnt += bs[1];
    bitBuf |= bs[0] << (24 - bitCnt);
    while (bitCnt >= 8) {
        unsigned char c = (bitBuf >> 16) & 255;
        stbiw__putc(s, c);
        if (c == 255) {
            stbiw__putc(s, 0);
        }
        bitBuf <<= 8;
        bitCnt -= 8;
    }
    *bitBufP = bitBuf;
    *bitCntP = bitCnt;
}

static void stbiw__jpg_DCT(float* d0p, float* d1p, float* d2p, float* d3p, float* d4p, float* d5p, float* d6p, float* d7p) {
    float d0 = *d0p, d1 = *d1p, d2 = *d2p, d3 = *d3p, d4 = *d4p, d5 = *d5p, d6 = *d6p, d7 = *d7p;
    float z1, z2, z3, z4, z5, z11, z13;

    float tmp0 = d0 + d7;
    float tmp7 = d0 - d7;
    float tmp1 = d1 + d6;
    float tmp6 = d1 - d6;
    float tmp2 = d2 + d5;
    float tmp5 = d2 - d5;
    float tmp3 = d3 + d4;
    float tmp4 = d3 - d4;

    // Even part
    float tmp10 = tmp0 + tmp3;   // phase 2
    float tmp13 = tmp0 - tmp3;
    float tmp11 = tmp1 + tmp2;
    float tmp12 = tmp1 - tmp2;

    d0 = tmp10 + tmp11;       // phase 3
    d4 = tmp10 - tmp11;

    z1 = (tmp12 + tmp13) * 0.707106781f; // c4
    d2 = tmp13 + z1;       // phase 5
    d6 = tmp13 - z1;

    // Odd part
    tmp10 = tmp4 + tmp5;       // phase 2
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    // The rotator is modified from fig 4-8 to avoid extra negations.
    z5 = (tmp10 - tmp12) * 0.382683433f; // c6
    z2 = tmp10 * 0.541196100f + z5; // c2-c6
    z4 = tmp12 * 1.306562965f + z5; // c2+c6
    z3 = tmp11 * 0.707106781f; // c4

    z11 = tmp7 + z3;      // phase 5
    z13 = tmp7 - z3;

    *d5p = z13 + z2;         // phase 6
    *d3p = z13 - z2;
    *d1p = z11 + z4;
    *d7p = z11 - z4;

    *d0p = d0;  *d2p = d2;  *d4p = d4;  *d6p = d6;
}

static void stbiw__jpg_calcBits(int val, unsigned short bits[2]) {
    int tmp1 = val < 0 ? -val : val;
    val = val < 0 ? val - 1 : val;
    bits[1] = 1;
    while (tmp1 >>= 1) {
        ++bits[1];
    }
    bits[0] = val & ((1 << bits[1]) - 1);
}

static int stbiw__jpg_processDU(stbi__write_context* s, int* bitBuf, int* bitCnt, float* CDU, int du_stride, float* fdtbl, int DC, const unsigned short HTDC[256][2], const unsigned short HTAC[256][2]) {
    const unsigned short EOB[2] = { HTAC[0x00][0], HTAC[0x00][1] };
    const unsigned short M16zeroes[2] = { HTAC[0xF0][0], HTAC[0xF0][1] };
    int dataOff, i, j, n, diff, end0pos, x, y;
    int DU[64];

    // DCT rows
    for (dataOff = 0, n = du_stride * 8; dataOff < n; dataOff += du_stride) {
        stbiw__jpg_DCT(&CDU[dataOff], &CDU[dataOff + 1], &CDU[dataOff + 2], &CDU[dataOff + 3], &CDU[dataOff + 4], &CDU[dataOff + 5], &CDU[dataOff + 6], &CDU[dataOff + 7]);
    }
    // DCT columns
    for (dataOff = 0; dataOff < 8; ++dataOff) {
        stbiw__jpg_DCT(&CDU[dataOff], &CDU[dataOff + du_stride], &CDU[dataOff + du_stride * 2], &CDU[dataOff + du_stride * 3], &CDU[dataOff + du_stride * 4],
            &CDU[dataOff + du_stride * 5], &CDU[dataOff + du_stride * 6], &CDU[dataOff + du_stride * 7]);
    }
    // Quantize/descale/zigzag the coefficients
    for (y = 0, j = 0; y < 8; ++y) {
        for (x = 0; x < 8; ++x, ++j) {
            float v;
            i = y * du_stride + x;
            v = CDU[i] * fdtbl[j];
            // DU[stbiw__jpg_ZigZag[j]] = (int)(v < 0 ? ceilf(v - 0.5f) : floorf(v + 0.5f));
            // ceilf() and floorf() are C99, not C89, but I /think/ they're not needed here anyway?
            DU[stbiw__jpg_ZigZag[j]] = (int)(v < 0 ? v - 0.5f : v + 0.5f);
        }
    }

    // Encode DC
    diff = DU[0] - DC;
    if (diff == 0) {
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTDC[0]);
    }
    else {
        unsigned short bits[2];
        stbiw__jpg_calcBits(diff, bits);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTDC[bits[1]]);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, bits);
    }
    // Encode ACs
    end0pos = 63;
    for (; (end0pos > 0) && (DU[end0pos] == 0); --end0pos) {
    }
    // end0pos = first element in reverse order !=0
    if (end0pos == 0) {
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, EOB);
        return DU[0];
    }
    for (i = 1; i <= end0pos; ++i) {
        int startpos = i;
        int nrzeroes;
        unsigned short bits[2];
        for (; DU[i] == 0 && i <= end0pos; ++i) {
        }
        nrzeroes = i - startpos;
        if (nrzeroes >= 16) {
            int lng = nrzeroes >> 4;
            int nrmarker;
            for (nrmarker = 1; nrmarker <= lng; ++nrmarker)
                stbiw__jpg_writeBits(s, bitBuf, bitCnt, M16zeroes);
            nrzeroes &= 15;
        }
        stbiw__jpg_calcBits(DU[i], bits);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTAC[(nrzeroes << 4) + bits[1]]);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, bits);
    }
    if (end0pos != 63) {
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, EOB);
    }
    return DU[0];
}

static int stbi_write_jpg_core(stbi__write_context* s, int width, int height, int comp, const void* data, int quality) {
    // Constants that don't pollute global namespace
    static const unsigned char std_dc_luminance_nrcodes[] = { 0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0 };
    static const unsigned char std_dc_luminance_values[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
    static const unsigned char std_ac_luminance_nrcodes[] = { 0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d };
    static const unsigned char std_ac_luminance_values[] = {
       0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,
       0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
       0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
       0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
       0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,
       0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
       0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
    };
    static const unsigned char std_dc_chrominance_nrcodes[] = { 0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 };
    static const unsigned char std_dc_chrominance_values[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
    static const unsigned char std_ac_chrominance_nrcodes[] = { 0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77 };
    static const unsigned char std_ac_chrominance_values[] = {
       0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
       0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
       0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
       0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
       0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,
       0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
       0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
    };
    // Huffman tables
    static const unsigned short YDC_HT[256][2] = { {0,2},{2,3},{3,3},{4,3},{5,3},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9} };
    static const unsigned short UVDC_HT[256][2] = { {0,2},{1,2},{2,2},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9},{1022,10},{2046,11} };
    static const unsigned short YAC_HT[256][2] = {
       {10,4},{0,2},{1,2},{4,3},{11,4},{26,5},{120,7},{248,8},{1014,10},{65410,16},{65411,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {12,4},{27,5},{121,7},{502,9},{2038,11},{65412,16},{65413,16},{65414,16},{65415,16},{65416,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {28,5},{249,8},{1015,10},{4084,12},{65417,16},{65418,16},{65419,16},{65420,16},{65421,16},{65422,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {58,6},{503,9},{4085,12},{65423,16},{65424,16},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {59,6},{1016,10},{65430,16},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {122,7},{2039,11},{65438,16},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {123,7},{4086,12},{65446,16},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {250,8},{4087,12},{65454,16},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {504,9},{32704,15},{65462,16},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {505,9},{65470,16},{65471,16},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {506,9},{65479,16},{65480,16},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {1017,10},{65488,16},{65489,16},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {1018,10},{65497,16},{65498,16},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {2040,11},{65506,16},{65507,16},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {65515,16},{65516,16},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{0,0},{0,0},{0,0},{0,0},{0,0},
       {2041,11},{65525,16},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
    };
    static const unsigned short UVAC_HT[256][2] = {
       {0,2},{1,2},{4,3},{10,4},{24,5},{25,5},{56,6},{120,7},{500,9},{1014,10},{4084,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {11,4},{57,6},{246,8},{501,9},{2038,11},{4085,12},{65416,16},{65417,16},{65418,16},{65419,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {26,5},{247,8},{1015,10},{4086,12},{32706,15},{65420,16},{65421,16},{65422,16},{65423,16},{65424,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {27,5},{248,8},{1016,10},{4087,12},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{65430,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {58,6},{502,9},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{65438,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {59,6},{1017,10},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{65446,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {121,7},{2039,11},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{65454,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {122,7},{2040,11},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{65462,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {249,8},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{65470,16},{65471,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {503,9},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{65479,16},{65480,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {504,9},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{65488,16},{65489,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {505,9},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{65497,16},{65498,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {506,9},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{65506,16},{65507,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {2041,11},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{65515,16},{65516,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {16352,14},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{65525,16},{0,0},{0,0},{0,0},{0,0},{0,0},
       {1018,10},{32707,15},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
    };
    static const int YQT[] = { 16,11,10,16,24,40,51,61,12,12,14,19,26,58,60,55,14,13,16,24,40,57,69,56,14,17,22,29,51,87,80,62,18,22,
                              37,56,68,109,103,77,24,35,55,64,81,104,113,92,49,64,78,87,103,121,120,101,72,92,95,98,112,100,103,99 };
    static const int UVQT[] = { 17,18,24,47,99,99,99,99,18,21,26,66,99,99,99,99,24,26,56,99,99,99,99,99,47,66,99,99,99,99,99,99,
                               99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
    static const float aasf[] = { 1.0f * 2.828427125f, 1.387039845f * 2.828427125f, 1.306562965f * 2.828427125f, 1.175875602f * 2.828427125f,
                                  1.0f * 2.828427125f, 0.785694958f * 2.828427125f, 0.541196100f * 2.828427125f, 0.275899379f * 2.828427125f };

    int row, col, i, k, subsample;
    float fdtbl_Y[64], fdtbl_UV[64];
    unsigned char YTable[64], UVTable[64];

    if (!data || !width || !height || comp > 4 || comp < 1) {
        return 0;
    }

    quality = quality ? quality : 90;
    subsample = quality <= 90 ? 1 : 0;
    quality = quality < 1 ? 1 : quality > 100 ? 100 : quality;
    quality = quality < 50 ? 5000 / quality : 200 - quality * 2;

    for (i = 0; i < 64; ++i) {
        int uvti, yti = (YQT[i] * quality + 50) / 100;
        YTable[stbiw__jpg_ZigZag[i]] = (unsigned char)(yti < 1 ? 1 : yti > 255 ? 255 : yti);
        uvti = (UVQT[i] * quality + 50) / 100;
        UVTable[stbiw__jpg_ZigZag[i]] = (unsigned char)(uvti < 1 ? 1 : uvti > 255 ? 255 : uvti);
    }

    for (row = 0, k = 0; row < 8; ++row) {
        for (col = 0; col < 8; ++col, ++k) {
            fdtbl_Y[k] = 1 / (YTable[stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
            fdtbl_UV[k] = 1 / (UVTable[stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
        }
    }

    // Write Headers
    {
        static const unsigned char head0[] = { 0xFF,0xD8,0xFF,0xE0,0,0x10,'J','F','I','F',0,1,1,0,0,1,0,1,0,0,0xFF,0xDB,0,0x84,0 };
        static const unsigned char head2[] = { 0xFF,0xDA,0,0xC,3,1,0,2,0x11,3,0x11,0,0x3F,0 };
        const unsigned char head1[] = { 0xFF,0xC0,0,0x11,8,(unsigned char)(height >> 8),STBIW_UCHAR(height),(unsigned char)(width >> 8),STBIW_UCHAR(width),
                                        3,1,(unsigned char)(subsample ? 0x22 : 0x11),0,2,0x11,1,3,0x11,1,0xFF,0xC4,0x01,0xA2,0 };
        s->func(s->context, (void*)head0, sizeof(head0));
        s->func(s->context, (void*)YTable, sizeof(YTable));
        stbiw__putc(s, 1);
        s->func(s->context, UVTable, sizeof(UVTable));
        s->func(s->context, (void*)head1, sizeof(head1));
        s->func(s->context, (void*)(std_dc_luminance_nrcodes + 1), sizeof(std_dc_luminance_nrcodes) - 1);
        s->func(s->context, (void*)std_dc_luminance_values, sizeof(std_dc_luminance_values));
        stbiw__putc(s, 0x10); // HTYACinfo
        s->func(s->context, (void*)(std_ac_luminance_nrcodes + 1), sizeof(std_ac_luminance_nrcodes) - 1);
        s->func(s->context, (void*)std_ac_luminance_values, sizeof(std_ac_luminance_values));
        stbiw__putc(s, 1); // HTUDCinfo
        s->func(s->context, (void*)(std_dc_chrominance_nrcodes + 1), sizeof(std_dc_chrominance_nrcodes) - 1);
        s->func(s->context, (void*)std_dc_chrominance_values, sizeof(std_dc_chrominance_values));
        stbiw__putc(s, 0x11); // HTUACinfo
        s->func(s->context, (void*)(std_ac_chrominance_nrcodes + 1), sizeof(std_ac_chrominance_nrcodes) - 1);
        s->func(s->context, (void*)std_ac_chrominance_values, sizeof(std_ac_chrominance_values));
        s->func(s->context, (void*)head2, sizeof(head2));
    }

    // Encode 8x8 macroblocks
    {
        static const unsigned short fillBits[] = { 0x7F, 7 };
        int DCY = 0, DCU = 0, DCV = 0;
        int bitBuf = 0, bitCnt = 0;
        // comp == 2 is grey+alpha (alpha is ignored)
        int ofsG = comp > 2 ? 1 : 0, ofsB = comp > 2 ? 2 : 0;
        const unsigned char* dataR = (const unsigned char*)data;
        const unsigned char* dataG = dataR + ofsG;
        const unsigned char* dataB = dataR + ofsB;
        int x, y, pos;
        if (subsample) {
            for (y = 0; y < height; y += 16) {
                for (x = 0; x < width; x += 16) {
                    float Y[256], U[256], V[256];
                    for (row = y, pos = 0; row < y + 16; ++row) {
                        // row >= height => use last input row
                        int clamped_row = (row < height) ? row : height - 1;
                        int base_p = (stbi__flip_vertically_on_write ? (height - 1 - clamped_row) : clamped_row) * width * comp;
                        for (col = x; col < x + 16; ++col, ++pos) {
                            // if col >= width => use pixel from last input column
                            int p = base_p + ((col < width) ? col : (width - 1)) * comp;
                            float r = dataR[p], g = dataG[p], b = dataB[p];
                            Y[pos] = +0.29900f * r + 0.58700f * g + 0.11400f * b - 128;
                            U[pos] = -0.16874f * r - 0.33126f * g + 0.50000f * b;
                            V[pos] = +0.50000f * r - 0.41869f * g - 0.08131f * b;
                        }
                    }
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 0, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 8, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 128, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 136, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);

                    // subsample U,V
                    {
                        float subU[64], subV[64];
                        int yy, xx;
                        for (yy = 0, pos = 0; yy < 8; ++yy) {
                            for (xx = 0; xx < 8; ++xx, ++pos) {
                                int j = yy * 32 + xx * 2;
                                subU[pos] = (U[j + 0] + U[j + 1] + U[j + 16] + U[j + 17]) * 0.25f;
                                subV[pos] = (V[j + 0] + V[j + 1] + V[j + 16] + V[j + 17]) * 0.25f;
                            }
                        }
                        DCU = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, subU, 8, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
                        DCV = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, subV, 8, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
                    }
                }
            }
        }
        else {
            for (y = 0; y < height; y += 8) {
                for (x = 0; x < width; x += 8) {
                    float Y[64], U[64], V[64];
                    for (row = y, pos = 0; row < y + 8; ++row) {
                        // row >= height => use last input row
                        int clamped_row = (row < height) ? row : height - 1;
                        int base_p = (stbi__flip_vertically_on_write ? (height - 1 - clamped_row) : clamped_row) * width * comp;
                        for (col = x; col < x + 8; ++col, ++pos) {
                            // if col >= width => use pixel from last input column
                            int p = base_p + ((col < width) ? col : (width - 1)) * comp;
                            float r = dataR[p], g = dataG[p], b = dataB[p];
                            Y[pos] = +0.29900f * r + 0.58700f * g + 0.11400f * b - 128;
                            U[pos] = -0.16874f * r - 0.33126f * g + 0.50000f * b;
                            V[pos] = +0.50000f * r - 0.41869f * g - 0.08131f * b;
                        }
                    }

                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y, 8, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCU = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, U, 8, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
                    DCV = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, V, 8, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
                }
            }
        }

        // Do the bit alignment of the EOI marker
        stbiw__jpg_writeBits(s, &bitBuf, &bitCnt, fillBits);
    }

    // EOI
    stbiw__putc(s, 0xFF);
    stbiw__putc(s, 0xD9);

    return 1;
}

STBIWDEF int stbi_write_jpg_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int quality)
{
    stbi__write_context s = { 0 };
    stbi__start_write_callbacks(&s, func, context);
    return stbi_write_jpg_core(&s, x, y, comp, (void*)data, quality);
}


#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_jpg(char const* filename, int x, int y, int comp, const void* data, int quality)
{
    stbi__write_context s = { 0 };
    if (stbi__start_write_file(&s, filename)) {
        int r = stbi_write_jpg_core(&s, x, y, comp, data, quality);
        stbi__end_write_file(&s);
        return r;
    }
    else
        return 0;
}
#endif

#endif // STB_IMAGE_WRITE_IMPLEMENTATION

/* Revision history
      1.16  (2021-07-11)
             make Deflate code emit uncompressed blocks when it would otherwise expand
             support writing BMPs with alpha channel
      1.15  (2020-07-13) unknown
      1.14  (2020-02-02) updated JPEG writer to downsample chroma channels
      1.13
      1.12
      1.11  (2019-08-11)

      1.10  (2019-02-07)
             support utf8 filenames in Windows; fix warnings and platform ifdefs
      1.09  (2018-02-11)
             fix typo in zlib quality API, improve STB_I_W_STATIC in C++
      1.08  (2018-01-29)
             add stbi__flip_vertically_on_write, external zlib, zlib quality, choose PNG filter
      1.07  (2017-07-24)
             doc fix
      1.06 (2017-07-23)
             writing JPEG (using Jon Olick's code)
      1.05   ???
      1.04 (2017-03-03)
             monochrome BMP expansion
      1.03   ???
      1.02 (2016-04-02)
             avoid allocating large structures on the stack
      1.01 (2016-01-16)
             STBIW_REALLOC_SIZED: support allocators with no realloc support
             avoid race-condition in crc initialization
             minor compile issues
      1.00 (2015-09-14)
             installable file IO function
      0.99 (2015-09-13)
             warning fixes; TGA rle support
      0.98 (2015-04-08)
             added STBIW_MALLOC, STBIW_ASSERT etc
      0.97 (2015-01-18)
             fixed HDR asserts, rewrote HDR rle logic
      0.96 (2015-01-17)
             add HDR output
             fix monochrome BMP
      0.95 (2014-08-17)
             add monochrome TGA output
      0.94 (2014-05-31)
             rename private functions to avoid conflicts with stb_image.h
      0.93 (2014-05-27)
             warning fixes
      0.92 (2010-08-01)
             casts to unsigned char to fix warnings
      0.91 (2010-07-17)
             first public release
      0.90   first internal release
*/

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
}

namespace U
{

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
				image.data[index + 0] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(static_cast<float>(rgba.r) * mixture_factor + static_cast<float>(image.data[index + 0]) * inverse_mixture_factor))));
				image.data[index + 1] = static_cast<unsigned char>(std::min(255, std::max(0, static_cast<int>(static_cast<float>(rgba.g) * mixture_factor + static_cast<float>(image.data[index + 1]) * inverse_mixture_factor))));
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
}


//#include "Folder.h"
namespace U
{
    namespace Folder
    {
        int create_folder_if_does_not_exist_already_no_message(std::string folderPath);

        int create_folder_if_does_not_exist_already(std::string folderPath);

        std::vector<std::string> getFilePathsInFolder(const std::string& folderPath);

        bool fileExists(const std::string& path);
    }
}

namespace U
{
    namespace Folder
    {
        int create_folder_if_does_not_exist_already_no_message(std::string folderPath)
        {
            namespace fs = std::filesystem;
            if (!fs::exists(folderPath)) {
                if (fs::create_directory(folderPath)) {
                    // std::cout << "Folder created successfully." << std::endl;
                }
                else {
                    // std::cerr << "Failed to create folder." << std::endl;
                    return 1;
                }
            }
            else {
                // std::cout << "Folder already exists." << std::endl;
            }
        }

        int create_folder_if_does_not_exist_already(std::string folderPath)
        {
            namespace fs = std::filesystem;
            if (!fs::exists(folderPath)) {
                if (fs::create_directory(folderPath)) {
                    std::cout << "Folder created successfully." << std::endl;
                }
                else {
                    std::cerr << "Failed to create folder." << std::endl;
                    return 1;
                }
            }
            else {
                std::cout << "Folder already exists." << std::endl;
            }
        }

        std::vector<std::string> getFilePathsInFolder(const std::string& folderPath)
        {
            namespace fs = std::filesystem;
            std::vector<std::string> filePaths;

            try {
                for (const auto& entry : fs::directory_iterator(folderPath)) {
                    if (entry.is_regular_file()) {
                        filePaths.push_back(entry.path().string());
                    }
                }
            }
            catch (const fs::filesystem_error& e) {
                std::cerr << "Filesystem error: " << e.what() << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "General exception: " << e.what() << std::endl;
            }

            return filePaths;
        }


        bool fileExists(const std::string& path)
        {
            return std::filesystem::exists(path);
        }
    }
}


//#include "Experiment_32_Matrix.h"
namespace U
{


    namespace Experiment_32::Matrix
    {
        struct F3x1
        {
            union
            {
                float x11;
                float x;
            };

            union
            {
                float x21;
                float y;
            };

            union
            {
                float x31;
                float z;
            };

            F3x1(float x, float y, float z);
        };

        struct F3x3
        {
            float x11, x12, x13;
            float x21, x22, x23;
            float x31, x32, x33;
        };

        struct F4x1
        {
            float x11;
            float x21;
            float x31;
            float x41;
        };

        struct F4x4
        {
            float x11, x12, x13, x14;
            float x21, x22, x23, x24;
            float x31, x32, x33, x34;
            float x41, x42, x43, x44;
        };

        F3x1 multiply(const F3x3& left_matrix, const F3x1& right_matrix);
        F4x1 multiply(const F4x4& left_matrix, const F4x1& right_matrix);

        F3x3 multiply(const F3x3& a, const F3x3& b);
        F4x4 multiply(const F4x4& a, const F4x4& b);

        // Multiply F3x3 by F3x1
        F3x1 operator*(const F3x3& left_matrix, const F3x1& right_matrix);

        // Multiply F4x4 by F4x1
        F4x1 operator*(const F4x4& left_matrix, const F4x1& right_matrix);

        // Multiply F3x3 by F3x3
        F3x3 operator*(const F3x3& a, const F3x3& b);

        // Multiply F4x4 by F4x4
        F4x4 operator*(const F4x4& a, const F4x4& b);

        float dot_product(const F3x1& a, const F3x1& b);

        float dot_product(const F4x1& a, const F4x1& b);

        // cross product
        F3x1 cross_product(const F3x1& a, const F3x1& b);

        // determinants
        float determinant(const F3x3& matrix);

        float determinant(const F4x4& matrix);

        // f3x1
        float f3x1_length(const F3x1& matrix);

        // f3x3

        F3x3 f3x3_create_identity_matrix();
        F3x3 f3x3_create_2d_translation_matrix(float x, float y);
        F3x3 f3x3_create_2d_scaling_matrix(float sx, float sy);
        F3x3 f3x3_create_2d_reflection_x_axis();
        F3x3 f3x3_create_2d_reflection_y_axis();
        F3x3 f3x3_create_2d_shearing_x_axis(float angle_beta);
        F3x3 f3x3_create_2d_rotation_around_origin(float angle_beta);
        F3x3 f3x3_create_2d_rotation_around_arbitrary_point(float angle_beta, float px, float py);
        F3x3 f3x3_create_2d_reflection_arbitrary_axis_x(float ax);
        void f3x3_apply_matrix(const F3x3& matrix, float& x, float& y);


        // f4x4

        F4x4 f4x4_init_matrix_scale(float sx, float sy, float sz);
        F4x4 f4x4_init_matrix_scale(float sx, float sy, float sz);
        F4x4 f4x4_init_matrix_rotation_x(float angle);
        F4x4 f4x4_init_matrix_rotation_y(float angle);
        F4x4 f4x4_init_matrix_rotation_z(float angle);
        F4x4 f4x4_init_matrix_rotation(float angle_x, float angle_y, float angle_z);
        F4x4 f4x4_init_matrix_orthogrpahic_projection(float r, float l, float t, float b, float n, float f);
        F4x4 f4x4_init_prespective_transformation(float n, float f, float fn);


        // printing
        // Overloaded << operator for F3x1
        std::ostream& operator<<(std::ostream& os, const F3x1& matrix);

        // Overloaded << operator for F3x3
        std::ostream& operator<<(std::ostream& os, const F3x3& matrix);

        // Overloaded << operator for F4x1
        std::ostream& operator<<(std::ostream& os, const F4x1& matrix);

        // Overloaded << operator for F4x4
        std::ostream& operator<<(std::ostream& os, const F4x4& matrix);


        // float

        float f_float_calculate_the_angle_for_xy_in_radians(float x, float y);
        float f_float_calculate_the_angle_for_xy_in_degrees(float x, float y);
        bool f_float_equals(float a, float b, float epsilon = 0.0001f);
        float f_float_interpolate(const float min_value, const float max_value, const float factor);

        namespace Test_
        {
            void run_tests();
        }
    }
}

namespace U
{
    namespace Experiment_32::Matrix
    {
        // Constructors
        F3x1::F3x1(float x, float y, float z)
            : x(x), y(y), z(z) {}


        F3x1 multiply(const F3x3& left_matrix, const F3x1& right_matrix)
        {
            return F3x1
            (
                left_matrix.x11 * right_matrix.x11 + left_matrix.x12 * right_matrix.x21 + left_matrix.x13 * right_matrix.x31,
                left_matrix.x21 * right_matrix.x11 + left_matrix.x22 * right_matrix.x21 + left_matrix.x23 * right_matrix.x31,
                left_matrix.x31 * right_matrix.x11 + left_matrix.x32 * right_matrix.x21 + left_matrix.x33 * right_matrix.x31
            );
        }

        F4x1 multiply(const F4x4& left_matrix, const F4x1& right_matrix)
        {
            return F4x1(
                left_matrix.x11 * right_matrix.x11 + left_matrix.x12 * right_matrix.x21 + left_matrix.x13 * right_matrix.x31 + left_matrix.x14 * right_matrix.x41,
                left_matrix.x21 * right_matrix.x11 + left_matrix.x22 * right_matrix.x21 + left_matrix.x23 * right_matrix.x31 + left_matrix.x24 * right_matrix.x41,
                left_matrix.x31 * right_matrix.x11 + left_matrix.x32 * right_matrix.x21 + left_matrix.x33 * right_matrix.x31 + left_matrix.x34 * right_matrix.x41,
                left_matrix.x41 * right_matrix.x11 + left_matrix.x42 * right_matrix.x21 + left_matrix.x43 * right_matrix.x31 + left_matrix.x44 * right_matrix.x41
            );
        }

        F3x3 multiply(const F3x3& a, const F3x3& b)
        {
            return F3x3{
                a.x11 * b.x11 + a.x12 * b.x21 + a.x13 * b.x31,
                a.x11 * b.x12 + a.x12 * b.x22 + a.x13 * b.x32,
                a.x11 * b.x13 + a.x12 * b.x23 + a.x13 * b.x33,

                a.x21 * b.x11 + a.x22 * b.x21 + a.x23 * b.x31,
                a.x21 * b.x12 + a.x22 * b.x22 + a.x23 * b.x32,
                a.x21 * b.x13 + a.x22 * b.x23 + a.x23 * b.x33,

                a.x31 * b.x11 + a.x32 * b.x21 + a.x33 * b.x31,
                a.x31 * b.x12 + a.x32 * b.x22 + a.x33 * b.x32,
                a.x31 * b.x13 + a.x32 * b.x23 + a.x33 * b.x33
            };
        }

        F4x4 multiply(const F4x4& a, const F4x4& b)
        {
            return F4x4{
                 a.x11 * b.x11 + a.x12 * b.x21 + a.x13 * b.x31 + a.x14 * b.x41,
                 a.x11 * b.x12 + a.x12 * b.x22 + a.x13 * b.x32 + a.x14 * b.x42,
                 a.x11 * b.x13 + a.x12 * b.x23 + a.x13 * b.x33 + a.x14 * b.x43,
                 a.x11 * b.x14 + a.x12 * b.x24 + a.x13 * b.x34 + a.x14 * b.x44,

                 a.x21 * b.x11 + a.x22 * b.x21 + a.x23 * b.x31 + a.x24 * b.x41,
                 a.x21 * b.x12 + a.x22 * b.x22 + a.x23 * b.x32 + a.x24 * b.x42,
                 a.x21 * b.x13 + a.x22 * b.x23 + a.x23 * b.x33 + a.x24 * b.x43,
                 a.x21 * b.x14 + a.x22 * b.x24 + a.x23 * b.x34 + a.x24 * b.x44,

                 a.x31 * b.x11 + a.x32 * b.x21 + a.x33 * b.x31 + a.x34 * b.x41,
                 a.x31 * b.x12 + a.x32 * b.x22 + a.x33 * b.x32 + a.x34 * b.x42,
                 a.x31 * b.x13 + a.x32 * b.x23 + a.x33 * b.x33 + a.x34 * b.x43,
                 a.x31 * b.x14 + a.x32 * b.x24 + a.x33 * b.x34 + a.x34 * b.x44,

                 a.x41 * b.x11 + a.x42 * b.x21 + a.x43 * b.x31 + a.x44 * b.x41,
                 a.x41 * b.x12 + a.x42 * b.x22 + a.x43 * b.x32 + a.x44 * b.x42,
                 a.x41 * b.x13 + a.x42 * b.x23 + a.x43 * b.x33 + a.x44 * b.x43,
                 a.x41 * b.x14 + a.x42 * b.x24 + a.x43 * b.x34 + a.x44 * b.x44
            };
        }

        F3x1 operator*(const F3x3& left_matrix, const F3x1& right_matrix)
        {
            return multiply(left_matrix, right_matrix);
        }

        F4x1 operator*(const F4x4& left_matrix, const F4x1& right_matrix)
        {
            return multiply(left_matrix, right_matrix);
        }

        F3x3 operator*(const F3x3& a, const F3x3& b)
        {
            return multiply(a, b);
        }

        F4x4 operator*(const F4x4& a, const F4x4& b)
        {
            return multiply(a, b);
        }

        // Dot product

        float dot_product(const F3x1& a, const F3x1& b)
        {
            return
                a.x11 * b.x11 +
                a.x21 * b.x21 +
                a.x31 * b.x31;
        }

        float dot_product(const F4x1& a, const F4x1& b)
        {
            return
                a.x11 * b.x11 +
                a.x21 * b.x21 +
                a.x31 * b.x31 +
                a.x41 * b.x41;
        }

        // cross product

        F3x1 cross_product(const F3x1& a, const F3x1& b)
        {
            return
                F3x1
                (
                    a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x
                );
        }


        // Determimants
        float determinant(const F3x3& matrix)
        {
            return matrix.x11 * (matrix.x22 * matrix.x33 - matrix.x23 * matrix.x32)
                - matrix.x12 * (matrix.x21 * matrix.x33 - matrix.x23 * matrix.x31)
                + matrix.x13 * (matrix.x21 * matrix.x32 - matrix.x22 * matrix.x31);
        }

        float determinant_4x4_a11(const F4x4& matrix)
        {
            return determinant
            (
                F3x3
                (
                    matrix.x22, matrix.x23, matrix.x24,
                    matrix.x32, matrix.x33, matrix.x34,
                    matrix.x42, matrix.x43, matrix.x44
                )
            );
        }

        float determinant_4x4_a12(const F4x4& matrix)
        {
            return determinant
            (
                F3x3
                (
                    matrix.x21, matrix.x23, matrix.x24,
                    matrix.x31, matrix.x33, matrix.x34,
                    matrix.x41, matrix.x43, matrix.x44
                )
            );
        }

        float determinant_4x4_a13(const F4x4& matrix)
        {
            return determinant
            (
                F3x3
                (
                    matrix.x21, matrix.x22, matrix.x24,
                    matrix.x31, matrix.x32, matrix.x34,
                    matrix.x41, matrix.x42, matrix.x44
                )
            );
        }

        float determinant_4x4_a14(const F4x4& matrix)
        {
            return determinant
            (
                F3x3
                (
                    matrix.x21, matrix.x22, matrix.x23,
                    matrix.x31, matrix.x32, matrix.x33,
                    matrix.x41, matrix.x42, matrix.x43
                )
            );
        }

        float determinant(const F4x4& matrix)
        {
            return
                (
                    (matrix.x11 * determinant_4x4_a11(matrix)) +
                    (-matrix.x12 * determinant_4x4_a12(matrix)) +
                    (matrix.x13 * determinant_4x4_a13(matrix)) +
                    (-matrix.x14 * determinant_4x4_a14(matrix))
                    );
        }

        // 3x1

        float f3x1_length(const F3x1& matrix)
        {
            return std::sqrtf(dot_product(matrix, matrix));
        }


        // 3x3

        F3x3 f3x3_create_identity_matrix()
        {
            return F3x3
            (
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_translation_matrix(float x, float y)
        {
            return F3x3
            (
                1.0f, 0.0f, x,
                0.0f, 1.0f, y,
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_scaling_matrix(float sx, float sy)
        {
            return F3x3
            (
                sx, 0.0f, 0.0f,
                0.0f, sy, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_reflection_x_axis()
        {
            return F3x3
            (
                1.0f, 0.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_reflection_y_axis()
        {
            return F3x3
            (
                -1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_shearing_x_axis(float angle_beta)
        {
            return F3x3
            (
                1.0f, std::tanf(angle_beta), 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_rotation_around_origin(float angle_beta)
        {
            return F3x3
            (
                std::cosf(angle_beta), -std::sinf(angle_beta), 0.0f,
                std::sinf(angle_beta), std::cosf(angle_beta), 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_rotation_around_arbitrary_point(float angle_beta, float px, float py)
        {
            // To rotate  point (x, y) about an arbitrary point (px, py) we first subtract (px, py) from the coordinates (x, y)
            // This enables us to perform the rotation about the origin.
            // Second, we perform the rotation, and third we 
            // add (px, py) to compensate for the original subtraction.

            // * we get the point to origin 
            // * perform rotation
            // * move the point back where it was
            // 

            // you should get the same answer if you apply this formula
            // [translate(px, py)] * [rotate beta] * [translate(-px, -py)] * [x, y, 1]

            return F3x3
            (
                std::cosf(angle_beta), -std::sinf(angle_beta), px * (1.0f - std::cosf(angle_beta)) + py * std::sinf(angle_beta),
                std::sinf(angle_beta), std::cosf(angle_beta), py * (1.0f - std::cosf(angle_beta)) - px * std::sinf(angle_beta),
                0.0f, 0.0f, 1.0f
            );
        }

        F3x3 f3x3_create_2d_reflection_arbitrary_axis_x(float ax)
        {
            // TODO try if you get the same if you construct the matrix this way
            // muliply(multiply(translate(ax, 0), multiply(recletion(), translate(-ax, 0))), matrix(x, y, 1))

            return F3x3
            (
                -1.0f, 0.0f, 2.0f * ax,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        void f3x3_apply_matrix(const F3x3& matrix, float& x, float& y)
        {
            F3x3 new_xy = multiply(matrix, F3x3(x, y, 1.0f));
            x = new_xy.x11;
            y = new_xy.x21;
        }

        // 4x4

        F4x4 f4x4_init_matrix_scale(float sx, float sy, float sz)
        {
            return
                F4x4
                (
                    sx, 0, 0, 0,
                    0, sy, 0, 0,
                    0, 0, sz, 0,
                    0, 0, 0, 1
                );
        }

        F4x4 f4x4_init_matrix_translate(float tx, float ty, float tz)
        {
            return
                F4x4
                (
                    1, 0, 0, tx,
                    0, 1, 0, ty,
                    0, 0, 1, tz,
                    0, 0, 0, 1
                );
        }

        F4x4 f4x4_init_matrix_rotation_x(float angle)
        {
            return
                F4x4
                (
                    1, 0, 0, 0,
                    0, std::cosf(angle), std::sinf(angle), 0,
                    0, -std::sinf(angle), std::cosf(angle), 0,
                    0, 0, 0, 1
                );
        }

        F4x4 f4x4_init_matrix_rotation_y(float angle)
        {
            return
                F4x4
                (
                    std::cosf(angle), 0, -std::sinf(angle), 0,
                    0, 1, 0, 0,
                    std::sinf(angle), 0, std::cosf(angle), 0,
                    0, 0, 0, 1
                );
        }

        F4x4 f4x4_init_matrix_rotation_z(float angle)
        {
            return
                F4x4
                (
                    std::cosf(angle), std::sinf(angle), 0, 0,
                    -std::sinf(angle), std::cosf(angle), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                );
        }

        F4x4 f4x4_init_matrix_rotation(float angle_rotation_x, float angle_rotation_y, float angle_rotation_z)
        {
            // R = Rz(alfa) * Ry(beta) * Rx(gama)

            // rotation around x than
            // rotation around y than
            // rotation around z

            // matrix4x4 * matrix4x4 * matrix4x4 * vector4x1

            // we first apply x, than we apply y, and than we apply z
            // x rool
            // y pitch
            // y yaw

            // you read from the back <-
            // the matrix on the right is the first matrix that's applied and than next one and than next one
            //return F4x4();

            return f4x4_init_matrix_rotation_z(angle_rotation_z) *
                f4x4_init_matrix_rotation_y(angle_rotation_y) *
                f4x4_init_matrix_rotation_x(angle_rotation_x);
        }

        // viewing projeciton transforms 

    // Object / Model Space
    // Scene / World Space

    // To define camera transformation 
    // we usually define the position of camera and the direction that the camera is looking at and what is our up vector this way
    // we can get the camera camera x, y, z coordinates in world space and if I know this vectors in world space 
    // Than I can easly construct the transformation matrix that would give me the transformation

    // Projection transformation
    // 
    // View / Camera Space -> Canonical View Volume ( from - 1.0, to 1.0 in all 3 axis )
    // Ortopgraphic projection
    //

    // right
    // left
    // top
    // botton
    // near
    // far
        F4x4 f4x4_init_matrix_orthogrpahic_projection(float r, float l, float t, float b, float n, float f)
        {
            return
                F4x4
                (
                    2.0f / (r - l), 0, 0, -((r + l) / (r - l)),
                    0, 2.0f / (t - b), 0, -((t + b) / (t - b)),
                    0, 0, 2.0 / (f - n), -((f + n) / (f - n)),
                    0, 0, 0, 1
                );
        }


        // if you combine prespective transformation and than ortographic transformation we get the prespective projection

    // Homogeneous Coordinates
    // 
    // | px |          | alfa * px |
    // | py |   equal  | alfa * py |
    // | pz |          | alfa * pz |
    // |  1 |          |      alfa |
    // 

        F4x4 f4x4_init_prespective_transformation(float n, float f, float fn)
        {
            return
                F4x4
                (
                    n, 0, 0, 0,
                    0, n, 0, 0,
                    0, 0, n + f, -fn,
                    0, 0, 1, 0
                );
        }

        // we first apply prespective transformation 
        // than we apply ortographic projection


        // printing
        std::ostream& operator<<(std::ostream& os, const F3x1& matrix)
        {
            os << "[" << matrix.x11 << "]\n"
                << "[" << matrix.x21 << "]\n"
                << "[" << matrix.x31 << "]";
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const F3x3& matrix)
        {
            os << "[" << matrix.x11 << ", " << matrix.x12 << ", " << matrix.x13 << "]\n"
                << "[" << matrix.x21 << ", " << matrix.x22 << ", " << matrix.x23 << "]\n"
                << "[" << matrix.x31 << ", " << matrix.x32 << ", " << matrix.x33 << "]";
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const F4x1& matrix)
        {
            os << "[" << matrix.x11 << "]\n"
                << "[" << matrix.x21 << "]\n"
                << "[" << matrix.x31 << "]\n"
                << "[" << matrix.x41 << "]";
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const F4x4& matrix)
        {
            os << "[" << matrix.x11 << ", " << matrix.x12 << ", " << matrix.x13 << ", " << matrix.x14 << "]\n"
                << "[" << matrix.x21 << ", " << matrix.x22 << ", " << matrix.x23 << ", " << matrix.x24 << "]\n"
                << "[" << matrix.x31 << ", " << matrix.x32 << ", " << matrix.x33 << ", " << matrix.x34 << "]\n"
                << "[" << matrix.x41 << ", " << matrix.x42 << ", " << matrix.x43 << ", " << matrix.x44 << "]";
            return os;
        }

        float f_float_calculate_the_angle_for_xy_in_radians(float x, float y)
        {
            return atan2(y, x);
        }

        float f_float_calculate_the_angle_for_xy_in_degrees(float x, float y)
        {
            return f_float_calculate_the_angle_for_xy_in_radians(x, y) * (180.0f / std::numbers::pi); // Convert the angle to degrees
        }

        bool f_float_equals(float a, float b, float epsilon)
        {
            return std::fabs(a - b) < epsilon;
        }

        float f_float_interpolate(const float min_value, const float max_value, const float factor)
        {
            return min_value * factor + max_value * (1.0f - factor);
        }

        // tests
        namespace Test_
        {
            void check(bool condition, const char* testName)
            {
                if (!condition)
                {
                    std::cerr << "Test failed: " << testName << std::endl;
                    // std::exit(EXIT_FAILURE);
                }
            }

            void run_tests()
            {
                std::cout << "run_tests\n";

                // Testing Multiply

                {
                    // testMultiply3x3With3x1
                    {
                        Matrix::F3x3 left_matrix = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                        Matrix::F3x1 right_matrix = { 1, 2, 3 };
                        Matrix::F3x1 expected_result = { 14, 32, 50 }; // Calculated as (1*1 + 2*2 + 3*3, 4*1 + 5*2 + 6*3, 7*1 + 8*2 + 9*3)

                        Matrix::F3x1 result = multiply(left_matrix, right_matrix);

                        if (Matrix::f_float_equals(result.x11, expected_result.x11) &&
                            Matrix::f_float_equals(result.x21, expected_result.x21) &&
                            Matrix::f_float_equals(result.x31, expected_result.x31))
                        {
                            // std::cout << "testMultiply3x3With3x1 PASSED" << std::endl;
                        }
                        else {
                            std::cout << "testMultiply3x3With3x1 FAILED" << std::endl;
                        }
                    }

                    // testMultiply3x3With3x1
                    {
                        Matrix::F3x3 left_matrix = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                        Matrix::F3x1 right_matrix = { 1, 2, 3 };
                        Matrix::F3x1 expected_result = { 14, 32, 50 }; // Calculated as (1*1 + 2*2 + 3*3, 4*1 + 5*2 + 6*3, 7*1 + 8*2 + 9*3)

                        Matrix::F3x1 result = multiply(left_matrix, right_matrix);

                        if (Matrix::f_float_equals(result.x11, expected_result.x11) &&
                            Matrix::f_float_equals(result.x21, expected_result.x21) &&
                            Matrix::f_float_equals(result.x31, expected_result.x31))
                        {
                            // std::cout << "testMultiply3x3With3x1 PASSED" << std::endl;
                        }
                        else {
                            std::cout << "testMultiply3x3With3x1 FAILED" << std::endl;
                        }
                    }

                    // testMultiply4x4With4x1
                    {
                        Matrix::F4x4 left_matrix = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
                        Matrix::F4x1 right_matrix = { 1, 2, 3, 4 };
                        Matrix::F4x1 expected_result = { 30, 70, 110, 150 }; // Calculated as (1*1 + 2*2 + 3*3 + 4*4, 5*1 + 6*2 + 7*3 + 8*4, 9*1 + 10*2 + 11*3 + 12*4, 13*1 + 14*2 + 15*3 + 16*4)

                        Matrix::F4x1 result = multiply(left_matrix, right_matrix);

                        if (
                            Matrix::f_float_equals(result.x11, expected_result.x11) &&
                            Matrix::f_float_equals(result.x21, expected_result.x21) &&
                            Matrix::f_float_equals(result.x31, expected_result.x31) &&
                            Matrix::f_float_equals(result.x41, expected_result.x41)
                            )
                        {
                            // std::cout << "testMultiply4x4With4x1 PASSED" << std::endl;
                        }
                        else
                        {
                            std::cout << "testMultiply4x4With4x1 FAILED" << std::endl;
                        }
                    }

                    // testMultiply3x3With3x3
                    {
                        Matrix::F3x3 a = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                        Matrix::F3x3 b = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
                        Matrix::F3x3 expected_result = { 30, 24, 18, 84, 69, 54, 138, 114, 90 }; // Calculated as matrix multiplication

                        Matrix::F3x3 result = multiply(a, b);

                        if (
                            Matrix::f_float_equals(result.x11, expected_result.x11) &&
                            Matrix::f_float_equals(result.x12, expected_result.x12) &&
                            Matrix::f_float_equals(result.x13, expected_result.x13) &&
                            Matrix::f_float_equals(result.x21, expected_result.x21) &&
                            Matrix::f_float_equals(result.x22, expected_result.x22) &&
                            Matrix::f_float_equals(result.x23, expected_result.x23) &&
                            Matrix::f_float_equals(result.x31, expected_result.x31) &&
                            Matrix::f_float_equals(result.x32, expected_result.x32) &&
                            Matrix::f_float_equals(result.x33, expected_result.x33)
                            )
                        {
                            // std::cout << "testMultiply3x3With3x3 PASSED" << std::endl;
                        }
                        else
                        {
                            std::cout << "testMultiply3x3With3x3 FAILED" << std::endl;
                        }
                    }

                    // testMultiply4x4With4x4
                    {
                        Matrix::F4x4 a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
                        Matrix::F4x4 b = { 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
                        Matrix::F4x4 expected_result = { 80, 70, 60, 50, 240, 214, 188, 162, 400, 358, 316, 274, 560, 502, 444, 386 }; // Calculated as matrix multiplication

                        Matrix::F4x4 result = multiply(a, b);

                        if (
                            Matrix::f_float_equals(result.x11, expected_result.x11) &&
                            Matrix::f_float_equals(result.x12, expected_result.x12) &&
                            Matrix::f_float_equals(result.x13, expected_result.x13) &&
                            Matrix::f_float_equals(result.x14, expected_result.x14) &&
                            Matrix::f_float_equals(result.x21, expected_result.x21) &&
                            Matrix::f_float_equals(result.x22, expected_result.x22) &&
                            Matrix::f_float_equals(result.x23, expected_result.x23) &&
                            Matrix::f_float_equals(result.x24, expected_result.x24) &&
                            Matrix::f_float_equals(result.x31, expected_result.x31) &&
                            Matrix::f_float_equals(result.x32, expected_result.x32) &&
                            Matrix::f_float_equals(result.x33, expected_result.x33) &&
                            Matrix::f_float_equals(result.x34, expected_result.x34) &&
                            Matrix::f_float_equals(result.x41, expected_result.x41) &&
                            Matrix::f_float_equals(result.x42, expected_result.x42) &&
                            Matrix::f_float_equals(result.x43, expected_result.x43) &&
                            Matrix::f_float_equals(result.x44, expected_result.x44)
                            )
                        {
                            // std::cout << "testMultiply4x4With4x4 PASSED" << std::endl;
                        }
                        else
                        {
                            std::cout << "testMultiply4x4With4x4 FAILED" << std::endl;
                        }
                    }
                }


                {
                    // Test 1: Identity matrix
                    Matrix::F3x3 identity = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
                    check(Matrix::f_float_equals(determinant(identity), 1.0f), "Identity matrix");

                    // Test 2: Zero matrix
                    Matrix::F3x3 zero = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                    check(Matrix::f_float_equals(determinant(zero), 0.0f), "Zero matrix");

                    // Test 3: Known determinant
                    Matrix::F3x3 matrix1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                    check(Matrix::f_float_equals(determinant(matrix1), 0.0f), "Known determinant 1");

                    // Test 4: Another known determinant
                    Matrix::F3x3 matrix2 = { 6, 1, 1, 4, -2, 5, 2, 8, 7 };
                    check(Matrix::f_float_equals(determinant(matrix2), -306.0f), "Known determinant 2");

                    // Test 5: Random matrix
                    Matrix::F3x3 matrix3 = { 3, 2, -1, 1, 0, 2, -2, 1, 3 };
                    check(Matrix::f_float_equals(determinant(matrix3), -21.0f), "Random matrix");
                }

                {
                    {
                        Matrix::F4x4 identityMatrix = { 1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 1 };

                        check(Matrix::f_float_equals(determinant(identityMatrix), 1.0f), "Identity Matrix Determinant");
                    }

                    {
                        Matrix::F4x4 zeroMatrix = {
                                      0, 0, 0, 0,
                                      0, 0, 0, 0,
                                      0, 0, 0, 0,
                                      0, 0, 0, 0
                        };

                        check(Matrix::f_float_equals(determinant(zeroMatrix), 0.0f), "Zero Matrix Determinant");
                    }


                    {
                        Matrix::F4x4 arbitraryMatrix = {
                            1, 2, 3, 4,
                                            5, 6, 7, 8,
                                            9, 10, 11, 12,
                                            13, 14, 15, 16
                        };
                        check(Matrix::f_float_equals(determinant(arbitraryMatrix), 0.0f), "Arbitrary Matrix Determinant");
                    }


                    {
                        Matrix::F4x4 singularMatrix =
                        {
                            1, 2, 3, 4,
                                   2, 4, 6, 8,
                                   3, 6, 9, 12,
                                   4, 8, 12, 16
                        };

                        check(Matrix::f_float_equals(determinant(singularMatrix), 0.0f), "Singular Matrix Determinant");
                    }




                    {

                        // Example 4x4 matrix
                        Matrix::F4x4 known_matrix_0 =
                        {
                             2.0f,  1.0f, 3.0f, 4.0f,
                             0.0f, -1.0f, 2.0f, 1.0f,
                             3.0f,  2.0f, 0.0f, 5.0f,
                            -1.0f,  3.0f, 2.0f, 1.0f
                        };

                        check(Matrix::f_float_equals(determinant(known_matrix_0), 35.0f), "Known Matrix Determinant");
                    }

                }


                // dot product test
                {
                    {
                        Matrix::F3x1 matrix_a = { 0.6f, -0.8f, 0.0f };
                        Matrix::F3x1 matrix_b = { 0.0f,  1.0f, 0.0f };
                        check(Matrix::f_float_equals(dot_product(matrix_a, matrix_b), -0.8f), "Dot product 0");
                    }

                    {
                        Matrix::F4x1 matrix_a = { 0.6f, -0.8f, 0.0f, 1.2f };
                        Matrix::F4x1 matrix_b = { 0.0f,  1.0f, 0.5f, -0.2f };
                        check(Matrix::f_float_equals(dot_product(matrix_a, matrix_b), -1.04), "Dot product 1");
                    }
                }

                // cross product test
                {
                    Matrix::F3x1 matrix_a = { 2.0f, -3.0f, 1.0f };
                    Matrix::F3x1 matrix_b = { 4.0f, -1.0f, 5.0f };
                    Matrix::F3x1 matrix_a_cross_b = cross_product(matrix_a, matrix_b);

                    check(Matrix::f_float_equals(matrix_a_cross_b.x, -14.0f), "cross product test 0 x");
                    check(Matrix::f_float_equals(matrix_a_cross_b.y, -6.0f), "cross product test 0 y");
                    check(Matrix::f_float_equals(matrix_a_cross_b.z, 10.0f), "cross product test 0 z");
                }

                // matrix 2d translate 
                {
                    {
                        Matrix::F3x3 matrix_transform = Matrix::f3x3_create_2d_translation_matrix(4.7f, 0.0f);
                        Matrix::F3x1 matrix_out = multiply(matrix_transform, Matrix::F3x1(0.0f, 0.0f, 1.0f));
                        check(Matrix::f_float_equals(matrix_out.x, 4.7), "matrix translate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 0.0), "matrix translate y test");
                    }

                    {
                        Matrix::F3x3 matrix_transform = Matrix::f3x3_create_2d_translation_matrix(0.0f, 2.7f);
                        Matrix::F3x1 matrix_out = multiply(matrix_transform, Matrix::F3x1(0.0f, 2.1f, 1.0f));
                        check(Matrix::f_float_equals(matrix_out.x, 0.0), "matrix translate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 4.8), "matrix translate y test");
                    }

                    {
                        Matrix::F3x3 matrix_transform = Matrix::f3x3_create_2d_translation_matrix(0.7f, 0.2f);
                        Matrix::F3x1 matrix_out = multiply(matrix_transform, Matrix::F3x1(10.0f, 10.0f, 1.0f));
                        check(Matrix::f_float_equals(matrix_out.x, 10.7f), "matrix translate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 10.2f), "matrix translate y test");
                    }

                    {
                        Matrix::F3x3 matrix_transform_0 = Matrix::f3x3_create_2d_translation_matrix(0.7f, 0.2f);
                        Matrix::F3x3 matrix_transform_1 = Matrix::f3x3_create_2d_translation_matrix(132.0f, 164.f);
                        Matrix::F3x1 matrix_out = multiply(matrix_transform_0 * matrix_transform_1, Matrix::F3x1(10.0f, 10.0f, 1.0f));
                        check(Matrix::f_float_equals(matrix_out.x, 0.7f + 132.0f + 10.0f), "matrix translate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 0.2f + 164.0f + 10.0f), "matrix translate y test");
                    }
                }

                // matrix 2d scale
                {
                    {
                        Matrix::F3x3 matrix_scale = Matrix::f3x3_create_2d_scaling_matrix(10.0f, 7.0f);
                        Matrix::F3x1 matrix_out = matrix_scale * Matrix::F3x1(4.0f, 172.0f, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, 4.0f * 10.0f), "matrix scale x test");
                        check(Matrix::f_float_equals(matrix_out.y, 172.0f * 7.0f), "matrix scale y test");
                    }

                    {
                        Matrix::F3x3 matrix_scale_0 = Matrix::f3x3_create_2d_scaling_matrix(10.0f, 0.7f);
                        Matrix::F3x3 matrix_scale_1 = Matrix::f3x3_create_2d_scaling_matrix(148.0f, 0.7474f);
                        Matrix::F3x1 matrix_out = matrix_scale_1 * matrix_scale_0 * Matrix::F3x1(10.27f, 0.478f, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, 10.27f * 148.0f * 10.0f), "matrix scale x test");
                        check(Matrix::f_float_equals(matrix_out.y, 0.478f * 0.7474f * 0.7f), "matrix scale y test");
                    }
                }

                // scale than translate
                {
                    {
                        Matrix::F3x3 matrix_scale = Matrix::f3x3_create_2d_scaling_matrix(4.7, 2.28f);
                        Matrix::F3x3 matrix_translate = Matrix::f3x3_create_2d_translation_matrix(8.0f, 17.0f);
                        Matrix::F3x1 matrix_out = matrix_translate * matrix_scale * Matrix::F3x1(0.4f, 7.8f, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, (0.4f * 4.7f) + 8.0f), "matrix scale than translate x test");
                        check(Matrix::f_float_equals(matrix_out.y, (7.8f * 2.28f) + 17.0f), "matrix scale than translate y test");
                    }
                }

                // translate than scale
                {
                    Matrix::F3x3 matrix_scale = Matrix::f3x3_create_2d_scaling_matrix(4.7, 2.28f);
                    Matrix::F3x3 matrix_translate = Matrix::f3x3_create_2d_translation_matrix(8.0f, 17.0f);
                    Matrix::F3x1 matrix_out = matrix_scale * matrix_translate * Matrix::F3x1(0.4f, 7.8f, 1.0f);
                    check(Matrix::f_float_equals(matrix_out.x, (0.4f + 8.0f) * 4.7f), "matrix translate than scale x test");
                    check(Matrix::f_float_equals(matrix_out.y, (7.8f + 17.0f) * 2.28f), "matrix translate than scale y test");
                }

                // rotate
                {
                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin(std::numbers::pi / 2.0f);
                        const float x = 1.0f;
                        const float y = 0.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, 0.0f), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, x), "matrix rotate y test");
                    }

                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin(std::numbers::pi);
                        const float x = 1.0f;
                        const float y = 0.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, -x), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 0.0f), "matrix rotate y test");
                    }

                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin((std::numbers::pi / 2.0f) * 3.0f);
                        const float x = 1.0f;
                        const float y = 0.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, 0.0), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, -x), "matrix rotate y test");
                    }

                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin(std::numbers::pi * 2.0f);
                        const float x = 1.0f;
                        const float y = 0.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, x), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 0.0f), "matrix rotate y test");
                    }




                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin(std::numbers::pi / 2.0f);
                        const float x = 0.0f;
                        const float y = 1.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, -y), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 0.0f), "matrix rotate y test");
                    }

                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin(std::numbers::pi);
                        const float x = 0.0f;
                        const float y = 1.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, 0.0f), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, -y), "matrix rotate y test");
                    }

                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin((std::numbers::pi / 2.0f) * 3.0f);
                        const float x = 0.0f;
                        const float y = 1.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, y), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, 0.0f), "matrix rotate y test");
                    }

                    {
                        Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin(std::numbers::pi * 2.0f);
                        const float x = 0.0f;
                        const float y = 1.0f;
                        Matrix::F3x1 matrix_out = matrix_rotate * Matrix::F3x1(x, y, 1.0f);
                        check(Matrix::f_float_equals(matrix_out.x, 0.0f), "matrix rotate x test");
                        check(Matrix::f_float_equals(matrix_out.y, y), "matrix rotate y test");
                    }

                }

                // scale, rotate, translate
                {
                    const float scale_x = 4.7;
                    const float scale_y = 2.28f;
                    const float translate_x = 8.0f;
                    const float translate_y = 17.0f;
                    const float rotation_angle = std::numbers::pi;

                    const float input_x = 0.4f;
                    const float input_y = 7.8f;

                    Matrix::F3x3 matrix_scale = Matrix::f3x3_create_2d_scaling_matrix(scale_x, scale_y);
                    Matrix::F3x3 matrix_translate = Matrix::f3x3_create_2d_translation_matrix(translate_x, translate_y);
                    Matrix::F3x3 matrix_rotate = Matrix::f3x3_create_2d_rotation_around_origin(rotation_angle);
                    Matrix::F3x1 matrix_out = matrix_translate * matrix_rotate * matrix_scale * Matrix::F3x1(input_x, input_y, 1.0f);

                    // scale test
                    {
                        Matrix::F3x1 matrix_tmp = matrix_scale * Matrix::F3x1(input_x, input_y, 1.0f);

                        check(Matrix::f_float_equals(matrix_tmp.x, input_x * scale_x), "matrix scale, rotate, translate x scale test");
                        check(Matrix::f_float_equals(matrix_tmp.y, input_y * scale_y), "matrix scale, rotate, translate y scale test");
                    }

                    // scale than rotate test
                    {

                        Matrix::F3x1 matrix_tmp = matrix_rotate * matrix_scale * Matrix::F3x1(input_x, input_y, 1.0f);

                        float angle_of_xy = Matrix::f_float_calculate_the_angle_for_xy_in_radians((input_x * scale_x), (input_y * scale_y));
                        float length_xy = Matrix::f3x1_length(Matrix::F3x1((input_x * scale_x), (input_y * scale_y), 0.0f));

                        check(Matrix::f_float_equals(matrix_tmp.x, length_xy * std::cosf(angle_of_xy + rotation_angle)), "matrix scale, rotate, translate x scale than rotate test");
                        check(Matrix::f_float_equals(matrix_tmp.y, length_xy * std::sinf(angle_of_xy + rotation_angle)), "matrix scale, rotate, translate y scale than rotate test");
                    }

                    // scale than rotate and than translate
                    {
                        Matrix::F3x1 matrix_tmp = matrix_translate * matrix_rotate * matrix_scale * Matrix::F3x1(input_x, input_y, 1.0f);

                        float angle_of_xy = Matrix::f_float_calculate_the_angle_for_xy_in_radians((input_x * scale_x), (input_y * scale_y));
                        float length_xy = Matrix::f3x1_length(Matrix::F3x1((input_x * scale_x), (input_y * scale_y), 0.0f));
                        float test_x = length_xy * std::cosf(angle_of_xy + rotation_angle) + translate_x;
                        float test_y = length_xy * std::sinf(angle_of_xy + rotation_angle) + translate_y;

                        check(Matrix::f_float_equals(matrix_tmp.x, test_x), "matrix scale, rotate, translate x scale than rotate than translate test");
                        check(Matrix::f_float_equals(matrix_tmp.y, test_y), "matrix scale, rotate, translate y scale than rotate than translate test");


                    }


                }


            }

        }


    }
}

namespace U
{
    namespace Generating_the_dataset
    {
        void save_image(ImageRGBA* image, int variant, int index)
        {

            // std::string folder_path = "C:/Users/Cosmos/Desktop/TheCanvas0000/effect/";
            // ImageRGBA_::save_png(*image, (folder_path + std::to_string(index) + ".png").c_str());


            std::string base_path = "E:/generate_dataset/";
            Folder::create_folder_if_does_not_exist_already_no_message(base_path);

            std::string folder_path = base_path + "variant_" + std::to_string(variant) + "/"; // std::string folder_path = "output/";

            Folder::create_folder_if_does_not_exist_already_no_message(folder_path);

            std::cout << "saving variant : " << variant << " index : " << index << "\n";

            ImageRGBA_::save_png(*image, (folder_path + std::to_string(index) + ".png").c_str());
        }

        void image_darken(ImageRGBA* image, float factor)
        {
            const int image_width = ImageRGBA_::get_width(*image);
            const int image_height = ImageRGBA_::get_height(*image);

            for (int y = 0; y < image_height; y++)
            {
                for (int x = 0; x < image_width; x++)
                {
                    const RGBA color = ImageRGBA_::get_pixel(*image, x, y);



                    ImageRGBA_::set_pixel
                    (
                        *image,
                        x,
                        y,
                        RGBA
                        (
                            static_cast<uint8_t>(static_cast<float>(color.r) * factor),
                            static_cast<uint8_t>(static_cast<float>(color.g) * factor),
                            static_cast<uint8_t>(static_cast<float>(color.b) * factor),
                            255
                        )
                    );
                }
            }
        }

        struct Line_2d_segment
        {
            float x0;
            float y0;
            float x1;
            float y1;
            RGBA color;
        };

        struct Transform_2d
        {
            float scale;
            float angle;
            float position_x;
            float position_y;
        };

        struct Box_agent
        {
            float x_min;
            float x_max;
            float y_min;
            float y_max;

            RGBA color;
            Transform_2d transform;
            Experiment_32::Matrix::F3x3 transform_matrix;

            bool enable_arrow_up;
            bool enable_arrow_down;
            bool enable_arrow_left;
            bool enable_arrow_right;

            RGBA color_arrow_up;
            RGBA color_arrow_down;
            RGBA color_arrow_left;
            RGBA color_arrow_right;
        };

        namespace Line_2d_segment_
        {

            void draw_line_bresenham_mix(ImageRGBA* image, int x1, int y1, int x2, int y2, RGBA color, float factor)
            {
                int dx = abs(x2 - x1);
                int dy = abs(y2 - y1);
                int error = dx - dy;

                int x_step = (x1 < x2) ? 1 : -1;
                int y_step = (y1 < y2) ? 1 : -1;

                while (x1 != x2 || y1 != y2) {
                    // pixels.push_back(std::make_pair(x1, y1));
                    ImageRGBA_::mix_with_pixel(*image, x1, y1, color, factor);

                    int error2 = error * 2;
                    if (error2 > -dy) {
                        error -= dy;
                        x1 += x_step;
                    }
                    if (error2 < dx) {
                        error += dx;
                        y1 += y_step;
                    }
                }
                // pixels.push_back(std::make_pair(x2, y2));  // Add the last point
                ImageRGBA_::mix_with_pixel(*image, x2, y2, color, factor);
            }

            void draw_line_bresenham(ImageRGBA* image, int x1, int y1, int x2, int y2, RGBA color)
            {
                int dx = abs(x2 - x1);
                int dy = abs(y2 - y1);
                int error = dx - dy;

                int x_step = (x1 < x2) ? 1 : -1;
                int y_step = (y1 < y2) ? 1 : -1;

                while (x1 != x2 || y1 != y2) {
                    // pixels.push_back(std::make_pair(x1, y1));
                    ImageRGBA_::set_pixel(*image, x1, y1, color);

                    int error2 = error * 2;
                    if (error2 > -dy) {
                        error -= dy;
                        x1 += x_step;
                    }
                    if (error2 < dx) {
                        error += dx;
                        y1 += y_step;
                    }
                }
                // pixels.push_back(std::make_pair(x2, y2));  // Add the last point
                ImageRGBA_::set_pixel(*image, x2, y2, color);
            }

            void draw_line_segment(ImageRGBA* image, const Line_2d_segment& line_segment, const Experiment_32::Matrix::F3x3& matrix)
            {
                Experiment_32::Matrix::F3x1 a = matrix * Experiment_32::Matrix::F3x1(line_segment.x0, line_segment.y0, 1.0f);
                Experiment_32::Matrix::F3x1 b = matrix * Experiment_32::Matrix::F3x1(line_segment.x1, line_segment.y1, 1.0f);

                draw_line_bresenham(image, a.x, a.y, b.x, b.y, line_segment.color);
            }

            void draw_line_segment_mix(ImageRGBA* image, const Line_2d_segment& line_segment, const Experiment_32::Matrix::F3x3& matrix, float factor)
            {
                Experiment_32::Matrix::F3x1 a = matrix * Experiment_32::Matrix::F3x1(line_segment.x0, line_segment.y0, 1.0f);
                Experiment_32::Matrix::F3x1 b = matrix * Experiment_32::Matrix::F3x1(line_segment.x1, line_segment.y1, 1.0f);

                draw_line_bresenham_mix(image, a.x, a.y, b.x, b.y, line_segment.color, factor);
            }

        }

        namespace Transform_2d_
        {
            void reset(Transform_2d& transform)
            {
                transform.angle = 0;
                transform.position_x = 0;
                transform.position_y = 0;
                transform.scale = 1.0f;
            }

            void copy_into(const Transform_2d& source, Transform_2d& destination)
            {
                destination.angle = source.angle;
                destination.position_x = source.position_x;
                destination.position_y = source.position_y;
                destination.scale = source.scale;
            }

            void scale_by_factor(Transform_2d& transform, float factor)
            {
                transform.scale *= factor;
            }

            void reset_scale(Transform_2d& transform)
            {
                transform.scale = 1.0f;
            }

            void rotate(Transform_2d& transform, float angle)
            {
                transform.angle += angle;
            }

            void set_rotation(Transform_2d& transform, float angle)
            {
                transform.angle = angle;
            }

            void set_position(Transform_2d& transform, float x, float y)
            {
                transform.position_x = x;
                transform.position_y = y;
            }

            void move(Transform_2d& transform, float x, float y)
            {
                transform.position_x = x;
                transform.position_y = y;
            }

            Experiment_32::Matrix::F3x3 calculate_from_transform_the_matrix(const Transform_2d& transform, const Box_agent& box_agent)
            {
                const float center_x = (box_agent.x_max - box_agent.x_min) * 0.5f;
                const float center_y = (box_agent.y_max - box_agent.y_min) * 0.5f;
                const float offset_x = -center_x - (box_agent.x_min);
                const float offset_y = -center_y - (box_agent.y_min);

                return  Experiment_32::Matrix::f3x3_create_2d_translation_matrix(transform.position_x, transform.position_y) *

                    Experiment_32::Matrix::f3x3_create_2d_translation_matrix(-offset_x, -offset_y) *
                    Experiment_32::Matrix::f3x3_create_2d_rotation_around_origin(transform.angle) *
                    Experiment_32::Matrix::f3x3_create_2d_scaling_matrix(transform.scale, transform.scale) *
                    Experiment_32::Matrix::f3x3_create_2d_translation_matrix(offset_x, offset_x) *

                    Experiment_32::Matrix::f3x3_create_identity_matrix();
            }
        }

        namespace Box_agent_
        {
            void print(const Box_agent& box_agent)
            {
                Experiment_32::Matrix::F3x1 a = box_agent.transform_matrix * Experiment_32::Matrix::F3x1(box_agent.x_min, box_agent.y_min, 1.0f);
                Experiment_32::Matrix::F3x1 b = box_agent.transform_matrix * Experiment_32::Matrix::F3x1(box_agent.x_max, box_agent.y_max, 1.0f);


                std::cout << "\n----\n";
                std::cout << "x_min : " << a.x << "\n";
                std::cout << "x_max : " << b.x << "\n";
                std::cout << "y_min : " << a.y << "\n";
                std::cout << "y_max : " << b.y << "\n";

            }

            bool valid_transform(const Box_agent& box_agent, const Experiment_32::Matrix::F3x3& transform_matrix, const float x_min, const float x_max, const float y_min, const float y_max)
            {
                Experiment_32::Matrix::F3x1 a = box_agent.transform_matrix * Experiment_32::Matrix::F3x1(box_agent.x_min, box_agent.y_min, 1.0f);
                Experiment_32::Matrix::F3x1 b = box_agent.transform_matrix * Experiment_32::Matrix::F3x1(box_agent.x_max, box_agent.y_max, 1.0f);

                const float x_min_new = a.x;
                const float x_max_new = b.x;
                const float y_min_new = a.y;
                const float y_max_new = b.y;

                return !(
                    x_min_new < x_min ||
                    x_min_new > x_max ||
                    x_max_new > x_max ||
                    x_max_new < x_min ||
                    y_min_new < y_min ||
                    y_min_new > y_max ||
                    y_max_new > y_max ||
                    y_max_new < y_min
                    );
            }

            void draw_triangle(
                ImageRGBA* image,
                const RGBA& color,
                Experiment_32::Matrix::F3x3 matrix,
                float x_0,
                float y_0,
                float x_1,
                float y_1,
                float x_2,
                float y_2,
                float scale
            )
            {

                const float offset_x = (x_0 + x_1 + x_2) * (1.0f / 3.0f);
                const float offset_y = (y_0 + y_1 + y_2) * (1.0f / 3.0f);

                Experiment_32::Matrix::F3x3 matrix_triangle =
                    matrix *
                    Experiment_32::Matrix::f3x3_create_2d_translation_matrix(offset_x, offset_y) *
                    Experiment_32::Matrix::f3x3_create_2d_scaling_matrix(scale, scale) *
                    Experiment_32::Matrix::f3x3_create_2d_translation_matrix(-offset_x, -offset_y);

                Line_2d_segment_::draw_line_segment(image, Line_2d_segment(x_0, y_0, x_1, y_1, color), matrix_triangle);
                Line_2d_segment_::draw_line_segment(image, Line_2d_segment(x_1, y_1, x_2, y_2, color), matrix_triangle);
                Line_2d_segment_::draw_line_segment(image, Line_2d_segment(x_2, y_2, x_0, y_0, color), matrix_triangle);
            }

            void draw_triangle_mix(
                ImageRGBA* image,
                const RGBA& color,
                Experiment_32::Matrix::F3x3 matrix,
                float x_0,
                float y_0,
                float x_1,
                float y_1,
                float x_2,
                float y_2,
                float scale,
                float factor
            )
            {

                const float offset_x = (x_0 + x_1 + x_2) * (1.0f / 3.0f);
                const float offset_y = (y_0 + y_1 + y_2) * (1.0f / 3.0f);

                Experiment_32::Matrix::F3x3 matrix_triangle =
                    matrix *
                    Experiment_32::Matrix::f3x3_create_2d_translation_matrix(offset_x, offset_y) *
                    Experiment_32::Matrix::f3x3_create_2d_scaling_matrix(scale, scale) *
                    Experiment_32::Matrix::f3x3_create_2d_translation_matrix(-offset_x, -offset_y);

                Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(x_0, y_0, x_1, y_1, color), matrix_triangle, factor);
                Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(x_1, y_1, x_2, y_2, color), matrix_triangle, factor);
                Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(x_2, y_2, x_0, y_0, color), matrix_triangle, factor);
            }



            void draw_box_agent(ImageRGBA* image, Box_agent& box_agent)
            {
                float x_min = box_agent.x_min;
                float x_max = box_agent.x_max;
                float y_min = box_agent.y_min;
                float y_max = box_agent.y_max;


                //basic square
                Line_2d_segment_::draw_line_segment(image, Line_2d_segment(x_min, y_min, x_min, y_max, box_agent.color), box_agent.transform_matrix);
                Line_2d_segment_::draw_line_segment(image, Line_2d_segment(x_max, y_min, x_max, y_max, box_agent.color), box_agent.transform_matrix);
                Line_2d_segment_::draw_line_segment(image, Line_2d_segment(x_min, y_min, x_max, y_min, box_agent.color), box_agent.transform_matrix);
                Line_2d_segment_::draw_line_segment(image, Line_2d_segment(x_min, y_max, x_max, y_max, box_agent.color), box_agent.transform_matrix);



                {
                    float x_0 = x_min;
                    float x_1 = (x_min + x_max) * 0.5f;
                    float x_2 = x_max;

                    float y_0 = y_min;
                    float y_1 = (x_min + x_max) * 0.5f;
                    float y_2 = y_max;


                    float T_0_0_x = x_0;
                    float T_0_1_x = x_1;
                    float T_0_2_x = x_2;
                    float T_1_0_x = x_0;
                    float T_1_1_x = x_1;
                    float T_1_2_x = x_2;
                    float T_2_0_x = x_0;
                    float T_2_1_x = x_1;
                    float T_2_2_x = x_2;

                    float T_0_0_y = y_0;
                    float T_0_1_y = y_0;
                    float T_0_2_y = y_0;
                    float T_1_0_y = y_1;
                    float T_1_1_y = y_1;
                    float T_1_2_y = y_1;
                    float T_2_0_y = y_2;
                    float T_2_1_y = y_2;
                    float T_2_2_y = y_2;

                    {
                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(T_1_0_x, T_1_0_y, T_0_1_x, T_0_1_y, box_agent.color), box_agent.transform_matrix);
                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(T_0_1_x, T_0_1_y, T_1_2_x, T_1_2_y, box_agent.color), box_agent.transform_matrix);
                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(T_1_2_x, T_1_2_y, T_2_1_x, T_2_1_y, box_agent.color), box_agent.transform_matrix);
                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(T_2_1_x, T_2_1_y, T_1_0_x, T_1_0_y, box_agent.color), box_agent.transform_matrix);
                    }

                    {
                        float U_0_x = (T_1_0_x + T_0_1_x) * 0.5f;
                        float U_1_x = (T_0_1_x + T_1_2_x) * 0.5f;
                        float U_2_x = (T_1_2_x + T_2_1_x) * 0.5f;
                        float U_3_x = (T_2_1_x + T_1_0_x) * 0.5f;

                        float U_0_y = (T_1_0_y + T_0_1_y) * 0.5f;
                        float U_1_y = (T_0_1_y + T_1_2_y) * 0.5f;
                        float U_2_y = (T_1_2_y + T_2_1_y) * 0.5f;
                        float U_3_y = (T_2_1_y + T_1_0_y) * 0.5f;

                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(U_0_x, U_0_y, U_1_x, U_1_y, box_agent.color), box_agent.transform_matrix);
                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(U_1_x, U_1_y, U_2_x, U_2_y, box_agent.color), box_agent.transform_matrix);
                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(U_2_x, U_2_y, U_3_x, U_3_y, box_agent.color), box_agent.transform_matrix);
                        Line_2d_segment_::draw_line_segment(image, Line_2d_segment(U_3_x, U_3_y, U_0_x, U_0_y, box_agent.color), box_agent.transform_matrix);


                        if (box_agent.enable_arrow_up)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle(image, box_agent.color_arrow_up, box_agent.transform_matrix, U_0_x, U_0_y, T_0_1_x, T_0_1_y, U_1_x, U_1_y, 0.1f * i);
                            }
                        }

                        if (box_agent.enable_arrow_right)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle(image, box_agent.color_arrow_right, box_agent.transform_matrix, U_1_x, U_1_y, T_1_2_x, T_1_2_y, U_2_x, U_2_y, 0.1f * i);
                            }
                        }

                        if (box_agent.enable_arrow_down)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle(image, box_agent.color_arrow_down, box_agent.transform_matrix, U_2_x, U_2_y, T_2_1_x, T_2_1_y, U_3_x, U_3_y, 0.1f * i);
                            }
                        }

                        if (box_agent.enable_arrow_left)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle(image, box_agent.color_arrow_left, box_agent.transform_matrix, U_3_x, U_3_y, T_1_0_x, T_1_0_y, U_0_x, U_0_y, 0.1f * i);
                            }
                        }

                    }






                }
            }

            void draw_box_agent_mix(ImageRGBA* image, Box_agent& box_agent, float factor)
            {
                float x_min = box_agent.x_min;
                float x_max = box_agent.x_max;
                float y_min = box_agent.y_min;
                float y_max = box_agent.y_max;


                //basic square
                Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(x_min, y_min, x_min, y_max, box_agent.color), box_agent.transform_matrix, factor);
                Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(x_max, y_min, x_max, y_max, box_agent.color), box_agent.transform_matrix, factor);
                Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(x_min, y_min, x_max, y_min, box_agent.color), box_agent.transform_matrix, factor);
                Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(x_min, y_max, x_max, y_max, box_agent.color), box_agent.transform_matrix, factor);



                {
                    float x_0 = x_min;
                    float x_1 = (x_min + x_max) * 0.5f;
                    float x_2 = x_max;

                    float y_0 = y_min;
                    float y_1 = (x_min + x_max) * 0.5f;
                    float y_2 = y_max;


                    float T_0_0_x = x_0;
                    float T_0_1_x = x_1;
                    float T_0_2_x = x_2;
                    float T_1_0_x = x_0;
                    float T_1_1_x = x_1;
                    float T_1_2_x = x_2;
                    float T_2_0_x = x_0;
                    float T_2_1_x = x_1;
                    float T_2_2_x = x_2;

                    float T_0_0_y = y_0;
                    float T_0_1_y = y_0;
                    float T_0_2_y = y_0;
                    float T_1_0_y = y_1;
                    float T_1_1_y = y_1;
                    float T_1_2_y = y_1;
                    float T_2_0_y = y_2;
                    float T_2_1_y = y_2;
                    float T_2_2_y = y_2;

                    {
                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(T_1_0_x, T_1_0_y, T_0_1_x, T_0_1_y, box_agent.color), box_agent.transform_matrix, factor);
                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(T_0_1_x, T_0_1_y, T_1_2_x, T_1_2_y, box_agent.color), box_agent.transform_matrix, factor);
                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(T_1_2_x, T_1_2_y, T_2_1_x, T_2_1_y, box_agent.color), box_agent.transform_matrix, factor);
                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(T_2_1_x, T_2_1_y, T_1_0_x, T_1_0_y, box_agent.color), box_agent.transform_matrix, factor);
                    }

                    {
                        float U_0_x = (T_1_0_x + T_0_1_x) * 0.5f;
                        float U_1_x = (T_0_1_x + T_1_2_x) * 0.5f;
                        float U_2_x = (T_1_2_x + T_2_1_x) * 0.5f;
                        float U_3_x = (T_2_1_x + T_1_0_x) * 0.5f;

                        float U_0_y = (T_1_0_y + T_0_1_y) * 0.5f;
                        float U_1_y = (T_0_1_y + T_1_2_y) * 0.5f;
                        float U_2_y = (T_1_2_y + T_2_1_y) * 0.5f;
                        float U_3_y = (T_2_1_y + T_1_0_y) * 0.5f;

                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(U_0_x, U_0_y, U_1_x, U_1_y, box_agent.color), box_agent.transform_matrix, factor);
                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(U_1_x, U_1_y, U_2_x, U_2_y, box_agent.color), box_agent.transform_matrix, factor);
                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(U_2_x, U_2_y, U_3_x, U_3_y, box_agent.color), box_agent.transform_matrix, factor);
                        Line_2d_segment_::draw_line_segment_mix(image, Line_2d_segment(U_3_x, U_3_y, U_0_x, U_0_y, box_agent.color), box_agent.transform_matrix, factor);


                        if (box_agent.enable_arrow_up)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle_mix(image, box_agent.color_arrow_up, box_agent.transform_matrix, U_0_x, U_0_y, T_0_1_x, T_0_1_y, U_1_x, U_1_y, 0.1f * i, factor);
                            }
                        }

                        if (box_agent.enable_arrow_right)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle_mix(image, box_agent.color_arrow_right, box_agent.transform_matrix, U_1_x, U_1_y, T_1_2_x, T_1_2_y, U_2_x, U_2_y, 0.1f * i, factor);
                            }
                        }

                        if (box_agent.enable_arrow_down)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle_mix(image, box_agent.color_arrow_down, box_agent.transform_matrix, U_2_x, U_2_y, T_2_1_x, T_2_1_y, U_3_x, U_3_y, 0.1f * i, factor);
                            }
                        }

                        if (box_agent.enable_arrow_left)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                draw_triangle_mix(image, box_agent.color_arrow_left, box_agent.transform_matrix, U_3_x, U_3_y, T_1_0_x, T_1_0_y, U_0_x, U_0_y, 0.1f * i, factor);
                            }
                        }

                    }






                }
            }

            void move(Box_agent& agent, float x, float y)
            {
                agent.transform_matrix = Experiment_32::Matrix::f3x3_create_2d_translation_matrix(x, y) * agent.transform_matrix;
            }




            void scale(Box_agent& agent, float scale_x, float scale_y)
            {
                const float center_x = (agent.x_max - agent.x_min) * 0.5f;
                const float center_y = (agent.y_max - agent.y_min) * 0.5f;
                const float offset_x = -center_x - (agent.x_min);
                const float offset_y = -center_y - (agent.y_min);
                move(agent, offset_x, offset_y);
                agent.transform_matrix = Experiment_32::Matrix::f3x3_create_2d_scaling_matrix(scale_x, scale_y) * agent.transform_matrix;
                move(agent, -offset_x, -offset_y);
            }

            void rotate_in_radians(Box_agent& agent, float angle_radians)
            {
                const float center_x = (agent.x_max - agent.x_min) * 0.5f;
                const float center_y = (agent.y_max - agent.y_min) * 0.5f;
                const float offset_x = -center_x - (agent.x_min);
                const float offset_y = -center_y - (agent.y_min);
                move(agent, offset_x, offset_y);
                agent.transform_matrix = Experiment_32::Matrix::f3x3_create_2d_rotation_around_origin(angle_radians) * agent.transform_matrix;
                move(agent, -offset_x, -offset_y);
            }


        }

        void run()
        {
            std::cout << "Generating_the_dataset" << "\n";

            const int width = 64;
            const int height = 64;

            ImageRGBA* image = ImageRGBA_::create(width, height);


            int number_of_images_to_generate = 4 * 10000;

            int image_num = 0;
            while (image_num < number_of_images_to_generate)
            {
                image_num += 1;

                int option = static_cast<int>(Random::generate_random_float_0_to_1() * 4.0);

                int number_of_lines = 100 + 20000 * Random::generate_random_float_0_to_1();

                RGBA color = RGBA_::generate_random_color();
                RGBA offset_color = RGBA_::generate_random_color();
                float alpha = 0.007 + 0.1 * Random::generate_random_float_0_to_1();

                ImageRGBA_::clear_with_color(*image, RGBA(0, 0, 0, 255));

                if (option == 0)
                {
                    for (int i = 0; i < 10000; i++)
                    {
                        int x_start = Random::generate_random_float_0_to_1() * width;
                        int y_start = Random::generate_random_float_0_to_1() * height;



                        int x_end = x_start + Random::generate_random_float_0_to_1() * width * 0.1 - (width * 0.1f * 0.5f);
                        int y_end = y_start + width * 0.4 * Random::generate_random_float_minus_one_to_plus_one();




                        Line_2d_segment_::draw_line_bresenham_mix(image, x_start, y_start, x_end, y_end, RGBA_::mix(color, offset_color, Random::generate_random_float_0_to_1()), alpha);
                    }
                }
                else if (option == 1)
                {
                    for (int i = 0; i < 10000; i++)
                    {
                        int x_start = Random::generate_random_float_0_to_1() * width;
                        int y_start = Random::generate_random_float_0_to_1() * height;



                        int x_end = x_start + width * 0.4 * Random::generate_random_float_minus_one_to_plus_one();
                        int y_end = y_start + Random::generate_random_float_0_to_1() * width * 0.1 - (width * 0.1f * 0.5f);


                        Line_2d_segment_::draw_line_bresenham_mix(image, x_start, y_start, x_end, y_end, RGBA_::mix(color, offset_color, Random::generate_random_float_0_to_1()), alpha);
                    }
                }
                else if (option == 2)
                {
                    for (int i = 0; i < 10000; i++)
                    {
                        int x_start = Random::generate_random_float_0_to_1() * width;
                        int y_start = Random::generate_random_float_0_to_1() * height;


                        float rnd_value = Random::generate_random_float_minus_one_to_plus_one() * 0.4;
                        int x_end = x_start + width * rnd_value;
                        int y_end = y_start + width * rnd_value;


                        Line_2d_segment_::draw_line_bresenham_mix(image, x_start, y_start, x_end, y_end, RGBA_::mix(color, offset_color, Random::generate_random_float_0_to_1()), alpha);
                    }
                }
                else if (option == 3)
                {
                    for (int i = 0; i < 10000; i++)
                    {
                        int x_start = Random::generate_random_float_0_to_1() * width;
                        int y_start = Random::generate_random_float_0_to_1() * height;


                        float rnd_value = Random::generate_random_float_minus_one_to_plus_one() * 0.4;
                        int x_end = x_start + width * rnd_value;
                        int y_end = y_start - width * rnd_value;


                        Line_2d_segment_::draw_line_bresenham_mix(image, x_start, y_start, x_end, y_end, RGBA_::mix(color, offset_color, Random::generate_random_float_0_to_1()), alpha);
                    }
                }

                save_image(image, option, image_num);

            }

        }
    }
}


int main()
{
	std::cout << "Generate dataset\n";
	
    U::Generating_the_dataset::run();

	return 0;
}