/*
 * Ray.h
 *
 *  Created on: Jul 14, 2016
 *      Author: hakosoft
 *      a smiple class that represent  a ray  that have orgine and direction this ray is
 *      the backprojection of pixel
 */

#ifndef RAY_H_
#define RAY_H_

#include "headers.h"

#include "glm/glm.hpp"

class Ray
{
	private:
		glm::vec3 _origine;			// ray orgine this is gonna be the camera position center
		glm::vec4 _direction;		// ray direction

		glm::vec3 _bbmin,_bbmax; 	// the volume bounding box so ray know what to intersect
		float _tnear,_tfar;	  // the near and far intersection  with bounding box

		std::vector<float> _depths;		// if there is an intersection we march inside our bounding box collect depths this depths will be teseted for corrolation

	public:
		Ray(const glm::vec3&, const glm::vec2&, const glm::vec3&, const glm::vec3&,const glm::mat4&); // this will create a ray with all wha it need
		virtual ~Ray();

		bool Hit();					// check for intresection according to this function if it does intersect than ok return _depths else the pixel depth set to empty
		void MarchRay(float);				// this function will march the ray  with t value (in the article is set to be 2.5 mm and collect depths
		std::vector<float> getDepthsSampled()const;	// this return the depths we collected as one array


		glm::vec3 Estimate3Dposition(int)const;	// this function will calculate the 3D position given a depth value(in this case the index to  where the depth value is saved) this 3D position used to calculate the NCC

		// for debuging
		glm::vec2 getTnea_far()const;
};

#endif /* RAY_H_ */
