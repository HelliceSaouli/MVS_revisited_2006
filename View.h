/*
 * ImageData.h
 *
 *  Created on: Jun 21, 2016
 *      Author: hakosoft
 */

#ifndef VIEW_H_
#define VIEW_H_
#include "headers.h"

#include <opencv2/opencv.hpp>
#include "glm/glm.hpp"



//this class will just load images and put it into big tabel since we are not in GPU and stuff will be easy
//camera paramtre will be loaded also

class View
{
	private:

		cv::Mat* 	_images;   					// the data holer for images to mark the pixel according to space carving algo we will use -1 for marked else not
		cv::Mat*	_padedImages;				// the same view but they have extrat pixel to deal with boreders
		glm::mat4* 	_projection; 				// the projection matrices from calibration process
		glm::vec3*  _CameraPosition;		   // this will store camera position that we can get from decomposing projection matrix
		glm::vec2*  _Lat_log;				  // latitude and longtitude so we can calculate the angular distance so we can pick the K nieghbore view


	public:
		int _width,_hight;
		unsigned int _N;			// the number of input images and it hight and width

		void CreateImageData(unsigned int,unsigned int);
		View();
		virtual ~View();

		float getImageNumber()const;
		glm::mat4 getThe_i_projectionMatrice(unsigned int)const;		// return a specifc projection matrix
		glm::vec3 getThe_i_CameraPosition(unsigned int)const;
		cv::Mat   getThe_i_View(unsigned int)const;
		glm::vec3 getColor(unsigned int, unsigned int,unsigned int)const;	// this wil return the pixel color
		glm::vec2 getThe_i_LatitudeandLongtitdue(unsigned int)const;
		glm::vec2* getAllLatitudeandLongtitdue()const;

		cv::Mat getROI0(unsigned int, unsigned int,unsigned int, unsigned int)const;	// this so stupid omg am i get worst
		cv::Mat getROI1(unsigned int, unsigned int,unsigned int, unsigned int)const;
};

#endif /* VIEW_H_ */
