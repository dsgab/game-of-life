import subprocess
import itertools
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def run_program(command):
    """
    Executa o programa e retorna o tempo de execução.
    """
    try:
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, check=True)
        return map(float,result.stdout.split())  # Retorna a saída do programa como float
    except subprocess.CalledProcessError as e:
        print(f"Erro ao executar o comando: {command}")
        print(e.stderr)
        return None

c_program = "./cbin"
go_program = "./gobin"

# Definindo as variáveis de entrada
tamTabuleiro = [32, 128, 512, 1024]
numIteracoes = [512, 1024]
numThreads = [1, 4, 8, 16]
numRepeticoes = 5

# Gerando as combinações de entrada
inputs = itertools.product(tamTabuleiro, numIteracoes, numThreads)

# Inicializando listas para armazenar os resultados
results_c = []

# Executar os testes para C
for tam, iteracoes, threads in inputs:
    command = [c_program, str(tam), str(iteracoes), str(threads)]

    times = []

    # Rodando cada configuração `numRepeticoes` vezes
    for i in range(numRepeticoes):
        print(f"executando: {command} : iteracao {i}")
        time = run_program(command)[0]
        times.append(time)
    # Calculando a média dos tempos
    avg_time = np.mean(times)

    # Salvando os resultados
    results_c.append({
        "Executavel": "C",
        "Tam_Tabuleiro": tam,
        "Num_Iteracoes": iteracoes,
        "Num_Threads": threads,
        "Tempo_": avg_time
    })

# Criando o DataFrame para tabelas
df_c = pd.DataFrame(results_c)


print(df_c,results_c)
