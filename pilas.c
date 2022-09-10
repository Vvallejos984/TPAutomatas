#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int potenciar(int, int);
void pushPilaNumChar(char[]);
char * popPilaNumChar();
int cadenaANum(char*, int);
int simboloCoincide(char, char[]);
int validarCadena(char[]);

struct nodoPilaNum{
    int val;
    struct nodoPilaNum *sig;
};

struct nodoPilaNumChar{
    char val[30];
    struct nodoPilaNumChar *sig;
};

struct nodoPilaOperador{
    char val[2];
    struct nodoPilaOperador *sig;
};

char octales[] = "1234567";
char hexadecimales[] = "123456789ABCDEF";

char terminalesEntero[3][10]={"-","0","123456789"};

int estadosEntero[3][3]={
    {1,3,2},
    {3,3,2},
    {3,2,2}
};

char terminalesDecOctHex[6][17]={
    "-",
    "0",
    "1234567",
    "89",
    "ABCDEF",
    "x"
};

int estadosAutomata[7][6]={
    {6,2,1,1,7,7},
    {7,1,1,1,7,7},
    {7,7,3,7,7,4},
    {7,3,3,7,7,7},
    {7,7,5,5,5,7},
    {7,5,5,5,5,7},
    {7,7,1,1,7,7},
};

struct nodoPilaNum *headPilaNum;
struct nodoPilaNumChar *headPilaNumChar;
struct nodoPilaOperador *headPilaOperador;
//gcc pilas.c -o pilas && pilas.exe
//-----------------------------------------
int main(){
    char cadena[] = "0x135AFCE42AF";
    //printf("%d", cadenaANum(cadena, sizeof(cadena)));
    validarCadena(cadena);
}
//-----------------------------------------

int potenciar(int base, int exp){
    int res=1;
    for(exp;exp>0;exp--){
        res*=base;
    }
    return res;
}

void pushPilaNum(int val){
    struct nodoPilaNum *ptr = (struct nodoPilaNum *)malloc(sizeof(struct nodoPilaNum));

    if (headPilaNum == NULL)
    {
        ptr->val = val;
        ptr->sig = NULL;
        headPilaNum = ptr;
    }
    else
    {
        ptr->val = val;
        ptr->sig = headPilaNum;
        headPilaNum = ptr;
    }
}

int popPilaNum(){
    int item;
    struct nodoPilaNum *ptr;
    if (headPilaNum != NULL)
    {
        item = headPilaNum->val;
        ptr = headPilaNum;
        headPilaNum = headPilaNum->sig;
        free(ptr);
        return item;
    }
    return 0;
}

void pushPilaNumChar(char val[]){
    struct nodoPilaNumChar *ptr = (struct nodoPilaNumChar *)malloc(sizeof(struct nodoPilaNumChar));

    if (headPilaNumChar == NULL)
    {
        strcpy(ptr->val, val);
        ptr->sig = NULL;
        headPilaNumChar = ptr;
    }
    else
    {
        strcpy(ptr->val, val);
        ptr->sig = headPilaNumChar;
        headPilaNumChar = ptr;
    }
}

char *popPilaNumChar(){
    static char item[]="";
    struct nodoPilaNumChar *ptr;
    if (headPilaNumChar != NULL)
    {
        strcpy(item, headPilaNumChar->val);
        ptr = headPilaNumChar;
        headPilaNumChar = headPilaNumChar->sig;
        free(ptr);
        return item;
    }
    return 0;
}

int cadenaANum(char cadena[], int size){
    int res=0;
    for(int i=0; i<size-1; i++){
        pushPilaNum(cadena[i]-'0');
    }
    for(int i=0; i<size-1; i++){
        res += popPilaNum()*potenciar(10,i);
    }
    return res;
}

int simboloCoincide(char input, char carac[]){
    int i = 0;
    while(carac[i]!='\0'){
        printf("%c == %c\n",input,carac[i]);
        if(input == carac[i])
            return 1;
        i+=1;
    }
    return 0;
}

int validarCadena(char cadena[]){
    int estado = 0;
    int caracter = 0;
    while(estado!=7){
        if(cadena[caracter]=='\0'){
            printf("Cadena '%s' valida", cadena);
            return 1;
        }
        int caracterValido = 0;
        printf("Estado actual: %d\n\n", estado);
        for(int i=0; i<6; i++){
            printf("Mirando columna %d\n", i);
            if(simboloCoincide(cadena[caracter], terminalesDecOctHex[i])){
                estado = estadosAutomata[estado][i];
                printf("Cambio a estado %d\n", estado);
                caracterValido = 1;
                break;
            }
        }
        if(!caracterValido){
            estado=7;
            printf("Cambio a estado %d\n", estado);
        }

        else{
            printf("Caracter '%c' OK\n\n", cadena[caracter]);
            caracter+=1;
        }
        
        
    }
    printf("Cadena '%s' invalida",cadena);
    return 0;
}