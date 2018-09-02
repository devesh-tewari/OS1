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
	char command[1000];
	int pos=0;
	char c;
	
	char H[PATH_MAX];  //used to store current(home) directory
	char* cur_dir=H;
	getcwd(H, sizeof(H));    //get current working directory
	bool goto_flag = false;

	cur_dir = navigate( cur_dir , H , goto_flag );  //navigate function handles file explorer input(keystrokes) and returns true when ':' is pressed
	//cout<<cur_dir;
	bool clear_command_mode=true;
	
	fflush(stdout);
	while(1)
	{	
		command[pos]='\0';
		c=getchar();
		if(clear_command_mode)
		{
			printf("%c[2K\033[200D:", 27);
			clear_command_mode=false;
		}
		switch(c)
		{
			case '\n':  if(command!=NULL  && pos>0)
				    	goto_flag  =  execute_command(  command  ,  H  ,  cur_dir  );
				    pos=0;
				    printf("%c[2K\033[200D:", 27);
				    break;

			case 127 :  printf("\033[D");
				    printf(" ");
				    printf("\033[D");
				    if(pos>0)
					pos--;
				    else
					printf(":");
				    break;

			/*case 'A' :  break;
			case 'B' :  break;
			case 'C' :  break;
			case 'D' :  break;*/

			case '\033':if( c=='A' || c=='B' || c=='C' || c=='D' )
				    	break;
				    cur_dir = navigate( cur_dir , H , goto_flag );
				    clear_command_mode=true;
				    pos=0;
				    break;

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
