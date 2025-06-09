#include "../biblioteca/procesador_de_textos.h"
#include "../biblioteca/menu.h"

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
void probar_procesador_de_texto() {

  FILE* texto;
  t_diccionario dic;
  t_lista podio;
  size_t podio_hasta = 5;


  //generar_texto();

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
    menu_diccionario();
}
