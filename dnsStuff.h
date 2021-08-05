#ifndef __DNSSTUFF_H__
#define __DNSSTUFF_H__


#include <stdint.h> //para uint
#include <netinet/in.h> //para in_addr


//estrutura para o header de DNS
typedef struct 
{
    uint16_t xid;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
}TipoHeader;

//estrutura para a pergunta DNS
typedef struct 
{
    uint16_t dnstype;
    uint16_t dnsclass;
}TipoPergunta;


//estrutura para a resposta dns de IPV4
typedef struct
{
    uint16_t compression;
    uint16_t type;
    uint16_t rclass;
    uint32_t ttl;
    uint16_t length;
    struct in_addr addr;
}__attribute__((packed)) TipoRespostaDNS_IPV4;


#endif