#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 255

typedef struct{//플레이어의 정보를 포함하는 구조체 정의
	char id[MAX_SIZE];
	int age;
	int hp;
	double x,y;
	char* friend_id[MAX_SIZE];//255개의 캐릭터 포인터 배열
	int friend_num;
}Player;

void getInfo(Player* player){//사용자의 정보를 저장
	printf("ID를 입력하세요: ");
	scanf("%s", player->id);
	printf("나이를 입력하세요: ");
	scanf("%d", &player->age);
	printf("HP를 입력하세요: ");
	scanf("%d", &player->hp);
	printf("X 좌표를 입력하세요: ");
	scanf("%lf", &player->x);
	printf("Y 좌표를 입력하세요: ");
	scanf("%lf", &player->y);

	char flag;//루프 제어 변수
	char tmp[MAX_SIZE];//임시로 id문자열을 저장할 변수
	int cnt = 0;//루프를 돈 횟수 저장

	do{//사용자의 동맹 id를 입력받음 + 동적할당
		printf("동맹 ID를 입력하세요: ");
		scanf("%s", tmp);
		player->friend_id[cnt] = (char*)malloc(strlen(tmp) + 1);
		strcpy(player->friend_id[cnt], tmp);
		cnt++;
		printf("동맹을 계속 추가할까요?:");
		getchar();
		scanf("%c", &flag);
	}while(flag != 'N');

	player->friend_num = cnt;//동맹의 수 저장
}

void writeInfo(){
	Player player;
	char loop;

	FILE* fp = fopen("player.bin", "ab");//파일을 append 모드로 open
	if(fp == NULL){
		fprintf(stderr, "파일 player.bin을 열 수 없습니다.\n");
		return;
	}

	getInfo(&player);//플레이어 정보를 입력받음

	fwrite(player.id, sizeof(player.id), 1, fp);//구조체에 저장된 정보를 파일에 write
	fwrite(&player.age, sizeof(player.age), 1, fp);
	fwrite(&player.hp, sizeof(player.hp), 1, fp);
	fwrite(&player.x, sizeof(player.x), 1, fp);
	fwrite(&player.y, sizeof(player.y), 1, fp);
	fwrite(&player.friend_num, sizeof(player.friend_num), 1, fp);//동맹의 수 만큼 반복
	for(int i = 0; i < player.friend_num; i++){
		int len = strlen(player.friend_id[i]);//문자열의 길이만큼 저장
		fwrite(&len, sizeof(len), 1, fp);
		fwrite(player.friend_id[i], sizeof(char) * len, 1, fp);
		free(player.friend_id[i]);
	}

	printf("완료되었습니다.\n");
	fclose(fp);
}

void readInfo(){//플레이어 정보를 읽는 함수
	Player player_tmp;//정보를 저장할 구조체 생성

	FILE* fp = fopen("player.bin", "rb");
	if(fp == NULL){
		fprintf(stderr, "파일 player.bin을 열 수 없습니다.\n");
		return;
	}

	fread(player_tmp.id, sizeof(player_tmp.id), 1, fp);//정보를 읽어서 구조체에 저장
	fread(&player_tmp.age, sizeof(player_tmp.age), 1, fp);
	fread(&player_tmp.hp, sizeof(player_tmp.hp), 1, fp);
	fread(&player_tmp.x, sizeof(player_tmp.x), 1, fp);
	fread(&player_tmp.y, sizeof(player_tmp.y), 1, fp);
	fread(&player_tmp.friend_num, sizeof(player_tmp.friend_num), 1, fp);
	
	int read_len = 0;
	for(int i = 0; i < player_tmp.friend_num; i++){//동맹의 수만큼 반복 + 동적할당
		fread(&read_len, sizeof(read_len), 1, fp);
		player_tmp.friend_id[i] = (char*)malloc(sizeof(char)*(read_len + 1));
		fread(player_tmp.friend_id[i], read_len, 1, fp);
	}

	printf("ID: %s / ", player_tmp.id);
	printf("나이: %d / ", player_tmp.age);
	printf("HP: %d / ", player_tmp.hp);
	printf("좌표:%.3f, %.3f\n", player_tmp.x, player_tmp.y);
	printf("등록된 동맹 수: %d명\n", player_tmp.friend_num);
	
	int i = 0;
	for(i = 0; i < player_tmp.friend_num - 1; i++){
		printf("%s / ", player_tmp.friend_id[i]);
		free(player_tmp.friend_id[i]);
	}
	printf("%s\n", player_tmp.friend_id[i]);
	free(player_tmp.friend_id[i]);

	fclose(fp);
	printf("완료되었습니다.\n");
}

void printMenu(){//입력 메뉴 출력 함수
	printf("\n1. 플레이어 정보 기록\n");
	printf("2. 플레이어 정보 확인\n");
	printf("3. 종료\n");
	printf("입력하세요> ");
}

int main(int argc, char* argv[]){//메인함수
	int loop = 1;
	int input = 0;

	do{
		printMenu();
		scanf("%d", &input);
		switch(input){
			case 1: writeInfo(); break;
			case 2: readInfo(); break;
			case 3: loop = 0; break;
			default: printf("Wrong Input...\n"); break;
		}
	}while(loop != 0);

	printf("종료되었습니다.\n");
	return 0;
}
