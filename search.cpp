#include <sys/stat.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include<termios.h>

using namespace std;

bool is_dir(const char* path)  //returns true if input is a directory
{
   struct stat statbuf;
   bool isDir = false;
   if (stat(path, &statbuf) != -1) 
   {
	if (S_ISDIR(statbuf.st_mode)) 
	{
	   isDir = true;
	}
   }
   return isDir;
}


char slash[2]="/";
vector<string> matches;
vector<string> search(char* src, char* match)
{
    if(is_dir(src))  //if destination is a directory, recursively call the copy function
    {
	DIR* p;
	struct dirent *d;
	p=opendir(src);        //opendir opens it's input directory and returns a directory pointer
	if(p==NULL)
	{
		perror("Cannot find directory\n");
		return matches;
	}
	while(d=readdir(p))    //keep reading files of directory until none are left
	{
		if(  ( strcmp(d->d_name,".")!=0 )  &&  ( strcmp(d->d_name,"..")!=0 )  )
		{
			char file_inside[400];
			strcpy(file_inside,src);
			strcat(file_inside,slash);
			strcat(file_inside,d->d_name);

			if( strcmp(d->d_name,match) == 0 )
			{
				string temp=src;
				matches.push_back(temp);
			}
			
			search ( file_inside , match );   //recursive call to search
		}
	}
    }

    return matches;
}


void refresh(vector<string> matches, int file_selected , char* searched_file)
{
    cout<<"\033[2J";
    cout<<"\033[H";
    int size=matches.size();
    cout<<"Search results for "<<searched_file<<" : "<<endl;
    char c;
    int i=0;
    while(i<size)
    {
	if(i==file_selected)
		cout << "\033[7;36m";  //highlight first selected line
	cout<<matches[i]<<" : "<<searched_file<<endl;
	cout<<"\033[0m";
	i++;
    }
}

void display(vector<string> matches, int file_selected , char* searched_file)
{
    cout<<"\033[2J";
    cout<<"\033[H";
    int size=matches.size();
    cout<<"Search results for "<<searched_file<<" : "<<endl;
    char c;
    int i=0;
    while(i<size)
    {
	if(i==0)
		cout << "\033[7;36m";  //highlight first selected line
	cout<<matches[i]<<" : "<<searched_file<<endl;
	cout<<"\033[0m";
	i++;
    }
    while (1)   //keep reading characters in insert mode
    {
	read( 0 , &c , 1 );
	if(c=='A')
	{
		if(file_selected>0)
		{
			file_selected--;
			refresh( matches , file_selected , searched_file );
		}
	}
	else if(c=='B')
	{cout<<"here";
		if(file_selected<size-1)
		{
			file_selected++;
			refresh( matches , file_selected , searched_file );
		}
	}
	else if(c=='\n')
	{
		int pid = fork();
		if (pid == 0) 
		{	
			char* temp=new char[400];
			strcpy(temp, &matches[file_selected][0] );
			strcat(temp,slash);
			strcat(temp, searched_file);
			execl("/usr/bin/xdg-open" , "xdg-open" , temp , (char *)0);
			exit(1);
		}
	}
	else if(c==127 || c==8)
		return ;
    }
}
int main(int argc, char* argv[]) 
{
    static struct termios oldtio, newtio;
        tcgetattr(0, &oldtio);       //taking terminal attributes to the structure 'oldtio'
        newtio = oldtio;
        newtio.c_lflag &= ~ICANON;   //this turns on non-canonical input
        newtio.c_lflag &= ~ECHO;     //so that the keystrokes are not displayed
        tcsetattr(0, TCSANOW, &newtio);   //this sets the changed attributes



    if(argc!=3)
    {
	cout<<"Insuffecient arguments\n";
	return 0;
    }

    vector<string> v ( search(argv[2],argv[1]) );
    int size=v.size();
    if(size==0)
    {
	cout<<" No Matches Found!";
	return 0;
    }
    //sort(v.begin(),v.end());

    display(v,0,argv[1]);


	tcsetattr(0, TCSANOW, &oldtio);         //go back to canonical mode

    return 0;
}
