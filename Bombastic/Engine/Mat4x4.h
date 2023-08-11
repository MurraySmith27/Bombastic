#ifndef MAT4X4_HEADER
#define MAT4X4_HEADER
#include "Vec4.h"

class Mat4x4
{

public:

	std::vector<Vec4> vals;

	Mat4x4();

	Mat4x4(Vec4 row1, Vec4 row2, Vec4 row3, Vec4 row4);

	Mat4x4(double mat[4][4]);

	bool inverse(Mat4x4& result);

	Mat4x4 transpose();

	Vec4 column(int index);

	Vec4 operator* (Vec4 other)
	{
		Vec4 result;

		result[0] = vals[0].dot(other);
		result[1] = vals[1].dot(other);
		result[2] = vals[2].dot(other);
		result[3] = vals[3].dot(other);
		return result;
;
	}

	Vec4& operator[] (int index) {
		return vals[index];
	}

	Mat4x4 operator* (Mat4x4 other) {
		return Mat4x4(
			Vec4((vals[0]).dot(other.column(0)), (vals[0]).dot(other.column(1)), (vals[0]).dot(other.column(2)), (vals[0]).dot(other.column(3))),
			Vec4((vals[1]).dot(other.column(0)), (vals[1]).dot(other.column(1)), (vals[1]).dot(other.column(2)), (vals[1]).dot(other.column(3))),
			Vec4((vals[2]).dot(other.column(0)), (vals[2]).dot(other.column(1)), (vals[2]).dot(other.column(2)), (vals[2]).dot(other.column(3))),
			Vec4((vals[3]).dot(other.column(0)), (vals[3]).dot(other.column(1)), (vals[3]).dot(other.column(2)), (vals[3]).dot(other.column(3)))
		);
	}
};


#endif

