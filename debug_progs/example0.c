#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define FILENAME "testfile"
#define FILE_SIZE 256

#define OVERWRITE_POINTER 156
#define READ_POINTER 152
#define READ_BYTES 10

int main (int argc, char **argv) 
{
	int fd;
	char buffer[FILE_SIZE];
	struct stat stbuf;

// まず、"testfile"という名前のファイルが存在するかどうかを確認し、
	if (access(FILENAME, F_OK) == 0)
	{
// 存在していたらそのファイルを削除する。
		if (unlink(FILENAME) == -1) 
		{
			exit(1);
		}
	}

// 次に、"testfile"という名前の(空の)ファイルを作成する。
	if ((fd = creat(FILENAME, S_IREAD | S_IWRITE)) == -1)
	{
// 作成に失敗したら、エラーメッセージを表示して終了。
		printf("creat() fail...");
		exit(1);
	}

	if (close(fd) == -1)
	{
		exit(1);
	} 
	if ((fd = open(FILENAME, O_RDWR)) == -1)
	{
		exit(1);
	}

// 作成に成功したら、ファイルの先頭から256バイト目までのすべてのバイトに、'X'という文字を書き込む。
	memset(buffer, 'X', FILE_SIZE);
	if (write(fd, buffer, FILE_SIZE) == -1)
	{
// 書き込みに失敗したら、エラーメッセージを表示して終了。
		printf("write() fail...");
		exit(1);
	}

// 次に、ファイルの先頭から156バイト目の位置から"test"という文字列を書き込む。
	if (lseek(fd, OVERWRITE_POINTER, SEEK_SET) == -1) 
	{
		printf("fail...");
		exit(1);
	}
	if (write(fd, "test", strlen("test")) == -1)
	{
// 書き込みに失敗したら、エラーメッセージを表示して終了。
		printf("write() overwrite fail...");
		exit(1);
	}

// 次に、ファイルの先頭から152バイト目の位置から10バイトを読み込む。
// 読み込みに失敗したら、エラーメッセージを表示して終了。
	if (lseek(fd, READ_POINTER, SEEK_SET) == -1) 
	{
		printf("seek fail...");
		exit(1);
	}
	memset(buffer, 0, FILE_SIZE);
	if (read(fd, buffer, READ_BYTES) == -1) 
	{
		printf("read fail...! :%s \n", strerror(errno));
		exit(1);
	}

// 読み込んだ内容を表示する。
	printf("Data: %s\n", buffer);
// ファイルの大きさ(単位: バイト)を調べて表示する。
	if (stat(FILENAME, &stbuf) == -1) 
	{
		exit(1);
	}
	printf("Size: %lld\n", stbuf.st_size);

}

