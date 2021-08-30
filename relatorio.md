# Simulação do modelo de Referência OSI pela construção de um protótipo de camada

## Fundamentos de Redes de Computadores

Prof. Fernando W. Cruz 1°. Semestre de 2021

## Alunos:

- Ana Carolina Carvalho da Silva - 190063441
- Ésio Gustavo Pereira Freitas - 170033066
- Victor Hugo Dias Coelho - 160019401

## Introdução

O objetivo desse trabalho é a implementação de um pseudo-protocolo de enlace de dados (camada N1 pseudo-enlace, como ilustrado na figura abaixo) contendo diálogo ponto-a-ponto, stop-and-wait e não-orientado à conexão. Significa dizer que as entidades da camada N-1 não precisam estabelecer conexão previamente antes de enviar os dados (podem enviar os dados diretamente ao outro lado).

## Rodar o Projeto

```sh
$ sudo docker-compose up --build

```

## Diário de atividade

- 24 de agosto: Configuração do projeto e entendimento do problema
- 25 de agosto: Criação de um simplex para poder entender a comunicação.
- 26 de agosto: Evolução do codigo para um duplex por meio de fork() da linguagem C
- 27 de agosto: Evolução do código de um duplex para o arquivo network_util.c
- 29 de agosto: Criação da negociação entre as camadas N-1
- 30 de agosto: Criação das camadas N por meio do arquivo queue_utils.c e aujustes para finalização do projeto.

## Funções ultilizadas

- fork: Clone o processo de chamada, criando uma cópia exata. Retorne -1 para erros, 0 para o novo processo e o ID do novo processo para o processo antigo.
- sendto: Envia N bytes de BUF no soquete FD para par no endereço ADDR. Retorna o número enviado ou -1 para erros.
- malloc: Aloca SIZE bytes de memória.
- memset: Define N bytes de S para C.
- recvfrom: pega um struct sockaddr \*, que diz de onde os dados vieram e preencherá fromlen com o tamanho de struct sockaddr
- socket: Cria um novo socket do tipo TYPE no domínio
- free: Liberar um bloco alocado por `malloc '

## Opinião sobre o experimento
