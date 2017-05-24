#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX_WEIGHT  5.0 /* 重みの初期値の範囲 */
#define D 50           /* 次元数 */
int sign(double s);
double random_double(double n);
void initialize_weights();
void bindecomp();
double  w[D], r[D];
/* sign関数 */
int sign(double s)
{
	if (s >= 0) {
		return 1;
	}
	else {
		return -1;
	}
}
/* ランダム作るやつ */
double random_double(double n)
/* 0.0 から n まで（ n より小さい）の実数の乱数を返す */
{
	return (double)rand() / ((double)RAND_MAX + 1.0) * n;
}
void initialize_weights()
{
	int seed, i; /* 作業変数 */
	//FILE *W;
	//errno_t er3;

	//er3 = fopen_s(&W, "w.txt", "w");  // ファイルを書き込み用にオープン
	/*if (er3 != 0) {    // オープンに失敗した場合
		printf("cannot open\n");         // エラーメッセージを出して
		exit(1);                         // 異常終了
	}*/
					/* 乱数の初期化 */
	printf("乱数の系列を初期化します．\n");
	printf("乱数のシード（任意の正の整数）:1\n");
	/*scanf_s("%d", &seed);
	if (seed < 1) seed = 1;
	srand(seed);*/
	srand(1);
	for (i = 0; i < D; i++) {
		w[i] = 2.0 * MAX_WEIGHT
			* random_double(1.0) - MAX_WEIGHT;
		//r[i] = w[i];  /* wで初期化 */
		//fprintf(W, "%f ", w[i]);
	}
	//fclose(W);         /* ファイルをクローズ(閉じる) */
}
/* 重みの2値化 */
void bindecomp()
{
	FILE *M, *cc;         // 出力ストリーム
	errno_t er, er2;
	int i, j, l, p, K;
	double c[100], mm[100][D];
	char ma[10], ca[10];
	for (K = 1; K <= 100; K++) {
		for (l = 0; l < D; l++) {
			r[l] = w[l];  /* wで初期化 */
			for (p = 0; p < 100; p++) {
				c[p] = 0.0;
				mm[p][l] = 0.0;
			}
		}
		sprintf_s(ma, sizeof(ma), "%s%d%s", "M", K, ".txt");
		sprintf_s(ca, sizeof(ca), "%s%d%s", "c", K, ".txt");
		er = fopen_s(&M, ma, "w");  // ファイルを書き込み用にオープン
		er2 = fopen_s(&cc, ca, "w");  // ファイルを書き込み用にオープン
		if (er != 0 || er2 != 0) {    // オープンに失敗した場合
			printf("cannot open\n");         // エラーメッセージを出して
			exit(1);                         // 異常終了
		}
		printf("%s %s\n", ma, ca);
		/* Greedyアルゴリズム */
		for (i = 0; i < K; i++) { /* 基底数 */
			for (j = 0; j < D; j++) { /* 入力次元数 */
				mm[i][j] = sign(r[j]); /* m_i = sign(r) */
				c[i] += mm[i][j] * r[j]; /* r^T m_i / ||m_i||^2 */
				fprintf(M, "%f ", mm[i][j]); // ファイルに書く
			}
			fprintf(M, "\n"); // ファイルに書く
			c[i] /= D;
			fprintf(cc, "%f\n", c[i]);
			for (j = 0; j < D; ++j) {
				r[j] = r[j] - mm[i][j] * c[i]; /* r= r - m_i c_i */
			}
		}
		fclose(M);          /* ファイルをクローズ(閉じる) */
		fclose(cc);         /* ファイルをクローズ(閉じる) */
	}
}
int main(void) {
	initialize_weights();
	bindecomp();
	FILE *gp;

	/* ---- グラフ作成 ---- */
	gp = popen("gnuplot -persist", "w");
	fprintf(gp, "set xrange [1:20]\n");
	fprintf(gp, "set yrange [3.0:0.0]\n");
	fprintf(gp, "plot '-' with lines linetype 1 title \"sin\"\n");
	for (i = 0; i <= NX; i++) {
		fprintf(gp, "%f\t%f\n", x[i], y[i]);    // データの書き込み
	}
	fprintf(gp, "e\n");
	pclose(gp);
}