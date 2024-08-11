#ifndef DS_LIB_H
#define DS_LIB_H

#include <stdint.h>

/*************************位图数据结构******************************/
typedef struct{
	uint32_t bitmap;
}Bitmap;

void BitmapInit(Bitmap * bmap);					//初始化
uint32_t GetBitmapPosCount(void);				//获得位图支持的标志位数量(未使用)
void BitmapSet(Bitmap * bmap,uint32_t pos);   	//设置位图某一位
void BitmapClear(Bitmap * bmap,uint32_t pos);	//清除位图某一位
uint32_t GetBitmapFirstSet(Bitmap * bmap);		//获得位图第一个设置的位置
/*************************位图数据结构*****************************/

#endif

