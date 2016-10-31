//20101605 SPprj1
//main


#include "20101605.h"


int main(){
	//변수 선언
	int i, len;
	int sw=0, quitSW=0 , historySW=0;
	//quit, history 명령어 SW 변수
	int history_cnt = 0;	//history에 저장된 수
	int dumpidx = 0;		//dump시 참조할 address
	int comm_idx[17] = {0,0, 1,1, 2,2, 3,3, 4,4, 5,5, 6,6, 7, 8, 9};
	//0:help, 1:dir, 2:quit, 3:history, 4:dump, 5:edit, 6:fill
	//7:reset, 8:opcode mnemonic, 9:opcodelist
	int enterchk;			//command 에 enter만 입력되는 것 chk

	
	//구조체 변수 선언
	ListNode1* call_Cnode;		//입력받은 명령어가 저장되는 node

	LinkedList1 *T1_List;		//명령어 token을 저장할 linked list
	LinkedList2 *T2_List;		//History를 저장할 linked list
	VM *vmemory;				//가상 메모리 할당할 변수
	ListNode2 *T2newnode;		//history 임시 저장 node

	///////////////////////////////////////////////////


	//프로그램 초기화
	htinit();					//hash table을 초기화

	//명령어를 저장할 Linked List의 headnode를 생성
	T1_List = T1_createList();	
	//History를 저장할 Linked List의 headnode를 생성
	T2_List = T2_createList();
	
	command_init();				//명령어 정의

	vmemory = memoryinit();	//1MB의 가상메모리 할당 및 '0'으로 초기화

	//opcode.txt 읽고 hast table 만든다
	makeHashtable();

	///////////////////////////////////////////////////



	///////////////////////////////////////////////////
	//command 입력 및 실행
	while(1){
		printf("sicsim> ");
		//전역 변수 초기화
		commachk = 0;	
		errorchk = 0;
		
		//명령어 입력(return: ENTER chk)
		enterchk = input(T1_List);

		//입력받은 명령어의 comma 수 확인
		len = strlen(getcommand);
		for(i=0 ; i<len ; i++){
			if(getcommand[i] == ',')
				commachk++;
		}
		quitSW = FALSE;			//TRUE가 되면 종료	
		historySW  = FALSE;		//TRUE가 되면 history를 출력
			
		if(enterchk){
			//명령어 입력시
			switch(command_chk(T1_List)){
				case 0:
					//입력받은 명령어가 잘못된 경우
					if(commaposchk){
		//comma위치 chk한 경우 이미  error message 따로 출력됨
						error(1);
					}
					break;

				case 1:
					//정상적인 명령어를 입력받음
					call_Cnode = T1_List -> headNode.link;
					for(i=0 ; i<17 ; i++){
						if(strcmp(call_Cnode -> comm_data, comm_arr[i]) == 0){
							//comm_arr: 정의된 명령어
							//정의된 명령어와 입력받은 명령어를 비교
							//sw에 실행할 명령어의 switch를 저장
							sw = comm_idx[i];
						}
					}
					switch(sw){
						case 0:	//help
							s_comm_help();
							break;
						case 1:	//dir
							s_comm_dir();
							break;
						case 2:	//quit
							quitSW = TRUE;
							break;
						case 3:	//history
							historySW = TRUE;
							break;
						case 4:	//dump
							dumpidx = m_comm_dump(vmemory, T1_List, dumpidx);
							if(!boundarychk){
								//boundary exception chk
								dumpidx = 0;
								boundarychk = TRUE;
							}
							break;
						case 5:	//edit
							m_comm_edit(vmemory, T1_List);
							break;
						case 6:	//fill
							m_comm_fill(vmemory, T1_List);
							break;
						case 7:	//reset
							m_comm_reset(vmemory);
							break;
						case 8:	//opcode mnemonic
							o_comm_mnemonic(T1_List);
							break;
						case 9:	//opcodelist
							o_comm_list();
							break;
					}
					break;
			}
			if(!errorchk){
				//잘못된 명령어가 아니라면 history list에 저장
				history_cnt++;
				T2newnode = T2_createNode(getcommand, history_cnt);
				T2_addElement(T2_List, T2newnode);
			}
			if(historySW){
				//history 를 보여줌
				s_comm_history(T2_List);
			}
		}
		else{
			//enter 만 입력시: 아무것도 하지않음
		}
		if(quitSW == TRUE){
			//종료하기 위하여 loop 탈출
			break;
		}

		
		T1_removeElement(T1_List);	//입력받은 명령어를 초기화
		//이때 head Node는 해당 안됨

	}	
	///////////////////////////////////////////////////
	//종료전 할당된 memory를 free
	memoryfree(vmemory);		//가상 memory 할당 해제
	T1_removeElement2(T1_List);	//T1 list 할당 해제(headNode포함)
	T2_removeElement(T2_List);	//T2 List 할당 해제
	T3_removeElement();			//T3 List 할당 해제
	///////////////////////////////////////////////////
	return 0;
}


