#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
	int8_t read_buf[1024];
	int8_t write_buf[1024] = "Debug with Strace\n";

	int fd = open("file.txt", O_RDWR, 0644);

	if(fd < 0)
	{
		printf("Cannot open file ...\n");
		return 0;
	}

	write(fd, write_buf, strlen(write_buf) + 1);

	lseek(fd, 0, SEEK_SET); // Dua con tro ve vi tri dau tien
	
	read(fd, read_buf, 1024);

	printf("Data = %s\n", read_buf);

	close(fd);

	return 0;
}
