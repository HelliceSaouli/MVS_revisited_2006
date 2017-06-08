#include "Ray.h"
#include "View.h"
#include "neighbers.h"
#include "depth.h"


int main(int, char**)
{


	// the volume bounding box
	std::cout<<"1-Initialization"<<std::endl;
	glm::vec3 bbmin(-0.023121,-0.038009, -0.091940);
	glm::vec3 bbmax(0.078626,  0.121636, -0.017395);

	float Threshold = 0.6;	// the threshold for NCC
	float Step = 0.0025;		// steps to march ray  0.0025 = 2.5 mm;
	int k = 4;				// the number of max neighbors
	unsigned int vectorsize = 25;		// the domain size 5x5 window
	unsigned int number_view = 5;

	std::cout<<"2- Creating list of Views"<<std::endl;
	View V;
	V.CreateImageData(number_view); // Veiws V = { V0.......Vn-1}
	unsigned int Width = V._width;
	unsigned int Hight = V._hight;

	for(unsigned int n = 0; n < number_view; n++)
	{
		glm::mat4 P = V.getThe_i_projectionMatrice(n);
		//glm::vec3 Camera_Origine = V.getThe_i_CameraPosition(n);
		std::cout<<n<<std::endl;


	}
	return 0;
}
