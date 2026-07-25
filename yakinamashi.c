#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_ITEMS 100
#define BUDGET 30000
#define COOL_RATE 0.9999 // 冷却率

typedef struct {
    char name[10];
    int price;
    int score;
} Item;

// 現在の解(パス)のコスト(価値)を計算する関数
int EvaluatePath(Item items[], int n, int state[], int *total_price) {
    int score = 0;
    *total_price = 0;
    for (int i = 0; i < n; i++) {
        if (state[i]) {
            score += items[i].score;
            *total_price += items[i].price;
        }
    }
    // 予算オーバーの場合はペナルティとして無効なスコア(-1)を返す
    if (*total_price > BUDGET) {
        return -1;
    }
    return score;
}

// 判定関数: 資料のP = e^((f(next)-f(curr))/T) を計算
// ※今回はコスト(スコア)を「最大化」したいので、(newcost - cost) としています。
int Probability(double cost, double newcost, double T) {
    // 0.0から1.0までの乱数
    double rnd = (double)rand() / RAND_MAX;
    
    // スコアが改善(newcost > cost)していれば、probは1以上になり確実に遷移する
    // 悪化していても、温度Tが高ければ高い確率で遷移を許容する
    double prob = exp((newcost - cost) / T);
    
    // 1だったら遷移する、0だったら遷移しない
    return (prob > rnd) ? 1 : 0;
}

int main() {
    Item items[MAX_ITEMS];
    int n = 0;
    
    // データの読み込み (前回作成した menu.txt を使用)
    FILE *fp = fopen("maxflow.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "エラー: maxflow.txt が開けません。\n");
        return 1;
    }
    while (fscanf(fp, "%s %d %d", items[n].name, &items[n].price, &items[n].score) != EOF) {
        n++;
    }
    fclose(fp);

    srand((unsigned int)time(NULL));

    // --- 実行時間の計測開始 ---
    clock_t start_time = clock();

    int current_state[MAX_ITEMS] = {0}; // 初期解（何も選んでいない状態）
    int best_state[MAX_ITEMS] = {0};
    int current_price = 0, best_price = 0;
    int cost = EvaluatePath(items, n, current_state, &current_price);
    int best_cost = cost;

    // 探索ループ: 1000度から開始し、繰り返し毎に温度を下げる
    for (double T = 1000.0; T > 0.1; T *= COOL_RATE) {
        cost = EvaluatePath(items, n, current_state, &current_price);
        
        // 近傍解の作成 (ランダムな1品の「選ぶ/選ばない」をスワップ(反転)する)
        int flip_idx = rand() % n;
        current_state[flip_idx] = 1 - current_state[flip_idx];
        
        int neighbor_price = 0;
        int newcost = EvaluatePath(items, n, current_state, &neighbor_price);
        
        // 予算内(有効な解)の場合のみ遷移を判定
        if (newcost != -1) {
            // Probability()関数でそのスワップを採用するか判断
            if (Probability(cost, newcost, T)) {
                // 採用する場合: 何もしない (反転した状態を維持)
                // さらに、これまでのベスト解を上回っていれば記録を更新
                if (newcost > best_cost) {
                    best_cost = newcost;
                    best_price = neighbor_price;
                    for (int i = 0; i < n; i++) best_state[i] = current_state[i];
                }
            } else {
                // そのスワップを採用しないので、元に戻す
                current_state[flip_idx] = 1 - current_state[flip_idx];
            }
        } else {
            // 予算オーバーの解なので元に戻す
            current_state[flip_idx] = 1 - current_state[flip_idx];
        }
    }

    // --- 実行時間の計測終了 ---
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // 結果の出力
    printf("最大の評価合計: %d\n", best_cost);
    printf("---------------------------\n");
    printf("選ばれた料理:\n");
    for (int i = 0; i < n; i++) {
        if (best_state[i]) {
            printf("料理%s (価格: %5d円, 評価: %d)\n", items[i].name, items[i].price, items[i].score);
        }
    }
    printf("---------------------------\n");
    printf("合計使用金額: %d円\n", best_price);
    printf("残り予算: %d円\n", BUDGET - best_price);
    printf("---------------------------\n");
    printf("アルゴリズムの実行時間: %.6f 秒\n", elapsed_time);

    return 0;
}