#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

#include "FileAtt.h"

int open_dir(char* dir)
{
	DIR* p;
	struct dirent *d;
	first_file=NULL;
	last_file=NULL;
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
		insert( File_Attributes );                          //
		num_files++;
	}
	dir_size=display(first_file);
	cout<<"Directory size: "<<dir_size<<"B\n";
	return num_files;
}

void navigate() 
{
    char c; 
    static struct termios oldtio, newtio;
    tcgetattr(0, &oldtio);       //taking terminal attributes to the structure 'oldtio'
    newtio = oldtio;
    newtio.c_lflag &= ~ICANON;   //this turns on non-canonical input
    newtio.c_lflag &= ~ECHO;     //so that the keystrokes are not displayed
    tcsetattr(0, TCSANOW, &newtio);   //this sets the changed attributes
    struct FileAttributes* curr_file=first_file;
    fflush(stdout);
    while (1) 
    {
        read(0, &c, 1);
        printf("%c", c);
	switch(c)
	{
		case '\n': //string s=curr_file->file_name;
			   char f[30];
			   strcpy(f,curr_file->file_name.c_str());
			   printf("%s\n",f);
			   open_dir(f);
			   break;
		case 'A':  if(curr_file->prev_file!=NULL);
			   {
				curr_file=curr_file->prev_file;
				printf("%c", c);
			   }
			   break;
		case 'B':  if(curr_file->next_file!=NULL);
			   {
				curr_file=curr_file->next_file;
				printf("%c", c);
			   }
			   break;
		case 'C':  printf("right");
			   break;
		case 'D':  printf("  left");
			   break;
    	}
	fflush(stdout);
        if (c == 'q') { break; }
    }
    printf("\n"); 
    tcsetattr(0, TCSANOW, &oldtio);         //go back to canonical mode

}
