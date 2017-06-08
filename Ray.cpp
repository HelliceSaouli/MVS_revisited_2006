/*
 * Ray.cpp
 *
 *  Created on: Jul 14, 2016
 *      Author: hakosoft
 */

#include "Ray.h"

Ray::Ray(const glm::vec3& CameraOrigine, const glm::vec2& PixelCoordinate, const glm::vec3& BBmin, const glm::vec3& BBmax, const glm::mat4 &CameraProjection)
{

	_origine = CameraOrigine;
	//_direction  = glm::vec4(PixelCoordinate.x,PixelCoordinate.y,-1.0,1.0);
	//glm::mat4 inverseProjection = glm::inverse(CameraProjection);	//cool shit x')

	//http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/FUSIELLO4/tutorial.html optical ray equation
	glm::mat3 P3x3;
	for(int i = 0; i< 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			P3x3[i][j] = CameraProjection[i][j];
		}
	}

	glm::mat3 invP3x3 = glm::inverse(P3x3);
	glm::vec3 m(PixelCoordinate.x,PixelCoordinate.y,1.0);
	glm::vec3 d = invP3x3 * m;

	_direction  =glm::normalize(glm::vec4(d.x,d.y,d.z,1.0));

	_bbmin = BBmin;
	_bbmax = BBmax;

}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

/*******************************************************************/

// intersect ray with a box  every body using this
// http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
bool Ray::Hit()
{
    // compute intersection of ray with all six bbox planes
    glm::vec3 invR = glm::vec3(1.0 /_direction.x, 1.0 /_direction.y, 1.0/_direction.z );

    glm::vec3 tbot = invR * (_bbmin - _origine);
    glm::vec3 ttop = invR * (_bbmax - _origine);

    // re-order intersections to find smallest and largest on each axis
    glm::vec3 tmin = glm::min(ttop, tbot);
    glm::vec3 tmax = glm::max(ttop, tbot);

    // find the largest tmin and the smallest tmax
    float largest_tmin = fmaxf(fmaxf(tmin.x, tmin.y), fmaxf(tmin.x, tmin.z));
    float smallest_tmax = fminf(fminf(tmax.x, tmax.y), fminf(tmax.x, tmax.z));

    _tnear = largest_tmin;
    _tfar = smallest_tmax;

    if(_tnear < 0)
    	_tnear = 0; // clamp to near plane

    return smallest_tmax > largest_tmin;
}

/******************************************************************************/

void Ray::MarchRay(float step)
{
	double t = _tnear;
	while(t < _tfar)	// while i'am in the box collect depths
	{
		t+= step;
		_depths.push_back(t);
	}
}

/*******************************************************************************/
std::vector<float> Ray::getDepthsSampled()const
{
	return _depths;
}

glm::vec3 Ray::Estimate3Dposition(int DepthListIndex)const
{
	glm::vec3 d(_direction.x,_direction.y,_direction.z);
	glm::vec3 Pos3D = _origine + (  _depths[DepthListIndex] * d ); // pos(t) = o + d * t  ray equation easy as f***
	return Pos3D;
}
/******************************************************************/
glm::vec2 Ray::getTnea_far()const
{
	return glm::vec2(_tnear,_tfar);
}
