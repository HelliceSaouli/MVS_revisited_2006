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
	/*glm::vec2 Refrence = Lats_longs[_refrenceView];
	int counter = 0;
	for(int i = 0; i < arraySize; i++)
	{
		if(i != _refrenceView)	// we don't need to see the diffrence betewen the same view
		{
			glm::vec2 nieghbor = Lats_longs[i];
			float angularDistance = AngularDistance(Refrence,nieghbor);


				if(angularDistance <= DegreeThreshol && counter < _k)
				{
					counter++;
					_Ck.push_back(i);		// add the refrence of the view to the list
				}


		}
	}*/


	// this down is ball shit but worth trying and didn't work
	_Ck.push_back((_refrenceView + 1) % arraySize);
	_Ck.push_back((_refrenceView + 2) % arraySize);


	if(_refrenceView == 0)
	{
		_Ck.push_back((arraySize - 1));
		_Ck.push_back((arraySize - 2));
	}
	else if(_refrenceView == 1)
	{
		_Ck.push_back((_refrenceView - 1));
		_Ck.push_back((arraySize - 1));
	}
	else
	{
		_Ck.push_back(_refrenceView - 1);
		_Ck.push_back(_refrenceView - 2);
	}
}

std::vector<unsigned int> neighbers::getCk()const
{
	return _Ck;
}
