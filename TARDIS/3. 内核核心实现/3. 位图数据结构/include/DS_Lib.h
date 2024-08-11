#ifndef DS_LIB_H
#define DS_LIB_H

#include <stdint.h>

/*************************λͼ���ݽṹ******************************/
typedef struct{
	uint32_t bitmap;
}Bitmap;

void BitmapInit(Bitmap * bmap);					//��ʼ��
uint32_t GetBitmapPosCount(void);				//���λͼ֧�ֵı�־λ����(δʹ��)
void BitmapSet(Bitmap * bmap,uint32_t pos);   	//����λͼĳһλ
void BitmapClear(Bitmap * bmap,uint32_t pos);	//���λͼĳһλ
uint32_t GetBitmapFirstSet(Bitmap * bmap);		//���λͼ��һ�����õ�λ��
/*************************λͼ���ݽṹ*****************************/

#endif

