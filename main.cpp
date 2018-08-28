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

int main()
{
	char dir[2]=".";
	char stack[PATH_MAX];   //used to store current(home) directory
	if (getcwd(stack, sizeof(stack)) == NULL)    //get current working directory
	{
    		perror("getcwd() error");
	}
	printf("\033[2J");  //clear screen
	printf("\033[H"); // move cursor to top
	printf("\033[1;36m%s\033[0m\n",stack);  //print current directory at first line
	int files=open_dir(dir); 
	navigate(stack, files);
	printf("\033[2J");     //clear screen
	printf("\033[H"); // move cursor to top
	return 0;
}
