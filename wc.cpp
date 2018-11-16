#include <iostream>
#include <string.h>
#include <ctype.h> // isspace
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define BUFFSIZE 5

// Joeseph Fritz, Mason Protsman, Andrew Phipps
// 1730 - wc.cpp

int isSpace(char c);

int main(int argc, char* argv[]){
  int totalLines = 0;
  int totalWords = 0;
  int totalCount = 0;
  
  // Determine the number of files to process. If there is more than one arg (meaning there was more input than just ./wc) set the fileCount to argc - 1, otherwise set it to one.
  int fileCount = (argc > 1) ? argc - 1 : 1;
  
  int usingOption = 0;
  // Check if the user provided any command line arguments. If they did set the bool to using option and decrement the fileCount by one (since the option is not a file).
  if(argc > 1 && (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "-w") == 0)){
    fileCount--;
    usingOption = 1;
  }
  
  // For every file the user inputed open, read, and count the number of bytes, words, and new lines.
  for(int i = 0; i < fileCount; i++){
    // Find the index of the filename in argv. It should be one plus i in order to exclude ./wc. If we're using an option we should also add one in order to exclude the option.
    int index = (i + 1) + usingOption;

    int fd;
    // If this file is a dash or if there was no options or files provided, read from standard input.
    if((argc > 1 && strcmp(argv[index], "-") == 0) || argc == 1)
      fd = STDIN_FILENO;
    // Otherwise open the file.
    else
      fd = open(argv[index], O_RDONLY);
    
    if(fd < 0){
      cout << "Could not open " << argv[index] << " fd = " << fd << endl;
      return -1;
    }

    int count = 0;
    int newLines = 0;
    int words = 0;
    
    int r = 0;
    char buf[BUFFSIZE];
    string input = "";
    // Read through the file and add each read buffer to a string for processing later.
    while((r = read(fd, buf, BUFFSIZE)) > 0){
      count += r; // Add to the count based on how many bytes were read.
      for(int i = 0; i < r; i++)
	input += buf[i];
    }

    for(int i = 0; i < input.length(); i++){
      if(isSpace(input[i]) == 0 && (((i+1) == input.length()) || isSpace(input[i+1]) == 1))
	words++;

      if(input[i] == '\n')
	newLines++;
    }

    totalLines += newLines;
    totalWords += words;
    totalCount += count;

    // Print results based on the tag provided
    if(argc > 1 && (strlen(argv[1]) > 1 && (argv[1])[0] == '-')){
      if(strcmp(argv[1], "-c") == 0)
	cout << count << " " << argv[index] << endl;
      else if(strcmp(argv[1], "-l") == 0)
	cout << newLines << " " << argv[index] << endl;
      else if(strcmp(argv[1], "-w") == 0)
	cout << words << " " << argv[index] << endl;
    }
    // Print all results if no tag was provided
    else{
      cout << "\t" << newLines << " " << words << " "  << count << " "  << ((fd != STDIN_FILENO) ? argv[index] : "") << endl;
    }
  }

  // Print total of all files if there was more than one
  if(fileCount > 1){
    // If a tag was provided only show the value for the specified tag.
    if(argc > 1 && (strlen(argv[1]) > 1 && (argv[1])[0] == '-')){
      if(strcmp(argv[1], "-c") == 0)
	cout << totalCount << " " << "total" << endl;
      else if(strcmp(argv[1], "-l") == 0)
	cout << totalLines << " " << "total" << endl;
      else if(strcmp(argv[1], "-w") == 0)
	cout << totalWords << " " << "total" << endl;
    }
    // Otherwise print all of the values.
    else
      cout << "\t" << totalLines << " " << totalWords << " " << totalCount << " total" << endl;
  }

  return 0;
}

int isSpace(char c){
  if(c == '\n' || c == '\t' || c == ' ')
    return 1;
  else
    return 0;
}
