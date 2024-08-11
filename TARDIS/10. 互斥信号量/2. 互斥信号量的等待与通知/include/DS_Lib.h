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
//
//
//
//
//
//
/*************************双向链表数据结构******************************/
typedef struct _Node{
	struct _Node *preNode;
	struct _Node *nextNode;
}Node;

typedef struct _List{
	Node  headNode;
	uint32_t nodeCount;
}List;
//获得节点的父结构的地址
#define NodeParent(node, parent, name)  (parent *)((uint32_t)node-(uint32_t)&((parent *)0)->name)
void NodeInit(Node * node);						//节点初始化
void ListInit(List * list);						//列表初始化
uint32_t GetListCount(List * list);				//获得列表内的节点数量
Node * ListFirstNode(List * list);				//获取链表第一个节点
Node * ListLastNode(List * list);				//获取链表最后一个节点
Node * ListNodePre(List * list, Node * node);	//获取链表节点的前一个节点
Node * ListNodeNext(List * list, Node * node);	//获取链表节点的下一个节点
void ListRemoveAllNodes(List * list);			//移除链表所有节点
void ListAddHead(List * list, Node * node);		//在链表头部插入节点
void ListAddTail(List * list, Node * node);		//在链表尾部插入节点
Node * ListRemoveFirst(List * list);			//删除第一个节点
void ListInsertAfter(List * list,Node * After,Node * nodeToInsert);//在指定的节点后插入节点
void ListRemoveNode(List * list,Node * node);	//删除链表中指定的节点
/*************************双向链表数据结构*****************************/


#endif

