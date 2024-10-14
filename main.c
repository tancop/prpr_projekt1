#include <stddef.h>
#include <stdio.h>

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

    rewind(*f_data);
    rewind(*f_string);
    rewind(*f_parse);

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
    int hist[26 + 26 + 10] = {0};
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

    for (int i = 0; i < (26 + 26 + 10); i++)
    {
        int freq = hist[i];
        if (freq > 0)
            printf("%c : %d\n", index_to_char[i], freq);
    }
}

int main()
{
    FILE *f_data = NULL, *f_string = NULL, *f_parse = NULL;
    char cmd;
    int should_end = 0;

    do
    {
        scanf(" %c", &cmd);

        switch (cmd)
        {
        case 'v':
            v(&f_data, &f_string, &f_parse);
            break;
        case 'h':
            h(f_string);
            break;
        default:
            should_end = 1;
            break;
        }
    } while (!should_end);

    return 0;
}
