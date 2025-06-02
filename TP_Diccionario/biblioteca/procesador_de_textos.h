#ifndef PROCESADOR_DE_TEXTOS_H_INCLUDED
#define PROCESADOR_DE_TEXTOS_H_INCLUDED
#include "diccionario.h"

#define TAM_CLAVE 500

typedef struct {
 char clave[TAM_CLAVE];
 size_t ocurrencias;
}t_registro_texto;

void generar_texto();
int abrir_archivo(FILE** pf,const char* nombre,const char* tipo);
int procesar_texto(FILE* pf,t_diccionario* pd);
//int generar_podio(t_lista* pl,comparar cmp,t_diccionario* pd,accion act,size_t* podio_hasta);
int generar_podio(t_lista* podio,comparar_lista cmp,t_diccionario* pd,comparar_diccionario cmp_dic,size_t* podio_hasta,void* param);


#endif // PROCESADOR_DE_TEXTOS_H_INCLUDED
