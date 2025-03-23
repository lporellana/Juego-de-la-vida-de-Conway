/*
  Juego de la vida de Conway implementado en C++ usando programación modular  
  Se añadieron las funciones toroidales para simular un espacio cíclico.
  Las celulas se simulan con una matriz de booleanos.
  Faltante: funcion para permitir al usuario determinar el tamaño del tablero, funcion para permitir al usuario determinar la cantidad de generaciones
  Autor: Lporellana
  Coautores: Clase 3er semestre Ingenieria en Sistemas de Información y Ciencias de la Computación sección B, Ingeniero Luis García
  22 de marzo de 2025
*/

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

/**  
 *@brief Limpia los elementos de la pantalla.
*/
void limpiar_pantalla() { cout << "\033[2J\033[H"; }

/**
  *@brief Mueve el cursor a la posición (x, y) de la pantalla.
  *@param x coordenada horizontal.
  *@param y coordenada vertical.     
*/
void gotoxy(int x, int y) { cout << "\033[" << y + 1 << ";" << x + 1 << "H"; }

/**
  *@brief Espera un tiempo.
  *@param ms tiempo en milisegundos.
*/
void esperar(int ms) {
  for (clock_t t = clock(); (clock() - t) * 1000 / CLOCKS_PER_SEC < ms;)
    ;
}

const int FILAS = 40;
const int COLUMNAS = 80;

/**
  *@brief Inicializa la matriz con valores falsos.
  *@param matriz matriz de booleanos.
*/
void inicializar_matriz(bool matriz[FILAS][COLUMNAS]) {
  for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
      matriz[i][j] = false;
    }
  }
}

/**
  *@brief Imprime la matriz en la pantalla.
  *@param matriz matriz de booleanos.
*/
void imprimir_matriz(bool matriz[FILAS][COLUMNAS]) {
  for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
      gotoxy(j, i);
      if (matriz[i][j]) {
        cout << "\xDB";
      } else {
        cout << " ";
      }
    }
  }
  cout << flush;
}

/** 
 * @brief Cuenta los vecinos vivos de una celda en el tablero.
 * @param fila Fila de la celda actual.
 * @param columna Columna de la celda actual.
 * @return Número de células vivas adyacentes (0-8).
 */
int contar_vecinos(bool matrizActual[FILAS][COLUMNAS], int i, int j) {
  
  /// Almacena la cantidad de vecinos que hay alrededor.
  int contador = 0;
  
  // PosAct + PosSig + Ext) El modulo del maximo de filas o columnas
  /// Implementación de tablero toroidal
  const int arriba = (i + (-1) + FILAS) % FILAS;
  const int abajo = (i + (+1)) % FILAS;
  const int izquierda = (j + (-1) + COLUMNAS) % COLUMNAS;
  const int derecha = (j + (+1)) % COLUMNAS;

  contador += matrizActual[arriba][izquierda];
  contador += matrizActual[arriba][j];
  contador += matrizActual[arriba][derecha];

  contador += matrizActual[i][izquierda];
  contador += matrizActual[i][derecha];

  contador += matrizActual[abajo][izquierda];
  contador += matrizActual[abajo][j];
  contador += matrizActual[abajo][derecha];

  return contador;
}

/** 
 * @brief Aplica las reglas del juego de la vida de Conway.
 * @param celulaActual Estado de la célula actual.
 * @param vecinosVivos Número de células vivas adyacentes.
 * @return Nuevo estado de la célula.
 */
bool aplicar_reglas(bool celulaActual, int vecinosVivos) {
  if (celulaActual && vecinosVivos > 3) {
    return false;
  }
  if (celulaActual && vecinosVivos < 2) {
    return false;
  }
  if (!celulaActual && vecinosVivos == 3) {
    return true;
  }

  return celulaActual;
}

/**
 * @brief Calcula la siguiente generación del juego de la vida. 
 * @param matrizActual generación actual. 
 * @param matrizSiguiente generación siguiente.
 */
void calcular_siguiente_generacion(bool matrizActual[FILAS][COLUMNAS],
                                      bool matrizSiguiente[FILAS][COLUMNAS]) {
  for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
      /// cuenta los vecinos que hay alrededor
      int vecinosContados = contar_vecinos(matrizActual, i, j);
      matrizSiguiente[i][j] =
          aplicar_reglas(matrizActual[i][j], vecinosContados);
    }
  }
}

/**
 * @brief Copia una matriz en otra.
 * @param matrizActual matriz actual.
 * @param matrizSiguiente matriz siguiente.
 */
void copiar_matriz(bool matrizActual[FILAS][COLUMNAS],
                   bool matrizSiguiente[FILAS][COLUMNAS]) {

  for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
      matrizActual[i][j] = matrizSiguiente[i][j];
    }
  }
}

int main() {

  srand(time(nullptr));

  bool matrizActual[FILAS][COLUMNAS];
  bool matrizSiguiente[FILAS][COLUMNAS];

  inicializar_matriz(matrizActual);

  matrizActual[0][0] = false;
  matrizActual[0][1] = true;
  matrizActual[0][2] = true;

  matrizActual[1][0] = true;
  matrizActual[1][1] = true;
  matrizActual[1][2] = false;

  matrizActual[2][0] = false;
  matrizActual[2][1] = true;
  matrizActual[2][2] = false;

  limpiar_pantalla();

  while (true) {
    imprimir_matriz(matrizActual);
    calcular_siguiente_generacion(matrizActual, matrizSiguiente);
    copiar_matriz(matrizActual, matrizSiguiente);
    esperar(50);
  }
  
  return 0;
}