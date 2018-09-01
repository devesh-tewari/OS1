#include <string>
#include <cstring>
#include <iostream>
#include <sys/stat.h>

#include <dirent.h>

using namespace std;

bool is_file(const char* path)  //returns true if the file exists
{
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

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

char slash[]="/";

int rename(char* filename, char* new_name , char* cwd)
{
    char* newNAME=new char[400];
    strcpy(newNAME,cwd);
    strcat(newNAME,slash);
    strcat(newNAME,new_name);
    DIR* p;
    struct dirent *d;
    p=opendir(cwd);        //opendir opens it's input directory and returns a directory pointer
    if(p==NULL)
    {
	perror("Cannot find");
	return 0;
    }
    
    while(d=readdir(p))    //keep reading files of directory until none are left
    {
	if(  ( strcmp(d->d_name,".")!=0 )  &&  ( strcmp(d->d_name,"..")!=0 )  )
	{
		if( strcmp(filename,d->d_name) == 0 )
		{
			char* temp=new char[400];

			strcpy(temp,cwd);
			strcat(temp,slash);
			strcat(temp,filename);
			strcpy(filename,temp);

			rename(filename, newNAME);
		}
	}
    }
    return 1;
}
    

int main(int argc, char* argv[]) 
{
    if(argc!=4)
    {
	cout<<"Insuffecient arguments\n";
	return 0;
    }
    /*if( !is_file(argv[1]) )
    {
	cout<<"File not found";
	return 0;
    }*/
    rename(argv[1],argv[2],argv[3]);
    
    return 0;
}
