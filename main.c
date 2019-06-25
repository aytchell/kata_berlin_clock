#include <stdio.h>

#define sec_per_min     60
#define min_per_hour    60
#define hour_per_day    24
#define sec_per_hour    (sec_per_min * min_per_hour)
#define sec_per_day     (sec_per_hour * hour_per_day)

#define sec_in_h(x)     (sec_per_hour * x)
#define sec_in_m(x)     (sec_per_min * x)

unsigned int *set_compare_row(
        unsigned int value, unsigned int *pos, unsigned int num)
{
    for (unsigned int i = 0; i < num; ++i, ++pos) *pos = value;
    return ++pos;
}

void fill_compares(unsigned int seconds, unsigned int *compares)
{
    unsigned int *mod_ptr = compares;

    mod_ptr = set_compare_row(seconds % 4,               mod_ptr,  1);
    mod_ptr = set_compare_row(seconds % (sec_per_day+1), mod_ptr,  4);
    mod_ptr = set_compare_row(seconds % sec_in_h(5),     mod_ptr,  4);
    mod_ptr = set_compare_row(seconds % sec_per_hour,    mod_ptr, 11);
    mod_ptr = set_compare_row(seconds % sec_in_m(5),     mod_ptr,  4);
}

static const unsigned int ge_than[] = {
    2, 0,

    sec_in_h(5), sec_in_h(10), sec_in_h(15), sec_in_h(20), 0,

    sec_in_h(1), sec_in_h(2), sec_in_h(3), sec_in_h(4), 0,

    sec_in_m( 5), sec_in_m(10), sec_in_m(15), sec_in_m(20),
    sec_in_m(25), sec_in_m(30), sec_in_m(35), sec_in_m(40),
    sec_in_m(45), sec_in_m(50), sec_in_m(55), 0,

    sec_in_m(1), sec_in_m(2), sec_in_m(3), sec_in_m(4), 0
};

void switch_lights(char *lights, unsigned int *compares)
{
    for (size_t pos = 0; pos < (sizeof(ge_than) / sizeof(unsigned int)); ++pos)
    {
        if (compares[pos] < ge_than[pos]) lights[pos] = 'O';
    }
}

void print_berlin_clock(unsigned int seconds)
{
    char lights[] = {
        'Y', '\n',
        'R', 'R', 'R', 'R', '\n',
        'R', 'R', 'R', 'R', '\n',
        'Y', 'Y', 'R', 'Y', 'Y', 'R', 'Y', 'Y', 'R', 'Y', 'Y', '\n',
        'Y', 'Y', 'Y', 'Y', '\0' };

    unsigned int compares[sizeof(lights)] = {0};

    fill_compares(seconds, compares);
    switch_lights(lights, compares);
    printf("%s\n", lights);
}

void print_usage(const char* cmdname)
{
    printf( "Usage: %s <time>\n\n"
            "  <time> has to be in the format 'hh:mm:ss'\n", cmdname);
}

#define EXIT_IF_NOT_DIGIT(X) if ((X) < '0' || (X) > '9') return sec_per_day+1
#define EXIT_IF_NOT(X) if (!(X)) return sec_per_day+1

unsigned int extract_seconds(const char* arg)
{
    unsigned int seconds = 0;
    unsigned int tmp = 0;

    for (size_t pos = 0; pos < 9; ++pos)
    {
        const char c = arg[pos];
        switch (pos)
        {
            case 0:
            case 3:
            case 6:
                EXIT_IF_NOT_DIGIT(c);
                tmp = c - '0';
                break;

            case 1:
            case 4:
            case 7:
                EXIT_IF_NOT_DIGIT(c);
                tmp = (tmp * 10) + (c - '0');
                seconds = (seconds * 60) + tmp;
                break;

            case 2:
            case 5:
                EXIT_IF_NOT(c == ':');
                break;

            case 8:
                EXIT_IF_NOT(c == '\0');
                break;
        }
    }

    return seconds;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    unsigned int seconds = extract_seconds(argv[1]);

    if (seconds > sec_per_day)
    {
        fprintf(stderr, "\n*** Malformed string given\n\n");
        print_usage(argv[0]);
        return 2;
    }

    print_berlin_clock(seconds);
}
