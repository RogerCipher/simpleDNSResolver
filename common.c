#include <stdlib.h>
#include <stdio.h>

void error(const char *msg) 
{
	//para apanhar mensagens de erro e dar exit do programa
    perror(msg);
    exit(1);
}

void limparEcra()
{
    //dar clear da console para unix based systems
    printf("\033[H\033[J");
}

void mostrarBanner()
{
    printf("\n----------------------------------");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("*       ___  _  ______                        ");
    printf("\n");
    printf("*      / _ \\/ |/ / __/ ___ ___ _____ ______ __");
    printf("\n");
    printf("*     / // /    /\\ \\  / _ `/ // / -_) __/ // /");
    printf("\n");
    printf("*    /____/_/|_/___/  \\_, /\\_,_/\\__/_/  \\_, / ");
    printf("\n");
    printf("*                      /_/             /___/  ");
    printf("\n");
    printf("* \n");
}