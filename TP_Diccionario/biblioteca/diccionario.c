#include "diccionario.h"

size_t funcion_hash(const unsigned char *clave, size_t len) {
  size_t hash = 5381;
  size_t i;
  for(i = 0; i < len; i++)
    hash = ((hash << 5) + hash) + clave[i]; /// hash * 33 + byte
  return hash;
}

void crear_diccionario(t_diccionario* pd,size_t capacidad_maxima,void* funcion_comparar) {
  pd->table_map = calloc(capacidad_maxima,sizeof(t_bucket));
  pd->capacidad_maxima = capacidad_maxima;
  pd->cmp = funcion_comparar;
}

int poner_diccionario(t_diccionario* pd,accion_diccionario act,const void* clave, const void* valor,size_t tam_clave, size_t tam_valor) {
  size_t indice = 0;
  size_t hash;
  int comp = 2;
  t_bucket* aux;
  t_nodo_bucket* nue;

  /// si no hubo memoria para el table_map salgo

  if(pd->table_map == NULL)
    return 0;

  hash = funcion_hash(clave,tam_clave);
  indice = hash % pd->capacidad_maxima;

  /// chequear si hay un bucket

  aux = pd->table_map + indice * sizeof(t_bucket);

  while((*aux) && (comp = pd->cmp((*aux)->clave,clave)) != 0)
    aux = &(*aux)->sig;

  /// controlar si hay clave duplicada

  if(comp == 0) {
    if(act)
      act((*aux)->valor,(void*)valor);
    return 2;
  }

  /// insertar en el bucket

  nue = (t_nodo_bucket*)malloc(sizeof(t_nodo_bucket));

  if(nue == NULL)
    return 0;

  nue->clave = malloc(tam_clave);

  if(nue->clave == NULL) {
    free(nue);
    return 0;
  }

  nue->valor = malloc(tam_valor);

  if(nue->valor == NULL) {
    free(nue->clave);
    free(nue);
    return 0;
  }

  memcpy(nue->clave,clave,tam_clave);
  memcpy(nue->valor,valor,tam_valor);
  nue->tam_clave = tam_clave;
  nue->tam_valor = tam_valor;
  nue->sig = NULL;

  *aux = nue;

  return 1;
}

int obtener_diccionario(const t_diccionario* pd,const void* clave,void* valor,size_t tam_clave,size_t tam_valor) {
  size_t hash;
  size_t indice;
  int complejidad = 1;
  t_bucket* aux;

  if(pd->table_map == NULL)
    return 0;

  /// se calcula el hash para luego calcular el indice

  hash = funcion_hash(clave,tam_clave);
  indice = hash % pd->capacidad_maxima;

  /// se apunta el table_map hacia donde esta el nodo bucket

  aux = (t_bucket*)((char*)pd->table_map + (indice * sizeof(t_nodo_bucket*)));

  while(*aux && pd->cmp((*aux)->clave,clave)) {
    aux = &(*aux)->sig;
    complejidad++;
  }

  /// si aux es nulo no encontró la clave en el bucket

  if(!*aux)
    return 0;

  memcpy(valor,(*aux)->valor,MINIMO(tam_valor,(*aux)->tam_valor));

  puts("");

  puts("======================================================");
  printf(" La busqueda se encontro con una complejidad de O(%d) \n",complejidad);
  puts("======================================================");

  puts("");

  return 1;
}

int sacar_diccionario(t_diccionario* pd,const void* clave,size_t tam_clave) {
  size_t hash;
  size_t indice;
  t_bucket* aux;
  int comp = 2;
  int i = 0;

  if(pd->table_map == NULL)
    return 0;

  /// se calcula el hash para luego calcular el indice

  hash = funcion_hash(clave,tam_clave);
  indice = hash % pd->capacidad_maxima;

  /// se apunta el table_map hacia donde esta el nodo bucket

  aux = (t_bucket*)((char*)pd->table_map + (indice * sizeof(t_nodo_bucket*)));

  /// si no hubo colisión encuentra desde un inicio, sino busca la clave en el bucket

  while(*aux && (comp = pd->cmp((*aux)->clave,clave)) != 0)
    aux = &(*aux)->sig;

  /// elimina el nodo bucket en el que estaba la clave

  eliminar_nodo_bucket(aux);

  aux = pd->table_map + i * sizeof(t_bucket);

  /// se controla si todos los nodos son nulos

  while(++i < pd->capacidad_maxima && *aux == NULL)
    aux = pd->table_map + i * sizeof(t_bucket);

  /// en caso de serlo se libera la memoria

  if(*aux == NULL) {
    free(pd->table_map);
    pd->table_map = NULL;
  }

  return 1;
}

void recorrer_diccionario(t_diccionario *pd,accion_diccionario act,void* param){
  t_bucket* aux = pd->table_map;
  int i = 0;

  if(pd->table_map == NULL)
    return;

  aux = pd->table_map + i * sizeof(t_bucket);

  while(i < pd->capacidad_maxima){
    while(*aux){
      act(*aux,param);
      aux = &(*aux)->sig;
    }
    i++;
    aux = pd->table_map + i * sizeof(t_bucket);
  }
}

void eliminar_nodo_bucket(t_bucket* pb) {
  t_nodo_bucket* elim = *pb;
  if(elim == NULL)
    return;
  *pb = elim->sig;
  free(elim->clave);
  free(elim->valor);
  free(elim);
}

void vaciar_bucket(t_bucket* pb) {
  t_nodo_bucket* elim = *pb;

  while(elim) {
    *pb = elim->sig;
    free(elim->clave);
    free(elim->valor);
    free(elim);
    elim = *pb;
  }
}

void vaciar_diccionario(t_diccionario* pd) {
  t_bucket *aux;
  int i = 0;

  if(pd->table_map == NULL)
    return;

  while(++i < pd->capacidad_maxima) {
    aux = pd->table_map + i * sizeof(t_bucket);
    vaciar_bucket(aux);
  }
  free(pd->table_map);
  pd->table_map = NULL;
}
