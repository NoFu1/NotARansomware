//
// Created by NoFu1 on 4/20/22.
//

#ifndef NOTARANSOMWARE_CRYPT_H
#define NOTARANSOMWARE_CRYPT_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

int lsRcrypt( char *path, char mode, char *key, char *iv);
void encryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv);
void decryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv);

#endif //NOTARANSOMWARE_CRYPT_H
