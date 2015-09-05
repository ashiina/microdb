/*
 * ファイルアクセス関連システムコール(低レベルI/O)を使った例題プログラム
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* テストに使うファイル名とファイルの大きさ */
#define FILENAME "testfile"
#define FILE_SIZE 256

/* テストに使う読み書きの位置と読み出しの長さ */
#define OVERWRITE_POINT 156
#define READ_POINT (OVERWRITE_POINT - 4)
#define READ_LENGTH 10

int main(int argc, char **argv)
{
    int fd;
    char buffer[FILE_SIZE];
    struct stat stbuf;

    /* ファイルが存在するかどうかを確認 */
    if (access(FILENAME, F_OK) == 0) {
        /* ファイルを削除 */
        if (unlink(FILENAME) == -1) {
            exit(1);
        }
    }

    /*
     * ファイルの作成
     *
     * ■■解説■■
     * creatシステムコールの第2引数には、作成するファイルに対する
     * アクセス権を指定します。以下では、ファイルを作った本人に
     * 読み出す権利(S_IRUSR)と書き込む権利(S_IWUSR)を与えています。
     */
    if ((fd = creat(FILENAME, S_IRUSR | S_IWUSR)) == -1) {
        exit(1);
    }

    /*
     * ファイルのクローズ
     *
     * ■■解説■■
     * 実は、creatシステムコールは、ファイルの作成とオープンを同時に
     * 行うシステムコールです。したがって、作成したファイルをすぐに
     * 使うのであれば、creatの返すファイルディスクリプタ(fd)をそのまま
     * 使っても構いません。ここでは、openシステムコールの使い方を示す
     * ために、わざといったんクローズしています。
     */
    if (close(fd) == -1) {
        exit(1);
    }

    /* ファイルのオープン */
    if ((fd = open(FILENAME, O_RDWR)) == -1) {
        exit(1);
    }

    /* ファイルに書き込むデータの作成 */
    memset(buffer, 'X', FILE_SIZE);

    /* ファイルへの書き込み */
    if (write(fd, buffer, FILE_SIZE) == -1) {
        exit(1);
    }

    /* アクセス位置の変更 */
    if (lseek(fd, OVERWRITE_POINT, SEEK_SET) == -1) {
        exit(1);
    }

    /* データの上書き */
    if (write(fd, "test", strlen("test")) == -1) {
        exit(1);
    }

    /* アクセス位置の変更 */
    if (lseek(fd, READ_POINT, SEEK_SET) == -1) {
        exit(1);
    }

    /* 読み出しのために配列をリセット */
    memset(buffer, 0, FILE_SIZE);

    /* ファイルからのデータの読み出し */
    if (read(fd, buffer, READ_LENGTH) == -1) {
        exit(1);
    }

    /* 読み出した内容を画面に出力 */
    printf("Data: %s\n", buffer);

    /* ファイルの大きさのチェック */
    if (stat(FILENAME, &stbuf) == -1) {
        exit(1);
    }

    /* ファイルのサイズを画面に出力 */
    printf("File size: %lld\n", stbuf.st_size);

    exit(0);
}


