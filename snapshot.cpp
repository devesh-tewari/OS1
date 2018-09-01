#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

bool is_dir(const char* path)  //returns true if input is a directory
{
   struct stat statbuf;
   bool isDir = false;
   if (stat(path, &statbuf) != -1) 
   {
	if (S_ISDIR(statbuf.st_mode)) 
	{
	   isDir = true;
	}
   }
   return isDir;
}

char slash[2]="/";

int snapshot(char* folder, char* dmp)
{
    ofstream dumpfile;
    dumpfile.open ( dmp , ios::out | ios::app );
    dumpfile<<(string)folder;
    dumpfile<<":";
    DIR* p;
    struct dirent *d;
    p=opendir(folder);        //opendir opens it's input directory and returns a directory pointer
    if(p==NULL)
    {
	perror("Cannot find directory\n");
	return 0;
    }
    vector<string> dir_list;   //list of directories in the current folder
    char filepath[PATH_MAX];
    
    while(d=readdir(p))    //keep reading files of directory until none are left
    {
	if(  ( strcmp(d->d_name,".")!=0 )  &&  ( strcmp(d->d_name,"..")!=0 )  )
	{
		strcpy(filepath,folder);
		strcat(filepath,slash);
		strcat(filepath,d->d_name);
		if(is_dir(filepath))
		{
			string temp(filepath);
			dir_list.push_back(temp);
		}
		dumpfile<<endl<<(string)d->d_name;
	}
    }

    int dir_count = dir_list.size();

    for(int i=0;i<dir_count;i++)
    {
	dumpfile<<endl<<endl;
	snapshot (  &dir_list[i][0] , dmp );  //recursively print the directories inside the current folder
    }

    dumpfile.close();
    return 1;
}
    

int main(int argc, char* argv[]) 
{
    if(argc!=3)
    {
	cout<<"Insuffecient arguments";
	return 0;
    }
    /*if( !is_dir(argv[1]) )
    {
	cout<<"Folder not found";
	return 0;
    }*/
    ofstream dumpfile;
    dumpfile.open ( argv[2] , ios::out | ios::trunc );  //clear the dumpfile
    dumpfile.close();

    snapshot(argv[1],argv[2]);
    
    //argv[2]
    
    return 0;
}
