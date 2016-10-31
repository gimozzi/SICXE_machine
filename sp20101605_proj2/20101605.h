//20101605 SP prj1
//20101605.h


#ifndef _20101605_H_
#define _20101605_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>



//상수 정의
#define MEM_ROW 65536		//memory row(Dec)
#define MEM_COL 16			//memory column(Dec)
#define BOUNDARY 65535		//memory boundary(Hex)
#define TABLESIZE 20		//hash table size
#define BLANK " "			//blank

#define FALSE 0				
#define TRUE 1

////////////////////
#define SYMTABSIZE 100		//SYMTAB size

// 전역변수 및 구조체  선언

//prj1에서 선언된 전역변수 -> prj2에서 삭제함
//input.c
//char comm_arr[20][20];		//정의된 명령어를 저장하는 배열
//char getcommand[81];		//입력받은 명령어를 저장하는 배열
//int commachk;				//comma 의 수 chk 하는 변수
//int errorchk;				//error 함수 작동 chk 하는 변수
//int boundarychk;			//boundary chk하는 변수
//int commaposchk;
//command.c

//errorhandling.c


//list.c

//Type1
//명령어 token을 저장할 Linked list structure
typedef struct ListNodeType1 ListNode1;
struct ListNodeType1{
	char comm_data[81];	
	struct ListNodeType1* link;
};
//type1의 head node
typedef struct T1_Head LinkedList1;
struct T1_Head{
	int currentElement;		//저장된 명령어의 개수
	ListNode1 headNode;		//head node
};


//Type2
//History를 저장할 Linked list structure
typedef struct ListNodeType2 ListNode2;
struct ListNodeType2{
	char comm_data[81];
	int order;
	struct ListNodeType2* link;
};
//type2의 head node
typedef struct T2_Head LinkedList2;
struct T2_Head{
	ListNode2 headNode;
};


//Type3
//Hash Table을 구성할 Linked list structure
typedef struct ListNodeType3 ListNode3;
struct ListNodeType3{
	unsigned char opcode;
	char mnemonic[81];
//	char format[81];
	int format;
	int opnum;	//operand 수
	struct ListNodeType3* link;
};
//type3의 head node
typedef struct T3_Head LinkedList3;
struct T3_Head{
	int currentElement;
	ListNode3 headNode;
};

//init.c
//memory 구조체
typedef struct VMemory VM;
struct VMemory{
	unsigned char memory[MEM_COL];
};
//hash table(head) 구조체
typedef struct HashTable HT;
struct HashTable{
	LinkedList3 head;
};
//hash table(head node) 선언
//HT hashtable[20];


///////////////////////
/////////prj2 구조체 선언

//Type4
//Hash Table(SYMTAB)을 구성할 Linked list structure
typedef struct ListNodeType4 ListNode4;
struct ListNodeType4{
	char label[7];	//
	int loc;		//address
	int flag;
	struct ListNodeType4* link;
};
//type4의 head node
typedef struct T4_Head LinkedList4;
struct T4_Head{
	int currentElement;
	ListNode4 headNode;
};

//hash table(head) 구조체
typedef struct HashTable2 HT2;
struct HashTable2{
	LinkedList4 head;	
};
//hash table(head node) 선언
//HT2 SYMTAB[100];

//Type5
//intermediate file 을 구성할 Linked list
typedef struct ListNodeType5 ListNode5;
struct ListNodeType5{
	int line;
	int loc;
	int PC;
//	char loc[6];//LOC는 16진수로 표현되나 10진수로 저장후 출력만 16진수로
	char label[7];
	char inst[7];
	char operand[10];
	char objectcode[9];
	char comment[81];
	
	int len;
	int commentchk;	
	struct ListNodeType5* link;
};
//type5의 head node
typedef struct T5_Head LinkedList5;
struct T5_Head{
	ListNode5 headNode;
};

//Type6
//정렬된 SYMTAB 저장
typedef struct ListNodeType6 ListNode6;
struct ListNodeType6{
	char label[7];
	int loc;
	struct ListNodeType6* link;
};
//type6의 head node
typedef struct T6_Head LinkedList6;
struct T6_Head{
	int currentElement;
	ListNode6 headNode;
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//함수 선언

//init.c
VM* memoryinit();	
void makeHashtable(HT* hashtable);
void htinit(HT* hashtable);
int hashFunction(char* token_mne);
void memoryfree(VM* vm);	


//input.c
void command_init(char** comm_arr);
int input(LinkedList1* pList, char* getcommand);	


//list.c
//Type1
LinkedList1* T1_createList();
ListNode1* T1_createNode(char* token);
void T1_addElement(LinkedList1* pList, ListNode1* newnode);
void T1_removeElement(LinkedList1* pList);
void T1_removeElement2(LinkedList1* pList);

//Type2
LinkedList2* T2_createList();
ListNode2* T2_createNode(char* token, int cnt);
void T2_addElement(LinkedList2* pList2, ListNode2* newnode);
void T2_removeElement(LinkedList2* pList);
void T2_display(LinkedList2* pList);
//Type3
LinkedList3* T3_createList();
ListNode3* T3_createNode(unsigned char opcode, char* mnemonic, int format, int opnum);
void T3_addElement(int key, ListNode3* newnode, HT* hashtable);
void T3_display(HT ht);
void T3_removeElement(HT* hashtable);

//
void display_mnemonic(int key, char* mnemonic, int* errorchk, HT* hashtable);


//command.c
int command_chk(LinkedList1* pList, int* commachk, int* errorchk, int* commaposchk, char* getcommand, char** comm_arr);
//입력받은 명령어 판단 함수


//Shell 관련 명령어 함수 선언
void s_comm_help();
void s_comm_dir();
void s_comm_history(LinkedList2* pList);
//Memory 관련 명령어 함수 선언
int m_comm_dump(VM* vm, LinkedList1* pList, int idx, int* commachk, int* errorchk, int* boundarychk);
void m_comm_edit(VM* vm, LinkedList1* pList, int* commachk, int* errorchk);
void m_comm_fill(VM* vm, LinkedList1* pList, int* commachk, int* errorchk);
void m_comm_reset(VM* vm);
//opcode Table 관련 명령어 함수 선언
void o_comm_mnemonic(LinkedList1* pList, int* errorchk, HT* hashtable);
void o_comm_list(HT* hashtable);


//errorhandling.c
void error(int e, int* errorchk);


///////////////
//prj2 추가된 함수

//command2.c 
int hashFunction2(char* token);
int FindSYM(HT2* SYMTAB, char* token);
int ret_SYM_loc(HT2* SYMTAB, char* token);
int FindOPTAB(HT* OPTAB, char* token);
int FindOPNUM(HT* OPTAB, char* token);
int ret_OPCODE(HT* OPTAB, char* token);
void a_comm_asm(LinkedList1* pList, HT* hashtable, HT2* SYMTAB, LinkedList5* INTER);
void a_comm_type(LinkedList1* pList);
void a_comm_symbol(HT2* SYMTAB, LinkedList6* SORT);


//list2.c
//Type4
LinkedList4* T4_createList();
ListNode4* T4_createNode(char* label, int loc, int flag);
void T4_addElement(int key, ListNode4* newnode, HT2* SYMTAB);
void T4_display(HT2 ht);
void T4_removeElement(HT2* SYMTAB);



//
//void display_mnemonic(int key, char* mnemonic);

//Type5
LinkedList5* T5_createList();
ListNode5* T5_createNode(int line, int loc, /* char* loc,*/ char* label, char* inst, char* operand, char* objectcode, char* comment, int commentchk, int PC);
void T5_addElement(LinkedList5* pList, ListNode5* newnode);
void T5_removeElement(LinkedList5* pList);
void T5_removeElement2(LinkedList5* pList);


//Type6
LinkedList6* T6_createList();
ListNode6* T6_createNode(char* label, int loc);
void T6_addElement(LinkedList6* SORT, ListNode6* newnode);
void T6_display(LinkedList6* SORT);
void T6_removeElement(LinkedList6* pList);
void T6_removeElement2(LinkedList6* pList);


#endif
