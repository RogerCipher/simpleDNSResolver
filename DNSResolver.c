#include <stdio.h> //para printf() e getch()
#include <stdlib.h> //para atoi() e exit()
#include <string.h> //para memset(), strlen() e strcpy()
#include <unistd.h> //para close()
#include <sys/types.h> //para sockaddr_in
#include <sys/socket.h> //para socket(), bind(), connect(), recv(), send()
#include <netdb.h> //para INADDR_ANY
#include <ctype.h> //para toupper()
#include <arpa/inet.h> //para inet_ntoa()
#include <signal.h> //para sigaction() (apanhar o ctrl + c)
//#include <netinet/in.h> //para
#include "intList.h" //uma linked list de ints para guardar o header e a pergunta do query
#include "dnsStuff.h" //para as estruturas que teem a ver com DNS
#include "common.h"


int main(int argc, char *argv[]) 
{

    //ver se foram passados 2 argumentos
    if (argc != 3) {
        printf("Usage: %s <dns-server> <hostname>\n", argv[0]);
        exit(1);
    }
    
    //inicializacao
    TipoHeader *header = NULL; //pointer para o header de DNS
    TipoPergunta *pergunta = (TipoPergunta *)malloc(sizeof(TipoPergunta)); //pointer para a pergunta DNS
    



    //criar uma lista para meter a mensagem toda
    TipoIntList *intList = criarList();
    unsigned char *tempBuffer0 = (unsigned char *)malloc(sizeof(TipoHeader) * sizeof(char)); //este buffer vai guardar o header em array

    header = (TipoHeader *)tempBuffer0; //ligar o header ao tempBuffer

    //vamos popular o header
    header->xid = 82; //numero id a toa
    header->flags = htons(0x100); //flags -> 0x100 recursion enabled, o resto das flags disabled
    header->qdcount = htons(1); //nr de perguntas


    //vamos meter o header para dentro da lista
    for(int i = 0; i < sizeof(TipoHeader); i++)
    {
        adicionarValorFinal(intList, tempBuffer0[i]);
    }
    //tempBuffer pode ser descartado
    free(tempBuffer0);

    /* -- guardar o hostname na lista, depois do header --*/
    //este "for" mete o hostname ja encoded na lista
    int j = 0;
    for(int i = 0; i < strlen(argv[2]); i++)
    {
        if(argv[2][i] != '.')
        {
            //se nao for um ponto metemos so o valor dele (em int) na lista
            j++;
            adicionarValorFinal(intList, (int)argv[2][i]);
        }
        else
        {
            //se for um ponto, voltamos para tras e metemos o numero de letras no sitio certo
            adicionarEm(intList, i - j + sizeof(TipoHeader), j);
            j = 0;
        }
    }
    //quando chegamos ao final vamos meter o ultimo numero na lista (ex: 3com)
    adicionarEm(intList, strlen(argv[2]) - j + sizeof(TipoHeader), j);
    //adicionar o \0 no final do nome para indicar que acabou
    adicionarValorFinal(intList, 0);


    //buffer temporario para guardar os conteudos do dnstype e dnsclass
    unsigned char *tempBuffer1 = (unsigned char *)malloc(sizeof(TipoPergunta));
    pergunta = (TipoPergunta *)tempBuffer1; //ligar a pergunta ao tempBuffer1

    pergunta->dnstype = htons(1); //tipo dns = A
    pergunta->dnsclass = htons(1); //class dns = IN (internet)

    //vamos meter a dns class e type para dentro da lista
    for(int i = 0; i < sizeof(TipoPergunta); i++)
    {
        adicionarValorFinal(intList, tempBuffer1[i]);
    }

    //char para guardar a lista
    int dnsBufferLen = tamanhoLista(intList);
    unsigned char dnsBuffer[dnsBufferLen];
    memset(dnsBuffer, 0, dnsBufferLen);

    //meter a lista para dentro do dnsBuffer
    for(int i = 0; i<tamanhoLista(intList); i++)
    {
        dnsBuffer[i] = (char)valorDaPosicao(intList, i);
    }

    //ja nao precisamos da linked list, vamos dar free disso:
    free_list(intList);


    //criar a socket
    int sock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
    if (sock < 0) {
      error("socket() failed");
    }

    //criar + preencher informacoes do server de dns
    struct sockaddr_in dnsServer;
    dnsServer.sin_family = AF_INET; //familia do endereço = Internet
    dnsServer.sin_addr.s_addr = inet_addr(argv[1]); 
    dnsServer.sin_port = htons(53); //a porta de DNS é a 53
    int dnsServerLen = sizeof(dnsServer);

    if(sendto(sock,(char*)dnsBuffer, dnsBufferLen ,0,(struct sockaddr*)&dnsServer,dnsServerLen) < 0)
    {
        error("sendto() failed");
    }



    //buffer temporario para guardar a resposta do servidor de dns
    //segundo o RFC 1035 (Section 2.3. 4. Size Limits) a parte de dns tem no maximo 512bytes:
    unsigned char *tempBuffer2 = (unsigned char *)malloc(512 * sizeof(char)); //512 * sizeof(char) = 512 bytes

    //receber mensagem do servidor de dns
    int msgSize;
    msgSize = recvfrom (sock,(char*)tempBuffer2, 512 , 0 , (struct sockaddr*)&dnsServer , (socklen_t*)&dnsServerLen);
    if(msgSize < 0)
    {
        error("recvfrom() failed");
    }

    //carregar a mensagem que recebemos no header
    header = (TipoHeader *)tempBuffer2;
    limparEcra();
    mostrarBanner();

    //mostrar as informaçoes do header
    printf("Pergunta sobre %s\n", argv[2]);
    printf("Resposta de %d bytes de %s: \n", msgSize, inet_ntoa(dnsServer.sin_addr));
    printf("Contem %d answer(s), %d authorative server(s) e %d aditional record(s) \n", ntohs(header->ancount), ntohs(header->nscount), ntohs(header->arcount));

    if(!ntohs(header->ancount))
    {
        error("nenhuma resposta encontrada.\n");
    }

    //criar um leitor de respostas e mete-lo na posicao do comeco da primeira resposta
    //unsigned char *leitorRespostas;
    //leitorRespostas = &tempBuffer2[dnsBufferLen];
    //mostrar as respostas
    TipoRespostaDNS_IPV4 *resposta = (TipoRespostaDNS_IPV4 *)malloc(sizeof(TipoRespostaDNS_IPV4));
    int espacoCNAME = 0;
    for(int i= 0; i < ntohs(header->ancount); i++)
    {
        printf("*---------------------");
        printf("Resposta %d: \n",i+1);
        //encher a resposta com a parte certa do buffer
        resposta = (TipoRespostaDNS_IPV4 *)&tempBuffer2[dnsBufferLen + sizeof(TipoRespostaDNS_IPV4)*i + espacoCNAME];

        if(ntohs(resposta->type) == 5)
        {
            //quando e type CNAME (5) ha coisas a mais, vamos dar skip disso e mostrar so o que interessa:
            printf("Nome: %s |", argv[2]);
            printf("Type: %d (CNAME) | Class: %d | \n", ntohs(resposta->type), ntohs(resposta->rclass));
            
            espacoCNAME = ntohs(resposta->length) - sizeof(struct in_addr);
            
        }
        else
        {
            //dar print dos dados da resposta actual
            printf("IPv4: %s \n", inet_ntoa(resposta->addr));
            printf("Type: %d | Class: %d | TTL: %d | \n", ntohs(resposta->type), ntohs(resposta->rclass),  htonl(resposta->ttl));
        }
    }

    //tempBuffer2 ja nao é preciso
    free(tempBuffer2);



    return 0;
}