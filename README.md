# Preguntas
- Hacer una especia de memoria, en la que se van almacenando las soluciones de cierta manera para que en el caso de que se quiera comparar soluciones, o lo que sea, se pueda acceder a todas esas soluciones de forma sencilla. Un fichero que las almacena de cierta forma esto se podria hacer a lo largo de la ejecucion del algoritmos para conocer con mas detalle las soluciones vecinas y todas esas soluciones que no son optimos locales. Seria como almacenar la informacion de un walk
- Medir la correlacion entre vecinos
- Se puede analizar la autocorrelación de valores de fitness a lo largo de un camino de búsqueda aleatoria.
- Podemos regular los valores que apareceran en las posiciones asociadas a cierta tarea para manejar la varianza y la dispersion.

# TODO
- Puntero a problem sin getInstance()
- Posible mejora del rendimiento, tener ya la matriz completa de todos los tij -> Sij + Pij
- Forma de igualdad, con distancias

- 2 rangos de valores para las tareas, 2 variantes de generacion, 5 instancias de cada combinación. 4 combinaciones de tarea, 4 combinaciones de máquina.
- Proporcion de las maquinas

- Luego, Sacar la información propia de varias instancias, generando asi un benchmark
- Segmentation fault, es probablemente en el shaking, deberia de poner condiciones o algo para debuguear

# Done But Not Commented
- Al comparar las soluciones, no son exactamente iguales pero tienen mismo TCT, comentarlo. Lo que se puede hacer es comprobar si la distancia es mayor a un limite, y si tienen menos de cierta distancia y mismo TCT son iguales.

# Implementación

El uso de punteros inteligentes se debe a que estamos usando punteros de forma dinámica, por lo que este tipo de punteros se gestionan solos y nos permiten tener mayor flexibilidad.
- Al tener distintas instancias de distintas caracteristicas obtenemos distintos espacios de soluciones que nos ayuda a caracterizar mejor el problema y poder predecir algunos aspectos en relacion a lo estudiado.