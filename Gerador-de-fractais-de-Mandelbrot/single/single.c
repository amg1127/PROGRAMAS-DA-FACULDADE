/* single.c */

#define _GNU_SOURCE

#include <getopt.h>
#include <gd.h>
#include <stdio.h>
#include <stdlib.h>
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

struct cplx {
    double real, imag;
};

struct cor {
    int vermelho, verde, azul;
    int gdCor;
};

struct cplx soma (struct cplx num1, struct cplx num2) {
    struct cplx res;
    res.real = num1.real + num2.real;
    res.imag = num1.imag + num2.imag;
    return (res);
}

struct cplx mult (struct cplx num1, struct cplx num2) {
    struct cplx res;
    res.real = num1.real * num2.real - num1.imag * num2.imag;
    res.imag = num1.real * num2.imag + num1.imag * num2.real;
    return (res);
}

int iteracao_mandelbrot (double x, double y, int maxiter) {
    int i;
    struct cplx num, z;
    num.real = x;
    num.imag = y;
    z.real = 0;
    z.imag = 0;
    for (i=0; i<maxiter; i++) {
        z = soma (mult (z, z), num);
        if (z.real * z.real + z.imag * z.imag > 4)
            break;
    }
    return (i);
}

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
                   "\n", argv0, MAXITERDEFAULT, LARGDEFAULT, ALTDEFAULT, MINXDEF, MAXXDEF, MINYDEF, MAXYDEF);
}

int main (int argc, char **argv) {
    gdImagePtr vazio = NULL;
    int largura = 0;
    int altura = 0;
    int maxiter = 0;
    int paletas = 0;
    int i = 0;
    int t = 0;
    int x = 0;
    int y = 0;
    double dx = 0;
    double dy = 0;
    double px1 = 0;
    double px2 = 0;
    double py1 = 0;
    double py2 = 0;
    double diffx = 0;
    double diffy = 0;
    double t2 = 0;
    int *iteras = NULL;
    char *saida = NULL;
    char *ptr = NULL;
    FILE *fdout = NULL;
    struct option args[] = {        { "help",           no_argument,       NULL, 'h' },
                                    { "output",         required_argument, NULL, 'o' },
                                    { "max-iterations", required_argument, NULL, 'm' },
                                    { "largura",        required_argument, NULL, 'l' },
                                    { "altura",         required_argument, NULL, 'a' },
                                    { "xmin",           required_argument, NULL, 'x' },
                                    { "xmax",           required_argument, NULL, 'X' },
                                    { "ymin",           required_argument, NULL, 'y' },
                                    { "ymax",           required_argument, NULL, 'Y' },
                                    { NULL,             0,                 NULL,  0  }        };
    struct cor *vetor = NULL;
    /* Processar argumentos da linha de comando */
    opterr = -1;
    while ((i = getopt_long (argc, argv, "ho:m:l:a:x:X:y:Y:", args, NULL)) != -1) {
        switch (i) {
            case 'h':
                usage (stdout, argv[0]);
                return (0);
            case 'o':
                assert (optarg != NULL);
                saida = optarg;
                break;
            case 'm':
            case 'l':
            case 'a':
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
                else
                    altura = t;
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
    if (px1 == px2) {
        fprintf (stderr, "Erro: intervalo no eixo X tem tamanho zero.\n");
        return (1);
    }
    if (py1 == py2) {
        fprintf (stderr, "Erro: intervalo no eixo Y tem tamanho zero.\n");
        return (1);
    }
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
    do
        vetor = (struct cor*) malloc (sizeof (struct cor) * paletas);
    while (vetor == NULL);
    do
        iteras = (int*) malloc (sizeof (int) * largura * altura);
    while (iteras == NULL);
    for (t = optind, i = 0; i < paletas; i++) {
        if (t >= argc) {
            vetor[i].vermelho = 0;
        } else {
            vetor[i].vermelho = strtol (argv[t++], &ptr, 10);
            if (errno != 0 || strcmp (ptr, "")) {
                fprintf (stderr, "Erro: '%s': numero invalido.\n", argv[--t]);
                usage (stderr, argv[0]);
                return (1);
            }
        }
        if (t >= argc) {
            vetor[i].verde = 0;
        } else {
            vetor[i].verde = strtol (argv[t++], &ptr, 10);
            if (errno != 0 || strcmp (ptr, "")) {
                fprintf (stderr, "Erro: '%s': numero invalido.\n", argv[--t]);
                usage (stderr, argv[0]);
                return (1);
            }
        }
        if (t >= argc) {
            vetor[i].azul = 0;
        } else {
            vetor[i].azul = strtol (argv[t++], &ptr, 10);
            if (errno != 0 || strcmp (ptr, "")) {
                fprintf (stderr, "Erro: '%s': numero invalido.\n", argv[--t]);
                usage (stderr, argv[0]);
                return (1);
            }
        }
    }
    if (maxiter < paletas) {
        fprintf (stderr, "Aviso: numero maximo de iteracoes de Mandelbrot foi aumentado para '%d'.\n", maxiter = paletas);
    }
    if (saida != NULL) {
        if ((fdout = fopen (saida, "wb")) == NULL) {
            perror ("Erro: o arquivo de saida nao pode ser aberto");
            return (1);
        }
    } else {
        fdout = stdout;
    }
    fprintf (stderr, "Pronto para gerar a imagem na area complexa: ( %f, %f ) x ( %f, %f ).\n", px1, py1, px2, py2);
    /* Agora eh a melhor parte... Desenhar o fractal */
    fprintf (stderr, "1. Calculando iteracoes de Mandelbrot...\n");
    t2 = -milisecs ();
    diffx = px2 - px1;
    diffy = py2 - py1;
    i = 0;
    for (x=0; x<largura; x++) {
        for (y=0; y<altura; y++) {
            dx = px1 + diffx * x / largura;
            dy = py1 + diffy * y / altura;
            iteras[i++] = iteracao_mandelbrot(dx, dy, maxiter) % paletas;
        }
    }
    t2 += milisecs ();
    i = (int) t2;
    fprintf (stderr, "   A operacao foi concluida. Tempo gasto foi de %d ms [ %dmin ; %ds ; %dms ].\n", i, i / 60000, (i / 1000) % 60, i % 1000);
    t2 = -milisecs ();
    /* Criar a imagem */
    fprintf (stderr, "2. Gerando imagem a partir da contagem de iteracoes...\n");
    vazio = gdImageCreateTrueColor (largura, altura);
    if (vazio == NULL) {
        fprintf (stderr, "Erro: nao foi possivel criar uma imagem com o tamanho especificado.\n");
        return (1);
    }
    for (i=0; i<paletas; i++) {
        vetor[i].gdCor = gdImageColorResolve (vazio, vetor[i].vermelho, vetor[i].verde, vetor[i].azul);
    }
    i = 0;
    for (x=0; x<largura; x++) {
        for (y=0; y<altura; y++) {
            gdImageSetPixel (vazio, x, y, vetor[ iteras[i++] ].gdCor);
        }
    }
    t2 += milisecs ();
    i = (int) t2;
    fprintf (stderr, "   Imagem criada com sucesso. Tempo gasto foi de %d ms [ %dmin ; %ds ; %dms ].\n", i, i / 60000, (i / 1000) % 60, i % 1000);
    fprintf (stderr, "3. Descarregando imagem agora...\n");
    gdImagePng (vazio, fdout);
    if (saida != NULL)
        fclose (fdout);
    gdImageDestroy (vazio);
    free (vetor);
    free (iteras);
    fprintf (stderr, "Concluido.\n");
    return (0);
}

/* single.c */
