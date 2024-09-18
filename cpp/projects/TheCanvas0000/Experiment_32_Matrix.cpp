#include "Experiment_32_Matrix.h"


#include <iostream>
#include <vector>
#include <string>
#include <numbers>
#include <cstdint>


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