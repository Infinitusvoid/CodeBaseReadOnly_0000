#pragma once
#pragma once
#pragma once

#include <iostream>

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