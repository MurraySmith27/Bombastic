#ifndef IMATERIAL_HEADER
#define IMATERIAL_HEADER

#include "Vec3.h"

class Material
{
public:
	Vec3 ambientColor;

	float ambientCoeff;

	Vec3 diffuseColor;

	Vec3 specularColor;

	float phong_exp;

	Material(Vec3 ambientColor, float ambientCoeff, Vec3 diffuseColor, Vec3 specularColor, float phong_exp);
};

#endif

