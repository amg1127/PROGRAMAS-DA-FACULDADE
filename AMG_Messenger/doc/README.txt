Para compilar com qt3, os comandos são:

$ qmake -makefile
$ make

Mas, se estiver usando qt4, eh necessaria uma adaptacao do codigo. Para isso, o comando eh:

$ qt3to4 AMG_Messenger.pro

E depois, compilar normalmente:

$ qmake -makefile
$ make

No Windows, sob certas configuracoes, o comando 'make' nao funciona. Digitar no lugar: mingw32-make, quando isso ocorrer.

