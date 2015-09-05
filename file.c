/*
 * file.c -- ファイルアクセスモジュール 
 */
#include <stdio.h>
#include <stdlib.h>
#include "microdb.h"

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * initializeFileModule -- ファイルアクセスモジュールの初期化処理
 *
 * 引数:
 *	なし
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result initializeFileModule()
{
    return OK;
}

/*
 * finalizeFileModule -- ファイルアクセスモジュールの終了処理
 *
 * 引数:
 *	なし
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result finalizeFileModule()
{
    return OK;
}

/*
 * createFile -- ファイルの作成
 *
 * 引数:
 *	filename: 作成するファイルのファイル名
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result createFile(char *filename)
{
	if ((creat(filename, S_IREAD | S_IWRITE)) == -1)
	{
		return NG;
	}
	return OK;
}

/*
 * deleteFile -- ファイルの削除
 *
 * 引数:
 *	name
 *
 * 返り値:
 * OK/NG
 */
Result deleteFile(char *filename)
{
	if (unlink(filename) == -1) 
	{
		return NG;
	}
	return OK;
}

/*
 * openFile -- ファイルのオープン
 *
 * 引数:
 *	filename
 *
 * 返り値:
 *	オープンしたファイルのFile構造体
 *	オープンに失敗した場合にはNULLを返す
 */
File *openFile(char *filename)
{
	File *file;
	file = malloc(sizeof(File));
	if (file == NULL) 
	{
		return NULL;
	}

	int desc;
	if ((desc = open(filename, O_RDWR)) == -1) 
	{
		return NULL;
	}

	file->desc = desc;
	strcpy(file->name, filename); 

	return file;
}

/*
 * closeFile -- ファイルのクローズ
 *
 * 引数:
 *	クローズするファイルのFile構造体
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result closeFile(File *file)
{
	if (close(file->desc) == -1) 
	{
		return NG;
	}
	free(file);
	return OK;
}

/*
 * readPage -- 1ページ分のデータのファイルからの読み出し
 *
 * 引数:
 *	file: アクセスするファイルのFile構造体
 *	pageNum: 読み出すページの番号
 *	page: 読み出した内容を格納するPAGE_SIZEバイトの領域
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result readPage(File *file, int pageNum, char *page)
{
	if (lseek(file->desc, (pageNum * PAGE_SIZE), SEEK_SET) == -1)
	{
		return NG;
	}
	if (read(file->desc, page, PAGE_SIZE) < PAGE_SIZE) 
	{
		return NG;
	}
	return OK;
}

/*
 * writePage -- 1ページ分のデータのファイルへの書き出し
 *
 * 引数:
 *	file: アクセスするファイルのFile構造体
 *	pageNum: 書き出すページの番号
 *	page: 書き出す内容を格納するPAGE_SIZEバイトの領域
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result writePage(File *file, int pageNum, char *page)
{
	if (lseek(file->desc, (pageNum * PAGE_SIZE), SEEK_SET) == -1)
	{
		return NG;
	}
	if (write(file->desc, page, strlen(page)) == -1) 
	{
		return NG;
	}
	return OK;
}

/*
 * getNumPage -- ファイルのページ数の取得
 *
 * 引数:
 *	filename: ファイル名
 *
 * 返り値:
 *	引数で指定されたファイルの大きさ(ページ数)
 *	エラーの場合には-1を返す
 */
int getNumPages(char *filename)
{
	struct stat stbuf;
	if (stat(filename, &stbuf) == -1) 
	{
		return -1;
	}
	return (stbuf.st_size / PAGE_SIZE);
}



