#include "20101605.h"

void command_init(char** comm_arr){
	//명령어를 정의합니다
	
	strcpy(comm_arr[0],"h");			//0
	strcpy(comm_arr[1],"help");			//0
	strcpy(comm_arr[2],"d");			//1
	strcpy(comm_arr[3],"dir");			//1
	strcpy(comm_arr[4],"q");			//2
	strcpy(comm_arr[5],"quit");			//2
	strcpy(comm_arr[6],"hi");			//3
	strcpy(comm_arr[7],"history");		//3
	strcpy(comm_arr[8],"du");			//4
	strcpy(comm_arr[9],"dump");			//4
	strcpy(comm_arr[10],"e");			//5
	strcpy(comm_arr[11],"edit");		//5
	strcpy(comm_arr[12],"f");			//6
	strcpy(comm_arr[13],"fill");		//6
	strcpy(comm_arr[14],"reset");		//7
	strcpy(comm_arr[15],"opcode");		//8
	strcpy(comm_arr[16],"opcodelist");	//9

	//prj2 추가된 명령어
	strcpy(comm_arr[17],"assemble");	//10
	strcpy(comm_arr[18],"type");		//11
	strcpy(comm_arr[19],"symbol");		//12

	//prj3 추가된 명령어
	strcpy(comm_arr[20],"progaddr");	//13
	strcpy(comm_arr[21],"loader");		//14
	strcpy(comm_arr[22],"run");			//15
	strcpy(comm_arr[23],"bp");			//16
	
}

int input(LinkedList1* pList, char* getcommand){
	//명령어를 입력받는 함수
	char strbuff[81];
	char *token;
	int i = 0;
	char delimiters[8] = " ,\t";	//첫번째 token이후를 구분
	char delimiter1[8] = " \t";		//첫번째 token(명령어) 구분
	ListNode1 *newnode;

	fgets(getcommand, 81, stdin);		//명령어를 입력받는다
	//enter 만 입력하면 FALSE return
	if(getcommand[0]=='\n'){
		return FALSE;
	}
	getcommand[strlen(getcommand)-1] = '\0';
	//fgets는 개행 까지 입력 받으므로 \n -> \0 으로 바꿔준다

	strcpy(strbuff, getcommand);		//입력받은 명령어를 strbuff에 복사
	token = strtok(strbuff, delimiter1);//token에 분할된 문자열 저장

	newnode = T1_createNode(token);		//token으로 node 생성
	T1_addElement(pList, newnode);		//생성된 node를 headnode에 연결


//	pList(Head Node)에 node를 추가하는 함수를 call
//	이후 while에서도  node추가 함수 call
	while(1){
		token = strtok(NULL, delimiters);//token에 분할된 문자열 저장
		if(token == NULL){
			break;
		}
		else{
			newnode = T1_createNode(token);	//token으로 node 생성
			T1_addElement(pList, newnode);	//생성된 node를 headnode에 연결
		}
	}
	//명령어를 입력받았으므로 TRUE return
	return TRUE;
}

