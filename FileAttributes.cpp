/*  #include <sys/stat.h>
    /**
     * Get the size of a file.
     * @return The filesize, or 0 if the file does not exist.
     */
    size_t getFilesize(const char* filename) {
        struct stat st;
        if(stat(filename, &st) != 0) {
            return 0;
        }
        return st.st_size;   
    }

size_t getFilesize(const std::string& filename) {
    struct stat st;
    if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;   
}
*/

//https://www.youtube.com/watch?v=qGcbxORzWFk
//https://www.google.com/search?client=ubuntu&hs=lCV&channel=fs&biw=1405&bih=707&tbm=isch&sa=1&ei=w1d9W6TeBNqR9QO9o5PIAw&q=st_mode+bits&oq=st_mode+bits&gs_l=img.3...214740.216489.0.216695.5.5.0.0.0.0.186.683.0j4.4.0....0...1c.1.64.img..1.1.182...0i8i30k1j0i24k1.0.w9K7SBLnEKs#imgrc=ldZZW1aczCpXOM:

/*
#include <pwd.h>
#include <grp.h>

struct group *grp;
struct passwd *pwd;

grp = getgrgid(gid);
printf("group: %s\n", grp->gr_name);

pwd = getpwuid(uid);
printf("username: %s\n", pwd->pw_name);
*/


#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
using namespace std;

struct FileAttributes
{
	string file_name;
	int    file_size;
	char   file_permissions[10];
	string last_modified;
};

struct FileAttributes getFileAttributes(string& file_name)
{
	struct stat s;
	if ( stat(file_name,&s) != 0 )                    //if the file does not exist
		return 0;

	struct FileAttributes FileAtt;	
	
	FileAtt.file_size=s.st_size;
	
	(S_ISDIR(fileStat.st_mode)) ? FileAtt.file_permissions[0]='d' : FileAtt.file_permissions[0]='-');    //here the bits of permissions stored in st_mode are changed to corresponding character
    	(fileStat.st_mode & S_IRUSR) ? FileAtt.file_permissions[1]='r' : FileAtt.file_permissions[1]='-');
    	(fileStat.st_mode & S_IWUSR) ? FileAtt.file_permissions[2]='w' : FileAtt.file_permissions[2]='-');
    	(fileStat.st_mode & S_IXUSR) ? FileAtt.file_permissions[3]='x' : FileAtt.file_permissions[3]='-');
    	(fileStat.st_mode & S_IRGRP) ? FileAtt.file_permissions[4]='r' : FileAtt.file_permissions[4]='-');
    	(fileStat.st_mode & S_IWGRP) ? FileAtt.file_permissions[5]='w' : FileAtt.file_permissions[5]='-');
	(fileStat.st_mode & S_IXGRP) ? FileAtt.file_permissions[6]='x' : FileAtt.file_permissions[6]='-');
	(fileStat.st_mode & S_IROTH) ? FileAtt.file_permissions[7]='r' : FileAtt.file_permissions[7]='-');
	(fileStat.st_mode & S_IWOTH) ? FileAtt.file_permissions[8]='w' : FileAtt.file_permissions[8]='-');
	(fileStat.st_mode & S_IXOTH) ? FileAtt.file_permissions[9]='x' : FileAtt.file_permissions[9]='-');

	string last_modified = ctime( &s.st_mtime );
	return FileAtt;
}






