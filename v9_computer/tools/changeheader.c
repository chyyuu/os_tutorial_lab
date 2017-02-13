#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <algorithm>

using namespace std;

char* content;
char* content1;

int main(int argc, char** argv) {

	int fd;
	fd = open(argv[1], O_RDONLY); /*打开/etc/passwd */
	float *start;
	struct stat sb;
	fstat(fd, &sb); /* 取得文件大小 */
	content1 = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	content = (char*)calloc(sb.st_size,sizeof(char));
	memcpy(content, content1, sb.st_size);
	close(fd);

	int size = sb.st_size - 16;
	FILE *fout = fopen(argv[1], "wb");
	fwrite(content, sizeof(int), 1, fout);
	fwrite(&size, sizeof(int), 1, fout);
	fwrite(content+4, sizeof(int), 1, fout);
	fwrite(content+8, sizeof(int), 1, fout);
	fwrite(content+12, sizeof(int), 1, fout);
	fwrite(content+16, sizeof(char), size, fout);
	fclose(fout);

	return 0;
}
