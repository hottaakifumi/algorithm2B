#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITEMS 100
#define BUDGET 30000

typedef struct {
    char name[10];
    int price;
    int score;
} Item;

int main() {
    Item items[MAX_ITEMS];
    int n = 0;
    
    // 1. データの読み込み
    FILE *fp = fopen("nappuzakku.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "エラー: nappuzakku.txt が開けません。\n");
        return 1;
    }
    while (fscanf(fp, "%s %d %d", items[n].name, &items[n].price, &items[n].score) != EOF) {
        n++;
    }
    fclose(fp);

    // --- 実行時間の計測開始 ---
    clock_t start_time = clock();

    // 2. DPテーブルの動的確保
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int *)calloc(BUDGET + 1, sizeof(int));
    }

    // 3. 動的計画法による計算
    for (int i = 0; i < n; i++) {
        for (int w = 0; w <= BUDGET; w++) {
            if (w >= items[i].price) {
                int select = dp[i][w - items[i].price] + items[i].score;
                int not_select = dp[i][w];
                dp[i+1][w] = (select > not_select) ? select : not_select;
            } else {
                dp[i+1][w] = dp[i][w];
            }
        }
    }

    // 4. 解の復元（どれを選んだか特定する）
    int w = BUDGET;
    int total_price = 0;
    int selected_flags[MAX_ITEMS] = {0}; // 選んだ料理を記録するフラグ

    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            selected_flags[i-1] = 1; // 料理が選ばれた
            w -= items[i-1].price;
            total_price += items[i-1].price;
        }
    }

    // --- 実行時間の計測終了 ---
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // 結果の出力
    printf("最大の評価合計: %d\n", dp[n][BUDGET]);
    printf("---------------------------\n");
    printf("選ばれた料理:\n");
    // 上から順に綺麗に出力するため、フラグ配列を使って表示
    for (int i = 0; i < n; i++) {
        if (selected_flags[i]) {
            printf("料理%s (価格: %5d円, 評価: %d)\n", items[i].name, items[i].price, items[i].score);
        }
    }
    printf("---------------------------\n");
    printf("合計使用金額: %d円\n", total_price);
    printf("残り予算: %d円\n", BUDGET - total_price);
    printf("---------------------------\n");
    printf("アルゴリズムの実行時間: %.6f 秒\n", elapsed_time);

    // メモリの解放
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return 0;
}