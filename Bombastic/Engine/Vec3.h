#ifndef VEC3_HEADER
#define VEC3_HEADER
#include <vector>
#include "Vec4.h"
#include <string>
class Vec3
{

public:

	std::vector<float> vals;

    Vec3();

    Vec3(Vec4 vec);

	Vec3(float x, float y, float z);

	float dot(Vec3 other);

    Vec3 cross(Vec3 other);

    float norm();

    Vec3 normalize();

    std::string to_string() {
        return std::to_string(vals[0]) + std::to_string(vals[1]) + std::to_string(vals[2]);
    }

    Vec3 operator/ (float x) {
        return Vec3(vals[0] / x, vals[1] / x, vals[2] / x);
    }

    float& operator[] (size_t i)
    {
        return vals[i];
    }

    Vec3 operator* (float coeff) {
        return Vec3(vals[0] * coeff, vals[1] * coeff, vals[2] * coeff);
    }

    Vec3 operator- () {
        return Vec3(-vals[0], -vals[1], -vals[2]);
    }

    Vec3 operator- (Vec3 other) {
        return Vec3(vals[0] - other[0], vals[1] - other[1], vals[2] - other[2]);
    }

    Vec3 operator+ (Vec3 other) {
        return Vec3(vals[0] + other[0], vals[1] + other[1], vals[2] + other[2]);
    }

    Vec3 operator* (Vec3 other) {
        return Vec3(vals[0] * other[0], vals[1] * other[1], vals[2] * other[2]);
    }

};
#endif

