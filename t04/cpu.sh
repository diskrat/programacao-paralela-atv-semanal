#!/usr/bin/env bash

gcc -Wall t04_2_multitarefas_cpu.c -fopenmp -lm -o t04_2_multitarefas_cpu.out
run_cpu_benchmark() {
	local threads="$1"
	echo "--------------- ${threads} thread(s) --------------------w"
	sudo env OMP_NUM_THREADS="${threads}" ./t04_2_multitarefas_cpu.out
}

run_cpu_benchmark 1
run_cpu_benchmark 3
run_cpu_benchmark 6
run_cpu_benchmark 12
run_cpu_benchmark 24
