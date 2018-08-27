#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <string>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include "FileAtt.h"
#include "display.h"
using namespace std;

void insert( struct FileAttributes FA )     //inserts the file attributes in a doubly linked list data structure
{
	struct FileAttributes* p=new struct FileAttributes;
	*p=FA;
	if(first_file == NULL && last_file==NULL)  //if linked list is empty(it will be empty every time we change directory)
	{	
		p->next_file=NULL;
		p->prev_file=NULL;
		first_file=p;
		last_file=p;
	}
	else
	{
		last_file->next_file=p;
		p->prev_file=last_file;
		last_file=p;
		p->next_file=NULL;
	}
}


struct FileAttributes getFileAttributes(char* file_name)      //this function returns the attributes of a file
{
	struct stat s;
	if ( stat(file_name,&s) != 0 )                    //if the file does not exist
	{	
		perror("Cannot open this file!");
		printf("\n%s\n",file_name);
	}
	struct FileAttributes File_Attributes;
	File_Attributes.file_name=file_name;
	File_Attributes.k=false;
	File_Attributes.m=false;
	float size=s.st_size;      //get file size
	if(size/1024.0>1.0)        //this checks if the size is in bytes, kilobytes, or megabytes
	{
		size=size/1024.0;
		File_Attributes.k=true;
		if(size/1024.0>1.0)
		{
			size=size/1024.0;
			File_Attributes.m=true;
			File_Attributes.k=false;
		}
	}
	File_Attributes.file_size=size;
	
	struct group *grp;
	struct passwd *pwd;

	grp = getgrgid(s.st_gid);    // get file group
	File_Attributes.owner_group = grp->gr_name;

	pwd = getpwuid(s.st_uid);    // get file owner
	File_Attributes.owner_user = pwd->pw_name;

	(S_ISDIR(s.st_mode) ) ? File_Attributes.file_permissions[0]='d':File_Attributes.file_permissions[0]='-';//check if directory? 
    	(s.st_mode & S_IRUSR) ? File_Attributes.file_permissions[1]='r' : File_Attributes.file_permissions[1]='-';//here the bits of permissions stored in st_mode are changed to corresponding character
    	(s.st_mode & S_IWUSR) ? File_Attributes.file_permissions[2]='w' : File_Attributes.file_permissions[2]='-';
    	(s.st_mode & S_IXUSR) ? File_Attributes.file_permissions[3]='x' : File_Attributes.file_permissions[3]='-';
    	(s.st_mode & S_IRGRP) ? File_Attributes.file_permissions[4]='r' : File_Attributes.file_permissions[4]='-';
    	(s.st_mode & S_IWGRP) ? File_Attributes.file_permissions[5]='w' : File_Attributes.file_permissions[5]='-';
	(s.st_mode & S_IXGRP) ? File_Attributes.file_permissions[6]='x' : File_Attributes.file_permissions[6]='-';
	(s.st_mode & S_IROTH) ? File_Attributes.file_permissions[7]='r' : File_Attributes.file_permissions[7]='-';
	(s.st_mode & S_IWOTH) ? File_Attributes.file_permissions[8]='w' : File_Attributes.file_permissions[8]='-';
	(s.st_mode & S_IXOTH) ? File_Attributes.file_permissions[9]='x' : File_Attributes.file_permissions[9]='-';
	File_Attributes.file_permissions[10]='\0';
	
	File_Attributes.last_modified = ctime( &s.st_mtime );  //get last modified timestamp
	return File_Attributes;
}





