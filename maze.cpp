#include <iostream> //Permite la entrada y salida estándar, es decir, mostrar mensajes en la pantalla y recibir entrada del usuario.
#include <vector> //Proporciona la capacidad de usar vectores, que son arreglos dinámicos en C++.
#include <ctime> //Incluye funciones para trabajar con el tiempo, Necesaria para usar la función time
#include <algorithm> //incluye una gran cantidad de algoritmos que se pueden aplicar a contenedores de datos, como vectores y listas en este ccaso para random.shuffle()
#include <cstdlib> //Proporciona funciones generales de C estándar, como srand y rand para generación de números aleatorios.
#include <thread> // Proporciona facilidades para trabajar con hilos (threads), para agregar delays en este caso.

using namespace std;

// Función para generar el camino utilizando DFS
void Generar_camino(vector<vector<int>> & laberinto, int x, int y) { //& referencia al laberinto ya que trabajaremos sobre la matriz original
    vector<pair<int, int>> direcciones{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; //crea un vector de pares que representan las cuatro posibles direcciones 
    random_shuffle(direcciones.begin(), direcciones.end()); //Barajamos los direcciones
                                                            //para generar caminos diferentes cada vez.

    for (auto direccion : direcciones) {//itera sobre las direcciones mezcladas para ir en esa direccion
    //auto: deduce automaticamente que el elemento dentro de direcciones es un par (osea tiene un valor x y un valor y).
    
        int nx = x + direccion.first * 2; //Que se mueva dos casillas para tener en cuenta los pasillos
        int ny = y + direccion.second * 2;

        if (nx > 0 && ny > 0 && nx < laberinto.size() - 1 && ny < laberinto[0].size() - 1 && laberinto[nx][ny] == 1) {
            laberinto[x + direccion.first][y + direccion.second] = 0; //Si crea un camino válido, le damos el valor de camino pasable: 0
            laberinto[nx][ny] = 0;  //Marcar la celda 2 como parte del camino
            Generar_camino(laberinto, nx, ny); //Esta función inicializa la matriz del laberinto y llama a Generar_camino 
                                                        // para crear el camino desde la entrada hasta la salida.
        }
    }
}

// Función para crear el laberinto
void Crear_laberinto(vector<vector<int>>& laberinto, int size) {
    srand(time(0)); // Inicializa la generación de números aleatorios basada en el tiempo actual, asegurando caminos aleatorios en cada ejecución.

    for (int i = 0; i < size; i++) {//Itera cada fila
        for (int j = 0; j < size; j++) {//Itera cada columna
            laberinto[i][j] = 1; //Entonces inicializamos la matriz llena de muros: 1
        }
    }

    // Generar el camino desde la entrada (1, 1)
    laberinto[1][1] = 0; //Definimos que la entrada siempre en esq sup izq y serà camino con: 0
    Generar_camino(laberinto, 1, 1); // para crear el camino dentro del laberinto.

    // Asegurar que la salida esté conectada al laberinto
    if (laberinto[size - 2][size - 3] == 1 && laberinto[size - 3][size - 2] == 1) {
        laberinto[size - 2][size - 3] = 0;
    }
    laberinto[size - 2][size - 2] = 0; // y definimos la salida, siempre en la esquina inferior derecha, con: 0
}

// Función para imprimir el laberinto
void Imprimir_laberinto(const vector<vector<int>>& laberinto, int size) {
    char walls = 35; // Representar los muros con caracteres o 35
    char hall = ' '; // Representar los vacíos con caracteres //250
    char path = '+';  // Representar el camino con caracteres

    for (int i = 0; i < size; i++) { // Itera cada fila
        for (int j = 0; j < size; j++) { // Itera cada columna
            if (i == 1 && j == 1) { // si las posiciones son iguales que la entreda definida
                cout << 'E';  // Entrada
            } else if (i == size - 2 && j == size - 2) { //Si las posiciones son iguales a la salida
                cout << 'S';  // Salida
            } else if (laberinto[i][j] == 1) { //Si esta llena del valor 1
                cout << walls; // Mostrar walls
            } else if (laberinto[i][j] == 2) { //Si esta llena del valor 2
                cout << path;  // Mostrara el camino
            } else { //Osino
                cout << hall; // va a mostrar el pasillo
            }
            cout << " ";//damos espacios entre columnas
        }
        cout << endl;//Salto de line por cada fila
    }
    cout << endl; // Espacio para separar los pasos
}

// Función recursiva para resolver el laberinto utilizando backtracking
bool Resolver_laberinto(vector<vector<int>>& laberinto, int x, int y) {
    // Si llegamos a la salida, serà cierto entonces retornamos true
    if (x == laberinto.size() - 2 && y == laberinto.size() - 2) {
        laberinto[x][y] = 2;  // Marcar la salida como parte del camino
        Imprimir_laberinto(laberinto, laberinto.size()); // Imprimir el laberinto final
        return true;
    }

    // Marcar la celda actual como visitada (2 para indicar camino recorrido)
    laberinto[x][y] = 2; //2 sera la celda visitada
    Imprimir_laberinto(laberinto, laberinto.size()); // Imprimir el laberinto en cada paso
    this_thread::sleep_for(chrono::milliseconds(200)); // Añadir un pequeño delay para visualizar el proceso

    // Intentar moverse en cada dirección válida
    vector<pair<int, int>> direcciones{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; //Las direcciones que tiene para moverse
    random_shuffle(direcciones.begin(), direcciones.end()); // Mezclar direcciones para visualización

    for (auto & direccion : direcciones) { //Iteremos sobre todos los elementos en direcciones
    //auto: El compilador deduce el tipo de direccion basado en el tipo de elementos en direcciones. En este caso, direccion es un pair<int, int>. osea tiene una tupla
    //&= hace referencia a cada "direccion" elemento en direcciones, de esta forma actuamos directamente sobre el original y no en una copia
        int nx = x + direccion.first; //Accede al primer elemento del par direccion en coordenada x
        int ny = y + direccion.second; //Accede al primer elemento del par direccion en coordenada y

        // Verificar si la próxima posición es válida, que esten dentro de los limites del laberinto
        if (nx >= 0 && ny >= 0 && nx < laberinto.size() && ny < laberinto[0].size() && laberinto[nx][ny] == 0) {//compara si las nuevas coordenas 
        //estan dentro de los limites del laberinto y de si es pasillo

//laberinto.size()= size() es una funcion miembro dentro de la caja: laberinto y con el punto accedemos a los miembros del objeto.

            // Llamar recursivamente para intentar resolver desde la nueva posición
            if (Resolver_laberinto(laberinto, nx, ny)) {
                return true;  // Si encontramos una solución, retornar true
            }
        }
    }

    // Si no encontramos una solución desde esta posición, retroceder (backtracking)
    laberinto[x][y] = 3;  // Desmarcar la celda para visualización de retroceso
    Imprimir_laberinto(laberinto, laberinto.size()); // Imprimir el laberinto en cada retroceso
    this_thread::sleep_for(chrono::milliseconds(200)); // Añadir un pequeño retraso para visualizar el proceso

    return false;  // Si no encontramos ninguna solución desde esta posición, entonces le indicamos que debemos retroceder, ya que no es true
} //el progrma va a ir buscando hasta que se le retorne true.


// Función principal
int main() {
    
    int size; //Size es miembro del contenedor laberinto
    cout << "Puts an integer number for the size of the maze: ";
    cin >> size;

    vector<vector<int>> laberinto(size, vector<int>(size, 1)); // Inicializar el laberinto con muros (valor 1)
    
    Crear_laberinto(laberinto, size);  // Generar el laberinto
    Imprimir_laberinto(laberinto, size);  // Imprimir el laberinto generado

    // Resolver el laberinto
    if (Resolver_laberinto(laberinto, 1, 1)) { //Si retorna true
        cout << "Se ha solucionado el laberinto." << endl;
    } else {//Si no retorna true
        cout << "No se ha solucionado el laberinto." << endl;
    }

    return 0;
}
