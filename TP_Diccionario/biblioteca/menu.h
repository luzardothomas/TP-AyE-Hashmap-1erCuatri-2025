#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "procesador_de_textos.h"
#include "diccionario.h"
#include <ctype.h>

#define MENU_MENSAJE "BIENVENIDO AL PROCESADOR DE TEXTOS\n"                           \
                     "Elija un lote de pruebas:\n"                        \
                     "A - LOTE 1\n"                       \
                     "B - LOTE 2\n"                       \
                     "C - LOTE 3\n"            \
                     "D - LOTE 4\n"            \
                     "S - Salir\n"                                  \
                     "-->"
#define MENU_OPCIONES "ABCDS"

/// FUNCIONES DE MENU

void menu_diccionario();
void usar_procesador_de_texto(const char* path_lote, size_t capacidad, size_t escalones);

#endif // MENU_H_INCLUDED
