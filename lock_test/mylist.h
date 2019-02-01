#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long LLONG;

#define TYPE LLONG
#define ARG_TYPE LLONG

#define FREE(ptr) if(ptr) { free(ptr); ptr = NULL; }

typedef struct __MYNODE {
	struct __MYNODE *pNext;
	struct __MYNODE *pPrev;
	TYPE data;
} MyNode;

typedef struct __MYLISTC {
	//MyNode m_node;

	MyNode *m_pNodeHead;
	MyNode *m_pNodeTail;

	int m_nCount;
} MyListC;

/* --------------------------------------------------------------------------------------- */

unsigned int semaphore;

void constructKListC(MyListC *pListC); // 리스트 자원 사용을 위한 초기화
void destructKListC(MyListC *pListC); // 리스트 전체 메모리 해제

int GetCountList(MyListC *pListC); 
int IsEmptyList(MyListC *pListC); 

void AddHead(MyListC *pListC,ARG_TYPE newElement); // HEAD 에 노드 추가. HEAD 위치 반환.
void AddTail(MyListC *pListC,ARG_TYPE newElement); // TAIL 에 노드 추가. TAIL 위치 반환.

void RemoveAllList(MyListC *pListC); // 리스트 모든 요소 메모리 해제
void RemoveByIndex( MyListC *pListC, int index ); // pListC 에서 index 에 해당하는 노드 삭제
void RemoveByNode( MyListC *pListC, MyNode *pItem ); // pListC 에서 pItem 에 해당하는 노드 삭제

TYPE FindByIndex( MyListC *pListC, int index ); // pListC 에서 index 에 해당하는 노드를 찾아 반환.
int FindByNode( MyListC *pListC, MyNode *pItem); // pListC 에서 노드 pItem 에 해당하는 index 를 찾아 반환.

void AddHeadList(MyListC *pListC,MyListC *pNewList); // 또다른 리스트를 현재 리스트 head 에 추가.
void AddTailList(MyListC *pListC,MyListC *pNewList); // 또다른 리스트를 현재 리스트 tail 에 추가.

void *GetHeadAddr(MyListC *pListC); // HEAD 노드 주소 리턴
void *GetTailAddr(MyListC *pListC); // TAIL 노드 주소 리턴

TYPE GetNext(MyListC *pListC,void **rAddr); // pListC 에서 다음 요소의 노드를 rAddr 에 반환. 리턴 타입은 데이터
TYPE GetPrev(MyListC *pListC,void **rAddr); // pListC 에서 이전 요소의 노드를 rAddr 에 반환. 리턴 타입은 데이터

void AllPrintNode(MyListC *pListC);


/*
void constructKListC(MyListC *pListC); // 리스트 자원 사용을 위한 초기화
void destructKListC(MyListC *pListC); // 리스트 전체 메모리 해제

int GetCountList(MyListC *pListC); 
int IsEmptyList(MyListC *pListC); 

void *AddHead(MyListC *pListC,ARG_TYPE newElement); // HEAD 에 노드 추가. HEAD 위치 반환. why?
void *AddTail(MyListC *pListC,ARG_TYPE newElement); // TAIL 에 노드 추가. TAIL 위치 반환.

void RemoveAllList(MyListC *pListC); // 리스트 모든 요소 메모리 해제
void RemoveByIndex( MyListC *pListC, int index ); // pListC 에서 index 에 해당하는 노드 삭제
void RemoveByNode( MyListC *pListC, MyNode *pItem ); // pListC 에서 pItem 에 해당하는 노드 삭제

TYPE FindByIndex( MyListC *pListC, int index ); // pListC 에서 index 에 해당하는 노드를 찾아 반환.
int FindByNode( MyListC *pListC, MyNode *pItem); // pListC 에서 노드 pItem 에 해당하는 index 를 찾아 반환.

void AddHeadList(MyListC pListC,MyListC pNewList); // 또다른 리스트를 현재 리스트 head 에 추가.
void AddTailList(MyListC pListC,MyListC pNewList); // 또다른 리스트를 현재 리스트 tail 에 추가.

void *GetTailAddr(MyListC *pListC); // TAIL 노드 주소 리턴
TYPE GetNext(MyListC *pListC,void **rAddr); // pListC 에서 다음 요소의 노드를 rAddr 에 반환. 리턴 타입은 데이터
TYPE GetPrev(MyListC *pListC,void **rAddr); // pListC 에서 이전 요소의 노드를 rAddr 에 반환. 리턴 타입은 데이터
*/
