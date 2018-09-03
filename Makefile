CC=g++
CFLAGS= -Wall -std=c++1z
DEPS = FileAtt.o navigate.o myCommands.o display.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

all: fileExplorer snapshot rename search move copy create_file create_dir delete_file delete_dir restore

fileExplorer: main.cpp FileAtt.cpp navigate.cpp myCommands.cpp display.cpp
	$(CC) -o fileExplorer main.cpp FileAtt.cpp navigate.cpp myCommands.cpp display.cpp

snapshot: snapshot.cpp
	$(CC) -o snapshot snapshot.cpp

rename: rename.cpp
	$(CC) -o rename rename.cpp

search: search.cpp
	$(CC) -o search search.cpp

move: move.cpp
	$(CC) -o move move.cpp

create_file: create_file.cpp
	$(CC) -o create_file create_file.cpp

create_dir: create_dir.cpp
	$(CC) -o create_dir create_dir.cpp

delete_file: delete_file.cpp
	$(CC) -o delete_file delete_file.cpp

delete_dir: delete_dir.cpp
	$(CC) -o delete_dir delete_dir.cpp

restore: restore.cpp
	$(CC) -o restore restore.cpp
