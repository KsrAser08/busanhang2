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
// ������ �̵�����
#define MOVE_LEFT 1
#define MOVE_STAY 0
// ������ ���ݴ��
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int train_length; //��������
int percentile_probability; //�̵� Ȯ��
int madongseok_stamina; //������ ü��
int madongseok_stamina_get; //������ ���׹̳� �⺻����
int turn = 0; //��� ° ������
int madongseok, zombie, citizen1, villain; //������ ���� �ù� ����
int citizen2, citizen3; //�ù�2 �ù�3
int change;//�ù� ���� ��ġ �ٲٱ�
int first_train; //�ʱ� ����
int citizen1_aggro = 1, madongseok_aggro = 1, villain_aggro = 1; //�ù� ������ ��׷� (�⺻���� 1)
int citizen_random, madongseok_random, villain_random; //�ù� ����Ȯ��, ������ ����Ȯ��, ���� ����Ȯ��
int citizen1_stay, zombie_stay, madongseok_stay, villain_stay; //�ù� ���� ������ ���� ����
int zombie_cant_move; //���� ������ �� ���� ���� ��
int before_citizen1_aggro, before_madongseok_aggro, before_villain_aggro; //�ٲ�� ���� ��׷�
int madongseok_choose; //������ �ൿ or �޽� scanf_s ����
int madongseok_no_move; //�������� �ʴ´� ������ ��
int madongseok_action_choose; //������ �׼� ����
int before_madongseok_stamina; //�ٲ�� ���� ������ ü��
int madongseok_holding_zombie = 0; //�������� ���� ����
int zombie_cant_move_right; //���� �������� �������� ���������� �̵��� �Ұ����� ��
int zombie_attack_of_madongseok; //���� �ùΰ� ������ �Ѵ� ���������� �� �������� ����
int villain_now_dead = 0; //���� �׾��� ��
int citizen_move_villain_move; //�ù��� ������ �� ������ ������
int round = 1;// ��������� �Ǵ�
int before_citizen1_place, before_citizen2_place, before_citizen3_place; //�ùε��� ��������? üũ
int citizen_die, citizen_left; //Ż�� Ȥ�� ��� ����
int citizen_count = 3; //��� �� ī��Ʈ
int citizen2_aggro = 1, before_citizen2_aggro, citizen2_stay; //�ù� 2 ���� ����
int citizen3_aggro = 1, before_citizen3_aggro, citizen3_stay; //�ù� 3 ���� ����

//�Լ� ������Ÿ��
void round1();
void round2();
void round3();

//�������� ���Ǵ� �Լ���

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

//�ù��� �̱� 
int citizen_win() {
	if (citizen1 == 1) {
		printf("\ncitizen succeeded in escaping\n");
		printf("YOU WIN!");
		round++;
	}
}

//�ù� ���� 
int citizen_dead_ending() {
	if (citizen1 + 1 == zombie) {
		printf("zombie attacked the citizen.\n");
		printf("\nGAME OEVER! citizen dead...\n");
		round++;
	}
}

//¦���� �ʱ�ȭ �׼�
void madongseok_action_no_even_number() {
	if (turn % 2 == 0) {
		madongseok_holding_zombie = 0;
	}
}

//�ٹٲٱ� �Լ�
void changing_the_line() {
	printf("\n");
}

//���� �⺻�� �ʱ�ȭ �Լ�
void game_reset() {
	before_citizen1_aggro = 1;
	citizen1_aggro = 1;
	before_madongseok_aggro = 1;
	madongseok_aggro = 1;
	madongseok_stamina = madongseok_stamina_get;
	turn = 0;
}



//�λ���(2) �Լ���

//ĳ���� ��ġ ���� 1
int charactor_position() {
	zombie = train_length - 3;
	madongseok = train_length - 2;
	citizen1 = train_length - 6;
}

//���� �Լ� 1
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

//���� �����̴� �Լ� 1
void zombie_move() {
	if (turn % 2 != 0) { //���� ¦���� �ƴ� ��
		if (1 == madongseok_holding_zombie) {//���� ���������� �ٵ�� ���� ��
			madongseok_holding_zombie = 1;
		}
		else {
			if (citizen1_aggro >= madongseok_aggro) { //�ù� ��׷ΰ� ������ ��׷κ��� ũ�ų� ���� ��
				zombie--;
			}
			else {
				if (zombie + 1 == madongseok) {
					zombie_cant_move_right++; //���� ���������� �̵��� ��, �������� �ڿ� ���� ��쿡 ����
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

//�ù� �����̴� �Լ� 1
void citizen_move() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //�̵��� ����
		citizen1--;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro++;
		if (citizen1_aggro > AGGRO_MAX) {
			citizen1_aggro = AGGRO_MAX;
		}
	}
	else { //�̵� ����
		citizen1_stay++;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro--;
		if (citizen1_aggro < AGGRO_MIN) {
			citizen1_aggro = AGGRO_MIN;
		}
	}
}

//���� �ٷ� �տ� ���� �� ������ �̵� 1
void madongseok_move_no_zombie() {
	if (zombie != madongseok - 1) {
		while (1) {
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //�տ� ���� ���� ��
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY || madongseok_choose == MOVE_LEFT) break;
		}
		if (madongseok_choose == MOVE_LEFT) { //�̵��� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++; //��׷� 1����
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			madongseok--; //�� ĭ �������� �̵�
		}
		else if (madongseok_choose == MOVE_STAY) { //�̵����� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //��׷� 1����
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_no_move++; // ������ �ȿ����� ��ȣ
		}
	}
}

//���� �ٷ� �տ� ������ ������ �̵� 1
void madongseok_move_with_zombie() {
	if (zombie == madongseok - 1) { //���� �ٷ� �տ� ���� ���
		while (1) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY) break;
		}
		if (madongseok_choose == MOVE_STAY) { //�̵����� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //��׷� 1����
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_stamina++; //ü�� 1����   
			if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			madongseok_no_move++;
		}

	}
}

//�ù� ���� ��Ȳ ��� 1
void citizen_now() {
	printf("\n");
	if (citizen1_stay == 1) { //�ù��� ����������
		if (citizen1_aggro != before_citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
		else if (before_citizen1_aggro == citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d)\n", citizen1, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
	else { //�ù��� �������� ���� ��
		if (citizen1_aggro == before_citizen1_aggro) {
			printf("citizen: %d -> %d(aggro : %d)\n", citizen1 + 1, citizen1, citizen1_aggro);
		}
		else {
			printf("citizen: %d -> %d(aggro : %d -> %d)\n", citizen1 + 1, citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
}

//���� �����Ȳ ��� 1
void zombie_now() {
	//���� �̵��� �� ���� ���� ��
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
		// ���� �̵��� ��
		if (citizen1_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}
		else {
			printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
			zombie_stay = 0;
		}
	}
}

//������ �����Ȳ ��� 1
void madongseok_now() {
	if (madongseok_no_move == 1) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) { //��׷ΰ� ������ �ٸ� ��
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

//�ù� �׼� ���� �� 1
void result() {
	if (citizen1 != 1) {
		printf("citizen does nothing.\n");
	}
}

//������ �׼� 1
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

//������ �޽� �ൿ ��� 1
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

//������ ���� �ൿ ��� 1
void madongseok_result_provoke() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}

// ������ �ٵ�� �ൿ ��� 1
void madongseok_result_pull() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) >= madongseok_random) { //�ٵ�� ����
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //��׷� 2 ����
			madongseok_stamina -= 1; //ü�� 1 ����
			madongseok_holding_zombie = 1; //���� ��Ҵ� ��ȣ ������
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

//������ ü���� 0�϶� �Ǵ� 1
int madongseok_stamina_zero(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
		round++;
	}
}

//���� �ֺ��� �ƹ��� ���� �� 1
void zombie_action_nobody() {
	if (zombie != madongseok - 1 && zombie != citizen1 + 1) {
		printf("zombie attacked nobody.\n\n");
	}
}

//���� �ֺ��� �ù� �Ǵ� �������� ���� �� 1
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

//���� �ֺ��� �� �� ���� �� ��׷� �� �� ���� 1
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

//�� �������� Ȯ���ϴ� �Լ� 1
void round_chack_round1() {
	if (round == 2) {
		game_reset();
		printf("\n\n\n\n");
		printf("* ROUND 2\n");
		round2();
	}
}



// �λ���(3) ���� �߰� �Լ���

//ĳ���� ��ġ ���� 2
int charactor_position_round2() {
	zombie = train_length - 3;
	madongseok = train_length - 2;
	citizen1 = train_length - 6;
	villain = train_length - 5;
}

//���� �Լ� 2
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

//���� �����̴� �Լ� 2
void zombie_move_round2() {
	if (turn % 2 != 0) { //���� ¦���� �ƴ� ��
		if (1 == madongseok_holding_zombie) {//���� ���������� �ٵ�� ���� ��
			madongseok_holding_zombie = 1;
		}
		else {
			if (citizen1_aggro >= madongseok_aggro) { //�ù� ��׷ΰ� ������ ��׷κ��� ũ�ų� ���� ��
				zombie--;
			}
			else {
				if (zombie + 1 == madongseok) {
					zombie_cant_move_right++; //���� ���������� �̵��� ��, �������� �ڿ� ���� ��쿡 ����
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

//�ù� �����̴� �Լ� 2
void citizen_move_round2() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //�̵��� ����
		citizen_move_villain_move = 1;
		citizen1--;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro++;
		if (citizen1_aggro > AGGRO_MAX) citizen1_aggro = AGGRO_MAX;
	}
	else { //�̵� ����
		citizen_move_villain_move = 2;
		citizen1_stay = 1;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro--;
		if (citizen1_aggro < AGGRO_MIN) citizen1_aggro = AGGRO_MIN;
	}
}

//���� �����̴� �Լ� 2
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

//���� �ٷ� �տ� ���� �� ������ �̵� 2
void madongseok_move_no_zombie_round2() {
	if (zombie != madongseok - 1) {
		while (1) {
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //�տ� ���� ���� ��
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY || madongseok_choose == MOVE_LEFT) break;
		}
		if (madongseok_choose == MOVE_LEFT) { //�̵��� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++; //��׷� 1����
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			madongseok--; //�� ĭ �������� �̵�
		}
		else if (madongseok_choose == MOVE_STAY) { //�̵����� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //��׷� 1����
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_no_move++; // ������ �ȿ����� ��ȣ
		}
	}
}

//���� �ٷ� �տ� ������ ������ �̵� 2
void madongseok_move_with_zombie_round2() {
	if (zombie == madongseok - 1) { //���� �ٷ� �տ� ���� ���
		while (1) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY) break;
		}
		if (madongseok_choose == MOVE_STAY) { //�̵����� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //��׷� 1����
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_stamina++; //ü�� 1����   
			if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			madongseok_no_move++;
		}

	}
}

//�ù� ���� ��Ȳ ��� 2
void citizen_now_round2() {
	printf("\n");
	if (citizen1_stay == 1) { //�ù��� ����������
		if (citizen1_aggro != before_citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
		else if (before_citizen1_aggro == citizen1_aggro) {
			printf("citizen: STAY -> %d (aggro: %d)\n", citizen1, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
	else { //�ù��� �������� ���� ��
		if (citizen1_aggro == before_citizen1_aggro) {
			printf("citizen: %d -> %d(aggro : %d)\n", citizen1 + 1, citizen1, citizen1_aggro);
		}
		else {
			printf("citizen: %d -> %d(aggro : %d -> %d)\n", citizen1 + 1, citizen1, before_citizen1_aggro, citizen1_aggro);
			citizen1_stay = 0;
		}
	}
}

//���� �����Ȳ ��� 2
void villain_now_round2() {
	if (villain_now_dead != 1) {
		if (villain != -1) {

			if (villain_stay == 1) { //�ù��� ������, ������ ���ߴ� ������ ����������
				if (villain_aggro != before_villain_aggro) {
					printf("villain: stay %d (aggro: %d -> %d)\n", villain, before_villain_aggro, villain_aggro);
					villain_stay = 0;
				}
				else if (before_villain_aggro == villain_aggro) {
					printf("villain: stay %d (aggro: %d)\n", villain, villain_aggro);
					villain_stay = 0;
				}
			}
			else { //������ �������� ���� ��
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

//���� �����Ȳ ��� 2
void zombie_now_round2() {
	//���� �̵��� �� ���� ���� ��
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
		// ���� �̵��� ��
		if (citizen1_aggro < madongseok_aggro || villain_aggro < madongseok_aggro) {
			printf("zombie : %d -> %d\n\n", zombie, zombie + 1);
		}
		else {
			printf("zombie : %d -> %d\n\n", zombie + 1, zombie);
			zombie_stay = 0;
		}
	}
}

//������ �����Ȳ ��� 2
void madongseok_now_round2() {
	if (madongseok_no_move == 1) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) { //��׷ΰ� ������ �ٸ� ��
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

//�ù� �׼� ���� �� 2
void citizen_action_round2() {
	if (citizen1 != 1) {
		printf("citizen does nothing.\n");
	}
}

//������ �׼� 2
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

//������ �޽� �ൿ ��� 2
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

//������ ���� �ൿ ��� 2
void madongseok_result_provoke_round2() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}

// ������ �ٵ�� �ൿ ��� 2
void madongseok_result_pull_round2() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) >= madongseok_random) { //�ٵ�� ����
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //��׷� 2 ����
			madongseok_stamina -= 1; //ü�� 1 ����
			madongseok_holding_zombie = 1; //���� ��Ҵ� ��ȣ ������
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

//������ ü���� 0�϶� �Ǵ� 2
int madongseok_stamina_zero_round2(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
	}
	round = 3;
}

//���� �ֺ��� �ƹ��� ���� �� 2
void zombie_action_nobody_round2() {
	if (zombie != madongseok - 1 && zombie != citizen1 + 1 && zombie != villain + 1) {
		printf("zombie attacked nobody.\n\n");
	}
}

//���� �ֺ��� �ù�, ���� �Ǵ� �������� ���� �� 2
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

//���� �ֺ��� �� �� ���� �� ��׷� �� �� ���� 2
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

//���� �׼� 2
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

//�ù��� �̱� 2
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



//�λ���(3) �ù�'��' �Լ�

// �ù�2, �ù�3 ��ġ���� 3
void citizens_line_round3() {
	while (1) {
		citizen2 = rand() % train_length / 4;
		citizen3 = rand() % train_length / 2;
		if (citizen2 != 0 && citizen3 != 0 && citizen2 != citizen3) {
			break;
		}
	}

}

//ĳ���� ��ġ ���� 3
int charactor_position_round3() {
	citizen1 = train_length - 6;
	zombie = train_length - 3;
	madongseok = train_length - 2;
}

//���� �Լ� 3
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

//���� �����̴� �Լ� 3
void zombie_move_round3() {
	if (turn % 2 != 0) { //���� ¦���� �ƴ� ��
		if (1 == madongseok_holding_zombie) {//���� ���������� �ٵ�� ���� ��
			madongseok_holding_zombie = 1;
		}
		else {
			if (citizen1_aggro >= madongseok_aggro) { //�ù� ��׷ΰ� ������ ��׷κ��� ũ�ų� ���� ��
				zombie--;
			}
			else {
				if (zombie + 1 == madongseok) {
					zombie_cant_move_right++; //���� ���������� �̵��� ��, �������� �ڿ� ���� ��쿡 ����
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

//�ù�1 �����̴� �Լ� 3
void citizen1_move_round3() {
	citizen_random = rand() % 101;
	if ((100 - percentile_probability) >= citizen_random) { //�̵��� ����
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
	else { //�̵� ����
		before_citizen1_place = citizen1;
		citizen1_stay++;
		before_citizen1_aggro = citizen1_aggro;
		citizen1_aggro--;
		if (citizen1_aggro < AGGRO_MIN) {
			citizen1_aggro = AGGRO_MIN;
		}
	}
}

//�ù�2 �����̴� �Լ� 3
void citizen2_move_round3() {
	citizen_random = rand() % 101;
	if (citizen2 != 1) {
		if (citizen1 + 1 == citizen2 || citizen3 + 1 == citizen2) {
			citizen2_stay = 1;
		}
		else {
			if ((100 - percentile_probability) >= citizen_random) { //�̵��� ����
				before_citizen2_place = citizen2;
				citizen2--;
				before_citizen2_aggro = citizen2_aggro;
				citizen2_aggro++;
				if (citizen2_aggro > AGGRO_MAX) {
					citizen2_aggro = AGGRO_MAX;
				}
			}
			else { //�̵� ����
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

//�ù�3 �����̴� �Լ� 3
void citizen3_move_round3() {
	citizen_random = rand() % 101;
	if (citizen3 != 1) {
		if (citizen1 + 1 == citizen3 || citizen2 + 1 == citizen3) {
			citizen3_stay = 1;
		}
		else {
			if ((100 - percentile_probability) >= citizen_random) { //�̵��� ����
				before_citizen3_place = citizen3;
				citizen3--;
				before_citizen3_aggro = citizen3_aggro;
				citizen3_aggro++;
				if (citizen3_aggro > AGGRO_MAX) {
					citizen3_aggro = AGGRO_MAX;
				}
			}
			else { //�̵� ����
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

//���� �ٷ� �տ� ���� �� ������ �̵� 3
void madongseok_move_no_zombie_round3() {
	if (zombie != madongseok - 1) {
		while (1) {
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //�տ� ���� ���� ��
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY || madongseok_choose == MOVE_LEFT) break;
		}
		if (madongseok_choose == MOVE_LEFT) { //�̵��� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro++; //��׷� 1����
			if (madongseok_aggro > AGGRO_MAX) madongseok_aggro = AGGRO_MAX;
			madongseok--; //�� ĭ �������� �̵�
		}
		else if (madongseok_choose == MOVE_STAY) { //�̵����� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //��׷� 1����
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_no_move++; // ������ �ȿ����� ��ȣ
		}
	}
}

//���� �ٷ� �տ� ������ ������ �̵� 3
void madongseok_move_with_zombie_round3() {
	if (zombie == madongseok - 1) { //���� �ٷ� �տ� ���� ���
		while (1) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &madongseok_choose);
			if (madongseok_choose == MOVE_STAY) break;
		}
		if (madongseok_choose == MOVE_STAY) { //�̵����� ������ ��
			before_madongseok_aggro = madongseok_aggro;
			madongseok_aggro--; //��׷� 1����
			if (madongseok_aggro < AGGRO_MIN) madongseok_aggro = AGGRO_MIN;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_stamina++; //ü�� 1����   
			if (madongseok_stamina > STM_MAX) madongseok_stamina = STM_MAX;
			madongseok_no_move++;
		}

	}
}

//�ù�1 ���� ��Ȳ ��� 3
void citizen1_now_round3() {
	if (before_citizen1_place == citizen1) {
		printf("citizen 1: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
	}
	else if (before_citizen1_place != citizen1) {
		printf("citizen 1: %d -> %d(aggro : %d -> %d)\n", citizen1 + 1, citizen1, before_citizen1_aggro, citizen1_aggro);
	}
	else {
		if (citizen1_stay == 1) { //�ù��� ����������
			if (citizen1_aggro != before_citizen1_aggro) {
				printf("citizen 1: STAY -> %d (aggro: %d -> %d)\n", citizen1, before_citizen1_aggro, citizen1_aggro);
				citizen1_stay = 0;
			}
			else if (before_citizen1_aggro == citizen1_aggro) {
				printf("citizen 1: STAY -> %d (aggro: %d)\n", citizen1, citizen1_aggro);
				citizen1_stay = 0;
			}
		}
		else { //�ù��� �������� ���� ��
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

//�ù�2 ���� ��Ȳ ��� 3
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
			if (citizen2_stay == 1) { //�ù��� ����������
				if (citizen2_aggro != before_citizen2_aggro) {
					printf("citizen 2: STAY -> %d (aggro: %d -> %d)\n", citizen2, before_citizen2_aggro, citizen2_aggro);
					citizen2_stay = 0;
				}
				else if (before_citizen2_aggro == citizen2_aggro) {
					printf("citizen 2: STAY -> %d (aggro: %d)\n", citizen2, citizen2_aggro);
					citizen2_stay = 0;
				}
			}
			else  if (citizen2_stay != 1) { //�ù��� �������� ���� ��
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

//�ù�3 ���� ��Ȳ ��� 3
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
			if (citizen3_stay == 1) { //�ù��� ����������
				if (citizen3_aggro != before_citizen3_aggro) {
					printf("citizen 3: STAY -> %d (aggro: %d -> %d)\n", citizen3, before_citizen3_aggro, citizen3_aggro);
					citizen3_stay = 0;
				}
				else if (before_citizen3_aggro == citizen3_aggro) {
					printf("citizen 3: STAY -> %d (aggro: %d)\n", citizen3, citizen3_aggro);
					citizen3_stay = 0;
				}
			}
			else if (citizen3_stay != 1) { //�ù��� �������� ���� ��
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

//���� �����Ȳ ��� 3
void zombie_now_round3() {
	//���� �̵��� �� ���� ���� ��
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
		// ���� �̵��� ��
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

//������ �����Ȳ ��� 3
void madongseok_now_round3() {
	if (madongseok_no_move == 1) {
		madongseok_choose = 0;
		if (madongseok_aggro != before_madongseok_aggro) { //��׷ΰ� ������ �ٸ� ��
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

//�ù�1 �׼� ���� �� 3
void result1_round3() {
	if (citizen1 != -1) {
		if (citizen1 != 1) {
			printf("citizen1 does nothing.\n");
		}
	}
}

//�ù�2 �׼� ���� �� 3
void result2_round3() {
	if (citizen2 != -1) {
		if (citizen2 != 1) {
			printf("citizen2 does nothing.\n");
		}
	}
}

//�ù�1 �׼� ���� �� 3
void result3_round3() {
	if (citizen3 != -1) {
		if (citizen3 != 1) {
			printf("citizen3 does nothing.\n");
		}
	}
}

//������ �׼� 3
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

//������ �޽� �ൿ ��� 3
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

//������ ���� �ൿ ��� 3
void madongseok_result_provoke_round3() {
	if (madongseok_action_choose == ACTION_PROVOKE) {
		printf("madongseok provoked zombie..\n");
		before_madongseok_aggro = madongseok_aggro;
		madongseok_aggro = AGGRO_MAX;
		printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok, before_madongseok_aggro, madongseok_aggro, madongseok_stamina);
	}
}

// ������ �ٵ�� �ൿ ��� 3
void madongseok_result_pull_round3() {
	madongseok_random = rand() % 101;
	if (madongseok_action_choose == ACTION_PULL) {
		if ((100 - percentile_probability) >= madongseok_random) { //�ٵ�� ����
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			before_madongseok_aggro = madongseok_aggro;
			before_madongseok_stamina = madongseok_stamina;
			madongseok_aggro += 2; //��׷� 2 ����
			madongseok_stamina -= 1; //ü�� 1 ����
			madongseok_holding_zombie = 1; //���� ��Ҵ� ��ȣ ������
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

//������ ü���� 0�϶� �Ǵ� 3
int madongseok_stamina_zero_round3(void) {
	if (madongseok_stamina == 0) {
		printf("madongseok's stamina is 0\n");
		printf("GAME OVER\n");
		exit(1);
	}
	return 0;
}

//���� �ֺ��� �ƹ��� ���� �� 3
void zombie_action_nobody_round3() {
	if (zombie != madongseok - 1 && zombie != citizen1 + 1 && zombie != citizen2 + 1 && zombie != citizen3 + 1) {
		printf("zombie attacked nobody.\n\n");
	}
}

//���� �ֺ��� �ù� �Ǵ� �������� ���� �� 3
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

//���� �ֺ��� �� �� ���� �� ��׷� �� �� ���� 3
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

//�ù�1 Ż�⼺�� 3
int citizen1_win_round3() {
	if (citizen1 == 1) {
		printf("citizen1 succeeded in escaping\n");
		citizen_left++;
		citizen_count--;
		citizen1 = -1;
		printf("%d citizen(s) alive(s)..\n", citizen_count);

	}
}

//�ù� 2 Ż�⼺�� 3
int citizen2_win_round3() {
	if (citizen2 == 1) {
		printf("citizen2 succeeded in escaping\n");
		citizen_count--;
		citizen_left++;
		citizen2 = -1;
		printf("%d citizen(s) alive(s)..\n", citizen_count);

	}
}

//�ù� 3 Ż�⼺�� 3
int citizen3_win_round3() {
	if (citizen3 == 1) {
		printf("citizen3 succeeded in escaping\n");
		citizen_count--;
		citizen_left++;
		citizen3 = -1;
		printf("%d citizen(s) alive(s)..\n", citizen_count);

	}
}

//�ù� �� ���̶� Ż�� 3
void citizens_left_round3() {
	if (citizen_count == 0 && citizen_left >= 1) {
		printf("citizen(s) left\n");
		printf("YOU WIN!\n");
		exit(1);
	}
}                    //exit ���� ���� �ٲ����          

//�ù� ��� ��� 3
void citizens_die_round3() {
	if (citizen_count == 0 && citizen_left == 0) {
		printf("citizens all die\n");
		printf("YOU LOSE...\n");
		exit(1);
	}
}                         //exit ���� ���� �ٲ����

//¦���� �ʱ�ȭ �׼� 3
void madongseok_action_no_even_number_round3() {
	if (turn % 2 == 0) {
		madongseok_holding_zombie = 0;
	}
}

//�ٹٲٱ� �Լ� 3
void changing_the_line_round3() {
	printf("\n");
}

// �λ��� (2)�� �Լ����� �ҷ����� �Լ� - STAGE 1
void round1() {
	changing_the_line();
	charactor_position(zombie, citizen1, madongseok);
	first_train = train(zombie, citizen1, madongseok); //�⺻���� ���
	changing_the_line();
	if (round = 1) {
		while (1) { //�ݺ� ����
			turn++;//�� ����
			citizen_move(); //�ù��̵�
			zombie_move(); //�����̵�
			train(zombie, citizen1, madongseok); //���� ���
			citizen_win(); //�ù��� �̱� ���
			round_chack_round1(); //���� üũ
			citizen_now(); //�ù� �����Ȳ ���
			zombie_now(); //���� �����Ȳ ���
			madongseok_move_with_zombie();//������ �ֺ��� ���� ���� �� �̵�
			madongseok_move_no_zombie();//������ �ֺ��� ���� ���� �� �̵�
			train(zombie, citizen1, madongseok); //�� ��° ���� ���
			changing_the_line(); //�� ����
			madongseok_now(); //������ �����Ȳ ���
			citizen_win(); //�ù��� �̱� ���
			result(); //��� ���
			round_chack_round1(); //���� üũ
			zombie_action_nobody(); //���� �ൿ(�ֺ� �ƹ��� ������)
			zombie_action_aggro_fight(); //���� �ൿ(��׷� �� ���� �� ���� (�ù� ���ݽ� ���� ��, ������ ���ݽ� STM 1����))
			zombie_action_attack_citizen(); //���� �ൿ(�ù��� ������ ��)
			round_chack_round1(); //���� üũ
			madongseok_action(); //������ �׼�
			madongseok_action_no_even_number(); //¦�� ���� �� ����Ǹ� �ȵǴ� �͵� �ʱ�ȭ
			changing_the_line(); //�� ����
			madongseok_result_rest(); //������ �޽� �׼� ��� ���
			madongseok_result_provoke(); //������ ���� �׼� ��� ���
			madongseok_result_pull(); //������ ���� �ٵ�� �׼� ��� ���
			madongseok_stamina_zero(); //������ ü���� 0�� ��
			round_chack_round1(); //���� üũ
		}
	}
	else {
		return 0;
	}
}

//�λ��� (3) ���� �߰��� �Լ����� �ҷ����� �Լ� - STAGE 2
void round2() {
	changing_the_line();
	charactor_position_round2(zombie, citizen1, madongseok, villain);
	first_train = train_round2(zombie, citizen1, madongseok, villain); //�⺻���� ���
	changing_the_line();
	if (round = 2) {
		while (1) { //�ݺ� ����
			turn++;//�� ����
			citizen_move_round2(); //�ù��̵�
			villain_move_round2(); //�����̵�
			zombie_move_round2(); //�����̵�
			train_round2(zombie, citizen1, madongseok, villain); //���� ���
			citizen_win_round2(); //�ù��� �̱� ���
			citizen_now_round2(); //�ù� �����Ȳ ���
			villain_now_round2(); //���� �����Ȳ ���
			zombie_now_round2(); //���� �����Ȳ ���
			madongseok_move_with_zombie_round2();//������ �ֺ��� ���� ���� �� �̵�
			madongseok_move_no_zombie_round2();//������ �ֺ��� ���� ���� �� �̵�
			train_round2(zombie, citizen1, madongseok, villain); //�� ��° ���� ���
			changing_the_line(); //�� ����
			madongseok_now_round2(); //������ �����Ȳ ���
			citizen_win_round2(); //�ù��� �̱� ���
			citizen_action_round2(); //��� ���
			villain_action_round2(); //���� �ൿ 
			zombie_action_nobody_round2(); //���� �ൿ(�ֺ� �ƹ��� ������)
			zombie_action_aggro_fight_round2(); //���� �ൿ(��׷� �� ���� �� ���� (�ù� ���ݽ� ���� ��, ������ ���ݽ� STM 1����))
			zombie_action_attack_citizen_round2(); //���� �ൿ(�ù��� ������ ��)
			madongseok_action_round2(); //������ �׼�
			madongseok_action_no_even_number(); //¦�� ���� �� ����Ǹ� �ȵǴ� �͵� �ʱ�ȭ
			changing_the_line(); //�� ����
			madongseok_result_rest_round2(); //������ �޽� �׼� ��� ���
			madongseok_result_provoke_round2(); //������ ���� �׼� ��� ���
			madongseok_result_pull_round2(); //������ ���� �ٵ�� �׼� ��� ���
			madongseok_stamina_zero_round2(); //������ ü���� 0�� ��
		}
	}
}

//�λ��� (3) �ù�'��'�� ������ �Լ����� �ҷ����� �Լ� - STAGE 3
void round3() {
	changing_the_line_round3();
	printf("\n\n");
	printf("* ROUND 3\n\n");
	citizens_line_round3();
	charactor_position(zombie, citizen1, citizen2, citizen3, madongseok);
	first_train = train_round3(zombie, citizen1, citizen2, citizen3, madongseok); //�⺻���� ���
	changing_the_line();
	while (1) { //�ݺ� ����
		turn++;//�� ����
		if (citizen3 != -1) {
			citizen3_move_round3(); //�ù��̵�
		}
		if (citizen2 != -1) {
			citizen2_move_round3(); //�ù��̵�
		}
		if (citizen1 != -1) {
			citizen1_move_round3(); //�ù��̵�
		}
		zombie_move_round3(); //�����̵�

		train_round3(zombie, citizen1, citizen2, citizen3, madongseok); //���� ���

		citizen3_now_round3(); //�ù�3 �����Ȳ ���
		citizen2_now_round3(); //�ù�2 �����Ȳ ���
		citizen1_now_round3(); //�ù�1 �����Ȳ ���
		zombie_now_round3(); //���� �����Ȳ ���

		madongseok_move_with_zombie_round3();//������ �ֺ��� ���� ���� �� �̵�
		madongseok_move_no_zombie_round3();//������ �ֺ��� ���� ���� �� �̵�

		train_round3(zombie, citizen1, citizen2, citizen3, madongseok); //�� ��° ���� ���

		changing_the_line_round3(); //�� ����

		madongseok_now_round3(); //������ �����Ȳ ���

		citizen3_win_round3(); //�ù�1 Ż���������
		result3_round3(); //�ù� 1��� ���
		citizen2_win_round3(); //�ù�2 Ż���������
		result2_round3(); //�ù� 2��� ���
		citizen1_win_round3(); //�ù�3 Ż���������
		result1_round3();  //�ù� 3��� ���

		citizens_left_round3(); //�ù��� Ż�⿡ �����Ͽ��°�?
		citizens_die_round3(); //�ù��� Ż�⿡ �����Ͽ��°�?


		zombie_action_nobody_round3(); //���� �ൿ(�ֺ� �ƹ��� ������)
		zombie_action_aggro_fight_round3(); //���� �ൿ(��׷� �� ���� �� ���� (�ù� ���ݽ� ���� ��, ������ ���ݽ� STM 1����))
		zombie_action_attack_citizen_round3(); //���� �ൿ(�ù��� ������ ��)

		citizens_left_round3(); //�ù��� Ż�⿡ �����Ͽ��°�?
		citizens_die_round3(); //�ù��� Ż�⿡ �����Ͽ��°�?

		madongseok_action_round3(); //������ �׼�
		madongseok_action_no_even_number_round3(); //¦�� ���� �� ����Ǹ� �ȵǴ� �͵� �ʱ�ȭ
		changing_the_line_round3(); //�� ����
		madongseok_result_rest_round3(); //������ �޽� �׼� ��� ���
		madongseok_result_provoke_round3(); //������ ���� �׼� ��� ���
		madongseok_result_pull_round3(); //������ ���� �ٵ�� �׼� ��� ���

		madongseok_stamina_zero_round3(); //������ ü���� 0�� ��
	}
}

//�� ���� �Լ�
int main(void) {
	srand((unsigned int)time(NULL)); //���� ����
	intro(); //��Ʈ�� ���
	train_length = variables("train length", LEN_MIN, LEN_MAX); //���� ���� �Է�
	madongseok_stamina = variables("madongseok stamina", STM_MIN, STM_MAX); //������ ���׹̳� �Է�
	madongseok_stamina_get = madongseok_stamina;
	percentile_probability = variables("percentile probability \'p\'", PROB_MIN, PROB_MAX); //Ȯ�� �Է�

	printf("\n* ROUND 1\n");
	round1();
}