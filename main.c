#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

int columna_de_tabla_comentario(int);
int columna_de_tabla_operadores(int);
int columna_de_tabla_digitos(int);
int columna_de_tabla_identificadores(int);
int columna_de_cadena(int);
int estado_comentarios(int);
int estado_operadores(int);
int estado_identificadores (int);
int estado_digitos(int);
int esPuntuacion (char);
int operadores_sin_resta(char);
int esOperador (char);
int esPalabraReservada (char *);
char siguienteCaracter(FILE *,char *);
int scanner(const char *,int);

static int tabla_comentario [10][9]={

        { 9 , 9 , 9 , 9 , 1 , 6 , 8 , 9 , 9 },
        { 9 , 9 , 9 , 9 , 2 , 9 , 9 , 3 , 9 },
        { 2 , 2 , 2 , 2 , 9 , 9 , 9 , 9 , 9 },
        { 3 , 3 , 3 , 3 , 9 , 7 , 3 , 4 , 9 },
        { 9 , 9 , 9 , 9 , 5 , 9 , 9 , 9 , 9 },
        { 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 },
        { 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 },
        { 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 },
        { 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 },
        { 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 , 9 }

};

static int tabla_operadores[14][10]={

        {  1 ,  6 ,  7 ,  8 ,  9 , 10 , 11 , 12 ,  4 , 13 },
        {  2 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 ,  5 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 },
        { 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 , 13 }

};

static int tabla_identificadores[4][4]={

        { 1 , 3 , 2 , 3 },
        { 1 , 1 , 1 , 3 },
        { 2 , 2 , 2 , 3 },
        { 3 , 3 , 3 , 3 }

};

static int tabla_digitos[3][2]={

        { 1 , 2 },
        { 1 , 2 },
        { 2 , 2 }

};

static int tabla_cadena[4][6]={

        { 3 , 3 , 3 , 3 , 1 , 3},
        { 1 , 1 , 1 , 1 , 2 , 3},
        { 3 , 3 , 3 , 3 , 3 , 3},
        { 3 , 3 , 3 , 3 , 3 , 3}
};

void mostrar_por_pantalla(char caracterLeido, FILE* archivoIn, int situacion, int nrolinea, char *palabra, char *ruta){
    int caracterObtenido=1;
    switch(situacion){
    case 1:
        if(scanner(palabra,situacion) == 1)
            printf("LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: COMENTARIO\n\n",nrolinea, palabra);
        else printf("LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n",nrolinea, palabra);
        break;
    case 2:
        if(scanner(palabra,situacion) == 1 )
            printf("LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: OPERADOR\n\n", nrolinea ,palabra);
        else printf("LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea, palabra);
        break;
    case 3:
        if(scanner(palabra,situacion) == 1)
        {
            if(esPalabraReservada(palabra))
                printf("LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: PALABRA RESERVADA\n\n",nrolinea, palabra);
            else{
                while (caracterObtenido){
                    if(siguienteCaracter(archivoIn,ruta)!=' ')
                        caracterObtenido=0;
                    else
                        caracterLeido=fgetc(archivoIn);
                }

                if (siguienteCaracter(archivoIn,ruta)=='(')
                    printf("LINEA NRO.: %d.     LEXEMA:  %s .   TOKEN: IDENTIFICADOR DE SUBPROGRAMA\n\n",nrolinea,palabra);
                else
                    printf("LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: IDENTIFICADOR\n\n",nrolinea,palabra);
            }
        }
        else printf("LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea, palabra);
        break;
    case 4:
        if (scanner(palabra,situacion) == 1)
            printf("LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: DIGITO\n\n", nrolinea,palabra);
        else printf("LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea,palabra);
        break;
    case 5:
        if (scanner(palabra,situacion) == 1)
            printf("LINEA NRO.: %d.     LEXEMA:  %s .      TOKEN: CADENA DE CARACTERES\n\n", nrolinea,palabra);
        else printf("LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea,palabra);
        break;
    case 6:
        if(esPuntuacion(caracterLeido))
            printf("LINEA NRO.: %d.     LEXEMA:  %c .    TOKEN:    CARACTER DE PUNTUACION\n\n",nrolinea,caracterLeido);
        break;
    case 7:
        printf("LINEA NRO.: %d.     LEXEMA:  %c .    TOKEN: ASIGNACION\n\n",nrolinea,caracterLeido);
        break;
    }
}

void escribir_archivo(char caracterLeido, FILE* archivoIn, FILE* archivoOut, int situacion, int nrolinea, char *palabra, char *ruta){
    int caracterObtenido=1;
      switch(situacion){
      case 1:
          if(scanner(palabra,situacion) == 1)
              fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: COMENTARIO\n\n",nrolinea, palabra);
          else fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n",nrolinea, palabra);
          break;
      case 2:
          if(scanner(palabra,situacion) == 1 )
              fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: OPERADOR\n\n", nrolinea ,palabra);
          else fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea, palabra);
          break;
      case 3:
          if(scanner(palabra,situacion) == 1)
          {
              if(esPalabraReservada(palabra))
                  fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: PALABRA RESERVADA\n\n",nrolinea, palabra);
              else{
                  while (caracterObtenido){
                      if(siguienteCaracter(archivoIn,ruta)!=' ')
                          caracterObtenido=0;
                      else
                          caracterLeido=fgetc(archivoIn);
                  }

                  if (siguienteCaracter(archivoIn,ruta)=='(')
                      fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .   TOKEN: IDENTIFICADOR DE SUBPROGRAMA\n\n",nrolinea,palabra);
                  else
                      fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: IDENTIFICADOR\n\n",nrolinea,palabra);
              }
          }
          else fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea, palabra);
          break;
      case 4:
          if (scanner(palabra,situacion) == 1)
              fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .    TOKEN: DIGITO\n\n", nrolinea,palabra);
          else fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea,palabra);
          break;
      case 5:
          if (scanner(palabra,situacion) == 1)
              fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .      TOKEN: CADENA DE CARACTERES\n\n", nrolinea,palabra);
          else fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %s .      ERROR LEXICO\n\n", nrolinea,palabra);
          break;
      case 6:
          if(esPuntuacion(caracterLeido))
              fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %c .    TOKEN:    CARACTER DE PUNTUACION\n\n",nrolinea,caracterLeido);
          break;
      case 7:
          fprintf(archivoOut,"LINEA NRO.: %d.     LEXEMA:  %c .    TOKEN: ASIGNACION\n\n",nrolinea,caracterLeido);
          break;
      }
}


int main(int argc, char *argv[]){
    FILE *archivoIn;
    FILE *archivoOut;

    archivoIn=fopen(argv[1],"r");
    if (archivoIn==NULL)
    {
        printf("Error: no se pudo abrir el archivo de lectura.\n");
        return -1;
    }

     if(argc==3)
    {
        archivoOut = fopen(argv[2], "w");

        if (archivoOut==NULL)
        {
            printf("Error: no se pudo abrir el archivo de escritura.\n");
            return -1;
        }

    }
    char caracterLeido;
    char palabra[100];
    int nrolinea=1, situacion, caracterObtenido=1;
    int i;

 do{
  caracterLeido=fgetc(archivoIn);
  situacion = 0;

  if( caracterLeido == '/' && siguienteCaracter(archivoIn,argv[1])=='/' && caracterLeido!=EOF){
      situacion=1;
      for(i=0; caracterLeido!='\n';i++){
          palabra[i]=caracterLeido;
          caracterLeido=fgetc(archivoIn);
      }

      palabra[i]='\0';

  }
  else if(siguienteCaracter(archivoIn,argv[1])=='*' && caracterLeido!=EOF){
      situacion=1;
      for(i=0; caracterLeido!='\n';i++){
          palabra[i]=caracterLeido;
          caracterLeido=fgetc(archivoIn);

      }

      palabra[i]='\0';
  }
  else if (esOperador(caracterLeido)){
      situacion = 2;

      if(caracterLeido=='=' && siguienteCaracter(archivoIn,argv[1])=='='){
          for(i=0; caracterLeido!=' ' && caracterLeido!='\n';i++){
              palabra[i]=caracterLeido;
              caracterLeido=fgetc(archivoIn);
          }
      }
      else{
      for(i=0; caracterLeido!=' ' && caracterLeido!='\n' && esOperador(caracterLeido);i++){
          palabra[i]=caracterLeido;
          caracterLeido=fgetc(archivoIn);
          }
      }

      if(isalnum(caracterLeido))
          ungetc(caracterLeido,archivoIn);
      palabra[i]='\0';

  }else if (isalpha(caracterLeido) || caracterLeido=='_'){
      situacion = 3;
      for(i=0; caracterLeido!=EOF && caracterLeido!=' ' && caracterLeido!='\n' && !esPuntuacion(caracterLeido) && !operadores_sin_resta(caracterLeido) && caracterLeido!='=';i++){
          palabra[i]=caracterLeido;
          caracterLeido=fgetc(archivoIn);
      }
      if(esPuntuacion(caracterLeido) || esOperador(caracterLeido))
          ungetc(caracterLeido,archivoIn);

      palabra[i]='\0';

  } else if (isdigit(caracterLeido)){
      situacion = 4;
      caracterObtenido=1;
      i=0;
      do{
          palabra[i]=caracterLeido;
          caracterLeido=fgetc(archivoIn);
          if (esPuntuacion(caracterLeido)|| esOperador(caracterLeido) || caracterLeido=='=')
              caracterObtenido=0;
          i++;
      }while(caracterLeido!=' ' && caracterObtenido);

      if(ispunct(caracterLeido))
          ungetc(caracterLeido,archivoIn);
      palabra[i]='\0';

  } else if (caracterLeido=='"'){
      situacion=5;
      i=0;
      caracterObtenido=1;
      do{
          if(siguienteCaracter(archivoIn,argv[1])=='"')
              caracterObtenido=0;
          palabra[i]=caracterLeido;
          caracterLeido=fgetc(archivoIn);
          i++;
      }while(caracterObtenido);
      palabra[i]='"';
      palabra[i+1]='\0';
  } else if (esPuntuacion(caracterLeido)){
      situacion = 6;
  } else if (caracterLeido=='=')
          situacion = 7;

    if (argc==2)
        mostrar_por_pantalla(caracterLeido,archivoIn,situacion,nrolinea,palabra,argv[1]);
    if (argc==3)
        escribir_archivo(caracterLeido,archivoIn,archivoOut,situacion,nrolinea,palabra,argv[1]);

    if(caracterLeido=='\n')
        nrolinea++;

  }while(caracterLeido!=EOF);

  fclose(archivoIn);
  if (argc==3)
    fclose(archivoOut);
  return 0;
}

int esPuntuacion (char c){
    if (c==';' || c==':' || c=='.' || c=='{' || c=='}' || c=='(' || c==')' || c=='[' || c==']' || c==92 || c=='^' || c==',')
        return 1;
    return 0;
}

int esPalabraReservada (char *puntero){
    char *palabrasReservadas[]={"printf","return","if","for","while","do","int","char","double","switch","else","case","bool","break","const","cotinue","default","delete","double","sizeof","return","static","void"};
    int cantidadDePalabras=sizeof(palabrasReservadas)/sizeof(palabrasReservadas[0]),i;
    for (i=0;i<cantidadDePalabras;i++){
        if(!strcmp(puntero,palabrasReservadas[i]))
            return 1;
    }

    return 0;
    }

int operadores_sin_resta(char c){
    if (c=='*' || c=='+' || c=='/' || c=='<' || c=='>' || c=='!' || c=='|' || c=='&')
        return 1;
    return 0;
}

int esOperador (char c){
    if (c=='*' || c=='+' || c=='/' || c=='-' || c=='<' || c=='>' || c=='!' || c=='|' || c=='&')
        return 1;
    return 0;
}

int scanner (const char *palabraLeida,int situacion){
    int estadoActual=0, i=0;

    switch(situacion){
    case 1: //Comentario
        for (estadoActual=0,i=0; palabraLeida[i]!='\0' && estadoActual!=9;i++)
            estadoActual = tabla_comentario[estadoActual][columna_de_tabla_comentario(palabraLeida[i])];
        return estadoActual == 2 || estadoActual == 5 || estadoActual == 7 || estadoActual == 8;
    case 2: // Operador
       for (estadoActual=0,i=0; palabraLeida[i]!='\0' && estadoActual!=13;i++)
            estadoActual = tabla_operadores[estadoActual][columna_de_tabla_operadores(palabraLeida[i])];
        return estado_operadores(estadoActual);
    case 3: // Identificadores
       for (estadoActual=0,i=0; palabraLeida[i]!='\0' && estadoActual!=3;i++)
            estadoActual = tabla_identificadores[estadoActual][columna_de_tabla_identificadores(palabraLeida[i])];
        return estadoActual == 1 || estadoActual == 2;
    case 4: // Digitos
       for (estadoActual=0,i=0; palabraLeida[i]!='\0' && estadoActual!=2;i++)
            estadoActual = tabla_digitos[estadoActual][columna_de_tabla_digitos(palabraLeida[i])];
        return estadoActual == 1;
    case 5: // Cadena de caracteres
       for (estadoActual=0,i=0; palabraLeida[i]!='\0' && estadoActual!=3;i++){
            estadoActual = tabla_cadena[estadoActual][columna_de_cadena(palabraLeida[i])];
       }
        return estadoActual == 2;
    default:
        break;
    }

    return 0;
}

int columna_de_cadena(int c){
        if ( isalpha(c) ) return 0;
        if ( isdigit(c) ) return 1;
        if ( ispunct(c) && c!='"' ) return 2;
        if ( isspace(c) ) return 3;
        if ( c == '"') return 4;
    return 5;
}

int estado_operadores (int estadoActual){
    switch (estadoActual)
    {
        case 2: return 1; break;
        case 3: return 1; break;
        case 5: return 1; break;
        case 6: return 1; break;
        case 7: return 1; break;
        case 8: return 1; break;
        case 9: return 1; break;
        case 10: return 1; break;
        case 11: return 1; break;
        case 12: return 1; break;
        case 13: return 0; break;
    }
    return 0;
}

int columna_de_tabla_comentario(int c){
        if ( isalpha(c)) return 0;
        if ( isdigit(c)) return 1;
        if ( esPuntuacion(c)) return 2;
        if ( isspace(c) )  return 3;
        if ( c == '/')   return 4;
        if ( c == '\0' )    return 5;
        if ( c == '\n' )    return 6;
        if ( c == '*' )     return 7;
    return 8;
}

int columna_de_tabla_operadores(int c){
        if ( c == '|' ) return 0;
        if ( c == '+' ) return 1;
        if ( c == '-' ) return 2;
        if ( c == '*' ) return 3;
        if ( c == '/' ) return 4;
        if ( c == '=' ) return 5;
        if ( c == '>' ) return 6;
        if ( c == '<' ) return 7;
        if ( c == '&' ) return 8;
    return 9;
}

int columna_de_tabla_identificadores (int c){
        if( isalpha(c) ) return 0;
        if( isdigit(c) ) return 1;
        if( c=='_' )  return 2;
    return 3;

}

int columna_de_tabla_digitos(int c){
        if( isdigit(c) ) return 0;
    return 1;
}

char siguienteCaracter(FILE *puntero, char *ruta){
    char caracter;
    int posicion;
    posicion= ftell(puntero);
    FILE *archivoInAux = fopen(ruta,"r");

    fseek(archivoInAux,posicion,SEEK_SET);
    caracter=fgetc(archivoInAux);
    fclose (archivoInAux);
    return caracter;
}
