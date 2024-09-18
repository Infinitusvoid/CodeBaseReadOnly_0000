#pragma once

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
