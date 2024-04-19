/**
 * @author	Hasan Onuralp AGCA
 * @date	19.04.2024
 * 
 * @brief	User Space App
 *			Steps
 *			create device node file (mknod) -> Perform file 
 *			operations through this node file
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>


#define MAJOR 25
#define MINOR 0

void exit_sys(const char *msg);

int main(void)
{
	int fd;
	const char *node_file = "device_node_file";
	mode_t mode = S_IFCHR | 0777;

	if(access(node_file, F_OK) != -1){
		//device node is already exists, skip the creation
	}else{
		if(mknod(node_file, mode, makedev(MAJOR, MINOR)) == -1)
			exit_sys("mknod");
	}

	if((fd = open(node_file, O_RDWR)) == -1)
		exit_sys("open");

	close(fd);

	return 0;

}

void exit_sys(const char *msg)
{
    perror(msg);

    exit(EXIT_FAILURE);
}