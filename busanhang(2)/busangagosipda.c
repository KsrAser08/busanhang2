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

int train_length; //열차길이
int percentile_probability; //이동 확률
int madongseok_stamina; //마동석 체력
int madongseok_stamina_get; //마동석 스테미나 기본지정
int turn = 0; //몇번 째 턴인지
int madongseok, zombie, citizen1, villain; //마동석 좀비 시민 빌런
int citizen2, citizen3; //시민2 시민3
int change;//시민 빌런 위치 바꾸기
int first_train; //초기 열차
int citizen1_aggro = 1, madongseok_aggro = 1, villain_aggro = 1; //시민 마동석 어그로 (기본설정 1)
int citizen_random, madongseok_random, villain_random; //시민 랜덤확률, 마동석 랜덤확률, 빌런 랜덤확률
int citizen1_stay, zombie_stay, madongseok_stay, villain_stay; //시민 좀비 마동석 빌런 정지
int zombie_cant_move; //좀비가 움직일 수 없는 턴일 때
int before_citizen1_aggro, before_madongseok_aggro, before_villain_aggro; //바뀌기 전의 어그로
int madongseok_choose; //마동석 행동 or 휴식 scanf_s 선택
int madongseok_no_move; //움직이지 않는다 선택할 때
int madongseok_action_choose; //마동석 액션 선택
int before_madongseok_stamina; //바뀌기 전의 마동석 체력
int madongseok_holding_zombie = 0; //마동석이 좀비를 잡음
int zombie_cant_move_right; //좀비가 마동석이 있음으로 오른쪽으로 이동이 불가능할 때
int zombie_attack_of_madongseok; //좀비가 시민과 마동석 둘다 인접해있을 때 마동석을 때림
int villain_now_dead = 0; //빌런 죽었을 때
int citizen_move_villain_move; //시민이 움직일 때 빌런도 움직임
int round = 1;// 몇라운드인지 판단
int before_citizen1_place, before_citizen2_place, before_citizen3_place; //시민들이 움직였나? 체크
int citizen_die, citizen_left; //탈출 혹은 사망 정리
int citizen_count = 3; //사람 수 카운트
int citizen2_aggro = 1, before_citizen2_aggro, citizen2_stay; //시민 2 관련 변수
int citizen3_aggro = 1, before_citizen3_aggro, citizen3_stay; //시민 3 관련 변수

//함수 프로토타입
void round1();
void round2();
void round3();

//공통으로 사용되는 함수들

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
int variables(char* massage, int MIN, int MAX) {
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

//시민이 이김 
int citizen_win() {
	if (citizen1 == 1) {
		printf("\ncitizen succeeded in escaping\n");
		printf("YOU WIN!");
		round++;
	}
}

//시민 죽음 
int citizen_dead_ending() {
	if (citizen1 + 1 == zombie) {
		printf("zombie attacked the citizen.\n");
		printf("\nGAME OEVER! citizen dead...\n");
		round++;
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

//게임 기본값 초기화 함수
void game_reset() {
	before_citizen1_aggro = 1;
	citizen1_aggro = 1;
	before_madongseok_aggro = 1;
	madongseok_aggro = 1;
	madongseok_stamina = madongseok_stamina_get;
	turn = 0;
}



//부산헹(2) 함수들

//캐릭터 위치 지정 1
int charactor_position() {
	zombie = train_length - 3;
	madongseok = train_length - 2;
	citizen1 = train_length - 6;
}

//열차 함수 1
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

//좀비 움직이는 함수 1
void zombie_move() {
	if (turn % 2 != 0) { //턴이 짝수가 아닐 때
		if (1 == madongseok_holding_zombie) {//좀비가 마동석한테 붙들려 있을 때
			madongseok_holding_zombie = 1;
		}
		else {
			if (citizen1_aggro >= madongseok_aggro) { //시민 어그로가 마동석 어그로보다 크거나 같을 때
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

//시민 움직이는 함수 1
void citizen_move() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //이동에 성공
		citizen1--;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro++;
		if (citizen1_aggro > AGGRO_MAX) {
			citizen1_aggro = AGGRO_MAX;
		}
	}
	else { //이동 실패
		citizen1_stay++;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro--;
		if (citizen1_aggro < AGGRO_MIN) {
			citizen1_aggro = AGGRO_MIN;
		}
	}
}

//좀비가 바로 앞에 없을 때 마동석 이동 1
void madongseok_move_no_zombie() {
	if (zombie != madongseok - 1) {
		while (1) {
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //앞에 좀비가 없을 때
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY || madongseok_choose == MOVE_LEFT) break;
		}
		if (madongseok_choose == MOVE_LEFT) { //이동함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++; //어그로 1증가
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			madongseok--; //한 칸 왼쪽으로 이동
		}
		else if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //어그로 1감소
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_no_move++; // 마동석 안움직임 신호
		}
	}
}

//좀비가 바로 앞에 있을때 마동석 이동 1
void madongseok_move_with_zombie() {
	if (zombie == madongseok - 1) { //좀비가 바로 앞에 있을 경우
		while (1) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY) break;
		}
		if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //어그로 1감소
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_stamina++; //체력 1증가   
			if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			madongseok_no_move++;
		}

	}
}

//시민 현재 상황 출력 1
void citizen_now() {
	printf("\n");
	if (citizen1_stay == 1) { //시민이 멈춰있을때
		if (citizen1_aggro != before_citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
		else if (before_citizen1_aggro == citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d)\n", citizen1, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
	else { //시민이 멈춰있지 않을 때
		if (citizen1_aggro == before_citizen1_aggro) {
			printf("citizen: %d -> %d(aggro : %d)\n", citizen1 + 1, citizen1, citizen1_aggro);
		}
		else {
			printf("citizen: %d -> %d(aggro : %d -> %d)\n", citizen1 + 1, citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
}

//좀비 현재상황 출력 1
void zombie_now() {
	//좀비가 이동할 수 없는 턴일 때
	if (zombie_cant_move == 1) {
		printf("zombie : STAY -> %d(Can't Move)\n", zombie);
		zombie_cant_move = 0;
	}
	else if (madongseok_holding_zombie == 1) {
		printf("zombie : STAY -> %d(madongseok hold)\n", zombie);
		madongseok_holding_zombie = 0;
	}
	else if (zombie_cant_move_right == 1) {
		printf("zombie : STAY -> %d(zombie can\'t move right)\n", zombie);
		zombie_cant_move_right = 0;
	}
	else {
		// 좀비가 이동할 때
		if (citizen1_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}
		else {
			printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
			zombie_stay = 0;
		}
	}
}

//마동석 현재상황 출력 1
void madongseok_now() {
	if (madongseok_no_move == 1) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) { //어그로가 이전과 다를 때
			printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
			madongseok_no_move = 0;
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok: stay %d (aggro: %d, stamina: %d)\n\n", madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
	else if (madongseok_choose == MOVE_LEFT) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok + 1, madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok move: %d -> %d (aggro: %d, stamina: %d)\n\n", madongseok + 1, madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
}

//시민 액션 없을 때 1
void result() {
	if (citizen1 != 1) {
		printf("citizen does nothing.\n");
	}
}

//마동석 액션 1
void madongseok_action() {
	if (madongseok - 1 == zombie) {
		while (1) {
			printf("madongseok action (%d.rest, %d.provoke, %d.pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
			scanf_s("%d", &madongseok_action_choose);
			if (madongseok_action_choose == ACTION_REST || madongseok_action_choose == ACTION_PROVOKE || madongseok_action_choose == ACTION_PULL) break;
		}
	}
	else {
		while (1) {
			printf("madongseok action (%d.rest, %d.provoke)>> ", ACTION_REST, ACTION_PROVOKE);
			scanf_s("%d", &madongseok_action_choose);
			if (madongseok_action_choose == ACTION_REST || madongseok_action_choose == ACTION_PROVOKE) break;
		}
	}
}

//마동석 휴식 행동 결과 1
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

//마동석 도발 행동 결과 1
void madongseok_result_provoke() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}

// 마동석 붙들기 행동 결과 1
void madongseok_result_pull() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) >= madongseok_random) { //붙들기 성공
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //어그로 2 증가
			madongseok_stamina -= 1; //체력 1 감소
			madongseok_holding_zombie = 1; //좀비를 잡았다 신호 보내기
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro : %d -> %d, stamina : %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}
		else { //붙들기 실패
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

//마동석 체력이 0일때 판단 1
int madongseok_stamina_zero(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
		round++;
	}
}

//좀비 주변에 아무도 없을 때 1
void zombie_action_nobody() {
	if (zombie != madongseok - 1 && zombie != citizen1 + 1) {
		printf("zombie attacked nobody.\n\n");
	}
}

//좀비 주변에 시민 또는 마동석이 있을 때 1
void zombie_action_attack_citizen() {
	if (zombie_attack_of_madongseok == 1) {

	}
	else {
		if (zombie == citizen1 + 1) {
			citizen_dead_ending();
		}
		else if (zombie == madongseok - 1) {
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zomibe attacked madongseok (stamina: %d -> %d)\n", madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero();
			}
		}
	}
}

//좀비 주변에 둘 다 있을 때 어그로 비교 및 공격 1
void zombie_action_aggro_fight() {
	if (citizen1 + 1 == zombie && zombie + 1 == madongseok) {
		if (citizen1_aggro <= madongseok_aggro) {
			zombie_attack_of_madongseok = 1;
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", citizen1_aggro, madongseok_aggro, madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero();
			}
		}
		else {
			citizen_dead_ending();
		}
	}
}

//몇 라운드인지 확인하는 함수 1
void round_chack_round1() {
	if (round == 2) {
		game_reset();
		printf("\n\n\n\n");
		printf("* ROUND 2\n");
		round2();
	}
}



// 부산헹(3) 빌런 추가 함수들

//캐릭터 위치 지정 2
int charactor_position_round2() {
	zombie = train_length - 3;
	madongseok = train_length - 2;
	citizen1 = train_length - 6;
	villain = train_length - 5;
}

//열차 함수 2
int train_round2(int ZOMB, int CITIZ, int MADONGS, int VILL) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < train_length; j++) {
			if (i == 0 || i == 2) printf("#");
			else {
				if ((j == 0) || (j == train_length - 1)) printf("#");
				else if (MADONGS == j) printf("M");
				else if (ZOMB == j) printf("Z");
				else if (CITIZ == j) printf("C");
				else if (VILL == j) printf("V");
				else printf(" ");
			}
		}
		printf("\n");
	}
}

//좀비 움직이는 함수 2
void zombie_move_round2() {
	if (turn % 2 != 0) { //턴이 짝수가 아닐 때
		if (1 == madongseok_holding_zombie) {//좀비가 마동석한테 붙들려 있을 때
			madongseok_holding_zombie = 1;
		}
		else {
			if (citizen1_aggro >= madongseok_aggro) { //시민 어그로가 마동석 어그로보다 크거나 같을 때
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

//시민 움직이는 함수 2
void citizen_move_round2() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //이동에 성공
		citizen_move_villain_move = 1;
		citizen1--;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro++;
		if (citizen1_aggro > AGGRO_MAX) citizen1_aggro = AGGRO_MAX;
	}
	else { //이동 실패
		citizen_move_villain_move = 2;
		citizen1_stay = 1;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro--;
		if (citizen1_aggro < AGGRO_MIN) citizen1_aggro = AGGRO_MIN;
	}
}

//빌런 움직이는 함수 2
void villain_move_round2() {
	if (villain_now_dead == 1) {

	}
	else {
		if (villain != -1) {
			if (citizen_move_villain_move == 1) {
				citizen_move_villain_move = 0;
				villain--;
				before_villain_aggro = villain_aggro;
				villain_aggro++;
				if (villain_aggro > AGGRO_MAX) villain_aggro = AGGRO_MAX;
			}
			else if (citizen_move_villain_move = 2) {
				citizen_move_villain_move = 0;
				villain_stay++;
				before_villain_aggro = villain_aggro;
				villain_aggro--;
				if (villain_aggro < AGGRO_MIN) villain_aggro = AGGRO_MIN;
			}
		}
	}
}

//좀비가 바로 앞에 없을 때 마동석 이동 2
void madongseok_move_no_zombie_round2() {
	if (zombie != madongseok - 1) {
		while (1) {
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //앞에 좀비가 없을 때
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY || madongseok_choose == MOVE_LEFT) break;
		}
		if (madongseok_choose == MOVE_LEFT) { //이동함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++; //어그로 1증가
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			madongseok--; //한 칸 왼쪽으로 이동
		}
		else if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //어그로 1감소
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_no_move++; // 마동석 안움직임 신호
		}
	}
}

//좀비가 바로 앞에 있을때 마동석 이동 2
void madongseok_move_with_zombie_round2() {
	if (zombie == madongseok - 1) { //좀비가 바로 앞에 있을 경우
		while (1) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY) break;
		}
		if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //어그로 1감소
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_stamina++; //체력 1증가   
			if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			madongseok_no_move++;
		}

	}
}

//시민 현재 상황 출력 2
void citizen_now_round2() {
	printf("\n");
	if (citizen1_stay == 1) { //시민이 멈춰있을때
		if (citizen1_aggro != before_citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
		else if (before_citizen1_aggro == citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d)\n", citizen1, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
	else { //시민이 멈춰있지 않을 때
		if (citizen1_aggro == before_citizen1_aggro) {
			printf("citizen: %d -> %d(aggro : %d)\n", citizen1 + 1, citizen1, citizen1_aggro);
		}
		else {
			printf("citizen: %d -> %d(aggro : %d -> %d)\n", citizen1 + 1, citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
}

//빌런 현재상황 출력 2
void villain_now_round2() {
	if (villain_now_dead != 1) {
		if (villain != -1) {

			if (villain_stay == 1) { //시민이 멈췄음, 빌런도 멈추니 빌런이 멈춰있을때
				if (villain_aggro != before_villain_aggro) {
					printf("villain: stay %d (aggro: %d -> %d)\n", villain, before_villain_aggro, villain_aggro);
					villain_stay = 0;
				}
				else if (before_villain_aggro == villain_aggro) {
					printf("villain: stay %d (aggro: %d)\n", villain, villain_aggro);
					villain_stay = 0;
				}
			}
			else { //빌런이 멈춰있지 않을 때
				if (citizen1_aggro == before_citizen1_aggro) {
					printf("villain: %d -> %d(aggro : %d)\n", villain + 1, villain, villain_aggro);
				}
				else {
					printf("villain: %d -> %d(aggro : %d -> %d)\n", villain + 1, villain, before_villain_aggro, villain_aggro);
					citizen1_stay = 0;
				}
			}
		}
	}
}

//좀비 현재상황 출력 2
void zombie_now_round2() {
	//좀비가 이동할 수 없는 턴일 때
	if (zombie_cant_move == 1) {
		printf("zombie : STAY -> %d(Can't Move)\n", zombie);
		zombie_cant_move = 0;
	}
	else if (madongseok_holding_zombie == 1) {
		printf("zombie : STAY -> %d(madongseok hold)\n", zombie);
		madongseok_holding_zombie = 0;
	}
	else if (zombie_cant_move_right == 1) {
		printf("zombie : STAY -> %d(zombie can\'t move right)\n", zombie);
		zombie_cant_move_right = 0;
	}
	else {
		// 좀비가 이동할 때
		if (citizen1_aggro < madongseok_aggro || villain_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}
		else {
			printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
			zombie_stay = 0;
		}
	}
}

//마동석 현재상황 출력 2
void madongseok_now_round2() {
	if (madongseok_no_move == 1) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) { //어그로가 이전과 다를 때
			printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
			madongseok_no_move = 0;
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok: stay %d (aggro: %d, stamina: %d)\n\n", madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
	else if (madongseok_choose == MOVE_LEFT) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok + 1, madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok move: %d -> %d (aggro: %d, stamina: %d)\n\n", madongseok + 1, madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
}

//시민 액션 없을 때 2
void citizen_action_round2() {
	if (citizen1 != 1) {
		printf("citizen does nothing.\n");
	}
}

//마동석 액션 2
void madongseok_action_round2() {
	if (madongseok - 1 == zombie) {
		while (1) {
			printf("madongseok action (%d.rest, %d.provoke, %d.pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
			scanf_s("%d", &madongseok_action_choose);
			if (madongseok_action_choose == ACTION_REST || madongseok_action_choose == ACTION_PROVOKE || madongseok_action_choose == ACTION_PULL) break;
		}
	}
	else {
		while (1) {
			printf("madongseok action (%d.rest, %d.provoke)>> ", ACTION_REST, ACTION_PROVOKE);
			scanf_s("%d", &madongseok_action_choose);
			if (madongseok_action_choose == ACTION_REST || madongseok_action_choose == ACTION_PROVOKE) break;
		}
	}
}

//마동석 휴식 행동 결과 2
void madongseok_result_rest_round2() {
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

//마동석 도발 행동 결과 2
void madongseok_result_provoke_round2() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}

// 마동석 붙들기 행동 결과 2
void madongseok_result_pull_round2() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) >= madongseok_random) { //붙들기 성공
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //어그로 2 증가
			madongseok_stamina -= 1; //체력 1 감소
			madongseok_holding_zombie = 1; //좀비를 잡았다 신호 보내기
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro : %d -> %d, stamina : %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}
		else { //붙들기 실패
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

//마동석 체력이 0일때 판단 2
int madongseok_stamina_zero_round2(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
	}
	round = 3;
}

//좀비 주변에 아무도 없을 때 2
void zombie_action_nobody_round2() {
	if (zombie != madongseok - 1 && zombie != citizen1 + 1 && zombie != villain + 1) {
		printf("zombie attacked nobody.\n\n");
	}
}

//좀비 주변에 시민, 빌런 또는 마동석이 있을 때 2
void zombie_action_attack_citizen_round2() {
	if (zombie_attack_of_madongseok == 1) {

	}
	else {
		if (citizen1 + 1 == zombie) {
			printf("zombie attack citizen...\n");
			printf("YOU LOSE");
			game_reset();
			round3();
		}
		else if (villain + 1 == zombie) {
			villain_now_dead = 1;
			villain = -1;
			printf("zombie attack villain..\n");
			printf("villain is now dead..\n");

		}
		else if (zombie == madongseok - 1) {
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zomibe attacked madongseok (stamina: %d -> %d)\n", madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero();
			}
		}
	}
}

//좀비 주변에 둘 다 있을 때 어그로 비교 및 공격 2
void zombie_action_aggro_fight_round2() {
	if (citizen1 + 1 == zombie && zombie + 1 == madongseok) {
		if (citizen1_aggro <= madongseok_aggro) {
			zombie_attack_of_madongseok = 1;
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", citizen1_aggro, madongseok_aggro, madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero();
			}
		}
		else {
			printf("zombie attack citizen...\n");
			printf("YOU LOSE");
			game_reset();
			round3();
		}
	}
	else if (villain + 1 == zombie && zombie + 1 == madongseok) {
		if (villain_aggro <= madongseok_aggro) {
			zombie_attack_of_madongseok = 1;
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", villain_aggro, madongseok_aggro, madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero();
			}
		}
		else {
			villain_now_dead = 1;
		}
	}
}

//빌런 액션 2
void villain_action_round2() {
	if (villain != -1) {
		if (villain_now_dead != 1) {
			villain_random = rand() % 100;
			if (villain - 1 == citizen1 || villain + 1 == citizen1) {
				printf("villain try 'bar arm'\n");
				if (villain_random < 30) {
					printf("Villain successfully caused 'bar arm'.\n");
					change = villain;
					villain = citizen1;
					citizen1 = change;
				}
				else {
					printf("The villain failed to cause 'bar arm'.\n");
				}
			}
		}
		else if (villain_now_dead == 1) {
			printf("villain was dead.\n");
		}
	}
}

//시민이 이김 2
int citizen_win_round2() {
	if (citizen1 == 1) {
		printf("\ncitizen succeeded in escaping\n");
		printf("YOU WIN!");
		game_reset();
		round3();
	}
	if (villain == 1) {
		printf("\nvillain escape before the citizen.\n");
		villain = -1;
	}
}



//부산헹(3) 시민'들' 함수

// 시민2, 시민3 위치지정 3
void citizens_line_round3() {
	while (1) {
		citizen2 = rand() % train_length / 4;
		citizen3 = rand() % train_length / 2;
		if (citizen2 != 0 && citizen3 != 0 && citizen2 != citizen3) {
			break;
		}
	}

}

//캐릭터 위치 지정 3
int charactor_position_round3() {
	citizen1 = train_length - 6;
	zombie = train_length - 3;
	madongseok = train_length - 2;
}

//열차 함수 3
int train_round3(int ZOMB, int CITIZ1, int CITIZ2, int CITIZ3, int MADONGS) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < train_length; j++) {
			if (i == 0 || i == 2) printf("#");
			else {
				if ((j == 0) || (j == train_length - 1)) printf("#");
				else if (MADONGS == j) printf("M");
				else if (ZOMB == j) printf("Z");
				else if (CITIZ1 == j) printf("C");
				else if (CITIZ2 == j) printf("C");
				else if (CITIZ3 == j) printf("C");
				else printf(" ");
			}
		}
		printf("\n");
	}
}

//좀비 움직이는 함수 3
void zombie_move_round3() {
	if (turn % 2 != 0) { //턴이 짝수가 아닐 때
		if (1 == madongseok_holding_zombie) {//좀비가 마동석한테 붙들려 있을 때
			madongseok_holding_zombie = 1;
		}
		else {
			if (citizen1_aggro >= madongseok_aggro) { //시민 어그로가 마동석 어그로보다 크거나 같을 때
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

//시민1 움직이는 함수 3
void citizen1_move_round3() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //이동에 성공
		if (citizen1 != 1) {
			before_citizen1_place = citizen1;
			citizen1--;
			before_citizen1_aggro = citizen1_aggro;
			citizen1_aggro++;
			if (citizen1_aggro > AGGRO_MAX) {
				citizen1_aggro = AGGRO_MAX;
			}
		}
	}
	else { //이동 실패
		before_citizen1_place = citizen1;
		citizen1_stay++;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro--;
		if (citizen1_aggro < AGGRO_MIN) {
			citizen1_aggro = AGGRO_MIN;
		}
	}
}

//시민2 움직이는 함수 3
void citizen2_move_round3() {
	citizen_random = rand() % 101;
	if (citizen2 != 1) {
		if (citizen1 + 1 == citizen2 || citizen3 + 1 == citizen2) {
			citizen2_stay = 1;
		}
		else {
			if ((100 - percentile_probability) >= citizen_random) { //이동에 성공
				before_citizen2_place = citizen2;
				citizen2--;
				before_citizen2_aggro = citizen2_aggro;
				citizen2_aggro++;
				if (citizen2_aggro > AGGRO_MAX) {
					citizen2_aggro = AGGRO_MAX;
				}
			}
			else { //이동 실패
				before_citizen2_place = citizen2;
				citizen2_stay = 1;
				before_citizen2_aggro = citizen2_aggro;
				citizen2_aggro--;
				if (citizen2_aggro < AGGRO_MIN) {
					citizen2_aggro = AGGRO_MIN;
				}
			}
		}
	}
	else if (citizen2 == 1) {
		before_citizen2_place = citizen2;
	}
	else if (citizen2 == -1) {
		citizen2 = -1;
	}
}

//시민3 움직이는 함수 3
void citizen3_move_round3() {
	citizen_random = rand() % 101;
	if (citizen3 != 1) {
		if (citizen1 + 1 == citizen3 || citizen2 + 1 == citizen3) {
			citizen3_stay = 1;
		}
		else {
			if ((100 - percentile_probability) >= citizen_random) { //이동에 성공
				before_citizen3_place = citizen3;
				citizen3--;
				before_citizen3_aggro = citizen3_aggro;
				citizen3_aggro++;
				if (citizen3_aggro > AGGRO_MAX) {
					citizen3_aggro = AGGRO_MAX;
				}
			}
			else { //이동 실패
				before_citizen3_place = citizen3;
				citizen3_stay = 1;
				before_citizen3_aggro = citizen3_aggro;
				citizen3_aggro--;
				if (citizen3_aggro < AGGRO_MIN) {
					citizen3_aggro = AGGRO_MIN;
				}
			}
		}
	}
	else if (citizen3 == 1) {
		before_citizen3_place = citizen3;
	}
	else if (citizen3 == -1) {
		citizen3 = -1;
	}
}

//좀비가 바로 앞에 없을 때 마동석 이동 3
void madongseok_move_no_zombie_round3() {
	if (zombie != madongseok - 1) {
		while (1) {
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //앞에 좀비가 없을 때
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY || madongseok_choose == MOVE_LEFT) break;
		}
		if (madongseok_choose == MOVE_LEFT) { //이동함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++; //어그로 1증가
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			madongseok--; //한 칸 왼쪽으로 이동
		}
		else if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //어그로 1감소
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_no_move++; // 마동석 안움직임 신호
		}
	}
}

//좀비가 바로 앞에 있을때 마동석 이동 3
void madongseok_move_with_zombie_round3() {
	if (zombie == madongseok - 1) { //좀비가 바로 앞에 있을 경우
		while (1) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY) break;
		}
		if (madongseok_choose == MOVE_STAY) { //이동안함 선택할 때
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //어그로 1감소
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_stamina++; //체력 1증가   
			if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			madongseok_no_move++;
		}

	}
}

//시민1 현재 상황 출력 3
void citizen1_now_round3() {
	if (before_citizen1_place == citizen1) {
		printf("citizen 1: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
	}
	else if (before_citizen1_place != citizen1) {
		printf("citizen 1: %d -> %d(aggro : %d -> %d)\n", citizen1 + 1, citizen1, before_citizen1_aggro, citizen1_aggro);
	}
	else {
		if (citizen1_stay == 1) { //시민이 멈춰있을때
			if (citizen1_aggro != before_citizen1_aggro) {
				printf("citizen 1: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
				citizen1_stay = 0;
			}
			else if (before_citizen1_aggro == citizen1_aggro) {
				printf("citizen 1: STAY -> %d (aggro: %d)\n", citizen1, citizen1_aggro);
				citizen1_stay = 0;
			}
		}
		else { //시민이 멈춰있지 않을 때
			if (citizen1_aggro == before_citizen1_aggro) {
				printf("citizen 1: %d -> %d(aggro : %d)\n", citizen1 + 1, citizen1, citizen1_aggro);
				citizen1_stay = 0;
			}
			else {
				printf("citizen 1: %d -> %d(aggro : %d -> %d)\n", citizen1 + 1, citizen1, before_citizen1_aggro, citizen1_aggro);
				citizen1_stay = 0;
			}
		}
	}
}

//시민2 현재 상황 출력 3
void citizen2_now_round3() {
	if (citizen2 != -1) {
		if (before_citizen2_place == citizen2 || citizen2_stay == 1) {
			printf("citizen 2: STAY -> %d (aggro: %d -> %d)\n", citizen2, citizen2_aggro + 1, citizen2_aggro);
			citizen2_stay = 0;
		}
		else if (before_citizen2_place != citizen2) {
			printf("citizen 2: %d -> %d(aggro : %d -> %d)\n", citizen2 + 1, citizen2, before_citizen2_aggro, citizen2_aggro);
		}
		else {
			if (citizen2_stay == 1) { //시민이 멈춰있을때
				if (citizen2_aggro != before_citizen2_aggro) {
					printf("citizen 2: STAY -> %d (aggro: %d -> %d)\n", citizen2, before_citizen2_aggro, citizen2_aggro);
					citizen2_stay = 0;
				}
				else if (before_citizen2_aggro == citizen2_aggro) {
					printf("citizen 2: STAY -> %d (aggro: %d)\n", citizen2, citizen2_aggro);
					citizen2_stay = 0;
				}
			}
			else  if (citizen2_stay != 1) { //시민이 멈춰있지 않을 때
				if (citizen2_aggro == before_citizen2_aggro) {
					printf("citizen 2: %d -> %d(aggro : %d)\n", citizen2 + 1, citizen2, citizen2_aggro);
					citizen2_stay = 0;
				}
				else {
					printf("citizen 2: %d -> %d(aggro : %d -> %d)\n", citizen2 + 1, citizen2, before_citizen2_aggro, citizen2_aggro);
					citizen2_stay = 0;
				}
			}
		}
	}
	else {
		printf("citizen2 is free!\n");
	}
}

//시민3 현재 상황 출력 3
void citizen3_now_round3() {
	printf("\n");
	if (citizen3 != -1) {
		if (before_citizen3_place == citizen3 || citizen3_stay == 1) {
			printf("citizen 3: STAY -> %d (aggro: %d -> %d)\n", citizen3, citizen3_aggro + 1, citizen3_aggro);
			citizen3_stay = 0;
		}
		else if (before_citizen3_place != citizen3) {
			printf("citizen 3: %d -> %d(aggro : %d -> %d)\n", citizen3 + 1, citizen3, before_citizen3_aggro, citizen3_aggro);
		}
		else {
			if (citizen3_stay == 1) { //시민이 멈춰있을때
				if (citizen3_aggro != before_citizen3_aggro) {
					printf("citizen 3: STAY -> %d (aggro: %d -> %d)\n", citizen3, before_citizen3_aggro, citizen3_aggro);
					citizen3_stay = 0;
				}
				else if (before_citizen3_aggro == citizen3_aggro) {
					printf("citizen 3: STAY -> %d (aggro: %d)\n", citizen3, citizen3_aggro);
					citizen3_stay = 0;
				}
			}
			else if (citizen3_stay != 1) { //시민이 멈춰있지 않을 때
				if (citizen3_aggro == before_citizen3_aggro) {
					printf("citizen 3: %d -> %d(aggro : %d)\n", citizen3 + 1, citizen3, citizen3_aggro);
					citizen3_stay = 0;
				}
				else {
					printf("citizen 3: %d -> %d(aggro : %d -> %d)\n", citizen3 + 1, citizen3, before_citizen3_aggro, citizen3_aggro);
					citizen3_stay = 0;
				}
			}
		}
	}
	else {
		printf("citizen3 is free!\n");
	}
}

//좀비 현재상황 출력 3
void zombie_now_round3() {
	//좀비가 이동할 수 없는 턴일 때
	if (zombie_cant_move == 1) {
		printf("zombie : STAY -> %d(Can't Move)\n", zombie);
		zombie_cant_move = 0;
	}
	else if (madongseok_holding_zombie == 1) {
		printf("zombie : STAY -> %d(madongseok hold)\n", zombie);
		madongseok_holding_zombie = 0;
	}
	else if (zombie_cant_move_right == 1) {
		printf("zombie : STAY -> %d(zombie can\'t move right)\n", zombie);
		zombie_cant_move_right = 0;
	}
	else {
		// 좀비가 이동할 때
		if (citizen1_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}
		else if (citizen2_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}
		else if (citizen3_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}
		else {
			printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
			zombie_stay = 0;
		}
	}
}

//마동석 현재상황 출력 3
void madongseok_now_round3() {
	if (madongseok_no_move == 1) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) { //어그로가 이전과 다를 때
			printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
			madongseok_no_move = 0;
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok: stay %d (aggro: %d, stamina: %d)\n\n", madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
	else if (madongseok_choose == MOVE_LEFT) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok + 1, madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok move: %d -> %d (aggro: %d, stamina: %d)\n\n", madongseok + 1, madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
}

//시민1 액션 없을 때 3
void result1_round3() {
	if (citizen1 != -1) {
		if (citizen1 != 1) {
			printf("citizen1 does nothing.\n");
		}
	}
}

//시민2 액션 없을 때 3
void result2_round3() {
	if (citizen2 != -1) {
		if (citizen2 != 1) {
			printf("citizen2 does nothing.\n");
		}
	}
}

//시민1 액션 없을 때 3
void result3_round3() {
	if (citizen3 != -1) {
		if (citizen3 != 1) {
			printf("citizen3 does nothing.\n");
		}
	}
}

//마동석 액션 3
void madongseok_action_round3() {
	if (madongseok - 1 == zombie) {
		while (1) {
			printf("madongseok action (%d.rest, %d.provoke, %d.pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
			scanf_s("%d", &madongseok_action_choose);
			if (madongseok_action_choose == ACTION_REST || madongseok_action_choose == ACTION_PROVOKE || madongseok_action_choose == ACTION_PULL) break;
		}
	}
	else {
		while (1) {
			printf("madongseok action (%d.rest, %d.provoke)>> ", ACTION_REST, ACTION_PROVOKE);
			scanf_s("%d", &madongseok_action_choose);
			if (madongseok_action_choose == ACTION_REST || madongseok_action_choose == ACTION_PROVOKE) break;
		}
	}
}

//마동석 휴식 행동 결과 3
void madongseok_result_rest_round3() {
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

//마동석 도발 행동 결과 3
void madongseok_result_provoke_round3() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}

// 마동석 붙들기 행동 결과 3
void madongseok_result_pull_round3() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) >= madongseok_random) { //붙들기 성공
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //어그로 2 증가
			madongseok_stamina -= 1; //체력 1 감소
			madongseok_holding_zombie = 1; //좀비를 잡았다 신호 보내기
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro : %d -> %d, stamina : %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}
		else { //붙들기 실패
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

//마동석 체력이 0일때 판단 3
int madongseok_stamina_zero_round3(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
		exit(1);
	}
	return 0;
}

//좀비 주변에 아무도 없을 때 3
void zombie_action_nobody_round3() {
	if (zombie != madongseok - 1 && zombie != citizen1 + 1 && zombie != citizen2 + 1 && zombie != citizen3 + 1) {
		printf("zombie attacked nobody.\n\n");
	}
}

//좀비 주변에 시민 또는 마동석이 있을 때 3
void zombie_action_attack_citizen_round3() {
	if (zombie_attack_of_madongseok == 1) {

	}
	else {
		if (zombie == citizen1 + 1) {
			citizen1 = -1;
			citizen_die++;
			citizen_count--;
			printf("citizen1 has been attacked by zombie.\n");
			printf("%d citizen(s) alive(s).", citizen_count);
		}
		if (zombie == citizen2 + 1) {
			citizen2 = -1;
			citizen_die++;
			citizen_count--;
			printf("citizen2 has been attacked by zombie.\n");
			printf("%d citizen(s) alive(s).", citizen_count);
		}
		if (zombie == citizen3 + 1) {
			citizen3 = -1;
			citizen_die++;
			citizen_count--;
			printf("citizen3 has been attacked by zombie.\n");
			printf("%d citizen(s) alive(s).", citizen_count);
		}
		else if (zombie == madongseok - 1) {
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zomibe attacked madongseok (stamina: %d -> %d)\n", madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero_round3();
			}
		}
	}
}

//좀비 주변에 둘 다 있을 때 어그로 비교 및 공격 3
void zombie_action_aggro_fight_round3() {
	if (citizen1 + 1 == zombie && zombie + 1 == madongseok) {
		if (citizen1_aggro <= madongseok_aggro) {
			zombie_attack_of_madongseok = 1;
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", citizen1_aggro, madongseok_aggro, madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero_round3();
			}
		}
		else {
			citizen1 = -1;
			citizen_die++;
			citizen_count--;
			printf("citizen1 has been attacked by zombie.\n");
			printf("%d citizen(s) alive(s).", citizen_count);
		}
	}
	else if (citizen2 + 1 == zombie && zombie + 1 == madongseok) {
		if (citizen2_aggro <= madongseok_aggro) {
			zombie_attack_of_madongseok = 1;
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", citizen2_aggro, madongseok_aggro, madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero_round3();
			}
		}
		else {
			citizen2 = -1;
			citizen_count--;
			printf("citizen2 has been attacked by zombie.\n");
			printf("%d citizen(s) alive(s).", citizen_count);
		}
	}
	else if (citizen3 + 1 == zombie && zombie + 1 == madongseok) {
		if (citizen3_aggro <= madongseok_aggro) {
			zombie_attack_of_madongseok = 1;
			madongseok_stamina--;
			if (madongseok_stamina < STM_MIN) madongseok_stamina = STM_MIN;
			printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stamina: %d -> %d)\n", citizen3_aggro, madongseok_aggro, madongseok_stamina + 1, madongseok_stamina);
			if (madongseok_stamina == 0) {
				madongseok_stamina_zero();
			}
		}
		else {
			citizen3 = -1;
			citizen_die++;
			citizen_count--;
			printf("citizen3 has been attacked by zombie.\n");
			printf("%d citizen(s) alive(s).", citizen_count);
		}
	}
}

//시민1 탈출성공 3
int citizen1_win_round3() {
	if (citizen1 == 1) {
		printf("citizen1 succeeded in escaping\n");
		citizen_left++;
		citizen_count--;
		citizen1 = -1;
		printf("%d citizen(s) alive(s)..\n", citizen_count);

	}
}

//시민 2 탈출성공 3
int citizen2_win_round3() {
	if (citizen2 == 1) {
		printf("citizen2 succeeded in escaping\n");
		citizen_count--;
		citizen_left++;
		citizen2 = -1;
		printf("%d citizen(s) alive(s)..\n", citizen_count);

	}
}

//시민 3 탈출성공 3
int citizen3_win_round3() {
	if (citizen3 == 1) {
		printf("citizen3 succeeded in escaping\n");
		citizen_count--;
		citizen_left++;
		citizen3 = -1;
		printf("%d citizen(s) alive(s)..\n", citizen_count);

	}
}

//시민 한 명이라도 탈출 3
void citizens_left_round3() {
	if (citizen_count == 0 && citizen_left >= 1) {
		printf("citizen(s) left\n");
		printf("YOU WIN!\n");
		exit(1);
	}
}                    //exit 빼고 라운드 바꿔야함          

//시민 모두 사망 3
void citizens_die_round3() {
	if (citizen_count == 0 && citizen_left == 0) {
		printf("citizens all die\n");
		printf("YOU LOSE...\n");
		exit(1);
	}
}                         //exit 빼고 라운드 바꿔야함

//짝수턴 초기화 액션 3
void madongseok_action_no_even_number_round3() {
	if (turn % 2 == 0) {
		madongseok_holding_zombie = 0;
	}
}

//줄바꾸기 함수 3
void changing_the_line_round3() {
	printf("\n");
}

// 부산헹 (2)의 함수들을 불러오는 함수 - STAGE 1
void round1() {
	changing_the_line();
	charactor_position(zombie, citizen1, madongseok);
	first_train = train(zombie, citizen1, madongseok); //기본열차 출력
	changing_the_line();
	if (round = 1) {
		while (1) { //반복 시작
			turn++;//턴 증가
			citizen_move(); //시민이동
			zombie_move(); //좀비이동
			train(zombie, citizen1, madongseok); //열차 출력
			citizen_win(); //시민이 이길 경우
			round_chack_round1(); //라운드 체크
			citizen_now(); //시민 현재상황 출력
			zombie_now(); //좀비 현재상황 출력
			madongseok_move_with_zombie();//마동석 주변에 좀비 있을 때 이동
			madongseok_move_no_zombie();//마동석 주변에 좀비 없을 때 이동
			train(zombie, citizen1, madongseok); //두 번째 열차 출력
			changing_the_line(); //줄 변경
			madongseok_now(); //마동석 현재상황 출력
			citizen_win(); //시민이 이길 경우
			result(); //결과 출력
			round_chack_round1(); //라운드 체크
			zombie_action_nobody(); //좀비 행동(주변 아무도 없을때)
			zombie_action_aggro_fight(); //좀비 행동(어그로 더 높은 쪽 공격 (시민 공격시 게임 끝, 마동석 공격시 STM 1감소))
			zombie_action_attack_citizen(); //좀비 행동(시민을 물었을 때)
			round_chack_round1(); //라운드 체크
			madongseok_action(); //마동석 액션
			madongseok_action_no_even_number(); //짝수 턴일 때 실행되면 안되는 것들 초기화
			changing_the_line(); //줄 변경
			madongseok_result_rest(); //마동석 휴식 액션 결과 출력
			madongseok_result_provoke(); //마동석 도발 액션 결과 출력
			madongseok_result_pull(); //마동석 좀비 붙들기 액션 결과 출력
			madongseok_stamina_zero(); //마동석 체력이 0일 때
			round_chack_round1(); //라운드 체크
		}
	}
	else {
		return 0;
	}
}

//부산헹 (3) 빌런 추가한 함수들을 불러오는 함수 - STAGE 2
void round2() {
	changing_the_line();
	charactor_position_round2(zombie, citizen1, madongseok, villain);
	first_train = train_round2(zombie, citizen1, madongseok, villain); //기본열차 출력
	changing_the_line();
	if (round = 2) {
		while (1) { //반복 시작
			turn++;//턴 증가
			citizen_move_round2(); //시민이동
			villain_move_round2(); //빌런이동
			zombie_move_round2(); //좀비이동
			train_round2(zombie, citizen1, madongseok, villain); //열차 출력
			citizen_win_round2(); //시민이 이길 경우
			citizen_now_round2(); //시민 현재상황 출력
			villain_now_round2(); //빌런 현재상황 출력
			zombie_now_round2(); //좀비 현재상황 출력
			madongseok_move_with_zombie_round2();//마동석 주변에 좀비 있을 때 이동
			madongseok_move_no_zombie_round2();//마동석 주변에 좀비 없을 때 이동
			train_round2(zombie, citizen1, madongseok, villain); //두 번째 열차 출력
			changing_the_line(); //줄 변경
			madongseok_now_round2(); //마동석 현재상황 출력
			citizen_win_round2(); //시민이 이길 경우
			citizen_action_round2(); //결과 출력
			villain_action_round2(); //빌런 행동 
			zombie_action_nobody_round2(); //좀비 행동(주변 아무도 없을때)
			zombie_action_aggro_fight_round2(); //좀비 행동(어그로 더 높은 쪽 공격 (시민 공격시 게임 끝, 마동석 공격시 STM 1감소))
			zombie_action_attack_citizen_round2(); //좀비 행동(시민을 물었을 때)
			madongseok_action_round2(); //마동석 액션
			madongseok_action_no_even_number(); //짝수 턴일 때 실행되면 안되는 것들 초기화
			changing_the_line(); //줄 변경
			madongseok_result_rest_round2(); //마동석 휴식 액션 결과 출력
			madongseok_result_provoke_round2(); //마동석 도발 액션 결과 출력
			madongseok_result_pull_round2(); //마동석 좀비 붙들기 액션 결과 출력
			madongseok_stamina_zero_round2(); //마동석 체력이 0일 때
		}
	}
}

//부산헹 (3) 시민'들'이 나오는 함수들을 불러오는 함수 - STAGE 3
void round3() {
	changing_the_line_round3();
	printf("\n\n");
	printf("* ROUND 3\n\n");
	citizens_line_round3();
	charactor_position(zombie, citizen1, citizen2, citizen3, madongseok);
	first_train = train_round3(zombie, citizen1, citizen2, citizen3, madongseok); //기본열차 출력
	changing_the_line();
	while (1) { //반복 시작
		turn++;//턴 증가
		if (citizen3 != -1) {
			citizen3_move_round3(); //시민이동
		}
		if (citizen2 != -1) {
			citizen2_move_round3(); //시민이동
		}
		if (citizen1 != -1) {
			citizen1_move_round3(); //시민이동
		}
		zombie_move_round3(); //좀비이동

		train_round3(zombie, citizen1, citizen2, citizen3, madongseok); //열차 출력

		citizen3_now_round3(); //시민3 현재상황 출력
		citizen2_now_round3(); //시민2 현재상황 출력
		citizen1_now_round3(); //시민1 현재상황 출력
		zombie_now_round3(); //좀비 현재상황 출력

		madongseok_move_with_zombie_round3();//마동석 주변에 좀비 있을 때 이동
		madongseok_move_no_zombie_round3();//마동석 주변에 좀비 없을 때 이동

		train_round3(zombie, citizen1, citizen2, citizen3, madongseok); //두 번째 열차 출력

		changing_the_line_round3(); //줄 변경

		madongseok_now_round3(); //마동석 현재상황 출력

		citizen3_win_round3(); //시민1 탈출했을경우
		result3_round3(); //시민 1결과 출력
		citizen2_win_round3(); //시민2 탈출했을경우
		result2_round3(); //시민 2결과 출력
		citizen1_win_round3(); //시민3 탈출했을경우
		result1_round3();  //시민 3결과 출력

		citizens_left_round3(); //시민이 탈출에 성공하였는가?
		citizens_die_round3(); //시민이 탈출에 실패하였는가?


		zombie_action_nobody_round3(); //좀비 행동(주변 아무도 없을때)
		zombie_action_aggro_fight_round3(); //좀비 행동(어그로 더 높은 쪽 공격 (시민 공격시 게임 끝, 마동석 공격시 STM 1감소))
		zombie_action_attack_citizen_round3(); //좀비 행동(시민을 물었을 때)

		citizens_left_round3(); //시민이 탈출에 성공하였는가?
		citizens_die_round3(); //시민이 탈출에 실패하였는가?

		madongseok_action_round3(); //마동석 액션
		madongseok_action_no_even_number_round3(); //짝수 턴일 때 실행되면 안되는 것들 초기화
		changing_the_line_round3(); //줄 변경
		madongseok_result_rest_round3(); //마동석 휴식 액션 결과 출력
		madongseok_result_provoke_round3(); //마동석 도발 액션 결과 출력
		madongseok_result_pull_round3(); //마동석 좀비 붙들기 액션 결과 출력

		madongseok_stamina_zero_round3(); //마동석 체력이 0일 때
	}
}

//총 메인 함수
int main(void) {
	srand((unsigned int)time(NULL)); //난수 설정
	intro(); //인트로 출력
	train_length = variables("train length", LEN_MIN, LEN_MAX); //열차 길이 입력
	madongseok_stamina = variables("madongseok stamina", STM_MIN, STM_MAX); //마동석 스테미나 입력
	madongseok_stamina_get = madongseok_stamina;
	percentile_probability = variables("percentile probability \'p\'", PROB_MIN, PROB_MAX); //확률 입력

	printf("\n* ROUND 1\n");
	round1();
}