#include <stdio.h>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;
	
void deleteDir(char*);
void delete_entry(char*, char*);

bool execute_command(char* command, char* home , char* cwd )
{
	char* token=new char[400];
	token = strtok (command," ");
	vector<char*> tokens;
	tokens.push_back(token);
	int i=0;
	while (token != NULL)
	{	
		//cout<<tokens[i++];
		token = strtok(NULL," ");
		tokens.push_back( token );
	}
	int size = tokens.size();
	const char **argv = new const char*[size+2];   // extra room for program name, null
    	argv [0] = tokens[0];         // by convention, argv[0] is program name
    	for (int j = 1;j < size;++j)     // copy args
        {	
		argv[j] = tokens[j];
		//cout<<argv[j]<<endl;
	}
	
	if(size>0)
        	argv [size-1] = NULL;

	i=0;
	while(argv[i])
	{
		if( tokens[i][0]=='~' )
		{
			//here;
			string t=tokens[i];
			t.replace(0,1,home);
			strcpy( (char*)argv[i] , t.c_str() );
		}
		else if( strcmp( tokens[i],"." ) == 0 )
		{
			strcpy( (char*)argv[i] , cwd);
		}
		i++;
	}

	//cout<<size<<endl;
	int pid;   //process ID
	int status=1;
	char* executable_path=new char[400];  //path where the executable command files are stored
	char slash[2] = "/";

	strcpy( executable_path , home );
	strcat( executable_path , slash );
	strcat( executable_path , tokens[0] );

	bool goto_flag=false;

	char RecycleBin_path[400];
	strcpy( RecycleBin_path , home );
	strcat( RecycleBin_path , "/RecycleBin" );

	char recyclePath[400];
	strcpy(recyclePath,RecycleBin_path);
	strcat(recyclePath , "/recyclePath" );

	bool in_bin =false;
	if( strcmp(cwd,RecycleBin_path) == 0 )
		in_bin=true;

	if( strcmp(tokens[0],"copy") == 0 )
	{
		char* temp = new char[400];
		strcpy(temp,cwd);
		argv[size-1]=temp;
		argv[size]=NULL;
		pid=fork();
		if(pid==0)
		{	
			for(i=1;i<size-2;i++)
			{
				status = execv( executable_path , (char**)argv );
			}
			if(status == -1)
				cout<<" error!";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"move") == 0 )
	{
		char* temp = new char[400];
		strcpy(temp,cwd);
		argv[size-1]=temp;
		argv[size]=NULL;
		pid=fork();
		if(pid==0)
		{
			for(i=1 ; i<size-2 ; i++)
				status = execv( executable_path , (char**)argv );
			if(status == -1)
				cout<<" error!";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"rename") == 0 )
	{
		char* temp = new char[400];
		strcpy(temp,cwd);
		argv[size-1]=temp;
		argv[size]=NULL;
		pid=fork();
		if(pid==0)
		{
			status = execv( executable_path , (char**)argv );
			if(status == -1)
				cout<<" error!";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"snapshot") == 0 )
	{
		char* temp = new char[400];
		strcpy(temp,cwd);
		argv[3]=temp;
		argv[4]=NULL;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "snapshot" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<" error!";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"search") == 0 )
	{
		char* temp=new char[400];
		strcpy( temp,cwd );
		argv[2]=temp;
		argv[3]=NULL;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "search" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<" error!";
			exit(1);
		}
		pid=wait(NULL);
	}

	else if( strcmp(tokens[0],"goto") == 0 )
	{
		if( tokens[1]=="/" )
			strcpy( (char*)argv[1] ,home);
		goto_flag=true;
		chdir(argv[1]);
	}

	else if( strcmp(tokens[0],"create_file") == 0 )
	{
		//cout<<argv[1]<<" "<<argv[2];
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "create_file" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<" error! ";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"create_dir") == 0 )
	{
		//cout<<argv[1]<<" "<<argv[2];
		pid=fork();
		if(pid==0)
		{
			status = execv( executable_path , (char**)argv );
			if(status == -1)
				cout<<" error! ";
			exit(1);
		}
	}
	
	else if( strcmp(tokens[0],"delete_file") == 0 )
	{
		if(in_bin)   //permanently remove file if in recycle bin folder
		{
			remove( tokens[1] );
			delete_entry( tokens[1], recyclePath );
			return false;
		}
		char* temp=new char[400];
		strcpy( temp,home );
		argv[3]=temp;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "delete_file" , argv[1] , argv[2] , argv[3] , NULL );
			if(status == -1)
				cout<<" error! ";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"restore") == 0 )
	{
		char* temp=new char[400];
		strcpy( temp,home );
		argv[2]=temp;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "restore" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<" error! ";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"delete_dir") == 0 )
	{
		if(in_bin)   //permanently remove dir if in recycle bin folder
		{
			deleteDir( tokens[1] );
			delete_entry( tokens[1], recyclePath );
			return false;
		}
		char* temp=new char[400];
		strcpy( temp,home );
		argv[3]=temp;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "delete_dir" , argv[1] , argv[2] , argv[3] , NULL );
			if(status == -1)
				cout<<" error! ";
			exit(1);
		}
	}

	return goto_flag;
}



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
void deleteDir(char* src)
{
    if(is_dir(src))  //if destination is a directory, recursively call the copy function
    {
	DIR* p;
	struct dirent *d;
	p=opendir(src);        //opendir opens it's input directory and returns a directory pointer
	if(p==NULL)
	{
		perror("Cannot find directory\n");
		return;
	}
	while(d=readdir(p))    //keep reading files of directory until none are left
	{
		if(  ( strcmp(d->d_name,".")!=0 )  &&  ( strcmp(d->d_name,"..")!=0 )  )
		{
			char file_inside[400];
			strcpy(file_inside,src);
			strcat(file_inside,slash);
			strcat(file_inside,d->d_name);

			deleteDir ( file_inside );   //recursive call to deleteDir
		}
	}
    }
    if( ! is_dir(src) )   //deletes files
	remove( src );
    else
	rmdir( src );   //deletes directory(empty)
    
}

void delete_entry(char* entry,char* recovery)
{
	string word, path;
	bool found=false;

	string paths[600];
	int i=0;

	fstream file;
	file.open( recovery );

	while (file >> word)
	{
        	if( strcmp( word.c_str() , entry ) == 0  )
		{
			file >> path;   //extract the recycle path(from where it was deleted) corresponding to the filename
			found=true;
			file >> word;
		}
		paths[i] = word;
		i++;
	}
	file.close();

	if(found)
	{
		file.open ( recovery , ios::out | ios::trunc );  //clear the file
		file.close();
		
		file.open ( recovery , ios::out | ios::app );
		int j=0;
		while(j<i-1)
		{
			file << paths[j++];   //the recyclePath file is restored after deleting the restored file
			file << " ";
			file << paths[j++];
			file << endl;
		}
	}
	file.close();
	
}
