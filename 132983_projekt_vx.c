#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define false (0)
#define true (1)

typedef struct DataRecord
{
    int id, zn, h1;
    double h2;
} DataRecord;

typedef struct ParseRecord
{
    char id[6];
    int n1, hodina, minuta;
    char *t;
    // dlzka retazca t
    int t_length;
} ParseRecord;

typedef struct Node
{
    char id[6];
    DataRecord data;
    ParseRecord parse;
    struct Node *prev;
    struct Node *next;
} Node;

void v1(FILE **f_data, FILE **f_string, FILE **f_parse)
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
        *f_string = fopen("string.txt", "rb");
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

    /* riadok v string.txt má max 6 znakov + \r\n + \0 */
    char id_string[9];

    while (fgets(id_string, 8, *f_string) != NULL)
    {
        if (id_string[0] == '\r' || id_string[0] == '\n')
        {
            /* prázdny riadok v string.txt */
            printf("ID. mer. modulu:\n");
        }
        else
        {
            printf("ID. mer. modulu: %6s\n", id_string);
            /* posunieme indikátor na začiatok ďalšieho riadku */
            fseek(*f_string, 1, SEEK_CUR);
        }

        int h1;
        double h2;

        /* zahodíme prvé dve čísla cez %* */
        fscanf(*f_data, "%*d %*d %d %lf", &h1, &h2);
        printf("Hodnota 1: %d\nHodnota 2: %lg\n", h1, h2);

        printf("Poznámka: ");

        int c;

        /* vypisujeme znaky z parse.txt do konca riadku */
        while ((c = fgetc(*f_parse)) != '\n' && c != '\r')
        {
            putchar(c);
        }
        if (c == '\r')
        {
            fgetc(*f_parse);
        }

        /* koniec riadku s poznámkou a prázdny riadok */
        printf("\n\n");
    }
    fflush(stdout);
}

void v2(int rec_count, int *a_data, double *a_data4, char *a_string,
        char **a_parse, int *a_parse_lengths)
{
    if (!a_data || !a_data4 || !a_string || !a_parse)
    {
        printf("V2: Nenaplnene polia.\n");
        return;
    }
    for (int i = 0; i < rec_count; ++i)
    {
        printf("ID. mer. modulu: ");
        if (a_string[i * 6] != '\n' && a_string[i * 6] != '\r')
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
    fflush(stdout);
}

void v3(Node *list)
{
    if (!list)
    {
        printf("V3: Nenaplnený spajany zoznam.\n");
        return;
    }
}

void v(FILE **f_data, FILE **f_string, FILE **f_parse, int rec_count,
       int *a_data, double *a_data4, char *a_string, char **a_parse,
       int *a_parse_lengths, Node *list)
{
    char subcmd;

    scanf(" %c", &subcmd);

    switch (subcmd)
    {
    case '1':
        v1(f_data, f_string, f_parse);
        break;
    case '2':
        v2(rec_count, a_data, a_data4, a_string, a_parse, a_parse_lengths);
        break;
    case '3':
        v3(list);
        break;
    default:
        printf("V: Nesprávne volba vypisu.\n");
        fflush(stdout);
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
    fflush(stdout);
}

void n(FILE *f_data, FILE *f_string, FILE *f_parse, int *rec_count,
       int **a_data, double **a_data4, char **a_string, char ***a_parse,
       int **a_parse_lengths)
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
        if (c == '\n' || c == '\r')
        {
            if (offset == 0)
                // riadok ma dlzku 0
                (*a_string)[base * 6] = '\n';
            // posunieme na dalsi char[6]
            ++base;
            offset = 0;

            // zahodime prvy znak CRLF
            if (c == '\r')
                fgetc(f_string);
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
    int f_pos = 1;
    int prev_f_pos = 0;
    // index v poli a_parse_lengths
    int pl_pos = 0;

    while ((c = fgetc(f_parse)) != EOF)
    {
        if (c == '\n')
        {
            (*a_parse_lengths)[pl_pos] = f_pos - prev_f_pos - 1;
            prev_f_pos = f_pos;
            ++pl_pos;
        }
        else if (c == '\r')
        {
            (*a_parse_lengths)[pl_pos] = f_pos - prev_f_pos - 1;
            prev_f_pos = f_pos;
            ++pl_pos;
            // zahodime druhy znak CRLF
            fgetc(f_parse);
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
        if (c == '\n' || c == '\r')
        {
            ++p_pos;
            if (p_pos >= *rec_count)
                // subor konci znakom '\n', nepotrebujeme za tym dalsi riadok
                break;
            // alokujeme vnutorne pole
            (*a_parse)[p_pos] =
                (char *)malloc((*a_parse_lengths)[p_pos] * sizeof(char));
            l_pos = 0;

            // preskocime znak \r v CRLF
            if (c == '\r')
                fgetc(f_parse);
            // preskocime znak \n
            c = fgetc(f_parse);
        }

        (*a_parse)[p_pos][l_pos] = c;

        ++l_pos;
    }
    fflush(stdout);
}

void m(FILE *f_data, FILE *f_string, FILE *f_parse, Node **list)
{
    if (!f_data || !f_string || !f_parse)
    {
        printf("M: Neotvoreny subor.\n");
        return;
    }

    rewind(f_data);
    rewind(f_string);
    rewind(f_parse);

    if (*list)
    {
        Node *node = *list;
        while (node->next != NULL)
        {
            node = node->next;
            free(node->id);
            free(node->parse.t);
            free(node->parse.id);
            free(node);
        }
        free(node->id);
        free(node->parse.t);
        free(node->parse.id);
        free(node);
    }

    // dlzka zoznamu
    int list_length = 0;
    Node *head;
    Node *current;

    /* riadok v string.txt má max 6 znakov + \r\n + \0 */
    char id_string[9];

    while (fgets(id_string, 8, f_string) != NULL)
    {
        char id[6] = {0};
        if (id_string[0] != '\r' && id_string[0] != '\n')
        {
            for (int i = 0; i < 6; ++i)
            {
                id[i] = id_string[i];
            }
            /* posunieme indikátor na začiatok ďalšieho riadku */
            fseek(f_string, 1, SEEK_CUR);
        }

        int h_id, h_zn, h1;
        double h2;

        /* zahodíme prvé dve čísla cez %* */
        fscanf(f_data, "%d %d %d %lf", &h_id, &h_zn, &h1, &h2);

        int c;

        // max dlzka riadku v parse.txt
        char buf[500];
        int buf_length = 0;

        // 6 znakov ID + \0
        char p_id[7] = {0};
        double p_n1 = 0;
        int timestamp = 0;

        // retazec na konci poznamky
        char *p_t = NULL;
        int t_length = 0;

        int part = 0;
        int last_index = 0;

        fgets(buf, 500, f_parse);

        for (int i = 0; i < 500; ++i)
        {
            c = buf[i];
            if (c == '#')
            {
                // oddelovac #
                switch (part)
                {
                case 0:
                    // retazec ID
                    if (i - last_index > 1)
                    {
                        // mame ID segment
                        for (int j = 0; j < i; ++j)
                        {
                            p_id[j] = buf[j];
                        }
                        printf("[id] i: %d, last_index: %d, id: %6s\n", i,
                               last_index, p_id);
                    }
                    break;
                case 1:
                    // N1
                    if (i - last_index > 1)
                    {
                        sscanf(buf + last_index + 1, "%lf", &p_n1);
                        printf("[n1] i: %d, last_index: %d, p_n1: %g\n", i,
                               last_index, p_n1);
                    }
                    break;
                case 2:
                    // timestamp
                    if (i - last_index > 1)
                    {
                        sscanf(buf + last_index + 1, "%d", &timestamp);
                        printf("[ts] i: %d, last_index: %d, timestamp: %d\n", i,
                               last_index, timestamp);
                    }
                    break;
                case 3:
                    // poznamka
                    if (i - last_index > 1)
                    {
                        p_t = (char *)malloc(buf_length * sizeof(char));
                        for (int i = last_index + 1; i < buf_length; ++i)
                        {
                            p_t[i - 1] = buf[i];
                        }
                        p_t[buf_length - 1] = '\0';
                        t_length = i - last_index;
                        printf("[str] i: %d, last_index: %d, p_t: %s, "
                               "t_length: %d\n",
                               i, last_index, p_t, t_length);
                    }
                    break;
                }
                ++part;
                last_index = i;
            }
            else if (c == '\n' || c == '\r')
            {
                // koniec riadku
                break;
            }
            ++buf_length;
        }

        // celociselne delenie na prve 2 cislice
        int p_hod = timestamp / 100;
        // modulo na posledne 2
        int p_min = timestamp % 100;

        printf("cas: %d:%d\n", p_hod, p_min);

        DataRecord data = {h_id, h_zn, h1, h2};
        ParseRecord parse = {
            {p_id[0], p_id[1], p_id[2], p_id[3], p_id[4], p_id[5]},
            p_n1,
            p_hod,
            p_min,
            p_t,
            buf_length};

        Node *node = (Node *)malloc(sizeof(Node));

        for (int i = 0; i < 6; ++i)
        {
            node->id[i] = id[i];
        }
        node->data = data;
        node->parse = parse;
        node->next = NULL;

        if (list_length == 0)
        {
            // uplny zaciatok
            node->prev = NULL;
            head = node;
        }
        else if (list_length == 1)
        {
            // je naplneny head ale nie current
            node->prev = head;
            current = node;
            head->next = node;
        }
        else
        {
            node->prev = current;
            current->next = node;
            current = node;
        }

        ++list_length;
    }

    *list = head;
    fflush(stdout);
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
        if (c == '\n' || c == '\r')
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
    fflush(stdout);
}

void w(int *rec_count, int **a_data, double **a_data4, char **a_string,
       char ***a_parse, int **a_parse_lengths)
{
    if (!*a_data || !*a_data4 || !*a_string || !*a_parse || !*a_parse_lengths)
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
        if (c == '\n' || c == '\r')
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
            if ((*a_string)[i * 6 + j] != query[j])
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

    // uvolnime polia v a_parse pre vymazane zaznamy
    int deleted_pos = 0;
    for (int i = 0; i < *rec_count; ++i)
    {
        if (i == deleted[deleted_pos])
        {
            free((*a_parse)[i]);
            ++deleted_pos;
        }
    }

    // posun cez ktory berieme nove prvky
    int offset = 1;
    // zacneme od prveho vymazaneho zaznamu a kopirujeme prvky dozadu
    for (int i = deleted[0]; i < (*rec_count - deleted_count); ++i)
    {
        if (i == deleted[deleted_pos])
        {
            ++offset;
        }

        (*a_data)[i * 3] = (*a_data)[(i + offset) * 3];
        (*a_data)[i * 3 + 1] = (*a_data)[(i + offset) * 3 + 1];
        (*a_data)[i * 3 + 2] = (*a_data)[(i + offset) * 3 + 2];

        (*a_data4)[i] = (*a_data4)[i + offset];

        // prepiseme cast a_string
        for (int j = 0; j < 6; ++j)
        {
            (*a_string)[i * 6 + j] = (*a_string)[(i + offset) * 6 + j];
        }

        (*a_parse)[i] = (*a_parse)[i + offset];
        (*a_parse_lengths)[i] = (*a_parse_lengths)[i + offset];
    }

    free(deleted);
    // znizime pocet zaznamov o vymazane
    *rec_count -= deleted_count;

    // realokujeme polia na mensiu velkost
    *a_data = (int *)realloc(*a_data, *rec_count * (3 * sizeof(int)));
    *a_data4 = (double *)realloc(*a_data4, *rec_count * sizeof(double));
    *a_string = (char *)realloc(*a_string, *rec_count * (6 * sizeof(char)));
    *a_parse = (char **)realloc(*a_parse, *rec_count * sizeof(char *));
    *a_parse_lengths =
        (int *)realloc(*a_parse_lengths, *rec_count * sizeof(int));

    printf("W: Vymazalo sa : %d zaznamov !\n", deleted_count);
    fflush(stdout);
}

void q(int *rec_count, int **a_data, double **a_data4, char **a_string,
       char ***a_parse, int **a_parse_lengths)
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

    if (pos >= *rec_count)
    {
        // pridavame na koniec pola
        pos = *rec_count;
    }
    ++(*rec_count);

    // realokujeme polia na novu velkost
    *a_data = (int *)realloc(*a_data, *rec_count * (3 * sizeof(int)));
    *a_data4 = (double *)realloc(*a_data4, *rec_count * sizeof(double));
    *a_string = (char *)realloc(*a_string, *rec_count * (6 * sizeof(char)));
    *a_parse = (char **)realloc(*a_parse, *rec_count * sizeof(char *));
    *a_parse_lengths =
        (int *)realloc(*a_parse_lengths, *rec_count * sizeof(int));

    // posunieme zaznamy za cielovym indexom o 1 dozadu
    for (int i = *rec_count - 1; i > pos; --i)
    {
        (*a_data)[3 * i] = (*a_data)[3 * (i - 1)];
        (*a_data)[3 * i + 1] = (*a_data)[3 * (i - 1) + 1];
        (*a_data)[3 * i + 2] = (*a_data)[3 * (i - 1) + 2];

        (*a_data4)[i] = (*a_data4)[i - 1];

        for (int j = 0; j < 6; ++j)
        {
            (*a_string)[6 * i + j] = (*a_string)[6 * (i - 1) + j];
        }

        (*a_parse)[i] = (*a_parse)[i - 1];
        (*a_parse_lengths)[i] = (*a_parse_lengths)[i - 1];
    }

    // zahodime znak \n z prikazu
    getchar();
    // nacitame vstup do pola
    for (int i = 0; i < 6; ++i)
    {
        (*a_string)[(pos * 6) + i] = getchar();
    }
    // zahodime znak \n za vstupom
    getchar();

    scanf("%d %d %d %lf", &(*a_data)[pos * 3], &(*a_data)[pos * 3 + 1],
          &(*a_data)[pos * 3 + 2], &(*a_data4)[pos]);

    // zaznam v parse.txt ma max 500 znakov
    char buf[500];
    // dlzka zaznamu
    int buf_size = 0;
    char c;
    getchar();
    while ((c = getchar()) != '\n' && c != '\r')
    {
        buf[buf_size] = c;
        ++buf_size;
    }

    // skopirujeme vstup do noveho pola
    (*a_parse)[pos] = (char *)malloc(buf_size * sizeof(char));
    for (int i = 0; i < buf_size; ++i)
    {
        (*a_parse)[pos][i] = buf[i];
    }

    (*a_parse_lengths)[pos] = buf_size;
    fflush(stdout);
}

int main(void)
{
    FILE *f_data = NULL, *f_string = NULL, *f_parse = NULL;
    int *a_data = NULL;          // prve 3 cisla zaznamu v data.txt
    double *a_data4 = NULL;      // posledne cislo zaznamu
    char *a_string = NULL;       // zaznam v string.txt
    char **a_parse = NULL;       // zaznam v parse.txt
    int *a_parse_lengths = NULL; // dlzky zaznamov parse.txt
    Node *list = NULL;           // zaciatok spajaneho zoznamu

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
              a_string, a_parse, a_parse_lengths, list);
            break;
        case 'h':
            h(f_string);
            break;
        case 'n':
            n(f_data, f_string, f_parse, &rec_count, &a_data, &a_data4,
              &a_string, &a_parse, &a_parse_lengths);
            break;
        case 'e':
            e(rec_count, a_parse, a_parse_lengths);
            break;
        case 'w':
            w(&rec_count, &a_data, &a_data4, &a_string, &a_parse,
              &a_parse_lengths);
            break;
        case 'q':
            q(&rec_count, &a_data, &a_data4, &a_string, &a_parse,
              &a_parse_lengths);
            break;
        case 'm':
            m(f_data, f_string, f_parse, &list);
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
        for (int i = 0; i < rec_count; ++i)
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

    // uvolnime spajany zoznam
    if (list)
    {
        Node *next_node;

        while (list)
        {
            next_node = list->next;
            free(list->parse.t);
            free(list);
            list = next_node;
        }
    }
    return 0;
}
