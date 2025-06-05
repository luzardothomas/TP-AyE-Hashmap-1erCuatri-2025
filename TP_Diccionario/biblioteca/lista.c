#include "lista.h"

void crear_lista(t_lista* pl) {
  *pl = NULL;
}

int insertar_al_principio_lista(t_lista* pl,const void* dato,size_t tam) {
  t_nodo_lista* nue;
  nue = (t_nodo_lista*)malloc(sizeof(t_nodo_lista));

  if(nue == NULL)
    return 0;

  nue->dato = malloc(tam);

  if(nue->dato == NULL) {
    free(nue);
    return 0;
  }

  memcpy(nue->dato,dato,tam);
  nue->tam_dato = tam;
  nue->sig = *pl;
  *pl = nue;

  return 1;
}

int insertar_en_orden_hasta(t_lista* pl,const void* dato,size_t tam,comparar_lista cmp,size_t* hasta) {
  t_nodo_lista* nue;

  while(*pl && cmp((*pl)->dato,dato) > 0)
    pl = &(*pl)->sig;

  if(*hasta > 0) {
    nue = (t_nodo_lista*)malloc(sizeof(t_nodo_lista));

    if(nue == NULL)
      return 0;

    nue->dato = malloc(tam);

    if(nue->dato == NULL) {
      free(nue);
      return 0;
    }

    memcpy(nue->dato,dato,tam);
    nue->tam_dato = tam;
    nue->sig = *pl;
    *pl = nue;
    (*hasta)--;
  }
  else if(*pl) {
    (*hasta)++;
    eliminar_nodo_del_final(pl);
    insertar_en_orden_hasta(pl,dato,tam,cmp,hasta);
  }

  return 1;
}

void recorrer_lista(const t_lista* pl,accion_lista act,void* param) {
  while(*pl) {
    act((*pl)->dato,param);
    pl = &(*pl)->sig;
  }
}

void eliminar_nodo_del_final(t_lista* pl) {
  if(*pl == NULL)
    return;

  while((*pl)->sig)
    pl = &(*pl)->sig;
  eliminar_nodo(pl);
}

void eliminar_nodo(t_lista* pl) {
  t_nodo_lista* elim = *pl;
  if(elim == NULL)
    return;
  *pl = elim->sig;
  free(elim->dato);
  free(elim);
}

void vaciar_lista(t_lista* pl) {
  t_nodo_lista* elim = *pl;

  while(elim) {
    *pl = elim->sig;
    free(elim->dato);
    free(elim);
    elim = *pl;
  }
}
