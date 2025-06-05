#include "procesador_de_textos.h"

void generar_texto() {
  FILE* f_texto;
  const char* texto = "Agustina Agustina Agustina Agustina Agustina Guns Guns Guns Guns And Roses Roses Rock And  !!!!!!!!!!!!!!!.......     .....,,,,,,,,,,::::::::";

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

char* procesar_palabra(char* linea) {
  while(*linea && es_letra(linea) == 1)
    linea++;
  return linea;
}

void procesar_linea(char* linea,t_diccionario* pd,accion_diccionario act) {
  char palabra[100];
  char* final_cadena =  NULL;
  size_t len;
  t_registro_texto reg;
  reg.ocurrencias = 1;

  while(*linea) {
    final_cadena = procesar_palabra(linea);

    if(linea != final_cadena) {
      len = final_cadena - linea;
      memcpy(palabra,linea,len);
      *(palabra + len) = '\0';
      strcpy(reg.clave,palabra);
      poner_diccionario(pd,act,reg.clave,&reg.ocurrencias,len + 1,sizeof(reg.ocurrencias));
      poner_diccionario(pd,act,"Palabras\0",&reg.ocurrencias,sizeof(reg.clave),sizeof(reg.ocurrencias));
    }

    linea = final_cadena;

    if(es_espacio(linea))
      poner_diccionario(pd,act,"Espacios\0",&reg.ocurrencias,sizeof(reg.clave),sizeof(reg.ocurrencias));
    else if(es_signo_puntuacion(linea))
      poner_diccionario(pd,act,"Signos de puntuacion\0",&reg.ocurrencias,sizeof(reg.clave),sizeof(reg.ocurrencias));

    linea++;
  }

}

int procesar_texto(FILE* pf,t_diccionario* pd,accion_diccionario act) {
  char linea[100];

  while(fgets(linea,100,pf))
    procesar_linea(linea,pd,act);

  return 1;
}

int generar_podio(t_lista* podio,comparar_lista cmp,accion_lista act,t_diccionario* pd,comparar_diccionario cmp_dic,size_t* podio_hasta,void* param) {
  t_registro_podio reg;
  t_bucket* aux;
  t_nodo_lista* mayor = NULL;
  size_t puesto = 1;
  size_t contar_puestos = 0;
  int i = 0;

  if(pd->table_map == NULL)
    return 0;

  aux = pd->table_map + i * sizeof(t_bucket);

  /// primero hay que cargar un TOP en la lista

  while(i < pd->capacidad_maxima) {

    while(*aux){
      if(cmp_dic(*aux,param)) {
        strcpy(reg.clave,(char*)(*aux)->clave);
        reg.ocurrencias = *((size_t*)(*aux)->valor);
        insertar_en_orden_hasta(podio,&reg,sizeof(reg),cmp,podio_hasta);
      }
      aux = &(*aux)->sig;
    }
    i++;
    aux = pd->table_map + i * sizeof(t_bucket);
  }

  /// ahora que tenemos el TOP es mas facil cargar los puestos del podio

  if(*podio) {
    act((*podio)->dato,&puesto);
    mayor = *podio;
    podio = &(*podio)->sig;
    contar_puestos++;
    while(*podio) {

      if(cmp(mayor->dato,(*podio)->dato) == 0)
        act((*podio)->dato,&puesto);
      else {
        mayor = *podio;
        puesto = contar_puestos + 1;
        act((*podio)->dato,&puesto);
      }

      contar_puestos++;
      podio = &(*podio)->sig;

    }

  }

  return 1;
}

