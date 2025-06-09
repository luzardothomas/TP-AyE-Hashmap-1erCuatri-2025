#include "menu.h"

void menu_diccionario()
{
    char opc;
    do
    {
        fflush(stdin);
        printf("%s", MENU_MENSAJE); //menu de opciones
        scanf("%c", &opc);

        opc = toupper(opc);

        if(strchr(MENU_OPCIONES, opc)==NULL)  //validacion de la opcion del usuario
        {
            system("CLS");
            printf("%s", MENU_MENSAJE);
            printf(" Opcion incorrecta. Vuelva a ingresar nuevamente.\n");
            system("pause");
            system("CLS");
        }

        system("cls");

        //entra al menu

        switch(opc)
        {
        case 'A':
            usar_procesador_de_texto("lote1.txt",2000);
            system("pause");
            break;
        case 'B':
            usar_procesador_de_texto("lote2.txt",2000);
            system("pause");
            break;
        case 'C':
            usar_procesador_de_texto("lote3.txt",2000);
            system("pause");
            break;
        case 'D':
            usar_procesador_de_texto("lote4.txt",2000);
            system("pause");
            break;
        }

        system("cls");
    }
    while(opc!='S');
}

void usar_procesador_de_texto(const char* path_lote, size_t capacidad) {

  FILE* texto;
  t_diccionario dic;
  t_lista podio;
  size_t podio_hasta = 3;

  if(!abrir_archivo(&texto,path_lote,"rt"))
    return;

  crear_diccionario(&dic,capacidad,comparar_palabras);
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
