#include <iostream>
#include <string.h>
#include <cctype>   // isdigit
#include <stdlib.h> // atoi
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define BUFFSIZE 10

// Joeseph Fritz, Mason Protsman, Andrew Phipps
// 1730 - head.cpp

int readWrite(char filename[], int lnCount);

int main(int argc, char* argv[]){
  if(argc < 2){
    cout << "Invalid command line arguments! Input should look like: " << endl;
    cout << "./head filename..." << endl << "or" << endl << "./head -n number filename..." << endl;
    return -1;
  }

  // lnCount is defaulted to 10 unless command line arguments are passed.
  int lnCount = 10;
  if(strcmp(argv[1], "-n") == 0 && isdigit((argv[2])[0]))
    lnCount = atoi(argv[2]);

  // No command line arguments provided (default line count)
  if(argc == 2){
    readWrite(argv[1], lnCount);
  }
  // Command line arguments provided (custom line count/multiple files)
  else{
    // One file provided with overridden lnCount
    if(argc == 4 && lnCount != 10)
      readWrite(argv[3], lnCount);
    // Multiple files provided
    else{
      // Determine at which index we should start (1 if the lnCount hasn't been overriden, 3 if is has).
      int i = (lnCount == 10) ? 1 : 3;
      // Loop through each of the files, providing a title and printing out the first few lines.
      for(; i < argc; i++){
	cout << endl << "==> " << argv[i] << " <==" << endl;
	readWrite(argv[i], lnCount);
      }
    }
  }
}

// Reads and writes the provided file up the lnCount (or less if the file contains less lines).
int readWrite(char filename[], int lnCount){
  int fd;
  fd = open(filename, O_RDONLY);
  if(fd < 0){
    cout << "Could not open " << filename << " fd = " << fd << endl;
    return fd;
  }

  char buf[BUFFSIZE];
  int r;
  while(lnCount > 0 && (r = read(fd, buf, BUFFSIZE)) > 0){
    // Count the number of lines and bytes in the input and decrement them from the lnCount.
    int byteCount = 0;
    for(int i = 0; i < r && lnCount > 0; i++){
      byteCount++;
      if(buf[i] == '\0' || buf[i] == '\n'){
	lnCount--;
      }
    }
    
    if(write(STDOUT_FILENO, buf, byteCount) != byteCount){
      cout << "Write error!" << endl;
      return -1;
    }
  }

  return 0;
}
