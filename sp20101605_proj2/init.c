#include "20101605.h"


//memory 할당 함수
VM* memoryinit(){
	VM* newVM;
	int i,j;

	//memory 할당
	newVM = (VM *)malloc(MEM_ROW * sizeof(VM));
	
	if(newVM == NULL)
		printf("할당실패\n");
	for(i=0 ; i< MEM_ROW ; i++){
		for(j=0; j<MEM_COL; j++){			
			newVM[i].memory[j]=0x0;	//메모리 값을 초기화(Hex)
		}
	}
	return newVM;
}

//hash table init
void htinit(HT* hashtable){
	int i;

	//hash table 값을 초기화 함
	for(i=0 ; i<TABLESIZE ; i++){
		hashtable[i].head.currentElement = 0;
		hashtable[i].head.headNode.link = NULL;
	}
}

//opcode.txt 를 읽고 hash table 생성
void makeHashtable(HT* hashtable){
	char read[81];			//한줄씩 읽은 문장을 저장
	char token_op[80];		//opcode
	char token_mne[80];		//mnemonic
	char token_form[80];	//format
	int form;
	char *token;			//strtok로 끊은 token 저장
	char *delimiters = " \t";//strtok delimiter
	int i, key;				//key: hash table key
	////////////////////////////////////////
	unsigned char op;		//hex로 변환된 opcode 저장
	int hex, len, hi;		//입력받은 opcode -> hex 위한 변수
	ListNode3* newNode;		//opcode table node
	int opnum;				//operand 개수
	char opnum2[6][7] = {
		"ADDR", "COMPR", "DIVR", "MULR", "RMO", "SUBR"
	};
	char opnum0[7][7] = {
		"FIX", "FLOAT", "HIO", "NORM", "RSUB", "SIO", "TIO"
	};

	//opcode.txt 스트림을 연다
	FILE *fopcode = fopen("opcode.txt", "r");
	
	while(fgets(read, 81, fopcode) != NULL){//한줄씩 읽음
		//enter까지 읽으므로 마지막은 EOS로 바꿔줌
		read[strlen(read)-1] = '\0';
		//delimiter를 기준으로 token을 나눔
		token = strtok(read, delimiters); 
		strcpy(token_op, token);
		token = strtok(NULL, delimiters);
		strcpy(token_mne, token);
		token = strtok(NULL, delimiters);
		strcpy(token_form, token);
		if(token_form[0] == '1')
			form = 1;
		else if(token_form[0] == '2')
			form = 2;
		else
			form = 3;
		//hash function call
		key = hashFunction(token_mne);

		//////////////////
		//hash Table 생성
		//T3 node 생성
		//16진수 -> 10진수로 바꿈 (opcode)
		len = strlen(token_op);
		for(hi = len-1, hex = 1, op = 0 ; hi >= 0 ; hi--, hex*=16){
			if(token_op[hi] >= 48 && token_op[hi] <= 57){
				op += (token_op[hi] - 48) * hex;
			}
			else if(token_op[hi] >= 65 && token_op[hi] <= 70){
				op += (token_op[hi] - 55) * hex;
			}
			else if(token_op[hi] >= 97 && token_op[hi] <= 102){
				op += (token_op[hi] - 87) * hex;
			}
		}
		opnum = 1;
		for(i=0 ; i<6; i++){
			if(strcmp(token_mne, opnum2[i]) == 0){
				opnum = 2;
			}
		}
		for(i=0 ; i<7; i++){
			if(strcmp(token_mne, opnum0[i]) == 0){
				opnum = 0;
			}
		}

		//hash table key 에 따라서 hash table에 저장(linked list)
		newNode = T3_createNode(op, token_mne, form, opnum);
		T3_addElement(key, newNode, hashtable);

	}
	//스트림을 닫아줌
	fclose(fopcode);
}

int hashFunction(char* token_mne){
	int key;
	int i;
	//hash function
	for(i=0, key=0 ; i<strlen(token_mne) ; i++){
		key += token_mne[i];
	}
	//////////////////
	//hash func 기능 추가
	switch(key%20){
		case 3:
			if(key % 20 == 3 && key > 300){
				key += 2;
			}
			break;
		case 11:
			if(key % 20 == 11 && key >270){
				key += 2;
			}
			break;
		case 14:
			if(key % 20 == 14 &&(key >300 || key <100) ){
				key += 12;
			}
			break;
		case 16:
			if(key % 20 == 16 && key >300){
				key += 6;	
			}
			break;
	}
	key = key % TABLESIZE;

	return key;
}


void memoryfree(VM* vm){
	free(vm);
}

