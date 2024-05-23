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
int madongseok, zombie, citizen; //������ ���� �ù�
int first_train;
//�Լ� ����
int intro();
int variables(char massage, int MIN, int MAX);
int train(int ZOMB, int CITIZ, int MADONGS);

int intro() {
	printf(" ------------- busanhang(2) ------------- \n");
	printf("|                                        |\n");
	printf("|         We must run the train to       |\n");
	printf("|              avoid zombies             |\n");
	printf("|                                        |\n");
	printf(" ---------------------------------------- \n");
}
int variables(char*massage,int MIN, int MAX) {
	int value;
	while (1) {
		printf("%s(%d ~ %d)>> ", massage, MIN, MAX);
		scanf_s("%d", &value);
		if (value >= MIN && value <= MAX) {
			break;
		}
	}
	madongseok = train_length - 2;
	zombie = train_length - 3;
	citizen = train_length - 6;
	return value;
}
int train(int ZOMB,int CITIZ,int MADONGS) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < train_length; j++) {
			if (i == 0 || i == 2) printf("#");
			else {
				if ((j == 0) || (j == train_length - 1)) printf("#");
				else if (MADONGS == j) printf("M");
				else if (CITIZ == j) printf("Z");
				else if (ZOMB == j) printf("C");
				else printf(" ");
			}
		}
		printf("\n");
	}
}
int main () {
	intro();
	train_length = variables("train length", LEN_MIN, LEN_MAX);
	madongseok_stamina = variables("madongseok stamina", STM_MIN, STM_MAX);
	percentile_probability = variables("percentile probability \'p\'", PROB_MIN, PROB_MAX);
	first_train = train(zombie, citizen, madongseok);
}