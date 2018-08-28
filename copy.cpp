#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

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
int copy(char* src, char* dst, bool first_call)
{
    char new_dir[200];
    if(is_dir(src))  //if destination is a directory, recursively call the copy function
    {
	//cout<<src<<" "<<dst;
	int status;
	char* dir_name=new char[200];  //destination directory name which has to be created
	strcpy( dir_name , src );
	dir_name = get_dir_name_from_path( dir_name ); //last folder of source path is now stored in dir_name
	//printf("\ndir_name: %s\n",dir_name);
	strcpy(new_dir,dst);   //since src is a directory, we have to create a new directory in destination folder
	if(first_call)   //on first call of this function, it will append dir_name to new_dir
	{
		strcat(new_dir,slash);
		strcat(new_dir,dir_name);
	}
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
			char file_inside[200];
			strcpy(file_inside,src);
			strcat(file_inside,slash);
			strcat(file_inside,d->d_name);  //file in the directory will have its path as directoryPATH/filename
			//printf("\n%s\n",file_inside);
			char temp_dst[200];
			strcpy(temp_dst,new_dir);
			strcat(temp_dst,slash);
			strcat(temp_dst,d->d_name);  //same with destination's new file on new directory new_dir
			//printf("%s\n",temp_dst);
			copy ( file_inside , temp_dst , false );   //recursive call to copy
		}
	}
    }

    else if(is_dir(dst))  //if source is a file and destination is a directory
    {
	char* file_name=new char[200];  //source file name
	strcpy( file_name , src );
	file_name = get_dir_name_from_path( file_name );
	strcat(dst,slash);
	strcat(dst,file_name);
    }

    ifstream source ( src , ios::binary );
    ofstream dest ( dst , ios::binary );

    if(!source) 
    {
        cout<<"Can't open source file(s)\n";
	//return 0;
    }
    if(!dest)
    { 
        cout<< "\nCan't open destination file: "<<dst<<" src:\n"<<src;
	//return 0;
    }

    dest << source.rdbuf();
    
    source.close();
    dest.close();

    struct stat fst;
    stat(src,&fst);
    chown(dst,fst.st_uid,fst.st_gid);   //update owner and group to original file
    chmod(dst,fst.st_mode);   //update the permissions like that of original file
    return 1;
}

int main(int argc, char* argv[]) 
{
    if(argc<3)
    {
	cout<<"Insuffecient arguments\n";
	return 0;
    }
    int i=1;
    while(i<argc-1)
    {
	if(!is_file(argv[i]) && !is_dir(argv[i]))
	{
		cout<<argv[i]<<": source does not exist!\n";
		return 0;
	}
	i++;
    }
    if(is_file(argv[argc-1]))
    {
	if(argc==3)
		cout<<argv[argc-1]<<" already exists. Over-write? (y/n): ";
	/*else
	{
		cout<<"Destination file does not exist!\n";
		return 0;
	}*/
	char c;
        cin>>c;
        if ( c== 'n' || c=='N' )
    		return 0;
    }
    
    for(i=1;i<argc-1;i++)
	copy( argv[i] , argv[argc-1], true );
    
    return 0;
}
