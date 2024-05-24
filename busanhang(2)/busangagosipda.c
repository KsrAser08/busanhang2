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
int turn = 1;
int madongseok, zombie, citizen; //������ ���� �ù�
int first_train; //�ʱ� ����
int citizen_aggro = 1, madongseok_aggro = 1; //�ù� ������ ��׷� (�⺻���� 1)
int citizen_random, zombie_random; //�ù� ����Ȯ��, ���� ����Ȯ��
int citizen_stay, zombie_stay, madongseok_stay; //�ù� ���� ������ ����
int zombie_cant_move; //���� ������ �� ���� ���� ��
int before_citizen_aggro, before_madongseok_aggro; //�ٲ�� ���� ��׷�
int madongseok_choose; //������ �ൿ or �޽� scanf_s ����
int madongseok_no_move; //�������� �ʴ´� ������ ��

//�Լ� ����
int intro(); //��Ʈ��
int variables(char massage, int MIN, int MAX); //Ȯ�� �Է¹޴� �Լ�
int train(int ZOMB, int CITIZ, int MADONGS); //���� ���

//��Ʈ�� �Լ�
int intro() { 
	printf(" ------------- busanhang(2) ------------- \n");
	printf("|                                        |\n");
	printf("|         We must run the train to       |\n");
	printf("|              avoid zombies             |\n");
	printf("|                                        |\n");
	printf(" ---------------------------------------- \n");
}

//���� �Է� �Լ�
int variables(char*massage,int MIN, int MAX) {
	int value;
	while (1) {
		printf("%s(%d ~ %d)>> ", massage, MIN, MAX);
		scanf_s("%d", &value);
		if (value >= MIN && value <= MAX) {
			break;
		}
	}
	//ĳ���� ��ġ ����
	madongseok = train_length - 2;
	zombie = train_length - 3;
	citizen = train_length - 6;
	return value;
}

//���� �Լ�
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

//���� �����̴� �Լ�
int zombie_move() {
	if (turn % 2 != 0) {
		zombie--;
	}
	else {
		zombie_cant_move++;
	}
}

//�ù� �����̴� �Լ�
int citizen_move() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //�̵��� ����
		citizen--;
		before_citizen_aggro = citizen_aggro;
		citizen_aggro++;
		if (citizen_aggro > AGGRO_MAX) {
			citizen_aggro = AGGRO_MAX;
		}
	}
	else { //�̵� ����
		citizen_stay++;
		before_citizen_aggro = citizen_aggro;
		citizen_aggro--;
		if (citizen_aggro < AGGRO_MIN) {
			citizen_aggro = AGGRO_MIN;
		}
	}
}

//������ �����̴� �Լ�
int madongseok_move() {
	while (1) {
		printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT);
		scanf_s("%d", &madongseok_choose);
		if (madongseok_choose == MOVE_LEFT) { //�̵��� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++;
			if (madongseok_aggro > AGGRO_MAX) {
				madongseok_aggro = AGGRO_MAX;
			}
			madongseok--;
		}
		else if (madongseok_choose == MOVE_STAY) { //�̵����� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--;
			madongseok_no_move++;
			if (madongseok_aggro < AGGRO_MIN) {
				madongseok_aggro = AGGRO_MIN;
			}
		} if (madongseok_choose == MOVE_LEFT || madongseok_choose == MOVE_STAY) break;
	}
}

//�ù� ���� ��Ȳ ���
int citizen_now() {
	printf("\n");
	if (citizen_stay == 1) { //�ù��� ����������
		if (citizen_aggro != before_citizen_aggro) {
			printf("citizen: STAY -> %d (aggro: %d -> %d)\n", citizen, before_citizen_aggro, citizen_aggro);
			citizen_stay = 0;
		} 
		else if(before_citizen_aggro == citizen_aggro) {
			printf("citizen: STAY -> %d (aggro: %d)\n", citizen, citizen_aggro);
			citizen_stay = 0;
		}
	} else { //�ù��� �������� ���� ��
		if (citizen_aggro == before_citizen_aggro) {
			printf("citizen: %d -> %d(aggro : %d)\n", citizen + 1, citizen, citizen_aggro);
		} else {
			printf("citizen: %d -> %d(aggro : %d -> %d)\n", citizen + 1, citizen, before_citizen_aggro, citizen_aggro);
			citizen_stay = 0;
		}
	}
}
//���� �����Ȳ ���
int zombie_now() {
	//���� �̵��� �� ���� ���� ��x`
	if (zombie_cant_move == 1) {
		printf("zombie : STAY -> %d(Can't Move)\n", zombie);
		zombie_cant_move = 0;
	} else {
		// ���� �̵��� ��
		printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
		zombie_stay = 0;
	}
}
//������ �����Ȳ ���
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
//�ٹٲٱ� �Լ�
int changing_the_line() {
	printf("\n");
}
int main (void) {
	srand((unsigned int)time(NULL)); //���� ����
	intro(); //��Ʈ�� ���
	train_length = variables("train length", LEN_MIN, LEN_MAX); //���� ���� �Է�
	madongseok_stamina = variables("madongseok stamina", STM_MIN, STM_MAX); //������ ���׹̳� �Է�
	percentile_probability = variables("percentile probability \'p\'", PROB_MIN, PROB_MAX); //Ȯ�� �Է�
	changing_the_line();
	first_train = train(zombie, citizen, madongseok); //�⺻���� ���
	changing_the_line();
	while (turn < 20) { //�ݺ� ����
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