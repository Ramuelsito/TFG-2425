# Preguntas
- Hacer una especia de memoria, en la que se van almacenando las soluciones de cierta manera para que en el caso de que se quiera comparar soluciones, o lo que sea, se pueda acceder a todas esas soluciones de forma sencilla. Un fichero que las almacena de cierta forma esto se podria hacer a lo largo de la ejecucion del algoritmos para conocer con mas detalle las soluciones vecinas y todas esas soluciones que no son optimos locales. Seria como almacenar la informacion de un walk
- Medir la correlacion entre vecinos
- Se puede analizar la autocorrelación de valores de fitness a lo largo de un camino de búsqueda aleatoria.
- Podemos regular los valores que apareceran en las posiciones asociadas a cierta tarea para manejar la varianza y la dispersion.
- Al tener distintas instancias de distintas caracteristicas obtenemos distintos espacios de soluciones que nos ayuda a caracterizar mejor el problema y poder predecir algunos aspectos en relacion a lo estudiado.
# TODO
- Puntero a problem sin getInstance()
- Discriminamos el S' con mismo TCT que otra.
- Posible mejora del rendimiento, tener ya la matriz completa de todos los tij -> Sij + Pij
- Hay un error que no considera el primer archivo de una instancia
- Almacenar correctamente lo de los entornos, y con ello lo de las soluciones. 
- 3 rangos de valores para las tareas, 6 combinaciones, 5 instancias de cada combinación. 4 combinaciones de tarea, 4 combinaciones de máquina.

- Luego, Sacar la información propia de varias instancias, generando asi un benchmark
- Seguiría, analizar las diferentes medidas propuestas que tenian que ver con la aplicación de los distintos entornos, para completar con ello la información necesaria para clasificar que entornos van mejor para cada tipo de instancia, y ademas generar el clasificador a partir de ahi.

# Done But Not Commented
- Primero tener un generador que haga instancias que podamos clusterizar de alguna forma.

