/*
 * NCC.h
 *
 *  Created on: Jul 20, 2016
 *      Author: hakosoft
 *
 *      this class is going to calculate the normalized cross corrolation betewen 2 vector v0,v1
 *      v0 is color vector of m x n size from block of pixel in refrence image
 *      v1 is color vector of m x n size from block of pixel in a j nieghbor view
 *
 *      the giving color here are not normalized so before any calculation is done we have to normalize
 *      this class result one value is NCC(R,Cj,d)..this value must be stored in some other class for later use
 */

#ifndef NCC_H_
#define NCC_H_

#include "headers.h"
#include "glm/glm.hpp"

class NCC
{
	private:
		glm::vec3*  _v0;			// vector of color from  refrence image of size m x n
		glm::vec3*  _v1;			// vector of color from  j nieghbor image of size m x n
		unsigned int _vectorSize;

		glm::vec3 _MeanV0;			// the averge of v0
		glm::vec3 _MeanV1;			// the averge of v1

		float _ncc;					// this the result it must be betewen [-1,1]
		// function that normalize color helper function
		glm::vec3 normalizeColor(const glm::vec3&)const;	// it's normalizers we deal with rgb like any 3D vector  so to normalize it we calculate the length than devid each element by it
		// function that compute the mean color of the v0 and v1
		void Averges();					// this function will take the normalized color vector v0 and v1 and find the avereges _MeanV0 and MeanV1

	public:
			NCC();
			virtual ~NCC();

			void Initializtion(const glm::vec3*,const glm::vec3*, const unsigned int&);

			void Compute();	// here we going to compute the normalized cross corrolation like the artical Multi-View Stereo Revisited Michael Goesele and al suggested
			float getNCC(); // return the resulting NCC


			void clearVectors();


};

#endif /* NCC_H_ */
