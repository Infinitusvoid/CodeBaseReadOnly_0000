#include <iostream>
#include <vector>
#include <string>

#include "Rectangle2d.h"
#include "Random.h"
#include "ImageRGBA.h"
#include "Folder.h"

#include <numbers>

#include <fstream>
#include <cstdint>
#include <cmath>
#include <functional>

#include "Experiment_32_Matrix.h"


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