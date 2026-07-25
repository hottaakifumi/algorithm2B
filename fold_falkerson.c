#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_VERTICES 100
#define INF 1000000000
#define NUM_RUNS 10000 // 計測用の実行回数

// ノードを分かりやすく管理するための列挙型（前回と同じインデックス）
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

// 残余グラフ用の辺構造体
typedef struct {
    int to;
    int cap;
    int cost;
    int rev; // 逆辺のインデックス
} Edge;

Edge graph[MAX_VERTICES][MAX_VERTICES];
int degree[MAX_VERTICES];

// グラフの初期化
void init_graph() {
    for (int i = 0; i < MAX_VERTICES; i++) {
        degree[i] = 0;
    }
}

// 辺の追加（残余グラフ構築のため、容量0の逆辺も同時に追加する）
void add_edge(int from, int to, int cap, int cost) {
    graph[from][degree[from]] = (Edge){to, cap, cost, degree[to]};
    graph[to][degree[to]] = (Edge){from, 0, -cost, degree[from]};
    degree[from]++;
    degree[to]++;
}

// 最小コストフロー（ダイクストラ法を用いたフォード・ファルカーソン法）
int min_cost_flow(int s, int t, int f, int num_nodes) {
    int res = 0;
    int dist[MAX_VERTICES];
    int prev_v[MAX_VERTICES];
    int prev_e[MAX_VERTICES];
    int used[MAX_VERTICES];

    while (f > 0) {
        for (int i = 0; i < num_nodes; i++) {
            dist[i] = INF;
            used[i] = 0;
        }
        dist[s] = 0;

        // ダイクストラ法による探索
        while (1) {
            int v = -1;
            for (int u = 0; u < num_nodes; u++) {
                if (!used[u] && (v == -1 || dist[u] < dist[v])) {
                    v = u;
                }
            }
            if (v == -1 || dist[v] == INF) break;
            used[v] = 1;

            for (int i = 0; i < degree[v]; i++) {
                Edge *e = &graph[v][i];
                if (e->cap > 0 && dist[e->to] > dist[v] + e->cost) {
                    dist[e->to] = dist[v] + e->cost;
                    prev_v[e->to] = v;
                    prev_e[e->to] = i;
                }
            }
        }

        if (dist[t] == INF) {
            return -1; // 到達不可
        }

        int d = f;
        for (int v = t; v != s; v = prev_v[v]) {
            if (d > graph[prev_v[v]][prev_e[v]].cap) {
                d = graph[prev_v[v]][prev_e[v]].cap;
            }
        }

        f -= d;
        res += d * dist[t];

        for (int v = t; v != s; v = prev_v[v]) {
            Edge *e = &graph[prev_v[v]][prev_e[v]];
            e->cap -= d;
            graph[v][e->rev].cap += d;
        }
    }
    return res;
}

int main() {
    int num_nodes = 58; 
    int start_node = N_s; 
    int end_node = N_Y;   
    int flow = 1;       

    clock_t start_time, end_time;
    double total_time = 0.0;
    int min_cost = 0;

    start_time = clock();

    // 計測のため10,000回繰り返す
    for (int i = 0; i < NUM_RUNS; i++) {
        init_graph();
        
        // === 画像データに基づく経路マッピング (容量はすべて1) ===
        // 上部ライン
        add_edge(N_s, N_a, 1, 3); add_edge(N_a, N_b, 1, 2); add_edge(N_b, N_c, 1, 2);
        add_edge(N_c, N_d, 1, 3); add_edge(N_d, N_e, 1, 2); add_edge(N_e, N_f, 1, 3);
        add_edge(N_f, N_g, 1, 1); add_edge(N_g, N_h, 1, 1); add_edge(N_h, N_i, 1, 2);
        add_edge(N_i, N_j, 1, 1); add_edge(N_j, N_k, 1, 2); add_edge(N_k, N_l, 1, 2);
        add_edge(N_l, N_m, 1, 2);

        // iからの分岐
        add_edge(N_i, N_n, 1, 2); add_edge(N_n, N_o, 1, 2); add_edge(N_o, N_p, 1, 2); add_edge(N_p, N_q, 1, 2);
        add_edge(N_i, N_F, 1, 3); add_edge(N_F, N_E, 1, 2); add_edge(N_E, N_B, 1, 3);

        // sからの斜めライン
        add_edge(N_s, N_r, 1, 2); add_edge(N_r, N_t, 1, 2); add_edge(N_t, N_u, 1, 2);
        add_edge(N_u, N_v, 1, 2); add_edge(N_v, N_w, 1, 2); add_edge(N_w, N_x, 1, 2);

        // fからの縦ライン
        add_edge(N_f, N_J, 1, 2); add_edge(N_J, N_I, 1, 2); add_edge(N_I, N_H, 1, 1);
        add_edge(N_H, N_G, 1, 2); add_edge(N_G, N_x, 1, 1);

        // 中央ライン
        add_edge(N_x, N_y, 1, 2); add_edge(N_y, N_z, 1, 2); add_edge(N_z, N_A, 1, 2);
        add_edge(N_A, N_B, 1, 1); add_edge(N_B, N_C, 1, 2); add_edge(N_C, N_D, 1, 2);

        // xからの斜めライン
        add_edge(N_x, N_K, 1, 1); add_edge(N_K, N_L, 1, 2); add_edge(N_L, N_M, 1, 1);
        add_edge(N_M, N_N, 1, 2); add_edge(N_N, N_O, 1, 1); add_edge(N_O, N_P, 1, 1);
        add_edge(N_P, N_Q, 1, 2); add_edge(N_Q, N_R, 1, 1); add_edge(N_R, N_S1, 1, 2);

        // Bからの縦ライン
        add_edge(N_B, N_T, 1, 2); add_edge(N_T, N_U, 1, 2); add_edge(N_U, N_V, 1, 2);
        add_edge(N_V, N_W, 1, 2); add_edge(N_W, N_X1, 1, 2); add_edge(N_X1, N_Y, 1, 3);

        // zからの縦ライン
        add_edge(N_z, N_b1, 1, 2); add_edge(N_b1, N_c1, 1, 1); add_edge(N_c1, N_d1, 1, 2);
        add_edge(N_d1, N_e1, 1, 2); add_edge(N_e1, N_f1, 1, 1); add_edge(N_f1, N_Z1, 1, 2);

        // 下部ライン
        add_edge(N_S1, N_a1, 1, 3); add_edge(N_a1, N_Z1, 1, 4); add_edge(N_Z1, N_Y, 1, 3);
        
        // 最小コストフローの計算
        min_cost = min_cost_flow(start_node, end_node, flow, num_nodes);
    }

    end_time = clock();

    total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000.0;
    double avg_time_ms = total_time / NUM_RUNS;

    printf("--- 最小コストフロー (流量: %d) ---\n", flow);
    if (min_cost == -1) {
        printf("指定された流量を流すことができません。\n");
    } else {
        printf("最小コスト(所要時間): %d\n", min_cost);
    }
    printf("1回あたりの平均実行時間: %f ミリ秒\n", avg_time_ms);

    return 0;
}