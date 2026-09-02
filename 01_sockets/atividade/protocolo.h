/*
 * protocolo.h — Definições do protocolo de comunicação
 *
 * Este header é compartilhado entre cliente e servidor.
 * Define os tipos de mensagem e constantes do jogo.
 *
 * Protocolo de mensagens (texto separado por '|'):
 *
 *   Servidor → Cliente:
 *     MSG|texto                        → mensagem genérica para exibir
 *     NOME|                            → solicita o nome do jogador
 *     AGUARDE|texto                    → pede para esperar (ex: outro jogador)
 *     RODADA|num|letra|tempo           → início de rodada (número, letra, tempo em seg)
 *     RESULTADO|texto                  → resultado da rodada
 *     PLACAR|nome1|pts1|nome2|pts2     → placar atual
 *     FIM|texto                        → fim do jogo + resultado final
 *
 *   Cliente → Servidor:
 *     NOME|nome_do_jogador             → resposta com o nome
 *     PALAVRA|palavra_digitada         → resposta com a palavra da rodada
 *     TIMEOUT|                         → tempo esgotado (não enviou a tempo)
 */

#ifndef PROTOCOLO_H
#define PROTOCOLO_H

/* ============================================================
 * Constantes do jogo
 * ============================================================ */
#define PORTA_PADRAO    7070
#define MAX_JOGADORES   2
#define TOTAL_RODADAS   5
#define TEMPO_LIMITE    10      /* segundos por rodada */
#define MIN_CARACTERES  5       /* tamanho mínimo da palavra */

/* ============================================================
 * Constantes de rede
 * ============================================================ */
#define BUFFER_SIZE     512
#define NOME_SIZE       32

/* ============================================================
 * Prefixos das mensagens do protocolo
 * ============================================================ */
#define PROTO_MSG       "MSG"
#define PROTO_NOME      "NOME"
#define PROTO_AGUARDE   "AGUARDE"
#define PROTO_RODADA    "RODADA"
#define PROTO_RESULTADO "RESULTADO"
#define PROTO_PLACAR    "PLACAR"
#define PROTO_FIM       "FIM"
#define PROTO_PALAVRA   "PALAVRA"
#define PROTO_TIMEOUT   "TIMEOUT"

/* Separador de campos na mensagem */
#define PROTO_SEP       "|"

#endif /* PROTOCOLO_H */
