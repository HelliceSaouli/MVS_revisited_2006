#ifndef HEADERS_H_
#define HEADERS_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <iterator>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>



#ifndef PI
# define PI 3.1415926535897932384626433832795                          //The number PI used in math
#endif

#ifndef LOG2
# define LOG2 0.69314718055994530941723212145818                       // The log
#endif


/* Define some helpful MACRO */
#ifndef SWAP
# define SWAP(a,b,c) ((c) = (a), (a) = (b),  (b) = (c))                  // Swapping between a and b
#endif

#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))								 // the min between a and b
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))								// the max between a and b
#endif

#ifndef CLAMP
# define CLAMP(x,min,max) ( (x) < (min) ? (min) : ((x > max) ?  (max) : (x)) ) // this will help a lot for example clamping color  when add them togthere
#endif

inline
float DegreeToRad(float Angle)
{
	return (Angle  * ( PI / 180.00));
}

inline
float RadToDegree(float Angle)
{
	return (Angle * 180.00) / PI;
}
#endif
