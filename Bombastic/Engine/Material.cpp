#include "stdafx.h"
#include "Material.h"


Material::Material(Vec3 ambientColor, float ambientCoeff, Vec3 diffuseColor, Vec3 specularColor, float phong_exp) : 
	ambientColor(ambientColor), ambientCoeff(ambientCoeff), diffuseColor(diffuseColor), specularColor(specularColor), phong_exp(phong_exp) {

}