#include "stdafx.h"
#include "Vec3.h"

Vec3::Vec3() {
	vals = std::vector<float>(3);
	vals[0] = 0;
	vals[1] = 0;
	vals[2] = 0;
}

Vec3::Vec3(Vec4 vec) {
	vals = std::vector<float>(3);
	vals[0] = vec[0];
	vals[1] = vec[1];
	vals[2] = vec[2];
}

Vec3::Vec3(float x, float y, float z) {
	vals = std::vector<float>(3);
	vals[0] = x;
	vals[1] = y;
	vals[2] = z;
}

float Vec3::dot(Vec3 other) {
	return vals[0] * other.vals[0] + vals[1] * other.vals[1] + vals[2] * other.vals[2];
}

float Vec3::norm() {
	return sqrt(vals[0] * vals[0] + vals[1] * vals[1] + vals[2] * vals[2]);
}

Vec3 Vec3::cross(Vec3 other) {
	return Vec3(
		vals[1] * other[2] - vals[2] * other[1],
		vals[2] * other[0] - vals[0] * other[2],
		vals[0] * other[1] - vals[1] * other[0]
	);
}

Vec3 Vec3::normalize() {
	float norm = this->norm();
	if (norm == 0) {
		return *this;
	}
	else {
		return *this / this->norm();
	}
}