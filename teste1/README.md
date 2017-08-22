# Relatório

Através do teste 1 pudemos observer o efeito que o paralalismo tem nessa aplicação e as variações de performance dos diferentes métodos de paralelismo aplicados.

Executando o projeto numa máquina com um Intel Core i7 4770K, com 8 núcleos, obtivemos um tempo médio de 2.987s para a versão sequencial, quando N = 64. No gráfico abaixo, temos os tempos médios para as versões com paralelismo para diferentes k.

![Grafico](https://raw.githubusercontent.com/rmobis/MC714/master/teste1/grafico.png)

Pode-se observar que há um ganho de performance significativo para ambos os métodos, mas o ganho é muito maior quando usamos threads. Isso se dá ao custo de criação dos processos, cópia de memória, etc.
