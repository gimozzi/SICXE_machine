//20101605
//SP prj3

#include "20101605.h"

//progaddr 설정
int l_comm_progaddr(LinkedList1* pList){
	int progaddr = 0;
	int i, hex;
	ListNode1* node;

	node = pList -> headNode.link;
	node = node->link;

	for(i=strlen(node->comm_data)-1, hex=1 ; i>=0 ; i--, hex*=16 ){
		if(node->comm_data[i] >=48 && node->comm_data[i]<=57){
			progaddr += (node->comm_data[i]-48)*hex;
		}
		else if(node->comm_data[i] >= 65 && node->comm_data[i] <= 70){
			progaddr += (node->comm_data[i]-55)*hex;
		}
		else if(node->comm_data[i] >= 97 && node->comm_data[i] <= 102){
			progaddr += (node->comm_data[i]-87)*hex;
		}
		else{
			printf("error, please input correct HEX address\n");
		}
	}
	return progaddr;
}

int l_comm_loader(LinkedList1* pList, HT3* ESTAB, int PROGADDR, VM* vm, LinkedList9* T9_List){
	int CSADDR = 0;
	int CSLTH = 0;

	char filename1[81];	//arg1
	char filename2[81];	//arg2
	char filename3[81];	//arg3

	FILE *fp_obj1;		
	FILE *fp_obj2;
	FILE *fp_obj3;

	ListNode1* T1_node;

	MAP* loadmap;
	int loadmapElement = 0;
	int total_length = 0;
	
	int i,j;
	int errorchk = 1;

	//////////////////////
	T1_node = pList->headNode.link;
	T1_node = T1_node -> link;
	if(pList->currentElement == 4){
		//obj 3개 입력
		strcpy(filename1, T1_node->comm_data);
		T1_node = T1_node -> link;
		strcpy(filename2, T1_node->comm_data);
		T1_node = T1_node -> link;
		strcpy(filename3, T1_node->comm_data);
		
		fp_obj1 = fopen(filename1, "r");
		if(fp_obj1 == NULL){
			printf("No such file\n");
			return;
		}
		fp_obj2 = fopen(filename2, "r");
		if(fp_obj2 == NULL){
			printf("No such file\n");
			return;
		}
		fp_obj3 = fopen(filename3, "r");
		if(fp_obj3 == NULL){
			printf("No such file\n");
			return;
		}
	}
	else if(pList->currentElement == 3){
		//obj 2개 입력
		strcpy(filename1, T1_node->comm_data);
		T1_node = T1_node -> link;
		strcpy(filename2, T1_node->comm_data);

		fp_obj1 = fopen(filename1, "r");
		if(fp_obj1 == NULL){
			printf("No such file\n");
			return;
		}
		fp_obj2 = fopen(filename2, "r");
		if(fp_obj2 == NULL){
			printf("No such file\n");
			return;
		}

	}
	else if(pList->currentElement == 2){
		//obj 1개 입력 
		strcpy(filename1, T1_node->comm_data);

		fp_obj1 = fopen(filename1, "r");
		if(fp_obj1 == NULL){
			printf("No such file\n");
			return;
		}

	}
	else{
		printf("error, loader 의 arg가 올바르지 않음\n");
		return;
	}


	//ESTAB 초기화
	T7_removeElement(ESTAB);
	//T record 초기화
	T9_removeElement(T9_List);
	
	////////////////////////////////////////////////////
	//pass1
	
	loadmap = (MAP *)malloc(100 * sizeof(MAP));

	if(pList->currentElement >= 2){
		CSADDR = PROGADDR;
		loadmapElement = Linking_Loader_Pass1(filename1, ESTAB, PROGADDR, &CSADDR, loadmap, loadmapElement, &errorchk);
	}
	if(pList->currentElement >= 3){
		loadmapElement = Linking_Loader_Pass1(filename2, ESTAB, PROGADDR, &CSADDR, loadmap, loadmapElement, &errorchk);
	}
	if(pList->currentElement >= 4){
		loadmapElement = Linking_Loader_Pass1(filename3, ESTAB, PROGADDR, &CSADDR, loadmap, loadmapElement, &errorchk);
	}
//	T7_display(ESTAB);	//이거 하려면 읽은 순서대로 저장하는 list 가 필요할거 같다
	if(!errorchk){
		//file stream close
		if(pList->currentElement == 4){
			fclose(fp_obj1);
			fclose(fp_obj2);
			fclose(fp_obj3);
		}
		else if(pList->currentElement == 3){
			fclose(fp_obj1);
			fclose(fp_obj2);
		}
		else if(pList->currentElement == 2){
			fclose(fp_obj1);
		}

		return;
	}




	////////////////////////////////////////////////////
	//pass2
	if(pList->currentElement >= 2 && errorchk){
		CSADDR = PROGADDR;
		Linking_Loader_Pass2(filename1, ESTAB, PROGADDR, &CSADDR, vm, &errorchk, T9_List);
	}
	if(pList->currentElement >= 3 && errorchk){
		Linking_Loader_Pass2(filename2, ESTAB, PROGADDR, &CSADDR, vm, &errorchk, T9_List);
	}
	if(pList->currentElement >= 4 && errorchk){
		Linking_Loader_Pass2(filename3, ESTAB, PROGADDR, &CSADDR, vm, &errorchk, T9_List);
	}

	//jump to location given by EXECADDR ( to start execution of loaded program)
	

	//loadmap 출력 
	if(errorchk){
		printf("\t\tcontrol\tsymbol \taddress\t\tlength\n");
		printf("\t\tsection\tname\n");
		printf("\t\t------------------------------------------\n");
		for(i=0 ; i<loadmapElement ; i++){
			if(loadmap[i].header){
				total_length += loadmap[i].length;
				printf("\t\t%-7s\t       \t%-7X\t\t%04X\n", loadmap[i].name_csect, loadmap[i].address, loadmap[i].length);	
			}
			else{
				printf("\t\t       \t%-7s\t%-7X\n", loadmap[i].name_symbol, loadmap[i].address);	
			}
		}
		printf("\t\t------------------------------------------\n");
		printf("\t\t       \t       \ttotal length\t%04X\n",total_length);
	}
	free(loadmap);

	//file stream close
	if(pList->currentElement == 4){
		fclose(fp_obj1);
		fclose(fp_obj2);
		fclose(fp_obj3);
	}
	else if(pList->currentElement == 3){
		fclose(fp_obj1);
		fclose(fp_obj2);
	}
	else if(pList->currentElement == 2){
		fclose(fp_obj1);
	}
	 
	return total_length;
}

int l_comm_run(REG* reg, int PROGADDR, VM* vm, LinkedList8* T8_List, HT* OPTAB, int total_length, LinkedList9* T9_List, int* runrunrun, int BP){
	int INST[32];  				// instruction
	int inst_format = 0;		// inst format: 1~4
	int opcode_6bit[8];			//opcode bit저장
	int read_opcode=0;			// memory 값(HEX) ->binary bit 변환위해 임시 저장 변후
	int opcode_dec=0;			// opcode bit -> DEC 
	int opcode_format=0;		//opcode format
	
	int empty_chk=0;

	int LOCCTR = 0;				// program address

	
	//T8 list : bp list
	ListNode8* T8_node;
	//T9 list - pc jump
	ListNode9* T9_node;

	char f2_inst[3][7] = {
		"CLEAR", "COMPR", "TIXR"
	};
	char f3_inst[17][7] = {
		"COMP", "J", "JEQ", "JLT", "JSUB", "LDA", "LDB", "LDCH", "LDT", "RD", "RSUB", "STA", "STCH", "STL", "STX", "TD", "WD"
	};
	int f2_inst_idx[3] = {180, 160, 184};
	int f3_inst_idx[17]={40, 60, 48, 56, 72, 0, 104, 80, 116, 216, 76, 12, 84, 20, 16, 224, 220};
	
	char* uregister = "AXLBSTF";

	int code1=0, code2=0, code3=0;	//변환 된 register
	int TA=0, value=0;				//Target address, value
	int TA_mem=0;					



	int i,j,k, bin;
	int loop_cnt=0;
	int endflag=0;

	char debug;

	int bp_flag=0;					//
	int BP_address;					//
	//init register
	if(*runrunrun == 0){			//처음 시작
		init_register(reg, PROGADDR);
		LOCCTR = PROGADDR;
	}
	else{							//BP 부터 다시 시작
		LOCCTR = BP;
	}
	//T9
	T9_node = T9_List -> headNode.link;  
	

/*
	for(i=0 ; i< 64 ; i++){
		printf("[%X]", vm[i/16].memory[i%16]);
	}
*/

//	T9_display(T9_List);
	T8_node = T8_List->headNode.link;

	for(;T8_node != NULL; T8_node = T8_node->link){
		if(BP < T8_node->address){
			BP_address = T8_node->address;
			bp_flag=1;
			break;
		}
	}
	if(bp_flag == 0){
		BP_address = -1;	
	}
	while(/*reg->PC*/LOCCTR < PROGADDR + total_length){	
		//if(loop_cnt++ >1500)
		//	break;
//		if(reg->PC == 48)
//			break;
		if(LOCCTR == BP_address ){
			//BP 이면 break 하고 현재 register 출력 후 현재 BP return
			printf("\t  A :%010X X :%-10X\n", reg->A, reg->X);
			printf("\t  L :%010X PC:%010X\n", reg->L, reg->PC);
			printf("\t  B :%010X S :%010X\n", reg->B, reg->S);
			printf("\t  T :%010X\n", reg->T);
			printf("\tStop at checkpoint[%X]\n", LOCCTR);
			BP = LOCCTR;
			*runrunrun = 1;
			return BP;
		}

		T9_node = T9_List->headNode.link;
		for(;T9_node != NULL; T9_node = T9_node->link){
			if(LOCCTR >= T9_node->address && LOCCTR < T9_node->address+T9_node->length){
				endflag=1;
				break;
			}
		}
		if(endflag==0)
			break;
				
//		if(  LOCCTR >  (T9_node->address + T9_node->length) ){
//			printf("========[%X]jump to ", T9_node->address);
//			printf("[%X]=========\n", T9_node->address + T9_node->length);
//			T9_node = T9_node->link;
//			LOCCTR = T9_node->address;
//		}
		//*/
		//memory 첫번째 부분이 opcode 인지 파악
		read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
		if(read_opcode == 0){
			break;
//			continue;
		}
		//opcode 를 6bit 로 변환
		for(i=0 ; i<8 ; i++) 
			opcode_6bit[i] = 0;
		for(i=7 ; read_opcode>0 ; i--){
			opcode_6bit[i] = read_opcode % 2;
			read_opcode /= 2;
		}
		opcode_6bit[6] = 0;
		opcode_6bit[7] = 0;
		//6bit opcode -> DEC
		for(i=0, bin=128, opcode_dec=0 ; i<8 ;i++, bin/=2)
			opcode_dec += opcode_6bit[i]*bin;

//		printf("opcode_dec[%X]", opcode_dec);
		//INST 배열 초기화
		for(i=0 ; i<32 ; i++)
			INST[i] = 0;
		if(opcode_format = determine_OPCODE(opcode_dec)){
		//opcode 라면 format 별 instruction 수행
//			printf("LOCCTR:[%5d][%5X] PC[%5X]opcode 임format[%d]_____",/*reg->PC, reg->PC,*/LOCCTR, LOCCTR,reg->PC,  opcode_format);
			switch(opcode_format){
				case 1:
					//foramt 1
					inst_format = 1;
					read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
//					printf("%02X",read_opcode);
					for(i=7 ; read_opcode>0 ; i--){
						INST[i] = read_opcode % 2;
						read_opcode /= 2;
					}
//					reg->PC++;
					reg->PC= ++LOCCTR;

//					printf("되나1?\n");
					break;
				case 2:
					//format2
					inst_format = 2;
					read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
//					printf("%02X",read_opcode);
					for(i=7 ; read_opcode>0 ; i--){
						INST[i] = read_opcode % 2;
						read_opcode /= 2;
					}
//					reg->PC++;
//					LOCCTR++;
					reg->PC= ++LOCCTR;
					read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
//					printf("%02X",read_opcode);
					for(i=7 ; read_opcode>0 ; i--){
						INST[i+8] = read_opcode % 2;
						read_opcode /= 2;
					}
//					reg->PC++;
//					LOCCTR++;
					reg->PC= ++LOCCTR;
					
					
					for(i=0 ; i<3 ; i++){
						if(opcode_dec == f2_inst_idx[i])
							break;
					}
					switch(i){
						case 0:
							//clear
							for(code1=0,bin=8,j=8 ; j<12 ; j++, bin/=2){
								code1 +=INST[j]*bin;
							}
							if(code1 == 0)	reg->A = 0;
							if(code1 == 1)	reg->X = 0;
							if(code1 == 2)	reg->L = 0;
							if(code1 == 3)	reg->B = 0;
							if(code1 == 4)	reg->S = 0;
							if(code1 == 5)	reg->T = 0;
							break;
						case 1:
							//compr
							for(code1=0,bin=8,j=8 ; j<12 ; j++, bin/=2){
								code1 +=INST[j]*bin;
							}
							for(code2=0,bin=8,j=12 ; j<16 ; j++, bin/=2){
								code2 +=INST[j]*bin;
							}
							if(code1 == 0) code1 = reg->A;
							if(code1 == 1) code1 = reg->X;
							if(code1 == 2) code1 = reg->L;
							if(code1 == 3) code1 = reg->B;
							if(code1 == 4) code1 = reg->S;
							if(code1 == 5) code1 = reg->T;
							
							if(code2 == 0) code2 = reg->A;
							if(code2 == 1) code2 = reg->X;
							if(code2 == 2) code2 = reg->L;
							if(code2 == 3) code2 = reg->B;
							if(code2 == 4) code2 = reg->S;
							if(code2 == 5) code2 = reg->T;
							
							if(code1 == code2) {	reg->CC = 0;/*printf("CC=0")*/;}
							if(code1 > code2){	reg->CC = 1; /*printf("CC=-1")*/;}
							if(code1 < code2) {	reg->CC = -1;/*printf("CC=1")*/;}
							break;
						case 2:
							//TIXR
//							printf("T..%d..T", loop_cnt++);
							for(code1=0,bin=8,j=8 ; j<12 ; j++, bin/=2){
								code1 +=INST[j]*bin;
							}
							reg->X++;
							if(code1 == 0) code1 = reg->A;
							if(code1 == 1) code1 = reg->X;
							if(code1 == 2) code1 = reg->L;
							if(code1 == 3) code1 = reg->B;
							if(code1 == 4) code1 = reg->S;
							if(code1 == 5) code1 = reg->T;

//							printf("X:[%X]R1:[%X]",reg->X, code1);
							if(reg->X == code1) 	reg->CC = 0;
							if(reg->X > code1)	reg->CC = 1;
							if(reg->X < code1) 	reg->CC = -1;
							
							break;
						default:
							printf("error\n");
							break;

					}
//					printf("되나2?\n");

					break;
				case 3:
					//format3(4)
					inst_format = 3;
					//printf("[");
					read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
//					printf("%02X",read_opcode);
					for(i=7 ; read_opcode>0 ; i--){
						INST[i] = read_opcode % 2;
						read_opcode /= 2;
					}
//					printf("TTTTTTTTTTTTTTTTTTTTTT\n");
//					reg->PC++;
//					LOCCTR++;
					reg->PC= ++LOCCTR;
					read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
//					printf("%02X",read_opcode);
					for(i=7 ; read_opcode>0 ; i--){
						INST[i+8] = read_opcode % 2;
						read_opcode /= 2;
					}
//					reg->PC++;
//					LOCCTR++;
					reg->PC= ++LOCCTR;
					read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
//					printf("%02X",read_opcode);
					for(i=7 ; read_opcode>0 ; i--){
						INST[i+16] = read_opcode % 2;
						read_opcode /= 2;
					}
//					reg->PC++;
//					LOCCTR++;
					reg->PC= ++LOCCTR;
					if(INST[11] == 1){	//e bit == 1( format 4)
						inst_format = 4;
						read_opcode = vm[/*reg->PC*/LOCCTR / 16].memory[/*reg->PC*/LOCCTR % 16];
//						printf("%02X",read_opcode);
						for(i=7 ; read_opcode>0 ; i--){
							INST[i+24] = read_opcode % 2;
							read_opcode /= 2;
						}
//						reg->PC++;
//						LOCCTR++;
					reg->PC= ++LOCCTR;
					}
//					printf("]");
//					printf("[");
					for(i=0 ; i<inst_format*8 ; i++){
//						printf("%d",INST[i]);
						if(i%4 == 0 && i!=0)
//							printf(" ");
						empty_chk += INST[i];
					}

//					printf("]empty chk[%d]",empty_chk);
					if(!empty_chk){
//						reg->PC -=2;
						LOCCTR-=2;
						empty_chk=0;
						break;
					}
//					printf("되나3?\n");
					//					if(empty_chk == 125)
					//						return;
//					printf("opcodedec[%d]\n", opcode_dec);


					//현재 메모리에 저장된 opcode 명령어 찾아내기
					for(i=0 ; i<17 ; i++){
						if(opcode_dec == f3_inst_idx[i])
							break;
//						if(comp_OPCODE(f3_inst_idx[i])){
//							printf("compOPCODE[%s]\n",f3_inst[i]);
//							break;
//						}
					}
					


					//TA 및 value 설정
					if(INST[11] == 1){	//format 4
						//TA는 20bit
						for(TA=0 , bin =1,j=31 ; j>=12 ; j--, bin*=2){
							TA += INST[j]*bin;	
						}

						//VALUE 설정(n, i)
						if(INST[6] == 0 && INST[7] == 1 ){
							//n=0, i=1 :#  immediate
							value = TA;			
						}
						if(INST[6] == 1 && INST[7] == 0 ){
							//n=1, i=0 :@  indirect
							TA_mem = vm[TA/16].memory[TA%16] * 16* 16 * 16 * 16;
							TA_mem += vm[(TA+1)/16].memory[(TA+1)%16] * 16 * 16 * 16;
							TA_mem += vm[(TA+2)/16].memory[(TA+2)%16] * 16 * 16;
							TA_mem += vm[(TA+3)/16].memory[(TA+3)%16] * 16;
							TA_mem += vm[(TA+4)/16].memory[(TA+4)%16];
							
							
							value = vm[TA_mem/16].memory[TA_mem%16] * 16* 16 * 16 * 16;
							value += vm[(TA_mem+1)/16].memory[(TA_mem+1)%16] * 16 * 16 * 16;
							value += vm[(TA_mem+2)/16].memory[(TA_mem+2)%16] * 16 * 16;
							value += vm[(TA_mem+3)/16].memory[(TA_mem+3)%16] * 16;
							value += vm[(TA_mem+4)/16].memory[(TA_mem+4)%16];

						}
						if(INST[6] == 1 && INST[7] == 1){
							//n=1, i=1 : simple addressing
							//value : TA..TA+2 의 값
							value = vm[TA/16].memory[TA%16] * 16* 16 * 16 * 16;
							value += vm[(TA+1)/16].memory[(TA+1)%16] * 16 * 16 *16;
							value += vm[(TA+2)/16].memory[(TA+2)%16] * 16 * 16;
							value += vm[(TA+3)/16].memory[(TA+3)%16] *16;
							value += vm[(TA+4)/16].memory[(TA+4)%16];
						}

					}
					else{				//format 3
						//TA 설정
						for(TA=0, bin=1, j=23 ; j>=12 ; j--, bin*=2){
							TA += INST[j] * bin;
						}
//						printf("PC:[%X]TAAAA[%X]",reg->PC, TA);
						if(INST[9]==1 && INST[10] ==0){
							//base relative
							TA += reg->B;
						}
						if(INST[9]==0 && INST[10] ==1){
							//pc relative
							if(TA >=2048)
								TA-=4096;
							TA += reg->PC;
						}
						if(INST[9]==0 && INST[10] ==0){
							//direct addressing
						}

						if(INST[8]==1){
							//indexed addressing
							TA += reg->X;
						}
//						if(TA>4095)
//							TA %= 4096;

						//VALUE 설정(n, i)
						if(INST[6] == 0 && INST[7] == 1 ){
							//n=0, i=1 :#  immediate
							value = TA;			
						}
						if(INST[6] == 1 && INST[7] == 0 ){
							//n=1, i=0 :@  indirect
							TA_mem = vm[TA/16].memory[TA%16] * 256* 256;
							TA_mem += vm[(TA+1)/16].memory[(TA+1)%16] * 256;
							TA_mem += vm[(TA+2)/16].memory[(TA+2)%16];
							
							
							value = vm[TA_mem/16].memory[TA_mem%16] * 256* 256;
							value += vm[(TA_mem+1)/16].memory[(TA_mem+1)%16] * 256;
							value += vm[(TA_mem+2)/16].memory[(TA_mem+2)%16];

						}
						if(INST[6] == 1 && INST[7] == 1){
							//n=1, i=1 : simple addressing
							//value : TA..TA+2 의 값
							value = vm[TA/16].memory[TA%16] * 256* 256;
							value += vm[(TA+1)/16].memory[(TA+1)%16] * 256;
							value += vm[(TA+2)/16].memory[(TA+2)%16];
//							printf("@@simple[%X]",value);
//							printf("memory value [%X][%X][%X]@@",vm[TA/16].memory[TA%16],vm[(TA+1)/16].memory[(TA+1)%16],vm[(TA+2)/16].memory[(TA+2)%16] );
						}
						if(INST[6] == 0 && INST[7] == 0){
							//standard SIC mode

						}


					}

//					printf("TA[%X], value[%X]\n",TA, value);
					//명령어 수행
					switch(i){
						case 0:	//comp
//							printf("=0=comp");
							if(reg->A == value)	reg->CC = 0;
							if(reg->A > value)	reg->CC = -1;
							if(reg->A < value)	reg->CC = 1;
//							printf("###########A:[%X], value:[%X]__CC:[%d]#############\n", reg->A, value, reg->CC);
							break;
						case 1:
							//J
//							printf("=1=J");
							reg->PC = TA;	
//							printf("reg->PC = TA[%X][%X]\n",reg->PC, TA);
							break;
						case 2: 
							//JEQ:CC = 
//							printf("=2=JEQ");
							if(reg->CC == 0)
								reg->PC = TA;
//							printf("CC:[%d]reg->PC = TA[%X][%X]\n",reg->CC,reg->PC, TA);

							break;
						case 3:
							//JLT:CC <
//							printf("=3=JLT");
							if(reg->CC == -1)
								reg->PC = TA;
//							printf("reg->PC = TA[%X][%X]\n",reg->PC, TA);
							break;
						case 4:
							//JSUB
//							printf("=4=JSUB");
							reg->L = reg->PC;
//							reg->PC = vm[TA/16].memory[TA%16];
							reg->PC = TA;
//							printf("reg->L = reg->PC[%X][%X]\n",reg->L, reg->PC);
//							printf("L register[%X]\n", reg->L);
//							reg->PC = ;
							break;
						case 5:	//LDA
//							printf("=5=LDA");
							reg->A = value;
//							printf("reg->A[%X], value[%X]\n", reg->A, value);
							break;
						case 6:	//LDB
//							printf("=6=LDB");
							reg->B = value;
//							printf("BBBBB[%X]\n", reg->B);
							break;
						case 7:	//LDCH (A의 rightmost BYTE에 (m) 저장
//							printf("=7=LDCH");
							reg->A -= (reg->A)%256;
							reg->A += vm[TA/16].memory[TA%16];
//							printf("reg->A [%X]\n", reg->A);
							break;
						case 8:	//LDT
//							printf("=8=LDT");
							reg->T = value;
//							printf("reg->T[%X]\n", reg->T);
							break;
						case 9:
							//RD
//							printf("=9=RD\n");
							reg->CC =1;
							break;
						case 10://RSUB
//							printf("=10=RSUB");
							reg->PC = reg->L;
//							printf("reg->PC = reg->L[%X][%X]\n", reg->PC, reg->L);
							//scanf("%c",&debug);
							break;
						case 11://STA
//							printf("=11=STA");
							vm[(TA+2)/16].memory[(TA+2)%16] = reg->A % 256;
							vm[(TA+1)/16].memory[(TA+1)%16] = (reg->A/256) % 256;
							vm[TA/16].memory[TA%16] = (reg->A/256/256) % 256;
//							printf("memory value [%X][%X][%X]\n",vm[TA/16].memory[TA%16],vm[(TA+1)/16].memory[(TA+1)%16],vm[(TA+2)/16].memory[(TA+2)%16] );
							break;
						case 12://STCH (A의 rightmost BYTE) 를 memory 에 저장
//							printf("=12=STCH");
							vm[TA/16].memory[TA%16] = (reg->A) % 256;
//							printf("memory value[%X]\n",vm[TA/16].memory[TA%16] );
							break;
						case 13://STL
//							printf("=13=STL");
							vm[(TA+2)/16].memory[(TA+2)%16] = reg->L % 256;
							vm[(TA+1)/16].memory[(TA+1)%16] = (reg->L/256) % 256;
							vm[TA/16].memory[TA%16] = (reg->L/256/256) % 256;
//							printf("memory value [%X][%X][%X]\n",vm[TA/16].memory[TA%16],vm[(TA+1)/16].memory[(TA+1)%16],vm[(TA+2)/16].memory[(TA+2)%16] );
							break;
						case 14://STX
//							printf("=14=STX");
							vm[(TA+2)/16].memory[(TA+2)%16] = reg->X % 256;
							vm[(TA+1)/16].memory[(TA+1)%16] = (reg->X/256) % 256;
							vm[TA/16].memory[TA%16] = (reg->X/256/256) % 256;
//							printf("memory value [%X][%X][%X]\n",vm[TA/16].memory[TA%16],vm[(TA+1)/16].memory[(TA+1)%16],vm[(TA+2)/16].memory[(TA+2)%16] );
							break;
						case 15:
//							printf("=15=TD\n");
							//TD
							reg->CC =1;
							break;
						case 16: 
							//WD
//							printf("=16=WD\n");
							reg->CC =1;
							break;
						default:
							printf("error\n");
							break;
	
					}


					break;
			}

			LOCCTR = reg->PC;
		}
		else{
		//아니면 PC++;

//			printf("else~~\n");
//		reg->PC++;
			LOCCTR++;
		}


	}


	printf("\t  A :%010X X :%-10X\n", reg->A, reg->X);
	printf("\t  L :%010X PC:%010X\n", reg->L, reg->PC);
	printf("\t  B :%010X S :%010X\n", reg->B, reg->S);
	printf("\t  T :%010X\n", reg->T);
	printf("\tEnd program\n");
	*runrunrun =0;
	return PROGADDR;

}

//breakpoint 명령어
void l_comm_bp(LinkedList1* pList, LinkedList8* T8_List){
	ListNode1* T1_node;
	ListNode8* newnode;
	int address=0;	
	int i, hex;




	if(pList->currentElement == 1){
		T1_node = pList->headNode.link;
		if(strcmp(T1_node->comm_data, "bp") == 0){
			//display bp list
//			printf("bp 출력해야되열\n");
			T8_display(T8_List);
				
		}
	}
	else if(pList->currentElement == 2){
		T1_node = pList->headNode.link;
		T1_node = T1_node -> link;
		


		if(strcmp(T1_node->comm_data, "clear") == 0){
			//bp clear
//			printf("bp clearrrrrrrr\n");
			T8_removeElement(T8_List);
			printf("\t[ok] clear all breakpoints\n");

		}
		else{
//			printf("bp adresssssssss\n");
			for(i=strlen(T1_node->comm_data)-1, hex=1 ; i>=0 ; i--, hex*=16 ){
				if(T1_node->comm_data[i] >=48 && T1_node->comm_data[i]<=57){
					address += (T1_node->comm_data[i]-48)*hex;
				}
				else if(T1_node->comm_data[i] >= 65 && T1_node->comm_data[i] <= 70){
					address += (T1_node->comm_data[i]-55)*hex;
				}
				else if(T1_node->comm_data[i] >= 97 && T1_node->comm_data[i] <= 102){
					address += (T1_node->comm_data[i]-87)*hex;
				}
				else{
					printf("error, please input correct HEX address\n");
					return;
				}
			}

			//set bp to bp list
//			printf("address[%d][%X]\n", address, address);
			newnode = T8_createNode(address);
			T8_addElement(T8_List, newnode);
			printf("\t[ok] create breakpoint %X\n", address);

		}


	}
	
}


