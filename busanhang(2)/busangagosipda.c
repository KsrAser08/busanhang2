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
// 마동석 이동방향
#define MOVE_LEFT 1
#define MOVE_STAY 0
// 좀비의 공격대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

//변수 선언
int train_length; //열차길이
int percentile_probability; //이동 확률
int madongseok_stamina; //마동석 체력
int turn = 0;
int madongseok, zombie, citizen; //마동석 좀비 시민
int first_train; //초기 열차
int citizen_aggro = 1, madongseok_aggro = 1; //시민 마동석 어그로 (기본설정 1)
int citizen_random, madongseok_random; //시민 랜덤확률, 마동석 랜덤확률
int citizen_stay, zombie_stay, madongseok_stay; //시민 좀비 마동석 정지
int zombie_cant_move; //좀비가 움직일 수 없는 턴일 때
int before_citizen_aggro, before_madongseok_aggro; //바뀌기 전의 어그로
int madongseok_choose; //마동석 행동 or 휴식 scanf_s 선택
int madongseok_no_move; //움직이지 않는다 선택할 때
int madongseok_action_choose; //마동석 액션 선택
int before_madongseok_stamina; //바뀌기 전의 마동석 체력
int madongseok_holding_zombie = 0; //마동석이 좀비를 잡음
int zombie_cant_move_right; //좀비가 마동석이 있음으로 오른쪽으로 이동이 불가능할 때

//인트로 함수
void intro() { 
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
	 return value;
}

//캐릭터 위치 지정
int charactor_position() {
	zombie = train_length - 3;
	madongseok = train_length - 2;
	citizen = train_length - 6;
}

//열차 함수
int train(int ZOMB, int CITIZ, int MADONGS) {
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
void zombie_move() {
	if (turn % 2 != 0) { //턴이 짝수가 아닐 때
		if (1 == madongseok_holding_zombie) {//좀비가 마동석한테 붙들려 있을 때
			madongseok_holding_zombie = 1; 
		}
		else {
			if (citizen_aggro >= madongseok_aggro) { //시민 어그로가 마동석 어그로보다 크거나 같을 때
				zombie--;
			}
			else {
				if (zombie + 1 == madongseok) {
					zombie_cant_move_right++; //좀비가 오른쪽으로 이동할 때, 마동석이 뒤에 있을 경우에 정지
				}
				else {
					zombie++; 
					madongseok_holding_zombie = 0;
				}
			}
		}
	}
	else zombie_cant_move++;
}

//시민 움직이는 함수
void citizen_move() {
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

//좀비가 바로 앞에 없을 때 마동석 이동
void madongseok_move_no_zombie() {
	if (zombie != madongseok - 1) {
		while (1) {
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //앞에 좀비가 없을 때
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_LEFT) { //이동함 선택할 때
				before_madongseok_aggro = madongseok_aggro;
				madongseok_aggro++; //어그로 1증가
				if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
				madongseok--; //한 칸 왼쪽으로 이동
			}else if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
				before_madongseok_aggro = madongseok_aggro;
				madongseok_aggro--; //어그로 1감소
				if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
				before_madongseok_stamina = madongseok_stamina;
				madongseok_stamina++; //체력 1증가   
				if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
				madongseok_no_move++; // 마동석 안움직임 신호
			} if (madongseok_choose == MOVE_LEFT || madongseok_choose == MOVE_STAY) break;
		}
	}
}
//좀비가 바로 앞에 있을때 마동석 이동
void madongseok_move_with_zombie() {
	if (zombie == madongseok - 1) { //좀비가 바로 앞에 있을 경우
		while (1) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY); 
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
				before_madongseok_aggro = madongseok_aggro;
				madongseok_aggro--; //어그로 1감소
				if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
				before_madongseok_stamina = madongseok_stamina;
				madongseok_stamina++; //체력 1증가   
				if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
				madongseok_no_move++;
				if (madongseok_choose == MOVE_LEFT || madongseok_choose == MOVE_STAY) {
					break;
				}
			}
		}
	}
}
//시민 현재 상황 출력
void citizen_now() {
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
void zombie_now() {
	//좀비가 이동할 수 없는 턴일 때
	if (zombie_cant_move == 1) {
		printf("zombie : STAY -> %d(Can't Move)\n", zombie);
		zombie_cant_move = 0;
	}else if (madongseok_holding_zombie == 1) { 
		printf("zombie : STAY -> %d(madongseok hold)\n", zombie); 
		madongseok_holding_zombie = 0;
	}else if (zombie_cant_move_right == 1) {
		printf("zombie : STAY -> %d(zombie can\'t move right)\n", zombie);
		zombie_cant_move_right = 0;
	}else {
		// 좀비가 이동할 때
		if (citizen_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}else {
			printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
			zombie_stay = 0;
		}
	}
}
//마동석 현재상황 출력
void madongseok_now(){
	if (madongseok_no_move == 1) {
		if (madongseok_aggro != before_madongseok_aggro) { //어그로가 이전과 다를 때
			if (madongseok_stamina != before_madongseok_stamina) { //체력이 이전과 다를 때
				printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);              ////////////////////////
				madongseok_no_move = 0;
			}else {
				printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
				madongseok_no_move = 0;
			}
		} else if(madongseok_aggro == before_madongseok_aggro) {
			if (before_madongseok_stamina != madongseok_stamina) printf("madongseok: stay %d (aggro: %d, stamina: %d -> %d)\n\n", madongseok, madongseok_aggro, before_madongseok_stamina,madongseok_stamina);
			else printf("madongseok: stay %d (aggro: %d, stamina: %d)\n\n", madongseok, madongseok_aggro, madongseok_stamina);
		}
	}else if (madongseok_choose == MOVE_LEFT) {
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok + 1, madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
		}else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok move: %d -> %d (aggro: %d, stamina: %d)\n\n", madongseok + 1, madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
}
//시민 액션 없을 때
void result() {
	if (citizen != 1) {
		printf("citizen does nothing.\n");
	}
}
//마동석 액션
void madongseok_action() {
	if (madongseok - 1 == zombie) {
		printf("madongseok action (%d.rest, %d.provoke, %d.pull)>> ", ACTION_REST,ACTION_PROVOKE,ACTION_PULL);
		scanf_s("%d", &madongseok_action_choose);
	}
	else {
		printf("madongseok action (%d.rest, %d.provoke)>> ", ACTION_REST, ACTION_PROVOKE);
		scanf_s("%d", &madongseok_action_choose);
	}
}
//마동석 휴식 행동 결과
void madongseok_result_rest() {
	if (madongseok_action_choose == ACTION_REST) {
		printf("madongseok rests...\n");
		before_madongseok_aggro = madongseok_aggro;
		before_madongseok_stamina = madongseok_stamina;
		madongseok_aggro--;
		madongseok_stamina++;
		if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
		else if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
		else if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
		else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
			madongseok_no_move = 0;
		}
		else if (madongseok_aggro == 0) {
			if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro: %d, stamina: %d -> %d)\n\n", madongseok, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}
	}
}
//마동석 도발 행동 결과
void madongseok_result_provoke() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}
// 마동석 붙들기 행동 결과
void madongseok_result_pull() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) > madongseok_random) { //붙들기 성공
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //어그로 2 증가
			madongseok_stamina -= 1; //체력 1 감소
			madongseok_holding_zombie = 1; //좀비를 잡았다 신호 보내기
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro : %d -> %d, stamina : %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}else { //붙들기 실패
			printf("madongseok failed to pull zombie\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //어그로 2 증가
			madongseok_stamina -= 1; //체력 1 감소
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro : %d -> %d, stamina : %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}
	}
}
//마동석 체력이 0일때 판단
int madongseok_stamina_zero(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
		exit(1);
	}
	return 0;
}

//좀비 주변에 아무도 없을 때
void zombie_action_nobody() {
	if (zombie != madongseok - 1 && zombie != citizen + 1) {
		printf("zombie attacked nobody.\n\n");
	}
}

//좀비 주변에 시민 또는 마동석이 있을 때
void zombie_action_attack_citizen() {
	if (zombie == citizen + 1) {
		citizen_dead_ending();
	}
	else if (zombie == madongseok - 1) {
		madongseok_stamina--;
		if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
		printf("Zombie attacked madongseok (stamina: %d -> %d)\n", madongseok_stamina + 1, madongseok_stamina);
		if (madongseok_stamina == 0) {
			madongseok_stamina_zero();
		}
	}
}

//좀비 주변에 둘 다 있을 때 어그로 비교 및 공격
void zombie_action_aggro_fight() {
	if (citizen + 1 == zombie && zombie + 1 == madongseok) {
		if (citizen_aggro <= madongseok_aggro) {
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zomibe attacked madongseok (stamina: %d -> %d)\n", madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero();
			}
		}
		else if (citizen_aggro > madongseok_aggro) {
			citizen_dead_ending();
		}
	}
}

//시민이 이김
int citizen_win() {
	if (citizen == 1) {
		printf("citizen succeeded in escaping\n");
		printf("YOU WIN!");
		exit(1);
	}
}
//시민 죽음
int citizen_dead_ending() {
	if (citizen + 1 == zombie) {
		printf("GAME OEVER! citizen dead...\n");
		exit(1);
	}
}
//짝수턴 초기화 액션
void madongseok_action_no_even_number() {
	if (turn % 2 == 0) {
		madongseok_holding_zombie = 0;
	}
}
//줄바꾸기 함수
void changing_the_line() {
	printf("\n");
}
//메인 함수
int main (void) {
	srand((unsigned int)time(NULL)); //난수 설정
	intro(); //인트로 출력
	train_length = variables("train length", LEN_MIN, LEN_MAX); //열차 길이 입력
	madongseok_stamina = variables("madongseok stamina", STM_MIN, STM_MAX); //마동석 스테미나 입력
	percentile_probability = variables("percentile probability \'p\'", PROB_MIN, PROB_MAX); //확률 입력
	changing_the_line();
	charactor_position(zombie, citizen, madongseok);
	first_train = train(zombie, citizen, madongseok); //기본열차 출력
	changing_the_line();
	while (1) { //반복 시작
		turn++;//턴 증가

		citizen_move(); //시민이동
		zombie_move(); //좀비이동

		train(zombie, citizen, madongseok); //열차 출력

		citizen_win(); //시민이 이길 경우
		citizen_now(); //시민 현재상황 출력
		zombie_now(); //좀비 현재상황 출력

		madongseok_move_with_zombie();//마동석 주변에 좀비 있을 때 이동
		madongseok_move_no_zombie();//마동석 주변에 좀비 없을 때 이동

		train(zombie, citizen, madongseok); //두 번째 열차 출력

		changing_the_line(); //줄 변경

		madongseok_now(); //마동석 현재상황 출력

		citizen_win(); //시민이 이길 경우
		result(); //결과 출력

		zombie_action_nobody(); //좀비 행동(주변 아무도 없을때)
		zombie_action_aggro_fight(); //좀비 행동(어그로 더 높은 쪽 공격 (시민 공격시 게임 끝, 마동석 공격시 STM 1감소))
		zombie_action_attack_citizen(); //좀비 행동(시민을 물었을 때)
		
		madongseok_action(); //마동석 액션
		madongseok_action_no_even_number(); //짝수 턴일 때 실행되면 안되는 것들 초기화
		changing_the_line(); //줄 변경
		madongseok_result_rest(); //마동석 휴식 액션 결과 출력
		madongseok_result_provoke(); //마동석 도발 액션 결과 출력
		madongseok_result_pull(); //마동석 좀비 붙들기 액션 결과 출력

		madongseok_stamina_zero(); //마동석 체력이 0일 때
	}
	return 0;
}