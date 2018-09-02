#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int main( int argc, char* argv[] )  //1st argument=filename, 2nd: home path
{
	if( argc != 3 )
	{
		cout<<" Error! ";
		return 0;
	}

	char bin_path[400];
	strcpy( bin_path , argv[2] );
	strcat(bin_path,"/RecycleBin");

	char restore_file[400];
	strcpy( restore_file , bin_path );
	strcat( restore_file , "/recyclePath.txt" );

	strcat(bin_path,"/");
	strcat(bin_path,argv[1]);

	string word, path;
	bool found=false;

	string paths[600];
	int i=0;

	fstream file;
	file.open( restore_file );

	while (file >> word)
	{
        	if( strcmp( word.c_str() , argv[1] ) == 0  )
		{
			file >> path;   //extract the recycle path(from where it was deleted) corresponding to the filename
			found=true;
			file >> word;
		}
		paths[i] = word;
		i++;
	}
	file.close();

	if(found)
	{
		
		rename( bin_path , &path[0] );

		file.open ( restore_file , ios::out | ios::trunc );  //clear the file
		file.close();
		
		file.open ( restore_file , ios::out | ios::app );
		int j=0;
		while(j<i-1)
		{
			file << paths[j++];   //the recyclePath file is serstored after deleting the restored file
			file << " ";
			file << paths[j++];
			file << endl;
		}
	}
	file.close();
	
	return 0;
}
