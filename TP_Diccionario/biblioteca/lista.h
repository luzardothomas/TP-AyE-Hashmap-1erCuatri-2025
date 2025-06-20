#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#define MINIMO(X,Y) (((X) < (Y)) ? (X) : (Y))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*accion_lista)(void*,void*);
typedef int (*comparar_lista)(const void*,const void*);

typedef struct s_nodo_lista {
  void* dato;
  size_t tam_dato;
  struct s_nodo_lista* sig;
}t_nodo_lista;

typedef t_nodo_lista* t_lista;

void crear_lista(t_lista* pl);
int insertar_en_orden(t_lista* pl,const void* dato,size_t tam,comparar_lista cmp);
void recorrer_lista(const t_lista* pl,accion_lista act,void* param);
void vaciar_lista(t_lista* pl);

#endif // LISTA_H_INCLUDED
