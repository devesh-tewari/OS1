#include <stdio.h>
#include <dirent.h>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

#include "FileAtt.h"
#include "navigate.h"
#include "myCommands.h"

using namespace std;
struct FileAttributes* first_file=NULL;
struct FileAttributes* last_file=NULL;


int main()
{
	char dir[2]=".";
	printf("\033[2J");  //clear screen
	printf("\033[H");   // move cursor to top
	//int files=open_dir(dir);
	static struct termios oldtio, newtio;
        tcgetattr(0, &oldtio);       //taking terminal attributes to the structure 'oldtio'
        newtio = oldtio;
        newtio.c_lflag &= ~ICANON;   //this turns on non-canonical input
        newtio.c_lflag &= ~ECHO;     //so that the keystrokes are not displayed
        tcsetattr(0, TCSANOW, &newtio);   //this sets the changed attributes
	bool enter_command_mode;
	char command[400];
	int pos=0;
	char c;
	
	char H[PATH_MAX];  //used to store current(home) directory
	char* cur_dir=H;
	getcwd(H, sizeof(H));    //get current working directory

	cur_dir = navigate( cur_dir , H );  //navigate function handles file explorer input(keystrokes) and returns true when ':' is pressed
	cout<<cur_dir;
	fflush(stdout);
	while(1)
	{	
		command[pos]='\0';
		c=getchar();
		switch(c)
		{
			case 27  :  cur_dir = navigate( cur_dir , H );
				    pos=0;
				    break;

			case '\n':  //execute_command( command );
				    pos=0;
				    break;

			//case '\b'

			default  :  command[pos]=c;
				    pos++;
				    putchar(c);
				    break;
		}
	}
	tcsetattr(0, TCSANOW, &oldtio);         //go back to canonical mode
	printf("\033[2J");     //clear screen
	printf("\033[H"); // move cursor to top
	return 0;
}
