#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <windows.h>
#define MAX_N 25
#define MAX_CHAR 256
#define MAX_WAY 1000
#define INF 1e9
void in_menu() {
    #define RESET   "\033[0m"
    #define RED     "\033[31m"
    #define GREEN   "\033[32m"
    #define YELLOW  "\033[33m"
    #define BLUE    "\033[34m"
    #define CYAN    "\033[36m"
    printf(CYAN " =============================================================\n" RESET);
    printf(GREEN "| %-61s|\n" RESET, "TRUONG DAI HOC BACH KHOA - DAI HOC DA NANG");
    printf(GREEN "| %-61s|\n" RESET, "KHOA: CONG NGHE THONG TIN");
    printf(YELLOW "| %-61s|\n" RESET, "PBL1: DO AN LAP TRINH TINH TOAN");
    printf(YELLOW "| %-61s|\n" RESET, "DE TAI: BAI TOAN ONG TIM MAT");
    printf(BLUE "| %-61s|\n" RESET, "GIAO VIEN HUONG DAN: TS. NGUYEN VAN HIEU");
    printf(RED "| %-61s|\n" RESET, "SINH VIEN: TRAN VAN KHANH HUNG - 25T_KHDL");
    printf(RED "| %-61s|\n" RESET, "           PHAM PHU HUNG - 25T_KHDL");
    printf(CYAN " =============================================================\n\n" RESET);
}
void in_mt(int weight[MAX_N][MAX_N], int n) {
    printf("Ma tran trong so:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", weight[i][j]);
        }
        printf("\n");
    }
}

void xoa_datafile(char *filename){
    FILE *out = fopen(filename, "w");
    fclose(out);
}
char *tenFileout = "output.txt", *tenFilein = "data.txt";
struct Nut {
    char tendinh;
    struct Nut* next;
};
void doc_input(FILE *file,
               int weight[MAX_N][MAX_N],
               int indexMap[MAX_CHAR],
               char nameIndex[MAX_N],
               int *n,
               char *goc) {

    int i, j;

    for (i = 0; i < MAX_N; i++) {
        for (j = 0; j < MAX_N; j++) {
            weight[i][j] = -1;
        }
    }

    for (i = 0; i < MAX_CHAR; i++) {
        indexMap[i] = -1;
    }

    *n = 0;

    fscanf(file, " %c", goc);

    indexMap[(unsigned char)*goc] = *n;
    nameIndex[*n] = *goc;
    (*n)++;

    char u, v;
    int w;

    while (fscanf(file, " %c %c %d", &u, &v, &w) == 3) {

        if (indexMap[(unsigned char)u] == -1) {
            indexMap[(unsigned char)u] = *n;
            nameIndex[*n] = u;
            (*n)++;
        }

        if (indexMap[(unsigned char)v] == -1) {
            indexMap[(unsigned char)v] = *n;
            nameIndex[*n] = v;
            (*n)++;
        }

        int iu = indexMap[(unsigned char)u];
        int iv = indexMap[(unsigned char)v];

        if (weight[iu][iv] == -1 || w > weight[iu][iv]) {
            weight[iu][iv] = w;
        }
    }

    if (file != stdin) {
        fclose(file);
    }
}
void ghi_ketqua(const char *filename, struct Nut* result,
                 int best, char goc, const char *nameTypeSolve) {

    FILE *out = fopen(filename, "a");
    if (!out) out = stdout;

    if (best <= 0) {
        fprintf(out, "\nKhong co chu trinh di qua tat ca cac dinh\n");
        printf("\nKhong co chu trinh di qua tat ca cac dinh\n");
    } else {
        fprintf(out, "\nHanh trinh toi uu (%d) (%s):\n", best, nameTypeSolve);
        printf("\nHanh trinh toi uu (%d) (%s):\n", best, nameTypeSolve);

        while (result->tendinh != goc)
            result = result->next;

        struct Nut *tmp = result;

        do {
            fprintf(out, "%c -> ", tmp->tendinh);
            printf("%c -> ", tmp->tendinh);
            tmp = tmp->next;
        } while (tmp->tendinh != goc);

        fprintf(out, "%c\n", tmp->tendinh);
        printf("%c\n", tmp->tendinh);
    }

    if (out != stdout) fclose(out);
}
void ktra_mt(int weight[MAX_N][MAX_N], int n, char nameIndex[MAX_N]) {

    if (n < 3) {
        printf("Khong co loi giai do khong du dinh de tao chu trinh (n >= 3)\n");
        exit(1);
    }

    int i, j;

    for (i = 0; i < n; i++) {

        int outDeg = 0;
        int inDeg = 0;

        for (j = 0; j < n; j++) {

            if (weight[i][j] != -1)
                outDeg++;

            if (weight[j][i] != -1)
                inDeg++;
        }

        if (outDeg <2) {
            printf("Dinh %c khong co canh di ra\n", nameIndex[i]);
            exit(1);
        }

        if (inDeg <2) {
            printf("Dinh %c khong co canh di vao\n", nameIndex[i]);
            exit(1);
        }
    }
}

int show_buoc_qhd = 0;
int dem_buoc_qhd = 0;
int max_buoc_qhd = 0;
void print_dp_table(int **dp, int fullMask, int n, const char *title) {
    if (!show_buoc_qhd || dem_buoc_qhd > max_buoc_qhd) return;
    printf("%s\n", title);
    printf("    ");
    for (int j = 0; j < n; j++) printf("%5d ", j);
    printf("\n");
    for (int mask = 0; mask <= fullMask; mask++) {
        printf("%3d:", mask);
        for (int j = 0; j < n; j++) {
            if (dp[mask][j] == INT_MIN) printf(" -INF ");
            else printf("%5d ", dp[mask][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void giai_qhd(int weight[MAX_N][MAX_N], int n, char nameIndex[MAX_N], char goc, int *best, struct Nut *result, int choice) {
    show_buoc_qhd = choice;
    dem_buoc_qhd = 0;
    max_buoc_qhd = 0;
    
    int fullMask = 1 << (n - 1), mask, j, k;
    int **dp = (int **)malloc(fullMask * sizeof(int *));
    int **parent = (int **)malloc(fullMask * sizeof(int *));
    if (!dp || !parent) {
        printf("Khong du bo nho de xu li\n");
        exit(1);
    }

    for (mask = 0; mask < fullMask; mask++) {
        dp[mask] = (int *)malloc(n * sizeof(int));
        parent[mask] = (int *)malloc(n * sizeof(int));
        if (!dp[mask] || !parent[mask]) {
            printf("Khong du bo nho de xu li\n");
            exit(1);
        }
        for (j = 0; j < n; j++) {
            dp[mask][j] = INT_MIN;
            parent[mask][j] = -1;
        }
    }

    for (j = 1; j < n; j++) {
        if (weight[0][j] > 0) {
            dp[1 << (j - 1)][j] = weight[0][j];
            parent[1 << (j - 1)][j] = 0;
        }
    }
    if (show_buoc_qhd) {
        printf("\n" YELLOW "=== PHUONG PHAP QHD + BITMASK ===" RESET "\n");
        printf("Nhap so buoc toi da muon hien thi (0 = hien thi tat ca): ");
        scanf("%d", &max_buoc_qhd);
        if (max_buoc_qhd == 0) max_buoc_qhd = INF;
        
        printf("\nBuoc khoi tao: Dat dp[1<<(j-1)][j] = weight[0][j] cho cac dinh j tu 1 den %d\n", n - 1);
        printf("Cac canh khoi tao:\n");
        for (j = 1; j < n; j++) {
            if (weight[0][j] > 0) {
                printf("  Canh tu dinh 0 (%c) den dinh %d (%c): %d\n", nameIndex[0], j, nameIndex[j], weight[0][j]);
            }
        }
        print_dp_table(dp, fullMask - 1, n, "Bang dp sau khoi tao:");
        dem_buoc_qhd++;
    }
    
    int countLoop = 0;
    if (show_buoc_qhd) {
        printf("Tong so mask can tinh: %d (tu 1 den %d)\n", fullMask - 1, fullMask - 1);
        printf("Nhap so mask ban muon hien thi (0 = hien thi tat ca): ");
        scanf("%d", &countLoop);
        if (countLoop > fullMask - 1) countLoop = fullMask - 1;
        if (countLoop == 0) countLoop = fullMask - 1;
    } else {
        countLoop = fullMask;
    }
    
    for (mask = 1; mask < fullMask; mask++) {
        int nextMask, cand;
        
        if (show_buoc_qhd && dem_buoc_qhd <= max_buoc_qhd && mask <= countLoop) {
            printf("\nBuoc %d: Tinh toan cho mask = %d (", dem_buoc_qhd, mask);
            for (int b = n - 2; b >= 0; b--) {
                if (mask & (1 << b)) printf("1");
                else printf("0");
            }
            printf(")\n");
            dem_buoc_qhd++;
        }
        
        for (j = 1; j < n; j++) {
            if (!(mask & (1 << (j - 1))) || dp[mask][j] == INT_MIN) continue;
            
            if (show_buoc_qhd && dem_buoc_qhd <= max_buoc_qhd && mask <= countLoop) {
                printf("  Xet dinh %d (%c) voi dp[%d][%d] = %d\n", j, nameIndex[j], mask, j, dp[mask][j]);
            }
            
            for (k = 1; k < n; k++) {
                if (mask & (1 << (k - 1)) || weight[j][k] <= 0) continue;
                
                nextMask = mask | (1 << (k - 1));
                cand = dp[mask][j] + weight[j][k];
                
                if (show_buoc_qhd && dem_buoc_qhd <= max_buoc_qhd && mask <= countLoop) {
                    printf("    -> Dinh %d (%c): dp[%d][%d] + canh(%c->%c) = %d + %d = %d",
                        k, nameIndex[k], mask, j, nameIndex[j], nameIndex[k], dp[mask][j], weight[j][k], cand);
                }
                
                if (cand > dp[nextMask][k]) {
                    dp[nextMask][k] = cand;
                    parent[nextMask][k] = j;

                    if (show_buoc_qhd && dem_buoc_qhd <= max_buoc_qhd && mask <= countLoop) {
                        printf(" [CAP NHAT]\n");
                        printf("       dp[%d][%c] = %d\n", nextMask, nameIndex[k], cand);
                        printf("       parent[%d][%c] = %c\n", nextMask, nameIndex[k], nameIndex[j]);
                    }
                }
                else {
                    if (show_buoc_qhd && dem_buoc_qhd <= max_buoc_qhd && mask <= countLoop) {
                        printf(" [KHONG CAP NHAT]\n");
                    }
                }
            }
        }
        
        if (show_buoc_qhd && dem_buoc_qhd <= max_buoc_qhd && mask <= countLoop) {
            printf("  Ket thuc mask %d\n", mask);
        }
    }
    *best = -1;
    int last = -1, total;
    fullMask--;
    if (show_buoc_qhd) {
        printf("\n" YELLOW "=== TIM DUONG DI TOI UU ===" RESET "\n");
        printf("Xet toan bo cac duong di ket thuc tai dinh j va quay ve dinh 0:\n");
    }
    
    for (j = 1; j < n; j++) {
        if (dp[fullMask][j] == INT_MIN || weight[j][0] <= 0) {
            if (show_buoc_qhd) {
                printf("Dinh %d (%c): Khong the quay ve diem xuat phat (dp[%d][%d] = %d)\n",
                    j, nameIndex[j], fullMask, j, dp[fullMask][j]);
            }
            continue;
        }
        
        total = dp[fullMask][j] + weight[j][0];
        
        if (show_buoc_qhd) {
            printf("Dinh %d (%c): dp[%d][%d] + canh(%c->%c) = %d + %d = %d",
                j, nameIndex[j], fullMask, j, nameIndex[j], nameIndex[0], dp[fullMask][j], weight[j][0], total);
        }
        
        if (total > *best) {
            *best = total;
            last = j;
            if (show_buoc_qhd) printf(" [TOI UU]\n");
        } else {
            if (show_buoc_qhd) printf("\n");
        }
    }

    if (show_buoc_qhd) {
        printf("\nKet qua: best = %d, last = %d (%c)\n", *best, last, last >= 0 ? nameIndex[last] : '?');
    }

    if (*best == -1) {
        for (mask = 0; mask <= fullMask; mask++) {
            free(dp[mask]);
            free(parent[mask]);
        }
        free(dp);
        free(parent);
        return;
    }
    mask = fullMask;
    int path[MAX_N];
    int cnt = 0;
    int cur = last;
    int temp;

    if (show_buoc_qhd) {
        printf("\n=== TRUY VET KET QUA ===\n");
        printf("Bat dau tu dinh %d (%c), mask = %d\n", last, nameIndex[last], fullMask);
    }

    while (cur != 0 && cur != -1) {
        if (show_buoc_qhd) {
            printf("Dinh %d (%c) <- parent[%d][%d] = %d (%c)\n",
                cur, nameIndex[cur], mask, cur, parent[mask][cur], nameIndex[parent[mask][cur]]);
        }

        path[cnt++] = cur;
        temp = parent[mask][cur];
        mask ^= (1 << (cur - 1));
        cur = temp;
    }
    result->tendinh = nameIndex[0];

    struct Nut *current = result;

    for (int i = cnt - 1; i >= 0; i--) {
        current->next = (struct Nut *)malloc(sizeof(struct Nut));
        current = current->next;
        current->tendinh = nameIndex[path[i]];
    }

    current->next = result;

    if (show_buoc_qhd) {
        printf("\nTruy vet hoan tat!\n");
    }
    for (mask = 0; mask <= fullMask; mask++) {
        free(dp[mask]);
        free(parent[mask]);
    }
    free(dp);
    free(parent);
    show_buoc_qhd = 0;
    return;
}

int chi_phi_min = INF;
int best_edges[MAX_N][2];
int current_edges[MAX_N][2]; 
int show_buoc_nc = 0;  
int dem_buoc_nc = 0;     
int max_buoc_nc = 0; 
char nameIndexGlobal[MAX_N];

void in_mt_nc(int matrix[MAX_N][MAX_N], int size, int row_lbls[], int col_lbls[], const char *title) {
    if (!show_buoc_nc || dem_buoc_nc > max_buoc_nc) return;
    if (title && title[0] != '\0') printf("%s\n", title);
    
    printf("      ");
    for (int j = 0; j < size; j++) printf("%5c ", nameIndexGlobal[col_lbls[j]]);
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("%5c ", nameIndexGlobal[row_lbls[i]]); 
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] == INF) printf("  INF ");
            else printf("%5d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int rut_gon_mt(int matrix[MAX_N][MAX_N], int size) {
    int reduction_sum = 0, i, j, min_val;
    for (i = 0; i < size; i++) {
        min_val = INF;
        for (j = 0; j < size; j++)
            if (matrix[i][j] < min_val) min_val = matrix[i][j];
        if (min_val != INF && min_val != 0) {
            for (j = 0; j < size; j++)
                if (matrix[i][j] != INF) matrix[i][j] -= min_val;
            if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc)
                printf("    Giam hang %d (tru %d)\n", i, min_val);
            reduction_sum += min_val;
        }
    }
    for (j = 0; j < size; j++) {
        min_val = INF;
        for (i = 0; i < size; i++)
            if (matrix[i][j] < min_val) min_val = matrix[i][j];
        if (min_val != INF && min_val != 0) {
            for (i = 0; i < size; i++)
                if (matrix[i][j] != INF) matrix[i][j] -= min_val;
            if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc)
                printf("    Giam cot %d (tru %d)\n", j, min_val);
            reduction_sum += min_val;
        }
    }
    return reduction_sum;
}

void tim_canh_tot(int matrix[MAX_N][MAX_N], int size, int *r, int *s, int row_lbls[], int col_lbls[]) {
    int max_regret = -1, i, j, min_row, min_col, regret;
    *r = -1; *s = -1;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (matrix[i][j] == 0) {
                min_row = INF, min_col = INF;
                for (int k = 0; k < size; k++) {
                    if (k != j && matrix[i][k] < min_row) min_row = matrix[i][k];
                    if (k != i && matrix[k][j] < min_col) min_col = matrix[k][j];
                }
                regret = (min_row == INF ? 0 : min_row) + (min_col == INF ? 0 : min_col);
                if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
                    printf("    Zero (%c -> %c): regret = %d\n", nameIndexGlobal[row_lbls[i]], nameIndexGlobal[col_lbls[j]], regret);         
                }
                if (regret > max_regret) {
                    max_regret = regret;
                    *r = i; *s = j;
                }
            }
        }
    }
    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc && *r != -1)
        printf("  Canh tot nhat: (%c -> %c) voi regret = %d\n", nameIndexGlobal[row_lbls[*r]], nameIndexGlobal[col_lbls[*s]], max_regret);
}

void print_current_edges_log(int edge_count) {
    if (!show_buoc_nc || dem_buoc_nc > max_buoc_nc) return;
    printf("Tap canh hien tai: ");
    for (int t = 0; t < edge_count; t++) {
        printf("(%c -> %c) ", nameIndexGlobal[current_edges[t][0]], nameIndexGlobal[current_edges[t][1]]);
    }
    printf("\n");
}

void print_current_edges_array_log(int edge_count) {
    if (!show_buoc_nc || dem_buoc_nc > max_buoc_nc) return;
    printf("current_edges:\n");
    for (int t = 0; t < edge_count; t++) {
        int u = current_edges[t][0];
        int v = current_edges[t][1];
        printf("  current_edges[%d][0] = %d, [1] = %d  => (%c -> %c)\n", t, u, v, nameIndexGlobal[u], nameIndexGlobal[v]);
    }
}

void tim_dau_cuoi_ct(int edge_count, int start_u, int start_v, int *head_out, int *tail_out) {
    int head = start_u;
    int tail = start_v;
    int found;
    do {
        found = 0;
        for (int i = 0; i < edge_count; i++) {
            if (current_edges[i][1] == head) {
                head = current_edges[i][0];
                found = 1;
                break;
            }
        }
    } while (found);
    do {
        found = 0;
        for (int i = 0; i < edge_count; i++) {
            if (current_edges[i][0] == tail) {
                tail = current_edges[i][1];
                found = 1;
                break;
            }
        }
    } while (found);

    *head_out = head; 
    *tail_out = tail; 
}

void branch_and_bound(int matrix[MAX_N][MAX_N], int size, int bound, int edge_count, int n_total, int row_lbls[], int col_lbls[]) {
    if (bound >= chi_phi_min) {
        if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc)
            printf("  Cat nhanh: bound (%d) >= chi_phi_min (%d)\n", bound, chi_phi_min);
        return;
    }
    
    if (size == 2) {
        int p = row_lbls[0], q = row_lbls[1];
        int u = col_lbls[0], v = col_lbls[1];
        
        int match_case = 0;
        int e1_r = -1, e1_s = -1, e2_r = -1, e2_s = -1;

        if (matrix[0][0] == 0 && matrix[1][1] == 0) {
            match_case = 1;
            e1_r = p; e1_s = u;
            e2_r = q; e2_s = v;
        }
        else if (matrix[0][1] == 0 && matrix[1][0] == 0) {
            match_case = 2;
            e1_r = p; e1_s = v;
            e2_r = q; e2_s = u;
        }

        if (match_case > 0) {
            if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
                printf("\nMa tran ve bac 2. Ap dung quy tac chon nhanh 2 canh cuoi (Truong hop (i)):\n");
                printf("  Chon canh: (%c -> %c) va (%c -> %c)\n", nameIndexGlobal[e1_r], nameIndexGlobal[e1_s], nameIndexGlobal[e2_r], nameIndexGlobal[e2_s]);
            }

            current_edges[edge_count][0] = e1_r;
            current_edges[edge_count][1] = e1_s;
            current_edges[edge_count + 1][0] = e2_r;
            current_edges[edge_count + 1][1] = e2_s;

            print_current_edges_array_log(edge_count + 2);

            if (bound < chi_phi_min) {
                chi_phi_min = bound;
                if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
                    printf("Tim thay giai phap: chi_phi_min = %d\n", chi_phi_min);
                }
                for (int i = 0; i < n_total; i++) {
                    best_edges[i][0] = current_edges[i][0];
                    best_edges[i][1] = current_edges[i][1];
                }
            }
            return;
        }
    }

    if (edge_count == n_total) {
        if (bound < chi_phi_min) {
            chi_phi_min = bound;
            if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
                printf("Tim thay giai phap: chi_phi_min = %d\n", chi_phi_min);
            }
            for (int i = 0; i < n_total; i++) {
                best_edges[i][0] = current_edges[i][0];
                best_edges[i][1] = current_edges[i][1];
            }
        }
        return;
    }

    int r, s; 
    tim_canh_tot(matrix, size, &r, &s, row_lbls, col_lbls);
    if (r == -1) return;

    int global_r = row_lbls[r];
    int global_s = col_lbls[s];
 
    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
        printf("\nBuoc %d: CHON canh (%c -> %c)\n", dem_buoc_nc + 1, nameIndexGlobal[global_r], nameIndexGlobal[global_s]);
        dem_buoc_nc++;
    }

    current_edges[edge_count][0] = global_r;
    current_edges[edge_count][1] = global_s;

    print_current_edges_log(edge_count + 1);

    int head_node, tail_node;
    tim_dau_cuoi_ct(edge_count + 1, global_r, global_s, &head_node, &tail_node);

    int next_matrix[MAX_N][MAX_N];
    int next_row_lbls[MAX_N], next_col_lbls[MAX_N];
    int next_size = 0;

    int print_lock_msg = 0;
    int lock_i = r; 
    int lock_j = s; 

    for (int i = 0; i < size; i++) {
        if (i == r) continue;
        int next_col_cnt = 0;
        for (int j = 0; j < size; j++) {
            if (j == s) continue;

            if (row_lbls[i] == tail_node && col_lbls[j] == head_node) {
                next_matrix[next_size][next_col_cnt] = INF;
                print_lock_msg = 1;
            } else {
                next_matrix[next_size][next_col_cnt] = matrix[i][j];
            }
            next_col_cnt++;
        }
        next_row_lbls[next_size] = row_lbls[i];
        next_size++;
    }

    int col_cnt = 0;
    for (int j = 0; j < size; j++) {
        if (j == s) continue;
        next_col_lbls[col_cnt++] = col_lbls[j];
    }

    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
        printf("Khoa hang %d, cot %d, va vi tri (%c -> %c):\n", lock_i, lock_j, nameIndexGlobal[tail_node], nameIndexGlobal[head_node]);
    }

    print_current_edges_array_log(edge_count + 1);
    printf("\n");

    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
        printf("  Ma tran sau khi giam cap:\n");
        in_mt_nc(next_matrix, next_size, next_row_lbls, next_col_lbls, "");
    }

    int res_reduction = rut_gon_mt(next_matrix, next_size);

    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
        printf("  Ma tran sau khi rut gon:\n");
        in_mt_nc(next_matrix, next_size, next_row_lbls, next_col_lbls, "");
        printf("  Tong giam: %d, bound moi: %d\n", res_reduction, bound + res_reduction);
    }

    branch_and_bound(next_matrix, next_size, bound + res_reduction, edge_count + 1, n_total, next_row_lbls, next_col_lbls);

    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
        printf("\nBuoc %d: KHONG CHON canh (%c -> %c)\n", dem_buoc_nc + 1, nameIndexGlobal[global_r], nameIndexGlobal[global_s]);
        dem_buoc_nc++;
    }

    int no_edge_matrix[MAX_N][MAX_N];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) no_edge_matrix[i][j] = matrix[i][j];
    }
    no_edge_matrix[r][s] = INF; 

    print_current_edges_log(edge_count);
    print_current_edges_array_log(edge_count);

    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
        printf("  Khoa canh (%c -> %c):\n", nameIndexGlobal[global_r], nameIndexGlobal[global_s]);
        in_mt_nc(no_edge_matrix, size, row_lbls, col_lbls, "  Ma tran sau khi khong chon canh:");
    }

    res_reduction = rut_gon_mt(no_edge_matrix, size);
    
    if (show_buoc_nc && dem_buoc_nc <= max_buoc_nc) {
        printf("  Ma tran sau khi rut gon:\n");
        in_mt_nc(no_edge_matrix, size, row_lbls, col_lbls, "");
        printf("  Tong giam: %d, bound moi: %d\n", res_reduction, bound + res_reduction);
    }

    branch_and_bound(no_edge_matrix, size, bound + res_reduction, edge_count, n_total, row_lbls, col_lbls);
}

int tao_mt_xuli(int matrix[MAX_N][MAX_N], int n){
    int M=-1,i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if(M<matrix[i][j])M=matrix[i][j];
            if (matrix[i][j] == -1 || i == j) matrix[i][j] = INF;
        }
    }
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(matrix[i][j]!=INF)matrix[i][j]=M-matrix[i][j];
        }
    }
    return M;
}
void giai_nhanh_can(int weight[MAX_N][MAX_N], int n, char nameIndex[MAX_N], char goc, int *best, struct Nut* res, int choice){
    int i, j;
    
    show_buoc_nc = choice;
    dem_buoc_nc = 0;
    max_buoc_nc = 0;
    
    for (i = 0; i < n; i++) {
        nameIndexGlobal[i] = nameIndex[i];
    }
    
    if (show_buoc_nc) {
        printf("\n" YELLOW "=== PHUONG PHAP NHANH CANH (BRANCH & BOUND) ===" RESET "\n");
        printf("Nhap so buoc toi da muon hien thi (0 = hien thi tat ca): ");
        scanf("%d", &max_buoc_nc);
        if(max_buoc_nc == 0) max_buoc_nc = INF;
        printf("\nKhoi tao ma tran chi phi (duoi dang cuc dai):\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (weight[i][j] == -1) printf("  INF ");
                else printf("%5d ", weight[i][j]);
            }
            printf("\n");
        }
    }
    int matrix[MAX_N][MAX_N];
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++) matrix[i][j] = weight[i][j];
    }
    int M = tao_mt_xuli(matrix, n);
    if (show_buoc_nc) {
        printf("\nSau khi chuan hoa (dao bai toan - M tru toan bo chi phi voi M la maximum chi phi): %d\n", M);
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (matrix[i][j] == INF) printf("  INF ");
                else printf("%5d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\nTien hanh giam hang va cot:\n");
    }

    int row_lbls[MAX_N], col_lbls[MAX_N];
    for (i = 0; i < n; i++) {
        row_lbls[i] = i;
        col_lbls[i] = i;
    }

    int initial_bound = rut_gon_mt(matrix, n);
    if (show_buoc_nc) {
        printf("\nMa tran sau khi rut gon ban dau:\n");
        in_mt_nc(matrix, n, row_lbls, col_lbls, "");
        printf("Bound ban dau: %d\n\n", initial_bound);
    }

    chi_phi_min = INF;
    dem_buoc_nc = 0;
    
    branch_and_bound(matrix, n, initial_bound, 0, n, row_lbls, col_lbls);
    
    *best = chi_phi_min >= INF ? -1 : M * n - chi_phi_min;
    if (show_buoc_nc) {
        printf("best = M * n - chi_phi_min\n");
        printf("     = %d * %d - %d\n", M, n, chi_phi_min);
        printf("     = %d\n", *best);
        printf("\n" YELLOW "=== KET QUA CUOI CUNG ===" RESET "\n");
        printf("Chi phi toi uu: %d\n", *best);
    }

    int cur = 0;
    struct Nut* cur_way = res;
    if (show_buoc_nc) {
        printf("\n========== TRUY VET CHU TRINH ==========\n");
        printf("Bat dau tu dinh: %d (%c)\n", cur, nameIndex[cur]);
        printf("Su dung best_edges:\n");
        for (int t = 0; t < n; t++) {
            printf("best_edges[%d] = (%d -> %d) (%c -> %c)\n",
                t, best_edges[t][0], best_edges[t][1], nameIndex[best_edges[t][0]], nameIndex[best_edges[t][1]]);
        }
        printf("\n");
    }
    n--;
    for (i = 0; i < n; i++) {
        if (show_buoc_nc) {
            printf("\nDang tai dinh %d (%c)\n", cur, nameIndex[cur]);
            printf("Duyet best_edges de tim canh bat dau tu %d\n", cur);
        }
        cur_way->tendinh = nameIndex[cur];
        for (j = 0; j <= n; j++) {
            if (show_buoc_nc) {
                printf("  Xet best_edges[%d] = (%d -> %d)\n", j, best_edges[j][0], best_edges[j][1]);
            }
            if (best_edges[j][0] == cur) {
                if (show_buoc_nc) {
                    printf("  --> MATCH: (%d -> %d)\n", best_edges[j][0], best_edges[j][1]);
                }
                cur = best_edges[j][1];
                if (show_buoc_nc) {
                    printf("  Di chuyen sang %d (%c)\n", cur, nameIndex[cur]);
                }
                break;
            }
        }
        cur_way->next = (struct Nut*)malloc(sizeof(struct Nut));
        cur_way = cur_way->next;
    }
    cur_way->tendinh = nameIndex[cur];
    cur_way->next = res;
    n++;

    show_buoc_nc = 0;
    return;
}

void printFileContent(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Khong mo duoc file %s\n", filename);
        return;
    }
    char ch;
    printf("\n===== NOI DUNG FILE %s =====\n", filename);
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    printf("\n============================\n");
    fclose(file);
}

int main(void) {
    in_menu();
    int weight[MAX_N][MAX_N], indexMap[MAX_CHAR], n;
    char nameIndex[MAX_N], goc;
    struct Nut *result = (struct Nut*)malloc(sizeof(struct Nut));
    result->next = NULL;
    int best;
    int choice;
    FILE *TypeRead;
    xoa_datafile(tenFileout);
    printf("Ban muon nhap tu ban phim (0) hay nhap tu file (!=0): ");
    scanf("%d", &choice);
    if(choice == 0) {
        TypeRead = stdin;
        printf("Du lieu se duoc doc tu ban phim (Voi window thi lap lai 2 lan (nhan ctrl + z -> enter) de thoat loop input [thay ctrl+z thanh ctrl + d voi linux(mac)]) \n");
    }
    else {
        TypeRead = fopen(tenFilein, "r");
        if (TypeRead == NULL) {
            printf("Khong mo duoc file data.txt\n");
            return 1;
        }
        printf("Du lieu se duoc doc tu file 'data.txt' \n");
        printFileContent(tenFilein);
    }
    doc_input(TypeRead, weight, indexMap, nameIndex, &n, &goc);
    printf("Du lieu da duoc doc va kiem tra thanh cong. So dinh: " GREEN "%d" RESET " \n", n);
    in_mt(weight, n);
    ktra_mt(weight, n, nameIndex);

    LARGE_INTEGER tic, toc, freq;
    
    do {   
        printf("\n============= MENU =============\n");
        printf("1. QHD + Bitmask (Uu tien : N<=21)\n");
        printf("2. Nhanh canh (Branch & Bound) (Uu tien : N<=28)\n");
        printf("0. Thoat\n");
        printf("================================\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);
        QueryPerformanceFrequency(&freq);

        switch(choice) {
            case 1: {
                printf("\nBan co muon hien thi cac buoc giai hay khong (y[!=0]/n[0]) : ");
                scanf("%d", &choice);
                QueryPerformanceCounter(&tic);
                giai_qhd(weight, n, nameIndex, goc, &best, result, choice);
                QueryPerformanceCounter(&toc);
                ghi_ketqua(tenFileout, result, best, goc, "QDH+BitMask");
                printf("Thoi gian chinh xac: %.6f ms (mili giay)\n", (double)(toc.QuadPart - tic.QuadPart) * 1000.0 / freq.QuadPart);
                break;
            }
            case 2: {
                printf("\nBan co muon hien thi cac buoc giai hay khong (y[!=0]/n[0]) : ");
                scanf("%d", &choice);
                QueryPerformanceCounter(&tic);
                giai_nhanh_can(weight, n, nameIndex, goc, &best, result, choice);
                QueryPerformanceCounter(&toc);
                ghi_ketqua(tenFileout, result, best, goc, "Nhanh Canh");
                printf("Thoi gian chinh xac: %.6f ms (mili giay)\n", (double)(toc.QuadPart - tic.QuadPart) * 1000.0 / freq.QuadPart);
                break;
            }
            case 0:
                printf("\nThoat chuong trinh.\n");
                break;
            default:
                printf("\nLua chon khong hop le!\n");
        }
        printf("\nBan co muon thuc hien them phuong phap gia khac khong (y[!=0]/n[0]): ");
        scanf("%d", &choice);
        if(!choice){
            printf("\nThoat chuong trinh.\n");
            break;
        }
    } while(choice);
    return 0;
}
