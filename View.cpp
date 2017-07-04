/*
 * ImageData.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: hakosoft
 */

#include "View.h"

void View::CreateImageData(unsigned int N, unsigned int vectorSize)	// vector size is the kernel m x m size and it is the amount  of pixel we are going to add
{
	_N = N;
	_images 		= new cv::Mat[_N];
	_padedImages	= new cv::Mat[_N];
	_projection 	= new glm::mat4[_N];
	_CameraPosition = new glm::vec3[_N];
	_Lat_log		= new glm::vec2[_N];

	// load images
	char* imagefileName = new char[255];
	//std::cout << "Load and store  data"<< std::endl;
	for(unsigned int i = 0; i < _N; i++)
	{

		// first load images
		sprintf(imagefileName,"resources/Data sets/Temple 47 image/visualize/%02d.png",i);
		std::string buffer(imagefileName);
		_images[i] = cv::imread(buffer); // read the image


	    if(! _images[i].data )                              // Check for invalid input
	    {
	        std::cout <<  "Could not open or find the image" << std::endl ;
	       exit(0);
	    }

	    _hight  = _images[i].rows;
	    _width  = _images[i].cols;
	}

	delete[]imagefileName;
	//std::cout << "Load Camera Projection matrices and estimate camera position"<< std::endl;
	//load projection matrix
	char* projectionfileName = new char[255];
	glm::mat4 Projection_Matrix;


	std::ifstream filep2("resources/Data sets/Temple 47 image/RotationMatrices.txt",std::ifstream::in);
    //check if file opened
    if(!filep2)
    {
        std::cout << " unable to open file Camera R !!"<<std::endl;
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
        std::cout << " unable to open file Camera Angular !!"<<std::endl;
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
			InveR = R.t();
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

			_Lat_log[i].x = -Ang[0];
			_Lat_log[i].y = Ang[1]-180;

			//clean
			R.release();
			InveR.release();
			T.release();

			// add border to images
			_padedImages[i] = _images[i];

				// we add exctra information in boundry  kernelsize pixel in all direction
				  int top, bottom, left, right;
				  top = bottom = left =  right = (int) sqrt(vectorSize);
				  cv::copyMakeBorder( _images[i], _padedImages[i], top, bottom, left, right, cv::BORDER_REPLICATE); // we replicate the boundingbox //BORDER_REPLICATE


		}
	filep2.close();
	filep3.close();
	filep4.close();
	delete[]projectionfileName;
}

View::View()
{
}
View::~View() {
}


glm::vec3 View::getColor(unsigned int ImageID,unsigned int u, unsigned int v)const
{
    cv::Vec3b intensity = _images[ImageID].at<cv::Vec3b>(v,u);
	glm::vec3 color ((float)intensity.val[2], (float)intensity.val[1], (float)intensity.val[0]);

	return color;
}

float View::getImageNumber()const
{
	return _N;
}

glm::mat4 View::getThe_i_projectionMatrice(unsigned int imageID)const
{
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

cv::Mat View::getROI0(unsigned int y, unsigned int x,unsigned int ImageID,unsigned int kernelsize)const
{
	// we add exctra information in boundry  kernelsize pixel in all direction
	  int top, bottom, left, right;
	  top = bottom = left =  right = (int) sqrt(kernelsize);

	  // know we calculate the new u and v that are gonna be in the padded image

	  int yy = y + left;
	  int xx = x + top;

	  // let's try to construct our rectongle for ROI
	  int _x,_y,w,h;

	  w = h = (int)sqrt(kernelsize);

	  _x = xx - int(w/2);
	  _y = yy - int(h/2);
	  cv::Rect Rec(_x, _y, w, h);	// some how it's fliped i don't know why
	  cv::Mat Roi = _padedImages[ImageID](Rec);
	//  _padedImages[ImageID].release();
	  return Roi;

}


cv::Mat View::getROI1(unsigned int v, unsigned int u,unsigned int ImageID,unsigned int kernelsize)const
{


	// we add exctra information in boundry  kernelsize pixel in all direction
	  int top, bottom, left, right;
	  top = bottom = left =  right = (int) sqrt(kernelsize);



	  // know we calculate the new u and v that are gonna be in the padded image

	  int yy = v + left;
	  int xx = u + top;

	  // let's try to construct our rectongle for ROI
	  int _x,_y,w,h;

	  w = h = (int)sqrt(kernelsize);

	  _x = xx - int(w/2);
	  _y = yy - int(h/2);
	  cv::Rect Rec(_x, _y, w, h);	// some how it's fliped i don't know why
	  cv::Mat Roi = _padedImages[ImageID](Rec);
	  //_padedImages[ImageID].release();
	  return Roi;

}
