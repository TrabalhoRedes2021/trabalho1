# trabalho1
Trabalho 1 de redes 

## Para compilar

~~~BASH
make all
~~~

## Para executar

~~~BASH
make run IP_SERVER="192.168.1.100" PORT_SERVER="8000" PORT_SERVER2="8888" IP_CLIENT="192.168.1.7" PORT_CLIENT="5000"
~~~

IP_SERVER: IP da máquina 1

PORT_SERVER: Porta do servidor que vai receber
PORT_SERVER2: Porta que vai ser o cliente na máquina 1

IP_CLIENT: IP da máquina 2
PORT_CLIENT: Porta que vai se conectar para enviar messagens

Mensagem para negociação: "len:tamanhodoBuffer"
Mensagem para print(salvar arquivo): "print"
Mensagem de parar a transmissão: "stop" e "exit"

O arquivo de csv vai ser transmitido !

Caso a primeira execução dê errado. Por favor tentar novamente !
Por algum motivo a primeira execução sempre está dando errado e a segunda da certo !

Camada de enlace verifica o tamanho do pacote;
Para facilitar mesmo que não tenha feito a negociação tem um default de 200
