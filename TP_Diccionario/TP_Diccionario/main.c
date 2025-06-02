#include "../biblioteca/procesador_de_textos.h"

#define TAM_PAL 5

void mostrar_dic_pal(void *a,void *b);
void mostrar_dic_texto(void *a,void *b);
void acumular_ocurrencia(void *a, const void *b);
int comparar_palabras(const void* a,const void* b);

typedef struct {
  char clave[TAM_PAL];
  char valor[TAM_PAL];
}t_registro;

int comparar_palabras(const void* a,const void* b) {
  char* pa = (char*)a;
  char* pb = (char*)b;
  return strcmp(pa,pb);
}

int comparar_indices(const void*a,const void* b) {
  size_t* pa = (size_t*)a;
  size_t* pb = (size_t*)b;
  return *pa - *pb;
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
  crear_diccionario(&dic,100,funcion_hash,comparar_palabras,mostrar_dic_pal,comparar_indices);

  for(i = 0 ; i < sizeof(lote)/sizeof(lote[0]); i++)
    poner_diccionario(&dic,lote[i].clave,lote[i].valor,sizeof(lote[i].clave),sizeof(lote[i].valor));

  recorrer_diccionario(&dic,NULL,NULL);

  for(i = 0 ; i < sizeof(lote)/sizeof(lote[0]); i++) {
    obtener_diccionario(&dic,lote[i].clave,valor,sizeof(lote[0].clave),sizeof(lote[0].valor));
    if(*valor)
      printf("|| El valor de la clave es: %s|| \n",valor);
  }


  for(i = 0 ; i < sizeof(lote)/sizeof(lote[0]); i++)
    sacar_diccionario(&dic,lote[i].clave,sizeof(lote[0].clave));

  recorrer_diccionario(&dic,NULL,NULL);


}

void acumular_ocurrencia(void *a, const void *b) {
  size_t *pa = (size_t*)a;
  *pa += 1;
}

int comparar_enteros(const void *a,const void *b) {
  int *pa = (int*)a;
  int *pb = (int*)b;
  return *pa - *pb;
}

void mostrar_enteros(void *a,void *b) {
  int *pa = (int*)a;
  printf("%d\n",*pa);
}

void probar_lista() {
  t_lista lista;
  int vector [] = {1,2,3,4,5,6,7};
  int i;
  crear_lista(&lista);
  size_t hasta = 5;
  for(i = 0 ; i < sizeof(vector)/sizeof(vector[0]) ; i++)
    insertar_en_orden_hasta(&lista,&vector[i],sizeof(vector[0]),comparar_enteros,&hasta);
  recorrer_lista(&lista,mostrar_enteros,NULL);
}

int comparar_ocurrencias(const void *a,const void *b) {
  t_registro_texto *pa = (t_registro_texto*)a;
  t_registro_texto *pb = (t_registro_texto*)b;
  return pa->ocurrencias - pb->ocurrencias;
}

void mostrar_podio_palabras(void* a,void* b) {
  t_registro_texto *pa = (t_registro_texto*)a;
  printf("Palabra '%s' Ocurrencias %ld\n",pa->clave,(long)pa->ocurrencias);
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

  crear_diccionario(&dic,100,funcion_hash,comparar_palabras,acumular_ocurrencia,comparar_indices);
  crear_lista(&podio);

  procesar_texto(texto,&dic);
  recorrer_diccionario(&dic,mostrar_dic_texto,NULL);

  printf("TOP %ld\n",(long)podio_hasta);

  generar_podio(&podio,comparar_ocurrencias,&dic,filtrar_texto,&podio_hasta,NULL);


  recorrer_lista(&podio,mostrar_podio_palabras,NULL);

  vaciar_lista(&podio);
  vaciar_dicconario(&dic);
  fclose(texto);


}

int main() {
  //probar_diccionario();
  probar_procesador_de_texto();
  //probar_lista();
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
