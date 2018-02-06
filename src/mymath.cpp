#include "mymath.h"

#include <cmath>

const float math_constant::PI = std::asin(1.f);
const float math_constant::TWO_PI = 2.f * PI;
const float math_constant::HALF_PI = PI / 2.f;

const Vector3 Vector3::Zero = Vector3();
const Vector3 Vector3::XAxis = Vector3(1.f);
const Vector3 Vector3::YAxis = Vector3(0.f, 1.f);
const Vector3 Vector3::ZAxis = Vector3(0.f, 0.f, 1.f);

Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

float &Vector3::operator[](int i) {
  return reinterpret_cast<float*>(this)[i];
}

float Vector3::operator[](int i) const {
  return const_cast<Vector3 *>(this)->operator[](i);
}

Vector3 &Vector3::operator+=(Vector3 const &rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Vector3 &Vector3::operator-=(Vector3 const &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Vector3 &Vector3::operator*=(Vector3 const &rhs) {
  x *= rhs.x;
  y *= rhs.y;
  z *= rhs.z;
  return *this;
}

float Vector3::sq_magnitude() const {
  return dot(*this, *this);
}

float Vector3::magnitude() const {
  return std::sqrt(sq_magnitude());
}

Vector3 operator-(Vector3 const &rhs) {
	return Vector3(-rhs.x, -rhs.y, -rhs.z);
}

Vector3 operator+(Vector3 const &lhs, Vector3 const &rhs) {
	return Vector3(lhs) += rhs;
}

Vector3 operator-(Vector3 const &lhs, Vector3 const &rhs) {
	return Vector3(lhs) -= rhs;
}

Vector3 operator*(Vector3 const &lhs, Vector3 const &rhs) {
	return Vector3(lhs) *= rhs;
}

Vector3 operator*(float c, Vector3 const &rhs) {
	return Vector3(rhs.x * c, rhs.y * c, rhs.z * c);
}

Vector3 operator*(Vector3 const &rhs, float c) {
	return c * rhs;
}

float dot(Vector3 const &lhs, Vector3 const &rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3 cross(Vector3 const &lhs, Vector3 const &rhs) {
	return Vector3((lhs.y * rhs.z) - (lhs.z * rhs.y), 
	               (lhs.x * rhs.z) - (lhs.z * rhs.x),
				   (lhs.y * rhs.x) - (lhs.x * rhs.y));
}

Vector3 normalize(Vector3 const &rhs) {
	float m = rhs.magnitude();
	return Vector3(rhs.x / m, rhs.y / m, rhs.z / m);
}

std::ostream &operator<<(std::ostream &os, Vector3 const &rhs) {
	os << rhs.x << " " << rhs.y << " " << rhs.z;
	return os;
}

std::istream &operator>>(std::istream &is, Vector3 &rhs) {
	is >> rhs.x >> rhs.y >> rhs.z;
	return is;
}

const Matrix3x3 Matrix3x3::Identity = Matrix3x3{};
const Matrix3x3 Matrix3x3::Zero = Matrix3x3(Vector3::Zero, 
                                            Vector3::Zero, 
                                            Vector3::Zero);

Matrix3x3::Matrix3x3(Vector3 const &m0, Vector3 const &m1, Vector3 const &m2) 
: matrix{m0, m1, m2} {}
	
Matrix3x3::Matrix3x3(float m00, float m01, float m02,
                     float m10, float m11, float m12,
                     float m20, float m21, float m22)
                   : Matrix3x3(Vector3(m00, m01, m02), 
                               Vector3(m10, m11, m12), 
                               Vector3(m20, m21, m22)) {}

Vector3 &Matrix3x3::operator[](int i) {
  return matrix[i];
}

Vector3 Matrix3x3::operator[](int i) const {
  return const_cast<Matrix3x3 *>(this)->operator[](i);
}

Matrix3x3 &Matrix3x3::operator+=(Matrix3x3 const &rhs) {
  for(int i = 0; i < 3; ++i) {
    matrix[i] += rhs.matrix[i];
  }
  return *this;
}

Matrix3x3 &Matrix3x3::operator-=(Matrix3x3 const &rhs) {
  for(int i = 0; i < 3; ++i) {
    matrix[i] -= rhs.matrix[i];
  }
  return *this;
}

Matrix3x3 operator+(Matrix3x3 const &lhs, Matrix3x3 const &rhs) {
  return Matrix3x3(lhs) += rhs;
}

Matrix3x3 operator-(Matrix3x3 const &lhs, Matrix3x3 const &rhs) {
  return Matrix3x3(lhs) -= rhs;
}

Matrix3x3 operator*(float c, Matrix3x3 const &rhs) {
	return Matrix3x3(c * rhs[0], c * rhs[1], c * rhs[2]);
}

Vector3 operator*(Matrix3x3 const &lhs, Vector3 const &rhs) {
  // calling transpose might be less efficient than hard-coding.
  Matrix3x3 t = Transpose(lhs);
  return Vector3(dot(t[0], rhs), dot(t[1], rhs), dot(t[2], rhs));
  
  /*
	return Vector3(lhs[0][0] * rhs[0] + lhs[1][0] * rhs[1] + lhs[2][0] * rhs[2],
				   lhs[0][0] * rhs[0] + lhs[1][0] * rhs[1] + lhs[2][0] * rhs[2],
				   lhs[0][0] * rhs[0] + lhs[1][0] * rhs[1] + lhs[2][0] * rhs[2]);
  */
}

Vector3 operator*(Vector3 const &lhs, Matrix3x3 const &rhs) {
	return Vector3(dot(lhs, rhs[0]), dot(lhs, rhs[1]), dot(lhs, rhs[2]));
}

Matrix3x3 operator*(Matrix3x3 const &lhs, Matrix3x3 const &rhs) {
	// calling transpose might be less efficient than hard-coding.
	Matrix3x3 t = Transpose(lhs);
	return Matrix3x3(dot(t[0], rhs[0]), dot(t[1], rhs[0]), dot(t[2], rhs[0]),
	                 dot(t[0], rhs[1]), dot(t[1], rhs[1]), dot(t[2], rhs[1]),
					         dot(t[0], rhs[2]), dot(t[1], rhs[2]), dot(t[2], rhs[2]));
}

Matrix3x3 Transpose(Matrix3x3 const &rhs) {
	return Matrix3x3(
		rhs[0][0], rhs[1][0], rhs[2][0],
		rhs[0][1], rhs[1][1], rhs[2][1],
		rhs[0][2], rhs[1][2], rhs[2][2]
	);
}

float Determinant(Matrix3x3 const &rhs) {
	return rhs[0][0] * ((rhs[1][1] * rhs[2][2]) - (rhs[2][1] * rhs[1][2])) 
	     + rhs[1][0] * ((rhs[2][1] * rhs[0][2]) - (rhs[0][1] * rhs[2][2])) 
		   + rhs[2][0] * ((rhs[0][1] * rhs[1][2]) - (rhs[1][1] * rhs[0][2]));
}

Matrix3x3 Inverse(Matrix3x3 const &rhs) {
	return (1.f / Determinant(rhs)) * rhs;
}

std::ostream &operator<<(std::ostream &os, Matrix3x3 const &rhs) {
	for(int i = 0; i < 3; ++i) {
		os << rhs[i] << ((i + 1 < 3) ? "\n" : "");
	}
	return os;
}

std::istream &operator>>(std::istream &is, Matrix3x3 &rhs) {
	for(int i = 0; i < 3; ++i) {
		is >> rhs[i];
	}
	return is;
}