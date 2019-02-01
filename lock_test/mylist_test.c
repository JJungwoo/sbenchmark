
#include "mylist.h"

//-1 8 7 -1  6 5 4 3 1
int main()
{
	MyListC *mylist = (MyListC *)malloc(sizeof(MyListC)); 
	MyListC *youlist = (MyListC *)malloc(sizeof(MyListC));
	MyNode* h_tmp = NULL, *t_tmp = NULL;	
	int index = 5;
	int count = 0;
	MyNode *tmpNode = (MyNode *)malloc(sizeof(MyNode));
	int result = 0;

	//memset을 통해 리스트의 모든 값을 NULL로 초기화
	memset(mylist, 0, sizeof(MyNode));		
	memset(youlist, 0, sizeof(MyNode));
	memset(tmpNode, 0, sizeof(MyNode));
		
	constructKListC(mylist);
	constructKListC(youlist);

	if(0 == IsEmptyList(mylist))
		puts("List is Empty...\n");
	else
		puts("List is exist..\n");

	if(NULL == mylist)
		printf("%d\n",mylist->m_nCount);

	printf("Get Count List: %d\n",GetCountList(mylist));

	puts("mlist--------------");
	AddTail(mylist, 1);
	AddTail(mylist, 2);
	AddTail(mylist, 3);
	AddTail(mylist, 4);
	AddTail(mylist, 5);
	AddTail(mylist, 6);
	AllPrintNode(mylist);
	tmpNode = mylist->m_pNodeTail->pPrev;
	result = GetPrev(mylist, &tmpNode);
	printf("Get Prev result: %d, PrevNode: %d\n"
		, result, tmpNode->data);
	puts("-------------------");
	
	//tmpNode = mylist->m_pNodeHead->pNext;
	//result = GetNext(mylist, &tmpNode);
	

	
	
	printf("Get Count List: %d\n",GetCountList(mylist));
	
	//printf("find node data: %d\n", 
	//FindByNode(mylist, mylist->m_pNodeHead->pNext));	
	//printf("Remove %d index ---\n",index);
	//RemoveByIndex(mylist, index);
	//puts("Remove by Head next node -------------------");
	//RemoveByNode(mylist, mylist->m_pNodeHead->pNext);
	//printf("find index: %d, counting data: %d\n"
	//	,index, FindByIndex(mylist, index));	
	
	/*
	puts("ylist--------------");
	AddTail(youlist, 8);
	AddTail(youlist, 7);
	AllPrintNode(youlist);
	puts("-------------------");
	AddTailList(mylist, youlist);
	AllPrintNode(mylist);
	
	printf("Get Count List: %d\n",GetCountList(mylist));
	printf("%d counting data: %d\n",count, FindByIndex(mylist, count));

	h_tmp = (MyNode *)GetHeadAddr(mylist);
	t_tmp = (MyNode *)GetTailAddr(mylist);

	printf("Test Get func H: %d, T: %d\n\n", h_tmp->data, t_tmp->data);

	//AddHeadList(mylist, youlist);
	*/
	
	AllPrintNode(mylist);
	puts("--------------------");
	RemoveAllList(mylist);
	//destructKListC(mylist);
	puts("--------------------");
	AllPrintNode(mylist);
	puts("!!!!!!!!!!!---------");
	AddHead(mylist, 4);
	AddHead(mylist, 5);
	AddHead(mylist, 6);

	printf("%d\n",mylist->m_nCount);
	AllPrintNode(mylist);
	RemoveAllList(mylist);
	puts("============");
	AllPrintNode(mylist);
	destructKListC(mylist);

	return 0;
}
