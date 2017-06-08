/*
 * neighbers.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: hakosoft
 */

#include "neighbers.h"

void neighbers::Initialise(unsigned int R,int k)
{
	_refrenceView = R;
	_k = k;

}

neighbers::neighbers()
{
	// TODO Auto-generated destructor stub
}
neighbers::~neighbers()
{
	// TODO Auto-generated destructor stub
}

void neighbers::CollectNeighboringView(const glm::vec2* Lats_longs,int arraySize,const float& DegreeThreshol)
{
	glm::vec2 Refrence = Lats_longs[_refrenceView];

	int counter = 0;
	for(int i = 0; i < arraySize; i++)
	{
		if(i != _refrenceView)	// we don't need to see the diffrence betewen the same view
		{
			glm::vec2 nieghbor = Lats_longs[i];

			float angularDistance = AngularDistance(Refrence,nieghbor);

			if(counter < _k) 				// k is the number of neiggbers we want to use if stilld didn' pass that number add one more
			{
				if(angularDistance <= DegreeThreshol)
				{
					counter++;
					_Ck.push_back(i);		// add the refrence of the view to the list
				}
			}
		}
	}
}

std::vector<unsigned int> neighbers::getCk()const
{
	return _Ck;
}
