#include "Rectangle2d.h"

#include <algorithm>
#include <cstdlib>
#include <functional>

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
