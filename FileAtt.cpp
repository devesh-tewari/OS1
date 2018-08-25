/*  #include <sys/stat.h>
    
     * Get the size of a file.
     * @return The filesize, or 0 if the file does not exist.
     
    size_t getFilesize(const char* filename) {
        struct stat st;
        if(stat(filename, &st) != 0) {
            return 0;
        }
        return st.st_size;   
    }

size_t getFilesize(const std::string& filename) {
    struct stat st;
    if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;   
}
*/

//https://www.youtube.com/watch?v=qGcbxORzWFk
//https://www.google.com/search?client=ubuntu&hs=lCV&channel=fs&biw=1405&bih=707&tbm=isch&sa=1&ei=w1d9W6TeBNqR9QO9o5PIAw&q=st_mode+bits&oq=st_mode+bits&gs_l=img.3...214740.216489.0.216695.5.5.0.0.0.0.186.683.0j4.4.0....0...1c.1.64.img..1.1.182...0i8i30k1j0i24k1.0.w9K7SBLnEKs#imgrc=ldZZW1aczCpXOM:

/*
#include <pwd.h>
#include <grp.h>

struct group *grp;
struct passwd *pwd;

grp = getgrgid(gid);
printf("group: %s\n", grp->gr_name);

pwd = getpwuid(uid);
printf("username: %s\n", pwd->pw_name);
*/

/*
int main (int argc, char **argv)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);
    return 0;  // make sure your main returns int
}*/

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

void insert( struct FileAttributes FA )           //inserts the file attributes in a doubly linked list data structure
{
	struct FileAttributes* p=new struct FileAttributes;
	*p=FA;
	if(first_file == NULL && last_file==NULL)
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
		

/*int display(struct FileAttributes* FileAtt)   // make dir size print in 2nd line
{
	int dir_size;
	cout.precision(1);
	cout.setf(ios::fixed);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int rows=w.ws_row-3;
	int columns=w.ws_col;
	int cur=0;
	//cout<<rows<<" ";
	while(FileAtt!=NULL && cur<rows)
	{
		if(cur==0)
		{
			cout << "\033[7;36m";
		}
		cout<<setw(18)<<FileAtt->file_name;
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
		cout<<setw(7)<<FileAtt->owner_user;
		cout<<setw(7)<<FileAtt->owner_group;
		cout<<setw(12)<<FileAtt->file_permissions;
		cout<<setw(27)<<FileAtt->last_modified;
		FileAtt=FileAtt->next_file;
		cout << "\033[0m";
		cur++;
	}
	return dir_size;
}*/

struct FileAttributes getFileAttributes(char* file_name)      //this function returns the attributes of a file
{
	struct stat s;
	/*if ( stat(file_name,&s) != 0 )                    //if the file does not exist
	{
		perror("Cannot open this file!");
	}*/
	//printf("%s\n",cwd);
	//strcpy(f,str.c_str());
	//printf("\n\n\n\n\n\n\n\n%s",file_name);
	if ( stat(file_name,&s) != 0 )                    //if the file does not exist
	{	
		perror("Cannot open this file!");
		printf("\n\n\n\n\n\n\n\n\n%s",file_name);
	}
	struct FileAttributes File_Attributes;
	File_Attributes.file_name=file_name;
	File_Attributes.k=false;
	File_Attributes.m=false;
	float size=s.st_size;
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

	grp = getgrgid(s.st_gid);
	File_Attributes.owner_group = grp->gr_name;

	pwd = getpwuid(s.st_uid);
	File_Attributes.owner_user = pwd->pw_name;

	(S_ISDIR(s.st_mode) ) ? File_Attributes.file_permissions[0]='d':File_Attributes.file_permissions[0]='-';    //here the bits of permissions stored in st_mode are changed to corresponding character
    	(s.st_mode & S_IRUSR) ? File_Attributes.file_permissions[1]='r' : File_Attributes.file_permissions[1]='-';
    	(s.st_mode & S_IWUSR) ? File_Attributes.file_permissions[2]='w' : File_Attributes.file_permissions[2]='-';
    	(s.st_mode & S_IXUSR) ? File_Attributes.file_permissions[3]='x' : File_Attributes.file_permissions[3]='-';
    	(s.st_mode & S_IRGRP) ? File_Attributes.file_permissions[4]='r' : File_Attributes.file_permissions[4]='-';
    	(s.st_mode & S_IWGRP) ? File_Attributes.file_permissions[5]='w' : File_Attributes.file_permissions[5]='-';
	(s.st_mode & S_IXGRP) ? File_Attributes.file_permissions[6]='x' : File_Attributes.file_permissions[6]='-';
	(s.st_mode & S_IROTH) ? File_Attributes.file_permissions[7]='r' : File_Attributes.file_permissions[7]='-';
	(s.st_mode & S_IWOTH) ? File_Attributes.file_permissions[8]='w' : File_Attributes.file_permissions[8]='-';
	(s.st_mode & S_IXOTH) ? File_Attributes.file_permissions[9]='x' : File_Attributes.file_permissions[9]='-';
	File_Attributes.file_permissions[10]='\0';
	
	File_Attributes.last_modified = ctime( &s.st_mtime );
	return File_Attributes;
}





