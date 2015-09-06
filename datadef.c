/*
 * datadef.c - データ定義モジュール
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "microdb.h"

/*
 * DEF_FILE_EXT -- データ定義ファイルの拡張子
 */
#define DEF_FILE_EXT ".def"

/*
 * initializeDataDefModule -- データ定義モジュールの初期化
 *
 * 引数:
 *	なし
 *
 * 返り値;
 *	成功ならOK、失敗ならNGを返す
 */
Result initializeDataDefModule()
{
	return OK;
}

/*
 * finalizeDataDefModule -- データ定義モジュールの終了処理
 *
 * 引数:
 *	なし
 *
 * 返り値;
 *	成功ならOK、失敗ならNGを返す
 */
Result finalizeDataDefModule()
{
	return OK;
}

/*
 * createTable -- 表(テーブル)の作成
 *
 * 引数:
 *	tableName: 作成する表の名前
 *	tableInfo: データ定義情報
 *
 * 返り値:
 *	成功ならOK、失敗ならNGを返す
 *
 * データ定義ファイルの構造(ファイル名: tableName.def)
 *   +-------------------+----------------------+-------------------+----
 *   |フィールド数       |フィールド名          |データ型           |
 *   |(sizeof(int)バイト)|(MAX_FIELD_NAMEバイト)|(sizeof(int)バイト)|
 *   +-------------------+----------------------+-------------------+----
 * 以降、フィールド名とデータ型が交互に続く。
 */
Result createTable(char *tableName, TableInfo *tableInfo)
{
	File *file;
	char page[PAGE_SIZE];
	char *p;
	int i;
	char *filename;
    /* [tableName].defという文字列を作る */
	getFileNameFromTableName(&filename, tableName);

    /* [tableName].defというファイルを作る */
	if (createFile(filename) != OK) 
	{
	}

    /* [tableName].defをオープンする */
	if ((file = openFile(filename)) == NULL) 
	{
	}

    /* PAGE_SIZEバイト分の大きさを持つ配列pageを初期化する */
	memset(page, 0, PAGE_SIZE);
	p = page;

    /* 配列pageの先頭にフィールド数を記録する */
	memcpy(p, &tableInfo->numField, sizeof(tableInfo->numField));
	p += sizeof(tableInfo->numField);

    /* それぞれのフィールドについて、フィールド名とデータ型をpageに記録する */
	for (i=0; i<tableInfo->numField; i++)
	{
		memcpy(p, &tableInfo->fieldInfo[i].name, sizeof(tableInfo->fieldInfo[i].name));
		p += sizeof(tableInfo->fieldInfo[i].name);
		memcpy(p, &tableInfo->fieldInfo[i].dataType, sizeof(tableInfo->fieldInfo[i].dataType));
		p += sizeof(tableInfo->fieldInfo[i].dataType);
	}

    /* 出来上がったpageをwritePageでファイル[tableName].defの0ページ目に記録する */
	if (writePage(file, 0, p) != OK)
	{
		return NG;
	}

    /* [tableName].defをクローズする */
	closeFile(file);

    /* OKを返す */
	return OK;
}

/*
 * dropTable -- 表(テーブル)の削除
 *
 * 引数:
 *	tableName
 *
 * 返り値:
 *	Result
 */
Result dropTable(char *tableName)
{
	char *filename;
	getFileNameFromTableName(&filename, tableName);
	return deleteFile(filename);
}

/*
 * getTableInfo -- 表のデータ定義情報を取得する関数
 *
 * 引数:
 *	tableName: 情報を表示する表の名前
 *
 * 返り値:
 *	tableNameのデータ定義情報を返す
 *	エラーの場合には、NULLを返す
 *
 * ***注意***
 *	この関数が返すデータ定義情報を収めたメモリ領域は、不要になったら
 *	必ずfreeTableInfoで解放すること。
 */
TableInfo *getTableInfo(char *tableName)
{
	TableInfo *tableInfo;
	File *file;
	char page[PAGE_SIZE];
	char *filename;

	getFileNameFromTableName(&filename, tableName);
	if ((file = openFile(filename)) == NULL) 
	{
	}

	memset(page, 0, PAGE_SIZE);
	readPage(file, 0, page);

	// TODO how to parse page into tableInfo? 
	

	return tableInfo;
}

/*
 * freeTableInfo -- データ定義情報を収めたメモリ領域の解放
 *
 * 引数:
 *	tableInfo
 *
 * 返り値:
 *	なし
 *
 * ***注意***
 *	関数getTableInfoが返すデータ定義情報を収めたメモリ領域は、
 *	不要になったら必ずこの関数で解放すること。
 */
void freeTableInfo(TableInfo *tableInfo) 
{
}

void getFileNameFromTableName (char **filename, char *tableName)
{
	int len;
	len = strlen(tableName) + strlen(DEF_FILE_EXT)+1;
	*filename = malloc(len);
	snprintf(*filename, len, "%s%s", tableName, DEF_FILE_EXT);
}

/*
 * printTableInfo -- テーブルのデータ定義情報を表示する(動作確認用)
 *
 * 引数:
 *	tableName: 情報を表示するテーブルの名前
 *
 * 返り値:
 *	なし
 */
void printTableInfo(char *tableName)
{
    TableInfo *tableInfo;
    int i;

    /* テーブル名を出力 */
    printf("\nTable %s\n", tableName);

    /* テーブルの定義情報を取得する */
    if ((tableInfo = getTableInfo(tableName)) == NULL) {
	/* テーブル情報の取得に失敗したので、処理をやめて返る */
	return;
    }

    /* フィールド数を出力 */
    printf("number of fields = %d\n", tableInfo->numField);

    /* フィールド情報を読み取って出力 */
    for (i = 0; i < tableInfo->numField; i++) {
	/* フィールド名の出力 */
	printf("  field %d: name = %s, ", i + 1, tableInfo->fieldInfo[i].name);

	/* データ型の出力 */
	printf("data type = ");
	switch (tableInfo->fieldInfo[i].dataType) {
	case TYPE_INTEGER:
	    printf("integer\n");
	    break;
	case TYPE_STRING:
	    printf("string\n");
	    break;
	default:
	    printf("unknown\n");
	}
    }

    /* データ定義情報を解放する */
    freeTableInfo(tableInfo);

    return;
}




