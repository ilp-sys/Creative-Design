#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 255

typedef struct{//회원 정보를 담는 구조체
	char name[MAX_SIZE];
	char age[3];
	char mail_add[MAX_SIZE];
}member;

typedef struct node{ //링크드리스트 노드 정의
	member data;
	struct node* link;
}node;

int checkMail(char* mail){
	char ch;
	int at_pos = 0, dot_pos = 0;
	int end_pos = strlen(mail) - 1;

	for(int i = 0; i < strlen(mail); i++){
		ch = mail[i];
		if(ch == '@') at_pos = i;
		if(ch == '.') dot_pos = i;
	}

	if((at_pos == 0) || (dot_pos == end_pos) || (dot_pos - at_pos) <= 1 || (dot_pos == 0)){
		printf("잘못된 이메일 형식입니다.\n");
		return 0;
	}
	return 1;
}

void addNewMem(node* head){//새로운 회원을 추가하는 함수
	char ch;
	node *nodeptr = head;
	node *p;

	do{
		//새로운 노드 생성
		node* newnode = (node*)malloc(sizeof(node));
	  printf("이름:");	
		getchar();
		scanf("%[^\n]s", (newnode->data).name);
	  printf("나이:");	
		scanf("%s", (newnode->data).age);
		getchar();
	  printf("이메일:");	
		scanf("%s", (newnode->data).mail_add);
		getchar();
		int flag = checkMail((newnode->data).mail_add);
		if(!flag){
	  	printf("이메일:");	
			scanf("%s", (newnode->data).mail_add);
			getchar();
			int flag2 = checkMail((newnode->data).mail_add);
			if(!flag2) return;
		}

		//링크드리스트 끝에 노드 연결
		if(head == NULL){
			head = newnode;
			newnode->link = NULL;
		}
		else{
			while(nodeptr != NULL){
				p = nodeptr;
				nodeptr = nodeptr->link;
			}
			nodeptr = newnode;
			p->link = nodeptr;
		}
	
		//반복 여부 확인
		printf("계속 입력할까요? (Y/N)");
		scanf("%c", &ch);
	}while(ch != 'N');

	printf("입력이 완료되었습니다\n");
}

node* search(node *head, char* name2find){
	node *nodeptr = head;
	while(nodeptr != NULL){
		if(!strcmp((nodeptr->data).name, name2find)) return nodeptr;
		nodeptr = nodeptr->link;
	}
	return NULL;
}

void editMemInfo(node* head){
	char name2find[MAX_SIZE];
	printf("수정할 사람 이름을 입력해주세요: ");
	getchar();
	scanf("%[^\n]s", name2find);
	
	node* ptr = search(head, name2find);
	getchar();

	if(ptr == NULL){
		printf("사용자 %s는 없습니다.\n", name2find);
		return;
	}
	else{
		printf("사용자 %s를 수정합니다.\n", name2find);
	  printf("이름:");	
		scanf("%[^\n]s", (ptr->data).name);
		getchar();
	  printf("나이:");	
		scanf("%s", (ptr->data).age);
		getchar();
	  printf("이메일:");	
		scanf("%s", (ptr->data).mail_add);
		getchar();
		int flag = checkMail((ptr->data).mail_add);
		if(!flag){
	  	printf("이메일:");	
			scanf("%s", (ptr->data).mail_add);
			getchar();
			int flag2 = checkMail((ptr->data).mail_add);
			if(!flag2) return;
		}
		printf("수정 완료되었습니다\n");
	}
}

void delMemInfo(node* head){
	char name2find[MAX_SIZE];
	printf("삭제할 사람 이름을 입력해주세요: ");
	getchar();
	scanf("%[^\n]s", name2find);

	node* node2rm = search(head, name2find);
	node* ptr = head;
	node *p;

	if(ptr == NULL){
		printf("사용자 %s는 없습니다.\n", name2find);
		return;
	}
	else{
		if(ptr == node2rm){
			head = head->link;
			free(ptr);
		}
		else{
			while(ptr != NULL){
				if(ptr == node2rm){
					p->link = node2rm->link;
					free(node2rm);
					return;
				}
				p = ptr;
				ptr = ptr->link;
			}
		}
		printf("삭제 완료되었습니다.\n");
	}
}

void allMemList(node *head){
	node *p = head;
	while(p != NULL){
		printf("%s / %s / %s \n", (p->data).name, (p->data).age, (p->data).mail_add);
		p = p->link;
	}
}

void node2txt(node* head){
	//파일 쓰기 모드로 열기
	FILE* fp = NULL;
	fp = fopen("memlist.txt", "w");
	if(fp == NULL){
		fprintf(stderr, "Can not open file memlist.txt\n");
		return;
	}

	node* ptr = head;
	while(ptr != NULL){
		fputc('/',fp); fputs(ptr->data.name, fp);  fputs("\n", fp);
		fputc('/',fp); fputs(ptr->data.age, fp);  fputs("\n", fp);
		fputc('/',fp); fputs(ptr->data.mail_add, fp);  fputs("\n", fp);
		ptr = ptr->link;
	}
	fclose(fp);
}

void insert(node** head, node* newnode){ //링크드리스트 마지막에 노드 삽입
	node* ptr = *head;
	node *p;
	if(*head == NULL){
		(*head) = newnode;
		newnode->link = NULL;
	}
	else{
		while(ptr!=NULL){
			p = ptr;
			ptr = ptr->link;
		}
		ptr = newnode;
		p->link = ptr;
	}
}

void printMenu(){
	printf("\n1. 신규회원 저장\n");
	printf("2. 회원 정보 수정\n");
	printf("3. 회원 삭제\n");
	printf("4. 모든 회원 리스트\n");
	printf("5. 종료 \n");
	printf("입력하세요>");
}

int main(int argc, char* argv[]){
	
	//head 포인터 선언
	node *head = NULL;
	
	//txt파일을 가져와 노드로 구성
	//파일 읽기 모드로 열기
	int i = 0;
	FILE* fp = NULL;
	fp = fopen("memlist.txt", "r");
	if(fp == NULL){
		fprintf(stderr, "Can not open file memlist.txt\n");
		return -1;
	}
	//텍스트 파일에 담긴 정보를 노드로 구성
	while(!feof(fp)){
		char ch;
		ch = fgetc(fp); 
		if(ch == '/'){
			char name[MAX_SIZE];
			char age[3];
			char mail_add[MAX_SIZE];
			if(i%3 == 0) fgets(name, sizeof(name), fp);
			else if(i%3 == 1) fgets(age, sizeof(age), fp);
			else{ // i % 3 == 2
				int j = 0;
				node *newnode = (node*)malloc(sizeof(node));
				newnode->link = NULL;
				fgets(mail_add, sizeof(mail_add), fp);
				for(j = 0; j < MAX_SIZE; j++){
					if(name[j] == '\n') name[j] = 0;
					if(mail_add[j] == '\n') mail_add[j] = 0;
				}
				for(j = 0; j < 3; j++) if(age[j] == '\n') age[j] = 0;
				strncpy(newnode->data.name, name, sizeof(newnode->data.name));
				strncpy(newnode->data.age, age, sizeof(newnode->data.age));
				strncpy(newnode->data.mail_add, mail_add, sizeof(newnode->data.mail_add));
				insert(&head, newnode);
			}
			i++;
		}
	}
	//파일 닫기
	fclose(fp);

	//링크드리스트 연산 리스트
	int input = 0;
	int loop = 1;
	while(loop){
		printMenu();
		scanf("%d", &input);
		switch(input){
			case 1: addNewMem(head); break;
			case 2: editMemInfo(head); break;
			case 3: delMemInfo(head); break;
			case 4: allMemList(head); break;
			case 5: loop = 0; break;
			default: printf("Wrong Input...\n");
		}
	}
	
	//노드정보를 텍스트파일에 적기
	node2txt(head);

	printf("종료합니다.\n");

	return 0;
}
