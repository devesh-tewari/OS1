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
	switch(c)
	{
/*enter*/	case '\n':   //next few lines appends the directory name to the current directory path
			   strcpy(path,cwd[i]);
			   strcat(path,slash);
			   strcat(path,curr_file->file_name.c_str());
			   if(is_file(path))   //if it's a file, open it in other process
			   {
				/*pid = fork();
				if (pid == 0) 
				{
				  execl("/usr/bin/xdg-open", "xdg-open", path, (char *)0);
				  exit(1);
				}*/
				execlp("/usr/bin/gedit","gedit",curr_file->file_name,NULL);
				break;
			   }
			   chdir(path);   //go to the directory where enter is pressed
			   printf("\033[2J");     //clear screen
			   printf("\033[H"); // move cursor to top
			   getcwd(cd, sizeof(cd));
			   printf("\033[1;36m%s\033[0m\n",cd);
			   strcpy(cwd[++i],cd);  //store the path in stack
			   top=i;
			   files=open_dir(path);
			   curr_file=first_file;  //reset curr_file pointer
			   curr_file_no=1;        //reset current file number
			   first_down=true;
			   break;

/*up*/		case 'A':  if(curr_file_no>1) //this condition ensures that curr_file does not point something invalid
			   {
				curr_file=curr_file->prev_file;  //point to previous file
				curr_file_no--;
				printf("\033[2J");     //clear screen
			   	printf("\033[H"); // move cursor to top
				getcwd(cd, sizeof(cd));
			   	printf("\033[1;36m%s\033[0m\n",cd);
				refresh(curr_file_no-1 , files, false);
			   }
			   break;

/*down*/	case 'B':  if(curr_file_no<files)
			   {
				printf("\033[B");
				curr_file=curr_file->next_file;  //point to next file
				curr_file_no++;
				printf("\033[2J");     //clear screen
			   	printf("\033[H");      //move cursor to top
				getcwd(cd, sizeof(cd));
			   	printf("\033[1;36m%s\033[0m\n",cd);
				refresh(curr_file_no-1 , files, first_down);
				first_down=false;
			   }
			   break;

/*left*/	case 'D':  if(i<=top && i>0)
			   {
				i--;
				strcpy(path,cwd[i]);
				chdir(path);
				printf("\033[2J");     //clear screen
			   	printf("\033[H"); // move cursor to top
				cout<<"\033[1;36m"<<cwd[i]<<"\033[0m"<<endl;
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
				cout<<"\033[1;36m"<<cwd[i]<<"\033[0m"<<endl;
				files=open_dir(path);
			   	curr_file=first_file;
			   	curr_file_no=1;
				first_down=true;
			   }
			   break;

/*back*/	case 127:  if(!strcmp(cwd[i],cwd[0]))
				break;
			   i++;
			   top=i;
			   chdir("..");
			   getcwd(path, sizeof(path));
			   strcpy(cwd[i],path);
			   printf("\033[2J");     //clear screen
			   printf("\033[H");      // move cursor to top
			   cout<<"\033[1;36m"<<cwd[i]<<"\033[0m"<<endl;
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
			   cout<<"\033[1;36m"<<cwd[i]<<"\033[0m"<<endl;
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
	first_file=NULL;  //delete the old data structure for prev. directory
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
	return num_files;
}
