#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <pwd.h>
#include <grp.h>
using namespace std;

struct FileAttributes
{
	string file_name;
	float  file_size;
	bool   k,m;
	string owner_user;
	string owner_group;
	char   file_permissions[11];
	string last_modified;
}File_Attributes;

struct FileAttributes* FileAtt=&File_Attributes;


struct FileAttributes* getFileAttributes(char* file_name)      //this function returns the attributes of a file
{
	struct stat s;
	if ( stat(file_name,&s) != 0 )                    //if the file does not exist
		return NULL;

	//struct FileAttributes* FileAtt;	
	
	FileAtt->k=false;
	FileAtt->m=false;
	float size=s.st_size;
	if(size/1024.0>1.0)        //this checks if the size is in bytes, kilobytes, or megabytes
	{
		size=size/1024.0;
		FileAtt->k=true;
		if(size/1024.0>1.0)
		{
			size=size/1024.0;
			FileAtt->m=true;
			FileAtt->k=false;
		}
	}
	FileAtt->file_size=size;
	
	struct group *grp;
	struct passwd *pwd;

	grp = getgrgid(s.st_gid);
	FileAtt->owner_group = grp->gr_name;

	pwd = getpwuid(s.st_uid);
	FileAtt->owner_user = pwd->pw_name;

	(S_ISDIR(s.st_mode)) ? FileAtt->file_permissions[0]='d' : FileAtt->file_permissions[0]='-';    //here the bits of permissions stored in st_mode are changed to corresponding character
    	(s.st_mode & S_IRUSR) ? FileAtt->file_permissions[1]='r' : FileAtt->file_permissions[1]='-';
    	(s.st_mode & S_IWUSR) ? FileAtt->file_permissions[2]='w' : FileAtt->file_permissions[2]='-';
    	(s.st_mode & S_IXUSR) ? FileAtt->file_permissions[3]='x' : FileAtt->file_permissions[3]='-';
    	(s.st_mode & S_IRGRP) ? FileAtt->file_permissions[4]='r' : FileAtt->file_permissions[4]='-';
    	(s.st_mode & S_IWGRP) ? FileAtt->file_permissions[5]='w' : FileAtt->file_permissions[5]='-';
	(s.st_mode & S_IXGRP) ? FileAtt->file_permissions[6]='x' : FileAtt->file_permissions[6]='-';
	(s.st_mode & S_IROTH) ? FileAtt->file_permissions[7]='r' : FileAtt->file_permissions[7]='-';
	(s.st_mode & S_IWOTH) ? FileAtt->file_permissions[8]='w' : FileAtt->file_permissions[8]='-';
	(s.st_mode & S_IXOTH) ? FileAtt->file_permissions[9]='x' : FileAtt->file_permissions[9]='-';
	FileAtt->file_permissions[10]='\0';
	
	FileAtt->last_modified = ctime( &s.st_mtime );
	return FileAtt;
}



int main()
{
	//char dirname[30];
	DIR* p;
	struct dirent *d;
	//printf("Enter directory name\n");
	//scanf("%s",dirname);
	p=opendir(".");
	if(p==NULL)
	{
		perror("Cannot find directory");
		return 0;
	}
	//struct FileAttributes* fAttr;
	int dir_size=0;
	cout.precision(1);
	cout.setf(ios::fixed);
	while(d=readdir(p))
	{
		FileAtt=getFileAttributes(d->d_name);
		cout<<setw(20)<<d->d_name;
		if(FileAtt->k==true)
		{
			cout<<setw(7)<<FileAtt->file_size<<"K";
			dir_size+=FileAtt->file_size * 1024;
		}
		else if(FileAtt->m==true)
		{
			cout<<setw(7)<<FileAtt->file_size<<"M";
			dir_size+=FileAtt->file_size * 1024 * 1024;
		}
		else
		{
			cout<<setw(7)<<(int)FileAtt->file_size<<"B";
			dir_size+=FileAtt->file_size;
		}
		cout<<setw(10)<<FileAtt->owner_user;
		cout<<setw(10)<<FileAtt->owner_group;
		cout<<setw(13)<<FileAtt->file_permissions;
		cout<<setw(30)<<FileAtt->last_modified;
		//cout<<"\n";
	}
	cout<<"Directory size: "<<dir_size<<" B\n";
	return 0;
}
