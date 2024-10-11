#include <stddef.h>
#include <stdio.h>

// program po spustení čaká na nový príkaz
#define RV_CONTINUE (0)
// program po spustení príkazu skončí
#define RV_END (1)

int v1(FILE **restrict f_data, FILE **restrict f_string,
       FILE **restrict f_parse)
{
    if (!*f_data)
    {
        *f_data = fopen("data.txt", "r");
        if (!*f_data)
        {
            // nepodarilo sa otvoriť súbor
            printf("V1: Neotvorene txt subory.\n");
            return RV_CONTINUE;
        }
    }
    if (!*f_string)
    {
        *f_string = fopen("string.txt", "r");
        if (!*f_string)
        {
            printf("V1: Neotvorene txt subory.\n");
            return RV_CONTINUE;
        }
    }
    if (!*f_parse)
    {
        *f_parse = fopen("parse.txt", "r");
        if (!*f_parse)
        {
            printf("V1: Neotvorene txt subory.\n");
            return RV_CONTINUE;
        }
    }

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
    return RV_CONTINUE;
}

int v(FILE **restrict f_data, FILE **restrict f_string, FILE **restrict f_parse)
{
    char subcmd;

    scanf(" %c", &subcmd);

    switch (subcmd)
    {
    case '1':
        return v1(f_data, f_string, f_parse);
    default:
        printf("V: Nesprávne volba vypisu.\n");
        return RV_CONTINUE;
    }
}

int h(FILE *f_string)
{
    if (!f_string)
    {
        // f_string je nulová adresa - neukazuje na súbor
        printf("H: Neotvoreny subor.\n");
        return RV_CONTINUE;
    }

    printf("todo h\n");

    return RV_END;
}

int main()
{
    FILE *f_data = NULL, *f_string = NULL, *f_parse = NULL;
    char cmd;
    int cmd_return;

    do
    {
        scanf(" %c", &cmd);

        switch (cmd)
        {
        case 'v':
            cmd_return = v(&f_data, &f_string, &f_parse);
            break;
        case 'h':
            cmd_return = h(f_string);
            break;
        default:
            cmd_return = RV_END;
        }
    } while (cmd_return == RV_CONTINUE);

    return 0;
}
