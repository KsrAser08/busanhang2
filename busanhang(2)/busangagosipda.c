#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//���� ����
#define LEN_MIN 15
#define LEN_MAX 50
//������ ü��
#define STM_MIN 0
#define STM_MAX 5
//Ȯ��
#define PROB_MIN 10
#define PROB_MAX 90
// ��׷� ����
#define AGGRO_MIN 0
#define AGGRO_MAX 5
// �������̵�����
#define MOVE_LEFT 1
#define MOVE_STAY 0
// �����ǰ��ݴ��
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
// �������ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

//���� ����
int train_length; //��������
int percentile_probability; //�̵� Ȯ��
int madongseok_stamina; //������ ü��
int turn = 0;

int input_value() {

	
}

int main () {
	//���� �Է¹ޱ�
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
