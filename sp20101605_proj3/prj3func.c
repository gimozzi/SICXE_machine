//sp_prj3
//prj3func.c

#include "20101605.h"

int comp_OPCODE(int inst_idx){
	FILE *fp = fopen("opcode.txt", "r");
	char opcode[3];
	char mnemonic[10];
	char format[5];
	int comp;
	int ret_format;
	

	while(fscanf(fp, "%s%s%s",opcode, mnemonic, format) ){
		comp = strtol(opcode, NULL, 16);
//		ret_format = format[0]-48;
	//	if(comp == opcode_dec){
	//		return ret_format;
	//	}
//		printf("comp[%d][%X][%d][%X]\n",comp, comp, inst_idx, inst_idx);
		if(comp == inst_idx){
			return TRUE;
		}
	}
	return FALSE;

}

int determine_OPCODE(int opcode_dec){	// memory에 저장된 값이 opcode인지 판별함.
	FILE *fp = fopen("opcode.txt", "r");
	char opcode[3];
	char mnemonic[10];
	char format[5];
	int comp;
	int ret_format;
	

	while(fscanf(fp, "%s%s%s",opcode, mnemonic, format) ){
		comp = strtol(opcode, NULL, 16);
		ret_format = format[0]-48;
		if(comp == opcode_dec){
			return ret_format;
		}
	}
	return FALSE;
}

int hashFunction3(char* token){		//ESTAB 의 hash function 
	int key;
	int i;

	//hash function 
	for(i=0, key=0 ; i<strlen(token) ; i++){
		key += token[i];
	}
	key = key % ESTABSIZE;

	return key;
}

int FindESTAB(HT3* ESTAB, char* token){
	ListNode7* temp;
	int count;
	int ret;
	int key;

	key = hashFunction3(token);
	temp = ESTAB[key].head.headNode.link;
	count = ESTAB[key].head.currentElement;

	for( ; temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(token, temp->name) == 0){
			//error

			ret = FALSE;
			return ret;
		}
	}
	if(count == 0){
		//ESTAB에 없으므로 추가해줘야됨

		ret = TRUE;
	}

	return ret;
}

int ret_ESTAB_address(HT3* ESTAB, char* token){
	ListNode7* temp;
	int count;
	int ret;
	int key;

	key = hashFunction3(token);
	temp = ESTAB[key].head.headNode.link;
	count = ESTAB[key].head.currentElement;

	for( ; temp != NULL ; temp = temp->link){
		count--;
		if(strcmp(token, temp->name) == 0){
			//error
			ret = temp->address;
			return ret;
		}
	}
	if(count == 0){
		//ESTAB에 없으므로 추가해줘야됨
//		printf("ESTAB  에 없음\n");
		ret = -1;
	}
	return ret;
}

//2자리의 HEX(1BYTE)를 dec 값으로 return 해주는 함수
int _2_digit_Hex_to_Dec(char* token){
	int dec=0;
	int i, hex;

	for(i=1, hex=1 ; i>=0 ; i--, hex*=16){
		if(token[i] >=48 && token[i]<=57){
			dec += (token[i]-48) * hex;
		}
		else if(token[i] >= 65 && token[i]<=70){
			dec += (token[i]-55) * hex;
		}
	}
	return dec;
}
//6자리의 HEX(3BYTEs)를 dec 값으로 return 해주는 함수
int _6_digit_Hex_to_Dec(char* token){
	int dec=0;
	int i, hex;

	for(i=5, hex=1 ; i>=0 ; i--, hex*=16){
		if(token[i] >=48 && token[i]<=57){
			dec += (token[i]-48) * hex;
		}
		else if(token[i] >= 65 && token[i]<=70){
			dec += (token[i]-55) * hex;
		}
	}
	return dec;
}

//loader 명령 후 load map 그리기 위한 value 저장
void write_loadmap(MAP* loadmap, int idx, char* name_csect, char* name_symbol, int address, int length, int header){
	strcpy(loadmap[idx].name_csect, name_csect);
	strcpy(loadmap[idx].name_symbol, name_symbol);
	loadmap[idx].address = address;
	loadmap[idx].length = length;
	loadmap[idx].header = header;
}

int Linking_Loader_Pass1(char* filename, HT3* ESTAB, int PROGADDR, int* CSADDR, MAP* loadmap, int loadmapElement, int* errorchk){
	int CSLTH = 0;
	char read[81];
	//ESTAB data
	char name[7];	
	int address;
	//string 에서 읽은 문자열 임시저장 var
	char _name[7];
	char _csaddr[7];
	char _cslth[7];
	char _address[7];
	//ESTAB var
	ListNode7* T7_newnode;
	int key;
	char* blank = "";


	int i=0, j;

	FILE *fp_filename = fopen(filename, "r");

	//begin
	//get PROGADDR from OS(= sicsim prompt 에서 입력받은 값)
	//set CSADDR to PROGADDR
	

	while(fgets(read, 81, fp_filename) != NULL){
		//enter 까지 읽으므로 마지막은 EOS로 바꿔줌
		if(strlen(read) > 1 && read[0] != 'E'){
			read[strlen(read)-1] = '\0';
		}
			
		//종료조건: E record가 나왔을 때
		if(read[0] == 'E'){
			//CSADDR에 CSLTH 더 해줌: 다음 Control Section의 시작주소가 된다
			(*CSADDR) += CSLTH;
			break;
		}

		if(read[0] == 'H'){
			//set CSLTH to control section length
			//search ESTAB for control section name
			for(i=1 ;i<=6 ;i++){
				if(read[i] ==' ')
					break;				
			}
			strncpy(name, &read[1], i-1);
			name[i-1] = '\0';

			strncpy(_csaddr, &read[7], 6);
			_csaddr[6] = '\0';
			strncpy(_cslth, &read[13], 6);
			_cslth[6] = '\0';

			//_csaddr, _cslth 를 Hex -> Dec 변환
			(*CSADDR) += _6_digit_Hex_to_Dec(_csaddr);		
			CSLTH = _6_digit_Hex_to_Dec(_cslth);


			address = (*CSADDR);

			//ESTAB 에 저장(name, value(=given by CSADDR))
			if(FindESTAB(ESTAB, name)){
				key = hashFunction3(name);	
				T7_newnode = T7_createNode(name, address, 1, CSLTH);
				T7_addElement(key, T7_newnode, ESTAB);
				//printf("%7s\t      \t%X\t%X\n",name, address, CSLTH);
				write_loadmap(loadmap,loadmapElement++,  name, blank, address, CSLTH, 1);
				
			}
			else{
				(*errorchk) = 0;
				printf("error, duplicated in ESTAB\n");
			}


		}
		if(read[0] == 'D'){
			//빈칸처리
			for(i=1 ; i<(int)strlen(read) ; i+=12){
				for(j=i; j<=i+5 ; j++){
					if(read[j] ==' ')
						break;				
				}
				strncpy(name, &read[i], j-i);
				name[j-i] = '\0';

				strncpy(_address, &read[i+6], 6);
				_address[6] = '\0';


				address = _6_digit_Hex_to_Dec(_address);		
				address += (*CSADDR);

//			printf(":%d::::::::::::::::::::::::[%s]\n",(int)strlen(read),name);
				if(FindESTAB(ESTAB, name)){
					key = hashFunction3(name);	
					T7_newnode = T7_createNode(name, address, 0, -1);
					T7_addElement(key, T7_newnode, ESTAB);
				//	printf("       \t%6s\t%X\n",name, address);
					write_loadmap(loadmap, loadmapElement++,  blank, name, address, CSLTH, 0);
				}
				else{
					(*errorchk) = 0;
					printf("error, duplicated in ESTAB\n");
				}
			}

		}


	}
	fclose(fp_filename);
	return loadmapElement;
}




void Linking_Loader_Pass2(char* filename, HT3* ESTAB, int PROGADDR, int* CSADDR, VM* vm, int* errorchk, LinkedList9* T9_List){
	int CSLTH = 0;
	char read[81];
	//ESTAB data
	char name[7];	
	int address;
	//string 에서 읽은 문자열 임시저장 var
	char _name[7];
	char _csaddr[7];
	char _cslth[7];
	//ESTAB var
	ListNode7* T7_newnode;
	int key;

	//T record List
	ListNode9* T9_newnode;
	int t_address;
	int t_length;
	
	//T record ( 1byte )
	char _address[3];
	char _1byte_objcode[3];
	int load_address;
	unsigned char load_value;

	//M record
	int modify_address;
	int modify_length;
	char _mlength[3];
	int msign;	// +: 0, -: 1
	int ref;	//reference array index
	int modify_value;


	int i=0, j;
	//reference number array
	RN* rn;
	int rn_index = 0;
	char _rnumber[3];
	char _rname[7];

	FILE *fp_filename = fopen(filename, "r");

	rn = (RN *)malloc(100 * sizeof(RN));


	while(fgets(read, 81, fp_filename) != NULL){
		//enter 까지 읽으므로 마지막은 EOS로 바꿔줌
		if(strlen(read) > 1 && read[0] != 'E'){
			read[strlen(read)-1] = '\0';
		}
//		printf("[%s]\n",read);
		
		//제일 처음 파일을 읽으면 H 로 시작하는 지 chk 하고 
		//CSLTH 를 set 해줌.
		if(read[0] == 'H'){
			//set CSLTH to control section length
			//search ESTAB for control section name
			strncpy(_csaddr, &read[7], 6);
			_csaddr[6] = '\0';
			strncpy(_cslth, &read[13], 6);
			_cslth[6] = '\0';

			//_csaddr, _cslth 를 Hex -> Dec 변환
			(*CSADDR) += _6_digit_Hex_to_Dec(_csaddr);		
			CSLTH = _6_digit_Hex_to_Dec(_cslth);

			//reference number array 만들기(01)
			for(i=1 ;i<=6 ;i++){
				if(read[i] ==' ')
					break;				
			}
			strncpy(_rname, &read[1], i-1);
			_rname[i-1] = '\0';
			rn[rn_index].number = 1;
			strcpy(rn[rn_index].name, _rname);
			rn[rn_index].address = ret_ESTAB_address(ESTAB, _rname);
			if(ret_ESTAB_address(ESTAB, _rname) == -1){
				printf("undefined symbol\n");
				(*errorchk)=0;
				return;
			}				
			rn_index++;
//			printf("R%dRRRRRR[%s][%s]\n",rn_index,_rnumber, _rname);
		}


		if(read[0] == 'E' && strlen(read) >1 ){
			//get the address of the first instruction: PC jump
			
			//set CSLTH to CASDDR: 다음 control section 의 시작 주소가 된다.
			(*CSADDR) += CSLTH;
			break;
		}
		if(read[0] == 'E' ){
			//CSADDR에 CSLTH 더 해줌: 다음 Control Section의 시작주소가 된다
			(*CSADDR) += CSLTH;
			break;
		}



		if(read[0] == 'R'){
			//EXTREF, reference number 로 참조함, ESTAB
			//reference number array 만들기

			for(i=1 ; ; i+=8){
				if(i >= strlen(read))
					break;
				
				strncpy(_rnumber, &read[i], 2);
				_rnumber[2] = '\0';
				for(j= i+2 ; j<=i+2+5 ;j++ ){
					if(read[j]==' ')
						break;
				}
				strncpy(_rname, &read[i+2], j-(i+2));
				_rname[j-(i+2)] = '\0';
				rn[rn_index].number = _2_digit_Hex_to_Dec(_rnumber);
				strcpy(rn[rn_index].name, _rname);
				rn[rn_index].address = ret_ESTAB_address(ESTAB, _rname);
				if(ret_ESTAB_address(ESTAB, _rname) == -1){
				printf("undefined symbol\n");
					(*errorchk)=0;
					return;
				}				
//				printf("R%dRRRRRR[%s][%s]\n",rn_index,_rnumber, _rname);
				rn_index++;
			}
		}
		if(read[0] == 'T'){
			//the object code is moved to the specified address
			//column 0: T
			//column 1~6: memory address
			strncpy(_address, &read[1], 6);
			_address[6] = '\0';
			load_address = _6_digit_Hex_to_Dec(_address);

			load_address += (*CSADDR);

			t_address = load_address;
			

			//column 7~8: length of T record
			strncpy(_1byte_objcode, &read[7], 2);
			_1byte_objcode[2] = '\0';
			t_length = _2_digit_Hex_to_Dec(_1byte_objcode);

			T9_newnode = T9_createNode(t_address, t_length);
			T9_addElement(T9_List, T9_newnode);	
		
			//column 9~ : object code : 2 character 가 1 byte 임 (1 character: 4bit = half byte)
			for(i=9 ; i<= strlen(read) - 2 ; i+=2){

				strncpy(_1byte_objcode, &read[i], 2);
//				printf("[%d]",i);
				_1byte_objcode[2] = '\0';
//printf("[%s]\n", _1byte_objcode);
				load_value = (unsigned char)_2_digit_Hex_to_Dec(_1byte_objcode);

				load_to_memory(vm, load_address , load_value);

				load_address += 1;
			}
		}
		if(read[0] == 'M'){
			//reference number 를 사용함..
			strncpy(_address, &read[1], 6);
			_address[6] = '\0';
			modify_address = _6_digit_Hex_to_Dec(_address);
			modify_address += (*CSADDR);
//printf("modify address[%X]\n",modify_address);
			strncpy(_mlength, &read[7], 2);
			_mlength[3] = '\0';
			modify_length = _2_digit_Hex_to_Dec(_mlength);
			
			if(read[9] == '+')
				msign = 0;
			else
				msign = 1;

			strncpy(_mlength, &read[10], 2);
			_mlength[3] = '\0';
			ref = _2_digit_Hex_to_Dec(_mlength);
			for(i=0 ; i< rn_index ; i++){
				if(rn[i].number == ref){
	//				printf("modify_value 는 [%s][%d][%X]\n",rn[i].name, rn[i].address, rn[i].address);
					modify_value = rn[i].address;
					break;
				}
			}
			modify_memory(vm, modify_address, modify_length, msign, modify_value);
			
		}
	}
	fclose(fp_filename);
	free(rn);
}

//M record 수행
void modify_memory(VM* vm, int address, int length, int msign, int modify_value){
	int line, idx;
	int i, hex;
	
	int modified_value=0;
	int val;


	for(i=address, hex=256*256 ; i<address+3 ; i++, hex/=256){
		line = i/16;
		idx = i%16;
		modified_value += vm[line].memory[idx] * hex;
	}
//	printf("====mvalue:[%d]==[%X]=====",modified_value, modified_value);
//	printf("modify[%X][%X][%d][%x]",address, length, msign, modify_value);

	if(msign == 0)
		modified_value += modify_value;
	else{
		modified_value -= modify_value;
	}
	
	if(modified_value < 0)
		modified_value += BOUND;
	if(modified_value > BOUND )
		modified_value %= BOUND;
	
//	printf("MV[%6d][%6X]__reference[%6d][%6X]\n", modified_value, modified_value, modify_value, modify_value);

//	printf("====mvalue:[%d]==[%X]=====",modified_value, modified_value);

//	printf("\t\tbit op:");
	for(i=address+2 ; i>=address ;i--){
		line = i/16;
		idx = i%16;
		val = modified_value %256;
		vm[line].memory[idx] = val;
//		printf("[%X]",val);
		modified_value /= 256 ;
	}
	
}

//T record 수행
void load_to_memory(VM* vm, int address, unsigned char value ){
	int line, idx;

	line = address / 16;
	idx = address % 16;
	vm[line].memory[idx] = value;
}


/////////////////////////
//run run run run run run
void init_register(REG* reg, int PROGADDR){
	reg->A = 0;
	reg->X = 0;
	reg->L = 0;
	
	reg->B = 0;
	reg->S = 0;
	reg->T = 0;

	reg->PC = PROGADDR;
	reg->CC = 0;
}

