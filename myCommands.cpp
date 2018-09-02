#include <stdio.h>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

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

	//cout<<size<<endl;
	int pid;
	int status=1;
	char* executable_path=new char[400];
	char slash[2] = "/";

	strcpy( executable_path , home );
	strcat( executable_path , slash );
	strcat( executable_path , tokens[0] );

	bool goto_flag=false;

	if( strcmp(tokens[0],"copy") == 0 )
	{
		pid=fork();
		if(pid==0)
		{	
			for(i=1;i<size-2;i++)
			{
				status = execv( executable_path , (char**)argv );
			}
			if(status == -1)
				cout<<"error!";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"move") == 0 )
	{
		pid=fork();
		if(pid==0)
		{
			for(i=1 ; i<size-2 ; i++)
				status = execv( executable_path , (char**)argv );
			if(status == -1)
				cout<<"error!";
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
				cout<<"error!";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"snapshot") == 0 )
	{
		char* temp=new char[400];
		strcpy( temp,home );
		argv[2]=temp;
		strcat( (char*)argv[2],"/dumpfile.txt");  //dumpfile is in ~(of project) directory
		argv[3]=NULL;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "snapshot" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<"error!";
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
				cout<<"error!";
			exit(1);
		}
		pid=wait(NULL);
	}

	else if( strcmp(tokens[0],"goto") == 0 )
	{
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
				cout<<"error! ";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"create_dir") == 0 )
	{
		//cout<<argv[1]<<" "<<argv[2];
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "create_dir" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<"error! ";
			exit(1);
		}
	}
	
	else if( strcmp(tokens[0],"delete_file") == 0 )
	{
		char* temp=new char[400];
		strcpy( temp,home );
		argv[2]=temp;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "delete_file" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<"error! ";
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
				cout<<"error! ";
			exit(1);
		}
	}

	else if( strcmp(tokens[0],"delete_dir") == 0 )
	{
		char* temp=new char[400];
		strcpy( temp,home );
		argv[2]=temp;
		pid=fork();
		if(pid==0)
		{
			status = execl( executable_path , "delete_dir" , argv[1] , argv[2] , NULL );
			if(status == -1)
				cout<<"error! ";
			exit(1);
		}
	}

	return goto_flag;
}

/*int main()
{
	string s="search navigate.h";
	char* command=&s[0];
	char home[]="/home/devesh/Desktop";
	char x[]="/home/devesh/Documents";
	execute_command(command,home,x);
	return 0;
}*/
