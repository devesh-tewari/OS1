#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <string>
#include <pwd.h>
#include <grp.h>
using namespace std;

struct FileAttributes
{
	string file_name;
	int    file_size;
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
	int size=s.st_size;
	if(size/1024!=0)        //this checks if the size is in bytes, kilobytes, or megabytes
	{
		size=size/1024;
		FileAtt->k=true;
		if(size/1024!=0)
		{
			size=size/1024;
			FileAtt->m=true;
			FileAtt->k=false;
		}
	}
	FileAtt->file_size=size;
	
	struct group *grp;
	struct passwd *pwd;

	grp = getgrgid(s.st_gid);              //this function converts groupID to group name
	FileAtt->owner_user = grp->gr_name;

	pwd = getpwuid(s.st_uid);              //this function converts userID to user name
	FileAtt->owner_group = pwd->pw_name;

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


