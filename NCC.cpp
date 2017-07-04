/*
 * NCC.cpp
 *
 *  Created on: Jul 20, 2016
 *      Author: hakosoft
 */

#include "NCC.h"

void NCC::Initializtion(const glm::vec3* V0,const glm::vec3* V1, const unsigned int& Size)
{

	_vectorSize = Size;
	_v0 = new glm::vec3[_vectorSize];
	_v1 = new glm::vec3[_vectorSize];

	// each element in v0 must be normlized
	for(int i = 0; i < _vectorSize; i++)
	{
		glm::vec3 RGB0 = normalizeColor(V0[i]);
	//	std::cout <<RGB0.x<<" "<<RGB0.y<<" "<<RGB0.z<<std::endl;
		glm::vec3 RGB1 = normalizeColor(V1[i]);

		_v0[i] = RGB0;
		_v1[i] = RGB1;
	}
	Averges(); // averge to calculate the Meanv0 and MeanV1
}

NCC::NCC()
{
	// TODO Auto-generated destructor stub
}

NCC::~NCC() {

}

glm::vec3 NCC::normalizeColor(const glm::vec3& RGB)const
{
	glm::vec3 r;

	float length = sqrt(glm::dot(RGB,RGB));	// smart ass doing smart thing LoL

	if(length == 0)
	{
		r.x = 0.0;
		r.y = 0.0;
		r.z = 0.0;
	}
	else
	{
		r.x = RGB.x / length;
		r.y = RGB.y / length;
		r.z = RGB.z / length;
	}

	return r;
}

void NCC::Averges()
{
	// the color are normalized we can use them for a start let's calculate the averge

	float r0 = 0.0;
	float g0 = 0.0;
	float b0 = 0.0;

	float r1 = 0.0;
	float g1 = 0.0;
	float b1 = 0.0;


	for(int i = 0; i < _vectorSize; i++)
	{
		r0 += _v0[i].x;
		g0 += _v0[i].y;
		b0 += _v0[i].z;

		r1 += _v1[i].x;
		g1 += _v1[i].y;
		b1 += _v1[i].z;
	}


	_MeanV0 = glm::vec3(r0 /(float)_vectorSize, g0/(float)_vectorSize, b0/(float)_vectorSize);
	//std::cout <<_MeanV0.x<<" "<<_MeanV0.y<<" "<<_MeanV0.z<<std::endl;
	_MeanV1 = glm::vec3(r1 /(float)_vectorSize, g1/(float)_vectorSize, b1/(float)_vectorSize);
	//std::cout <<_MeanV1.x<<" "<<_MeanV1.y<<" "<<_MeanV1.z<<std::endl;
}

void NCC::Compute() // easy bizzi lovely quizy
{

	float upperFraction = 0.0;

	for(int j = 0; j < _vectorSize; j++)
	{

		glm::vec3 a = _v0[j] - _MeanV0;
		glm::vec3 b = _v1[j] - _MeanV1;

		upperFraction += glm::dot(a,b);
	}


	glm::vec3 downFractionA(0.0);
	glm::vec3 downFractionB(0.0);

	for(int j = 0; j < _vectorSize; j++)
	{
		downFractionA += glm::pow((_v0[j] - _MeanV0),glm::vec3(2.0));
		downFractionB += glm::pow((_v1[j] - _MeanV1),glm::vec3(2.0));
	}
	float donwFraction = sqrt(glm::dot(downFractionA,downFractionB));

	if(donwFraction == 0)
		_ncc = -1.0;
	else
		_ncc = upperFraction / donwFraction;

	_ncc = CLAMP(_ncc,-1.0,1.0);
}

float NCC::getNCC()
{
	return _ncc;
}

void NCC::clearVectors()
{
	delete[] _v0;
	delete[] _v1;
}
