import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.multioutput import MultiOutputClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from sklearn.metrics import accuracy_score, classification_report
import matplotlib.pyplot as plt
import seaborn as sns

# --- 0.5 Funciones auxiliares para procesar los datos ---

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

# --- 1. Carga y Preprocesamiento ---

def preprocess_data(file_path):
    # Carga el archivo CSV
    df = pd.read_csv(file_path)
    
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

    for i in range(5):
        for j in range(4):
            df[f"Success_{i}_{j}"] = df['Success Rate Matrix'].apply(lambda x: x[i, j] if i < x.shape[0] and j < x.shape[1] else 0)

    for i in range(4):
        df[f"Order_{i}"] = df["Best Order"].apply(lambda x: x[i])
    
    df_clean = df.drop(columns=['Instance Name', 'Times Used', 'Times Improved', 'Times Used Conditioned', 'Times Improved Conditioned', 'Success Rate Matrix', 'Best Order'])
    # # Ejemplo: mostrar la primera fila de cada estructura reconstruida
    # print("Times Used Vec: \n", times_used_vector)
    # print("Times Improved Vec: \n", times_improved_vector)
    # print("Times Used Conditioned Mat:\n", times_used_conditioned[0])
    # print("Times Improved Conditioned Mat:\n", times_improved_conditioned)
    # # print("Success Rate:\n", df['Success Rate Matrix'].head())
    # # print("Best Order (primera fila):", df['Best Order'].head())
    # print(df)
    return df_clean, df

def train_order_prediction(features, y_order):
    # Dividir los datos en conjuntos de entrenamiento y prueba
    X_train, X_test, y_train, y_test = train_test_split(features, y_order, test_size=0.2, random_state=42)

    # Escalar las características
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)

    # Entrenar el modelo MultiOutputClassifier con RandomForest
    model = MultiOutputClassifier(RandomForestClassifier(n_estimators=100, random_state=42))
    model.fit(X_train_scaled, y_train)

    # Predecir en el conjunto de prueba
    y_pred = model.predict(X_test_scaled)

    # Evaluar el modelo    
    for i in range(y_test.shape[1]):
        print(f"Order_{i} accuracy: {accuracy_score(y_test[:, i], y_pred[:, i]):.2f}")
        print(f"Order_{i} report:\n{classification_report(y_test[:, i], y_pred[:, i])}")

    exact_match = np.all(y_test == y_pred, axis=1).mean()
    print(f"Exact match accuracy (todas las posiciones bien): {exact_match:.2f}")
    return model



# --- MAIN PIPELINE ---

def main():
    file_path = '../Results/sourceData.csv'
    df_clean, df_original = preprocess_data(file_path)

    # Separar features y labels
    allowed_features = [
        "n", "m", "Proportion n/m", "Min Time", "Max Time",
        "Range", "Mean", "Median", "Variance", "Standard Deviation"
    ]
    features = df_clean[allowed_features].values
    y_order = df_clean[["Order_0", "Order_1", "Order_2", "Order_3"]].values

    # 1. Predecir el orden óptimo
    print(df_clean.head())
    model_order = train_order_prediction(features, y_order)

    # # 2. Clusterizar instancias por comportamiento heurístico
    # columna que represente el grupo
    # kmeans = cluster_instances(features, df_original)

    # # 3. Predecir efectividad de una estrategia
    # strategy_model = train_strategy_effectiveness(features, df_original)

if __name__ == "__main__":
    main()