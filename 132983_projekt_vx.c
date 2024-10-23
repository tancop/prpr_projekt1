#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define false (0)
#define true (1)

void v1(FILE **restrict f_data, FILE **restrict f_string,
        FILE **restrict f_parse)
{
    if (!*f_data)
    {
        *f_data = fopen("data.txt", "r");
        if (!*f_data)
        {
            // nepodarilo sa otvoriť súbor
            printf("V1: Neotvorene txt subory.\n");
            return;
        }
    }
    if (!*f_string)
    {
        *f_string = fopen("string.txt", "r");
        if (!*f_string)
        {
            printf("V1: Neotvorene txt subory.\n");
            return;
        }
    }
    if (!*f_parse)
    {
        *f_parse = fopen("parse.txt", "r");
        if (!*f_parse)
        {
            printf("V1: Neotvorene txt subory.\n");
            return;
        }
    }

    // vrátime sa na začiatok súborov
    rewind(*f_data);
    rewind(*f_string);
    rewind(*f_parse);

    // riadok v string.txt má max 6 znakov + \n
    char id_string[7];

    while (fgets(id_string, 7, *f_string))
    {
        if (id_string[0] == '\n')
        {
            // prázdny riadok v string.txt
            printf("ID. mer. modulu:\n");
        }
        else
        {
            printf("ID. mer. modulu: %s\n", id_string);
            // posunieme indikátor na začiatok ďalšieho riadku
            fseek(*f_string, 1, SEEK_CUR);
        }

        int h1;
        double h2;

        // zahodíme prvé dve čísla cez %*
        fscanf(*f_data, "%*d %*d %d %lf", &h1, &h2);
        printf("Hodnota 1: %d\nHodnota 2: %lg\n", h1, h2);

        printf("Poznámka: ");

        int c;

        // vypisujeme znaky z parse.txt do konca riadku
        while ((c = fgetc(*f_parse)) != '\n')
        {
            putchar(c);
        }

        // koniec riadku s poznámkou a prázdny riadok
        printf("\n\n");
    }
}

void v2(int rec_count, int *restrict a_data, double *restrict a_data4,
        char *restrict a_string, char **restrict a_parse,
        int *restrict a_parse_lengths, int *restrict a_deleted, int s_deleted)
{
    for (int i = 0; i < rec_count; ++i)
    {
        // skontrolujeme ci je zaznam vymazany
        int is_deleted = false;
        for (int j = 0; j < s_deleted; ++j)
        {
            if (a_deleted[j] == i)
            {
                // zaznam sme uz vymazali
                is_deleted = true;
                break;
            }
        }
        if (is_deleted)
            continue;

        printf("ID. mer. modulu: ");
        if (a_string[i * 6] != '\n')
        {
            // riadok nie je prazdny
            for (int j = 0; j < 6; ++j)
            {
                putchar(a_string[i * 6 + j]);
            }
        }
        printf("\n");

        // tretie cislo z riadku
        printf("Hodnota 1: %d\n", a_data[i * 3 + 2]);
        printf("Hodnota 2: %g\n", a_data4[i]);

        printf("Poznámka: ");
        // dlzka zaznamu v a_parse
        int length = a_parse_lengths[i];
        for (int j = 0; j < length; ++j)
        {
            putchar(a_parse[i][j]);
        }
        // koniec zaznamu a prazdny riadok
        printf("\n\n");
    }
    return;
}

void v(FILE **restrict f_data, FILE **restrict f_string,
       FILE **restrict f_parse, int rec_count, int *restrict a_data,
       double *restrict a_data4, char *restrict a_string,
       char **restrict a_parse, int *restrict a_parse_lengths,
       int *restrict a_deleted, int s_deleted)
{
    char subcmd;

    scanf(" %c", &subcmd);

    switch (subcmd)
    {
    case '1':
        return v1(f_data, f_string, f_parse);
    case '2':
        return v2(rec_count, a_data, a_data4, a_string, a_parse,
                  a_parse_lengths, a_deleted, s_deleted);
    default:
        printf("V: Nesprávne volba vypisu.\n");
    }
}

void h(FILE *f_string)
{
    if (!f_string)
    {
        // f_string je nulová adresa - neukazuje na súbor
        printf("H: Neotvoreny subor.\n");
        return;
    }

    // vrátime indikátor na začiatok
    rewind(f_string);

    const char *index_to_char =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    // malé + veľké + číslice
#define hist_length (26 + 26 + 10)
    int hist[hist_length] = {0};
    int c;

    while ((c = fgetc(f_string)) != EOF)
    {
        if (c >= '0' && c <= '9')
        {
            // číslica
            hist[(c - '0') + 52]++;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            // veľké písmeno
            hist[(c - 'A')]++;
        }
        else if (c >= 'a' && c <= 'z')
        {
            // malé písmeno
            hist[(c - 'a') + 26]++;
        }
    }

    for (int i = 0; i < hist_length; i++)
    {
        int freq = hist[i];
        if (freq > 0)
            // znak sa nachádza v súbore
            printf("%c : %d\n", index_to_char[i], freq);
    }
}

void n(FILE *restrict f_data, FILE *restrict f_string, FILE *restrict f_parse,
       int *rec_count, int **restrict a_data, double **restrict a_data4,
       char **restrict a_string, char ***restrict a_parse,
       int **restrict a_parse_lengths, int **restrict a_deleted,
       int *restrict s_deleted)
{
    if (!f_data || !f_string || !f_parse)
    {
        printf("N: Neotvoreny subor.\n");
        return;
    }

    rewind(f_data);
    rewind(f_string);
    rewind(f_parse);

    *rec_count = 0;
    if (*a_data)
    {
        // uz bolo vytvorene
        free(*a_data);
    }
    if (*a_data4)
    {
        free(*a_data4);
    }
    if (*a_string)
    {
        free(*a_string);
    }
    if (*a_parse)
    {
        free(*a_parse);
    }
    if (*a_parse_lengths)
    {
        free(*a_parse_lengths);
    }
    if (*a_deleted)
    {
        free(*a_deleted);
        *s_deleted = 0;
    }

    int s_data = 0, s_string = 0, s_parse = 0;
    int c;
    // spocitame riadky v data.txt
    while ((c = fgetc(f_data)) != EOF)
    {
        if (c == '\n')
        {
            ++s_data;
        }
    }
    rewind(f_data);
    while ((c = fgetc(f_string)) != EOF)
    {
        if (c == '\n')
        {
            ++s_string;
        }
    }
    rewind(f_string);
    while ((c = fgetc(f_parse)) != EOF)
    {
        if (c == '\n')
        {
            ++s_parse;
        }
    }
    rewind(f_parse);

    // vyberieme najvacsiu dlzku pola
    int size = s_data;
    if (s_string > size)
        size = s_string;
    if (s_parse > size)
        size = s_parse;

    // nastavime vsetky dlzky na size
    *rec_count = size;

    // kazdy zaznam v data.txt ma 3 inty
    *a_data = (int *)malloc(*rec_count * (3 * sizeof(int)));
    // a 1 double
    *a_data4 = (double *)malloc(*rec_count * sizeof(double));

    for (int i = 0; i < *rec_count; ++i)
    {
        fscanf(f_data, "%d %d %d %lf", &(*a_data)[3 * i], &(*a_data)[3 * i + 1],
               &(*a_data)[3 * i + 2], &(*a_data4)[i]);
    }

    *a_string = (char *)malloc(*rec_count * 6);
    int base = 0, offset = 0;

    // pouzijeme uz deklarovany int c
    while ((c = fgetc(f_string)) != EOF)
    {
        if (c == '\n')
        {
            if (offset == 0)
                // riadok ma dlzku 0
                (*a_string)[base * 6] = '\n';
            // posunieme na dalsi char[6]
            ++base;
            offset = 0;
        }
        else
        {
            // zapiseme znak do pola
            (*a_string)[base * 6 + offset] = c;
            ++offset;
        }
    }

    *a_parse_lengths = (int *)malloc(*rec_count * sizeof(int));

    // pozicia v subore parse.txt
    int f_pos = 0;
    int prev_f_pos = 0;
    // index v poli a_parse_lengths
    int pl_pos = 0;

    while ((c = fgetc(f_parse)) != EOF)
    {
        if (c == '\n')
        {
            (*a_parse_lengths)[pl_pos] = f_pos - prev_f_pos;
            prev_f_pos = f_pos;
            ++pl_pos;
        }
        ++f_pos;
    }

    rewind(f_parse);

    *a_parse = (char **)malloc(*rec_count * sizeof(char *));

    // poloha v poli a_parse
    int p_pos = 0;
    // poloha v riadku parse.txt
    int l_pos = 0;

    (*a_parse)[0] = (char *)malloc((*a_parse_lengths)[0] * sizeof(char));
    // nacitame obsah parse.txt do a_parse
    while ((c = fgetc(f_parse)) != EOF)
    {
        if (c == '\n')
        {
            ++p_pos;
            if (p_pos >= *rec_count)
                // subor konci znakom '\n', nepotrebujeme za tym dalsi riadok
                break;
            // alokujeme vnutorne pole
            (*a_parse)[p_pos] =
                (char *)malloc((*a_parse_lengths)[p_pos] * sizeof(char));
            l_pos = 0;
        }

        (*a_parse)[p_pos][l_pos] = c;

        ++l_pos;
    }
}

void e(int rec_count, char **a_parse, int *a_parse_lengths)
{
    if (!a_parse || !a_parse_lengths)
    {
        printf("E: Polia nie su vytvorene.\n");
        return;
    }
    // hladany retazec - max 20 znakov
    char query[20];
    // dlzka hladaneho retazca
    int query_length = 0;

    // zahodime znak \n z predchadzajuceho prikazu
    getchar();
    // nacitame hladane slovo
    for (int i = 0; i < 20; ++i)
    {
        char c = getchar();
        if (c == '\n')
            // koniec vstupu
            break;
        query[i] = c;
        ++query_length;
    }

    // hladame v kazdom zazname
    for (int i = 0; i < rec_count; ++i)
    {
        int length = a_parse_lengths[i];
        // poloha v poli query
        int query_idx = 0;
        for (int j = 0; j < length; ++j)
        {
            if (a_parse[i][j] == query[query_idx])
            {
                // znak v a_parse[i] sa zhoduje so znakom v query
                ++query_idx;
                if (query_idx == query_length)
                {
                    // nasli sme vsetky znaky, vypiseme riadok
                    for (int k = 0; k < length; ++k)
                    {
                        putchar(a_parse[i][k]);
                    }
                    printf("\n");
                    break;
                }
            }
            else
            {
                // resetujeme polohu v query
                query_idx = 0;
            }
        }
    }
}

void w(int *restrict rec_count, int *restrict a_data, double *restrict a_data4,
       char *restrict a_string, char **restrict a_parse,
       int *restrict a_parse_lengths)
{
    if (!a_data || !a_data4 || !a_string || !a_parse || !a_parse_lengths)
    {
        printf("W: Polia nie su vytvorene.\n");
        return;
    }
    // ID ma vzdy 6 znakov
    char query[6];
    // zahodime znak \n z predchadzajuceho prikazu
    getchar();
    // nacitame hladane ID
    for (int i = 0; i < 6; ++i)
    {
        char c = getchar();
        if (c == '\n')
            // koniec vstupu
            break;
        query[i] = c;
    }

    // indexy zaznamov ktore potrebujeme vymazat
    int *deleted = (int *)malloc(*rec_count * sizeof(int));
    // pocet vymazanych zaznamov
    int deleted_count = 0;
    for (int i = 0; i < *rec_count; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            if (a_string[i * 6 + j] != query[j])
                // ID v zazname sa nerovna hladanemu
                break;
            if (j == 5)
            {
                // sme na konci zaznamu a vsetky znaky sa rovnaju
                deleted[deleted_count] = i;
                ++deleted_count;
            }
        }
    }

    // nastavime aby podmienka (i == deleted[offset]) neplatila
    // pre extra prvok ked deleted[0] = 0
    deleted[deleted_count] = -1;

    if (deleted_count == 0)
    {
        // ziadne zaznamy nemaju hladane ID
        printf("W: Vymazalo sa : 0 zaznamov !\n");
        return;
    }

    // posun cez ktory berieme nove prvky
    int offset = 1;
    // zacneme od prveho vymazaneho zaznamu
    for (int i = deleted[0]; i < (*rec_count - deleted_count); ++i)
    {
        if (i == deleted[offset])
        {
            // uvolnime pole v a_parse
            free(a_parse[i]);
            ++offset;
        }
        a_data[i * 3] = a_data[(i + offset) * 3];
        a_data[i * 3 + 1] = a_data[(i + offset) * 3 + 1];
        a_data[i * 3 + 2] = a_data[(i + offset) * 3 + 2];

        a_data4[i] = a_data4[i + offset];

        // prepiseme cast a_string
        for (int j = 0; j < 6; ++j)
        {
            a_string[i * 6 + j] = a_string[(i + offset) * 6 + j];
        }

        a_parse[i] = a_parse[i + offset];
        a_parse_lengths[i] = a_parse_lengths[i + offset];
    }

    free(deleted);
    // znizime pocet zaznamov o vymazane
    *rec_count -= deleted_count;

    printf("W: Vymazalo sa : %d zaznamov !\n", deleted_count);
}

void q(int *restrict rec_count, int **restrict a_data,
       double **restrict a_data4, char **restrict a_string,
       char ***restrict a_parse, int **restrict a_parse_lengths)
{
    if (!*a_data || !*a_data4 || !*a_string || !*a_parse || !*a_parse_lengths)
    {
        printf("Q: Polia nie su vytvorene.\n");
        return;
    }

    int pos;

    // nacitame poziciu a prevedieme na 0 based index
    scanf("%d", &pos);
    --pos;

    if (pos > *rec_count)
    {
        // pridavame na koniec pola
        pos = *rec_count + 1;
        ++(*rec_count);

        // realokujeme polia na novu velkost
        *a_data = (int *)realloc(*a_data, *rec_count * (3 * sizeof(int)));
        *a_data4 = (double *)realloc(*a_data, *rec_count * sizeof(double));
        *a_string = (char *)realloc(*a_string, *rec_count * (6 * sizeof(char)));
        *a_parse = (char **)realloc(*a_parse, *rec_count * sizeof(char *));
        *a_parse_lengths =
            (int *)realloc(*a_parse_lengths, *rec_count * sizeof(int));
    }

    // zahodime znak \n z prikazu
    getchar();
    // nacitame vstup do pola
    for (int i = 0; i < 6; ++i)
    {
        (*a_string)[(pos * 6) + i] = getchar();
    }
    // zahodime znak \n
    getchar();

    scanf("%d %d %d %lf", &(*a_data)[pos * 3], &(*a_data)[pos * 3 + 1],
          &(*a_data)[pos * 3 + 2], &(*a_data4)[pos]);

    // zaznam v parse.txt ma max 500 znakov
    char buf[500];
    // dlzka zaznamu
    int buf_size = 0;
    char c;
    getchar();
    while ((c = getchar()) != '\n')
    {
        buf[buf_size] = c;
        ++buf_size;
    }

    if ((*a_parse)[pos])
        free((*a_parse)[pos]);
    // skopirujeme vstup do noveho pola
    (*a_parse)[pos] = (char *)malloc(buf_size * sizeof(char));
    for (int i = 0; i < buf_size; ++i)
    {
        (*a_parse)[pos][i] = buf[i];
    }

    (*a_parse_lengths)[pos] = buf_size;
}

int main()
{
    FILE *f_data = NULL, *f_string = NULL, *f_parse = NULL;
    int *a_data = NULL;          // prve 3 cisla zaznamu v data.txt
    double *a_data4 = NULL;      // posledne cislo zaznamu
    char *a_string = NULL;       // zaznam v string.txt
    char **a_parse = NULL;       // zaznam v parse.txt
    int *a_parse_lengths = NULL; // dlzky zaznamov parse.txt
    int *a_deleted = NULL;       // vymazane zaznamy na recyklaciu
    int s_deleted = 0;           // velkost a_deleted

    int rec_count = 0; // max pocet zaznamov v dynamickych poliach

    char cmd;
    int should_end = false;

    do
    {
        // nechceme načítať \n pred znakom
        scanf(" %c", &cmd);

        switch (cmd)
        {
        case 'v':
            v(&f_data, &f_string, &f_parse, rec_count, a_data, a_data4,
              a_string, a_parse, a_parse_lengths, a_deleted, s_deleted);
            break;
        case 'h':
            h(f_string);
            break;
        case 'n':
            n(f_data, f_string, f_parse, &rec_count, &a_data, &a_data4,
              &a_string, &a_parse, &a_parse_lengths, &a_deleted, &s_deleted);
            break;
        case 'e':
            e(rec_count, a_parse, a_parse_lengths);
            break;
        case 'w':
            w(&rec_count, a_data, a_data4, a_string, a_parse, a_parse_lengths);
            break;
        case 'q':
            q(&rec_count, &a_data, &a_data4, &a_string, &a_parse,
              &a_parse_lengths);
            break;
        default:
            // príkaz nie je podporovaný
            should_end = true;
            break;
        }
    } while (!should_end);

    // zatvoríme všetky súbory ak sú otvorené
    if (f_data)
        fclose(f_data);
    if (f_string)
        fclose(f_string);
    if (f_parse)
        fclose(f_parse);

    // uvolnime dynamicke polia
    if (a_data)
        free(a_data);
    if (a_data4)
        free(a_data4);
    if (a_string)
        free(a_string);
    if (a_parse)
    {
        for (int i = 0; i < rec_count + 1; ++i)
        {
            if (a_parse[i])
                free(a_parse[i]);
        }
        free(a_parse);
    }
    if (a_parse_lengths)
    {
        free(a_parse_lengths);
    }
    if (a_deleted)
        free(a_deleted);

    return 0;
}
