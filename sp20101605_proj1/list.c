//20101605 SP prj1
//list.c

//linked list 함수가 정의됨

#include "20101605.h"

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/* Type1 */
//명령어 저장 list

//T1의 Head Node를 생성하는 함수
LinkedList1* T1_createList(){

	LinkedList1 *pList;

	pList = (LinkedList1 *)malloc(sizeof(LinkedList1));

	if(pList != NULL){
		pList -> currentElement = 0;
		pList -> headNode.link = NULL;
	}
	else{
		printf("Error: T1 Headnode memory allocation\n");
		return NULL;
	}

	return pList;
}

//T1의 Node를 생성하는 함수
ListNode1* T1_createNode(char* token){
	ListNode1* newNode;
	newNode = (ListNode1 *)malloc(sizeof(ListNode1));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
		strcpy(newNode -> comm_data, token);
		newNode -> link = NULL;
	}
	else{
		printf("Error: T1 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;

}

//T1의 node를 추가하는 함수
void T1_addElement(LinkedList1* pList, ListNode1* newnode){
	ListNode1* pPreNode;
	ListNode1* pNewNode;
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

//T1의 node를 삭제하는 함수
void T1_removeElement(LinkedList1* pList){
	ListNode1* preNode;
	ListNode1* delNode;
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
//T1의 node를 삭제하는 함수(head 포함)
void T1_removeElement2(LinkedList1* pList){
	ListNode1* preNode;
	ListNode1* delNode;
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
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/* Type2 */
//History 저장 list

//T2의 Head Node를 생성하는 함수
LinkedList2* T2_createList(){

	LinkedList2 *pList;

	pList = (LinkedList2 *)malloc(sizeof(LinkedList2));

	if(pList != NULL){
		pList -> headNode.link = NULL;
	}
	else{
		printf("Error: T2 Headnode memory allocation\n");
		return NULL;
	}

	return pList;
}

//T2의 Node를 생성하는 함수
ListNode2* T2_createNode(char* token, int cnt){
//	printf("createnode\n");
	ListNode2* newNode;

	newNode = (ListNode2 *)malloc(sizeof(ListNode2));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
		strcpy(newNode -> comm_data, token);
		newNode -> order = cnt;
		newNode -> link = NULL;
	}
	else{
		printf("Error: T2 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;
}

//T2의 node를 추가하는 함수
void T2_addElement(LinkedList2* pList, ListNode2* newnode){
	ListNode2* pPreNode;
	ListNode2* pNewNode;
	pPreNode = &(pList -> headNode);
	pNewNode = newnode;

	if(pList -> headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;
		pPreNode -> link = pNewNode;
	}
	else{
		while(pPreNode -> link != NULL){
			pPreNode = pPreNode -> link;
		}
		pPreNode -> link = pNewNode;
	}
}

//T2 list를 삭제하는 함수
void T2_removeElement(LinkedList2* pList){
	ListNode2* preNode;
	ListNode2* delNode;
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
	}
	free(pList);
}
	
//T2 list display(history 저장됨)
void T2_display(LinkedList2* pList){
	ListNode2* temp;
	temp = pList -> headNode.link;
	for(;temp != NULL ; temp = temp->link){
		printf("%-4d%s\n", temp -> order,temp -> comm_data);
	}
}






///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/* Type3 */
//opcode table 저장 list

//T3의 Node를 생성하는 함수
ListNode3* T3_createNode(unsigned char opcode,char* mnemonic, char* format){

	ListNode3* newNode;

	newNode = (ListNode3 *)malloc(sizeof(ListNode3));
	if(newNode != NULL){
		//strcpy 로 명령어 token을 저장해줌
		newNode -> opcode = opcode;
		strcpy(newNode -> mnemonic, mnemonic);
		strcpy(newNode -> format, format);
		newNode -> link = NULL;
	}
	else{
		printf("Error: T3 Node memory allocation\n");
		return NULL;
	}
	
	return newNode;
}
////////////////////////////

//T3의 node를 추가하는 함수
void T3_addElement(int key, ListNode3* newnode){
	ListNode3* pPreNode;
	ListNode3* pNewNode;

	pPreNode = &(hashtable[key].head.headNode);
	pNewNode = newnode;

	if(hashtable[key].head.headNode.link == NULL){
		//Head node의 link 가 Null을 가리킬때
		pNewNode -> link = pPreNode -> link;	
		pPreNode -> link = pNewNode;
		hashtable[key].head.currentElement++;
	}
	else{
		while(pPreNode -> link != NULL){
			pPreNode = pPreNode -> link;
		}
		pPreNode -> link = pNewNode;
		hashtable[key].head.currentElement++;
	}
}

//opcode table을 display
void T3_display(HT ht){
	ListNode3* temp;

	temp = ht.head.headNode.link;
	
	for(;temp != NULL ; temp = temp->link){
		printf(" [%s,%X]",temp->mnemonic, temp->opcode);
		if(temp->link != NULL)
			printf(" ->");
	}
}

//opcode mnemonic 명령어 입력시 opcode에 해당하는 mnemonic display
void display_mnemonic(int key, char* mnemonic){
	ListNode3* temp;
	int count;

	temp = hashtable[key].head.headNode.link;
	count = hashtable[key].head.currentElement;
	for(;temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(mnemonic, temp->mnemonic) == 0){
			printf("opcode is %X\n", temp->opcode);
			return;
		}
	}
	if(count == 0){
		error(3);
	}
}

//T3 list를 삭제하는 함수
void T3_removeElement(){
	ListNode3* preNode;
	ListNode3* delNode;
 	int i = 0;
	for(i=0 ; i<TABLESIZE ; i++){
		if(hashtable[i].head.headNode.link != NULL){
			preNode = hashtable[i].head.headNode.link;
			while(1){
				delNode = preNode;
				preNode = preNode -> link;
				free(delNode);

				if(preNode == NULL)
					break;
			}
			hashtable[i].head.headNode.link = NULL;
		}
	}
}
