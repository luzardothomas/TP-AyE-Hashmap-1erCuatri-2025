#ifndef PROCESADOR_DE_TEXTOS_H_INCLUDED
#define PROCESADOR_DE_TEXTOS_H_INCLUDED
#include "diccionario.h"

#define TAM_CLAVE 500
#define TAM_PAL 5

typedef struct {
 char clave[TAM_CLAVE];
 size_t ocurrencias;
}t_registro_texto;

typedef struct {
 char clave[TAM_CLAVE];
 size_t ocurrencias;
 size_t puesto;
}t_registro_podio;

typedef struct {
  char clave[TAM_PAL];
  char valor[TAM_PAL];
}t_registro;

void generar_texto();
int abrir_archivo(FILE** pf,const char* nombre,const char* tipo);
int es_letra(const char* car);
int es_signo_puntuacion(const char* car);
int es_espacio(const char* car);
char* procesar_palabra(char* linea);
void procesar_linea(char* linea,t_diccionario* pd,accion_diccionario act);
int procesar_texto(FILE* pf,t_diccionario* pd,accion_diccionario act);
int generar_podio(t_lista* podio,comparar_lista cmp,accion_lista act,t_diccionario* pd,comparar_diccionario cmp_dic,size_t* podio_hasta,void* param);

int comparar_palabras(const void* a,const void* b);
void acumular_ocurrencia(void *a,void *b);
void mostrar_dic_texto(void *a,void *b);
int comparar_ocurrencias(const void *a,const void *b);
void mostrar_podio_palabras(void* a,void* b);
void ubicar_puestos(void* a,void * b);
int filtrar_texto(const void* a,const void* b);
void mostrar_dic_pal(void *a,void *b);

#endif // PROCESADOR_DE_TEXTOS_H_INCLUDED
