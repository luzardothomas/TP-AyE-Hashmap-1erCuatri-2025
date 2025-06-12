#include "procesador_de_textos.h"

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

void a_minuscula(char* letra) {
  *letra = (*letra >= 'A' && *letra <= 'Z') ? *letra + ('a' - 'A') : *letra;
}

char* procesar_palabra(char* linea) {
  while(*linea && es_letra(linea) == 1) {
    a_minuscula(linea);
    linea++;
  }
  return linea;
}

void procesar_linea(char* linea,t_diccionario* pd,accion_diccionario act,size_t* palabras,size_t* espacios,size_t* signos_puntuacion) {
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
      (*palabras)++;
    }

    linea = final_cadena;

    if(es_espacio(linea))
      (*espacios)++;
    else if(es_signo_puntuacion(linea))
      (*signos_puntuacion)++;

    linea++;
  }

}

int procesar_texto(FILE* pf,t_diccionario* pd,accion_diccionario act) {
  char linea[100];
  size_t cant_palabras = 0;
  size_t cant_espacios = 0;
  size_t cant_signos_puntuacion = 0;


  while(fgets(linea,100,pf))
    procesar_linea(linea,pd,act,&cant_palabras,&cant_espacios,&cant_signos_puntuacion);

  printf("Cantidad de palabras: %ld\n",(long)cant_palabras);
  printf("Cantidad de espacios: %ld\n",(long)cant_espacios);
  printf("Cantidad de signos de puntuacion: %ld\n",(long)cant_signos_puntuacion);

  return 1;
}

int generar_podio(t_lista* podio,comparar_lista cmp,accion_lista act,t_diccionario* pd,size_t escalones,void* param) {
  t_registro_podio reg;
  t_bucket* aux;
  t_nodo_lista* mayor = NULL;
  t_nodo_lista** elim;
  size_t puesto = 1;
  int ganadores = 1;
  int comp = 2;
  int i = 0;

  if(pd->table_map == NULL)
    return 0;

  aux = pd->table_map + i * sizeof(t_bucket);

  /// primero hay que cargar en orden los elementos del diccionario en una lista

  while(i < pd->capacidad_maxima) {

    while(*aux){
      strcpy(reg.clave,(char*)(*aux)->clave);
      reg.ocurrencias = *((size_t*)(*aux)->valor);
      insertar_en_orden(podio,&reg,sizeof(reg),cmp);

      aux = &(*aux)->sig;
    }
    i++;
    aux = pd->table_map + i * sizeof(t_bucket);
  }

  /// ahora que tenemos una lista en orden generamos el podio

  mayor = *podio;

  i = 0;

  while(*podio && puesto <= escalones) {
    ganadores = 1;

    while(*podio && (comp = cmp(mayor->dato,(*podio)->dato)) == 0) {
      act((*podio)->dato,&puesto);
      podio = &(*podio)->sig;
      if(i != 0)
        ganadores++;
      i = 1;
    }

    mayor = *podio;
    puesto += ganadores;

    if(*podio) {
      act((*podio)->dato,&puesto);
      elim = podio;
      podio = &(*podio)->sig;

    }
  }

  if(*podio)
    vaciar_lista(elim);

  return 1;
}

int comparar_palabras(const void* a,const void* b) {
  char* pa = (char*)a;
  char* pb = (char*)b;
  return strcmp(pa,pb);
}

void acumular_ocurrencia(void *a, void *b) {
  size_t *pa = (size_t*)a;
  *pa += 1;
}

void mostrar_dic_texto(void *a,void *b) {
  t_nodo_bucket *aux = (t_nodo_bucket*)a;
  char *cla = (char*)aux->clave;
  size_t *val = (size_t*)aux->valor;

  printf("|Clave: '%s' Ocurrencias: %ld| ",cla,(long)*val);

  if(aux->sig)
    printf(" ==> ");
  else
    puts("");
}

int comparar_ocurrencias(const void *a,const void *b) {
  t_registro_podio *pa = (t_registro_podio*)a;
  t_registro_podio *pb = (t_registro_podio*)b;
  return pa->ocurrencias - pb->ocurrencias;
}

void mostrar_podio_palabras(void* a,void* b) {
  t_registro_podio *pa = (t_registro_podio*)a;
  printf("[PUESTO %ld] Palabra '%s' Ocurrencias %ld\n",(long)pa->puesto,pa->clave,(long)pa->ocurrencias);
}

void ubicar_puestos(void* a,void * b) {
  t_registro_podio *pa = (t_registro_podio*)a;
  size_t* pb = (size_t*)b;
  pa->puesto = *pb;
}

void mostrar_dic_pal(void *a,void *b){
  t_nodo_bucket *aux = (t_nodo_bucket*)a;
  char *cla = (char*)aux->clave;
  char *val = (char*)aux->valor;
  printf("|Clave: %s Valor: %s|",cla,val);

  if(aux->sig)
    printf(" ==> ");
  else
    puts("");

}
