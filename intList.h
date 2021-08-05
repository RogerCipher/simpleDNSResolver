#ifndef __INTLIST_H__
#define __INTLIST_H__

//estrutura para guardar os valores de qName
typedef struct TipoAN
{
    struct TipoAN *anterior;
    struct TipoAN *proximo;
    int value;
}TipoASCIINumber;

//estrutura para iterar pelo qName
typedef struct 
{
    int iter;
    int comprimento;
    TipoASCIINumber *cabeca;
    TipoASCIINumber *actual;
}TipoIntList;

TipoIntList *criarList();
int resetIterPos(TipoIntList *list);
int temProximo(TipoASCIINumber *num);
int temAnterior(TipoASCIINumber *num);

int moverIterParaProximo(TipoIntList *list);
int moverIterParaAnterior(TipoIntList *list);

TipoASCIINumber *criarASCII(int valor);
int adicionarValorFinal(TipoIntList *list, int valor);
int adicionarValor(TipoIntList *list, int valor);
int moverIter(TipoIntList *list, int pos);
void adicionarEm(TipoIntList *list, int pos, int value);
void imprimirLista(TipoIntList *list);
int tamanhoLista(TipoIntList *list);
int valorDaPosicao(TipoIntList *list, int pos);
void free_list(TipoIntList *list);

#endif