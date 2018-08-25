#include<stdio.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

#define max_history 100
#include "FileAtt.h"
#include "display.h"

int open_dir(char*);  //this function navigates through the input directory and stores all its files/subdirectories, andn returns an integer specifying the number of files in the input directory

/*int file_no_at_term_top=0;
int file_no_at_term_bottom;
void refresh(int cur)
{
	cout.precision(1);
	cout.setf(ios::fixed);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	file_no_at_term_bottom= file_no_at_term_top + w.ws_row-3;   // -3 because 1st line is occupied & last line is for status and one is extra xD
	int columns=w.ws_col;
	int i=0,j=0;
	//cout<<cur<<" ";
	struct FileAttributes* FileAtt=first_file;
	if(cur>=file_no_at_term_bottom)
	{
		file_no_at_term_top++;
		file_no_at_term_bottom++;
	}
	else if(cur<file_no_at_term_top)
	{
		file_no_at_term_bottom--;
		file_no_at_term_top--;
	}
	//cout<<rows<<" ";
	while(FileAtt!=NULL)
	{
		if(i>=file_no_at_term_top && i<file_no_at_term_bottom)
		{
			if(cur==i)
			{
				cout << "\033[7;36m";
			}
			cout<<setw(18)<<FileAtt->file_name;
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
			cout<<setw(7)<<FileAtt->owner_user;
			cout<<setw(7)<<FileAtt->owner_group;
			cout<<setw(12)<<FileAtt->file_permissions;
			cout<<setw(27)<<FileAtt->last_modified;
			
			cout << "\033[0m";
		}
		FileAtt=FileAtt->next_file;
		i++;
	}
}*/

bool is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

void navigate(char* stack, int files)   //cwd is an array which stores browsing history, files are the no of files called by main
{
    char c;
    char cwd[max_history][PATH_MAX];   //this stores browing history in a stack structure
    char path[PATH_MAX];
    strcpy(cwd[0],stack);
    static struct termios oldtio, newtio;
    tcgetattr(0, &oldtio);       //taking terminal attributes to the structure 'oldtio'
    newtio = oldtio;
    newtio.c_lflag &= ~ICANON;   //this turns on non-canonical input
    newtio.c_lflag &= ~ECHO;     //so that the keystrokes are not displayed
    tcsetattr(0, TCSANOW, &newtio);   //this sets the changed attributes
    struct FileAttributes* curr_file=first_file;
    char slash[2];
    int pid;
    strcpy(slash,"/");
    char cd[PATH_MAX];
    int i=0;     //current stack position
    int top=0;   //stack top of history
    int curr_file_no=1;
    bool first_down=true;
    fflush(stdout);
    while (1)   //keep reading characters in insert mode till we read ':'
    {
        read(0, &c, 1);
	//printf("%c", c);
	//if(c!='A' || c!='B')
	//	printf("\b");
	switch(c)
	{
/*enter*/	case '\n':   //next few lines appends the directory name to the current directory path
			   strcpy(path,cwd[i]);
			   strcat(path,slash);
			   strcat(path,curr_file->file_name.c_str());
			   if(is_file(path))
			   {
				pid = fork();
				if (pid == 0) 
				{
				  execl("/usr/bin/xdg-open", "xdg-open", path, (char *)0);
				  exit(1);
				}
				break;
			   }
			   chdir(path);   //go to the directory where enter is pressed
			   printf("\033[2J");     //clear screen
			   printf("\033[H"); // move cursor to top
			   getcwd(cd, sizeof(cd));
			   printf("%s\n",cd);
			   strcpy(cwd[++i],cd);  //store the path in stack
			   top=i;
			   files=open_dir(path);
			   curr_file=first_file;
			   curr_file_no=1;
			   first_down=true;
			   break;

/*up*/		case 'A':  if(curr_file_no>1) //this condition ensures that curr_file does not point something invalid
			   {
				printf("\033[A");
				curr_file=curr_file->prev_file;
				curr_file_no--;
				printf("\033[2J");     //clear screen
			   	printf("\033[H"); // move cursor to top
				getcwd(cd, sizeof(cd));
			   	printf("%s\n",cd);
				refresh(curr_file_no-1 , files, false);
				//printf("up ");
			   }
			   break;

/*down*/	case 'B':  if(curr_file_no<files)
			   {
				printf("\033[B");
				curr_file=curr_file->next_file;
				curr_file_no++;
				printf("\033[2J");     //clear screen
			   	printf("\033[H"); // move cursor to top
				getcwd(cd, sizeof(cd));
			   	printf("%s\n",cd);
				if(first_down)
					refresh(curr_file_no-1 , files, true);
				else
					refresh(curr_file_no-1 , files, false);
				first_down=false;
				//printf("down ");
			   }
			   break;

/*left*/	case 'D':  if(i<=top && i>0)
			   {
				i--;
				strcpy(path,cwd[i]);
				chdir(path);
				printf("\033[2J");     //clear screen
			   	printf("\033[H"); // move cursor to top
				cout<<cwd[i]<<endl;
				files=open_dir(path);
			   	curr_file=first_file;
			   	curr_file_no=1;
				first_down=true;
			   }
			   break;

/*right*/	case 'C':  if(i<top && i>=0)
			   {
				i++;
				strcpy(path,cwd[i]);
				chdir(path);
				printf("\033[2J");     //clear screen
			   	printf("\033[H");      // move cursor to top
				cout<<cwd[i]<<endl;
				files=open_dir(path);
			   	curr_file=first_file;
			   	curr_file_no=1;
				first_down=true;
			   }
			   break;

/*back*/	case 127:  if(!strcmp(cwd[i],cwd[0]))
				break;
			   //if(i==top)
			   //	{top++;}
			   //i++;
			   i++;
			   top=i;
			   chdir("..");
			   getcwd(path, sizeof(path));
			   strcpy(cwd[i],path);
			   printf("\033[2J");     //clear screen
			   printf("\033[H");      // move cursor to top
			   cout<<cwd[i]<<endl;
			   files=open_dir(path);
			   curr_file=first_file;
			   curr_file_no=1;
			   first_down=true;
			   break;

/*home*/	case 'h':  if(!strcmp(cwd[i],cwd[0]))
				break;
			   if(i==top)
				{top++;}
			   i++;
			   chdir(cwd[0]);
			   getcwd(path, sizeof(path));
			   strcpy(cwd[i],path);
			   printf("\033[2J");     //clear screen
			   printf("\033[H");      // move cursor to top
			   cout<<cwd[i]<<endl;
			   files=open_dir(path);
			   curr_file=first_file;
			   curr_file_no=1;
			   first_down=true;
			   break;
    	}
	fflush(stdout);
        if (c == ':') { break; }
    }
    printf("\n"); 
    tcsetattr(0, TCSANOW, &oldtio);         //go back to canonical mode

}


int open_dir(char* dir)
{
	DIR* p;
	struct dirent *d;
	first_file=NULL;
	last_file=NULL;
	p=opendir(dir);        //opendir opens it's input directory and returns a directory pointer
	if(p==NULL)
	{
		perror("Cannot find directory");
		return 0;
	}
	int dir_size=0;
	int num_files=0;
	struct FileAttributes File_Attributes;
	struct FileAttributes* FileAtt;
	while(d=readdir(p))    //keep reading files of directory until none are left
	{
		File_Attributes=getFileAttributes(d->d_name);
		insert( File_Attributes );    //this function inserts the file attributes in a doubly linked list
		num_files++;
	}
	dir_size=display(first_file);
	//cout<<"...";
	//cout<<"Directory size: "<<dir_size<<"B";
	printf("\033[H"); // move cursor to top
	//printf("\033[B");
	return num_files;
}
