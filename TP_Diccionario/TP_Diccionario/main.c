#include "../biblioteca/procesador_de_textos.h"

#define TAM_PAL 5

int comparar_palabras(const void* a,const void* b);
void acumular_ocurrencia(void *a,void *b);
int comparar_ocurrencias(const void *a,const void *b);
void mostrar_podio_palabras(void* a,void* b);
void ubicar_puestos(void* a,void * b);
int filtrar_texto(const void* a,const void* b);
void mostrar_dic_pal(void *a,void *b);
void mostrar_dic_texto(void *a,void *b);

typedef struct {
  char clave[TAM_PAL];
  char valor[TAM_PAL];
}t_registro;

int comparar_palabras(const void* a,const void* b) {
  char* pa = (char*)a;
  char* pb = (char*)b;
  return strcmp(pa,pb);
}

void probar_diccionario(){
  t_registro lote[] = {{"hola","vaca"},
                       {"holb","vacb"},
                       {"holc","vacc"},
                       {"hold","vacd"},
                       {"hole","vace"},
                       {"holf","vacf"},
                       {"holg","vacg"},
                       {"holh","vach"},
                       {"holi","vaci"},
                      };

  int i;
  char valor[TAM_PAL];


  t_diccionario dic;
  crear_diccionario(&dic,100,comparar_palabras);

  for(i = 0 ; i < sizeof(lote)/sizeof(lote[0]); i++)
    poner_diccionario(&dic,acumular_ocurrencia,lote[i].clave,lote[i].valor,sizeof(lote[i].clave),sizeof(lote[i].valor));

  recorrer_diccionario(&dic,mostrar_dic_pal,NULL);

  for(i = 0 ; i < sizeof(lote)/sizeof(lote[0]); i++) {
    obtener_diccionario(&dic,lote[i].clave,valor,sizeof(lote[0].clave),sizeof(lote[0].valor));
    if(*valor)
      printf("|| El valor de la clave es: %s|| \n",valor);
  }


  for(i = 0 ; i < sizeof(lote)/sizeof(lote[0]); i++)
    sacar_diccionario(&dic,lote[i].clave,sizeof(lote[0].clave));

  recorrer_diccionario(&dic,mostrar_dic_pal,NULL);


}

void acumular_ocurrencia(void *a, void *b) {
  size_t *pa = (size_t*)a;
  *pa += 1;
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

int filtrar_texto(const void* a,const void* b) {
  t_nodo_bucket* aux = (t_nodo_bucket*)a;

  char* pa = (char*)aux->clave;

  return strcmp(pa,"Palabras") != 0 &&
         strcmp(pa,"Espacios") != 0 &&
         strcmp(pa,"Signos de puntuacion") != 0;
}

void probar_procesador_de_texto() {

  FILE* texto;
  t_diccionario dic;
  t_lista podio;
  size_t podio_hasta = 5;


  generar_texto();

  if(!abrir_archivo(&texto,"texto.txt","rt"))
    return;

  crear_diccionario(&dic,100,comparar_palabras);
  crear_lista(&podio);

  procesar_texto(texto,&dic,acumular_ocurrencia);
  recorrer_diccionario(&dic,mostrar_dic_texto,NULL);

  printf("------------------------- PODIO DE %ld -------------------------\n",(long)podio_hasta);

  generar_podio(&podio,comparar_ocurrencias,ubicar_puestos,&dic,filtrar_texto,&podio_hasta,NULL);

  recorrer_lista(&podio,mostrar_podio_palabras,NULL);

  vaciar_lista(&podio);
  vaciar_diccionario(&dic);
  fclose(texto);


}

int main() {
  puts("============================ PARTE 1 ============================");
  probar_diccionario();
  puts("============================ PARTE 2 ============================");
  probar_procesador_de_texto();

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
