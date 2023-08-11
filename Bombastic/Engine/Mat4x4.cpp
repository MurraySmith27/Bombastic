#include "stdafx.h"
#include "Mat4x4.h"

Mat4x4::Mat4x4() {
	vals = std::vector<Vec4>(4);
	vals[0] = Vec4();
	vals[1] = Vec4();
	vals[2] = Vec4();
	vals[3] = Vec4();
}

Mat4x4::Mat4x4(Vec4 row1, Vec4 row2, Vec4 row3, Vec4 row4) {

	vals = std::vector<Vec4>(4);
	vals[0] = row1;
	vals[1] = row2;
	vals[2] = row3;
	vals[3] = row4;
}

bool Mat4x4::inverse(Mat4x4& result) {
	//to compute the inverse, we do LU decomp and then solve for each column of the inverse LUx=b

	Mat4x4 copy;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			copy[i][j] = vals[i][j];
		}
	}


	for (int k = 0; k < 3; k++) {
		for (int i = k + 1; i < 4; i++) {
			if (copy[k][k] == 0) {
				return false;
			}

			copy[i][k] = copy[i][k] / copy[k][k];

			for (int j = k + 1; j < 4; j++) {
				copy[i][j] = copy[i][j] - copy[i][k] * copy[k][j];
			}
		}
	}
	Mat4x4 L(
		Vec4(1, 0, 0, 0),
		Vec4(copy[1][0], 1, 0, 0),
		Vec4(copy[2][0], copy[2][1], 1, 0),
		Vec4(copy[3][0], copy[3][1], copy[3][2], 1)
	);

	Mat4x4 U = copy;

	Mat4x4 test1 = L * U;

	//now we have the LU decomposition of the matrix. solve for each column using forward and then backward substitution.
	Vec4 columns[4];
	Vec4 temp[4];

	for (int col = 0; col < 4; col++) {

		for (int i = 0; i < 4; i++) {
			temp[col][i] = col == i;
			for (int j = 0; j < i; j++) {
				temp[col][i] = temp[col][i] - L[i][j] * temp[col][j];
			}
			//omit a step here because all diagonals are 1.
		}

		//now we do backward substitution to get the column:
		for (int i = 3; i >= 0; i--) {
			columns[col][i] = temp[col][i];
			for (int j = i + 1; j < 4; j++) {
				columns[col][i] = columns[col][i] - U[i][j] * columns[col][j];
			}
			if (U[i][i] == 0) {
				return false;
			}
			columns[col][i] = columns[col][i] / U[i][i];
		}

	}

	//now columns contains the inverse's columns
	for (int i = 0; i < 4; i++) {
		result[i] = Vec4(
			columns[0][i], columns[1][i], columns[2][i], columns[3][i]
		);
	}

	Mat4x4 I = (*this) * result;
	return true;
}

Mat4x4 Mat4x4::transpose() {
	return Mat4x4(
		Vec4(vals[0][0], vals[1][0], vals[2][0], vals[3][0]),
		Vec4(vals[0][1], vals[1][1], vals[2][1], vals[3][1]),
		Vec4(vals[0][2], vals[1][2], vals[2][2], vals[3][2]),
		Vec4(vals[0][3], vals[1][3], vals[2][3], vals[3][3])
	);
}

Mat4x4::Mat4x4(double mat[4][4]) {
	vals = std::vector<Vec4>(4);
	for (int i = 0; i < 4; i++) {
		vals[i] = Vec4(mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
	}
}

Vec4 Mat4x4::column(int index) {
	return Vec4(vals[0][index], vals[1][index], vals[2][index], vals[3][index]);
}
