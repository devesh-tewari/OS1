#include <string.h>


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <iomanip>

using namespace std;

struct FileAttributes
{
	string file_name;
	float  file_size;
	bool   k,m;
	string owner_user;
	string owner_group;
	char   file_permissions[11];
	string last_modified;
	struct FileAttributes* next_file;
	struct FileAttributes* prev_file;
};

struct FileAttributes getFileAttributes(char*);

extern struct FileAttributes* first_file;
extern struct FileAttributes* last_file;

void insert( struct FileAttributes);  
int display(struct FileAttributes*);


