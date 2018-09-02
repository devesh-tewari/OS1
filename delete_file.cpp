#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

char* get_file_name_from_path(char* path)  //this function returns the last folder from an input path
{
    string str;
    str=path;
    int k=str.find_last_of("/");
    if(k==-1)  //if the input does not have any slashes
	return path;
    int size =str.size();
    string name=str.substr(k+1,(size-k));
    //printf("\nname: %s\n",name);
    //cout <<"NAME: "<< name;
    return &name[0];
}

int main( int argc, char* argv[] )  //1st argument=filename 2nd: home path
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
	//char name[100];
	//name = get_file_name_from_path(argv[1]);
	strcat(bin_path, get_file_name_from_path(argv[1]) );
		//cout<<argv[1]<<" "<<bin_path;

	if (   rename( argv[1] , bin_path )   == 0 )  //move the file to RecycleBin
	{
		fstream file;
		file.open( restore_file , ios::out | ios::app );
		//cout<<get_file_name_from_path(argv[1]);
		file << (string)get_file_name_from_path(argv[1]);   //append the name and path of deleted file to the recyclePath file
		file << " ";
		file << argv[1];
		file << endl;
		file.close();
	}
	else
	{
		cout<<" Error! ";
	}
	
	return 0;
}
