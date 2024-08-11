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
//
//
//
//
//
//
/*************************˫���������ݽṹ******************************/
typedef struct _Node{
	struct _Node *preNode;
	struct _Node *nextNode;
}Node;

typedef struct _List{
	Node  headNode;
	uint32_t nodeCount;
}List;
//��ýڵ�ĸ��ṹ�ĵ�ַ
#define NodeParent(node, parent, name)  (parent *)((uint32_t)node-(uint32_t)&((parent *)0)->name)
void NodeInit(Node * node);						//�ڵ��ʼ��
void ListInit(List * list);						//�б��ʼ��
uint32_t GetListCount(List * list);				//����б��ڵĽڵ�����
Node * ListFirstNode(List * list);				//��ȡ�����һ���ڵ�
Node * ListLastNode(List * list);				//��ȡ�������һ���ڵ�
Node * ListNodePre(List * list, Node * node);	//��ȡ����ڵ��ǰһ���ڵ�
Node * ListNodeNext(List * list, Node * node);	//��ȡ����ڵ����һ���ڵ�
void ListRemoveAllNodes(List * list);			//�Ƴ��������нڵ�
void ListAddHead(List * list, Node * node);		//������ͷ������ڵ�
void ListAddTail(List * list, Node * node);		//������β������ڵ�
Node * ListRemoveFirst(List * list);			//ɾ����һ���ڵ�
void ListInsertAfter(List * list,Node * After,Node * nodeToInsert);//��ָ���Ľڵ�����ڵ�
void ListRemoveNode(List * list,Node * node);	//ɾ��������ָ���Ľڵ�
/*************************˫���������ݽṹ*****************************/


#endif

