#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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

typedef struct cadenaNum{
    char val[100];
    int tipoNum; //0 dec, 1 oct, 2 hex
}cadenaNum;

typedef struct cadenaNum{

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

int potenciar(int, int);
void pushPilaNumChar(char[]);
char * popPilaNumChar();
int cadenaDecADec(char*);
int cadenaOctADec(char*);
int cadenaHexADec(char*);
int cadenaANum(cadenaNum);
int simboloCoincide(char, char[]);
int validarCadena(char[]);
int validarMultipleCadena(char[]);
void separarPorTerminos(char[]);

//gcc pilas.c -o pilas && pilas.exe
//-----------------------------------------
int main(){
    cadenaNum cadena;
    scanf("%s", cadena.val);
    printf("\nCadena: %s", cadena.val);
    cadena.tipoNum = validarCadena(cadena.val);



    
    getchar();
    getchar();
    system("cls");
    int lel = cadenaANum(cadena);
    printf("Resultado: %d\n", lel);
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
    //printf("Pushed %d\n", headPilaNum->val); //Debug message
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

int cadenaDecADec(char cadena[]){
    int i = 0,
    res = 0,
    size = 0,
    sgn = 1;

    if(cadena[0]=='-'){
        i = 1;
        sgn = (-1);
    }
    while(cadena[i]!='\0'){
        pushPilaNum(cadena[i]-'0');
        i+=1;
        size+=1;
    }
    for(i=0; i<size; i++){
        res += popPilaNum()*potenciar(10,i);
    }
    return res*sgn;
}

int cadenaOctADec(char cadena[]){
    int i = 1,
    res = 0,
    size = 0;

    while(cadena[i]!='\0'){
        pushPilaNum(cadena[i]-'0');
        i+=1;
        size+=1;
    }
    for(i=0; i<size; i++){
        res += popPilaNum()*potenciar(8,i);
    }
    return res;
}

int cadenaHexADec(char cadena[]){
    int i = 2,
    res = 0,
    size = 0;

    while(cadena[i]!='\0'){
        if(isdigit(cadena[i]))
            pushPilaNum(cadena[i]-'0');
        else
            pushPilaNum(cadena[i]-'A'+10);
        i+=1;
        size+=1;
    }
    for(i=0; i<size; i++){
        res += popPilaNum()*potenciar(16,i);
    }
    return res;
}

int cadenaANum(cadenaNum cadena){
    switch(cadena.tipoNum){
        case 0: printf("Cadena invalida\n"); return 0;
        case 1: return cadenaDecADec(cadena.val);
        case 2: return cadenaOctADec(cadena.val);
        case 3: return cadenaHexADec(cadena.val);
    }
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
    int tipo = 0;
    int estado = 0;
    int caracter = 0;
    while(estado!=7){
        if(cadena[caracter]=='\0' && caracter!=0){
            printf("Cadena '%s' valida\n", cadena);
            switch(estado){
                case 1: tipo=1; printf("Es decimal\n"); break;
                case 3: tipo=2; printf("Es octal\n"); break;
                case 5: tipo=3; printf("Es hexadecimal\n"); break;
                default: tipo = 0; 
            }
            break;
        }
        if(cadena[0]=='\0'){
            estado=7;
            break;
        } 
        int caracterValido = 0;
        printf("Estado actual: %d\n\n", estado); //Debug message
        for(int i=0; i<6; i++){
            printf("Mirando columna %d\n", i); //Debug message
            if(simboloCoincide(cadena[caracter], terminalesDecOctHex[i])){
                estado = estadosAutomata[estado][i];
                printf("Cambio a estado %d\n", estado);
                caracterValido = 1;
                break;
            }
        }
        if(!caracterValido){
            estado=7;
            printf("Cambio a estado %d\n", estado); //Debug message
        }

        else{
            printf("Caracter '%c' OK\n\n", cadena[caracter]); //Debug message
            caracter+=1;
        }
        
    }
    if(estado==7){
        printf("Cadena '%s' invalida",cadena);
    }
        
    return tipo;
}

// 1. Convertir todos los numeros de X a decimal, separando por terminos (+, -, *)
void separarPorTerminos(char cadena[]){
    int i = 0;
    char acumulador[100];

    while (cadena[i] != '\0'){
        if(cadena[i] != '+' || cadena[i] != '-' || cadena[i] != '*' || cadena[i] != '&' || cadena[i] != '\0') {
//            acumulador[i] = cadena[i];
            strcat(acumulador, cadena[i]);
//
        } else{

            cadenaNum cadenaNum1 = {
                    .tipoNum=validarCadena(acumulador)
            };

            strcpy(cadenaNum1.val, acumulador);

            int decimalResult = cadenaANum(cadenaNum1);

            char stringResult[100];

            sprintf(stringResult, "%d", decimalResult);

            printf("Resultado %s\n", stringResult);

//            agregar a la cola lo acumulado de la expresion strings de decimales

            strcpy(acumulador, "");
        }
        i++;
    }
}

// Crear 2 colas para numeros y para notacion polaca y Pila para operadores
// Sacar elementos de la primera cola
// 2. Convertirlos a string para ponerlos en la cola (numeros)
// 3. Agregar los operadores en una pila
// 4. Descomponer la cola y operar