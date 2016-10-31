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



// 전역변수 및 구조체  선언


//input.c
char comm_arr[17][20];		//정의된 명령어를 저장하는 배열
char getcommand[81];		//입력받은 명령어를 저장하는 배열
int commachk;				//comma 의 수 chk 하는 변수
int errorchk;				//error 함수 작동 chk 하는 변수
int boundarychk;			//boundary chk하는 변수
int commaposchk;
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
	char format[81];
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
HT hashtable[20];
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//함수 선언

//init.c
VM* memoryinit();						
void makeHashtable();
void htinit();
int hashFunction(char* token_mne);
void memoryfree(VM* vm);	


//input.c
void command_init();
int input(LinkedList1* pList);				//


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
ListNode3* T3_createNode(unsigned char opcode, char* mnemonic, char* format);
void T3_addElement(int key, ListNode3* newnode);
void T3_display(HT ht);
void T3_removeElement();

//
void display_mnemonic(int key, char* mnemonic);


//command.c
int command_chk(LinkedList1* pList);	//입력받은 명령어 판단 함수

//Shell 관련 명령어 함수 선언
void s_comm_help();
void s_comm_dir();
void s_comm_history(LinkedList2* pList);
//Memory 관련 명령어 함수 선언
int m_comm_dump(VM* vm, LinkedList1* pList, int idx);
void m_comm_edit(VM* vm, LinkedList1* pList);
void m_comm_fill(VM* vm, LinkedList1* pList);
void m_comm_reset(VM* vm);
//opcode Table 관련 명령어 함수 선언
void o_comm_mnemonic(LinkedList1* pList);
void o_comm_list();


//errorhandling.c
void error(int e);



#endif
