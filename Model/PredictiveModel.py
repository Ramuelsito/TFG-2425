import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.multioutput import MultiOutputClassifier
from mpl_toolkits.mplot3d import Axes3D  
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from sklearn.metrics import accuracy_score, classification_report
import matplotlib.pyplot as plt
import seaborn as sns
import joblib

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
    return df_clean, df


# --- 2. Clusterización de instancias por comportamiento heurístico ---

def mahalanobis_whitening(X):
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)
    cov = np.cov(X_scaled, rowvar=False)
    inv_cov = np.linalg.inv(cov)
    L = np.linalg.cholesky(inv_cov)
    X_maha = X_scaled @ L
    return X_maha

def elbow_method(features, max_k=10, use_mahalanobis=False):
    if use_mahalanobis:
        features = mahalanobis_whitening(features)
    scaler = StandardScaler()
    features_scaled = scaler.fit_transform(features)
    inertias = []
    for k in range(1, max_k + 1):
        kmeans = KMeans(n_clusters=k, random_state=42)
        kmeans.fit(features_scaled)
        inertias.append(kmeans.inertia_)
    plt.figure(figsize=(8, 4))
    plt.plot(range(1, max_k + 1), inertias, marker='o')
    plt.xlabel('Número de clusters')
    plt.ylabel('Inercia')
    plt.title('Método del codo para elegir K')
    plt.savefig("elbow_method.png")
    plt.close()

def cluster_instances(features, df_original, n_clusters=5, use_mahalanobis=False):
    # Mahalanobis whitening si se solicita
    if use_mahalanobis:
        features = mahalanobis_whitening(features)
    # Escalar las características
    scaler = StandardScaler()
    features_scaled = scaler.fit_transform(features)

    # Aplicar KMeans para clusterizar
    kmeans = KMeans(n_clusters=n_clusters, random_state=42)
    clusters = kmeans.fit_predict(features_scaled)
    df_original['Cluster'] = clusters

    # PCA para visualización
    pca_3d = PCA(n_components=3)
    features_pca_3d = pca_3d.fit_transform(features_scaled)
    pca_2d = PCA(n_components=2)
    features_pca_2d = pca_2d.fit_transform(features_scaled)

    # Visualización 3D
    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')
    scatter = ax.scatter(features_pca_3d[:, 0], features_pca_3d[:, 1], features_pca_3d[:, 2],
                         c=clusters, cmap='viridis', s=60)
    ax.set_title('Clusters visualizados con PCA (3D)')
    ax.set_xlabel('PC1')
    ax.set_ylabel('PC2')
    ax.set_zlabel('PC3')
    legend1 = ax.legend(*scatter.legend_elements(), title="Cluster")
    ax.add_artist(legend1)
    plt.tight_layout()
    plt.savefig("clusters_visualization_3d.png")
    plt.close()

    # Visualización 2D
    plt.figure(figsize=(8, 6))
    sns.scatterplot(x=features_pca_2d[:, 0], y=features_pca_2d[:, 1], hue=clusters, palette='viridis', s=60)
    plt.xlabel('PC1')
    plt.ylabel('PC2')
    plt.title('Clusters visualizados con PCA (2D)')
    plt.legend(title="Cluster")
    plt.tight_layout()
    plt.savefig("clusters_visualization_2d.png")
    plt.close()

    for i in range(n_clusters):
        mean_original = df_original[df_original['Cluster'] == i][["n", "m", "Proportion n/m", "Min Time", "Standard Deviation"]].mean()
        print(f"Media de las features originales para Cluster {i}:\n{mean_original.values}")

    print("\nNúmero de entradas por cluster:")
    print(df_original['Cluster'].value_counts())
    return kmeans

# --- 3. Predicción del orden óptimo de movimientos ---
def train_order_prediction(features, y_order):
    # Dividir los datos en conjuntos de entrenamiento y prueba
    X_train, X_test, y_train, y_test = train_test_split(features, y_order, test_size=0.25, random_state=42)

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

    # Guarda el modelo y el scaler
    joblib.dump(model, "model_order.pkl")
    joblib.dump(scaler, "scaler_order.pkl")
    return model, scaler



# --- MAIN PIPELINE ---

def main():
    file_path = '../Results/sourceData.csv'
    df_clean, df_original = preprocess_data(file_path)

    # Separar features y labels
    # Verificar que las columnas se están seleccionando bien
    allowed_features = [
        "n", "m", "Proportion n/m", "Min Time", "Standard Deviation"
    ]
    print("Columnas seleccionadas como features:")
    print(df_clean[allowed_features].head())

    # Calcular y mostrar la matriz de correlación
    corr_matrix = df_clean[allowed_features].corr()
    print("\nMatriz de correlación entre features:")
    print(corr_matrix)
    plt.figure(figsize=(10, 8))
    sns.heatmap(corr_matrix, annot=True, cmap='coolwarm')
    plt.title("Matriz de correlación entre features")
    plt.tight_layout()
    plt.savefig("correlation_matrix.png")
    plt.close()

    features = df_clean[allowed_features].values
    y_order = df_clean[["Order_0", "Order_1", "Order_2", "Order_3"]].values

    # Mostrar todos los órdenes diferentes en el dataset
    unique_orders = np.unique(y_order, axis=0)
    print("Órdenes diferentes en el dataset:")
    for order in unique_orders:
        print(order)


# -------------------------- BALANCEO DE DATOS --------------------------
    majority_mask = np.all(y_order == [3, 1, 0, 2], axis=1)
    minority_mask = ~majority_mask

    n_minority = np.sum(minority_mask)
    majority_indices = np.where(majority_mask)[0]
    minority_indices = np.where(minority_mask)[0]

    np.random.seed(42)
    selected_majority = np.random.choice(majority_indices, size=n_minority, replace=False)
    balanced_indices = np.concatenate([selected_majority, minority_indices])

    # Reordena los datos balanceados
    features_balanced = features[balanced_indices]
    y_order_balanced = y_order[balanced_indices]
    
    print("Tamaño del dataset balanceado:", len(features_balanced))
    print("Nº instancias orden mayoritario en balanceado:", np.sum(np.all(y_order_balanced == [3, 1, 0, 2], axis=1)))
    print("Nº instancias minoritarias en balanceado:", np.sum(np.any(y_order_balanced != [3, 1, 0, 2], axis=1)))

# -------------------------- Predecir el orden óptimo --------------------------
    print(df_clean.head())
    model_order, scaler = train_order_prediction(features_balanced, y_order_balanced)

# -------------------------- Clusterizar instancias por comportamiento heurístico --------------------------
    cluster_features = df_original[["n", "m", "Proportion n/m", "Min Time", "Standard Deviation"]].values
    elbow_method(cluster_features, max_k=10, use_mahalanobis=True)
    kmeans = cluster_instances(cluster_features, df_original, n_clusters=4, use_mahalanobis=True)

    print("Ejemplo de entrada con orden [3, 1, 2, 0]:")
    print(df_original[np.all(y_order == [3, 1, 2, 0], axis=1)].head(5))
    print(df_original[df_original["Instance Name"] == "I50j_2m_S1_UU_1.txt"])
    print(df_original[df_original["Instance Name"] == "I50j_2m_S1_UU_1.txt"][["Instance Name", "Max TCT", "Mean TCT","Min TCT"]])

if __name__ == "__main__":
    main()