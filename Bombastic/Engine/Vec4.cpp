#include "stdafx.h"
#include "Vec4.h"

Vec4::Vec4() {
	vals = std::vector<float>(4);
	vals[0] = 0;
	vals[1] = 0;
	vals[2] = 0;
	vals[3] = 0;
}


Vec4::Vec4(float a, float b, float c, float d) {
	vals = std::vector<float>(4);
	vals[0] = a;
	vals[1] = b;
	vals[2] = c;
	vals[3] = d;
}

float Vec4::dot(Vec4 other) {
	return vals[0] * other.vals[0] + vals[1] * other.vals[1] + vals[2] * other.vals[2] + vals[3] * other.vals[3];
}