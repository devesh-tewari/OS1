#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

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
	cout<<dst;

	ofstream file( dst );

	return 0;
}
