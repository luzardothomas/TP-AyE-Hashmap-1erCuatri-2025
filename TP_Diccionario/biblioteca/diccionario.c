#include "diccionario.h"

size_t funcion_hash(const unsigned char *clave, size_t len) {
  size_t hash = 5381;
  size_t i;
  for(i = 0; i < len; i++)
    hash = ((hash << 5) + hash) + clave[i]; /// hash * 33 + byte
  return hash;
}

void crear_diccionario(t_diccionario* pd,size_t capacidad_maxima,void* funcion_hash,void* funcion_comparar,void* funcion_accion,comparar_lista cmp_indice) {
  pd->table_map = calloc(capacidad_maxima,sizeof(t_bucket));
  pd->capacidad_maxima = capacidad_maxima;
  pd->funcion_hash = funcion_hash;
  pd->cmp = funcion_comparar;
  pd->act = funcion_accion;
  pd->indice_claves = NULL;
  pd->cmp_indice = cmp_indice;
}

int poner_diccionario(t_diccionario* pd,const void* clave, const void* valor,size_t tam_clave, size_t tam_valor) {
  size_t indice = 0;
  size_t hash;
  int comp = 2;
  t_bucket* aux;
  t_bucket* inicio;
  t_nodo_bucket* nue;

  if(pd->funcion_hash == NULL || pd->table_map == NULL)
    return 0;

  hash = pd->funcion_hash(clave,tam_clave);
  indice = (long)hash % pd->capacidad_maxima;

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

  /// chequear si hay un bucket

  aux = pd->table_map + indice *sizeof(t_bucket);
  inicio = aux;

  while((*aux) && (comp = pd->cmp((*aux)->clave,clave)) != 0)
    aux = &(*aux)->sig;


  /// controlar si hay clave duplicada

  if(comp == 0) {
    free(nue->clave);
    free(nue->valor);
    free(nue);
    if(pd->act)
      pd->act((*aux)->valor,(void*)valor);
    return 2;
  }

  *aux = nue;

  if(*inicio && !(*inicio)->sig)
    insertar_al_principio_lista(&pd->indice_claves,&indice,sizeof(indice));


  return 1;
}

int obtener_diccionario(const t_diccionario* pd,const void* clave,void* valor,size_t tam_clave,size_t tam_valor) {
  size_t hash;
  size_t indice;
  int complejidad = 1;
  t_bucket* aux;

  if(pd->funcion_hash == NULL || pd->table_map == NULL || pd->indice_claves == NULL)
    return 0;

  /// se calcula el hash para luego calcular el indice

  hash = pd->funcion_hash(clave,tam_clave);
  indice = (long)hash % pd->capacidad_maxima;

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
  t_lista* buscar_indice;

  if(pd->funcion_hash == NULL || pd->table_map == NULL || pd->indice_claves == NULL)
    return 0;

  /// se calcula el hash para luego calcular el indice

  hash = pd->funcion_hash(clave,tam_clave);
  indice = (long)hash % pd->capacidad_maxima;

  /// se apunta el table_map hacia donde esta el nodo bucket

  aux = (t_bucket*)((char*)pd->table_map + (indice * sizeof(t_nodo_bucket*)));

  /// si no hubo colisión encuentra desde un inicio, sino busca la clave en el bucket

  while(*aux && (comp = pd->cmp((*aux)->clave,clave)) != 0)
    aux = &(*aux)->sig;

  /// si es el unico nodo bucket hay que eliminar la clave de la lista de claves

  if(comp == 0 && !(*aux)->sig) {
    buscar_indice = buscar_lista(&pd->indice_claves,&indice,pd->cmp_indice);

    if(*buscar_indice)
      eliminar_nodo(buscar_indice);
  }

  eliminar_nodo_bucket(aux);

  /// si ya no hay claves libero la memoria y pongo en nulo al table map

  if(pd->indice_claves == NULL) {
    free(pd->table_map);
    pd->table_map = NULL;
  }

  return 1;
}

void recorrer_diccionario(t_diccionario *pd,accion_diccionario act,void* param){
  t_lista *aux = &pd->indice_claves;
  t_bucket* bucket = pd->table_map;
  size_t indice;

  if(pd->funcion_hash == NULL || pd->table_map == NULL || pd->indice_claves == NULL)
    return;

  if(act)
    pd->act = act;

  while(*aux) {

    indice = *((size_t*)(*aux)->dato);
    bucket = pd->table_map + indice * sizeof(t_bucket);

    while(*bucket) {
      pd->act(*bucket,param);
      bucket = &(*bucket)->sig;
    }

    aux = &(*aux)->sig;
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

void vaciar_dicconario(t_diccionario* pd) {
  t_bucket *aux;
  int i = 0;

  if(pd->indice_claves == NULL)
    return;

  while(++i < pd->capacidad_maxima) {
    aux = pd->table_map + i*sizeof(t_bucket);
    vaciar_bucket(aux);
  }
  free(pd->table_map);
  pd->table_map = NULL;
}
