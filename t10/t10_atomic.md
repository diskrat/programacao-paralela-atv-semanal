# Tarefa 10: Operações atômicas

Implemente novamente o estimador da tarefa 8 usando um contador compartilhado e o `rand_r` protegendo  a soma de _hits_ com `#pragma omp critical` e com `#pragma omp atomic`. Compare essas duas implementações com suas versões que usam contadores privados. Agora, compare esssas com uma 5ª versão que utiliza apenas a cláusula `reduction` ao invés das diretivas de sincronização. Reflita sobre a aplicavbilidade de desses mecanismos em termos de desempenho e produtividade e proponha um roteiro para quando utilizar qual mecanismo de sincronização, incluindo `critical`  nomeaadas e _locks_ explícitos.

