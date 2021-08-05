#include <stdio.h>
#include <stdlib.h>

#include "intList.h"
#include "common.h"
/*-- Funcoes para tratar da linked list de inteiros--*/

//criar uma Qname List
TipoIntList *criarList()
{
    TipoIntList *asciiList = (TipoIntList *)malloc(sizeof(TipoIntList));
    asciiList->actual = NULL;
    asciiList->cabeca = NULL;
    asciiList->iter = 0;
    asciiList->comprimento = 0;
    return asciiList;
}

//dar reset do iterador
int resetIterPos(TipoIntList *list)
{
    list->actual = list->cabeca;
    list->iter = 0;
    return 1;
}

//retorna 0 se nao tiver, 1 se tiver
int temProximo(TipoASCIINumber *num)
{
    if(num->proximo == NULL)
    {
        return 0;
    }
    return 1;
}
//retorna 0 se nao tiver, 1 se tiver
int temAnterior(TipoASCIINumber *num)
{
    if(num->anterior == NULL)
    {
        return 0;
    }
    return 1;
}
//retorna 0 se nao tiver anterior
int moverIterParaAnterior(TipoIntList *list)
{
    if(temAnterior(list->actual))
    {
        list->actual = list->actual->anterior;
        list->iter --;
        return 1;
    }
    return 0;
}
//retorna 0 nao tiver proximo
int moverIterParaProximo(TipoIntList *list)
{
    if(temProximo(list->actual))
    {
        list->actual = list->actual->proximo;
        list->iter ++;
        return 1;
    }
    return 0;
}
//cria um elemento TipoASCIINumber a partir de um valor inteiro
TipoASCIINumber *criarASCII(int valor)
{
    TipoASCIINumber *asciiNum = (TipoASCIINumber *)malloc(sizeof(TipoASCIINumber));
    asciiNum->value = valor;
    return asciiNum;
}
//adiciona um valor no final da lista
int adicionarValorFinal(TipoIntList *list, int valor)
{
    TipoASCIINumber *asciiNumTemp = criarASCII(valor);  
    //ver se ainda n existe nada:
    if(list->cabeca == NULL)
    {
        //se nao ha cabeça vamos criar
        asciiNumTemp->anterior = NULL;
        asciiNumTemp->proximo = NULL;
        list->cabeca = asciiNumTemp;
        list->iter = 0;
        list->comprimento ++;
        return 1;
    }
    else
    {
        resetIterPos(list);
        //ir para o final da lista:
        for(int i = 0; i < list->comprimento; i++)
        {
            moverIterParaProximo(list);
        }
        //adicionar um elemento no final da lista
        list->actual->proximo = asciiNumTemp;
        asciiNumTemp->proximo = NULL;
        asciiNumTemp->anterior = list->actual;
        list->comprimento++;
        resetIterPos(list);

        return 1;
    }

}

//adiciona um elemento onde está o iter
int adicionarValor(TipoIntList *list, int valor)
{
    if(list->iter == 0)
    {
        TipoASCIINumber *asciiNumTemp = criarASCII(valor);
        //estamos na head
        if(list->actual == NULL)
        {
            //n ha head, vamos criar
            list->cabeca = asciiNumTemp;
            list->cabeca->anterior = NULL;
            list->cabeca->proximo = NULL;
            list->actual = list->cabeca;
            list->comprimento++;
        }
        else
        {
            TipoASCIINumber *asciiNumTemp = criarASCII(valor);
            //a head existe, vamos trocala:
            //movemos a head para a frente 
            //e metemos este elemento como head, ligando á head antiga como proximo
            asciiNumTemp->proximo = list->actual;
            asciiNumTemp->anterior = NULL;
            list->actual->anterior = asciiNumTemp;
            list->cabeca = asciiNumTemp;
            list->actual = asciiNumTemp;
            list->comprimento++;
        }

    }
    else if(temAnterior(list->actual))
    {
        TipoASCIINumber *asciiNumTemp = criarASCII(valor);
        //estamos num elemento que tem anterior e proximo,
        //vamos ligar o elemento anterior a este,
        //vamos ligar o proximo elemento a este:
        asciiNumTemp->proximo = list->actual;
        asciiNumTemp->anterior = list->actual->anterior;
        list->actual->anterior->proximo = asciiNumTemp;
        list->actual->anterior = asciiNumTemp;
        list->comprimento++;
    }
    resetIterPos(list);
    return 1;
}

//return 0 se der erro, 1 se for possivel, move o iterador para a posicao "pos"
int moverIter(TipoIntList *list, int pos)
{
    if(pos == list->iter)
    {
        //ja ca estamos
        return 1;
    }
    else if(pos > list->iter)
    {
        //temos que mandar o iter para a frente
        int dif = pos - list->iter;
        for(int i=0; i< dif; i++)
        {
            moverIterParaProximo(list);
        }
        return 1;
    }
    else if(pos < list->iter)
    {
        //temos que mandar o iter para a frente
        int dif = list->iter - pos;
        for(int i=0; i< dif; i++)
        {
            moverIterParaAnterior(list);
        }
        return 1;
    }
    return 0;
}

//funcao para adicionar um elemento com o valor value na posicao pos da linked list
void adicionarEm(TipoIntList *list, int pos, int value)
{
    if(pos < list->comprimento)
    {
        moverIter(list, pos);
        adicionarValor(list, value);
    }
    else if(pos == list->comprimento)
    {
        adicionarValorFinal(list, value);
    }
    else
    {
        error("\nnao é possivel adicionar um valor na lista numa posicao maior que o comprimento da lista");
    }
}

//serve apenas para debugging
void imprimirLista(TipoIntList *list)
{
    printf("\n Lista: \n");

    resetIterPos(list);

    for(int i = 0; i < list->comprimento; i++)
    {
        printf("|%d",list->actual->value);
        moverIterParaProximo(list);
    }
    printf("\n");
}

//da return do tamanho actual da lista
int tamanhoLista(TipoIntList *list)
{
    return list->comprimento;
}

//da return do valor de uma posicao da lista
int valorDaPosicao(TipoIntList *list, int pos)
{
    moverIter(list, pos);
    return list->actual->value;
}

//da free de todos os elementos da lista, e depois finalmente da free da intList
void free_list(TipoIntList *list)
{
    int tamanhoOriginal = tamanhoLista(list);

    for(int i = 0; i < tamanhoOriginal; i++)
    {
        //aqui vamos dar free de cada elemento da lista 1 a 1
        moverIter(list, tamanhoOriginal -i -1);
        free(list->actual->proximo);
        list->comprimento --;
    }
    //aqui vamos dar free da lista em si agora que ja nao tem elementos
    free(list);
}