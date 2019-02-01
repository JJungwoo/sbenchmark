#include "mylist.h"

void constructKListC(MyListC *pListC) {		// ����Ʈ ����ü �ʱ�ȭ ����.
	if(NULL != pListC->m_pNodeHead) {		// ����Ʈ ����� NULL�� �ƴϸ� �̹� ����Ʈ�� ������.
		puts("List is exist");
		return;
	}
	pListC->m_pNodeHead = (MyNode *)malloc(sizeof(MyNode));
	pListC->m_pNodeTail = (MyNode *)malloc(sizeof(MyNode));
	pListC->m_pNodeHead->data = -1;
	pListC->m_pNodeTail->data = -1;

	pListC->m_pNodeHead->pNext = pListC->m_pNodeTail;	// Head�� Tail �ʱ� ���� 
	pListC->m_pNodeTail->pPrev = pListC->m_pNodeHead;
	pListC->m_pNodeHead->pPrev = NULL;
	pListC->m_pNodeTail->pNext = NULL;
	pListC->m_nCount = 0;					// ����Ʈ�� ����� ����� ������ ��Ÿ��.
}

void destructKListC(MyListC *pListC) {
	if(NULL == pListC) {
		puts("List isn't initialization...");
		return;
	} else if(pListC->m_pNodeHead->pNext == pListC->m_pNodeTail &&	// ����Ʈ�� �ʱ⼳�� �������� Ȯ��
		pListC->m_pNodeTail->pPrev == pListC->m_pNodeHead && 
		0 == pListC->m_nCount) {	 // ����Ʈ�� count�� ���� �ٸ� ��尡 �����ִ��� Ȯ���ϰ� �޸����� �Ѵ�.
		FREE(pListC->m_pNodeHead);
		FREE(pListC->m_pNodeTail);
		FREE(pListC);
	} else
		puts("List isn't empty error!!!\n");
}

int GetCountList(MyListC *pListC) {
	if(NULL == pListC || 0 == pListC->m_nCount) {	// ����Ʈ�� �����ϴ��� ����ó���� ��尡 �ϳ��� ���� ��
		puts("List is Empty...");
		return 0;
	}
	return pListC->m_nCount;		// ���� ����Ʈ�� ����� ��� ī��Ʈ ��ȯ
}

int IsEmptyList(MyListC *pListC) {	// 0�� ���� ����Ʈ NULL, 1�� ���� ����Ʈ�� ������ ����.
	if(NULL != pListC && 0 < pListC->m_nCount) 
		return 1;
	return 0;
}

void AddHead(MyListC *pListC,ARG_TYPE newElement) {
	MyNode* NewNode = NULL;
	if(NULL == pListC) {
		puts("List isn't initialization...");
		return;
	} else {
		NewNode = (MyNode *)malloc(sizeof(MyNode));

		NewNode->pNext = pListC->m_pNodeHead->pNext;
		pListC->m_pNodeHead->pNext->pPrev = NewNode;
		pListC->m_pNodeHead->pNext = NewNode;
		NewNode->pPrev = pListC->m_pNodeHead;

		NewNode->data = newElement;

		pListC->m_nCount += 1;
	}
}

void AddTail(MyListC *pListC,ARG_TYPE newElement) {
	MyNode* NewNode = NULL;
	if(NULL == pListC) {
		puts("List is Empty...");
		return;
	} else {
		NewNode = (MyNode *)malloc(sizeof(MyNode));

		NewNode->pPrev = pListC->m_pNodeTail->pPrev;
		pListC->m_pNodeTail->pPrev->pNext = NewNode;
		pListC->m_pNodeTail->pPrev = NewNode;
		NewNode->pNext = pListC->m_pNodeTail;

		NewNode->data = newElement;		// ���ο� ��忡 ������ ����.
		pListC->m_nCount += 1;			// ����Ʈ�� ��� �߰��Ǿ ī��Ʈ ������Ŵ.
	}
}

void RemoveAllList(MyListC *pListC) {  // ����Ʈ ��� ��� �޸� ����
	MyNode *curHNode = NULL;
	MyNode *tmpNode = NULL;
	int i = 0, total = 0;

	curHNode = pListC->m_pNodeHead->pNext;	// ����������� ��ü ��带 ���������� ��ȸ��
	total = pListC->m_nCount;
	for( i = 0 ; i < total ; i++) {			// ����Ʈ ��ü ��� ������ ���� ��ȸ ����
		tmpNode = curHNode->pNext;
		FREE(curHNode);
		curHNode = tmpNode;
		pListC->m_nCount -= 1;
	}
	// ���� ����Ʈ�� �ٽ� ���� ���� ����� ������ ���¸� ó���� ���� ����
	pListC->m_pNodeHead->pNext = pListC->m_pNodeTail;	
	pListC->m_pNodeTail->pPrev = pListC->m_pNodeHead;
}

void RemoveByIndex( MyListC *pListC, int index ) { // pListC ���� index �� �ش��ϴ� ��� ����
	MyNode *curHNode = NULL;	// ������ ��� ��ġ ����
	MyNode *tmpNode = NULL;		
	int i = 0, ch = 0;

	// �Է� ���� index ���� ��������, ���� ����Ʈ�� �����ͺ��� ū ������ üũ
	if(0 > index || pListC->m_nCount < index){		
		puts("index error...");
		return;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {	// ����Ʈ�� �����Ͱ� �����ϴ��� üũ
		puts("List is Empty...");
		return;
	}
	curHNode = pListC->m_pNodeHead->pNext;
	for(i = 0 ; i < index ; i++) {		
		tmpNode = curHNode->pNext;
		curHNode = tmpNode;
	}
	//	tmpNode�� ������ curHNode ���� ��� ��ġ�� �����ؼ� 
	//	curHNode�� ���� ��带 ������ �� �����.
	tmpNode = curHNode->pNext;	

	tmpNode->pPrev = curHNode->pPrev;
	curHNode->pPrev->pNext = tmpNode;

	// curHNode �� ������ ����̱� ������ ����� �����͸� NULL�� �ʱ�ȭ
	curHNode->pPrev = NULL;		
	curHNode->pNext = NULL;
	FREE(curHNode);
	pListC->m_nCount -= 1;
}

void RemoveByNode( MyListC *pListC, MyNode *pItem ) { // pListC ���� pItem �� �ش��ϴ� ��� ���� 
	MyNode *curHNode = NULL;
	MyNode *tmpNode = NULL;
	int i = 0;
	
	if(NULL == pItem) {			// ���ڷ� ���� pItem �������� �ùٸ��� üũ
		puts("pItem error...");
		return;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {
		puts("List is Empty...");
		return;
	}
	curHNode = pListC->m_pNodeHead->pNext;
	while(pItem != curHNode) {
		// ã�� ��尡 Tail������ ���ٸ� ����Ʈ�� ���� ����̱� ������ �Լ� ����.
		if(pListC->m_pNodeTail == curHNode->pNext){		
			puts("Node isn't find...");
			return;
		}
		tmpNode = curHNode->pNext;
		curHNode = tmpNode;
	}
	//	tmpNode�� ������ curHNode ���� ��� ��ġ�� �����ؼ� 
	//	curHNode�� ���� ��带 ������ �� �����.
	tmpNode = curHNode->pNext;

	tmpNode->pPrev = curHNode->pPrev;
	curHNode->pPrev->pNext = tmpNode;

	// curHNode �� ������ ����̱� ������ ����� �����͸� NULL�� �ʱ�ȭ
	curHNode->pPrev = NULL;
	curHNode->pNext = NULL;
	FREE(curHNode);
	pListC->m_nCount -= 1;
}

		

TYPE FindByIndex( MyListC *pListC, int index) {
	MyNode *curHNode = NULL;
	MyNode *curTNode = NULL;
	MyNode *tmpNode = NULL;
	int i = 0;

	// index ���� ���� ����Ʈ�� count���� �� ũ�� overflow error �߻�
	if(index > pListC->m_nCount){				
		puts("index overflow error...");
		return -1;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {	// ����Ʈ�� ���� �ִ��� Ȯ��
		puts("List is Empty...");
		return -1;
	} else {
		//printf("TEST!!!! index: %d, count: %d\n",index,pListC->m_nCount);
		// ã������ index�� �߰����� ���� ��, Header�������� ã��.
		if((((pListC->m_nCount - 1)/2) - index) >= 0) {	// ī���Ϳ� -1�� count�� 1���� ī���õǱ� ����
			curHNode = pListC->m_pNodeHead->pNext;
			for(i = 0 ; i < index ; i++) {		// ����Ʈ�� ��� ���� ��尡 �ٷ� 0��° ī����
				puts("head");
				tmpNode = curHNode->pNext;
				curHNode = tmpNode;
			}
			return curHNode->data;
		} else {	// ã������ index�� �߰����� ũ�� Tail, �� �ڿ��� ���� ã��.
			curTNode = pListC->m_pNodeTail->pPrev;
			// ����Ʈ�� ���� ���� ��尡 �ٷ� 0��° ī����
			for(i = 0 ; i < ((pListC->m_nCount - 1) - index) ; i++) {	
				puts("tail");
				tmpNode = curTNode->pPrev; 
				curTNode = tmpNode;
			}
			return curTNode->data;
		}
	}
}

int FindByNode( MyListC *pListC, MyNode *pItem) {
	MyNode *curHNode = NULL;
	MyNode *tmpNode = NULL;
	int index = 0;

	if(NULL == pItem) {			// ���ڷ� ���� pItem �������� �ùٸ��� üũ
		puts("pItem error...");
		return -1;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {	// ����Ʈ�� ������ ���� Ȯ��
		puts("List is Empty...");
		return -1;
	} else { 
		curHNode = pListC->m_pNodeHead;
		while(pItem != curHNode) {		// ã�� ���� ���� ��尡 ���� ������ �ݺ�
			// ã�� ��尡 Tail������ ���ٸ� ����Ʈ�� ���� ����̱� ������ �Լ� ����.
			if(pListC->m_pNodeTail == curHNode->pNext){		
				puts("Node isn't find...");
				return -1;
			}
			tmpNode = curHNode->pNext;
			curHNode = tmpNode;
			index += 1;
		}
	}
	return index;
}

void AddHeadList(MyListC *pListC, MyListC *pNewList) { // �Ǵٸ� ����Ʈ�� ���� ����Ʈ head �� �߰�.
	if((NULL == pListC || NULL == pNewList)) {
		puts("List isn't initialization...");
		return;
	} else {
		pNewList->m_pNodeTail->pPrev->pNext = pListC->m_pNodeHead->pNext;
		pListC->m_pNodeHead->pNext->pPrev = pNewList->m_pNodeTail->pPrev;
		pListC->m_pNodeHead->pNext = pNewList->m_pNodeHead->pNext;
		pNewList->m_pNodeHead->pNext->pPrev = pListC->m_pNodeHead;
		pListC->m_nCount += pNewList->m_nCount;

		pNewList->m_pNodeHead->pNext = pNewList->m_pNodeTail;	//pNewList�� �ʱ⼳������ ������Ŵ
		pNewList->m_pNodeTail->pPrev = pNewList->m_pNodeHead;
	}
}

void AddTailList(MyListC *pListC, MyListC *pNewList) { // �Ǵٸ� ����Ʈ�� ���� ����Ʈ tail �� �߰�.
	if(NULL == pListC || NULL == pNewList) {
		puts("List isn't initialization...");
		return;
	} else {
		pNewList->m_pNodeHead->pNext->pPrev = pListC->m_pNodeTail->pPrev;
		pListC->m_pNodeTail->pPrev->pNext = pNewList->m_pNodeHead->pNext;
		pNewList->m_pNodeTail->pPrev->pNext = pListC->m_pNodeTail;
		pListC->m_pNodeTail->pPrev = pNewList->m_pNodeTail->pPrev;
		pListC->m_nCount += pNewList->m_nCount;
		
		pNewList->m_pNodeHead->pNext = pNewList->m_pNodeTail;	//pNewList�� �ʱ⼳������ ������Ŵ
		pNewList->m_pNodeTail->pPrev = pNewList->m_pNodeHead;
	}
}

void *GetHeadAddr(MyListC *pListC) {  // HEAD ��� �ּ� ����
	if(NULL == pListC) {
		puts("List isn't initialization...");
		return NULL;
	} else 
		return pListC->m_pNodeHead;
}

void *GetTailAddr(MyListC *pListC) { // TAIL ��� �ּ� ����
	if(NULL == pListC) {
		puts("List isn't initialization...");
		return NULL;
	} else 
		return pListC->m_pNodeTail;
}

// rAddr�� ����Ű�� ����� �����͸� ��ȯ�ϰ� rAddr�� ���� ����� ���� ��� �ּҸ� ����ش�.
TYPE GetNext(MyListC *pListC,void **rAddr) { // pListC ���� ���� ����� ��带 rAddr �� ��ȯ. ���� Ÿ���� ������
   MyNode *curHNode = NULL;
   MyNode *tmpNode = NULL;
   int result = 0;

   if (NULL == *rAddr) {      // �Է¹��� �ּҰ� NULL���� Ȯ��
      puts("input Address error...");
      return -1;
   }
   if (NULL == pListC || 0 == pListC->m_nCount) {   // ����Ʈ�� �����Ͱ� �����ϴ��� Ȯ��
      puts("List is Empty...");
      return -1;
   }
   else {
      curHNode = pListC->m_pNodeHead->pNext;
      while (*rAddr != curHNode) {      // ã�� ���� ���� ��尡 ���� ������ �ݺ�
         // Tail���� ������ �ݺ����� ��, ã�� ��尡 ���� ��� ����Ʈ�� ���� ������ ���� �߻�
         if (pListC->m_pNodeTail == curHNode->pNext) {
            puts("Node isn't find...");
            return -1;
         }
         tmpNode = curHNode->pNext;
         curHNode = tmpNode;
      }
      *rAddr = curHNode->pNext;   // ã�� ��� ���� ��� �ּҸ� ��ȯ
      result = curHNode->data;   // ã�� ��� ������ ��� �� ��ȯ
   }
   return result;
}

// rAddr�� ����Ű�� ����� �����͸� ��ȯ�ϰ� rAddr�� ���� ����� ���� ��� �ּҸ� ����ش�.
TYPE GetPrev(MyListC *pListC,void **rAddr) { // pListC ���� ���� ����� ��带 rAddr �� ��ȯ. ���� Ÿ���� ������
   MyNode *curHNode = NULL;
   MyNode *tmpNode = NULL;
   int result = 0;

   if (NULL == *rAddr) {      // �Է¹��� �ּҰ� NULL���� Ȯ��
      puts("input Address error...");
      return -1;
   }
   if (NULL == pListC || 0 == pListC->m_nCount) {   // ����Ʈ�� �����Ͱ� �����ϴ��� Ȯ��
      puts("List is Empty...");
      return -1;
   }
   else {
      curHNode = pListC->m_pNodeTail->pPrev;
      while (*rAddr != curHNode) {      // ã�� ���� ���� ��尡 ���� ������ �ݺ�
         // Head���� ������ �ݺ����� ��, ã�� ��尡 ���� ��� ����Ʈ�� ���� ������ ���� �߻�
         if (pListC->m_pNodeHead == curHNode->pPrev) {
            puts("Node isn't find...");
            return -1;
         }
         tmpNode = curHNode->pPrev;
         curHNode = tmpNode;
      }
      *rAddr = curHNode->pPrev;   // ã�� ��� ���� ��� �ּҸ� ��ȯ
      result = curHNode->data;   // ã�� ��� ������ ��� �� ��ȯ
   }
   return result;
}

void AllPrintNode(MyListC *pListC) {	// ���� ����Ʈ�� �����ϴ� ��� ������ ���
	MyNode *curHNode = NULL;
	MyNode *tmpNode = NULL;
	int i = 0;
	if(NULL == pListC || 0 == pListC->m_nCount) {	// ����Ʈ�� �����Ͱ� �ִ��� Ȯ��
		puts("List is Empty...");
		return;
	}
	curHNode = pListC->m_pNodeHead->pNext;			// ����Ʈ ����������� ������ ��ȸ
	for( i = 0 ; i < pListC->m_nCount ; i++) {		// ����Ʈ ī��Ʈ���� �ݺ�
		printf("%d count node data: %d\n",i,curHNode->data);
		tmpNode = curHNode->pNext;
		curHNode = tmpNode;
	}
}

