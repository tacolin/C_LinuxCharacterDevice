#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define RED  	"\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define NONE 	"\033[m"

int main( int argc, char* argv[])
{
	int fd;

	if ( 2 > argc )
	{
		printf(RED "[taco-error] input parameters are not enough\n" NONE);
		goto _HINT;
	}

	fd = open("/dev/tacobuf", O_RDWR);
	if ( fd < 0 )
	{
		printf(RED "[taco-error] open device tacobuf failed\n" NONE);
		printf(GREEN "[taco] 1) maybe you should insert kernel module first\n" NONE);
		printf(GREEN "[taco] 2) maybe you should use sudo to execute this program\n" NONE);
		return -1;
	}

	if ( 0 == strcmp( argv[1], "read" ) )
	{
		unsigned char _ubuf[128];

		memset(_ubuf, 0, 128);

		if ( 0 > read(fd, _ubuf, 128) )
		{
			printf(RED "[taco-error] read data from tacobuf failed\n" NONE);
			close(fd);
			return -1;			
		}		
		
		printf(YELLOW "[taco] read data : %s\n" NONE, _ubuf);
	}
	else if ( 0 == strcmp( argv[1], "write" ) )
	{
		if ( 3 > argc )
		{
			printf(RED "[taco-error] input parameters are not enough\n" NONE);
			goto _HINT;
		}

		if ( 0 > write(fd, argv[2], strlen(argv[2])+1 ) )
		{
			printf(RED "[taco-error] write data to tacobuf failed\n" NONE);
			close(fd);
			return -1;
		}
		printf(YELLOW "[taco] write data : %s\n" NONE, argv[2]);
	}
	else
	{
		goto _HINT;
	}
	
	close(fd);
	return 0;

_HINT:
	printf(GREEN "[taco] action should be 'read' or 'write'\n" NONE );
	printf(GREEN "[taco] sudo ./main read\n" NONE );
	printf(GREEN "[taco] sudo ./main write 'your input string'\n" NONE );
	close(fd);
	return -1;
}
