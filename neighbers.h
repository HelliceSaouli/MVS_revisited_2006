/*
 * neighbers.h
 *
 *  Created on: Jul 19, 2016
 *      Author: hakosoft
 *
 *      this class contain the K neighboring  view for a giving R refrence view
 *      to get the neighboring view we calculate the angular distance using longtitiude and latitude
 *
 *      to smplify the implementation  this class will use only index say we have a refrence View From the class View R this
 *      image is stored in the 10 element array with this class we will find the indexs of neighboring  view and store them in an array
 *      So R is the 10 element in view images its neighbers are the view number 12, 16, 26 for example
 */

#ifndef NEIGHBERS_H_
#define NEIGHBERS_H_

#include "headers.h"
#include "glm/glm.hpp"
class neighbers
{
	private:
		unsigned int _refrenceView;					// this is the index of the refrence view
		std::vector<unsigned int> _Ck; 				//set of k neighboring views their refrences the view are stored in View class array
		int _k;										// number of neighboring views

	public:
		neighbers();				//wich refrence we are using and how many  neighbores we want
		virtual ~neighbers();

		void Initialise(unsigned int,int);
		void CollectNeighboringView(const glm::vec2*,int,const float&);					// this will take array of longtitude and latiitude calculate the angular distance betewene te refrence view and all othere view if the angular distance is smallar or equal to threshold
																						// we will add the index of the view to _Ck we will add according to k
		std::vector<unsigned int> getCk()const;											// return the list of  neighboring views

		inline float AngularDistance(const glm::vec2& A, const glm::vec2& B)	// this is actualy not the angula distance but the bearing betewen 2 point but the orginal artical used the word  angular distance to express the bearing
		{
			float Theta;	// this is in radianet

			float Lat1  = DegreeToRad(A.x);
			float Lat2  = DegreeToRad(B.x);

			float Long1 = DegreeToRad(A.y);
			float Long2 = DegreeToRad(B.y);

			// the formula : θ = atan2(sin(Δlong)*cos(lat2), cos(lat1)*sin(lat2) − sin(lat1)*cos(lat2)*cos(Δlong))
			float DeltaLong =  Long2 - Long1;
			Theta = atan2(sin(DeltaLong)*cos(Lat2), (cos(Lat1) * sin(Lat2))- (sin(Lat1)*cos(Lat2) * cos(DeltaLong)) );

			return RadToDegree(Theta);

		}
};

#endif /* NEIGHBERS_H_ */
