#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

bool is_file(const char* path)  //returns true if the file exists
{
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
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

//char* name=new char[40];
char* get_dir_name_from_path(char* path)  //this function returns the last folder from an input path
{
    string str;
    str=path;
    int k=str.find_last_of("/");
    if(k==-1)  //if the input does not have any slashes
	return path;
    int size =str.size();
    string name=str.substr(k+1,(size-k));
    //printf("\nname: %s\n",name);
    //cout <<"NAME: "<< name;
    return &name[0];
}

char slash[2]="/";
int move(char* src, char* dst, bool first_call)
{
    int len;
    char new_dir[PATH_MAX];
    if(is_dir(src))  //if destination is a directory, recursively call the move function
    {
	//cout<<src<<" "<<dst;
	int status;
	char* dir_name=new char[PATH_MAX];  //destination directory name which has to be created
	strcpy( dir_name , src );
	dir_name = get_dir_name_from_path( dir_name ); //last folder of source path is now stored in dir_name
	//printf("\ndir_name: %s\n",dir_name);
	strcpy(new_dir,dst);   //since src is a directory, we have to create a new directory in destination folder
	/*if(first_call)   //on first call of this function, it will append dir_name to new_dir
	{
		strcat(new_dir,slash);
		strcat(new_dir,dir_name);
	}*/
	//printf("\nnew dir:%s\n",new_dir);
	status = mkdir( new_dir , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );  //make new directory new_dir(like the one in source)
	if(status==-1)
		cout<<"Unable to create directory";
	//printf("\ndes_dir: %s\n",dst);
	DIR* p;
	struct dirent *d;
	p=opendir(src);        //opendir opens it's input directory and returns a directory pointer
	if(p==NULL)
	{
		perror("Cannot find directory\n");
		return 0;
	}
	while(d=readdir(p))    //keep reading files of directory until none are left
	{
		//printf("\n%s\n",d->d_name);
		//printf("%d",strlen(d->d_name));
		if(  ( strcmp(d->d_name,".")!=0 )  &&  ( strcmp(d->d_name,"..")!=0 )  )
		{
			//printf("\n%s\n",d->d_name);
			char file_inside[PATH_MAX];
			strcpy(file_inside,src);
			strcat(file_inside,slash);
			strcat(file_inside,d->d_name);  //file in the directory will have its path as directoryPATH/filename
			//printf("\n%s\n",file_inside);
			char temp_dst[PATH_MAX];
			strcpy(temp_dst,new_dir);
			strcat(temp_dst,slash);
			strcat(temp_dst,d->d_name);  //same with destination's new file on new directory new_dir
			//printf("%s\n",temp_dst);
			move ( file_inside , temp_dst , false );   //recursive call to move
		}
	}
    }

    else if(is_dir(dst))  //if source is a file and destination is a directory
    {
	char* file_name=new char[PATH_MAX];  //source file name
	strcpy( file_name , src );
	len=strlen(dst);
	file_name = get_dir_name_from_path( file_name );
	strcat(dst,slash);
	strcat(dst,file_name);
    }

    if( !is_dir(src) && !is_dir(dst) )
    {
	 struct stat s;
	 stat(src,&s);
	 int original_uid=s.st_uid;    //store original user of source
	 int original_gid=s.st_gid;    //store original group of source
	 int original_permissions=s.st_mode;   //store original permissions

	 if (rename(src, dst) != 0)   //rename system call renames(moves) files
	 {
		printf("Error!");
		return 0;
	 }

	 chown ( dst , original_uid , original_gid );   //update owner and group to original file
	 chmod ( dst , original_permissions );   //update the permissions like that of original file
    }
    //dst[len]='\0';
    if( is_dir(src) )
    {
	rmdir(src);
    }

    return 1;
}


int main( int argc, char* argv[] )  //1st argument=filename, 2nd: home path
{
	if( argc != 3 )
	{
		cout<<" Error! ";
		return 0;
	}

	char bin_path[400];
	strcpy( bin_path , argv[2] );
	strcat(bin_path,"/RecycleBin");

	char restore_file[400];
	strcpy( restore_file , bin_path );
	strcat( restore_file , "/recyclePath.txt" );

	strcat(bin_path,"/");
	strcat(bin_path,argv[1]);

	string word, path;
	bool found=false;

	string paths[600];
	int i=0;

	fstream file;
	file.open( restore_file );

	while (file >> word)
	{
        	if( strcmp( word.c_str() , argv[1] ) == 0  )
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
		if(!is_dir( bin_path ))
			rename( bin_path , &path[0] );
		else
			move( bin_path , &path[0] , true );

		file.open ( restore_file , ios::out | ios::trunc );  //clear the file
		file.close();
		
		file.open ( restore_file , ios::out | ios::app );
		int j=0;
		while(j<i-1)
		{
			file << paths[j++];   //the recyclePath file is serstored after deleting the restored file
			file << " ";
			file << paths[j++];
			file << endl;
		}
	}
	file.close();
	
	return 0;
}
