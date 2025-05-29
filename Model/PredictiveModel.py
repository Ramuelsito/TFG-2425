import pandas as pd
import numpy as np

# Reconstruir 'Times Used' y 'Times Improved' como vectores de 4 posiciones
def parse_vector(colName):
    return [int(x) for x in str(colName).strip().split()]

# Reconstruir 'Times Used Conditioned' y 'Times Improved Conditioned' como matrices 5x4
def parse_matrix(colName):
    # Quita comillas y espacios, separa por ';' y luego por espacio
    rows = str(colName).replace('"', '').strip().split(';')
    matrix = []
    for row in rows:
        if row.strip() == '':
            continue
        matrix.append([int(x) for x in row.strip().split()])
    return np.array(matrix)

def calculate_success_rate_matrix(times_used_conditioned, times_improved_conditioned):
     # Evita división por cero usando np.where
    times_used_conditioned = times_used_conditioned.astype(float)
    times_improved_conditioned = times_improved_conditioned.astype(float)
    rate = np.where(times_used_conditioned != 0, times_improved_conditioned / times_used_conditioned, 0)
    return rate

def greedy_best_order(rate_matrix):
    n_moves = rate_matrix.shape[1]
    order = []
    used = set()

    # Primera selección: fila 0
    current_row = 0
    for _ in range(n_moves):
        best_move = -1
        best_value = -np.inf
        for move in range(n_moves):
            if move in used:
                continue
            value = rate_matrix[current_row, move]
            # Busca el valor más cercano a 1
            if best_move == -1 or abs(1 - value) < abs(1 - best_value):
                best_value = value
                best_move = move

        order.append(best_move)
        used.add(best_move)
        current_row = best_move + 1
        if current_row >= rate_matrix.shape[0]:
            break
    return order



# Carga el archivo CSV
df = pd.read_csv('../Results/sourceData.csv')

# Convertimos las columnas de 'Times Used' y 'Times Improved' a vectores
times_used_vector = df['Times Used'].apply(parse_vector)
times_improved_vector = df['Times Improved'].apply(parse_vector)

# Convertimos las columnas de 'Times Used Conditioned' y 'Times Improved Conditioned' a matrices
times_used_conditioned = df['Times Used Conditioned'].apply(parse_matrix)
times_improved_conditioned = df['Times Improved Conditioned'].apply(parse_matrix)

# Crea la columna de matrices de rate
df['Success Rate Matrix'] = [
    calculate_success_rate_matrix(u, i)
    for u, i in zip(times_used_conditioned, times_improved_conditioned)
]

# Aplica el algoritmo greedy para encontrar el mejor orden
df['Best Order'] = df['Success Rate Matrix'].apply(greedy_best_order)

# Ejemplo: mostrar la primera fila de cada estructura reconstruida
print("Times Used Vec: \n", times_used_vector)
print("Times Improved Vec: \n", times_improved_vector)
print("Times Used Conditioned Mat:\n", times_used_conditioned[0])
print("Times Improved Conditioned Mat:\n", times_improved_conditioned)
# print("Success Rate:\n", df['Success Rate Matrix'].head())
# print("Best Order (primera fila):", df['Best Order'].head())
print(df)