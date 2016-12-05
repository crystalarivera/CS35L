#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>             // Allow C to recognize your system calls
#include <sys/syscall.h>     // Get entry point for system calls

// uses read/write to read and write each byte
// The nbyte argument should be 1

void errorReport(const char* msg);

int main(int argc, const char* argv[])
{
  const char* from = argv[1];
  const char* to   = argv[2];
  const size_t len = strlen(from);


  /* checks to see if inputs are valid */
  //are there 3 arguments?
  if (argc != 3)
    errorReport("Number of arguments is not 3.");
  //is from the same size as two?
  if (len != strlen(to))
    errorReport("Different sizes of key translations.");
  //does from only contain unique bytes/chars?
  for(size_t i = 0; i < len; i++)
      for(size_t j = i+1; j < len; j++)
	  if(from[j] == from[i])
	    errorReport("From should not contain duplicate bytes.");


  /* translate */
  char curr;
  ssize_t io = read(STDIN_FILENO, &curr, 1);
  if (io <= 0) errorReport("Error reading from file.");
  while(io > 0) //0 acts as EOF
    {
      for(size_t i = 0; i < len; i++)
	{
	  if(curr == from[i])
	    {
	      curr = to[i];
	      break;
	    }
	}
      io = write(STDOUT_FILENO, &curr, 1);
      if (io <= 0) 
	{
	  errorReport("Error writing to stdout.");
	}
      io = read(STDIN_FILENO, &curr, 1);
      if (io <= -1) //-1 is output if error 
	{
	  errorReport("Error reading from file.");
	}
    }
  return 0;
}

void errorReport(const char* msg)
{
  fprintf(stderr, "Error: %s \n", msg);
  exit(1);
}
