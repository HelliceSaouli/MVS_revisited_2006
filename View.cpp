/*
 * ImageData.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: hakosoft
 */

#include "View.h"

void View::CreateImageData(unsigned int N)
{
	_N = N;
	_images 		= new cv::Mat[_N];
	_projection 	= new glm::mat4[_N];
	_CameraPosition = new glm::vec3[_N];

	// load images
	char* imagefileName = new char[255];
	//std::cout << "Load and store image data"<< std::endl;
	for(unsigned int i = 0; i < _N; i++)
	{

		// first load images
		sprintf(imagefileName,"resources/Data sets/Temple 47 image/visualize/%02d.png",i);
		std::string buffer(imagefileName);
		cv::Mat RGB;

		RGB = cv::imread(buffer,CV_LOAD_IMAGE_COLOR); // read the image
		_images[i].create(RGB.size(),CV_8UC4);
	    if(! RGB.data )                              // Check for invalid input
	    {
	        std::cout <<  "Could not open or find the image" << std::endl ;
	       exit(0);
	    }
	    cv::cvtColor(RGB, _images[i], CV_BGR2RGBA, 4);
	    _hight  = _images[i].rows;
	    _width  = _images[i].cols;

	    RGB.release();

	}

	delete[]imagefileName;
//	std::cout << "Load Camera Projection matrices and estimate camera position"<< std::endl;
	//load projection matrix
	char* projectionfileName = new char[255];
	glm::mat4 Projection_Matrix;


	std::ifstream filep2("resources/Data sets/Temple 47 image/RotationMatrices.txt",std::ifstream::in);
    //check if file opened
    if(!filep2)
    {
        std::cout << " unable to open file Camera position !!"<<std::endl;
        exit(EXIT_FAILURE);
    }

	std::ifstream filep3("resources/Data sets/Temple 47 image/CameraPosition.txt",std::ifstream::in);
    //check if file opened
    if(!filep3)
    {
        std::cout << " unable to open file Camera position !!"<<std::endl;
        exit(EXIT_FAILURE);
    }
	std::ifstream filep4("resources/Data sets/Temple 47 image/Angular.txt",std::ifstream::in);
    //check if file opened
    if(!filep4)
    {
        std::cout << " unable to open file Camera position !!"<<std::endl;
        exit(EXIT_FAILURE);
    }


	for(unsigned int i = 0; i < _N; i++)
	{
			// read projection Matrices P
				sprintf(projectionfileName,"resources/Data sets/Temple 47 image/projection/%d.txt",i);
				std::ifstream filep(projectionfileName,std::ifstream::in);


				if(!filep)
				{
					std::cout << " unable to open file!!!"<<std::endl;
					exit(EXIT_FAILURE);
				}
				for(int x = 0; x < 3 ; x++)
				{

					for(int y = 0; y < 4; y++)
					{
						filep>>Projection_Matrix[x][y];

					}
				}
				filep.close();

				Projection_Matrix[3][0] =  0.0;
				Projection_Matrix[3][1] =  0.0;
				Projection_Matrix[3][2] =  0.0;
				Projection_Matrix[3][3] =  1.0;
				_projection[i] = glm::transpose(Projection_Matrix);
			/******************************************************************************/

			// read Rotation Matrices R

			//some matrices
			cv::Mat R(3,3,cv::DataType<float>::type); // rotation matrix
			cv::Mat InveR(3,3,cv::DataType<float>::type); // rotation matrix inverse R^-1

			for(int a = 0; a < 3; a++)
			{
				for(int b= 0; b < 3 ; b++)
				{
					filep2>> R.at<float>(a,b);
				}
			}
			//std::cout << R <<std::endl;
			cv::invert(R,InveR);
			/**********************************************************************************/


			cv::Mat T(3,1,cv::DataType<float>::type); // translation vector
			cv::Mat Cameraposition(3,1,cv::DataType<float>::type); // camera center position = -R^-1 * T

			for(int k = 0; k < 3 ; k++)
			{
				filep3>>T.at<float>(k);
			}

			//std::cout << T <<std::endl;
			Cameraposition = -InveR * T;
			//std::cout << Cameraposition <<std::endl;
			_CameraPosition[i].x = Cameraposition.at<float>(0);
			_CameraPosition[i].y = Cameraposition.at<float>(1);
			_CameraPosition[i].z = Cameraposition.at<float>(2);

			float Ang[2];
			for(int e = 0; e < 2 ; e++)
			{
				filep4>>Ang[e];
			}

			_Lat_log[i].x = Ang[0];
			_Lat_log[i].y = Ang[1];

			//clean
			R.release();
			InveR.release();
			T.release();
		}
	filep2.close();
	filep3.close();
	filep4.close();
	delete[]projectionfileName;

}

View::View(){
}
View::~View() {
}


glm::vec3 View::getColor(unsigned int ImageID,unsigned int u, unsigned int v)const
{
    cv::Vec4b intensity = _images[ImageID].at<cv::Vec4b>(u,v);
	glm::vec3 color (intensity.val[0], intensity.val[1], intensity.val[2]);

	return color;
}

float View::getImageNumber()const
{
	return _N;
}

glm::mat4 View::getThe_i_projectionMatrice(unsigned int imageID)const
{
	std::cout<<"wtf is wrong"<<std::endl;
	return _projection[imageID];
}

glm::vec3 View::getThe_i_CameraPosition(unsigned int imageID)const
{
	return _CameraPosition[imageID];
}

cv::Mat View::getThe_i_View(unsigned int ImageID)const
{
	return _images[ImageID];
}

glm::vec2 View::getThe_i_LatitudeandLongtitdue(unsigned int ImageID)const
{
	return _Lat_log[ImageID];
}

glm::vec2* View::getAllLatitudeandLongtitdue()const
{
	return _Lat_log;
}

cv::Mat View::getROI(unsigned int u, unsigned int v,unsigned int ImageID,unsigned int kernelsize)const
{
	cv::Mat dst = _images[ImageID];

	// we add exctra information in boundry  kernelsize pixel in all direction
	  int top, bottom, left, right;
	  top = bottom = left =  right = kernelsize;
	  cv::copyMakeBorder( _images[ImageID], dst, top, bottom, left, right, cv::BORDER_REPLICATE); // we replicate the boundingbox //BORDER_REPLICATE


	  // know we calculate the new u and v that are gonna be in the padded image

	  int newU = u + left;
	  int newV = v + top;

	  // let's try to construct our rectongle for ROI
	  int startx,starty,w,h;

	  w = h = (int)sqrt(kernelsize);

	  startx = newV - int(h/2);
	  starty = newU - int(w/2);
	  cv::Rect Rec(startx, starty, w, h);	// some how it's fliped i don't know why

	  cv::Mat Roi = dst(Rec);
	  dst.release();
	  return Roi;


}
