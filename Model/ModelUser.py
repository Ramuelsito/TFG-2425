import joblib
import numpy as np

# Cargar modelo y scaler entrenados
model = joblib.load("model_order.pkl")
scaler = joblib.load("scaler_order.pkl")

# Características de la nueva instancia (ajusta los valores)
nueva_instancia = {
    "n": 50,
    "m": 2,
    "Proportion n/m": 25,
    "Min Time": 2,
    "Max Time": 222,
    "Range": 220,
    "Mean": 112.26,
    "Median": 115,
    "Variance": 2622.14,
    "Standard Deviation": 51.2068
}
allowed_features = [
    "n", "m", "Proportion n/m", "Min Time", "Max Time",
    "Range", "Mean", "Median", "Variance", "Standard Deviation"
]
X_new = np.array([[nueva_instancia[feat] for feat in allowed_features]])
X_new_scaled = scaler.transform(X_new)
predicted_order = model.predict(X_new_scaled)
print("Orden predicho:", predicted_order[0])