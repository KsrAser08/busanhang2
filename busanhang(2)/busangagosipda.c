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
int turn = 0;

int input_value() {

	
}

int main () {
	//변수 입력받기
	printf("train length(%d ~ %d)", LEN_MIN, LEN_MAX);
	scanf_s("%d", &train_length);
	if (train_length < LEN_MIN || train_length > LEN_MAX) {
		

	}
	printf("percentile probability  (%d ~ %d) >> ", PROB_MIN, PROB_MAX);
	scanf_s("%d", &percentile_probability);
	if (percentile_probability  < PROB_MIN || percentile_probability  > PROB_MAX) {
		
		

	}
	return 0;

}
