//각종 명령어를 처리하는 함수가 작성된 파일입니다.

#include "20101605.h"

//입력받은 명령어 chk(명령어 인지 error인지 판단)
int command_chk(LinkedList1* pList, int* commachk, int* errorchk, int* commaposchk, char* getcommand, char** comm_arr){
	int comm_arg[24] = {0,0, 0,0, 0,0, 0,0, 9,9, 2,2, 3,3, 0, 1, 0, 1, 1, 0/*prj3*/, 1, 0, 0, 0};
	//각각 명령어의 argument 개수
	// dump는 0, 1, 2 셋 다 되므로 따로 처리함
	//prj2 수정됨 17 -> 20
	//prj3 수정됨 20 -> 24 

	ListNode1* node;
	int i;
	int ret = FALSE;
	int flag;
	/////////////comma 위치 chk위한 변수 선언
	char *token;
	char delimiter[8] = " \t";
	char copycomm[81];
	int comchkcnt;
	char tempstring[81];
	
	node = pList -> headNode.link;
	//comma 수와 비교 함
	for(i=0 ; i<strlen(node->comm_data) ; i++){
		if(node->comm_data[i]==','){
			(*commachk)--;
		}
	}
	
	for(i=0 ,flag=FALSE; i<24; i++){
		if(strcmp(node->comm_data, comm_arr[i]) == 0){
			flag = TRUE;
			break;
		}
	}

	//////////////////////
	//comma 위치 chk
	(*commaposchk) = TRUE;
	strcpy(copycomm, getcommand);
	comchkcnt = 0;
	token = strtok(copycomm, delimiter);
	comchkcnt++;
	while(1){
		token = strtok(NULL, delimiter);
		comchkcnt++;
		if(token == NULL && tempstring[strlen(tempstring)-1] == ',' && pList->currentElement >1){
			//입력된 명령어 제일 마지막에 , 가 나온 경우
			error(6, errorchk);
			(*commaposchk) = FALSE;
			return ret;
		}
		else if(token == NULL){
			break;
		}
		else{
			if(comchkcnt==2 && token[0] ==','){
				//명령어 바로 다음 , 가 나온 경우
				error(6, errorchk);
				(*commaposchk) = FALSE;
				return ret;
			}		
		}
		strcpy(tempstring, token);
	}
	//prj3 에서 loader, bp 는 따로 처리(arg 수 가 다양하기 때문...)
	node = pList -> headNode.link;
	if(flag && strcmp(node->comm_data, comm_arr[21]) == 0 && pList->currentElement <= 4){
		//loader

		return TRUE;
	}
	else if(flag && strcmp(node->comm_data, comm_arr[23]) == 0 && pList->currentElement <= 2){
		//bp
		return TRUE;
	}

	
	/*
	else if(flag && strcmp(node->comm_data, comm_arr[20]) == 0 && pList->currentElement == 2){
		//progaddr
		printf("progaaaaaaa\n");
		return TRUE;
	}
	*/

	//flag 가 TRUE => 명령어 list의 첫번째 node data가 명령어와 일치함
	switch(pList->currentElement - 1){
		//currentElement: 명령어 list에 있는 단어의 수
		case 0:
			if(flag && comm_arg[i] == 0){	//dump 이외 arg가 0개인 명령어
				ret = TRUE;
			}
			else if(flag && (i==8 || i==9) ){  //dump인 경우
				ret = TRUE;
			}
			else{							//예외
				ret = FALSE;
			}
			break;

		case 1:								//dump(arg1)
			if(flag && ( i==8 || i==9 )){
				ret = TRUE;
			}
			else if(flag && comm_arg[i] == 1){//opcode mnemonic
				
				ret = TRUE;
			}
			else{
				ret = FALSE;
			}		
			break;

		case 2:								//dump(arg1, arg2)
			if(flag && ( i==8 || i==9 )){
				
				ret = TRUE;

			}
			else if(flag && comm_arg[i]==2){//edit(arg1, arg2)
				ret = TRUE;
			}
			else{
				ret = FALSE;
			}
			break;

		case 3:								//fill(arg1, arg2, arg3)
			if(flag && comm_arg[i] == 3){
				ret = TRUE;
			}
			break;

		default:							//그 외
			ret = FALSE;

	}
	return ret;
}

//Shell 관련 명령어 함수

void s_comm_help(){		//help
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	/* prj2 구현 */
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
}

void s_comm_dir(){		//dir
	DIR *pDir;
	struct dirent *dir;
	struct stat buf;
	char exechk[4];
	int dircnt = 0;
	int i, k;

	if( (pDir = opendir(".")) == NULL){
		printf("directory open error\n");
	}

	while( (dir= readdir(pDir) ) != NULL ){
		if(dir -> d_ino == 0)
			continue;
		lstat(dir -> d_name, &buf);
		if(dir->d_name[0]!='.'){
			if( S_ISDIR(buf.st_mode) ){	//directory
				//디렉토리 출력
				printf("%s/", dir->d_name);
				k = strlen(dir->d_name) + 1;
			}
			else if(S_ISREG(buf.st_mode)){
				//실행파일 chk(out으로 끝나면 *을 붙여줌)
				exechk[0]=dir->d_name[strlen(dir->d_name)-3];
				exechk[1]=dir->d_name[strlen(dir->d_name)-2];
				exechk[2]=dir->d_name[strlen(dir->d_name)-1];
				exechk[3]='\0';
				if(strcmp("out",exechk) == 0){//exe file name
					printf("%s*",dir->d_name);
					k = strlen(dir->d_name) + 1;
				}
				else{						  //file name
					//파일이름 출력
					printf("%s",dir->d_name);
					k = strlen(dir->d_name);
				}
			}
			//16 이하인 경우 공백 표시
			if(k<=16){
				for(i= 16-k ; i>0 ; i--){
					printf(" ");
				}
			}
			else if(k > 16){//16자 이상인 경우 파일2개해당 공백표시
				dircnt++;
				for(i=32-k ; i>0 ;i--){
					printf(" ");
				}
			}
			if(dircnt%5 == 4){//파일 5개 마다 줄 바꿔줌
				printf("\n");
			}
			dircnt++;
		}
		else{
		}

	}
	//개행이 안되어 있으면 개행해줌
	if(dircnt%5 != 0)
		printf("\n");
	closedir(pDir);
}

void s_comm_history(LinkedList2* pList){  //history
	//T2 LIst를 display
	T2_display(pList);
}


//Memory 관련 명령어 함수
//dump
int m_comm_dump(VM* vm, LinkedList1* pList, int idx, int* commachk, int* errorchk, int* boundarychk){
	int line;		//출력 할 line 수를 저장할 변수
	int i, j, k;	
	int sw;			//dump 의 arg에 따라 switch를 구분할 변수
	int start, end;
	char string[81];
	int hi;
	int hex, len;
	int idxstart, idxend;
	ListNode1* temp;


	sw = pList -> currentElement;
	j=0;	//j 는 반환할 address count에 사용됨
	(*boundarychk) = TRUE;

	//arg가 있는 경우
	start = 0;
	end = 0;

	temp = pList -> headNode.link;
	//첫번째 arg 처리
	if(temp -> link != NULL){
		temp = temp->link;
		strcpy(string, temp -> comm_data);
		len = strlen(string);
		//Hex 문자열 -> Hex 수 로 변경
		for(hi=len-1, hex=1;hi >= 0;hi--, hex*=16){
			if(string[hi] >= 48 && string[hi] <= 57){
				start += (string[hi] - 48) * hex;
			}
			else if(string[hi] >= 65 && string[hi] <= 70){
				start += (string[hi] - 55) * hex;
			}
			else if(string[hi] >= 97 && string[hi] <= 102){
				start += (string[hi] - 87) * hex;
			}
			else{
				error(4, errorchk);
				break;

			}
		}
	}
	//error처리 요함

	//두번째 arg 처리
	if(temp -> link != NULL){
		temp = temp->link;
		strcpy(string, temp -> comm_data);
		len = strlen(string);
		//Hex 문자열 -> Hex 수 로 변경
		for(hi=len-1, hex=1 ; hi >= 0;hi--, hex*=16){
			if(string[hi] >= 48 && string[hi] <= 57){
				end += (string[hi] - 48) * hex;
			}
			else if(string[hi] >= 65 && string[hi] <= 70){
				end += (string[hi] - 55) * hex;
			}
			else if(string[hi] >= 97 && string[hi] <= 102){
				end += (string[hi] - 87) * hex;
			}
			else{
				error(4, errorchk);
				break;
			}

		}
	}
	//error 처리 요함

	if(!(*errorchk)){
		switch(sw-1){
			case 0://case 0: arg 없음
				if((*commachk) == 0){
					//line 변수에 출력할 line수를 지정해줌
					if(idx % 16 == 0){	//시작위치가 [0]인 경우
						line = (idx + 160)/16;	
						idxstart = idx ;
						idxend = idx + 160;
					}
					else{//시작위치가 [0]이 아닌 경우(한줄 더 출력해야됨)
						line = (idx + 160)/16 + 1;
						idxstart = idx;
						idxend = idx + 160 - 1;
					}
				}
				else{
					error(1, errorchk);
				}
				
				break;
			case 1://case 1: arg 1개
				if((*commachk) == 0){
					idx = start;
					//line 변수에 출력할 line수를 지정해줌
					if(idx % 16 == 0){	//시작위치가 [0]인 경우
						line = (idx + 160)/16;	
						idxstart = idx ;
						idxend = idx + 160;
					}
					else{//시작위치가 [0]이 아닌 경우(한줄 더 출력해야됨)
						line = (idx + 160)/16 + 1;
						idxstart = idx;
						idxend = idx + 160 - 1;
					}
				}
				else{
					error(1, errorchk);
				}
				break;
			case 2://case 2: arg 2개
				if((*commachk) == 1){
					idx = start;
					if(idx % 16 == 0){	//시작위치가 [0]인 경우
						line = (idx + (end - start + 1))/16;
					}
					else{//시작위치가 [0]이 아닌 경우(한줄 더 출력)
						line = (idx + (end - start + 1))/16 +1;
					}
					idxstart = idx;
					idxend = end;
					if(end - start < 0){
						error(5, errorchk);
					}
				}
				else{
					error(6, errorchk);
				}
				break;
		}
		///////////////////////////////
		//errorchk
		///////////////////////////////
		if(!(*errorchk)){
			for(k=idx/16 ; k<line ; k++){
				if(idx + j > BOUNDARY){
					(*boundarychk) = FALSE;
				}
				if(idxend == BOUNDARY && idx + j > BOUNDARY){
					break;
				}
				if(!(*boundarychk) ){
					error(0, errorchk);
					break;
				}
				//memory address 출력
				printf("%05X ", k * 16);

				//memory 내용 출력(Hex)
				for(i=0 ; i<16 ;i++){
					if(idx + j > BOUNDARY){
						(*boundarychk) = FALSE;
					}
					if(!(*boundarychk))
						break;
					if(k*16 + i >= idxstart && k*16 + i <= idxend){
						printf("%02X ", vm[k].memory[i]);
						j++;
					}
					else{
						printf("   ");
					}
				}
				printf("; ");

				//memory 내용 출력(ASCII)
				for(i=0 ; i<16 ; i++){
					if(!(*boundarychk))
						break;
					//0x20 == 32, 0x7E == 127
					if(vm[k].memory[i]>= 32 && vm[k].memory[i]<=127){
						printf("%c", vm[k].memory[i]);
					}
					else{
						printf(".");
					}
				}
				printf("\n");
				if(!(*boundarychk)){
					error(0, errorchk);			
					break;
				}
			}
			idx += j;
		}

	}
	return idx;
}

void m_comm_edit(VM* vm, LinkedList1* pList, int* commachk, int* errorchk){//edit
	int start, value;
	int hex, len, hi;
	int line, idx;
	char string[81];
	ListNode1* temp;

	if(pList -> currentElement == 3 && (*commachk) == 1){
		//명령어 token에 저장된 start 문자열을 int 형식으로 바꿔줌
		temp = pList -> headNode.link;
		temp = temp->link;
		strcpy(string, temp -> comm_data);
		start = 0;
		len = strlen(string);		
		//Hex로 입력된 수(문자열 -> 수)변환
		for(hi = len-1, hex = 1 ; hi >= 0 ; hi--, hex*=16){
			if(string[hi] >= 48 && string[hi] <= 57){
				start += (string[hi] - 48) * hex;
			}
			else if(string[hi] >= 65 && string[hi] <= 70){
				start += (string[hi] - 55) * hex;
			}
			else if(string[hi] >= 97 && string[hi] <= 102){
				start += (string[hi] - 87) * hex;
			}
			else{
				error(4, errorchk);
				break;
			}
		}
		if((*errorchk)){
			return;
		}
		if(start > BOUNDARY){
			error(0, errorchk);
		}
		//두번째 arg 처리
		temp = temp->link;
		strcpy(string, temp -> comm_data);
		len = strlen(string);
		value = 0;
		//Hex로 입력된 수(문자열 -> 수)변환
		for(hi=len-1, hex=1 ; hi >= 0;hi--, hex*=16){
			if(string[hi] >= 48 && string[hi] <= 57){
				value += (string[hi] - 48) * hex;
			}
			else if(string[hi] >= 65 && string[hi] <= 70){
				value += (string[hi] - 55) * hex;
			}
			else if(string[hi] >= 97 && string[hi] <= 102){
				value += (string[hi] - 87) * hex;
			}
			else{
				error(4, errorchk);
				break;
			}
		}
		if((*errorchk)){
			return;
		}

		if(value < 0 || value > 255){
			//입력된 valeu ( 00 ~ FF )
			error(2, errorchk);
		}
		//해당 주소의 값을 변경
		line = start / 16;
		idx = start % 16;
		vm[line].memory[idx] = value;	

	}
	else{
		error(6, errorchk);
	}
}

void m_comm_fill(VM* vm, LinkedList1* pList, int* commachk, int* errorchk){//fill
	int start, end, value;
	int line, idx;
	int i, k;
	int hi, hex, len;
	char string[81];
	ListNode1* temp;
	
	if(pList -> currentElement == 4 && (*commachk) == 2){
		//명령어 token에 저장된 start 문자열을 int 형식으로 바꿔줌
		temp = pList -> headNode.link;
		temp = temp->link;
		strcpy(string, temp -> comm_data);
		start = 0;
		len = strlen(string);		
		//Hex로 입력된 수(문자열 -> 수)변환
		for(hi = len-1, hex = 1 ; hi >= 0 ; hi--, hex*=16){
			if(string[hi] >= 48 && string[hi] <= 57){
				start += (string[hi] - 48) * hex;
			}
			else if(string[hi] >= 65 && string[hi] <= 70){
				start += (string[hi] - 55) * hex;
			}
			else if(string[hi] >= 97 && string[hi] <= 102){
				start += (string[hi] - 87) * hex;
			}
			else{
				error(4, errorchk);
				break;
			}
		}
		if((*errorchk)){
			return;
		}
		if(start > BOUNDARY){
			error(0, errorchk);
		}
		//두번째 arg 처리
		temp = temp->link;
		strcpy(string, temp -> comm_data);
		end = 0;
		len = strlen(string);
		for(hi=len-1, hex=1 ; hi >= 0;hi--, hex*=16){
			if(string[hi] >= 48 && string[hi] <= 57){
				end += (string[hi] - 48) * hex;
			}
			else if(string[hi] >= 65 && string[hi] <= 70){
				end += (string[hi] - 55) * hex;
			}
			else if(string[hi] >= 97 && string[hi] <= 102){
				end += (string[hi] - 87) * hex;
			}
			else{
				error(4, errorchk);
				break;
			}
		}
		if((*errorchk)){
			return;
		}
		if(start > BOUNDARY){
			error(0, errorchk);
		}
		//세번째 arg 처리
		temp = temp->link;
		strcpy(string, temp -> comm_data);
		len = strlen(string);
		value = 0;
		for(hi=len-1, hex=1 ; hi >= 0;hi--, hex*=16){
			if(string[hi] >= 48 && string[hi] <= 57){
				value += (string[hi] - 48) * hex;
			}
			else if(string[hi] >= 65 && string[hi] <= 70){
				value += (string[hi] - 55) * hex;
			}
			else if(string[hi] >= 97 && string[hi] <= 102){
				value += (string[hi] - 87) * hex;
			}
			else{
				error(4, errorchk);
				break;
			}
		}
		if((*errorchk)){
			return;
		}
		if(value < 0 || value > 255){
			error(2, errorchk);
			return;
		}
		if(start > end){
			error(5, errorchk);
			return;
		}
		///////////////////////
		//해당 주소의 값을 변경
		for(i=start ; i <=  end ;i++){
			line =  i / 16;
			idx = i % 16;
			vm[line].memory[idx] = value;	
		}
		
	}
	else{
		error(6, errorchk);
	}

}

void m_comm_reset(VM* vm){	//reset: 전체 메모리값을 0으로 변경
	int i, j;
	for(i=0 ; i<MEM_ROW ; i++){
		for(j=0 ; j<MEM_COL ; j++){
			vm[i].memory[j] = 0x0;
		}
	}
}


//opcode Table 관련 명령어 함수 

void o_comm_mnemonic(LinkedList1* pList, int* errorchk, HT* hashtable){
	//opcode mnemonic
	ListNode1* temp;
	int key;
	int i;
	char mnemonic[81];

	temp = pList -> headNode.link;
	temp = temp->link;
	strcpy(mnemonic, temp->comm_data);
	
	key = hashFunction(mnemonic);	//hash func호출하여 key 반환받음

	display_mnemonic(key, mnemonic, errorchk, hashtable);
}

void o_comm_list(HT* hashtable){		//opcode list
	int i;	
	//display
	for(i=0 ; i<TABLESIZE ; i++){
		printf("%d :", i);
		T3_display(hashtable[i]);	
		printf("\n");
	}

}



