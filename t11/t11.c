#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 16
#define NX SIZE           // Resolução no eixo X
#define NY SIZE           // Resolução no eixo Y
#define NZ SIZE           // Resolução no eixo Z
#define STEPS 2500       // Passos no tempo
#define TOTAL_SIZE (NX * NY * NZ)

// Função para guardar o frame atual no ficheiro CSV
void salvar_frame_csv(FILE* f, int frame_id, double* u, double* v, double* w, int nx, int ny, int nz) {
    // Limiar: Apenas guarda pontos com velocidade considerável
    // Isto evita um CSV gigante cheio de zeros

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                int idx = (i * ny + j) * nz + k;
                
                // Calcula a magnitude da velocidade (tamanho do vetor)
                double mag = sqrt(u[idx]*u[idx] + v[idx]*v[idx] + w[idx]*w[idx]);
                if (mag > 0.01) fprintf(f, "%d,%d,%d,%d,%lf\n", frame_id, i, j, k, mag);
            }
        }
    }
}

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

    // 1. Inicialização e Perturbação num cubo central
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < NY; j++) {
            for (int k = 0; k < NZ; k++) {
                int idx = (i * NY + j) * NZ + k;
                u[idx] = 0.0; v[idx] = 0.0; w[idx] = 0.0;

                // Raio da perturbação (2 unidades para um grid de 16)
                int raio = 2;
                
                // Perturbação volumétrica restrita ESTRITAMENTE ao centro
                if (i >= NX/2 - raio && i <= NX/2 + raio &&
                    j >= NY/2 - raio && j <= NY/2 + raio &&
                    k >= NZ/2 - raio && k <= NZ/2 + raio) {
                    
                    // Intensidade inicial
                    u[idx] = 10.0;
                    v[idx] = 10.0;
                    w[idx] = 10.0;
                }
                u_new[idx] = u[idx]; v_new[idx] = v[idx]; w_new[idx] = w[idx];
            }
        }
    }

    clock_t start_time = clock();

    // Controlo para guardar quadros da animação
    int salvar_a_cada = 10; // Tira uma "foto" a cada 10 passos
    int frame_id = 0;

    // Abre o ficheiro CSV uma única vez antes do loop temporal
    FILE* arquivo_csv = fopen("animacao_completa.csv", "w");
    if (arquivo_csv) {
        fprintf(arquivo_csv, "frame,x,y,z,magnitude\n"); // Cabeçalho do CSV
    } else {
        // Liberta memória antes de sair em caso de erro
        free(u); free(v); free(w);
        free(u_new); free(v_new); free(w_new);
        return 1;
    }

    // 2. Loop de Evolução Temporal
    for (int t = 0; t < STEPS; t++) {
        
        for (int i = 1; i < NX - 1; i++) {
            for (int j = 1; j < NY - 1; j++) {
                for (int k = 1; k < NZ - 1; k++) {
                    int idx = (i * NY + j) * NZ + k;

                    // Stencil de Diferenças Finitas em 3D
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

        // 3. Condições de Contorno (Faces nas extremidades)
        for (int j = 0; j < NY; j++) {
            for (int k = 0; k < NZ; k++) {
                int idx_min = (0 * NY + j) * NZ + k;
                int idx_max = ((NX - 1) * NY + j) * NZ + k;
                u_new[idx_min] = v_new[idx_min] = w_new[idx_min] = 0.0;
                u_new[idx_max] = v_new[idx_max] = w_new[idx_max] = 0.0;
            }
        }

        // Troca de ponteiros para preparar o próximo passo
        double *tmp_u = u; u = u_new; u_new = tmp_u;
        double *tmp_v = v; v = v_new; v_new = tmp_v;
        double *tmp_w = w; w = w_new; w_new = tmp_w;

        // 4. SALVAR FRAME NO TEMPO (No mesmo CSV)
        if (t % salvar_a_cada == 0 && arquivo_csv) {
            salvar_frame_csv(arquivo_csv, frame_id, u, v, w, NX, NY, NZ);
            frame_id++;
        }
    }

    // Fecha o ficheiro CSV no final de toda a simulação
    if (arquivo_csv) {
        fclose(arquivo_csv);
    }

    clock_t end_time = clock();
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Simulação 3D concluída em %.4f segundos.\n", cpu_time_used);

    // Liberação de memória
    free(u); free(v); free(w);
    free(u_new); free(v_new); free(w_new);
    return 0;
}