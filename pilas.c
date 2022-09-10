#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int potenciar(int, int);
void pushPilaChar(char*);
const char * popPilaChar();
int cadenaANum(char*, int);
int simboloCoincide(char, char[]);
int validarCadena(char[]);

struct nodoPilaNum{
    int val;
    struct nodoPilaNum *sig;
};

struct nodoPilaChar{
    char val[30];
    struct nodoPilaChar *sig;
};

char octales[] = "1234567";
char hexadecimales[] = "123456789ABCDEF";

char terminalesEntero[3][10]={"-","0","123456789"};

int estadosEntero[4][3]={
    {2,4,3},
    {4,4,3},
    {4,3,3},
    {4,4,4}
};

struct nodoPilaNum *headPilaNum;
struct nodoPilaChar *headPilaChar;
//gcc pilas.c -o pilas && pilas.exe
//-----------------------------------------
int main(){
    char cadena[] = "-33482";
    /*char * roberto = "roberto";
    char * juancarlos = "juancarlos";
    //printf("%d", cadenaANum(cadena, sizeof(cadena)));
    pushPilaChar(roberto);
    pushPilaChar(juancarlos);
    printf("%s \n",popPilaChar());
    printf("%s \n",popPilaChar());*/
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

void pushPilaChar(char *val){
    struct nodoPilaChar *ptr = (struct nodoPilaChar *)malloc(sizeof(struct nodoPilaChar));

    if (headPilaChar == NULL)
    {
        strcpy(ptr->val, val);
        ptr->sig = NULL;
        headPilaChar = ptr;
    }
    else
    {
        strcpy(ptr->val, val);
        ptr->sig = headPilaChar;
        headPilaChar = ptr;
    }
}

const char *popPilaChar(){
    char *item;
    struct nodoPilaChar *ptr;
    if (headPilaChar != NULL)
    {
        strcpy(item, headPilaChar->val);
        ptr = headPilaChar;
        headPilaChar = headPilaChar->sig;
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
    int estado = 1;
    int caracter = 0;
    while(estado!=4){
        if(cadena[caracter]=='\0'){
            printf("Cadena valida");
            return 1;
        }
        int caracterValido = 0;
        for(int i=0; i<3; i++){
            printf("Mirando columna %d\n", i);
            caracterValido += simboloCoincide(cadena[caracter], terminalesEntero[i]);
            if(caracterValido>0)
                break;
        }
        if(caracterValido==0)
            estado=4;
        else{
            printf("Caracter '%c' OK\n\n", cadena[caracter]);
            caracter+=1;
        }
        
        
    }
    printf("Caracter invalido %c",cadena[caracter]);
    return 0;
}