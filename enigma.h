#ifndef ENIGMA_H
#define ENIGMA_H

typedef struct {
    char *translation; /* 26 bytes array */
    int position;      /* From 0 to 25 */
} Rotor;

Rotor initRotor(char* translation, int position);
void initRotors(short rnumbers, int rpositions[]);

char substituer(char lettre, Rotor rotor);
char inv_substituer(char lettre, Rotor rotor);
char cryptletter(char lettre);
int rotate(Rotor *rotor);
void cryptstream(FILE* stream, short rnumbers, int rpositions[]);
char* cryptbuf(char* buffer, size_t size, short rnumbers, int rpositions[]);

#endif
