#!/usr/bin/env bash
gcc -Wall t04_1_multitarefas_mem.c -fopenmp -o t04_1_multitarefas_mem.out
run_memory_benchmark() {
	local threads="$1"
	echo "--------------- ${threads} thread(s) --------------------"
	sudo env OMP_NUM_THREADS="${threads}" ./t04_1_multitarefas_mem.out
}

run_memory_benchmark 1
run_memory_benchmark 3
run_memory_benchmark 6
run_memory_benchmark 12
run_memory_benchmark 24