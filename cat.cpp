#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define BUFFSIZE 5

// Joeseph Fritz, Mason Protsman, Andrew Phipps
// 1730 - cat.cpp

void readWriteInput();

int main(int argc, char* argv[]){  
  // there are no arguments passed.
  if(argc == 1){
    readWriteInput();
    
    return 0;
  }

  // there are actual arguments
  for(int i = 1; i < argc; i++){
    char buf[BUFFSIZE];
    // If the arg is a dash, read from standard input and write to standard out.
    if(strcmp(argv[i], "-") == 0){
      readWriteInput();
    }
    // Otherwise, try and read in a file name and write its contents to standard out.
    else{
      int fd;
      fd = open(argv[i], O_RDONLY);
      if(fd < 0){
	cout << "Could not open " << argv[i] << " fd = " << fd << endl;
      }
      else{
	int r;
	while ((r = read(fd, buf, BUFFSIZE)) > 0){
	  if(write(STDOUT_FILENO, buf, r) != r){
	    cout << "Write error!" << endl;
	  }
	}
      }
    }
  }

  return 0;
}

// Reads from standard input and writes the read message to standard out.
void readWriteInput(){
  char buf[BUFFSIZE];
  int r;
  while((r = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
    if(write(STDOUT_FILENO, buf, r) != r){
      cout << "write error!" << endl;
    }
  }
}
