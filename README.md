## Not A Ransomware

This software encrypts all files recursively from a given starting folder.

Make sure you don't do dumb stuff with this software.

## Usage
./notaransomware [options] [starting folder]

### Options

-m: Specifies the mode. Use `encrypt` or `decrypt` -> defaults to `encrypt`

-k: Specifies the 256bit key (use 32 chars) -> defaults to `12345678901234561234567890123456`

-i: Specifies the 128bit initialisation vector (use 16 chars) -> defaults to `1234567890123456`

### Example 
`./notaransomware -m encrypt -k 12345678901234561234567890123456 /home`

## Function

The software explores the filesystem using the `dirent.h` c library.
For the encryption the aes implementation of openssl is used -> `openssl/aes.h`.

## Compiling

As mentioned the openssl implementation of aes is used. So to build the dev package of openssl has to be installed.

`sudo dnf install openssl-devel`

`sudo apt install libssl-dev`

Or whatever name this package has on your system.

After that just use following command to build:

`gcc main.c crypt.h crypt.c -lssl -lcrypto -o notaransomware`

### Precompiled versions

The compiled version runs on Fedora 35