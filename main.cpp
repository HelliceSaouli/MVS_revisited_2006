
#include "headers.h"
#include "View.h"
#include "Ray.h"
#include "neighbers.h"
#include "depth.h"


int main(int, char**)
{
	// the volume bounding box
	//std::cout<<"1-Initialization"<<std::endl;
	glm::vec3 bbmin(-0.023121,-0.038009, -0.091940);
	glm::vec3 bbmax(0.078626,  0.121636, -0.017395);

	float Threshold = 0.6;	// the threshold for NCC
	float Step = 0.0025;		// steps to march ray  0.0025 = 2.5 mm;
	int k = 4;				// the number of max neighbors
	unsigned int vectorsize = 5 * 5;		// the domain size 5x5 window
	unsigned int number_view = 47;

	//std::cout<<"2- Creating list of Views"<<std::endl;
	View V;
	V.CreateImageData(number_view,vectorsize); // Veiws V = { V0.......Vn-1}
	unsigned int Width = V._width;
	unsigned int Hight = V._hight;
	// list of resulted depth maps
	//std::cout<<"3-Initializaing the list of depth maps"<<std::endl;
	cv::Mat depth_map[number_view];
	for(unsigned int i = 0; i < number_view; i++)
		depth_map[i].create(Hight,Width,cv::DataType<float>::type);

	//std::cout<<"4-Initializing the lis of nieghbor for every view"<<std::endl;
	// let's find the niegborhrs of every view
	neighbers ListOfneighbers[number_view];

	//std::cout<<"5-get lattitude and longtitude to use later"<<std::endl;
	// let's start the algorithm
	glm::vec2* Lat_Long = V.getAllLatitudeandLongtitdue();	// this will help to calculate the angular distance so we can find the nieghbors

	//std::cout<<"6-Calculate the lis of nieghbor for every view"<<std::endl;
	// initiliaze and calulate niegbors for direct use next time
	for(unsigned int n= 0; n < number_view; n++)
	{
		ListOfneighbers[n].Initialise(n,k);	// only 4 neighbers
		ListOfneighbers[n].CollectNeighboringView(Lat_Long,number_view,4.0);// 4.0 degree // this worng need to be corrected
	}
	//std::cout<<"7-start calculating the depth maps"<<std::endl;
	for(unsigned int n = 0; n < number_view; n++)
	{
			// get the position of camera and
			std::cout<<"a-The "<<n<< " -th Projection and position is calculated"<<std::endl;
			glm::mat4 P = V.getThe_i_projectionMatrice(n);	// the projection matrix of this specifc view
			glm::vec3 Camera_Origine = V.getThe_i_CameraPosition(n);
			std::vector<unsigned int> ck = ListOfneighbers[n].getCk();	// list ck of niegboring view
			// we no start to cast rays from each pixel
			//std::cout<<"b-The"<< n<< " view is going to be refrence "<<std::endl;

			float ctr = 0.0;	// just for checking how much left in calculation
			for(unsigned int x = 0; x < Width  ; x++)
			{
				for(unsigned int y = 0; y < Hight; y++)
				{
					//std::cout<<"-The ("<< x<<","<<y<<") of the "<<n<<" is acceed "<<std::endl;
					// let's create and cast ray in bounding box
					glm::vec2 uv(x,y);	//in reality we are in pixel(y,x) in original image

					Ray BackProjected(Camera_Origine,uv,bbmin,bbmax,P);
					//std::cout<<"-The ray("<< x<<","<<y<<") of the "<<n<<" is created "<<std::endl;

					bool hit = BackProjected.Hit();
					//std::cout<<"-The ray("<< x<<","<<y<<") of the "<<n<<" check for hit "<<std::endl;
					// if we get hit with bounding box march the ray cocllect depth samples and do the corrlolation else set depth to infinity
					if(!hit)
					{
						depth_map[n].at<float>(y,x) = 725.0 ;

						//std::cout<<"-The ray("<< x<<","<<y<<") of the "<<n<<" didn't hit "<<std::endl;
					}
					else	// we get the hit
					{
						// list of possible depths for every pixel (x,y)
						std::vector<depth> di;
						//std::cout<<"-The ray("<< x<<","<<y<<") of the "<<n<<" did  hit "<<std::endl;
						BackProjected.MarchRay(Step);
						//std::cout<<"-The ray("<< x<<","<<y<<") of the "<<n<<" marching "<<std::endl;
						std::vector<float> depths = BackProjected.getDepthsSampled();

						//now the hard part
						int size = depths.size();
						//std::cout<<"-The ray("<< x<<","<<y<<") of the "<<n<<" collected "<< size<< " depth value"<<std::endl;
						for(unsigned int l = 0; l < size; l++)	// for every element depth in the march ray
						{
							depth d;

							// some variable to intialize class depth
							glm::vec3 Esitimatedpos = BackProjected.Estimate3Dposition(l); // the estimated 3D position of the depth d stored in the l position in the list depths
							//std::cout<<"the "<<l<<" th The ray("<< x<<","<<y<<") of the "<<n<<"  collected it's niegbhor and estimated it 3D pos"<<std::endl;

							d.Initialization(y,x,n,depths[l],ck,vectorsize,Esitimatedpos);
							d.ComputeNCCDomaines(V);
							d.ComputeNCC();
							d.Valid(Threshold);

							//std::cout<<"the depth"<<l<<" of  The ray("<< x<<","<<y<<") of the "<<n<<" computed it's stuff "<<std::endl;
							//check if the d is valid
							if(d.isValid())
							{
								//std::cout<<"the depth "<<l<<" oth The ray("<< x<<","<<y<<") of the "<<n<<" is  valid "<<std::endl;
								d.ComputeCorrolation();
								di.push_back(d);
							}

						}

						if(di.size() >= 1)  // if there is valid depth othere wise it's a hole empty
						{
						// now let's find the max of Corrloation in  all depths
							unsigned int index_of_correct_depth = 0;
							float maxCorrloation = di[0].getCorr_d();
							for(int s = 1; s < di.size(); s++)
							{
								if(di[s].getCorr_d() >= maxCorrloation)
								{
									maxCorrloation = di[s].getCorr_d();
									index_of_correct_depth = s;
								}
							}

							// the depth value is the depth reside in lis di it's index is  index_of_correct_depth
							di[index_of_correct_depth].ComputeConfidence(Threshold);	// it's confidence value
							//save the depth in it corrsponding pixel in depth_map
							depth_map[n].at<float>(y,x) = di[index_of_correct_depth].getDetph() * 1000.0; // convert to mm i guess
						}
						else
							depth_map[n].at<float>(y,x) = 725.0;

						depths.clear();
						di.clear();
					}

					ctr += 1.0;
					//std::cout << (ctr * 100.0) / (Hight * Width) << " %"<< std::endl;
				}
			}

			ck.clear();
			double min;
			double max;
			cv::minMaxIdx(depth_map[n], &min, &max);
			cv::Mat adjMap;
			std::cout<<min<<" "<<max<<std::endl;
			// expand your range to 0..255. Similar to histEq();
			depth_map[n].convertTo(adjMap,CV_8UC1, 255 / int(max-min),-min);

			cv::Mat falseColorsMap;
			cv::applyColorMap(adjMap, falseColorsMap, 2);  // cv::COLORMAP_AUTUMN

			char* fileName = new char[255];
		     sprintf(fileName,"depth%03d.png",n);
			cv::imwrite(fileName, falseColorsMap) ;


	}

	return 0;
}
