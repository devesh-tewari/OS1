#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

#include "FileAtt.h"
#include "navigate.h"

using namespace std;
struct FileAttributes* first_file=NULL;
struct FileAttributes* last_file=NULL;
/*
struct FileAttributes* first_file=NULL;
struct FileAttributes* last_file=NULL;

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
		

int display(struct FileAttributes* FileAtt)
{
	int dir_size;
	cout.precision(1);
	cout.setf(ios::fixed);
	while(FileAtt!=NULL)
	{
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
	}
	return dir_size;
}
*/

/*int open_dir(char* dir)
{
	DIR* p;
	struct dirent *d;
	p=opendir(dir);
	if(p==NULL)
	{
		perror("Cannot find directory");
		return 0;
	}
	int dir_size=0;
	int num_files=0;
	struct FileAttributes File_Attributes;
	struct FileAttributes* FileAtt;
	while(d=readdir(p))
	{
		File_Attributes=getFileAttributes(d->d_name);
		insert( File_Attributes );                          
		num_files++;
	}
	dir_size=display(first_file);
	cout<<"Directory size: "<<dir_size<<"B\n";
	return num_files;
}*/

int main()
{
	char dir[2]=".";
	char stack[PATH_MAX];   //used to store browse history
	if (getcwd(stack, sizeof(stack)) == NULL)    //get current working directory
	{
    		perror("getcwd() error");
	}
	printf("\033[2J");  //clear screen
	printf("\033[H"); // move cursor to top
	printf("%s\n",stack);
	int files=open_dir(dir); 
	navigate(stack, files);
	printf("\033[2J");     //clear screen
	printf("\033[H"); // move cursor to top
	return 0;
}
