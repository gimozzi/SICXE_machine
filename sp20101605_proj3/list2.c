//20101605 SP prj2
//list2.c

#include "20101605.h"


/* Type4 */
//symbol table 저장 list

//T4의 Node를 생성하는 함수
ListNode4* T4_createNode(char* label, int loc, int flag){

	ListNode4* newNode;

	newNode = (ListNode4 *)malloc(sizeof(ListNode4));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
		strcpy(newNode -> label, label);
		newNode -> loc = loc;
		newNode -> flag = flag;
		newNode -> link = NULL;
	}
	else{
		printf("Error: T4 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;
}
////////////////////////////

//T4의 node를 추가하는 함수
void T4_addElement(int key, ListNode4* newnode, HT2* SYMTAB){
	ListNode4* pPreNode;
	ListNode4* pNewNode;

	pPreNode = &(SYMTAB[key].head.headNode);
	pNewNode = newnode;

	if(SYMTAB[key].head.headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;	
		pPreNode -> link = pNewNode;
		SYMTAB[key].head.currentElement++;
	}
	else{
		while(pPreNode -> link != NULL){
			pPreNode = pPreNode -> link;
		}
		pPreNode -> link = pNewNode;
		SYMTAB[key].head.currentElement++;
	}
}

//symbol table을 display
void T4_display(HT2 ht){
	ListNode4* temp;

	temp = ht.head.headNode.link;
	
	for(;temp != NULL ; temp = temp->link){
//		printf(" [%s,%X]",temp->mnemonic, temp->opcode);
		if(temp->link != NULL)
			printf(" ->");
	}
}


//T4 list를 삭제하는 함수
void T4_removeElement(HT2* SYMTAB){
	ListNode4* preNode;
	ListNode4* delNode;
 	int i = 0;
	for(i=0 ; i<SYMTABSIZE ; i++){
		if(SYMTAB[i].head.headNode.link != NULL){
			preNode = SYMTAB[i].head.headNode.link;
			while(1){
				delNode = preNode;
				preNode = preNode -> link;
				free(delNode);

				if(preNode == NULL)
					break;
			}
			SYMTAB[i].head.headNode.link = NULL;
			SYMTAB[i].head.currentElement = 0;
		}
	}
}

///////////////////////////////////////////////////////////
/* Type5 */
//intermediate file list


//T5의 Head Node를 생성하는 함수
LinkedList5* T5_createList(){

	LinkedList5 *pList;

	pList = (LinkedList5 *)malloc(sizeof(LinkedList5));

	if(pList != NULL){
//		pList -> currentElement = 0;
		pList -> headNode.link = NULL;
	}
	else{
		printf("Error: T5 Headnode memory allocation\n");
		return NULL;
	}

	return pList;
}


//T5의 Node를 생성하는 함수
ListNode5* T5_createNode(int line, int loc, /*char* loc,*/ char* label, char* inst, char* operand, char* objectcode, char* comment, int commentchk, int PC){
	ListNode5* newNode;
	newNode = (ListNode5 *)malloc(sizeof(ListNode5));
	if(newNode != NULL){
		newNode -> line = line;
		newNode -> loc = loc;
		strcpy(newNode -> label, label);
		strcpy(newNode -> inst, inst);
		strcpy(newNode -> operand, operand);
		strcpy(newNode -> objectcode, objectcode);
		strcpy(newNode -> comment, comment);
		newNode -> len = 0;
		newNode -> commentchk = commentchk;
		newNode -> PC = PC;

		newNode -> link = NULL;
	}
	else{
		printf("Error: T5 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;

}

//T5의 node를 추가하는 함수
void T5_addElement(LinkedList5* pList, ListNode5* newnode){
	ListNode5* pPreNode;
	ListNode5* pNewNode;
	pPreNode = &(pList -> headNode);
	pNewNode = newnode;

	if(pList -> headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;
		pPreNode -> link = pNewNode;
//		pList->currentElement++;
	}
	else{
		while(pPreNode -> link != NULL){
			pPreNode = pPreNode -> link;
		}
		pPreNode -> link = pNewNode;
//		pList->currentElement++;
	}

}

//T5의 node를 삭제하는 함수
void T5_removeElement(LinkedList5* pList){
	ListNode5* preNode;
	ListNode5* delNode;
 	int i = 0;
	if(pList -> headNode.link != NULL){
		preNode = pList->headNode.link;

		while(1){
			delNode = preNode;
			preNode = preNode -> link;
			free(delNode);

			if(preNode == NULL)
				break;
		}
		pList->headNode.link = NULL;
//		pList->currentElement = 0;
	}
}

//T5의 node를 삭제하는 함수(head 포함)
void T5_removeElement2(LinkedList5* pList){
	ListNode5* preNode;
	ListNode5* delNode;
 	int i = 0;
	if(pList -> headNode.link != NULL){
		preNode = pList->headNode.link;

		while(1){
			delNode = preNode;
			preNode = preNode -> link;
			free(delNode);

			if(preNode == NULL)
				break;
		}
		pList->headNode.link = NULL;
//		pList->currentElement = 0;
	}
	free(pList);
}

///////////////////////////////////////////////////////////
//Type6
//정렬된 SYMTAB 저장

//T6의 Head Node를 생성하는 함수
LinkedList6* T6_createList(){

	LinkedList6 *pList;

	pList = (LinkedList6 *)malloc(sizeof(LinkedList6));

	if(pList != NULL){
		pList -> currentElement = 0;
		pList -> headNode.link = NULL;
	}
	else{
		printf("Error: T6 Headnode memory allocation\n");
		return NULL;
	}

	return pList;
}
//T6의 Node를 생성하는 함수
ListNode6* T6_createNode(char* label, int loc){

	ListNode6* newNode;

	newNode = (ListNode6 *)malloc(sizeof(ListNode6));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
		strcpy(newNode -> label, label);
		newNode -> loc = loc;
		newNode -> link = NULL;
	}
	else{
		printf("Error: T6 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;
}
////////////////////////////
//T6의 node를 추가하는 함수
void T6_addElement(LinkedList6* SORT, ListNode6* newnode){
	ListNode6* pPreNode;
	ListNode6* pNewNode;
	int current=0;
	int chk;

	pPreNode = &(SORT -> headNode);
	pNewNode = newnode;

	if(SORT -> headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;	
		pPreNode -> link = pNewNode;
		SORT->currentElement++;
	}
	else{//내림차순 정렬해서 삽입
		chk=0;
		while(pPreNode -> link != NULL){
//		printf("[cu:%d][%d]pre[%s]new[%s]\n",current,strcmp(pPreNode->link->label, pNewNode->label),pPreNode->link->label, pNewNode->label);
			current++;
			if(strcmp(pPreNode->link->label,pNewNode->label) > 0){
				chk = 1;
				if(current == 1){
					chk = 2;
				}
				break;
			}
			pPreNode = pPreNode -> link;

		}
		if(current == SORT->currentElement && chk == 0){
			pPreNode -> link = pNewNode;
			SORT->currentElement++;
		}
		else if(chk == 1){//preNode 이전
			pNewNode->link = pPreNode->link;
			pPreNode->link = pNewNode;
			SORT->currentElement++;
		}
		else if(chk == 2){//첫번째 node 이전
			pNewNode->link = SORT -> headNode.link;
			SORT -> headNode.link = pNewNode;
			SORT->currentElement++;
		}
	}
}
//symbol table을 display
void T6_display(LinkedList6* SORT){
	ListNode6* temp;

	temp = SORT->headNode.link;
	if(temp == NULL){
		printf("There is no SYMTAB\n");
		return;
	}
		
	for(;temp != NULL ; temp = temp->link){
		printf("\t%s\t%04X\n",temp->label, temp->loc);
	}
}
//T6의 node를 삭제하는 함수
void T6_removeElement(LinkedList6* pList){
	ListNode6* preNode;
	ListNode6* delNode;
 	int i = 0;
	if(pList -> headNode.link != NULL){
		preNode = pList->headNode.link;

		while(1){
			delNode = preNode;
			preNode = preNode -> link;
			free(delNode);

			if(preNode == NULL)
				break;
		}
		pList->headNode.link = NULL;
		pList->currentElement = 0;
	}
}

//T6의 node를 삭제하는 함수(head 포함)
void T6_removeElement2(LinkedList6* pList){
	ListNode6* preNode;
	ListNode6* delNode;
 	int i = 0;
	if(pList -> headNode.link != NULL){
		preNode = pList->headNode.link;

		while(1){
			delNode = preNode;
			preNode = preNode -> link;
			free(delNode);

			if(preNode == NULL)
				break;
		}
		pList->headNode.link = NULL;
//		pList->currentElement = 0;
	}
	free(pList);
}
