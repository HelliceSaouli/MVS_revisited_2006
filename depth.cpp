/*
 * depth.cpp
 *
 *  Created on: Jul 20, 2016
 *      Author: hakosoft
 */

#include "depth.h"

depth::depth() {
	// TODO Auto-generated constructor stub

}

depth::~depth() {
	_ck.clear();
	_cv.clear();
}

/************************************************************************/
float depth::getDetph()const
{
	return _di;
}

float depth::getConfValue()const
{
	return confidence_d;
}

float depth::getCorr_d()const
{
	return corr_d;
}
/************************************************************************/

void depth::Initialization(const unsigned int &y,const unsigned int& x,const unsigned int& refrenceview ,const float& depth , const std::vector<unsigned int> ck, const unsigned int& vectorsize, const glm::vec3& pos3D)
{
	_refe  = refrenceview;
	_y 	   = y;
	_x     = x;
	_di    = depth;
	_valid = false;

	nccData element;

	for(int i = 0; i < ck.size(); i++)
	{
		element.c = ck[i];
		_ck.push_back(element);
	}
	n_x_m_Size = vectorsize;
	_position_3D = pos3D;

}

void depth::ComputeNCCDomaines(const View& images)	// this the worst and hardest function ever
{

	// let's start with v0 it is vector of image refrence and it calculated one time
	glm::vec3*  v0 = new glm::vec3[n_x_m_Size];
	cv::Mat Roi_0 = images.getROI0(_y,_x,_refe,n_x_m_Size);

	//convert roi matrix to vector
	int idx = 0;
	for(int i  = 0; i< Roi_0.rows; i++ )
	{
		for(int j = 0; j <Roi_0.cols; j++)
		{
		    cv::Vec3b intensity = Roi_0.at<cv::Vec3b>(i,j);
		    v0[idx] = glm::vec3 ((float)intensity.val[0],(float) intensity.val[1],(float) intensity.val[2]);
		    idx++;
		}
	}
	// clear the Roi
	Roi_0.release();

	// do the same thing this time with neighbor images but here i have to project the 3D point of depth to vies so i can get u and v
	for(int i = 0 ; i < _ck.size(); i++)
	{
		glm::vec3*  v1 = new glm::vec3[n_x_m_Size];

		glm::mat4 P = images.getThe_i_projectionMatrice(_ck[i].c);	// the  ck neighbor projection matrix

		glm::vec2 screen = Projection(_position_3D,P);	// project the estimated 3D position of the depth to images
		int u = (int)screen.x;
		int v = (int)screen.y;

		if(isInside(u,v,images._hight,images._width) == false)// the projected depth is not inside the neigbhor view so v1 will have all 0 value
		{
			for(int a = 0; a < n_x_m_Size;a++ )
				v1[a] = glm::vec3 (255.0, 255.0, 255.0);

		}
		else
		{
			cv::Mat Roi_1 = images.getROI1(v,u,_ck[i].c,n_x_m_Size);

			//convert roi matrix to vector
			int idx = 0;
			for(int i  = 0; i< Roi_1.rows; i++ )
			{
				for(int j = 0; j <Roi_1.cols; j++)
				{
					cv::Vec3b intensity = Roi_1.at<cv::Vec3b>(i,j);
					v1[idx] = glm::vec3 ((float)intensity.val[2],(float) intensity.val[1],(float) intensity.val[0]);
					idx++;
				}
			}

			// clear the Roi
			Roi_1.release();
		}

		// let's feed our v0 and v1 to our _ck array  speacilay the NCC element
		_ck[i].ncc_d.Initializtion(v0,v1,n_x_m_Size);	// omg sooooo hard lol

		delete[] v1;
	}
	delete[]v0;
}

void depth::ComputeNCC()		// this one is izi loop in the vector _ck and ask the element ncc of the data struct nccData to  compute the ncc value since we arleady offred all what we need to calculate ......blabla bla am tiered as fuck
{
	for(int i = 0 ; i < _ck.size(); i++)
	{
		_ck[i].ncc_d.Compute();
		_ck[i].ncc_d.clearVectors();	//after i computed the ncc value and store it i think i can free some memory v0;v1 ithink the memory leak was here
	}
}

//We therefore define that a depth value d is valid if NCC(R, Cj , d) is larger than some threshold thresh for at least two views in C.
void depth::Valid(const float &threshold)
{

	int counter = 0;
	for(int i  = 0; i < _ck.size(); i++)	// count how many we corrlated view we had
	{

		if(_ck[i].ncc_d.getNCC() > threshold)
			counter++;
	}
	if(counter >= 2)// this mean th depth is considred valid we go and collect _cv so we can calculate corr(d)
	{
		_valid = true;

		for(int i  = 0; i < _ck.size(); i++)	// count how many we corrlated view we had
		{
			if(_ck[i].ncc_d.getNCC() > threshold)
				{
					nccData element = _ck[i];
					_cv.push_back(element);	// feeling up the   The set of all views with NCC larger than thresh for a given depth d is denoted as Cv (d).

				}
		}
	}
	else
		_valid = false;
}

bool depth::isValid()
{
	return _valid;
}
//For a valid depth d we compute a correlation value corr(d) as the mean of the NCC values of all views in Cv (d):

void depth::ComputeCorrolation()	// this must be done after checking if the depth is valid
{
	float some = 0.0;

	for(int i = 0; i < _cv.size(); i++ ) //run throw _cv and some the ncc easy
	{
		some += _cv[i].ncc_d.getNCC();
	}

	corr_d = some / _cv.size();
}

void depth::ComputeConfidence(const float &threshold)	// use this function only if the depth is confiremed to be the one we need mean after find that this depth  has the max corr(d)
{
	float some = 0.0;

	for(int i = 0; i < _cv.size(); i++ ) //run throw _cv and some the ncc easy
	{
		some += (_cv[i].ncc_d.getNCC() -threshold) ;
	}

	float downfraction = _ck.size() * (1- threshold);

	confidence_d = some / downfraction;
}
