#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NX 150           // Resolução no eixo X
#define NY 150           // Resolução no eixo Y
#define NZ 150           // Resolução no eixo Z
#define STEPS 200        // Passos no tempo
#define TOTAL_SIZE (NX * NY * NZ)

int main() {
    double dx = 1.0, dy = 1.0, dz = 1.0;
    double nu = 0.05;    // Viscosidade
    double dt = 0.1;     // Passo temporal seguro para estabilidade

    // Alocação das componentes de velocidade U, V e W
    double *u = (double *)malloc(TOTAL_SIZE * sizeof(double));
    double *v = (double *)malloc(TOTAL_SIZE * sizeof(double));
    double *w = (double *)malloc(TOTAL_SIZE * sizeof(double));
    
    double *u_new = (double *)malloc(TOTAL_SIZE * sizeof(double));
    double *v_new = (double *)malloc(TOTAL_SIZE * sizeof(double));
    double *w_new = (double *)malloc(TOTAL_SIZE * sizeof(double));

    if (!u || !v || !w || !u_new || !v_new || !w_new) {
        fprintf(stderr, "Erro ao alocar memória 3D.\n");
        return 1;
    }

    // 1. Inicialização e Perturbação em um cubo central (Execução Serial)
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < NY; j++) {
            for (int k = 0; k < NZ; k++) {
                int idx = (i * NY + j) * NZ + k;
                u[idx] = 0.0; v[idx] = 0.0; w[idx] = 0.0;

                // Perturbação volumétrica no centro do domínio
                if (i > NX/2 - 10 && i < NX/2 + 10 &&
                    j > NY/2 - 10 && j < NY/2 + 10 &&
                    k > NZ/2 - 10 && k < NZ/2 + 10) {
                    u[idx] = 10.0;
                    v[idx] = 10.0;
                    w[idx] = 10.0;
                }
                u_new[idx] = u[idx]; v_new[idx] = v[idx]; w_new[idx] = w[idx];
            }
        }
    }

    // Medição de tempo usando a biblioteca padrão <time.h>
    clock_t start_time = clock();

    // 2. Loop de Evolução Temporal
    for (int t = 0; t < STEPS; t++) {
        
        for (int i = 1; i < NX - 1; i++) {
            for (int j = 1; j < NY - 1; j++) {
                for (int k = 1; k < NZ - 1; k++) {
                    int idx = (i * NY + j) * NZ + k;

                    // Stencil de Diferenças Finitas em 3D (7 pontos)
                    double u_xx = (u[idx + NY*NZ] - 2.0 * u[idx] + u[idx - NY*NZ]) / (dx * dx);
                    double u_yy = (u[idx + NZ]    - 2.0 * u[idx] + u[idx - NZ])    / (dy * dy);
                    double u_zz = (u[idx + 1]     - 2.0 * u[idx] + u[idx - 1])     / (dz * dz);
                    u_new[idx] = u[idx] + nu * dt * (u_xx + u_yy + u_zz);

                    double v_xx = (v[idx + NY*NZ] - 2.0 * v[idx] + v[idx - NY*NZ]) / (dx * dx);
                    double v_yy = (v[idx + NZ]    - 2.0 * v[idx] + v[idx - NZ])    / (dy * dy);
                    double v_zz = (v[idx + 1]     - 2.0 * v[idx] + v[idx - 1])     / (dz * dz);
                    v_new[idx] = v[idx] + nu * dt * (v_xx + v_yy + v_zz);

                    double w_xx = (w[idx + NY*NZ] - 2.0 * w[idx] + w[idx - NY*NZ]) / (dx * dx);
                    double w_yy = (w[idx + NZ]    - 2.0 * w[idx] + w[idx - NZ])    / (dy * dy);
                    double w_zz = (w[idx + 1]     - 2.0 * w[idx] + w[idx - 1])     / (dz * dz);
                    w_new[idx] = w[idx] + nu * dt * (w_xx + w_yy + w_zz);
                }
            }
        }

        // 3. Condições de Contorno (Paredes do cubo com velocidade zero)
        for (int j = 0; j < NY; j++) {
            for (int k = 0; k < NZ; k++) {
                // Faces X min e X max
                int idx_min = (0 * NY + j) * NZ + k;
                int idx_max = ((NX - 1) * NY + j) * NZ + k;
                u_new[idx_min] = v_new[idx_min] = w_new[idx_min] = 0.0;
                u_new[idx_max] = v_new[idx_max] = w_new[idx_max] = 0.0;
            }
        }

        // Troca de ponteiros (Swap rápido)
        double *tmp_u = u; u = u_new; u_new = tmp_u;
        double *tmp_v = v; v = v_new; v_new = tmp_v;
        double *tmp_w = w; w = w_new; w_new = tmp_w;
    }

    clock_t end_time = clock();
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Simulação 3D Serial concluída em %.4f segundos.\n", cpu_time_used);

    int c_idx = ((NX / 2) * NY + (NY / 2)) * NZ + (NZ / 2);
    printf("Velocidade no centro geométrico: U=%.4f, V=%.4f, W=%.4f\n", u[c_idx], v[c_idx], w[c_idx]);

    // Liberação de memória
    free(u); free(v); free(w);
    free(u_new); free(v_new); free(w_new);
    return 0;
}