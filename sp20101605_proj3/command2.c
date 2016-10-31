//20101605 SP prj2
//command2.c

#include "20101605.h"

void a_comm_asm(LinkedList1* pList, HT* hashtable,/*  char** directives,*/ HT2* SYMTAB, LinkedList5* INTER){
	ListNode1* T1_temp;

	//
	ListNode5* T5_disp;
	//
	ListNode4* T4_newnode;
	ListNode5* T5_newnode;

	LinkedList1 *T1_asm;
	ListNode1* T1_asm_newnode;
	ListNode1* T1_asm_node;
	char filename[81];
	char read[81];
	char read_copy[81];
	char *token;
	char delimiters[8] = " \t";		//' '과 '\t'으로 구분
	char end[4];
	char F4_token[7];

	//
	int LOCCTR=0;			
	int PC=0;				
	int BASE = -1;

	//intermediate file contents 
	int line = 0;
	int loc=0;
	char label[7];
	char inst[7];
	char operand[10];
	char objectcode[9];
	char comment[81];

	//file pointer
	FILE *fp_asm;
	FILE *fp_lst;
	FILE *fp_obj;

	int i,j ;
	int key;			//optab의 hash func key 저장

	int START_sw = 0;
	int FORMAT_sw;
	char F4inst[7];		//inst 를 저장할 문자열	
	char F4inst2[7];	//inst 를 저장할 문자열
//	int BYTE_C_cnt;
	int quot_cnt;		//C' ', X' '  에서 ' 의 개수를 셈
	char operand_idx[10];
	//////////////////////////////////////////////////////////////////
	//pass2
	ListNode5* T5_pass;		//intermediate file list를 참조하기 위한 변수
	int INST[32];			//object code 생성 (각 bit 마다 저장)
	int INST_len;			//format마다 정해진 길이를 저장
	int N, I, X, B, P, E;	//N I X B P E 를 bit로 저장하기 위한 변수(0, 1)
	int WRITE;				//.lst, .obj 파일 생성을 결정하는 변수
	int dec, bin, hex;		//주소값, object code 계산 등 진수 변환 시 사용되는 변수
	int opnum;				//operand 수를 파악하는 변수 
	int TA;					//Target address 를 저장하는 변수
	char copy_operand[10];	//operand copy
	char* copy_token;
	char* REGISTER = "AXLBSTF";		//레지스터(대문자)
	char* lowerREGISTER = "AXLBSTF";//레지스터(소문자)
	int file_length;		//file length 저장
	int column;				//obj 파일 column 게산
	char T_record[100];		//obj 파일 T record object code 저장
	int T_start_loc;		//obj 파일 T record start loc
	
	
	
	int segchk=1;







	T1_asm = T1_createList();

	T1_temp = pList -> headNode.link;
	T1_temp = T1_temp->link;
	strcpy(filename, T1_temp->comm_data);


	fp_asm = fopen(filename, "r");

	WRITE = TRUE;
	if(fp_asm == NULL){	// 입력받은 .asm 파일이 없다
		printf("No such file\n");
		WRITE = -1;
	}
	else{				// 입력받은 .asm 파일이 있다
		//기존의 SYMTAB이 존재한다면 삭제함
		T4_removeElement(SYMTAB);	


		//////////////////////////////////////////////////////////////////////
		//pass1


		while( fgets(read, 81, fp_asm) != NULL ){
			read[strlen(read)-1] = '\0';
			strcpy(comment, read);

			if(loc>1048575){
				printf("Error, loc over flow\n");
				WRITE = -1;
				break;
			}

			if(read[0] == '.'){	//코멘트
				//				strcpy(comment, read);
//				line += 5;		//comment 는 line 저장 안함
				loc = 0;
				strcpy(label, "");
				strcpy(inst, "");
				strcpy(operand, "");
				strcpy(objectcode, "");

				//intermediate file에 저장
				T5_newnode = T5_createNode(line, loc, label, inst, operand, objectcode, comment, 1, PC);
				T5_addElement(INTER, T5_newnode);
			}
			else{				//코멘트가 아님
				//.asm 의 한줄을 저장(T1 list)
				token = strtok(read, delimiters);
				T1_asm_newnode = T1_createNode(token);
				T1_addElement(T1_asm, T1_asm_newnode);

				while(1){
					token = strtok(NULL, delimiters);
					if(token == NULL){
						break;
					}
					else{
						T1_asm_newnode = T1_createNode(token);
						T1_addElement(T1_asm, T1_asm_newnode);
					}
				}

				//END면 while 탈출
				{
					T1_asm_node = T1_asm -> headNode.link;
					if(strcmp(T1_asm_node->comm_data, "END") == 0){
						//End 관련 처리  
						LOCCTR = PC;
						loc = LOCCTR;
						line += 5;
						strcpy(label, "");//1
						//						T1_asm_node = T1_asm_node->link;	  //2
						strcpy(inst, T1_asm_node->comm_data); //2
						T1_asm_node = T1_asm_node->link;	  	//3
						strcpy(operand, T1_asm_node->comm_data);//3
						strcpy(objectcode, "");
						//PROGRAM LENGTH 계산
						file_length = loc;
						//intermediate file 에 저장
						T5_newnode = T5_createNode(line, loc, label, inst, operand, objectcode, comment, 0, PC);
						T5_addElement(INTER, T5_newnode);
						break;
					}	
				}

				//START chk
				if(START_sw == 0){
					if(T1_asm->currentElement == 3){
						T1_asm_node = T1_asm -> headNode.link;
						strcpy(label, T1_asm_node->comm_data);
						T1_asm_node = T1_asm_node -> link;
						if(strcmp(T1_asm_node->comm_data, "START") == 0){
							START_sw = 1;
							strcpy(inst, T1_asm_node->comm_data);
							T1_asm_node = T1_asm_node->link;
							strcpy(operand, T1_asm_node->comm_data);
							//LOCCTR = atoi(T1_asm_node->comm_data);
							PC = atoi(T1_asm_node->comm_data);

							LOCCTR = PC - 0;
							loc = LOCCTR;
							line = 5;

							//SYMTAB에 추가(label, LOCCTR)
							key = hashFunction2(label);
							T4_newnode = T4_createNode(label, loc, 0);
							T4_addElement(key, T4_newnode, SYMTAB);

							//intermediate파일에 저장
							T5_newnode = T5_createNode(line, loc, label, inst, operand, objectcode, comment, 0, PC);
							T5_addElement(INTER, T5_newnode);
						}
					}

				}
				else{	//START가 아님
					T1_asm_node = T1_asm -> headNode.link;//1

					memset(F4inst2, 0, 7);
					if(inst[0] == '+'){
						for(i=0 ; i<strlen(T1_asm_node->comm_data)-1 ; i++){
							F4inst2[i] = T1_asm_node->comm_data[i+1];
						}
					}
					else{
						strcpy(F4inst2, T1_asm_node->comm_data);
					}

					if(T1_asm->currentElement == 3){	//label이 있다
						//SYMTAB 에 label 있는지 검색
						if(FindSYM(SYMTAB, T1_asm_node->comm_data) && !FindOPTAB(hashtable, F4inst2 )){
							//SYMTAB에 없음

							LOCCTR = PC;
							loc = LOCCTR;

							line += 5;
							strcpy(label, T1_asm_node->comm_data);//1
							T1_asm_node = T1_asm_node->link;	  //2
							strcpy(inst, T1_asm_node->comm_data); //2
							T1_asm_node = T1_asm_node->link;	  	//3
							strcpy(operand, T1_asm_node->comm_data);//3

							memset(F4inst, 0, 7);
							if(inst[0] == '+'){
								for(i=0 ; i<strlen(inst)-1 ; i++){
									F4inst[i] = inst[i+1];
								}
							}
							else{
								strcpy(F4inst, inst);
							}
							if(FORMAT_sw = FindOPTAB(hashtable, F4inst) ){	//return 값은 FORMAT(1, 2, 3(/4)) or FALSE(0)
								switch(FORMAT_sw ){
									case 1:
										PC += 1;
										break;
									case 2:
										PC += 2;
										break;
									case 3:
										if(inst[0] == '+'){	//FORMAT 4
											PC += 4;
										}
										else{				//FORMAT 3
											PC += 3;
										}
										break;

								}
							}
							else{
								//없으면(assemble directives 인지 보고 처리)
								if(strcmp(inst, "WORD") == 0){
									//WORD 에 operand 저장(DEC -> HEX)	
									PC += 3;
								}
								else if(strcmp(inst, "RESW") == 0){
									//RESW *  operand (입력: DEC, 프로그램에선 HEX로 씀)
									//#: immediate?
									PC += 3 * atoi(operand);
									//								PC = 0;

								}
								else if(strcmp(inst, "BYTE") == 0){
									//C' ' , X' ' 처리
								//	BYTE_C_cnt = 0;
									quot_cnt = 2;
									if(operand[0] == 'C'){
										for(i=1; i<strlen(operand); i++){
											if(operand[i] == '\'' && i==1)
												quot_cnt--;
											else if(operand[i] == '\'' && i == strlen(operand)-1)
												quot_cnt--;
										}
										if(quot_cnt == 0){
											PC += strlen(operand)-3;
										}
										else{
											printf("quot error");
											printf("error line: %7s   %7s   %10s\n",label, inst, operand);
											WRITE = -1;
											break;
										}
									}
									else if(operand[0] == 'X'){
										if((strlen(operand) - 3) % 2 == 1){
											printf("error: X: ' '안의 hex 수가 홀수\n");
											printf("error line: %7s   %7s   %10s\n",label, inst, operand);
											WRITE = -1;
											break;
										}
										else{
											PC += (strlen(operand)-3)/2;
										}
									}

								}
								else if(strcmp(inst, "RESB") == 0){
									//RESB * operand (DEC -> HEX)
									//gg
									PC += 1 * atoi(operand);
								}
								else if(strcmp(inst, "BASE") == 0){
									loc = 0;
								}
								else if(strcmp(inst, "NOBASE") == 0){
									loc = 0;
								}
								else{
									//그것도 아니면 -> error
									printf("pass1 error[%s][%d]\n", inst, T1_asm->currentElement);
									WRITE = -1;
								}
							}
							//없으면 SYMTAB에 추가(label, LOCCTR)
							key = hashFunction2(label);
							T4_newnode = T4_createNode(label, loc, 0);
							T4_addElement(key, T4_newnode, SYMTAB);

							//intermediate파일에 저장
							T5_newnode = T5_createNode(line, loc, label, inst, operand, objectcode, comment, 0, PC);
							T5_addElement(INTER, T5_newnode);
						}
						else if(FindOPTAB(hashtable, T1_asm_node->comm_data)){

							LOCCTR = PC;
							loc = LOCCTR;

							line += 5;
							strcpy(label, "");
							strcpy(inst, T1_asm_node->comm_data); //2

							memset(F4inst, 0, 7);
							if(inst[0] == '+'){
								for(i=0 ; i<strlen(inst)-1 ; i++){
									F4inst[i] = inst[i+1];
								}
							}
							else{
								strcpy(F4inst, inst);
							}
							//OPTAB에서 inst 검색
							if(FORMAT_sw = FindOPTAB(hashtable, F4inst) ){	//return 값은 FORMAT(1, 2, 3(/4)) or FALSE(0)
								//있으면 F1~F4 처리
								switch(FORMAT_sw){
									case 1:
										PC += 1;
										break;
									case 2:
										PC += 2;
										break;
									case 3:
										//3과 4 구분
										if(inst[0] == '+'){	//FORMAT 4
											PC += 4;
										}
										else{				//FORMAT 3
											PC += 3;
										}
										break;
								}
							}
							T1_asm_node = T1_asm_node->link;	  //2
							strcpy(operand, T1_asm_node->comm_data);//3
							T1_asm_node = T1_asm_node->link;	  	//3
							strcpy(operand_idx, T1_asm_node->comm_data);//
							strcat(operand, operand_idx);

							//intermediate파일에 저장
							T5_newnode = T5_createNode(line, loc, label, inst, operand, objectcode, comment, 0, PC);
							T5_addElement(INTER, T5_newnode);
						}
						else{
							//SYMTAB에 label 중복(error)
							printf("error label 중복(SYMTAB)\n");
							printf("error line: %7s   %7s   %10s\n",label, inst, operand);
							WRITE = -1;
						}
					}
					else{		//label이 비어있음
						line += 5;
						LOCCTR = PC;
						loc = LOCCTR;
						//						strcpy(label, T1_asm_node->comm_data);//1 label이 비었다
						strcpy(label, "");
						//						T1_asm_node = T1_asm_node->link;	  //2
						strcpy(inst, T1_asm_node->comm_data); //2
						strcpy(operand, "");
						if(strcmp(inst, "RSUB") != 0){
							T1_asm_node = T1_asm_node->link;	  	//3
							strcpy(operand, T1_asm_node->comm_data);//3 
						}

						memset(F4inst, 0, 7);
						if(inst[0] == '+'){
							for(i=0 ; i<strlen(inst)-1 ; i++){
								F4inst[i] = inst[i+1];
							}
						}
						else{
							strcpy(F4inst, inst);
						}
						//OPTAB에서 inst 검색
						if(FORMAT_sw = FindOPTAB(hashtable, F4inst) ){	//return 값은 FORMAT(1, 2, 3(/4)) or FALSE(0)
							//있으면 F1~F4 처리
							switch(FORMAT_sw){
								case 1:
									PC += 1;
									break;
								case 2:
									PC += 2;
									break;
								case 3:
									//3과 4 구분
									if(inst[0] == '+'){	//FORMAT 4
										PC += 4;
									}
									else{				//FORMAT 3
										PC += 3;
									}
									break;
							}
						}
						else{
							//없으면(assemble directives 인지 보고 처리)
							loc = 0;
							if(strcmp(inst, "WORD") == 0){
								PC += 3;
							}
							else if(strcmp(inst, "RESW") == 0){
								//#: immediate?
								PC += 3 * atoi(operand);

							}
							else if(strcmp(inst, "BYTE") == 0){
								//C' ' , X' ' 처리
//								BYTE_C_cnt = 0;
								quot_cnt = 2;
								if(operand[0] == 'C'){
									for(i=1; i<strlen(operand); i++){
										if(operand[i] == '\'' && i==1)
											quot_cnt--;
										else if(operand[i] == '\'' && i == strlen(operand)-1)
											quot_cnt--;
									}
									if(quot_cnt == 0){
										PC += strlen(operand)-3;
									}
									else{
										printf("quot error");
										printf("error line: %7s   %7s   %10s\n",label, inst, operand);
										WRITE = -1;
									}
								}
								else if(operand[0] == 'X'){
									if((strlen(operand) - 3) % 2 == 1){
										printf("error: X ' ' 안의 hex 개수가 홀수\n");
										printf("error line: %7s   %7s   %10s\n",label, inst, operand);
									}
									else{
										PC += (strlen(operand)-3)/2;
									}
								}
							}
							else if(strcmp(inst, "RESB") == 0){
								PC += 1 * atoi(operand);
							}
							else if(strcmp(inst, "BASE") == 0){
							}
							else if(strcmp(inst, "NOBASE") == 0){
							}
							else{
								//그것도 아니면 -> error
								printf("labelX pass1 error[%s][%d]\n", inst, T1_asm->currentElement);
								WRITE = -1;


							}


						}//assemble directives 처리 끝

						//error 아니면 intermediate 파일에 저장
						//intermediate파일에 저장
						T5_newnode = T5_createNode(line, loc, label, inst, operand, objectcode, comment, 0, PC);
						T5_addElement(INTER, T5_newnode);

					}//label 비어있음 처리 끝

				}//start 아닌 라인 처리 끝

			}//코멘트 아님(명령어 라인)처리 끝

			T1_removeElement(T1_asm);

		}	//while 끝


		if(WRITE != 1){
			T5_removeElement(INTER);
			if(fp_asm != NULL){
				fclose(fp_asm);
			}
			return;
		}

		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		//pass2

		WRITE = 1;

//			printf("[%2d]seg chk\n", segchk++);
		//intermediate file 에서 읽음
		T5_pass = INTER -> headNode.link;
//		printf("segchk[%s]\n,",T5_pass->inst);
		if(strcmp(T5_pass -> inst, "START") == 0){
			//첫번째 LINE이 START로 시작하면
			//START는 object code 없음

			strcpy(objectcode, "");
			INST_len=-1;
			for(; T5_pass != NULL ; T5_pass = T5_pass->link){
				for(i=0 ; i<32; i++){
					INST[i] = 0;
				}
				INST_len = 0;
				if(strcmp(T5_pass -> inst, "END") == 0 ){
					//inst가 END 면 loop 탈출
					//END는 object code 없음
					break;
				}

				if(T5_pass -> comment[0] == '.'){
					//comment임
				}
				else{//comment 가 아님
					//inst를 OPTAB에서 탐색
					//if 있으면
					//format 1~4 로 구분하여 obj code 생성
					//				printf("[%s]\n",T5_pass->comment);
					strcpy(copy_operand, T5_pass->operand);
					strcpy(inst, T5_pass->inst);
					memset(F4inst, 0, 7);
					if(inst[0] == '+'){
						for(i=0 ; i<strlen(inst)-1 ; i++){
							F4inst[i] = inst[i+1];
						}
					}
					else{
						strcpy(F4inst, inst);
					}
					FORMAT_sw = FindOPTAB(hashtable, F4inst);
					opnum = FindOPNUM(hashtable, F4inst);
					if(FORMAT_sw){
						//					dec = strtol(T5_pass->inst, NULL, 16);	//opcode(HEX) -> dec
						dec = ret_OPCODE(hashtable, F4inst);
						for(i=8-1 ; i>=0 ;i--){
							if(dec > 1){
								INST[i] = dec%2;
								dec /= 2;
							}
							else if(dec == 1){
								INST[i] = dec;
								dec /= 2;
							}
							else{
								INST[i] = dec;
							}
						}
						switch(FORMAT_sw){
						case 1:	//F1
							INST_len = 2;
							T5_pass->len = INST_len;
							//opcode 입력
							dec = ret_OPCODE(hashtable, F4inst);
							for(i=8-1 ; i>=0 ;i--){
								if(dec > 1){
									INST[i] = dec%2;
									dec /= 2;
								}
								else if(dec == 1){
									INST[i] = dec;
									dec /= 2;
								}
								else{
									INST[i] = dec;
								}
							}
							break;
						case 2: //F2
							INST_len = 4;
							T5_pass->len = INST_len;
							//opcode 입력
							dec = ret_OPCODE(hashtable, F4inst);
							for(i=8-1 ; i>=0 ;i--){
								if(dec > 1){
									INST[i] = dec%2;
									dec /= 2;
								}
								else if(dec == 1){
									INST[i] = dec;
									dec /= 2;
								}
								else{
									INST[i] = dec;
								}
							}
							//operand 입력
							if(opnum == 0){
								for(i=8; i<16 ; i++)
									INST[i] = 0;

							}
							else if(opnum == 1){
								for(i=0 ; i<7; i++){
									if(REGISTER[i]== T5_pass->operand[0]  && strlen(T5_pass->operand) == 1){
										dec = i;
										break;
									}
									if(lowerREGISTER[i]== T5_pass->operand[0]  && strlen(T5_pass->operand) == 1){
										dec = i;
										break;
									}
									
								}
								for(i=12 ; i<16 ; i++)
									INST[i] = 0;
								for(i=11 ; i>=8 ;i--){
									if(dec > 1){
										INST[i] = dec%2;
										dec /= 2;
									}
									else if(dec == 1){
										INST[i] = dec;
										dec /= 2;
									}
									else{
										INST[i] = dec;
									}
								}
							}
							else if(opnum == 2){
//								printf("[%s]\n", copy_operand);
								token = strtok(copy_operand, " ,");
								for(i=0 ; i<7; i++){
									if(REGISTER[i]== token[0]  && strlen(T5_pass->operand) == 1){
										dec = i;
										break;
									}
									if(lowerREGISTER[i]== token[0]  && strlen(T5_pass->operand) == 1){
										dec = i;
										break;
									}
								}
								for(i=11 ; i>=8 ;i--){
									if(dec > 1){
										INST[i] = dec%2;
										dec /= 2;
									}
									else if(dec == 1){
										INST[i] = dec;
										dec /= 2;
									}
									else{
										INST[i] = dec;
									}
								}

								token = strtok(NULL, " ,");
								for(i=0 ; i<7; i++){
									if(REGISTER[i]== token[0] ){
										dec = i;
										break;
									}
									if(lowerREGISTER[i]== token[0] ){
										dec = i;
										break;
									}
								}
								for(i=15 ; i>=12 ;i--){
									if(dec > 1){
										INST[i] = dec%2;
										dec /= 2;
									}
									else if(dec == 1){
										INST[i] = dec;
										dec /= 2;
									}
									else{
										INST[i] = dec;
									}
								}
							}
							break;
						case 3: //F3 or F4
							//INST 배열 완성 후 object code 입력

							//							if(T5_pass->inst[0] == '+'){	//F4('+')

							INST_len = 6;				//inst len	처리해야됨

							//opcode(6bit) 입력 
							dec = ret_OPCODE(hashtable, F4inst);
							for(i=8-1 ; i>=0 ;i--){
								if(dec > 1){
									INST[i] = dec%2;
									dec /= 2;
								}
								else if(dec == 1){
									INST[i] = dec;
									dec /= 2;
								}
								else{
									INST[i] = dec;
								}
							}
							INST[6]=0;	//F3, F4 opcode는 6bit(7, 8번째 bit는 N, I)
							INST[7]=0;

							if(strcmp(T5_pass->operand,"") != 0){
								memset(operand, 0, 9);
								for(i=0 ; i<strlen(T5_pass->operand)-1 ; i++){
									operand[i] = T5_pass->operand[i+1];		
								}
								operand[i] = '\0';
							}
							if(T5_pass->operand[0] == '#' && FindSYM(SYMTAB, operand) == TRUE){
								//if(operand: #number) && not #address
								//dec 에 number 대입

								memset(operand, 0, 9);
								for(i=0 ; i<strlen(T5_pass->operand)-1 ; i++){
									operand[i] = T5_pass->operand[i+1];		
								}
								operand[i] = '\0';
								dec = atoi(operand);

								
								if(dec >= 0 && dec <= 4095){
									//if(0<= number <=4095)
									//opcode6 + 010000 + disp
									//disp: number(dec) -> hex
									N = 0;
									I = 1;
									X = 0;
									B = 0;
									P = 0;
									E = 0;

									for(i=23 ; i>=12 ;i--){
										if(dec > 1){
											INST[i] = dec%2;
											dec /= 2;
										}
										else if(dec == 1){
											INST[i] = dec;
											dec /= 2;
										}
										else{
											INST[i] = dec;
										}
									}
								}
								else if(dec >= 4096 && dec < 1048576 && T5_pass->inst[0] == '+'){
									//else if(4096 < number <1048576 && '+'
									//opcode6 + 010001 + address(20)
									N = 0;
									I = 1;
									X = 0;
									B = 0;
									P = 0;
									E = 1;

									for(i=31 ; i>=12 ;i--){
										if(dec > 1){
											INST[i] = dec%2;
											dec /= 2;
										}
										else if(dec == 1){
											INST[i] = dec;
											dec /= 2;
										}
										else{
											INST[i] = dec;
										}
									}
									INST_len = 8;				//inst len	처리해야됨
								}
								else{
									//else : error, immediate number out of range
								}

							}
							else if(strcmp(T5_pass->operand, "") != 0){
								memset(operand, 0, 10);
								if(T5_pass->operand[0] == '@' || T5_pass->operand[0] == '#'){
									for(i=0 ; i<strlen(T5_pass->operand)-1 ; i++){
										operand[i] = T5_pass->operand[i+1];
									}
									operand[i] = '\0';
								}
								else{
									strcpy(operand, T5_pass->operand);
								}
								for(i=0, j=strlen(operand) ; i<strlen(operand) ; i++){
									if(operand[i] == ','){
										operand[i++] = '\0';
										for(; i<j; i++){
											operand[i] = 0;
										}
										break;
									}
								}

								TA = ret_SYM_loc(SYMTAB, operand);
								if(TA == -1){
									printf("error, SYMTAB에 operand 없다\n");
									printf("error line: %7s   %7s   %10s\n",label, inst, operand);
									WRITE = -1;
									break;
								}

								if(T5_pass->inst[0] == '+'){
									//if '+' 면 opcode(6) + 110001 + 20bit address
									N = 1;
									I = 1;
									X = 0;
									B = 0;
									P = 0;
									E = 1;
									
									dec = TA;
									for(i=31 ; i>= 12 ; i--){
										if(dec > 1){
											INST[i] = dec%2;
											dec /= 2;
										}
										else if(dec == 1){
											INST[i] = dec;
											dec /= 2;
										}
										else{
											INST[i] = dec;
										}
									}
									if(T5_pass->operand[0] == '#')
										N=0;


									if(T5_pass->operand[0] == '#' && FindSYM(SYMTAB, operand) == 1){
										N = 0;
										I = 1;
										X = 0;
										B = 0;
										P = 0;
										E = 1;
										dec = atoi(operand);
										for(i=31 ; i>= 12 ; i--){
											if(dec > 1){
												INST[i] = dec%2;
												dec /= 2;
											}
											else if(dec == 1){
												INST[i] = dec;
												dec /= 2;
											}
											else{
												INST[i] = dec;
											}
										}
									}


									INST_len = 8;				//inst len	처리해야됨
								}
								else if( (TA-T5_pass->PC) >= -2048 && (TA-T5_pass->PC) <= 2047 && T5_pass->inst[0] != '+'){
									//else if(-2048 <= TA-PC <=2047) : PC relative, op6+ 110010 + disp(2's com)
									//-2048 ~ -1 (dec) : 800 ~ FFF(2's complement HEX)
									dec = TA - T5_pass->PC;

									N = 1;
									I = 1;
									X = 0;
									B = 0;
									P = 1;
									E = 0;

									//disp
									if(dec < 0 ){
										//음수일 경우 2's complement 로 계산해 줘야 됨
										dec += 4096;
									}
									for(i=23 ; i>=12 ;i--){
										if(dec > 1){
											INST[i] = dec%2;
											dec /= 2;
										}
										else if(dec == 1){
											INST[i] = dec;
											dec /= 2;
										}
										else{
											INST[i] = dec;
										}
									}

								}
								else if(BASE != -1 && (TA-BASE) >= 0 && (TA-BASE) <=4095 && T5_pass->inst[0] != '+'){
									//else if(BASE 가 set && 0<= TA-B <=4095): op6 + 110100 + disp(disp = TA - (BASE))
									dec = TA - BASE;

									N = 1;
									I = 1;
									X = 0;
									B = 1;
									P = 0;
									E = 0;

									//disp
									for(i=23 ; i>=12 ;i--){
										if(dec > 1){
											INST[i] = dec%2;
											dec /= 2;
										}
										else if(dec == 1){
											INST[i] = dec;
											dec /= 2;
										}
										else{
											INST[i] = dec;
										}
									}
								}
								else{
									//else : error, instruction addressing error
									printf("error, instruction addressing error\n");
									printf("error line: %7s   %7s   %10s\n",label, inst, operand);								
									WRITE = -1;
									break;
								}

							}
							else if(strcmp(T5_pass->operand, "") == 0){
								//else(operand == "") && opnum ==0(RSUB 등..): opcode6 + 110000 + disp(0)
								N = 1;
								I = 1;
								X = 0;
								B = 0;
								P = 0;
								E = 0;

								//disp
								for(i=12 ; i<24 ; i++){
									INST[i] = 0;
								}
							}

							if(T5_pass->operand[0] == '#'){
								//if(#operand): nix(010)
								N = 0;
								I = 1;
								X = 0;
							}
							if(T5_pass->operand[0] == '@'){
								//if(@operand): nix(100)
								N = 1;
								I = 0;
								X = 0;
							}

							////////////우짜노 여까지 왔는데 
							if(strlen(T5_pass->operand) != 0){
								strcpy(copy_operand, T5_pass->operand);
								for(i=0 ; i<strlen(copy_operand) ; i++){
									if(copy_operand[i] == ','){
										break;
									}
								}
								token = strtok(copy_operand, ",");
								if(FindSYM(SYMTAB, token) != 1 && T5_pass->operand[i+1] == 'X'){
									X =1 ;
								}
								else if(FindSYM(SYMTAB, token) != 1 && T5_pass->operand[i+1] == 'x'){
									X =1 ;
								}
							}
							INST[6] = N;
							INST[7] = I;
							INST[8] = X;
							INST[9] = B;
							INST[10] = P;
							INST[11] = E;

							T5_pass->len = INST_len;	//inst len	처리해야됨
							break;
						}
						//INST[] 에서 읽어 intermediate file 에 object code 입력
						memset(objectcode, 0, 9);
						for(i=0 ; i<INST_len ; i++){
							objectcode[i] = 0;
							for(j=0,bin=8, dec=0 ; j<4 ;j++, bin/=2){
								dec += bin * INST[(4*i) + j];							
							}
							if(dec >= 0 && dec < 10){
								objectcode[i] = dec + 48;
							}
							else if(dec >=10 && dec <= 15){
								objectcode[i] = dec + 55;
							}
							else{
								printf("error...\n");
							}

						}
						objectcode[i] = '\0';
						strcpy(T5_pass->objectcode, objectcode);
					}
					else{
						//else if(inst 가 directives)
						if(strcmp(T5_pass -> inst, "BYTE") == 0){
							//BYTE
							memset(objectcode, 0, 9);
							if(T5_pass -> operand[0] == 'X'){
								for(i=0 ; i<strlen(T5_pass->operand)-3; i++){
									objectcode[i] = T5_pass->operand[i+2];		
								}
								objectcode[i] = '\0';
							}
							else if(T5_pass -> operand[0] == 'C'){
								for(i=0, j=0; i<strlen(T5_pass->operand)-3; i++, j++){
									dec = T5_pass->operand[i+2];
									objectcode[i+j] = (dec/16) + 48;
									if(dec%16 <10){
										objectcode[i+j+1] = (dec%16) + 48;
									}
									else{
										objectcode[i+j+1] = (dec%16) + 55;
									}
								}
								objectcode[i+j] = '\0';
							}
							else{
								printf("error, BYTE operand는 C or X 로 시작해야됨\n");
								printf("error line: %7s   %7s   %10s\n",label, inst, operand);								
								WRITE = -1;

							}
						}
						else if(strcmp(T5_pass -> inst, "WORD") == 0){
							//WORD
							memset(objectcode, 0, 9);
							dec = atoi(T5_pass->operand);
							for(i=6-1 ; i>=0; i++){
								if(dec >15 ){
									if(dec%16 <10){
										objectcode[i] = (dec%16)+48;
									}
									else{
										objectcode[i] = (dec%16)+55;
									}
									dec /= 16;
								}
								else if(dec>=1 && dec<=15){
									if(dec%16 <10){
										objectcode[i] = dec+48;
									}
									else{
										objectcode[i] = dec+55;
									}
									dec /= 16;
								}
								else{
									objectcode[i] = '0';
								}
							}
							objectcode[i] = '\0';
						}
						else if(strcmp(T5_pass -> inst, "RESB") == 0){
							//RESB:		 object code 없음
							strcpy(objectcode, "");
						}
						else if(strcmp(T5_pass -> inst, "RESW") == 0){
							//RESW: 	 object code 없음
							strcpy(objectcode, "");
						}
						else if(strcmp(T5_pass -> inst, "BASE") == 0){
							//BASE:		 object code 없음
							BASE = ret_SYM_loc(SYMTAB, T5_pass->operand);
							strcpy(objectcode, "");
						}
						else if(strcmp(T5_pass -> inst, "NOBASE") == 0){
							//NOBASE:	 object code 없음
							BASE = -1;
							strcpy(objectcode, "");
						}
						
						//생성된 object code를 immediate file 에 저장
						strcpy(T5_pass->objectcode, objectcode);

					}


				}//end: comment가 아님

			}//end: loop
		}
		else{//START로 시작안함 (ERROR)
			printf("start로 시작안함\n");
			printf("error line: %7s   %7s   %10s\n",label, inst, operand);								
			WRITE = -1;
		}
	}

	//////////////////////////////////////////////////////////////////////
	//pass2가 error 없이 완료되면
	//intermediate file에서 읽어 .lst와 .obj 파일을 생성함
	if(WRITE == 1){	//파일 생성
		//.lst 파일 생성
		filename[strlen(filename)-1]='t';
		filename[strlen(filename)-2]='s';
		filename[strlen(filename)-3]='l';
		fp_lst = fopen(filename, "wt");
		T5_disp = INTER ->headNode.link;
		for(; T5_disp != NULL; T5_disp = T5_disp->link){
			if(T5_disp->commentchk == 1){
//				fprintf(fp_lst,"%4d      %s\n",T5_disp->line, T5_disp->comment);
			}
			else if(strcmp(T5_disp->inst, "BASE") ==0 || strcmp(T5_disp->inst, "NOBASE") == 0 || strcmp(T5_disp->inst, "END") == 0){
				fprintf(fp_lst,"%4d\t    \t%-6s\t%-6s\t%-20s%-s\n",T5_disp->line, T5_disp->label, T5_disp->inst,T5_disp->operand, T5_disp->objectcode);
			}
			else{
				fprintf(fp_lst,"%4d\t%04X\t%-6s\t%-6s\t%-20s%-s\n",T5_disp->line, T5_disp->loc, T5_disp->label, T5_disp->inst,T5_disp->operand, T5_disp->objectcode);
			}
		}

		fclose(fp_lst);
	
		printf("\t    output file : [%s], ",filename);

		//.obj 파일 생성
		filename[strlen(filename)-1]='j';
		filename[strlen(filename)-2]='b';
		filename[strlen(filename)-3]='o';
		fp_obj = fopen(filename, "wt");

		T5_disp = INTER -> headNode.link;
		//for1
		for(column = 60; T5_disp != NULL; T5_disp = T5_disp->link){
			//H
			if(strcmp(T5_disp->inst,"START") == 0){
				fprintf(fp_obj, "H%-6s%06X%06X\n",T5_disp->label, T5_disp->loc, file_length);
			}
			else{
				//T
				if(strcmp(T5_disp->inst, "END") == 0){
					break;
				}
				if(column == 60 &&( (strcmp(T5_disp->inst, "RESW") != 0 && strcmp(T5_disp->inst,"RESB") != 0 ) ) && T5_disp->commentchk != 1 ){
					strcpy(T_record, "");
					T_start_loc = T5_disp->loc;
//					strcat(T_record, "^");
					strcat(T_record, T5_disp->objectcode);
					column -= (T5_disp->PC - T5_disp->loc);
				}
				else if( (strcmp(T5_disp->inst, "RESW") != 0 && strcmp(T5_disp->inst,"RESB") != 0 && strcmp(T5_disp->inst,"BASE") != 0 && strcmp(T5_disp->inst, "NOBASE") != 0) && T5_disp->commentchk != 1){
//					strcat(T_record, "^");
					strcat(T_record, T5_disp->objectcode);
					column -= (T5_disp->PC - T5_disp->loc);
				}

				if(	( (strcmp(T5_disp->link-> inst,"RESW") == 0 || strcmp(T5_disp->link->inst, "RESB") == 0) && T_start_loc != -1) 
						&&( column != 60
							|| (29 - (T5_disp->PC - T_start_loc)) < (T5_disp->link->PC - T5_disp->link->loc))
						|| (T5_disp->PC - T_start_loc) == 29 
						|| strcmp(T5_disp->link->inst, "END") == 0){

					fprintf(fp_obj, "T%06X%02X%s\n", T_start_loc , T5_disp->PC - T_start_loc, T_record);
					column = 60;
					T_start_loc = -1;
				}

			}
		}
		//for2
		T5_disp = INTER -> headNode.link;
		for(column = 0; T5_disp != NULL; T5_disp = T5_disp->link){
			//M
			if(strcmp(T5_disp->operand,"") != 0){
				memset(operand, 0, 9);
				for(i=0 ; i<strlen(T5_disp->operand)-1 ; i++){
					operand[i] = T5_disp->operand[i+1];		
				}
				operand[i] = '\0';
			}
			//	if(T5_pass->operand[0] == '#' && FindSYM(SYMTAB, operand) == TRUE){
			if(T5_disp->inst[0] == '+' &&( T5_disp->operand[0] != '#'||(T5_disp->operand[0]=='#' && FindSYM(SYMTAB, operand)==0))){
				fprintf(fp_obj, "M%06X05\n",T5_disp->loc + 1);
			}
		}
		fprintf(fp_obj,"E000000\n");

		fclose(fp_obj);

		printf("[%s]\n",filename);

	}
	else{		//파일 생성 안함
//		printf("error1!!! 파일 생성 안한다\n");

	}



/*
	//////test/////
	//intermediate file출력
	printf("line_LOC__LABEL__INST___OPERAND_________OBJECTCODE\n");
	T5_disp = INTER ->headNode.link;
	for(; T5_disp != NULL; T5_disp = T5_disp->link){
		if(T5_disp->commentchk == 1){
	//		printf("%4d      %s\n",T5_disp->line, T5_disp->comment);
		}
		else if(strcmp(T5_disp->inst, "BASE") ==0 || strcmp(T5_disp->inst, "NOBASE") == 0 || strcmp(T5_disp->inst, "END") == 0){
			printf("%4d          %-6s %-6s %-9s       %-10s\n",T5_disp->line,  T5_disp->label, T5_disp->inst,T5_disp->operand, T5_disp->objectcode);
		}
		else{
			printf("%4d     %04X %-6s %-6s %-9s       %-10s\n",T5_disp->line, T5_disp->loc, T5_disp->label, T5_disp->inst,T5_disp->operand, T5_disp->objectcode);
		}
	}


	T5_disp = INTER -> headNode.link;
	//for1
	for(column = 60; T5_disp != NULL; T5_disp = T5_disp->link){
		//H
		if(strcmp(T5_disp->inst,"START") == 0){
			printf("H^%-6s^%06X^%06X\n",T5_disp->label, T5_disp->loc, file_length);
		}
		else{
		//T
			if(strcmp(T5_disp->inst, "END") == 0){
				break;
			}
			if(column == 60 &&( (strcmp(T5_disp->inst, "RESW") != 0 && strcmp(T5_disp->inst,"RESB") != 0 ) ) && T5_disp->commentchk != 1 ){
				strcpy(T_record, "");
				T_start_loc = T5_disp->loc;
				strcat(T_record, "^");
				strcat(T_record, T5_disp->objectcode);
				column -= (T5_disp->PC - T5_disp->loc);
			}
			else if( (strcmp(T5_disp->inst, "RESW") != 0 && strcmp(T5_disp->inst,"RESB") != 0 && strcmp(T5_disp->inst,"BASE") != 0 && strcmp(T5_disp->inst, "NOBASE") != 0) && T5_disp->commentchk != 1){
				strcat(T_record, "^");
				strcat(T_record, T5_disp->objectcode);
				column -= (T5_disp->PC - T5_disp->loc);
			}

			if(	( (strcmp(T5_disp->link-> inst,"RESW") == 0 || strcmp(T5_disp->link->inst, "RESB") == 0) && T_start_loc != -1) 
				&&( column != 60
					|| (29 - (T5_disp->PC - T_start_loc)) < (T5_disp->link->PC - T5_disp->link->loc))
						|| (T5_disp->PC - T_start_loc) == 29 
							|| strcmp(T5_disp->link->inst, "END") == 0){

				printf("T^%06X^%02X%s\n", T_start_loc , T5_disp->PC - T_start_loc, T_record);
				column = 60;
				T_start_loc = -1;
			}
			
		}
	}
	//for2
	T5_disp = INTER -> headNode.link;
	for(column = 0; T5_disp != NULL; T5_disp = T5_disp->link){
		//M
		if(T5_disp->inst[0] == '+' && T5_disp->operand[0] != '#'){
			printf("M^%06X^05\n",T5_disp->loc + 1);
		}
	}
	printf("E^000000\n");
*/


	//////////////////////////////
	//intermediate file free:
	T5_removeElement(INTER);
	if(fp_asm != NULL){
		fclose(fp_asm);
	}



}

void a_comm_type(LinkedList1* pList){
	ListNode1* temp;
	char filename[81];
	char read[81];	
	FILE *fp;

	DIR *pDir;
	struct dirent *dir;
	struct stat buf;


	temp = pList -> headNode.link;
	temp = temp->link;
	strcpy(filename, temp->comm_data);

	if( (pDir = opendir(".")) == NULL){
		printf("[type]directory open error\n");
	}
	lstat(filename, & buf);

	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("No such file or directory\n");		
	}
	else if(S_ISDIR(buf.st_mode)){
		printf("[type]%s is a directory\n", filename);
	}
	else if(filename[strlen(filename)-1]=='t' && filename[strlen(filename)-2]=='u'&&filename[strlen(filename)-3]=='o'&&filename[strlen(filename)-4]=='.'){
		printf("[type]%s is exe file\n", filename);

	}
	else{
		while(fgets(read, 81, fp) != NULL){
			printf("%s",read);
		}
	}
	if(fp != NULL)
		fclose(fp);
}

void a_comm_symbol(HT2* SYMTAB, LinkedList6* SORT){
	int i;

	ListNode4* T4_temp;
	ListNode6* T6_newnode;

	//출력 할 리스트 새로 만들면서 정렬(내림차순)
	for(i=0 ; i<SYMTABSIZE ; i++){
		T4_temp = SYMTAB[i].head.headNode.link;
		for(;T4_temp != NULL ; T4_temp = T4_temp->link){
			T6_newnode = T6_createNode(T4_temp->label,T4_temp->loc);
			T6_addElement(SORT, T6_newnode);
		}
	}

	//SYMTAB 출력
	T6_display(SORT);

}


