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

void constructKListC(MyListC *pListC); // ����Ʈ �ڿ� ����� ���� �ʱ�ȭ
void destructKListC(MyListC *pListC); // ����Ʈ ��ü �޸� ����

int GetCountList(MyListC *pListC); 
int IsEmptyList(MyListC *pListC); 

void AddHead(MyListC *pListC,ARG_TYPE newElement); // HEAD �� ��� �߰�. HEAD ��ġ ��ȯ.
void AddTail(MyListC *pListC,ARG_TYPE newElement); // TAIL �� ��� �߰�. TAIL ��ġ ��ȯ.

void RemoveAllList(MyListC *pListC); // ����Ʈ ��� ��� �޸� ����
void RemoveByIndex( MyListC *pListC, int index ); // pListC ���� index �� �ش��ϴ� ��� ����
void RemoveByNode( MyListC *pListC, MyNode *pItem ); // pListC ���� pItem �� �ش��ϴ� ��� ����

TYPE FindByIndex( MyListC *pListC, int index ); // pListC ���� index �� �ش��ϴ� ��带 ã�� ��ȯ.
int FindByNode( MyListC *pListC, MyNode *pItem); // pListC ���� ��� pItem �� �ش��ϴ� index �� ã�� ��ȯ.

void AddHeadList(MyListC *pListC,MyListC *pNewList); // �Ǵٸ� ����Ʈ�� ���� ����Ʈ head �� �߰�.
void AddTailList(MyListC *pListC,MyListC *pNewList); // �Ǵٸ� ����Ʈ�� ���� ����Ʈ tail �� �߰�.

void *GetHeadAddr(MyListC *pListC); // HEAD ��� �ּ� ����
void *GetTailAddr(MyListC *pListC); // TAIL ��� �ּ� ����

TYPE GetNext(MyListC *pListC,void **rAddr); // pListC ���� ���� ����� ��带 rAddr �� ��ȯ. ���� Ÿ���� ������
TYPE GetPrev(MyListC *pListC,void **rAddr); // pListC ���� ���� ����� ��带 rAddr �� ��ȯ. ���� Ÿ���� ������

void AllPrintNode(MyListC *pListC);


/*
void constructKListC(MyListC *pListC); // ����Ʈ �ڿ� ����� ���� �ʱ�ȭ
void destructKListC(MyListC *pListC); // ����Ʈ ��ü �޸� ����

int GetCountList(MyListC *pListC); 
int IsEmptyList(MyListC *pListC); 

void *AddHead(MyListC *pListC,ARG_TYPE newElement); // HEAD �� ��� �߰�. HEAD ��ġ ��ȯ. why?
void *AddTail(MyListC *pListC,ARG_TYPE newElement); // TAIL �� ��� �߰�. TAIL ��ġ ��ȯ.

void RemoveAllList(MyListC *pListC); // ����Ʈ ��� ��� �޸� ����
void RemoveByIndex( MyListC *pListC, int index ); // pListC ���� index �� �ش��ϴ� ��� ����
void RemoveByNode( MyListC *pListC, MyNode *pItem ); // pListC ���� pItem �� �ش��ϴ� ��� ����

TYPE FindByIndex( MyListC *pListC, int index ); // pListC ���� index �� �ش��ϴ� ��带 ã�� ��ȯ.
int FindByNode( MyListC *pListC, MyNode *pItem); // pListC ���� ��� pItem �� �ش��ϴ� index �� ã�� ��ȯ.

void AddHeadList(MyListC pListC,MyListC pNewList); // �Ǵٸ� ����Ʈ�� ���� ����Ʈ head �� �߰�.
void AddTailList(MyListC pListC,MyListC pNewList); // �Ǵٸ� ����Ʈ�� ���� ����Ʈ tail �� �߰�.

void *GetTailAddr(MyListC *pListC); // TAIL ��� �ּ� ����
TYPE GetNext(MyListC *pListC,void **rAddr); // pListC ���� ���� ����� ��带 rAddr �� ��ȯ. ���� Ÿ���� ������
TYPE GetPrev(MyListC *pListC,void **rAddr); // pListC ���� ���� ����� ��带 rAddr �� ��ȯ. ���� Ÿ���� ������
*/
