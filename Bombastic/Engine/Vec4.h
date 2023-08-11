#ifndef VEC4_HEADER
#define VEC4_HEADER
#include <vector>

class Vec4
{

public:

	std::vector<float> vals;

	Vec4();

	

	Vec4(float a, float b, float c, float d);

	float dot(Vec4 other);
	
	Vec4 operator- (Vec4 other) {
		return Vec4(vals[0] - other[0], vals[1] - other[1], vals[2] - other[2], vals[3] - other[3]);
	}

	float& operator[] (size_t i)
	{
		return vals[i];
	}

	Vec4 operator* (float multiplier) {
		return Vec4(vals[0] * multiplier, vals[1] * multiplier, vals[2] * multiplier, vals[3] * multiplier);
	}

	Vec4 operator/ (float x) {
		return Vec4(vals[0] / x, vals[1] / x, vals[2] / x, vals[3] / x);
	}

	Vec4 operator+ (Vec4 other) {
		return Vec4(vals[0] + other[0], vals[1] + other[1], vals[2] + other[2], vals[3] + other[3]);
	}
};
#endif

