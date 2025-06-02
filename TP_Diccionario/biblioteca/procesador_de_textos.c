#include "procesador_de_textos.h"

void generar_texto() {
  FILE* f_texto;
  const char* texto = "Agustina Agustina Agustina Agustina Agustina Guns Guns Guns and roses roses  !!!!!!!!!!!!!!!.......     .....,,,,,,,,,,::::::::";

  f_texto = fopen("texto.txt","wt");
  if(f_texto == NULL)
    return;

  fprintf(f_texto,"%s",texto);
  fclose(f_texto);
}

int abrir_archivo(FILE** pf,const char* nombre,const char* tipo) {
  *pf = fopen(nombre,tipo);
  if(!*pf)
    return 0;
  return 1;
}

int es_letra(const char* car) {
  return (*car >= 'A' && *car <= 'Z') || (*car >= 'a' && *car <= 'z');
}

int es_signo_puntuacion(const char* car) {
  char signos_puntuacion[] = {'.',',',';',':','?','!','(',')','[',']','{','}','\'','"'};
  int i = 0;
  while(i < sizeof(signos_puntuacion)/sizeof(signos_puntuacion[0]) && *car != signos_puntuacion[i])
    i++;
  return i < sizeof(signos_puntuacion)/sizeof(signos_puntuacion[0]);
}

int es_espacio(const char* car) {
  return *car == ' ';
}

int procesar_texto(FILE* pf,t_diccionario* pd) {
  char linea[100];
  char palabra[100];
  char* inicio_cadena = NULL;
  char* final_cadena = NULL;
  t_registro_texto reg;
  int i;

  reg.ocurrencias = 1;

  while(fgets(linea,100,pf)) {
    i = 0;

    while(*(linea + i)) {

      inicio_cadena = linea + i;
      final_cadena = inicio_cadena;

      while(es_letra(linea + i)) {
        i++;
        final_cadena = linea + i;
      }

      if(inicio_cadena < final_cadena) {
        memcpy(palabra,inicio_cadena,final_cadena - inicio_cadena);
        *(palabra + (final_cadena - inicio_cadena)) = '\0';
        strcpy(reg.clave,palabra);
        poner_diccionario(pd,reg.clave,&reg.ocurrencias,sizeof(reg.clave),sizeof(reg.ocurrencias));
        poner_diccionario(pd,"Palabras\0",&reg.ocurrencias,sizeof(reg.clave),sizeof(reg.ocurrencias));
      }

      if(es_espacio(linea + i)) {
        poner_diccionario(pd,"Espacios\0",&reg.ocurrencias,sizeof(reg.clave),sizeof(reg.ocurrencias));
      }
      else if(es_signo_puntuacion(linea + i)) {
        poner_diccionario(pd,"Signos de puntuacion\0",&reg.ocurrencias,sizeof(reg.clave),sizeof(reg.ocurrencias));
      }

      i++;

    }
  }

  return 1;
}

int generar_podio(t_lista* podio,comparar_lista cmp,t_diccionario* pd,comparar_diccionario cmp_dic,size_t* podio_hasta,void* param) {
  t_registro_texto reg;
  t_bucket* bucket;
  t_lista* lista;
  size_t indice;

  if(pd->indice_claves == NULL || pd->table_map == NULL || pd->funcion_hash == NULL)
    return 0;

  lista = &pd->indice_claves;


  while(*lista) {
    indice = *((size_t*)(*lista)->dato);
    bucket = pd->table_map + indice * sizeof(t_bucket);

    while(*bucket) {

      if(cmp_dic(*bucket,param)) {

        strcpy(reg.clave,(char*)(*bucket)->clave);
        reg.ocurrencias = *((size_t*)(*bucket)->valor);
        insertar_en_orden_hasta(podio,&reg,sizeof(reg),cmp,podio_hasta);
      }

      bucket = &(*bucket)->sig;
    }

    lista = &(*lista)->sig;

  }

  return 1;
}
