fileExplorer

1. RecycleBin must reside in ~ (home path of the project). The file 'recyclePath' inside RecycleBin must not be deleted.

2. 'delete_file' and 'delete_dir' commands must be provided with path including the filename.
	e.g. delete_file foo.txt ~/folder1/foo.txt
	     delete_dir abc.md ~/abc.md

3. Each command except goto has a different independent executable file. The 'execute_command' function in 'myCommands.cpp' calls fork/exec for the corresponding command.

4. 'restore' command can work anywhere. It will look for file to be restored in RecycleBin. 

5. Vertical and Horizontal overflow are handled dynamically. If the termical size changes, the next button pressed will handle the changes in size. (buttons UP, DOWN, LEFT, RIGHT, ENTER, BACKSPACE, h )

6. To run the program, enter command './fileExplorer'
