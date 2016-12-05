#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// uses getchar/putchar, read from STDIN and write to STDOUT

void errorReport(const char* msg);

int main(int argc, const char * argv[])
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
    errorReport("Error: Different lengths of key translations.");
  //does from only contain unique bytes/chars?
  for(size_t i = 0; i < len; i++)
      for(size_t j = i+1; j < len; j++)
	  if(from[j] == from[i])
	    errorReport("From should not contain duplicate bytes.");


  /* translate */
  char curr = getchar();
  if (ferror(stdin) || curr == '\n') errorReport("Error reading from file.");

  while(curr != EOF)
    {
      for(size_t i = 0; i < len; i++)
	{
	  if(curr == from[i])
	    {
	      curr = to[i];
	      break;
	    }
	}
      if (putchar(curr) == EOF) errorReport("Error writing to stdout.");
      curr = getchar();
      if (ferror(stdin)) errorReport("Error reading from file.");
    }
  return 0;
}

void errorReport(const char* msg)
{
  fprintf(stderr, "Error: %s \n", msg);
  exit(1);
}
