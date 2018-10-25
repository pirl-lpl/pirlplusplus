/*	stub
*/

#include	<iostream>
#include	<iomanip>
#include	<cstdlib>
#include	<limits>
using namespace std;


#ifndef COORDINATE_TYPE
#define COORDINATE_TYPE int
#endif
#ifndef	DIMENSIONS_TYPE
#define	DIMENSIONS_TYPE unsigned COORDINATE_TYPE
#endif

#define AS_STRING(name)			STRINGIFIED(name)
#define STRINGIFIED(name)		#name


int
main
	(
	int		count,
	char**	arguments
	)
{
cout << boolalpha
	 << "COORDINATE_TYPE is signed? "
	 	<< numeric_limits<COORDINATE_TYPE>::is_signed << endl
	 << "DIMENSIONS_TYPE (" << AS_STRING (DIMENSIONS_TYPE) << ") is signed? "
	 	<< numeric_limits<DIMENSIONS_TYPE>::is_signed << endl;

exit (0);
}
