#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

void do_ls(char **argv) {
	const char *dir_path = ".";
	struct dirent *dp;
	struct stat buf;

	DIR *dirp = opendir(dir_path);

	printf("%8s\t%8s\t%20s\t%8s\n", "inode", "nlink", "name", "size");

	while ((dp = readdir(dirp)) != NULL) {
	  lstat(dp->d_name, &buf);
	  // printf("%d\n", buf.st_mode);
	  printf("%8ld\t%8ld\t%20s\t%8ld\n", (long int)buf.st_ino, (long int)buf.st_nlink, dp->d_name, (long int)buf.st_size);
	}
	closedir(dirp);
}