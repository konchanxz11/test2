#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX_WEIGHT  5.0 /* �d�݂̏����l�͈̔� */
#define D 50           /* ������ */
int sign(double s);
double random_double(double n);
void initialize_weights();
void bindecomp();
double  w[D], r[D];
/* sign�֐� */
int sign(double s)
{
	if (s >= 0) {
		return 1;
	}
	else {
		return -1;
	}
}
/* �����_������� */
double random_double(double n)
/* 0.0 ���� n �܂Łi n ��菬�����j�̎����̗�����Ԃ� */
{
	return (double)rand() / ((double)RAND_MAX + 1.0) * n;
}
void initialize_weights()
{
	int seed, i; /* ��ƕϐ� */
	//FILE *W;
	//errno_t er3;

	//er3 = fopen_s(&W, "w.txt", "w");  // �t�@�C�����������ݗp�ɃI�[�v��
	/*if (er3 != 0) {    // �I�[�v���Ɏ��s�����ꍇ
		printf("cannot open\n");         // �G���[���b�Z�[�W���o����
		exit(1);                         // �ُ�I��
	}*/
					/* �����̏����� */
	printf("�����̌n������������܂��D\n");
	printf("�����̃V�[�h�i�C�ӂ̐��̐����j:1\n");
	/*scanf_s("%d", &seed);
	if (seed < 1) seed = 1;
	srand(seed);*/
	srand(1);
	for (i = 0; i < D; i++) {
		w[i] = 2.0 * MAX_WEIGHT
			* random_double(1.0) - MAX_WEIGHT;
		//r[i] = w[i];  /* w�ŏ����� */
		//fprintf(W, "%f ", w[i]);
	}
	//fclose(W);         /* �t�@�C�����N���[�Y(����) */
}
/* �d�݂�2�l�� */
void bindecomp()
{
	FILE *M, *cc;         // �o�̓X�g���[��
	errno_t er, er2;
	int i, j, l, p, K;
	double c[100], mm[100][D];
	char ma[10], ca[10];
	for (K = 1; K <= 100; K++) {
		for (l = 0; l < D; l++) {
			r[l] = w[l];  /* w�ŏ����� */
			for (p = 0; p < 100; p++) {
				c[p] = 0.0;
				mm[p][l] = 0.0;
			}
		}
		sprintf_s(ma, sizeof(ma), "%s%d%s", "M", K, ".txt");
		sprintf_s(ca, sizeof(ca), "%s%d%s", "c", K, ".txt");
		er = fopen_s(&M, ma, "w");  // �t�@�C�����������ݗp�ɃI�[�v��
		er2 = fopen_s(&cc, ca, "w");  // �t�@�C�����������ݗp�ɃI�[�v��
		if (er != 0 || er2 != 0) {    // �I�[�v���Ɏ��s�����ꍇ
			printf("cannot open\n");         // �G���[���b�Z�[�W���o����
			exit(1);                         // �ُ�I��
		}
		printf("%s %s\n", ma, ca);
		/* Greedy�A���S���Y�� */
		for (i = 0; i < K; i++) { /* ��ꐔ */
			for (j = 0; j < D; j++) { /* ���͎����� */
				mm[i][j] = sign(r[j]); /* m_i = sign(r) */
				c[i] += mm[i][j] * r[j]; /* r^T m_i / ||m_i||^2 */
				fprintf(M, "%f ", mm[i][j]); // �t�@�C���ɏ���
			}
			fprintf(M, "\n"); // �t�@�C���ɏ���
			c[i] /= D;
			fprintf(cc, "%f\n", c[i]);
			for (j = 0; j < D; ++j) {
				r[j] = r[j] - mm[i][j] * c[i]; /* r= r - m_i c_i */
			}
		}
		fclose(M);          /* �t�@�C�����N���[�Y(����) */
		fclose(cc);         /* �t�@�C�����N���[�Y(����) */
	}
}
int main(void) {
	initialize_weights();
	bindecomp();
	FILE *gp;

	/* ---- �O���t�쐬 ---- */
	gp = popen("gnuplot -persist", "w");
	fprintf(gp, "set xrange [1:20]\n");
	fprintf(gp, "set yrange [3.0:0.0]\n");
	fprintf(gp, "plot '-' with lines linetype 1 title \"sin\"\n");
	for (i = 0; i <= NX; i++) {
		fprintf(gp, "%f\t%f\n", x[i], y[i]);    // �f�[�^�̏�������
	}
	fprintf(gp, "e\n");
	pclose(gp);
}