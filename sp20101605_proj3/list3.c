//sp_prj3
//list3.c

#include "20101605.h"


/* Type7 */
//ESTAB 저장 list

//T7의 Node를 생성하는 함수
ListNode7* T7_createNode(char* name, int address, int csect, int length){

	ListNode7* newNode;

	newNode = (ListNode7 *)malloc(sizeof(ListNode7));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
		strcpy(newNode -> name, name);
		newNode -> address = address;
		newNode -> csect = csect;
		newNode -> length = length;
		newNode -> link = NULL;
	}
	else{
		printf("Error: T7 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;
}
////////////////////////////

//T7의 node를 추가하는 함수
void T7_addElement(int key, ListNode7* newnode, HT3* ESTAB){
	ListNode7* pPreNode;
	ListNode7* pNewNode;

	pPreNode = &(ESTAB[key].head.headNode);
	pNewNode = newnode;

	if(ESTAB[key].head.headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;	
		pPreNode -> link = pNewNode;
		ESTAB[key].head.currentElement++;
	}
	else{
		while(pPreNode -> link != NULL){
			pPreNode = pPreNode -> link;
		}
		pPreNode -> link = pNewNode;
		ESTAB[key].head.currentElement++;
	}
}


//ESTAB을 display
void T7_display(HT3 ht){
	ListNode7* temp;
	int total_length=0;

	temp = ht.head.headNode.link;
	printf("control\tsymbol\taddress\tlength\n");
	printf("section\tname\n");
	printf("------------------------------------------------\n");
	for(;temp != NULL ; temp = temp->link){
		if(temp->csect){
			printf("%7s\t      \t%X\t%X\n",temp->name, temp->address, temp->length );
		}
		else{
			printf("       \t%6s\t%X\n",temp->name, temp->address);
		}
	}
	printf("------------------------------------------------\n");
	printf("\t\t\t\ttotal length\t%d\n",total_length);
}


//T7 list를 삭제하는 함수
void T7_removeElement(HT3* ESTAB){
	ListNode7* preNode;
	ListNode7* delNode;
 	int i = 0;
	for(i=0 ; i<ESTABSIZE ; i++){
		if(ESTAB[i].head.headNode.link != NULL){
			preNode = ESTAB[i].head.headNode.link;
			while(1){
				delNode = preNode;
				preNode = preNode -> link;
				free(delNode);

				if(preNode == NULL)
					break;
			}
			ESTAB[i].head.headNode.link = NULL;
			ESTAB[i].head.currentElement = 0;
		}
	}
}


////////////////////////////////////////////////////////
//bp list
/* Type8 */
//명령어 저장 list

//T8의 Head Node를 생성하는 함수
LinkedList8* T8_createList(){

	LinkedList8 *pList;

	pList = (LinkedList8 *)malloc(sizeof(LinkedList8));

	if(pList != NULL){
		pList -> currentElement = 0;
		pList -> headNode.link = NULL;
	}
	else{
		printf("Error: T8 Headnode memory allocation\n");
		return NULL;
	}

	return pList;
}

//T8의 Node를 생성하는 함수
ListNode8* T8_createNode(int address){
	ListNode8* newNode;
	newNode = (ListNode8 *)malloc(sizeof(ListNode8));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
//		strcpy(newNode -> comm_data, token);
		newNode->address = address;
		newNode -> link = NULL;
	}
	else{
		printf("Error: T8 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;

}
//T8의 node를 추가하는 함수
//삽입정렬로 합니다
void T8_addElement(LinkedList8* pList, ListNode8* newnode){
	ListNode8* pPreNode;
	ListNode8* pNewNode;
	ListNode8* temp;
	pPreNode = &(pList -> headNode);
	pNewNode = newnode;
	int i;
	int terminal = 0;

//	printf("T8add\n");
	
	if(pList -> headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;
		pPreNode -> link = pNewNode;
		pList->currentElement++;
//		printf("T8 add 0\n");
//		printf("000 newnode [%d]\n", pNewNode->address);
	}
	else{	
		temp = pPreNode;
		pPreNode = pPreNode -> link;
		for(i=0 ;i<pList->currentElement ; i++){
//				printf("[%d]preadd[%d]____newadd[%d]\n",i, pPreNode->address, pNewNode->address);
			if(pPreNode->address > pNewNode->address /*|| pPreNode == NULL*/){
				break;
			}
			if(pPreNode->link == NULL){
				terminal = 1;
				break;
			}
			temp = pPreNode;
			pPreNode = pPreNode -> link;
			
		}
	
		if(/*i==pList->currentElement*/ terminal  ){
			//terminal 에 붙임
//				printf("preadd[%d][%X]____newadd[%d][%X]\n", pPreNode->address, pPreNode->address, pNewNode->address, pNewNode->address);
			pPreNode -> link = pNewNode;
//		printf("[%d]T8 add 3[currnet%d]\n",i, pList->currentElement);
		}
		else if(i==0){
			//head 다음에 붙임
//				printf("preadd[%d][%X]____newadd[%d]\n", pPreNode->address,pPreNode->address, pNewNode->address);
			pNewNode->link = pList->headNode.link;
			pList->headNode.link = pNewNode;
//		printf("[%d]T8 add 1[currend%d]\n",i, pList->currentElement);
		}
		else{
			//중간에 붙임
//				printf("preadd[%d]____newadd[%d]\n", pPreNode->address, pNewNode->address);
			pNewNode->link = temp->link;
			temp->link = pNewNode;
//		printf("[%d]T8 add 2[current%d]\n",i, pList->currentElement);
		}

		pList->currentElement++;
	}

}
//T8의 node를 삭제하는 함수
void T8_removeElement(LinkedList8* pList){
	ListNode8* preNode;
	ListNode8* delNode;
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
//T8의 node를 삭제하는 함수(head 포함)
void T8_removeElement2(LinkedList8* pList){
	ListNode8* preNode;
	ListNode8* delNode;
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
	free(pList);
}

//T8 list display
void T8_display(LinkedList8* pList){
	ListNode8* temp;
	temp = pList -> headNode.link;
	printf("\t\tbreakpoint\n");
	printf("\t\t----------\n");
	for(;temp != NULL ; temp = temp->link){
		printf("\t\t%X\n", temp -> address);
	}
}


///////////////////////////////////////////////////////////
/* Type9 */
//T record list
//T9 list display
void T9_display(LinkedList9* pList){
	ListNode9* temp;
	temp = pList -> headNode.link;
	printf("\t\tT_record\n");
	printf("\t\t----------\n");
	for(;temp != NULL ; temp = temp->link){
		printf("[%6X][%6X]\n", temp -> address, temp -> length);
	}
}

//T9의 Head Node를 생성하는 함수
LinkedList9* T9_createList(){

	LinkedList9 *pList;

	pList = (LinkedList9 *)malloc(sizeof(LinkedList9));

	if(pList != NULL){
		pList -> currentElement = 0;
		pList -> headNode.link = NULL;
	}
	else{
		printf("Error: T9 Headnode memory allocation\n");
		return NULL;
	}

	return pList;
}

//T9의 Node를 생성하는 함수
ListNode9* T9_createNode(int address, int length){
	ListNode9* newNode;
	newNode = (ListNode9 *)malloc(sizeof(ListNode9));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
//		strcpy(newNode -> comm_data, token);
		newNode->address =  address;
		newNode->length = length;
		newNode -> link = NULL;
	}
	else{
		printf("Error: T9 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;

}


//T9의 node를 추가하는 함수
void T9_addElement(LinkedList9* pList, ListNode9* newnode){
	ListNode9* pPreNode;
	ListNode9* pNewNode;
	pPreNode = &(pList -> headNode);
	pNewNode = newnode;

	if(pList -> headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;
		pPreNode -> link = pNewNode;
		pList->currentElement++;
	}
	else{
		while(pPreNode -> link != NULL){
			pPreNode = pPreNode -> link;
		}
		pPreNode -> link = pNewNode;
		pList->currentElement++;
	}

}

//T9의 node를 삭제하는 함수
void T9_removeElement(LinkedList9* pList){
	ListNode9* preNode;
	ListNode9* delNode;
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
//T9의 node를 삭제하는 함수(head 포함)
void T9_removeElement2(LinkedList9* pList){
	ListNode9* preNode;
	ListNode9* delNode;
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
	free(pList);
}

