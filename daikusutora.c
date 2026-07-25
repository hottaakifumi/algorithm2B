#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define V 58
#define MAX_EDGES 10 // 各ノードから出る最大経路数（余裕を持たせて10）

// ノードを分かりやすく管理するための列挙型
enum Node {
    N_s, N_a, N_b, N_c, N_d, N_e, N_f, N_g, N_h, N_i, N_j, N_k, N_l, N_m,
    N_n, N_o, N_p, N_q,
    N_r, N_t, N_u, N_v, N_w, N_x,
    N_J, N_I, N_H, N_G,
    N_y, N_z, N_A, N_B, N_C, N_D,
    N_F, N_E,
    N_K, N_L, N_M, N_N, N_O, N_P, N_Q, N_R, N_S1,
    N_T, N_U, N_V, N_W, N_X1, N_Y,
    N_b1, N_c1, N_d1, N_e1, N_f1, N_Z1,
    N_a1
};

// 経路（辺）を管理する構造体
typedef struct {
    int to;
    int weight;
} Edge;

// 隣接リストと、各ノードの接続数を管理する配列
Edge graph[V][MAX_EDGES];
int degree[V];

// グラフの初期化
void init_graph() {
    for (int i = 0; i < V; i++) {
        degree[i] = 0;
    }
}

// 経路（辺）を追加する関数
void addEdge(int u, int v, int weight) {
    graph[u][degree[u]] = (Edge){v, weight};
    degree[u]++;
}

// 隣接リストを用いたダイクストラ法の本体
int dijkstra(int src, int dest) {
    int dist[V];
    bool used[V];

    // 初期化
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        used[i] = false;
    }
    dist[src] = 0;

    // 最短経路の計算
    while (1) {
        int u = -1;
        // 未確定のノードの中で、最小の所要時間を持つノードを探す
        for (int i = 0; i < V; i++) {
            if (!used[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }

        // 全てのノードを処理した、または目的地に到達できない場合
        if (u == -1 || dist[u] == INT_MAX) break;
        used[u] = true;

        // 目的地に到達したらループを抜ける（高速化）
        if (u == dest) break;

        // 【ここが隣接行列との違い】繋がっている経路（degree）だけを確認する
        for (int i = 0; i < degree[u]; i++) {
            Edge e = graph[u][i];
            if (dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
            }
        }
    }
    return dist[dest];
}

int main() {
    // グラフの初期化
    init_graph();

    // === 画像データに基づく経路と所要時間のマッピング ===
    // 上部ライン
    addEdge(N_s, N_a, 3); addEdge(N_a, N_b, 2); addEdge(N_b, N_c, 2);
    addEdge(N_c, N_d, 3); addEdge(N_d, N_e, 2); addEdge(N_e, N_f, 3);
    addEdge(N_f, N_g, 1); addEdge(N_g, N_h, 1); addEdge(N_h, N_i, 2);
    addEdge(N_i, N_j, 1); addEdge(N_j, N_k, 2); addEdge(N_k, N_l, 2);
    addEdge(N_l, N_m, 2);

    // iからの分岐
    addEdge(N_i, N_n, 2); addEdge(N_n, N_o, 2); addEdge(N_o, N_p, 2); addEdge(N_p, N_q, 2);
    addEdge(N_i, N_F, 3); addEdge(N_F, N_E, 2); addEdge(N_E, N_B, 3);

    // sからの斜めライン
    addEdge(N_s, N_r, 2); addEdge(N_r, N_t, 2); addEdge(N_t, N_u, 2);
    addEdge(N_u, N_v, 2); addEdge(N_v, N_w, 2); addEdge(N_w, N_x, 2);

    // fからの縦ライン
    addEdge(N_f, N_J, 2); addEdge(N_J, N_I, 2); addEdge(N_I, N_H, 1);
    addEdge(N_H, N_G, 2); addEdge(N_G, N_x, 1);

    // 中央ライン
    addEdge(N_x, N_y, 2); addEdge(N_y, N_z, 2); addEdge(N_z, N_A, 2);
    addEdge(N_A, N_B, 1); addEdge(N_B, N_C, 2); addEdge(N_C, N_D, 2);

    // xからの斜めライン
    addEdge(N_x, N_K, 1); addEdge(N_K, N_L, 2); addEdge(N_L, N_M, 1);
    addEdge(N_M, N_N, 2); addEdge(N_N, N_O, 1); addEdge(N_O, N_P, 1);
    addEdge(N_P, N_Q, 2); addEdge(N_Q, N_R, 1); addEdge(N_R, N_S1, 2);

    // Bからの縦ライン
    addEdge(N_B, N_T, 2); addEdge(N_T, N_U, 2); addEdge(N_U, N_V, 2);
    addEdge(N_V, N_W, 2); addEdge(N_W, N_X1, 2); addEdge(N_X1, N_Y, 3);

    // zからの縦ライン
    addEdge(N_z, N_b1, 2); addEdge(N_b1, N_c1, 1); addEdge(N_c1, N_d1, 2);
    addEdge(N_d1, N_e1, 2); addEdge(N_e1, N_f1, 1); addEdge(N_f1, N_Z1, 2);

    // 下部ライン
    addEdge(N_S1, N_a1, 3); addEdge(N_a1, N_Z1, 4); addEdge(N_Z1, N_Y, 3);

    // === 実行と時間計測 ===
    int start_node = N_s;
    int dest_node = N_Y;
    int shortest_time = 0;
    int loops = 10000; // 計測精度を上げるためのループ回数

    clock_t start_time = clock();

    for (int i = 0; i < loops; i++) {
        shortest_time = dijkstra(start_node, dest_node);
    }

    clock_t end_time = clock();

    // 実行時間をミリ秒に変換（1回あたりの平均時間に直す）
    double total_time_ms = ((double) (end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    double avg_time_ms = total_time_ms / loops;

    printf("--- 結果 (隣接リスト版 ダイクストラ法) ---\n");
    printf("s から Y までの最短所要時間: %d\n", shortest_time);
    printf("アルゴリズムの実行時間 (平均): %.6f ミリ秒\n", avg_time_ms);

    return 0;
}