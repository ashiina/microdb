#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEF_FILE_EXT ".def"

#define N 256
void getFileName(char **str) {
    char name[] = "チャーリー・ブラウン";
    char sex[] = "男性";
    int age = 4;
	*str = malloc(N);
//	*str = "HELLO!";
    snprintf(*str, N, "%sさんは%d歳の%sです...", name, age, sex);
}

void getFileNameFromTableName (char **filename, char *tableName)
{
	int len;
	len = strlen(tableName) + strlen(DEF_FILE_EXT)+1;
	*filename = malloc(len);
	snprintf(*filename, len, "%s%s", tableName, DEF_FILE_EXT);
}

int main () {
	char tableName[] = "testTable";
	char *filename;
	getFileNameFromTableName(&filename, tableName);

	printf("f:%s\n", filename);
	printf("done!\n");
    return 0;
}



