#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//열차 길이
#define LEN_MIN 15
#define LEN_MAX 50
//마동석 체력
#define STM_MIN 0
#define STM_MAX 5
//확률
#define PROB_MIN 10
#define PROB_MAX 90
// 어그로 범위
#define AGGRO_MIN 0
#define AGGRO_MAX 5
// 마동석이동방향
#define MOVE_LEFT 1
#define MOVE_STAY 0
// 좀비의공격대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
// 마동석행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

//변수 선언
int train_length; //열차길이
int percentile_probability; //이동 확률
int madongseok_stamina; //마동석 체력
int turn = 1;
int madongseok, zombie, citizen; //마동석 좀비 시민
int first_train; //초기 열차
int citizen_aggro = 1, madongseok_aggro = 1; //시민 마동석 어그로 (기본설정 1)
int citizen_random, zombie_random; //시민 랜덤확률, 좀비 랜덤확률
int citizen_stay, zombie_stay, madongseok_stay; //시민 좀비 마동석 정지
int zombie_cant_move; //좀비가 움직일 수 없는 턴일 때
int before_citizen_aggro, before_madongseok_aggro; //바뀌기 전의 어그로
int madongseok_choose; //마동석 행동 or 휴식 scanf_s 선택
int madongseok_no_move; //움직이지 않는다 선택할 때

//함수 선언
int intro(); //인트로
int variables(char massage, int MIN, int MAX); //확률 입력받는 함수
int train(int ZOMB, int CITIZ, int MADONGS); //열차 출력

//인트로 함수
int intro() { 
	printf(" ------------- busanhang(2) ------------- \n");
	printf("|                                        |\n");
	printf("|         We must run the train to       |\n");
	printf("|              avoid zombies             |\n");
	printf("|                                        |\n");
	printf(" ---------------------------------------- \n");
}

//변수 입력 함수
int variables(char*massage,int MIN, int MAX) {
	int value;
	while (1) {
		printf("%s(%d ~ %d)>> ", massage, MIN, MAX);
		scanf_s("%d", &value);
		if (value >= MIN && value <= MAX) {
			break;
		}
	}
	//캐릭터 위치 지정
	madongseok = train_length - 2;
	zombie = train_length - 3;
	citizen = train_length - 6;
	return value;
}

//열차 함수
int train(int ZOMB,int CITIZ,int MADONGS) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < train_length; j++) {
			if (i == 0 || i == 2) printf("#");
			else {
				if ((j == 0) || (j == train_length - 1)) printf("#");
				else if (MADONGS == j) printf("M");
				else if (ZOMB == j) printf("Z");
				else if (CITIZ == j) printf("C");
				else printf(" ");
			}
		}
		printf("\n");
	}
}

//좀비 움직이는 함수
int zombie_move() {
	if (turn % 2 != 0) {
		zombie--;
	}
	else {
		zombie_cant_move++;
	}
}

//시민 움직이는 함수
int citizen_move() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //이동에 성공
		citizen--;
		before_citizen_aggro = citizen_aggro;
		citizen_aggro++;
		if (citizen_aggro > AGGRO_MAX) {
			citizen_aggro = AGGRO_MAX;
		}
	}
	else { //이동 실패
		citizen_stay++;
		before_citizen_aggro = citizen_aggro;
		citizen_aggro--;
		if (citizen_aggro < AGGRO_MIN) {
			citizen_aggro = AGGRO_MIN;
		}
	}
}

//마동석 움직이는 함수
int madongseok_move() {
	while (1) {
		printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT);
		scanf_s("%d", &madongseok_choose);
		if (madongseok_choose == MOVE_LEFT) { //이동함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++;
			if (madongseok_aggro > AGGRO_MAX) {
				madongseok_aggro = AGGRO_MAX;
			}
			madongseok--;
		}
		else if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--;
			madongseok_no_move++;
			if (madongseok_aggro < AGGRO_MIN) {
				madongseok_aggro = AGGRO_MIN;
			}
		} if (madongseok_choose == MOVE_LEFT || madongseok_choose == MOVE_STAY) break;
	}
}

//시민 현재 상황 출력
int citizen_now() {
	printf("\n");
	if (citizen_stay == 1) { //시민이 멈춰있을때
		if (citizen_aggro != before_citizen_aggro) {
			printf("citizen: STAY -> %d (aggro: %d -> %d)\n", citizen, before_citizen_aggro, citizen_aggro);
			citizen_stay = 0;
		} 
		else if(before_citizen_aggro == citizen_aggro) {
			printf("citizen: STAY -> %d (aggro: %d)\n", citizen, citizen_aggro);
			citizen_stay = 0;
		}
	} else { //시민이 멈춰있지 않을 때
		if (citizen_aggro == before_citizen_aggro) {
			printf("citizen: %d -> %d(aggro : %d)\n", citizen + 1, citizen, citizen_aggro);
		} else {
			printf("citizen: %d -> %d(aggro : %d -> %d)\n", citizen + 1, citizen, before_citizen_aggro, citizen_aggro);
			citizen_stay = 0;
		}
	}
}
//좀비 현재상황 출력
int zombie_now() {
	//좀비가 이동할 수 없는 턴일 때x`
	if (zombie_cant_move == 1) {
		printf("zombie : STAY -> %d(Can't Move)\n", zombie);
		zombie_cant_move = 0;
	} else {
		// 좀비가 이동할 때
		printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
		zombie_stay = 0;
	}
}
//마동석 현재상황 출력
int madongseok_now(){
	if (madongseok_no_move == 1) {
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
			madongseok_no_move = 0;
		}
		else if(madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok: stay %d (aggro: %d, stamina: %d)\n", madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
	else if (madongseok_choose == MOVE_LEFT) {
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n", madongseok + 1, madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseokL move %d -> %d (aggro: %d, stamina: %d)\n", madongseok + 1, madongseok, madongseok_aggro, madongseok_stamina);
		}
		
	}
}
//줄바꾸기 함수
int changing_the_line() {
	printf("\n");
}
int main (void) {
	srand((unsigned int)time(NULL)); //난수 설정
	intro(); //인트로 출력
	train_length = variables("train length", LEN_MIN, LEN_MAX); //열차 길이 입력
	madongseok_stamina = variables("madongseok stamina", STM_MIN, STM_MAX); //마동석 스테미나 입력
	percentile_probability = variables("percentile probability \'p\'", PROB_MIN, PROB_MAX); //확률 입력
	changing_the_line();
	first_train = train(zombie, citizen, madongseok); //기본열차 출력
	changing_the_line();
	while (turn < 20) { //반복 시작
		citizen_move();
		zombie_move();
		train(zombie, citizen, madongseok);
		citizen_now();
		zombie_now();
		madongseok_move();
		train(zombie, citizen, madongseok);
		madongseok_now();
		turn++;
	}
}