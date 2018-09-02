#include <string>
#include <cstring>
#include <iostream>
#include <sys/stat.h>

using namespace std;

int main( int argc, char* argv[] )
{
	if( argc != 3 )
	{
		cout<<" Error! ";
		return 0;
	}

	char dst[400];
	
	strcpy( dst , argv[2] );
	strcat( dst , "/" );
	strcat( dst , argv[1] );

	if(   mkdir( dst , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )   == -1)
		cout<<" Error! ";
}
	
