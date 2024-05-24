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
int first_train; //�ʱ� ����
int citizen_aggro = 1, madongseok_aggro = 1; //�ù� ������ ��׷� (�⺻���� 1)
int citizen_random, madongseok_random; //�ù� ����Ȯ��, ������ ����Ȯ��
int citizen_stay, zombie_stay, madongseok_stay; //�ù� ���� ������ ����
int zombie_cant_move; //���� ������ �� ���� ���� ��
int before_citizen_aggro, before_madongseok_aggro; //�ٲ�� ���� ��׷�
int madongseok_choose; //������ �ൿ or �޽� scanf_s ����
int madongseok_no_move; //�������� �ʴ´� ������ ��
int madongseok_action_choose; //������ �׼� ����
int before_madongseok_stamina; //�ٲ�� ���� ������ ü��
int madongseok_holding_zombie; //�������� ���� ����

//��Ʈ�� �Լ�
void intro() { 
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
void zombie_move() {
	if (turn % 2 != 0) {
		zombie--;
	}
	else {
		zombie_cant_move++;
	}
}

//�ù� �����̴� �Լ�
void citizen_move() {
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
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
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
void zombie_now() {
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
void madongseok_now(){
	if (madongseok_no_move == 1) {
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
			madongseok_no_move = 0;
		}
		else if(madongseok_aggro == before_madongseok_aggro) {
			printf("madongseok: stay %d (aggro: %d, stamina: %d)\n\n", madongseok, madongseok_aggro, madongseok_stamina);
		}
	}
	else if (madongseok_choose == MOVE_LEFT) {
		if (madongseok_aggro != before_madongseok_aggro) {
			printf("madongseok: move %d -> %d (aggro: %d -> %d, stamina: %d)\n\n", madongseok + 1, madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
		}
		else if (madongseok_aggro == before_madongseok_aggro) {
			printf("madongseokL move %d -> %d (aggro: %d, stamina: %d)\n\n", madongseok + 1, madongseok, madongseok_aggro, madongseok_stamina);
		}
		
	}
}
//�ùΰ� ���� �׼� ������
void result() {
	if (citizen != 1) {
		printf("citizen does nothing.\n");
	}
	if (zombie != madongseok - 1) {
		printf("zombie attacked nobody.\n\n");
	}
}
//������ �׼�
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
//������ �޽� �ൿ ���
void madongseok_result_rest() {
	if (madongseok_action_choose == ACTION_REST) {
		printf("madongseok rests...\n");
		before_madongseok_aggro = madongseok_aggro;
		before_madongseok_stamina = madongseok_stamina;
		madongseok_aggro--;
		madongseok_stamina++;
		if (madongseok_aggro != before_madongseok_aggro) {
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			else if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
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
//������ ���� �ൿ ���
void madongseok_result_provoke() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}
// ������ �ٵ�� �ൿ ���
void madongseok_result_pull() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) > madongseok_random) { //�ٵ�� ����
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //��׷� 2 ����
			madongseok_stamina -= 1; //ü�� 1 ����
			madongseok_holding_zombie; //���� ��Ҵ� ��ȣ ������
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro : %d -> %d, stamina : %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}
		else { //�ٵ�� ����
			printf("madongseok failed to pull zombie\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //��׷� 2 ����
			madongseok_stamina -= 1; //ü�� 1 ����
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			else if (madongseok_aggro < STM_MIN) madongseok_aggro = STM_MIN;
			printf("madongseok: %d (aggro : %d -> %d, stamina : %d -> %d)\n\n", madongseok, before_madongseok_aggro, madongseok_aggro, before_madongseok_stamina, madongseok_stamina);
		}
	}
}
//������ ü���� 0�϶� �Ǵ�
int madongseok_stamina_zero(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
		exit(1);
	}
}
//�ٹٲٱ� �Լ�
void changing_the_line() {
	printf("\n");
}
//���� �Լ�
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
		turn++;//�� ����

		citizen_move(); //�ù��̵�
		zombie_move(); //�����̵�

		train(zombie, citizen, madongseok); //���� ���

		citizen_now(); //�ù� �����Ȳ ���
		zombie_now(); //���� �����Ȳ ���

		madongseok_move();//������ �̵�

		train(zombie, citizen, madongseok); //�� ��° ���� ���

		changing_the_line(); //�� ����

		madongseok_now(); //������ �����Ȳ ���

		result(); //��� ���
		
		madongseok_action(); //������ �׼�
		changing_the_line(); //�� ����
		madongseok_result_rest(); //������ �޽� �׼� ��� ���
		madongseok_result_provoke(); //������ ���� �׼� ��� ���
		madongseok_result_pull(); //������ ���� �ٵ�� �׼� ��� ���




		madongseok_stamina_zero(); //������ ü���� 0�� ��
	}
	return 0;
}