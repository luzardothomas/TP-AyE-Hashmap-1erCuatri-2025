#include "lista.h"

void crear_lista(t_lista* pl) {
  *pl = NULL;
}

int insertar_en_orden(t_lista* pl,const void* dato,size_t tam,comparar_lista cmp) {
  t_nodo_lista* nue;

  while(*pl && cmp((*pl)->dato,dato) > 0)
    pl = &(*pl)->sig;

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

void recorrer_lista(const t_lista* pl,accion_lista act,void* param) {
  while(*pl) {
    act((*pl)->dato,param);
    pl = &(*pl)->sig;
  }
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
