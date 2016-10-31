//errorhandling.c

#include "20101605.h"

//각종 error 상황의 message를 출력하고
//errorchk -> TRUE 로 설정함
void error(int e){
	switch(e){
		case 0:
			printf("boundary exception!\n");
			break;
		case 1:
			printf("command error\n");
			break;
		case 2:
			printf("Hex value error(input 00~FF)\n");
			break;
		case 3:
			printf("Mnemonic doesn't exist\n");
			break;
		case 4:
			printf("error, please enter a correct Hex number\n");
			break;
		case 5:
			printf("error, arg1(start) must be lese than arg2(end)\n");
			break;
		case 6:
			printf("Put a comma between Hex numbers\n");
			break;
		
	}
	errorchk = TRUE;	
}

