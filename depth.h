/*
 * depth.h
 *
 *  Created on: Jul 20, 2016
 *      Author: hakosoft
 *
 *      this class will do the heavy leftting almost
 *      this class will store first of all a single  value of dethp wich is calculated using the ray intersection
 *      that mean to march the ray we will have an array of this class to store data in it
 *
 *		so it will be like this depth d[numberofsimplesperarra]
 *
 *		we will use a bool called valid to mark the deth as valid for other uses or not according to the articl
 *		also we need 3D estimated position P this P will be projected into Ck view and return v1 the vector used for
 *		Normalized cross corlloation
 *
 *		so we need Ck and P, finally Cv(d)
 *		i guess list of Ncc is needed to so we can compare to a threshold
 */

#ifndef DEPTH_H_
#define DEPTH_H_

#include "headers.h"
#include "View.h"
#include "NCC.h"

#include "glm/glm.hpp"

struct nccData
{
	unsigned int c;			// wich image  am working on
	NCC ncc_d;				// it relative ncc
};


class depth
{
	private:
		float _di;							// the i_th depth d of a pixel (u,v) of an R refrence view
	    unsigned int _x,_y;				// pixel corrdinate
		unsigned int _refe;					// view refrence id
		bool _valid;						// to confirme that this particluer depth is valid
		std::vector<nccData> _ck;			// list of nigthbor view ck with thier ncc stored in one data
		std::vector<nccData> _cv;		    // The set of all views with NCC larger than thresh for a given depth d
		glm::vec3 _position_3D;				// the estimated 3D posiotion --> using ray.estimate3D()


		float corr_d;						//for a valid depth d we compute a correlation value corr(d)
		float confidence_d;					// We also compute a confidence value conf(d) for each re-covered depth value this value is calculated only if this depth win a trade

		unsigned int n_x_m_Size;			// the size of the domain vectors
	public:
		depth();
		virtual ~depth();

		// data we get to use in depth map in othere word the output of this class is the depth and it's confidence value
		float getDetph()const;
		float getConfValue()const;
		float getCorr_d()const;

		void Initialization(const unsigned int &,const unsigned int&,const unsigned int& ,const float&, const std::vector<unsigned int>, const unsigned int&, const glm::vec3&);	// intialization all i need is the depth the view refrence we are using, and it's neighbors view also we need to tell that th depth is not valid yet also we need the 3D estimated position
		void ComputeNCCDomaines(const View&);		// this will compute the v0 and v1 the 2 need domain vector that are used
		void ComputeNCC();							// this will compute the NCC for every neighbore view

		void Valid(const float&);					// this function will check if the depth is valid that mean at least there is 2 nieghbor view where the NCC is bigger than threshold and build cv list
		void ComputeCorrolation();					// it's simply the mean NCC from list of cv
		void ComputeConfidence(const float&);		// follow the formula on the article


		bool isValid();

		/******************************************* some inline function we need **************************************/
		inline glm::vec2 Projection(glm::vec3 Pos, glm::mat4 M)
		{
			glm::vec4 A(Pos.x,Pos.y,Pos.z,1.0);
			glm::vec4 w(0.0);

			w = M * A;

			glm::vec2 screen(w.x/w.z,w.y/w.z);

			return screen;
		}

		bool isInside(int u, int v, int h, int w)				// to make sure that the projected
		{
			if((u >= 0  && u < w) && (v  >=0 && v < h))
				return true;
			else
				return false;
		}
};

#endif /* DEPTH_H_ */
