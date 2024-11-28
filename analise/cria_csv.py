import subprocess
import itertools
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

#coloquei os binarios nesses arquivos
c_program = "./cbin"
go_program = "./gobin"

# Definindo as variáveis de entrada
tamTabuleiro = [32, 128, 512, 1024]
numIteracoes = [256, 512, 1024]
numThreads = [1, 2, 4, 8, 16]
numRepeticoes = 5

# Gerando as combinações de entrada
# Inicializando listas para armazenar os resultados
results_c = []

# Executar os testes para C
for tam, iteracoes, threads in itertools.product(tamTabuleiro, numIteracoes, numThreads):
    command = [c_program, str(tam), str(iteracoes), str(threads)]

    times = []

    # Rodando cada configuração `numRepeticoes` vezes
    for i in range(numRepeticoes):
        print(f"executando: {command} : iteracao {i}")
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, check=True)
        time = float(result.stdout.strip())
        times.append(time)

    # Calculando a média dos tempos
    avg_time = np.mean(times)

    # Salvando os resultados
    results_c.append({
        "Executavel": "C",
        "Tam_Tabuleiro": tam,
        "Num_Iteracoes": iteracoes,
        "Num_Threads": threads,
        "Tempo": avg_time
    })

# Criando o DataFrame para tabelas
df_c = pd.DataFrame(results_c)

df_c.to_csv("c_values.csv")

print(df_c)
print(results_c)

# Executar os testes para Go
results_go = []
for tam, iteracoes, threads in itertools.product(tamTabuleiro, numIteracoes, numThreads):
    command = [go_program, str(tam), str(iteracoes), str(threads)]

    times_conc = []
    times_pool = []
    # Rodando cada configuração `numRepeticoes` vezes
    for i in range(numRepeticoes):
        print(f"executando: {command} : iteracao {i}")
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, check=True)
        time_conc, time_pool = map(float,result.stdout.split())
        times_conc.append(time_conc)
        times_pool.append(time_pool)

    # Calculando a média dos tempos
    avg_conc_time = np.mean(times_conc)
    avg_pool_time = np.mean(times_pool)


    # Salvando os resultados
    results_go.append({
        "Executavel": "Go",
        "Tam_Tabuleiro": tam,
        "Num_Iteracoes": iteracoes,
        "Num_Threads": threads,
        "Tempo_Conc": avg_conc_time,
        "Tempo_Pool": avg_pool_time,
    })

df_go = pd.DataFrame(results_go)
df_go.to_csv("go_values.csv")
print(df_go)

