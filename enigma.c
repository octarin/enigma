/* 
 * This file contains all of the funtions used to process text like the enigma
 * machine.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "enigma.h"


/* Global Variables */

/* The rotors and reflectors : 
 * (http://users.telenet.be/d.rijmenants/en/enigmatech.htm)
Entry = ABCDEFGHIJKLMNOPQRSTUVWXYZ (rotor right side)   
        ||||||||||||||||||||||||||
I     = EKMFLGDQVZNTOWYHXUSPAIBRCJ
II    = AJDKSIRUXBLHWTMCQGZNPYFVOE
III   = BDFHJLCPRTXVZNYEIWGAKMUSQO
IV    = ESOVPZJAYQUIRHXLNFTGKDCMWB
V     = VZBRGITYUPSDNHLXAWMJQOFECK
*/

char *dispositions[] = 
{ 
     "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
     "AJDKSIRUXBLHWTMCQGZNPYFVOE",
     "BDFHJLCPRTXVZNYEIWGAKMUSQO",
     "ESOVPZJAYQUIRHXLNFTGKDCMWB",
     "VZBRGITYUPSDNHLXAWMJQOFECK"
};

char *reflectors[] =
{
    "YRUHQSLDPXNGOKMIEBFZCWVJAT",
    "FVPJIAOYEDRZXWGCTKUQSBNMHL"
};

char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* Les différents rotors */
Rotor rotors[3];


/* Fonctions */


/* initRotors: initialise les 3 rotors en fonction du choix de l'utilisateur */
void initRotors(short rnumbers, int rpositions[])
{
    rotors[0] = initRotor(dispositions[(rnumbers & 0x7) - 1], rpositions[0]);
    rotors[1] = initRotor(dispositions[((rnumbers & 0x38) >> 3) - 1], 
            rpositions[1]);
    rotors[2] = initRotor(dispositions[((rnumbers & 0x1c0) >> 6) - 1], 
            rpositions[2]);
}

/* initRotor: initialise la structure d'un rotor 
 * et sa position. */
Rotor initRotor(char* translation, int position)
{
    Rotor buf;
    buf.translation = translation;
    buf.position = position;
    return buf;
}

/* rotate: incrémente la position d'un rotor modulo 26 */
int rotate(Rotor *rotor)
{
    rotor->position += 1;
    if (rotor->position == 26) {
        rotor->position = 0;
        return 1;
    } else {
        return 0;
    }
}

/* substituer: renvoie le résultat du passage d'une
 * lettre à travers un rotor */
char substituer(char lettre, Rotor rotor)
{
    int lpos = lettre - 'A';
    lettre = rotor.translation[(lpos + rotor.position) % 26];
    return lettre;
}

/* inv_substituer: équivalent de "substituer" mais en faisant passer
 * la lettre dans le sens inverse */
char inv_substituer(char lettre, Rotor rotor)
{
    int lpos = strchr(rotor.translation, lettre) - rotor.translation;
    lettre = ((lpos - rotor.position) % 26);
    lettre = (lettre < 0) ? lettre + 'A' + 26 : lettre + 'A';
    return lettre;
}

/* reflector: renvoie la lettre passée par le réflecteur */
char reflector(char lettre)
{
    int lpos = lettre - 'A';
    lettre = reflectors[0][lpos % 26];
    return lettre;
}

/* cryptletter: renvoie la lettre chiffrée en la faisant passer
 * par les 3 rotors, le reflecteur puis les 3 rotors en sens inverse */
char cryptletter(char lettre)
{
    int i;

    for (i = 0; i < 3; ++i)
        lettre = substituer(lettre, rotors[i]);

    lettre = reflector(lettre);

    for (i = 2; i >= 0; --i)
        lettre = inv_substituer(lettre, rotors[i]);

    return lettre;
}

/* cryptstream: affiche la version chiffrée d'un descripteur de fichier
 * passé en argument */
void cryptstream(FILE* stream, short rnumbers, int rpositions[])
{
    int lettre, rotate_next;

    initRotors(rnumbers, rpositions);

    while ((lettre = fgetc(stream)) != EOF) {

        if (strchr(alphabet, lettre)) {
            putchar(cryptletter(lettre));

            rotate_next = rotate(&rotors[0]);
            if (rotate_next) {
                rotate_next = rotate(&rotors[1]);
                if (rotate_next)
                    rotate(&rotors[2]);
            } 
        } else {
            putchar(lettre);
        }
    }
}

/* cryptbuf: affiche la version chiffrée d'une chaîne de caractère de
 * longueur donnée passée en argument */
char* cryptbuf(char* buffer, size_t size, short rnumbers, int rpositions[])
{
    int lettre, rotate_next;
    int i;

    initRotors(rnumbers, rpositions);
    for (i = 0; i < size; ++i) {
        lettre = buffer[i];
        if (strchr(alphabet, lettre)) {
            buffer[i] = cryptletter(lettre);

            rotate_next = rotate(&rotors[0]);
            if (rotate_next) {
                rotate_next = rotate(&rotors[1]);
                if (rotate_next)
                    rotate(&rotors[2]);
            }
        } else {
            buffer[i] = lettre;
        }
    }
    return buffer;
}

