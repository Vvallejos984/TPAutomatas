#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

struct nodoNum{
    int val;
    struct nodoNum *sig;
};

struct nodoString{
    char val[30];
    struct nodoString *sig;
};
typedef struct cadenaNum{
    char val[30];
    int tipoNum; //0 dec, 1 oct, 2 hex
}cadenaNum;

typedef struct colaNumeros{
    struct cadenaNum val;
    struct colaNumeros *sig;
}colaNumeros;

char terminalesDecOctHex[6][17]={
    "-",
    "0",
    "1234567",
    "89",
    "ABCDEFabcdef",
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

struct colaNumeros *headColaNumerosVarios;
struct colaNumeros *tailColaNumerosVarios;
//Estructuras para validación y cola de expresión inicial
struct nodoNum *headPilaNum = NULL;

struct nodoString *headPilaExp = NULL;

//Estructuras para conversión de expresión
struct nodoString *headPilaPostfix = NULL;

struct nodoString *headColaNum = NULL;
struct nodoString *tailColaNum = NULL;

struct nodoString *headPilaOper = NULL;

int potenciar(int, int);
void pushPilaOper(char[]);
char * popPilaOper();
int cadenaDecADec(char*);
int cadenaOctADec(char*);
int cadenaHexADec(char*);
int cadenaANum(cadenaNum);
int simboloCoincide(char, char[]);
int validarCadena(char[]);
int validarMultipleCadena(char[]);
void separarPorTerminos(char[]);
void pushPilaExp(char*);
char * popPilaExp();
void pushPilaPostfix(char*);
char * popPilaPostfix();
void pushColaNum(char*);
char * popColaNum();
void vaciarColaNum();
void vaciarPilaOper();
void vaciarPilaPostfix();
void infijaAPostfix();

const char* leerArchivo();

void validarCadenaNumeros(char *);
void invertirPilaPostfix();

int mult();
int sum();
int rest();

int darValor();

//gcc pilas.c -o pilas && pilas.exe
//-----------------------------------------
int main() {
//    123+41-4*3542+41436+10 -> 27442
//    0173+0x29-4*06726+0xA1DC+10 -> 27442 (correcto)
//    Resultado que tira = -55450 (mal) seguramente es por el tema de la procedencia de opereadores que dijiste

//    12&0x12p4C&0x43n5A&1&47&02312
    char opcion1;
    char opcion2;
    char cadena[100] = "";

    system("cls");
    printf("Quiero ingresar la expresion mediante: \n");
    printf("1. Archivo de texto.\n");
    printf("2. Consola.\n");
    printf("3. No quiero resolver una expresion solo quiero contar numeros.\n");

    scanf(" %c", &opcion1);

    switch (opcion1) {
        case '1':
            system("cls");
//            printf("Elegiste ingresar la expresion por archivo de texto.\n");
            const char *res = leerArchivo();
            strcpy(cadena, res);
            break;
        case '2':
            system("cls");
            printf("Elegiste ingresar la expresion por consola: \n");
            scanf("%s", cadena);
            break;
        case '3':
            system("cls");
            printf("Quiero contar numeros desde: \n");
            printf("1. Archivo de texto.\n");
            printf("2. Consola.\n");
            scanf(" %c", &opcion2);
            switch (opcion2) {
                case '1':
                    system("cls");
//                    printf("Elegiste ingresar numeros por archivo de texto.\n");
                    const char *resp = leerArchivo();
                    strcpy(cadena, resp);
                    break;
                case '2':
                    system("cls");
                    printf("Elegiste ingresar numeros por consola: \n");
                    scanf("%s", cadena);
                    break;
                default:
                    printf("No se elegio ninguno\n");
                    break;
            }
            break;
        default:
            printf("NO SE ELIGIO METODO DE INGRESO");
            break;
    }
    system("cls");
    if(opcion1 == '3'){
        validarCadenaNumeros(cadena);
    }else{
        separarPorTerminos(cadena);
        infijaAPostfix();
        printf("\nResultado:\n\n%s = %d\n", cadena, darValor());
    }
}
//-----------------------------------------

void validarCadenaNumeros(char * cadena){
    system("cls");
    printf("Cadena recibida: %s\n\n", cadena);
    int i = 0;
    int j = 0;

    int decimales = 0;
    int octales = 0;
    int hexadecimales = 0;

    char acumulador[30] = "";
    int tipos[100];
    long sizeChar = strlen(cadena);
    while(sizeChar + 1 > i){
        if(cadena[i] != '&' && cadena[i] != '\0' && cadena[i] != ' ') {
            strncat(acumulador, &cadena[i], 1);

        } else {
            tipos[j] = validarCadena(acumulador);
            if(tipos[j] == 0){
                printf("Cadena invalida encontrada, se detuvo el conteo de numeros.\n");
                exit(-1);
            }
            pushPilaExp(acumulador);
            strcpy(acumulador, "");
            j++;
        }
        i++;
    }
    i = 0;

    struct nodoString *nuevo = headPilaExp;
    while (nuevo != NULL){
        switch (tipos[i]) {
            case 1: decimales++;
                break;
            case 2: octales++;
                break;
            case 3: hexadecimales++;
                break;
            default:
                break;
        }
        nuevo = nuevo->sig;
        i++;
    }
    char pluralDec[] = "es";
    char pluralOct[] = "es";
    char pluralHex[] = "es";

    if(!(decimales-1))
        strcpy(pluralDec,"");
    if(!(octales-1))
        strcpy(pluralOct,"");
    if(!(hexadecimales-1))
        strcpy(pluralHex,"");

    printf("\nLa expresion ingresada tiene: \n\n");
    printf("%d decimal%s\n", decimales, pluralDec);
    printf("%d octal%s\n", octales, pluralOct);
    printf("%d hexadecimal%s\n", hexadecimales, pluralHex);
}


const char* leerArchivo(){
    FILE* archivo;
    char cwd[100];
    getcwd(cwd, sizeof(cwd));

    char a;
    static char expresiones[] = "";
    char finalDir[] = "";

    int lastBarra = 0;
    int length = strlen(cwd);

    for (int i = 0; i < length-1; i++) {
        if(cwd[i] == '/' && cwd[i]){
            lastBarra = i;
        }
    }

    for (int i = 0; i < lastBarra; ++i) {
        strncat(finalDir, &cwd[i], 1);
    }

    archivo = fopen(strcat(finalDir, "/expresiones.txt"), "r");

    if(archivo == NULL){
        printf("[ERROR] EL ARCHIVO NO SE PUEDE ABRIR.\n");
        return 0;
    }

    do{
        a = fgetc(archivo);
        if(a != EOF){
            strncat(expresiones, &a, 1);
        }
    } while (a != EOF);

    fclose(archivo);
    printf("Expresion recuperada del archivo de texto: %s\n", expresiones);

    return expresiones;
}

int potenciar(int base, int exp){
    int res=1;
    for(exp;exp>0;exp--){
        res*=base;
    }
    return res;
}

void pushPilaNum(int val){
    struct nodoNum *ptr = (struct nodoNum *)malloc(sizeof(struct nodoNum));

    if (headPilaNum == NULL){
        ptr->val = val;
        ptr->sig = NULL;
        headPilaNum = ptr;
    }
    else{
        ptr->val = val;
        ptr->sig = headPilaNum;
        headPilaNum = ptr;
    }
}

int popPilaNum(){
    int item;
    struct nodoNum *ptr;
    if (headPilaNum != NULL){
        item = headPilaNum->val;
        ptr = headPilaNum;
        headPilaNum = headPilaNum->sig;
        free(ptr);
        return item;
    }
    return 0;
}

void pushPilaOper(char val[]){
    struct nodoString *ptr = (struct nodoString *)malloc(sizeof(struct nodoString));

    if (headPilaOper == NULL)
    {
        strcpy(ptr->val, val);
        ptr->sig = NULL;
        headPilaOper = ptr;
    }
    else
    {
        strcpy(ptr->val, val);
        ptr->sig = headPilaOper;
        headPilaOper = ptr;
    }
}

char *popPilaOper(){
    static char item[100]="";
    struct nodoString *ptr;
    if (headPilaOper != NULL)
    {
        strcpy(item, headPilaOper->val);
        ptr = headPilaOper;
        headPilaOper = headPilaOper->sig;
        free(ptr);
        return item;
    }
    return "";
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
        else{
            if(cadena[i]-'a'<0)
                pushPilaNum(cadena[i]-'A'+10);//017-0x2a+55
            else
                pushPilaNum(cadena[i]-'a'+10);
        }
                     
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
        case 0: printf("Cadena invalida encontrada, se detuvo el proceso\n");
            exit(-1);
        case 1: return cadenaDecADec(cadena.val);
        case 2: return cadenaOctADec(cadena.val);
        case 3: return cadenaHexADec(cadena.val);
    }
}

int simboloCoincide(char input, char carac[]){
    int i = 0;
    while(carac[i]!='\0'){
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
            printf("Cadena '%s' valida - ", cadena);
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
        for(int i=0; i<6; i++){
            if(simboloCoincide(cadena[caracter], terminalesDecOctHex[i])){
                estado = estadosAutomata[estado][i];
                caracterValido = 1;
                break;
            }
        }
        if(!caracterValido){
            estado=7;
        }

        else{
            caracter+=1;
        }
        
    }
    if(estado==7){
        printf("Cadena '%s' invalida\n",cadena);
    }
        
    return tipo;
}

void pushPilaExp(char *val){
    struct nodoString *ptr = (struct nodoString *)malloc(sizeof(struct nodoString));

    if (headPilaExp == NULL)
    {
        strcpy(ptr->val, val);
        ptr->sig = NULL;
        headPilaExp = ptr;
    }
    else
    {
        strcpy(ptr->val, val);
        ptr->sig = headPilaExp;
        headPilaExp = ptr;
    }
}

char *popPilaExp(){
    static char item[100]="";
    struct nodoString *ptr;
    if (headPilaExp != NULL)
    {
        strcpy(item, headPilaExp->val);
        ptr = headPilaExp;
        headPilaExp = headPilaExp->sig;
        free(ptr);
        return item;
    }
    return "";
}

void pushColaNum(char *x){
    struct nodoString *nuevo;
    nuevo=malloc(sizeof(struct nodoString));
    strcpy(nuevo->val, x);
    nuevo->sig = NULL;
    if (headColaNum == NULL){
        headColaNum = nuevo;
        tailColaNum = nuevo;
    }
    else{
        tailColaNum->sig = nuevo;
        tailColaNum = nuevo;
    }
    
}

char *popColaNum(){
    static char item[]="";
    if (headColaNum != NULL)
    {
        strcpy(item,headColaNum->val);
        headColaNum = headColaNum->sig;
        return item;
    }
    return "0";
}

void pushPilaPostfix(char *val){
    struct nodoString *ptr = (struct nodoString *)malloc(sizeof(struct nodoString));

    if (headPilaPostfix == NULL)
    {
        strcpy(ptr->val, val);
        ptr->sig = NULL;
        headPilaPostfix = ptr;
    }
    else
    {
        strcpy(ptr->val, val);
        ptr->sig = headPilaPostfix;
        headPilaPostfix = ptr;
    }
}

char *popPilaPostfix(){
    static char item[100]="";
    struct nodoString *ptr;
    if (headPilaPostfix != NULL)
    {
        strcpy(item, headPilaPostfix->val);
        ptr = headPilaPostfix;
        headPilaPostfix = headPilaPostfix->sig;
        free(ptr);
        return item;
    }
    return "";
}

// 1. Convertir todos los numeros de X a decimal, separando por terminos (+, -, *)
void separarPorTerminos(char cadena[]){
    int i = 0;
    char acumulador[30] = "";

    long sizeChar = strlen(cadena);
    while(sizeChar + 1 > i){
        if(cadena[i] != '+' && cadena[i] != '-' && cadena[i] != '*' && cadena[i] != '&' && cadena[i] != '\0' && cadena[i] != ' ') {


            strncat(acumulador, &cadena[i], 1);

        } else{
            char stringResult[30];

            cadenaNum cadenaNum1 = {
                    .tipoNum=validarCadena(acumulador)
            };

            strcpy(cadenaNum1.val, acumulador);

            int decimalResult = cadenaANum(cadenaNum1);

//            Convierte de int a string
            sprintf(stringResult, "%d", decimalResult);

//            agregar a la cola lo acumulado de la expresion strings de decimales
            pushPilaExp(stringResult);

//            Vaciar acumulador y agregar operador a la cola
            strcpy(acumulador, "");
            if(cadena[i] == '+' || cadena[i] == '-' || cadena[i] == '*'){
                strncat(acumulador, &cadena[i], 1);
                pushPilaExp(acumulador);
                strcpy(acumulador, "");
            }  
        }
        i++;
    }
}

void vaciarColaNum(){
    while(headColaNum!=NULL){
        pushPilaPostfix(popColaNum());
    }
}

void vaciarPilaOper(){

    while(headPilaOper!=NULL){
        pushPilaPostfix(popPilaOper());
    }
}

void infijaAPostfix(){
    int i = 0;
    while(headPilaExp!=NULL){
        if(i%2){
            pushPilaOper(popPilaExp());

            if(!strcmp(headPilaOper->val,"+") || !strcmp(headPilaOper->val,"-")){
                vaciarPilaOper();
                vaciarColaNum();
            }
        }else{
            pushColaNum(popPilaExp());
        }
        i++;
    }
    vaciarPilaOper();
    vaciarColaNum();
    invertirPilaPostfix();
}

void invertirPilaPostfix(){
    while(headPilaPostfix!=NULL){
        pushPilaExp(popPilaPostfix());
    }
    headPilaPostfix=headPilaExp; 
}

int mult(){
    int b = darValor();
    int a = darValor();

    return a*b;
}

int sum(){
    int b = darValor();
    int a = darValor();

    return a+b;
}

int rest(){
    int b = darValor();
    int a = darValor();

    return a-b;
}

int darValor(){
    char * dato = "";
    dato = popPilaPostfix();
    if(isdigit(dato[0]) || isdigit(dato[1]))
        return cadenaDecADec(dato);
    else{
        switch(dato[0]){
            case '+': return sum(); break;
            case '-': return rest(); break;
            case '*': return mult(); break;
        }
    }

}