#include "DS_Lib.h"

#define firstNode headNode.nextNode				//获取链表的第一个节点
#define lastNode  headNode.preNode				//获取链表的最后一个节点

void NodeInit(Node * node){						//节点初始化
	node->preNode = node;
	node->nextNode = node;
}


void ListInit(List * list){						//列表初始化
	list->firstNode = &(list->headNode);
	list->lastNode  = &(list->headNode);
	list->nodeCount = 0;
}

uint32_t GetListCount(List * list){				//获得列表内的节点数量
	return list->nodeCount;
}

Node * ListFirstNode(List * list){				//获取链表第一个节点
	return (list->nodeCount!=0)?(list->firstNode):((Node *)0);
}

Node * ListLastNode(List * list){				//获取链表最后一个节点
	return (list->nodeCount!=0)?(list->lastNode):((Node *)0);
}

Node * ListNodePre(List * list, Node * node){	//获取链表节点的前一个节点
	return (node->preNode!=node)?(node->preNode):((Node *)0);
}

Node * ListNodeNext(List * list, Node * node){	//获取链表节点的下一个节点
	return (node->nextNode!=node)?(node->nextNode):((Node *)0);
}

void ListRemoveAllNodes(List * list){			//移除链表所有节点
	uint32_t count;
	Node * nextNode;
	
	nextNode = list->firstNode;
	for(count = list->nodeCount; count!=0 ;count--){
		Node * currentNode = nextNode;
		nextNode = nextNode->nextNode;
		
		currentNode->preNode  = currentNode;
		currentNode->nextNode = currentNode;
	}
	list->firstNode = &(list->headNode);
	list->lastNode  = &(list->headNode);
	list->nodeCount = 0;
}

void ListAddHead(List * list, Node * node){	//在链表头部插入节点
	node->preNode  = &(list->headNode);
	node->nextNode = list->firstNode;
	
	list->firstNode->preNode = node;
	list->firstNode = node;
	
	list->nodeCount++;
}

void ListAddTail(List * list, Node * node){	//在链表尾部插入节点
	node->nextNode  = &(list->headNode);
	node->preNode = list->lastNode;
	
	list->lastNode->nextNode = node;
	list->lastNode = node;
	
	list->nodeCount++;
}

Node * ListRemoveFirst(List * list){			//删除第一个节点
	Node * node = (Node *)0;
	if(list->nodeCount != 0){
		  
		node = list->firstNode;
		
		node->nextNode->preNode = &(list->headNode);
		list->firstNode = node->nextNode;
		
		list->nodeCount--;
	}
	return node;
}

void ListInsertAfter(List * list,Node * After,Node * nodeToInsert){	//在指定的节点后插入节点
	nodeToInsert->preNode = After;
	nodeToInsert->nextNode = After->nextNode;
	
	nodeToInsert->nextNode->preNode = nodeToInsert;
	After->nextNode = nodeToInsert;
	
	list->nodeCount++;
}

void ListRemoveNode(List * list,Node * node){	//删除链表中指定的节点
	node->preNode->nextNode = node->nextNode;
	node->nextNode->preNode = node->preNode;
	list->nodeCount--;
}


