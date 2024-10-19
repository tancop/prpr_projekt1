#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
    char id_string[6];

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

void v(FILE **restrict f_data, FILE **restrict f_string,
       FILE **restrict f_parse)
{
    char subcmd;

    scanf(" %c", &subcmd);

    switch (subcmd)
    {
    case '1':
        return v1(f_data, f_string, f_parse);
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
       int **restrict a_data, int *restrict s_data, double **restrict a_data4,
       char **restrict a_string, int *restrict s_string,
       char **restrict a_parse, int *restrict s_parse,
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

    if (*a_data)
    {
        // uz bolo vytvorene
        free(*a_data);
        *s_data = 0;
    }
    if (*a_data4)
    {
        free(*a_data4);
    }
    if (*a_string)
    {
        free(*a_string);
        *s_string = 0;
    }
    if (*a_parse)
    {
        free(*a_parse);
        *s_parse = 0;
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

    int c;
    // spocitame riadky v data.txt
    while ((c = fgetc(f_data)) != EOF)
    {
        if (c == '\n')
        {
            ++(*s_data);
        }
    }
    rewind(f_data);
    while ((c = fgetc(f_string)) != EOF)
    {
        if (c == '\n')
        {
            ++(*s_string);
        }
    }
    rewind(f_string);
    while ((c = fgetc(f_parse)) != EOF)
    {
        if (c == '\n')
        {
            ++(*s_parse);
        }
    }
    rewind(f_parse);

    printf("riadky: data %d, string %d, parse %d\n", *s_data, *s_string,
           *s_parse);
    // vyberieme najvacsiu dlzku pola
    int size = *s_data;
    if (*s_string > size)
        size = *s_string;
    if (*s_parse > size)
        size = *s_parse;

    // kazdy zaznam v data.txt ma 3 inty
    *a_data = (int *)malloc(*s_data * (3 * sizeof(int)));
    // a 1 double
    *a_data4 = (double *)malloc(*s_data * sizeof(double));

    for (int i = 0; i < *s_data; ++i)
    {
        fscanf(f_data, "%d %d %d %lf", &(*a_data)[3 * i], &(*a_data)[3 * i + 1],
               &(*a_data)[3 * i + 2], &(*a_data4)[i]);
    }

    *a_string = (char *)malloc(*s_string * 6);
    int base = 0, offset = 0;

    // pouzijeme uz deklarovany int c
    while ((c = fgetc(f_string)) != EOF)
    {
        if (c == '\n')
        {
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

    *a_parse_lengths = (int *)malloc(*s_parse * sizeof(int));

    // pozicia v subore parse.txt
    int f_pos = 0;
    int prev_f_pos = 0;
    // index v poli a_parse_lengths
    int pl_pos = 0;

    // dlzka celeho pola a_parse
    int total_length = 0;

    while ((c = fgetc(f_parse)) != EOF)
    {
        if (c == '\n')
        {
            (*a_parse_lengths)[pl_pos] = f_pos - prev_f_pos;
            prev_f_pos = f_pos;
            f_pos = 0;
            ++pl_pos;
        }
        ++f_pos;
        ++total_length;
    }

    rewind(f_parse);

    *a_parse = (char *)malloc(total_length);

    for (int i; i < total_length; ++i)
    {
        if ((c = fgetc(f_parse)) == '\n')
        {
            // nacitame prvy znak z dalsieho riadku
            c = fgetc(f_parse);
        }
        (*a_parse)[i] = c;
    }
}

int main()
{
    FILE *f_data = NULL, *f_string = NULL, *f_parse = NULL;
    int *a_data = NULL;          // prve 3 cisla zaznamu v data.txt
    int s_data = 0;              // velkost a_data
    double *a_data4 = NULL;      // posledne cislo zaznamu
    char *a_string = NULL;       // zaznam v string.txt
    int s_string = 0;            // velkost a_string
    char *a_parse = NULL;        // zaznam v parse.txt
    int *a_parse_lengths = NULL; // dlzky retazcov v a_parse
    int s_parse = 0;             // pocet zaznamov v a_parse
    int *a_deleted = NULL;       // vymazane zaznamy na recyklaciu
    int s_deleted = 0;           // velkost s_deleted

    char cmd;
    int should_end = 0;

    do
    {
        // nechceme načítať \n pred znakom
        scanf(" %c", &cmd);

        switch (cmd)
        {
        case 'v':
            v(&f_data, &f_string, &f_parse);
            break;
        case 'h':
            h(f_string);
            break;
        case 'n':
            n(f_data, f_string, f_parse, &a_data, &s_data, &a_data4, &a_string,
              &s_string, &a_parse, &s_parse, &a_parse_lengths, &a_deleted,
              &s_deleted);
            break;
        default:
            // príkaz nie je podporovaný
            should_end = 1;
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
        free(a_parse);
    if (a_parse_lengths)
        free(a_parse_lengths);
    if (a_deleted)
        free(a_deleted);

    return 0;
}
