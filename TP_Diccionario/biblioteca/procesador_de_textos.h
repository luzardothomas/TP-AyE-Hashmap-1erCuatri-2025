#ifndef PROCESADOR_DE_TEXTOS_H_INCLUDED
#define PROCESADOR_DE_TEXTOS_H_INCLUDED
#include "diccionario.h"

#define TAM_CLAVE 500

typedef struct {
 char clave[TAM_CLAVE];
 size_t ocurrencias;
}t_registro_texto;

typedef struct {
 char clave[TAM_CLAVE];
 size_t ocurrencias;
 size_t puesto;
}t_registro_podio;

void generar_texto();
int abrir_archivo(FILE** pf,const char* nombre,const char* tipo);
int es_letra(const char* car);
int es_signo_puntuacion(const char* car);
int es_espacio(const char* car);
char* procesar_palabra(char* linea);
void procesar_linea(char* linea,t_diccionario* pd,accion_diccionario act);
int procesar_texto(FILE* pf,t_diccionario* pd,accion_diccionario act);
int generar_podio(t_lista* podio,comparar_lista cmp,accion_lista act,t_diccionario* pd,comparar_diccionario cmp_dic,size_t* podio_hasta,void* param);


#endif // PROCESADOR_DE_TEXTOS_H_INCLUDED
