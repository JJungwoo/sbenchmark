#include "mylist.h"

void constructKListC(MyListC *pListC) {		// 리스트 구조체 초기화 과정.
	if(NULL != pListC->m_pNodeHead) {		// 리스트 헤더가 NULL이 아니면 이미 리스트가 존재함.
		puts("List is exist");
		return;
	}
	pListC->m_pNodeHead = (MyNode *)malloc(sizeof(MyNode));
	pListC->m_pNodeTail = (MyNode *)malloc(sizeof(MyNode));
	pListC->m_pNodeHead->data = -1;
	pListC->m_pNodeTail->data = -1;

	pListC->m_pNodeHead->pNext = pListC->m_pNodeTail;	// Head와 Tail 초기 설정 
	pListC->m_pNodeTail->pPrev = pListC->m_pNodeHead;
	pListC->m_pNodeHead->pPrev = NULL;
	pListC->m_pNodeTail->pNext = NULL;
	pListC->m_nCount = 0;					// 리스트에 연결된 노드의 개수를 나타냄.
}

void destructKListC(MyListC *pListC) {
	if(NULL == pListC) {
		puts("List isn't initialization...");
		return;
	} else if(pListC->m_pNodeHead->pNext == pListC->m_pNodeTail &&	// 리스트가 초기설정 상태인지 확인
		pListC->m_pNodeTail->pPrev == pListC->m_pNodeHead && 
		0 == pListC->m_nCount) {	 // 리스트에 count를 통해 다른 노드가 남아있는지 확인하고 메모리해제 한다.
		FREE(pListC->m_pNodeHead);
		FREE(pListC->m_pNodeTail);
		FREE(pListC);
	} else
		puts("List isn't empty error!!!\n");
}

int GetCountList(MyListC *pListC) {
	if(NULL == pListC || 0 == pListC->m_nCount) {	// 리스트가 존재하는지 예외처리와 노드가 하나도 없을 때
		puts("List is Empty...");
		return 0;
	}
	return pListC->m_nCount;		// 현재 리스트에 저장된 노드 카운트 반환
}

int IsEmptyList(MyListC *pListC) {	// 0은 현재 리스트 NULL, 1은 현재 리스트에 데이터 존재.
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

		NewNode->data = newElement;		// 새로운 노드에 데이터 저장.
		pListC->m_nCount += 1;			// 리스트에 노드 추가되어서 카운트 증가시킴.
	}
}

void RemoveAllList(MyListC *pListC) {  // 리스트 모든 노드 메모리 해제
	MyNode *curHNode = NULL;
	MyNode *tmpNode = NULL;
	int i = 0, total = 0;

	curHNode = pListC->m_pNodeHead->pNext;	// 헤더에서부터 전체 노드를 순차적으로 순회함
	total = pListC->m_nCount;
	for( i = 0 ; i < total ; i++) {			// 리스트 전체 노드 개수를 통해 순회 시작
		tmpNode = curHNode->pNext;
		FREE(curHNode);
		curHNode = tmpNode;
		pListC->m_nCount -= 1;
	}
	// 이후 리스트를 다시 쓰기 위해 헤더와 테일의 상태를 처음과 같이 설정
	pListC->m_pNodeHead->pNext = pListC->m_pNodeTail;	
	pListC->m_pNodeTail->pPrev = pListC->m_pNodeHead;
}

void RemoveByIndex( MyListC *pListC, int index ) { // pListC 에서 index 에 해당하는 노드 삭제
	MyNode *curHNode = NULL;	// 삭제할 노드 위치 저장
	MyNode *tmpNode = NULL;		
	int i = 0, ch = 0;

	// 입력 받은 index 값이 정상인지, 현재 리스트의 데이터보다 큰 값인지 체크
	if(0 > index || pListC->m_nCount < index){		
		puts("index error...");
		return;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {	// 리스트에 데이터가 존재하는지 체크
		puts("List is Empty...");
		return;
	}
	curHNode = pListC->m_pNodeHead->pNext;
	for(i = 0 ; i < index ; i++) {		
		tmpNode = curHNode->pNext;
		curHNode = tmpNode;
	}
	//	tmpNode에 삭제할 curHNode 다음 노드 위치를 저장해서 
	//	curHNode의 양쪽 노드를 연결할 때 사용함.
	tmpNode = curHNode->pNext;	

	tmpNode->pPrev = curHNode->pPrev;
	curHNode->pPrev->pNext = tmpNode;

	// curHNode 는 삭제할 노드이기 때문에 양방향 포인터를 NULL로 초기화
	curHNode->pPrev = NULL;		
	curHNode->pNext = NULL;
	FREE(curHNode);
	pListC->m_nCount -= 1;
}

void RemoveByNode( MyListC *pListC, MyNode *pItem ) { // pListC 에서 pItem 에 해당하는 노드 삭제 
	MyNode *curHNode = NULL;
	MyNode *tmpNode = NULL;
	int i = 0;
	
	if(NULL == pItem) {			// 인자로 받은 pItem 변수값이 올바른지 체크
		puts("pItem error...");
		return;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {
		puts("List is Empty...");
		return;
	}
	curHNode = pListC->m_pNodeHead->pNext;
	while(pItem != curHNode) {
		// 찾는 노드가 Tail전까지 없다면 리스트에 없는 노드이기 때문에 함수 종료.
		if(pListC->m_pNodeTail == curHNode->pNext){		
			puts("Node isn't find...");
			return;
		}
		tmpNode = curHNode->pNext;
		curHNode = tmpNode;
	}
	//	tmpNode에 삭제할 curHNode 다음 노드 위치를 저장해서 
	//	curHNode의 양쪽 노드를 연결할 때 사용함.
	tmpNode = curHNode->pNext;

	tmpNode->pPrev = curHNode->pPrev;
	curHNode->pPrev->pNext = tmpNode;

	// curHNode 는 삭제할 노드이기 때문에 양방향 포인터를 NULL로 초기화
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

	// index 값이 현재 리스트의 count보다 더 크면 overflow error 발생
	if(index > pListC->m_nCount){				
		puts("index overflow error...");
		return -1;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {	// 리스트에 값이 있는지 확인
		puts("List is Empty...");
		return -1;
	} else {
		//printf("TEST!!!! index: %d, count: %d\n",index,pListC->m_nCount);
		// 찾으려는 index가 중간보다 작을 때, Header에서부터 찾음.
		if((((pListC->m_nCount - 1)/2) - index) >= 0) {	// 카운터에 -1은 count는 1부터 카운팅되기 때문
			curHNode = pListC->m_pNodeHead->pNext;
			for(i = 0 ; i < index ; i++) {		// 리스트의 헤더 다음 노드가 바로 0번째 카운팅
				puts("head");
				tmpNode = curHNode->pNext;
				curHNode = tmpNode;
			}
			return curHNode->data;
		} else {	// 찾으려는 index가 중간보다 크면 Tail, 즉 뒤에서 부터 찾음.
			curTNode = pListC->m_pNodeTail->pPrev;
			// 리스트의 테일 이전 노드가 바로 0번째 카운팅
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

	if(NULL == pItem) {			// 인자로 받은 pItem 변수값이 올바른지 체크
		puts("pItem error...");
		return -1;
	}
	if(NULL == pListC || 0 == pListC->m_nCount) {	// 리스트에 데이터 존재 확인
		puts("List is Empty...");
		return -1;
	} else { 
		curHNode = pListC->m_pNodeHead;
		while(pItem != curHNode) {		// 찾는 노드와 현재 노드가 같을 때까지 반복
			// 찾는 노드가 Tail전까지 없다면 리스트에 없는 노드이기 때문에 함수 종료.
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

void AddHeadList(MyListC *pListC, MyListC *pNewList) { // 또다른 리스트를 현재 리스트 head 에 추가.
	if((NULL == pListC || NULL == pNewList)) {
		puts("List isn't initialization...");
		return;
	} else {
		pNewList->m_pNodeTail->pPrev->pNext = pListC->m_pNodeHead->pNext;
		pListC->m_pNodeHead->pNext->pPrev = pNewList->m_pNodeTail->pPrev;
		pListC->m_pNodeHead->pNext = pNewList->m_pNodeHead->pNext;
		pNewList->m_pNodeHead->pNext->pPrev = pListC->m_pNodeHead;
		pListC->m_nCount += pNewList->m_nCount;

		pNewList->m_pNodeHead->pNext = pNewList->m_pNodeTail;	//pNewList를 초기설정으로 원복시킴
		pNewList->m_pNodeTail->pPrev = pNewList->m_pNodeHead;
	}
}

void AddTailList(MyListC *pListC, MyListC *pNewList) { // 또다른 리스트를 현재 리스트 tail 에 추가.
	if(NULL == pListC || NULL == pNewList) {
		puts("List isn't initialization...");
		return;
	} else {
		pNewList->m_pNodeHead->pNext->pPrev = pListC->m_pNodeTail->pPrev;
		pListC->m_pNodeTail->pPrev->pNext = pNewList->m_pNodeHead->pNext;
		pNewList->m_pNodeTail->pPrev->pNext = pListC->m_pNodeTail;
		pListC->m_pNodeTail->pPrev = pNewList->m_pNodeTail->pPrev;
		pListC->m_nCount += pNewList->m_nCount;
		
		pNewList->m_pNodeHead->pNext = pNewList->m_pNodeTail;	//pNewList를 초기설정으로 원복시킴
		pNewList->m_pNodeTail->pPrev = pNewList->m_pNodeHead;
	}
}

void *GetHeadAddr(MyListC *pListC) {  // HEAD 노드 주소 리턴
	if(NULL == pListC) {
		puts("List isn't initialization...");
		return NULL;
	} else 
		return pListC->m_pNodeHead;
}

void *GetTailAddr(MyListC *pListC) { // TAIL 노드 주소 리턴
	if(NULL == pListC) {
		puts("List isn't initialization...");
		return NULL;
	} else 
		return pListC->m_pNodeTail;
}

// rAddr이 가리키는 노드의 데이터를 반환하고 rAddr은 현재 노드의 다음 노드 주소를 담아준다.
TYPE GetNext(MyListC *pListC,void **rAddr) { // pListC 에서 다음 요소의 노드를 rAddr 에 반환. 리턴 타입은 데이터
   MyNode *curHNode = NULL;
   MyNode *tmpNode = NULL;
   int result = 0;

   if (NULL == *rAddr) {      // 입력받은 주소가 NULL인지 확인
      puts("input Address error...");
      return -1;
   }
   if (NULL == pListC || 0 == pListC->m_nCount) {   // 리스트에 데이터가 존재하는지 확인
      puts("List is Empty...");
      return -1;
   }
   else {
      curHNode = pListC->m_pNodeHead->pNext;
      while (*rAddr != curHNode) {      // 찾는 노드와 현재 노드가 같을 때까지 반복
         // Tail전의 노드까지 반복했을 때, 찾는 노드가 없을 경우 리스트에 없기 때문에 에러 발생
         if (pListC->m_pNodeTail == curHNode->pNext) {
            puts("Node isn't find...");
            return -1;
         }
         tmpNode = curHNode->pNext;
         curHNode = tmpNode;
      }
      *rAddr = curHNode->pNext;   // 찾은 노드 뒤의 노드 주소를 반환
      result = curHNode->data;   // 찾은 노드 데이터 결과 값 반환
   }
   return result;
}

// rAddr이 가리키는 노드의 데이터를 반환하고 rAddr은 현재 노드의 이전 노드 주소를 담아준다.
TYPE GetPrev(MyListC *pListC,void **rAddr) { // pListC 에서 이전 요소의 노드를 rAddr 에 반환. 리턴 타입은 데이터
   MyNode *curHNode = NULL;
   MyNode *tmpNode = NULL;
   int result = 0;

   if (NULL == *rAddr) {      // 입력받은 주소가 NULL인지 확인
      puts("input Address error...");
      return -1;
   }
   if (NULL == pListC || 0 == pListC->m_nCount) {   // 리스트에 데이터가 존재하는지 확인
      puts("List is Empty...");
      return -1;
   }
   else {
      curHNode = pListC->m_pNodeTail->pPrev;
      while (*rAddr != curHNode) {      // 찾는 노드와 현재 노드가 같을 때까지 반복
         // Head전의 노드까지 반복했을 때, 찾는 노드가 없을 경우 리스트에 없기 때문에 에러 발생
         if (pListC->m_pNodeHead == curHNode->pPrev) {
            puts("Node isn't find...");
            return -1;
         }
         tmpNode = curHNode->pPrev;
         curHNode = tmpNode;
      }
      *rAddr = curHNode->pPrev;   // 찾은 노드 앞의 노드 주소를 반환
      result = curHNode->data;   // 찾은 노드 데이터 결과 값 반환
   }
   return result;
}

void AllPrintNode(MyListC *pListC) {	// 현재 리스트에 존재하는 모든 데이터 출력
	MyNode *curHNode = NULL;
	MyNode *tmpNode = NULL;
	int i = 0;
	if(NULL == pListC || 0 == pListC->m_nCount) {	// 리스트에 데이터가 있는지 확인
		puts("List is Empty...");
		return;
	}
	curHNode = pListC->m_pNodeHead->pNext;			// 리스트 헤더에서부터 순차적 순회
	for( i = 0 ; i < pListC->m_nCount ; i++) {		// 리스트 카운트까지 반복
		printf("%d count node data: %d\n",i,curHNode->data);
		tmpNode = curHNode->pNext;
		curHNode = tmpNode;
	}
}

