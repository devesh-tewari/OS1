#include<sys/ioctl.h>
#include<unistd.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "display.h"
#include "FileAtt.h"
using namespace std;

char fname[100];

//display is called once every time we change directory
int display(struct FileAttributes* FileAtt)   // make dir size print in 2nd line
{
	int dir_size;
	cout.precision(1);
	cout.setf(ios::fixed);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int rows=w.ws_row-3;  // -3 because 1st line is occupied & last line is for status and one is extra xD
	int cur=0;
	//cout<<rows<<" ";
	while(FileAtt!=NULL && cur<rows)
	{
		if(cur==0)
		{
			cout << "\033[7;36m";  //highlight first selected line
		}
		strcpy(fname,FileAtt->file_name.c_str());
		if(strlen(FileAtt->file_name.c_str())>18)
		{
			fname[16]='.';
			fname[17]='.';
			fname[18]='\0';
		}
		cout<<setw(18)<<fname;
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
		cout<<setw(8)<<FileAtt->owner_user;
		cout<<setw(8)<<FileAtt->owner_group;
		cout<<setw(12)<<FileAtt->file_permissions;
		cout<<setw(27)<<FileAtt->last_modified;
		FileAtt=FileAtt->next_file;
		cout << "\033[0m";
		cur++;
	}
	return dir_size;
}

int file_no_at_term_top=0;
int file_no_at_term_bottom;
void refresh(int cur,int no_of_files,bool first_down)      //this function refreshes screen everytime up or down is pressed and handles overflow
{
	if(first_down)   //when a new file is open we have to reset, term_top. so we do this first time we press down
	{
		file_no_at_term_top=0;
	}
	cout.precision(1);
	cout.setf(ios::fixed);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);  //function to get terminal size
	file_no_at_term_bottom= file_no_at_term_top + w.ws_row-3;   // -3 because 1st line is occupied & last line is for status and one is extra xD
	int i=0,j=0;
	struct FileAttributes* FileAtt=first_file;
	if( no_of_files > w.ws_row-3 )
	{
		if(cur>=file_no_at_term_bottom)    //if down arrow key leads to overflow
		{
			//cout<<"down ";
			file_no_at_term_top++;
			file_no_at_term_bottom++;
		}
		else if( cur<file_no_at_term_top)   //if up arrow key leads to overflow
		{
			//cout<<"up ";
			file_no_at_term_bottom--;
			file_no_at_term_top--;
		}
	}
	while(FileAtt!=NULL)
	{
		if(i>=file_no_at_term_top && i<file_no_at_term_bottom)
		{
			if(cur==i)
			{
				cout << "\033[7;36m";   //highlight selected file
			}
			strcpy(fname,FileAtt->file_name.c_str());
			if(strlen(FileAtt->file_name.c_str())>18)
			{
				fname[16]='.';
				fname[17]='.';
				fname[18]='\0';
			}
			cout<<setw(18)<<fname;
			if(FileAtt->k==true)
			{
				cout<<setw(7)<<FileAtt->file_size<<"K";
				//dir_size+=FileAtt->file_size * 1024;
			}
			else if(FileAtt->m==true)
			{
				cout<<setw(7)<<FileAtt->file_size<<"M";
				//dir_size+=FileAtt->file_size * 1024 * 1024;
			}
			else
			{
				cout<<setw(7)<<(int)FileAtt->file_size<<"B";
				//dir_size+=FileAtt->file_size;
			}
			cout<<setw(8)<<FileAtt->owner_user;
			cout<<setw(8)<<FileAtt->owner_group;
			cout<<setw(12)<<FileAtt->file_permissions;
			cout<<setw(27)<<FileAtt->last_modified;
			
			cout << "\033[0m";
		}
		FileAtt=FileAtt->next_file;
		i++;
	}
}
