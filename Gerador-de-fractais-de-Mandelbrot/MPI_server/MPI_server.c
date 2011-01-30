/* MPI_server.c */
#include <ctype.h>
#include <mpi.h>
#include <gd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

/* Defines constantes... (hehehehe) */
/* Host que escutara a porta TCP (e, por isso, sera o mestre da comunicacao) */
#ifndef HOSTQUEESCUTA
#define HOSTQUEESCUTA "localhost"
#endif
/* Porta TCP que sera aberta em um dos nos */
#ifndef PORTATCP
#define PORTATCP 1111
#endif
/* Tamanho padrao para buffers (riscos de BUFFER OVERFLOW se muito baixo) */
#define TAMBUFF 4096
/* Tamanho maximo para largura e altura (CUIDADO: ESTE NUMERO NAO PODE SER MAIOR QUE 46340, A DESOBEDIENCIA CAUSARA INTEGER OVERFLOW) */
#define MAXDIMENSION 2000
/* Numero maximo de iteracoes */
#define MAXMAXITER 20000
/* Tamanho maximo da paleta (nao deve passar de 65535, a desobediencia causara INTEGER OVERFLOW) */
#define MAXPALLEN 1024

/* Esses parametros podem ser alterados na solicitacao */
/* Largura padrao da imagem */
#define LARGDEFAULT 1024
/* Altura padrao da imagem */
#define ALTDEFAULT 768
/* Numero de iteracoes de Mandelbrot */
#define MAXITERDEFAULT 10
/* Menor valor no eixo X */
#define MINXDEFAULT -2.0
/* Maior valor no eixo X */
#define MAXXDEFAULT 2.0
/* Menor valor no eixo Y */
#define MINYDEFAULT -2.0
/* Maior valor no eixo Y */
#define MAXYDEFAULT 2.0
/* Tamanho da paleta de cores */
#define PALLENDEFAULT 8

/* Macros */
#ifdef VERBOSE
#warning Modo GRITANTE habilitado.
#define __S__(c) c
#else
#define __S__(c) {;}
#endif
#define assert_mpi(cond) if (! (cond)) { fprintf (stderr, "[pid=%d] %s:%d Assertion '%s' failed.\n", \
                                         getpid(), \
                                         __FILE__, \
                                         __LINE__, \
                                         __STRING(cond)); \
                                         MPI_Abort (MPI_COMM_WORLD, -1); \
                                         sleep (5); \
                                         abort (); \
                                       }
#define GETMEMORY(var,type,units) { __S__(fprintf (stderr, "[pid=%d] %s:%d Getting %d bytes (%d units of type '%s'). Pointer is '%s'.\n", \
                                    getpid(), \
                                    __FILE__, \
                                    __LINE__, \
                                    (units) * sizeof(type), \
                                    (units), \
                                    __STRING(type), \
                                    __STRING(var))); \
                                    do \
                                        var = (type*) malloc ((units) * sizeof(type)); \
                                    while (var == NULL); \
                                    __S__(fprintf (stderr, "[pid=%d] %s:%d Pointer '%s' now points to address %p.\n", \
                                    getpid(), \
                                    __FILE__, \
                                    __LINE__, \
                                    __STRING(var), \
                                    var)); \
                                  }
#define FREEMEMORY(var) { __S__(fprintf (stderr, "[pid=%d] %s:%d Address %p (pointed by '%s') is going to be freed.\n", \
                          getpid (), \
                          __FILE__, \
                          __LINE__, \
                          var, \
                          __STRING(var))); \
                          free (var); \
                          var = NULL; \
                          __S__(fprintf (stderr, "[pid=%d] %s:%d Pointer '%s' points to NULL now.\n", \
                          getpid (), \
                          __FILE__, \
                          __LINE__, \
                          __STRING(var))); \
                        }
#define DEBUGA(msg) { __S__(fprintf (stderr, "[pid=%d] %s:%d %s\n", \
                     getpid (), \
                     __FILE__, \
                     __LINE__, \
                     msg)); \
                   }
#define ACERTAFAIXA(var,n1,n2) if ((n1)<(n2)) { \
                                   if ((var)>(n2)) { \
                                       (var)=(n2); \
                                   } else if ((var)<(n1)) { \
                                       (var)=(n1); \
                                   } \
                               } else if ((n2)<(n1)) { \
                                   if ((var)>(n1)) { \
                                       (var)=(n1); \
                                   } else if ((var)<(n2)) { \
                                       (var)=(n2); \
                                   } \
                               } else { \
                                   (var) = (n1); \
                               }

/* Comandos que o mestre vai passar para os seus subordinados (e que vai receber de um cliente) */
#define CMDSETLARG 1
#define CMDSETALT 2
#define CMDSETMAXITER 3
#define CMDSETMINX 4
#define CMDSETMAXX 5
#define CMDSETMINY 6
#define CMDSETMAXY 7
#define CMDSETPALLEN 8
#define CMDMAKE 9
#define CMDHALT 10

typedef struct {
    double real, imag;
} cplx_t;

typedef struct {
    int vermelho, verde, azul;
    int gd_cor;
} cor_t;

struct {
    int portaTCP;
    char *host;
    int largura;
    int altura;
    int maxiter;
    double x1;
    double x2;
    double diffx;
    double y1;
    double y2;
    double diffy;
    cor_t *paleta;
    int pallen;
    int rank;
    int size;
    int rank_master;
    unsigned short int *linhaIMG;
    unsigned short int *IMGcompleta;
    MPI_Request *usRecebes_reqs;
    int *usRecebes_nums;
} global = {
    PORTATCP,
    HOSTQUEESCUTA,
    0,
    0,
    MAXITERDEFAULT,
    MINXDEFAULT,
    MAXXDEFAULT,
    MAXXDEFAULT - MINXDEFAULT,
    MINYDEFAULT,
    MAXYDEFAULT,
    MAXYDEFAULT - MINYDEFAULT,
    NULL,
    0,
    -1,
    0,
    -1,
    NULL,
    NULL,
    NULL
};

int divarredpcima (int dividendo, int divisor) {
    int q = dividendo / divisor;
    if (dividendo % divisor) q++;
    return (q);
}

void ajustaTamPaleta (int novoTam) {
    cor_t *n;
    int l;
    if (novoTam <= 0) return;
    DEBUGA ("Alocando espaco para a paleta.");
    GETMEMORY (n, cor_t, novoTam);
    l = (novoTam > global.pallen) ? global.pallen : novoTam;
    if (global.paleta != NULL) {
        memcpy (n, global.paleta, sizeof (cor_t) * l);
        DEBUGA ("Ja existia uma paleta... Limpando...");
        FREEMEMORY (global.paleta);
    }
    global.paleta = n;
    if (novoTam > global.pallen)
        memset (&n[global.pallen], 0, sizeof (cor_t) * (novoTam - global.pallen));
    global.pallen = novoTam;
}

void ajustaTamImagem (int novoLarg, int novoAlt) {
    unsigned short int *n;
    int l, novoquan, velhoquan;
    if (novoLarg <= 0 || novoAlt <= 0) return;
    if (global.rank == global.rank_master) {
        novoquan = novoLarg * novoAlt;
        velhoquan = global.largura * global.altura;
        DEBUGA ("Alocando espaco para a imagem completa.");
        GETMEMORY (n, unsigned short int, novoquan);
        l = (novoquan > velhoquan) ? velhoquan : novoquan;
        if (global.IMGcompleta != NULL) {
            memcpy (n, global.IMGcompleta, sizeof (unsigned short int) * l);
            DEBUGA ("Ja existia uma imagem... Limpando...");
            FREEMEMORY (global.IMGcompleta);
        }
        global.IMGcompleta = n;
        if (novoquan > velhoquan)
            memset (&n[velhoquan], 0, sizeof (unsigned short int) * (novoquan - velhoquan));
    } else {
        DEBUGA ("Alocando espaco para uma linha de imagem...");
        novoquan = novoLarg;
        velhoquan = global.largura;
        GETMEMORY (n, unsigned short int, novoquan);
        l = (novoquan > velhoquan) ? velhoquan : novoquan;
        if (global.linhaIMG != NULL) {
            memcpy (n, global.linhaIMG, sizeof (unsigned short int) * l);
            DEBUGA ("Ja existia uma linha de imagem... Limpando...");
            FREEMEMORY (global.linhaIMG);
        }
        global.linhaIMG = n;
        if (novoquan > velhoquan)
            memset (&n[velhoquan], 0, sizeof (unsigned short int) * (novoquan - velhoquan));
    }
    global.largura = novoLarg;
    global.altura = novoAlt;
}

cplx_t soma (cplx_t num1, cplx_t num2) {
    cplx_t res;
    res.real = num1.real + num2.real;
    res.imag = num1.imag + num2.imag;
    return (res);
}

cplx_t mult (cplx_t num1, cplx_t num2) {
    cplx_t res;
    res.real = num1.real * num2.real - num1.imag * num2.imag;
    res.imag = num1.real * num2.imag + num1.imag * num2.real;
    return (res);
}

int iteracao_mandelbrot (int x, int y) {
    int i;
    cplx_t num, z;
    num.real = global.x1 + global.diffx * x / global.largura;
    num.imag = global.y1 + global.diffy * y / global.altura;
    z.real = 0;
    z.imag = 0;
    for (i=0; i<global.maxiter; i++) {
        z = soma (mult (z, z), num);
        if (z.real * z.real + z.imag * z.imag > 4)
            break;
    }
    return (i % global.pallen);
}

void lookForTheMaster () {
    /* Funcao para eleger 1 mestre... Com 120 nos, isso demora muito! Ainda bem que isso so vai rodar 1 vez so (ou deveria)... */
    int i, r, num;
    for (i = 0; i < global.size; i++) {
        num = (global.rank == global.rank_master) ? global.rank : -1;
        r = MPI_Bcast (&num, 1, MPI_INT, i, MPI_COMM_WORLD);
        assert_mpi (r == MPI_SUCCESS);
        if (i != global.rank) {
            assert_mpi (num >= -1 && num < global.size);
            if (num != -1) {
                if (global.rank_master != -1) {
                    /* Tem 2 mestres no ambiente MPI. Abortar... */
                    assert_mpi (0);
                }
                global.rank_master = num;
            }
        }
    }
    /* Verificar se um mestre foi encontrado */
    assert_mpi (global.rank_master != -1);
}

void geraMandelbrot (void) {
    /* Modificacao que o Pilla pediu: o mestre fica esperando os escravos gerarem linhas de cores, ao inves de distribuir igualmente */
    int linha, r, x, escravo;
    if (global.rank == global.rank_master) {
        linha = 0;
        for (escravo = 0; escravo < global.rank_master; escravo++) {
            global.usRecebes_nums[linha] = linha;
            r = MPI_Isend (&(global.usRecebes_nums[linha]), 1, MPI_INT, escravo, 0, MPI_COMM_WORLD, &(global.usRecebes_reqs[linha]));
            assert_mpi (r == MPI_SUCCESS);
            linha++;
        }
        for (escravo = global.rank_master + 1; escravo < global.size; escravo++) {
            global.usRecebes_nums[linha] = linha;
            r = MPI_Isend (&(global.usRecebes_nums[linha]), 1, MPI_INT, escravo, 0, MPI_COMM_WORLD, &(global.usRecebes_reqs[linha]));
            assert_mpi (r == MPI_SUCCESS);
            linha++;
        }
        while (linha - global.size + 1 < global.altura) {
            r = MPI_Waitany (global.size - 1, global.usRecebes_reqs, &escravo, MPI_STATUS_IGNORE);
            assert_mpi (r == MPI_SUCCESS);
            x = escravo;
            if (escravo >= global.rank_master)
                escravo++;
            if (global.usRecebes_nums[x] != -1) {
                if (global.usRecebes_nums[x] < global.altura) {
                    r = MPI_Irecv(&global.IMGcompleta[global.usRecebes_nums[x] * global.largura], global.largura, MPI_UNSIGNED_SHORT, escravo, 0, MPI_COMM_WORLD, &(global.usRecebes_reqs[x]));
                    assert_mpi (r == MPI_SUCCESS);
                    global.usRecebes_nums[x] = -1;
                }
            } else {
                global.usRecebes_nums[x] = linha;
                r = MPI_Isend (&(global.usRecebes_nums[x]), 1, MPI_INT, escravo, 0, MPI_COMM_WORLD, &(global.usRecebes_reqs[x]));
                assert_mpi (r == MPI_SUCCESS);
                linha++;
            }
        }
        r = MPI_Waitall (global.size - 1, global.usRecebes_reqs, MPI_STATUSES_IGNORE);
        assert_mpi (r == MPI_SUCCESS);
    } else {
        r = MPI_Recv (&linha, 1, MPI_INT, global.rank_master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        assert_mpi (r == MPI_SUCCESS);
        while (linha >= 0 && linha < global.altura) {
            for (x = 0; x < global.largura; x++) {
                global.linhaIMG[x] = iteracao_mandelbrot (x, linha);
            }
            r = MPI_Send (global.linhaIMG, global.largura, MPI_UNSIGNED_SHORT, global.rank_master, 0, MPI_COMM_WORLD);
            assert_mpi (r == MPI_SUCCESS);
            r = MPI_Recv (&linha, 1, MPI_INT, global.rank_master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            assert_mpi (r == MPI_SUCCESS);
        }
    }
}

int lealgumacoisa (FILE* f, char *buffer, int bufflen) {
    static char tmpbuff [TAMBUFF];
    static int i = 0;
    if (bufflen < 2)
        return (0);
    if (i != bufflen) {
        i = bufflen;
        sprintf (tmpbuff, "%%%ds", bufflen - 1);
    }
    return (fscanf (f, tmpbuff, buffer) == 1);
}

void mandaImagem (FILE *f) {
    gdImagePtr img;
    int i, x, y;
    assert_mpi (global.IMGcompleta != NULL);
    img = gdImageCreateTrueColor (global.largura, global.altura);
    if (img == NULL) return;
    for (i = 0; i < global.pallen; i++) {
        global.paleta[i].gd_cor = gdImageColorResolve (img, global.paleta[i].vermelho, global.paleta[i].verde, global.paleta[i].azul);
    }
    for (i = 0, y = 0; y < global.altura; y++) {
        for (x = 0; x < global.largura; x++, i++) {
            gdImageSetPixel(img, x, y, global.paleta[global.IMGcompleta[i]].gd_cor);
        }
    }
    gdImagePng (img, f);
    gdImageDestroy (img);
}

void master (int socketnaescuta) {
    int cli, numi, cmd, r;
    double numd;
    static char buffin [TAMBUFF];
    FILE *cliente;
    DEBUGA ("Atuando como mestre.");
    global.rank_master = global.rank;
    lookForTheMaster ();
    ajustaTamPaleta (PALLENDEFAULT);
    ajustaTamImagem (LARGDEFAULT, ALTDEFAULT);
    GETMEMORY (global.usRecebes_reqs, MPI_Request, global.size - 1);
    GETMEMORY (global.usRecebes_nums, int, global.size - 1);
    /* Aguardar por uma conexao TCP */
    while ((cli = accept (socketnaescuta, NULL, NULL)) != -1) {
        /* Temos um cliente na linha... Vamos ver o que ele quer... OS OUTROS QUE ESPEREM!!! */
        cliente = fdopen (cli, "a+");
        assert_mpi (cliente != NULL);
        while (lealgumacoisa (cliente, buffin, TAMBUFF)) {
            if (strcasecmp (buffin, "LARG") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%d", &numi);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numi, 1, MAXDIMENSION);
                    if (global.largura != numi) {
                        ajustaTamImagem (numi, global.altura);
                        cmd = CMDSETLARG;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numi, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %d\n", global.largura);
            } else if (strcasecmp (buffin, "ALT") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%d", &numi);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numi, 1, MAXDIMENSION);
                    if (global.altura != numi) {
                        ajustaTamImagem (global.largura, numi);
                        cmd = CMDSETALT;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numi, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %d\n", global.altura);
            } else if (strcasecmp (buffin, "MAXITER") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%d", &numi);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numi, global.pallen, MAXMAXITER);
                    if (global.maxiter != numi) {
                        global.maxiter = numi;
                        cmd = CMDSETMAXITER;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numi, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %d\n", global.maxiter);
            } else if (strcasecmp (buffin, "MINX") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%lg", &numd);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numd, -2.0, 2.0);
                    if (global.x1 != numd) {
                        global.x1 = numd;
                        global.diffx = global.x2 - global.x1;
                        cmd = CMDSETMINX;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numd, 1, MPI_DOUBLE, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %.030lg\n", global.x1);
            } else if (strcasecmp (buffin, "MAXX") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%lg", &numd);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numd, -2.0, 2.0);
                    if (global.x2 != numd) {
                        global.x2 = numd;
                        global.diffx = global.x2 - global.x1;
                        cmd = CMDSETMAXX;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numd, 1, MPI_DOUBLE, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %.030lg\n", global.x2);
            } else if (strcasecmp (buffin, "MINY") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%lg", &numd);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numd, -2.0, 2.0);
                    if (global.y1 != numd) {
                        global.y1 = numd;
                        global.diffy = global.y2 - global.y1;
                        cmd = CMDSETMINY;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numd, 1, MPI_DOUBLE, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %.030lg\n", global.y1);
            } else if (strcasecmp (buffin, "MAXY") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%lg", &numd);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numd, -2.0, 2.0);
                    if (global.y2 != numd) {
                        global.y2 = numd;
                        global.diffy = global.y2 - global.y1;
                        cmd = CMDSETMAXY;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numd, 1, MPI_DOUBLE, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %.030lg\n", global.y2);
            } else if (strcasecmp (buffin, "PALLEN") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%d", &numi);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numi, 1, MAXPALLEN);
                    if (global.pallen != numi) {
                        ajustaTamPaleta (numi);
                        cmd = CMDSETPALLEN;
                        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                        r = MPI_Bcast (&numi, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                        assert (r == MPI_SUCCESS);
                    }
                }
                fprintf (cliente, "OK %d\n", global.pallen);
            } else if (strcasecmp (buffin, "PAL") == 0) {
                if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                r = sscanf (buffin, "%d", &numi);
                if (r == EOF) {
                    break;
                } else if (r == 1) {
                    ACERTAFAIXA (numi, 0, global.pallen - 1);
                    if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                    r = sscanf (buffin, "%d", &(global.paleta[numi].vermelho));
                    if (r == EOF) {
                        break;
                    } else if (r == 1) {
                        ACERTAFAIXA (global.paleta[numi].vermelho, 0, 255);
                    }
                    if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                    r = sscanf (buffin, "%d", &(global.paleta[numi].verde));
                    if (r == EOF) {
                        break;
                    } else if (r == 1) {
                        ACERTAFAIXA (global.paleta[numi].verde, 0, 255);
                    }
                    if (! lealgumacoisa (cliente, buffin, TAMBUFF)) break;
                    r = sscanf (buffin, "%d", &(global.paleta[numi].azul));
                    if (r == EOF) {
                        break;
                    } else if (r == 1) {
                        ACERTAFAIXA (global.paleta[numi].azul, 0, 255);
                    }
                    fprintf (cliente, "OK %d %d %d %d\n", numi, global.paleta[numi].vermelho, global.paleta[numi].verde, global.paleta[numi].azul);
                }
            } else if (strcasecmp (buffin, "MAKE") == 0) {
                cmd = CMDMAKE;
                r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                assert (r == MPI_SUCCESS);
                geraMandelbrot ();
                fprintf (cliente, "OK\n");
            } else if (strcasecmp (buffin, "DUMP") == 0) {
                mandaImagem (cliente);
                break;
            } else if (strcasecmp (buffin, "AGORA") == 0) {
                fprintf (cliente, "OK %.030lg\n", MPI_Wtime());
#ifdef ACCEPT_HALT
#warning Desligar esse define [ACCEPT_HALT] mais tarde!
            } else if (strcasecmp (buffin, "HALT") == 0) {
                cmd = CMDHALT;
                r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank, MPI_COMM_WORLD);
                assert (r == MPI_SUCCESS);
                fprintf (cliente, "OK\n");
                return;
#endif
            } else {
                /* Usuario enviou um comando invalido */
                break;
            }
            fflush (cliente);
        }
        fclose (cliente);
    }
    /* Saiu do while??? Isso eh ruim...! */
    assert_mpi (0);
}

void slave () {
    int cmd, param, r;
    double paramd;
    DEBUGA ("Atuando como escravo...");
    lookForTheMaster ();
    ajustaTamImagem (LARGDEFAULT, ALTDEFAULT);
    global.pallen = PALLENDEFAULT;
    while (1) {
        /* Esperar por um comando do mestre */
        DEBUGA ("Esperando por um comando...");
        r = MPI_Bcast (&cmd, 1, MPI_INT, global.rank_master, MPI_COMM_WORLD);
        DEBUGA ("Recebi um comando...");
        assert (r == MPI_SUCCESS);
        switch (cmd) {
            case CMDSETLARG:
            case CMDSETALT:
            case CMDSETMAXITER:
            case CMDSETPALLEN:
                r = MPI_Bcast (&param, 1, MPI_INT, global.rank_master, MPI_COMM_WORLD);
                assert (r == MPI_SUCCESS);
                if (cmd == CMDSETLARG) {
                    ACERTAFAIXA (param, 1, MAXDIMENSION);
                    ajustaTamImagem (param, global.altura);
                } else if (cmd == CMDSETALT) {
                    ACERTAFAIXA (param, 1, MAXDIMENSION);
                    ajustaTamImagem (global.largura, param);
                } else if (cmd == CMDSETMAXITER) {
                    ACERTAFAIXA (param, 1, MAXMAXITER);
                    global.maxiter = param;
                } else {
                    ACERTAFAIXA (param, 1, MAXPALLEN);
                    global.pallen = param;
                }
                break;
            case CMDSETMINX:
            case CMDSETMAXX:
            case CMDSETMINY:
            case CMDSETMAXY:
                r = MPI_Bcast (&paramd, 1, MPI_DOUBLE, global.rank_master, MPI_COMM_WORLD);
                assert (r == MPI_SUCCESS);
                ACERTAFAIXA (paramd, -2.0, 2.0);
                if (cmd == CMDSETMINX) {
                    global.x1 = paramd;
                } else if (cmd == CMDSETMAXX) {
                    global.x2 = paramd;
                } else if (cmd == CMDSETMINY) {
                    global.y1 = paramd;
                } else {
                    global.y2 = paramd;
                }
                global.diffx = global.x2 - global.x1;
                global.diffy = global.y2 - global.y1;
                break;
            case CMDMAKE:
                geraMandelbrot ();
                break;
            case CMDHALT:
                return;
            default:
                /* Comando invalido! */
                assert_mpi (0);
        }
    }
}

int main (int argc, char **argv) {
    int r, sd;
    int l = TAMBUFF;
    static char buff[TAMBUFF];
    struct sockaddr_in endereco;
    /* Inicializar o ambiente MPI */
    r = MPI_Init (&argc, &argv);
    assert (r == MPI_SUCCESS);
    r = MPI_Comm_size (MPI_COMM_WORLD, &(global.size));
    assert_mpi (r == MPI_SUCCESS);
    r = MPI_Comm_rank (MPI_COMM_WORLD, &(global.rank));
    assert_mpi (r == MPI_SUCCESS);
    /* Por causa da modificacao do Pilla, agora o mestre nao trabalha sozinho */
    assert_mpi (global.size > 1);
    /* Se ate aqui, OK, entao, vamos descobrir qual eh o host onde o MPI esta rodando agora */
    /* Se a funcao der buffer overflow, a culpa nao eh minha... */
    r = MPI_Get_processor_name (buff, &l);
    assert_mpi (r == MPI_SUCCESS);
    buff[l] = '\0';
    srand (time (NULL));
    if (strncmp (buff, global.host, strlen (global.host)) == 0) {
        /* Entao, estou rodando no candidato a mestre...
           Candidato, porque, se a porta escolhida no define estiver em uso, eu me elejo escravo */
        sd = socket (PF_INET, SOCK_STREAM, 0);
        assert_mpi (sd != -1);
        r = setsockopt (sd, SOL_TCP, TCP_NODELAY, &l, sizeof (int));
        assert_mpi (r == 0);
        endereco.sin_family = AF_INET;
        endereco.sin_port = htons (global.portaTCP);
        endereco.sin_addr.s_addr = INADDR_ANY;
        r = bind (sd, (struct sockaddr *) &endereco, sizeof (struct sockaddr_in));
        if (r == 0) {
            r = listen (sd, 1);
            assert_mpi (r == 0);
            master (sd);
            close (sd);
        } else {
            assert_mpi (r == -1);
            if (errno == EADDRINUSE) {
                close (sd);
                slave ();
            } else {
                /* Acaba com tudo */
                close (sd);
                assert_mpi (0);
            }
        }
    } else {
        /* Vou virar escravo */
        slave ();
    }
    /* O programa nao deveria chegar aqui sob condicoes normais... */
    DEBUGA ("Comando 'HALT' recebido. Terminando agora...");
    r = MPI_Finalize ();
    assert (r == MPI_SUCCESS);
    return (0);
}
/* MPI_server.c */
