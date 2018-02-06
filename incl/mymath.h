#ifndef MY_MATH_H
#define MY_MATH_H

#include <iostream>

struct math_constant {
  static const float PI;
  static const float TWO_PI;
  static const float HALF_PI;
};

struct Vector3 {
	float x,y,z;
	
	Vector3(float = 0.f, float = 0.f, float = 0.f);
	
	static const Vector3 Zero;
	static const Vector3 XAxis;
	static const Vector3 YAxis;
	static const Vector3 ZAxis;
	
	float &operator[](int);
	float operator[](int) const;
	
	Vector3 &operator+=(Vector3 const &);
	
	Vector3 &operator-=(Vector3 const &);
	
	Vector3 &operator*=(Vector3 const &);
	
	float sq_magnitude() const;
	
	float magnitude() const;
};

Vector3 operator-(Vector3 const &);
Vector3 operator+(Vector3 const &, Vector3 const &);
Vector3 operator-(Vector3 const &, Vector3 const &);
Vector3 operator*(Vector3 const &, Vector3 const &);
Vector3 operator*(float, Vector3 const &);
Vector3 operator*(Vector3 const &, float);

float dot(Vector3 const &, Vector3 const &);
Vector3 cross(Vector3 const &, Vector3 const &);
Vector3 normalize(Vector3 const &);

std::ostream &operator<<(std::ostream &, Vector3 const &);
std::istream &operator>>(std::istream &, Vector3 &);

struct Matrix3x3 {
	Vector3 matrix[3];
	
	Matrix3x3(Vector3 const & = Vector3::XAxis, 
	          Vector3 const & = Vector3::YAxis, 
			      Vector3 const & = Vector3::ZAxis);
	
	Matrix3x3(float, float, float,
	          float, float, float,
			      float, float, float);
						
	static const Matrix3x3 Identity;
	static const Matrix3x3 Zero;
	
	Vector3 &operator[](int);
	Vector3 operator[](int) const;
	
	Matrix3x3 &operator+=(Matrix3x3 const &);
	Matrix3x3 &operator-=(Matrix3x3 const &);
};

Matrix3x3 operator+(Matrix3x3 const &, Matrix3x3 const &);
Matrix3x3 operator-(Matrix3x3 const &, Matrix3x3 const &);

Matrix3x3 operator*(float, Matrix3x3 const &);
Vector3 operator*(Matrix3x3 const &, Vector3 const &);
Vector3 operator*(Vector3 const &, Matrix3x3 const &);
Matrix3x3 operator*(Matrix3x3 const &, Matrix3x3 const &);

Matrix3x3 Transpose(Matrix3x3 const &);
float Determinant(Matrix3x3 const &);
Matrix3x3 Inverse(Matrix3x3 const &);

std::ostream &operator<<(std::ostream &, Matrix3x3 const &);
std::istream &operator>>(std::istream &, Matrix3x3 &);

#endif