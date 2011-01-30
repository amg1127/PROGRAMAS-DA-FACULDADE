#define _GNU_SOURCE

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define LARGDEFAULT 640
#define ALTDEFAULT 480
#define MAXITERDEFAULT 1000
#define PALDEFAULT 8
#define MINXDEF -2.0
#define MAXXDEF 2.0
#define MINYDEF -2.0
#define MAXYDEF 2.0
#define PORTATCPDEFAULT 1111
#define HOSTDEFAULT "h3p.g3pd.ucpel.tche.br"

double milisecs (void) {
    struct timeval t;
    struct timezone z;
    double r = 0;
    int s;
    s = gettimeofday (&t, &z);
    if (s == 0) {
        r = 1000.0L * t.tv_sec + 0.001 * t.tv_usec;
    }
    return (r);
}

void usage (FILE *disp, char *argv0) {
    fprintf (disp, "Uso: %s <opcoes> r g b [r g b [r g b ...]]\n"
                   "As seguintes opcoes sao suportadas:\n"
                   "    -h, --help                       Mostra este texto de ajuda.\n"
                   "    -H, --host                       Especifica o nome da maquina que executa o servidor que gera fractais (padrao: %s).\n"
                   "    -p, --porta                      Especifica a porta TCP/IP utilizada pelo servidor de fractais (padrao: %d).\n"
                   "    -o, --output <arquivo>           Especifica o arquivo que contem a imagem produzida pelas iteracoes de mandelbrot (padrao: stdout).\n"
                   "    -m, --max-iterations <numero>    Especifica o numero maximo de iteracoes de mandelbrot (padrao: %d).\n"
                   "    -l, --largura <numero>           Especifica a largura de imagem, em pixels. (padrao: %d)\n"
                   "    -a, --altura <numero>            Especifica a altura de imagem, em pixels. (padrao: %d)\n"
                   "    -x, --xmin <numero>              Especifica o menor valor no eixo X. (padrao: %.2f)\n"
                   "    -X, --xmax <numero>              Especifica o maior valor no eixo X. (padrao: %.2f)\n"
                   "    -y, --ymin <numero>              Especifica o menor valor no eixo Y. (padrao: %.2f)\n"
                   "    -Y, --ymax <numero>              Especifica o maior valor no eixo Y. (padrao: %.2f)\n\n"
                   "'r', 'g' e 'b' sao numeros entre 0 e 255 que indicam a intensidade das componentes RGB em uma cor da paleta.\nPara que o "
                   "programa trabalhe com varias cores, utilize sequencias de numeros 'r', 'g' e 'b'.\n\nExemplo: 255 0 0 0 128 0 0 0 128 128 128 128 [vermelho, verde, azul e cinza]\n"
                   "\n", argv0, HOSTDEFAULT, PORTATCPDEFAULT, MAXITERDEFAULT, LARGDEFAULT, ALTDEFAULT, MINXDEF, MAXXDEF, MINYDEF, MAXYDEF);
}

int main (int argc, char **argv) {
    int largura = 0;
    int altura = 0;
    int maxiter = 0;
    int paletas = 0;
    int i = 0;
    int t = 0;
    int r = 0;
    int nav = 0;
    double px1 = 0;
    double px2 = 0;
    double py1 = 0;
    double py2 = 0;
    double t2 = 0;
    char *saida = NULL;
    char *ptr = NULL;
    char *servidor = NULL;
    char servpadrao[] = HOSTDEFAULT;
    char buffresp[3][256];
    char zero[] = "0";
    int portatcp = 0;
    int soquete = 0;
    FILE *fdout = NULL;
    FILE *fdsock = NULL;
    struct option args[] = {        { "help",           no_argument,       NULL, 'h' },
                                    { "host",           required_argument, NULL, 'H' },
                                    { "porta",          required_argument, NULL, 'p' },
                                    { "output",         required_argument, NULL, 'o' },
                                    { "max-iterations", required_argument, NULL, 'm' },
                                    { "largura",        required_argument, NULL, 'l' },
                                    { "altura",         required_argument, NULL, 'a' },
                                    { "xmin",           required_argument, NULL, 'x' },
                                    { "xmax",           required_argument, NULL, 'X' },
                                    { "ymin",           required_argument, NULL, 'y' },
                                    { "ymax",           required_argument, NULL, 'Y' },
                                    { NULL,             0,                 NULL,  0  }        };
    struct hostent *ender = NULL;
    struct sockaddr_in reco;
    /* Processar argumentos da linha de comando */
    opterr = -1;
    while ((i = getopt_long (argc, argv, "hH:p:o:m:l:a:x:X:y:Y:", args, NULL)) != -1) {
        switch (i) {
            case 'h':
                usage (stdout, argv[0]);
                return (0);
            case 'H':
                assert (optarg != NULL);
                servidor = optarg;
                break;
            case 'o':
                assert (optarg != NULL);
                saida = optarg;
                break;
            case 'm':
            case 'l':
            case 'a':
            case 'p':
                assert (optarg != NULL);
                t = strtol (optarg, &ptr, 10);
                if (errno != 0 || strcmp (optarg, "") == 0 || strcmp (ptr, "") != 0) {
                    fprintf (stderr, "Erro: '%s': argumento invalido.\n", optarg);
                    usage (stderr, argv[0]);
                    return (1);
                }
                if (i == 'm')
                    maxiter = t;
                else if (i == 'l')
                    largura = t;
                else if (i == 'a')
                    altura = t;
                else
                    portatcp = t;
                break;
            case 'x':
            case 'X':
            case 'y':
            case 'Y':
                assert (optarg != NULL);
                t2 = strtod (optarg, &ptr);
                if (errno != 0 || strcmp (optarg, "") == 0 || strcmp (ptr, "") != 0) {
                    fprintf (stderr, "Erro: '%s': argumento invalido.\n", optarg);
                    usage (stderr, argv[0]);
                    return (1);
                }
                if (i == 'x')
                    px1 = t2;
                else if (i == 'X')
                    px2 = t2;
                else if (i == 'y')
                    py1 = t2;
                else 
                    py2 = t2;
                break;
            case '?':
            case ':':
                usage (stderr, argv[0]);
                return (1);
            default:
                fprintf (stderr, "Erro: funcao getopt_long() retornou um valor invalido: '%d'\n", i);
                abort ();
        }
    }
    if (maxiter < 1)
        maxiter = MAXITERDEFAULT;
    if (largura < 1)
        largura = LARGDEFAULT;
    if (altura < 1)
        altura = ALTDEFAULT;
    if (px1 == 0)
        px1 = MINXDEF;
    if (px2 == 0)
        px2 = MAXXDEF;
    if (py1 == 0)
        py1 = MINYDEF;
    if (py2 == 0)
        py2 = MAXYDEF;
    if (portatcp == 0)
        portatcp = PORTATCPDEFAULT;
    if (servidor == NULL)
        servidor = servpadrao;
    i = argc - optind;
    if (i == 0) {
        fprintf (stderr, "Erro: nenhuma codificacao de cor foi passada.\n");
        return (1);
    }
    paletas = i / 3;
    if (i % 3) {
        fprintf (stderr, "Aviso: componentes de cor nao-especificados assumem o valor zero, por padrao.\n");
        paletas++;
    }
    fprintf (stderr, "Localizando host '%s'...\n", servidor);
    ender = gethostbyname2 (servidor, AF_INET);
    if (ender == NULL) {
        fprintf (stderr, "Erro: host nao encontrado.\n");
        return (1);
    }
    fprintf (stderr, "Host foi localizado com sucesso.\n");
    reco.sin_family = AF_INET;
    reco.sin_port = htons (portatcp);
    for (nav = 0; ender->h_addr_list[nav] != NULL; nav++) {
        fprintf (stderr, "Conectando-se a '%d.%d.%d.%d:%d'...\n",
                         (unsigned char) ender->h_addr_list[nav][0],
                         (unsigned char) ender->h_addr_list[nav][1],
                         (unsigned char) ender->h_addr_list[nav][2],
                         (unsigned char) ender->h_addr_list[nav][3],
                         portatcp);
        soquete = socket (PF_INET, SOCK_STREAM, 0);
        if (soquete == -1) {
            perror ("Erro: socket nao pode ser aberto");
            continue;
        }
        i = -1;
        r = setsockopt (soquete, SOL_TCP, TCP_NODELAY, &i, sizeof (int));
        if (r) {
            perror ("Erro: nao se pode definir opcoes para o socket");
            close (soquete);
            continue;
        }
        reco.sin_addr.s_addr = *( (int*) (ender->h_addr_list[nav]) );
        r = connect (soquete, &reco, sizeof (struct sockaddr_in));
        if (r) {
            perror ("Erro: nao foi possivel estabelecer a conexao");
            close (soquete);
            continue;
        }
        fprintf (stderr, "Host conectado. Enviando dados para a geracao do Fractal...\n");
        if (saida != NULL) {
            if ((fdout = fopen (saida, "wb")) == NULL) {
                perror ("Erro: o arquivo de saida nao pode ser aberto");
                return (1);
            }
        } else {
            fdout = stdout;
        }
        fdsock = fdopen (soquete, "a+");
        if (fdsock == NULL) {
            perror ("Erro: nao foi possivel criar canal de entrada e saida com o servidor");
            return (1);
        }
        if (fprintf (fdsock, "LARG %d\n", largura) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %d", &i) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (i != largura) {
            fprintf (stderr, "Aviso: servidor alterou a largura da imagem para %dpx.\n", largura = i);
        }
        if (fprintf (fdsock, "ALT %d\n", altura) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %d", &i) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (i != altura) {
            fprintf (stderr, "Aviso: servidor alterou a altura da imagem para %dpx.\n", altura = i);
        }
        if (fprintf (fdsock, "MAXITER %d\n", maxiter) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %d", &i) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (i != maxiter) {
            fprintf (stderr, "Aviso: servidor alterou o numero maximo de iteracoes para %d.\n", maxiter = i);
        }
        if (fprintf (fdsock, "MINX %.030lg\n", px1) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %lg", &t2) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (t2 != px1) {
            fprintf (stderr, "Aviso: O servidor alterou a abcissa menor especificada na imagem para %.030lg.\n", px1 = t2);
        }
        if (fprintf (fdsock, "MAXX %.030lg\n", px2) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %lg", &t2) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (t2 != px2) {
            fprintf (stderr, "Aviso: O servidor alterou a abcissa maior especificada na imagem para %.030lg.\n", px2 = t2);
        }
        if (fprintf (fdsock, "MINY %.030lg\n", py1) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %lg", &t2) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (t2 != py1) {
            fprintf (stderr, "Aviso: O servidor alterou a ordenada menor especificada na imagem para %.030lg.\n", py1 = t2);
        }
        if (fprintf (fdsock, "MAXY %.030lg\n", py2) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %lg", &t2) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (t2 != py2) {
            fprintf (stderr, "Aviso: O servidor alterou a ordenada maior especificada na imagem para %.030lg.\n", py2 = t2);
        }
        if (fprintf (fdsock, "PALLEN %d\n", paletas) < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %d", &i) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (i != paletas) {
            fprintf (stderr, "Aviso: servidor alterou o tamanho da paleta de cores para %d.\n", paletas = i);
        }
        for (i = 0; i < paletas && optind < argc; optind += 3, i++) {
            if (fprintf (fdsock, "PAL %d %s %s %s\n", i,
                ( (optind     < argc) ? argv[optind    ] : zero ),
                ( (optind + 1 < argc) ? argv[optind + 1] : zero ),
                ( (optind + 2 < argc) ? argv[optind + 2] : zero )) < 0) {
                perror ("Erro: nao foi possivel comunicar-se com o servidor");
                return (1);
            }
            fflush (fdsock);
            if (fscanf (fdsock, "OK %d %256s %256s %256s", &t, buffresp[0], buffresp[1], buffresp[2]) != 4) {
                perror ("Erro: nao foi possivel comunicar-se com o servidor");
                return (1);
            }
            if (i != t) {
                fprintf (stderr, "Erro: servidor retornou uma resposta invalida.\n");
                return (1);
            }
            if (strcmp (( (optind     < argc) ? argv[optind    ] : zero ), buffresp[0]) ||
                strcmp (( (optind + 1 < argc) ? argv[optind + 1] : zero ), buffresp[1]) ||
                strcmp (( (optind + 2 < argc) ? argv[optind + 2] : zero ), buffresp[2])) {
                fprintf (stderr, "Aviso: servidor alterou a paleta de cores: pos=%d; r=%s; g=%s; b=%s.\n", t, buffresp[0], buffresp[1], buffresp[2]);
            }
        }
        fprintf (stderr, "Concluido. Criando imagem agora...\n");
        if (fprintf (fdsock, "AGORA\n") < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %lg", &px1) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (fprintf (fdsock, "MAKE\n") < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK") != 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        if (fprintf (fdsock, "AGORA\n") < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        if (fscanf (fdsock, "OK %lg", &px2) != 1) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        t2 = 1000 * (px2 - px1);
        t = (int) t2;
        fprintf (stderr, "Imagem criada com sucesso. Tempo gasto foi de %d ms [ %dmin ; %ds ; %dms ].\n", t, t / 60000, (t / 1000) % 60, t % 1000);
        fprintf (stderr, "Fazendo download da imagem agora...\n");
        px1 = milisecs ();
        if (fprintf (fdsock, "DUMP\n") < 0) {
            perror ("Erro: nao foi possivel comunicar-se com o servidor");
            return (1);
        }
        fflush (fdsock);
        t = 0;
        while ((i = fgetc (fdsock)) != EOF) {
            fputc (i, fdout);
            t++;
        }
        if (saida != NULL)
            fclose (fdout);
        fclose (fdsock);
        px2 = milisecs ();
        t2 = px2 - px1;
        i = (int) t2;
        fprintf (stderr, "Download efetuado com sucesso. Tempo gasto foi de %d ms [ %dmin ; %ds ; %dms ].\n", i, i / 60000, (i / 1000) % 60, i % 1000);
        if (t2) {
            fprintf (stderr, "Taxa de transmissao: %.2f KB/s.\n", t / t2);
        }
        fprintf (stderr, "Concluido.\n");
        return (0);
    }
    fprintf (stderr, "Erro: nao foi possivel conectar-se a nenhum dos enderecos encontrados.\n");
    return (1);
}
