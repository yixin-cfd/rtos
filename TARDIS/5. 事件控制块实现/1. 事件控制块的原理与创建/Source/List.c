#include "DS_Lib.h"

#define firstNode headNode.nextNode				//��ȡ����ĵ�һ���ڵ�
#define lastNode  headNode.preNode				//��ȡ��������һ���ڵ�

void NodeInit(Node * node){						//�ڵ��ʼ��
	node->preNode = node;
	node->nextNode = node;
}


void ListInit(List * list){						//�б��ʼ��
	list->firstNode = &(list->headNode);
	list->lastNode  = &(list->headNode);
	list->nodeCount = 0;
}

uint32_t GetListCount(List * list){				//����б��ڵĽڵ�����
	return list->nodeCount;
}

Node * ListFirstNode(List * list){				//��ȡ�����һ���ڵ�
	return (list->nodeCount!=0)?(list->firstNode):((Node *)0);
}

Node * ListLastNode(List * list){				//��ȡ�������һ���ڵ�
	return (list->nodeCount!=0)?(list->lastNode):((Node *)0);
}

Node * ListNodePre(List * list, Node * node){	//��ȡ����ڵ��ǰһ���ڵ�
	return (node->preNode!=node)?(node->preNode):((Node *)0);
}

Node * ListNodeNext(List * list, Node * node){	//��ȡ����ڵ����һ���ڵ�
	return (node->nextNode!=node)?(node->nextNode):((Node *)0);
}

void ListRemoveAllNodes(List * list){			//�Ƴ��������нڵ�
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

void ListAddHead(List * list, Node * node){	//������ͷ������ڵ�
	node->preNode  = &(list->headNode);
	node->nextNode = list->firstNode;
	
	list->firstNode->preNode = node;
	list->firstNode = node;
	
	list->nodeCount++;
}

void ListAddTail(List * list, Node * node){	//������β������ڵ�
	node->nextNode  = &(list->headNode);
	node->preNode = list->lastNode;
	
	list->lastNode->nextNode = node;
	list->lastNode = node;
	
	list->nodeCount++;
}

Node * ListRemoveFirst(List * list){			//ɾ����һ���ڵ�
	Node * node = (Node *)0;
	if(list->nodeCount != 0){
		  
		node = list->firstNode;
		
		node->nextNode->preNode = &(list->headNode);
		list->firstNode = node->nextNode;
		
		list->nodeCount--;
	}
	return node;
}

void ListInsertAfter(List * list,Node * After,Node * nodeToInsert){	//��ָ���Ľڵ�����ڵ�
	nodeToInsert->preNode = After;
	nodeToInsert->nextNode = After->nextNode;
	
	nodeToInsert->nextNode->preNode = nodeToInsert;
	After->nextNode = nodeToInsert;
	
	list->nodeCount++;
}

void ListRemoveNode(List * list,Node * node){	//ɾ��������ָ���Ľڵ�
	node->preNode->nextNode = node->nextNode;
	node->nextNode->preNode = node->preNode;
	list->nodeCount--;
}


