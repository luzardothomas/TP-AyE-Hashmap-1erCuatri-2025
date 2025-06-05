#ifndef DICCIONARIO_H_INCLUDED
#define DICCIONARIO_H_INCLUDED
#define MINIMO(X,Y) (((X) < (Y)) ? (X) : (Y))
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*accion_diccionario)(void*,void*);
typedef int (*comparar_diccionario)(const void*,const void*);

typedef struct {
  void* table_map;
  size_t capacidad_maxima;
  comparar_diccionario cmp;
}t_diccionario;

typedef struct s_nodo_bucket {
  void* clave;
  void* valor;
  size_t tam_clave;
  size_t tam_valor;
  struct s_nodo_bucket* sig;
}t_nodo_bucket;

typedef t_nodo_bucket* t_bucket;

size_t funcion_hash(const unsigned char *data,size_t len);

/// PRIMITIVAS DE DICCIONARIO

void crear_diccionario(t_diccionario* pd,size_t capacidad_maxima,void* funcion_comparar);
int poner_diccionario(t_diccionario* pd,accion_diccionario act,const void* clave, const void* valor,size_t tam_clave, size_t tam_valor);
int obtener_diccionario(const t_diccionario* pd,const void* clave,void* valor,size_t tam_clave,size_t tam_valor);
int sacar_diccionario(t_diccionario* pd,const void* clave,size_t tam_clave);
void recorrer_diccionario(t_diccionario *pd,accion_diccionario act,void* param);
void vaciar_diccionario(t_diccionario* pd);

/// PRIMITIVA DE BUCKET

void eliminar_nodo_bucket(t_bucket* pb);
void vaciar_bucket(t_bucket* pb);

#endif // DICCIONARIO_H_INCLUDED
