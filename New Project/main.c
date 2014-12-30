#include <stdio.h>
#include <stdlib.h>
#define max(a,b) ( (a) > (b) ? (a) : (b) )

void print_default_verse()
{
 printf("For God loved the world so much that he gave his only Son, so that everyone who believes in him may not die but have eternal life.\n-- John 3:16"); 
}

long int fsize(FILE *file)
{
 long int currentpos;
 long int size;
  currentpos = ftell(file);
  fseek(file,0,SEEK_END);
  size = ftell(file);
  fseek(file,currentpos,SEEK_SET);
  return size;
}

long int custom_random(long int range)
{
   long int number;
   time_t t;
   srand((unsigned) time(&t));
   
   /* In case our MAX_RAND is on 32767 */ 
   number = (  rand() & 0xFFFF ) | 
            ( (rand() & 0x7FFF ) << 16 );
			
   return number % range;
}

int main(int argc, char *argv[])
{
    FILE *file;
	long int filesize;
    char buffer[2048+1]; /* Add space for NULL terminator */ 
	long int buffersize;
	long int start;
	long int bufferstart;
	char *startpointer;
	char *currentpointer;
	
	if( argc <= 1 ) 
	{
	  printf("No verse file specified!\n");
	  print_default_verse( );
	  return -1;
	}
	
	file = fopen(argv[1],"r");
	if(!file)
	{
	  printf("Could not open file %1\n",argv[1]);
	  print_default_verse( );
	  return -1;
	}
	
	/* find random position in file */ 
	filesize = fsize(file);
	start = custom_random( filesize );
	
	printf("FileSize:%i,Start:%i\n",filesize,start);
	
	/* read 1024 on either side of the random position */ 
	bufferstart = max( 0, start - 1024 );
	
	fseek(file,start,SEEK_SET);
	buffersize = fread(buffer,1,2048,file);
	fclose( file );
	if( buffersize <= 0 )
	{
		printf("Error reading file!\n");
		print_default_verse();
		return -1;
	}
	
	printf("Read %i bytes!",buffersize);
	
	buffer[buffersize] = 0;
	
	start = start - bufferstart;
	startpointer = &buffer[start];
	
	while(startpointer >= buffer && *startpointer != '%')
	  startpointer--;
	  
	if(*startpointer == '%' )
       startpointer++;

	currentpointer = startpointer;
	
	while(*currentpointer)
	{
	  if(*currentpointer =='%')
	  {
	    *currentpointer = 0;
		break;
	  }
	  currentpointer++;
	}
	
	printf("%s",startpointer);
}
