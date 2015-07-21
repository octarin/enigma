#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "main.h"
#include "enigma.h"

int main(int argc, char* argv[])
{
    short rotornumbers = 0123;
    int rpositions[3] = {0};
    char* input_filename;
    FILE* fd;

    /* Checks the cli arguments */
    if (parseoptions(argc, argv, &input_filename, &rotornumbers, rpositions)
            == EXIT_FAILURE) {
        printusage(argv[0]);
        return EXIT_FAILURE;
    }

    if ((fd = fopen(input_filename, "r")) == NULL) {
        perror("fopen:");
        return EXIT_FAILURE;
    }
    cryptstream(fd, rotornumbers, rpositions);

    return EXIT_SUCCESS;
}

static void printusage(char* name)
{
    printf("Usage : %s [-r abc] [-p p1,p2,p3] <input file>\n", name);
}

static int parseoptions(int argc, char** argv, char** input_filename, 
                        short* rotornumbers, int rpositions[])
{
    char option;
    char* c;
    int i;

    while ((option = getopt(argc, argv, "r:p:")) != -1) {
        switch (option) {
            case 'r':
                *rotornumbers = strtol(optarg, NULL, 8);
                if (*rotornumbers < 0123 || *rotornumbers > 0543) {
                    printf("You must specify 3 different rotors between 1" \
			   "and 5.\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'p':
                c = optarg;
                for (i = 0; i < 3; ++i) {
                    while (*c != ',' && *c != '\0') ++c;
                    --c;
                    rpositions[i] = strtol(optarg, &c, 10);
                    optarg = c + 1;
                }

                break;
            default:
                return EXIT_FAILURE;
                break;
        }
    }

    if (optind >= argc) {
        printf("Expected filename\n");
        return EXIT_FAILURE;
    }
    *input_filename = argv[optind];

    return EXIT_SUCCESS;
}
