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
    fflush(stdout);
    while (1)   //keep reading characters in insert mode till we read ':'
    {
	//disable echo
        read(0, &c, 1);
	if(c=='A' && file_selected>0)
	{
		file_selected--;
		refresh( matches , file_selected , searched_file );
	}
	else if(c=='B' && file_selected<size-1)
	{
		file_selected++;
		refresh( matches , file_selected , searched_file );
	}
	else if(c=='\n')
	{
		/*pid = fork();
		if (pid == 0) 
		{
			execl("/usr/bin/xdg-open", "xdg-open", path, (char *)0);
			exit(1);
		}*/
	}
	//else if(c=='\033')
	//	return ;
	fflush(stdout);
    }
}
int main(int argc, char* argv[]) 
{
    if(argc!=2)
    {
	cout<<"Insuffecient arguments\n";
	return 0;
    }

    vector<string> v ( search(".",argv[1]) );
    int size=v.size();
    if(size==0)
    {
	cout<<"No Matches Found!";
	return 0;
    }
    //sort(v.begin(),v.end());

    display(v,0,argv[1]);

    return 0;
}
