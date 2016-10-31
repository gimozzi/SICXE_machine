//20101605 SP prj2
//prj2func.c

#include "20101605.h"

int hashFunction2(char* token){
	int key;
	int i;

	//hash function 
	for(i=0, key=0 ; i<strlen(token) ; i++){
		key += token[i];
	}
	key = key % SYMTABSIZE;

	return key;
}

int FindSYM(HT2* SYMTAB, char* token){
	ListNode4* temp;
	int count;
	int ret;
	int key;

	key = hashFunction2(token);
	temp = SYMTAB[key].head.headNode.link;
	count = SYMTAB[key].head.currentElement;

	for( ; temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(token, temp->label) == 0){
			//error

			ret = FALSE;
			return ret;
		}
	}
	if(count == 0){
		//SYMTAB에 없으므로 추가해줘야됨

		ret = TRUE;
	}

	return ret;
}

int ret_SYM_loc(HT2* SYMTAB, char* token){
	ListNode4* temp;
	int count;
	int ret;
	int key;

	key = hashFunction2(token);
	temp = SYMTAB[key].head.headNode.link;
	count = SYMTAB[key].head.currentElement;

	for( ; temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(token, temp->label) == 0){
			//error

			ret = temp->loc;
			return ret;
		}
	}
	if(count == 0){
		//SYMTAB에 없으므로 추가해줘야됨
		printf("symtab에 없다\n");
		ret = -1;
	}

	return ret;
}

int FindOPTAB(HT* OPTAB, char* token){
	ListNode3* temp;
	int count;
	int ret;
	int key;

	key = hashFunction(token);
	temp = OPTAB[key].head.headNode.link;
	count = OPTAB[key].head.currentElement;

	for( ; temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(token, temp->mnemonic) == 0){
			//OPTAB에서 찾음!

			ret = temp->format;
			return ret;
		}
	}
	if(count == 0){
		//OPTAB에 없음 
		ret = FALSE;
	}

	return ret;
}

int FindOPNUM(HT* OPTAB, char* token){
	ListNode3* temp;
	int count;
	int ret;
	int key;

	key = hashFunction(token);
	temp = OPTAB[key].head.headNode.link;
	count = OPTAB[key].head.currentElement;

	for( ; temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(token, temp->mnemonic) == 0){
			//OPTAB에서 찾음!

			ret = temp->opnum;
			return ret;
		}
	}
	if(count == 0){
		//OPTAB에 없음 
		ret = -1;
	}

	return ret;
}

int ret_OPCODE(HT* OPTAB, char* token){
	ListNode3* temp;
	int count;
	int ret;
	int key;

	key = hashFunction(token);
	temp = OPTAB[key].head.headNode.link;
	count = OPTAB[key].head.currentElement;

	for( ; temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(token, temp->mnemonic) == 0){
			//OPTAB에서 찾음!

			ret = (int)temp->opcode;
			return ret;
		}
	}
	if(count == 0){
		//OPTAB에 없음 
		ret = -1;
	}

	return ret;
}


