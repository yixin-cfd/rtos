#include "DS_Lib.h"
//初始化
void BitmapInit(Bitmap * bmap)					
{
	bmap->bitmap = 0;
}
//获得位图支持的标志位数量(未使用)
uint32_t GetBitmapPosCount(void)				
{
	return 32;
}
//设置位图某一位
void BitmapSet(Bitmap * bmap,uint32_t pos)   	
{
	bmap->bitmap |= (1<<pos);
}
//清除位图某一位
void BitmapClear(Bitmap * bmap,uint32_t pos)	
{
	bmap->bitmap &= ~(1<<pos);
}
//获得位图第一个设置的位置
uint32_t GetBitmapFirstSet(Bitmap * bmap)		
{
	static const uint8_t quickFindTable[] =
	{
		/* 00 */ 0xff, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 10 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 20 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 30 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 40 */ 6,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 50 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 60 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 70 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 80 */ 7,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* 90 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* A0 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* B0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* C0 */ 6,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* D0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* E0 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		/* F0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
	};
	if(bmap->bitmap == 0){
		return GetBitmapPosCount();
	}
	if(bmap->bitmap & 0xFF){
		return quickFindTable[bmap->bitmap & 0xFF];
	}
	else if(bmap->bitmap & 0xFF00){
		return quickFindTable[(bmap->bitmap >> 8) & 0xFF] + 8;
	}
	else if(bmap->bitmap & 0xFF0000){
		return quickFindTable[(bmap->bitmap >> 16) & 0xFF] + 16;
	}
	else{
		return quickFindTable[(bmap->bitmap >> 24) & 0xFF] + 24;
	}
}





