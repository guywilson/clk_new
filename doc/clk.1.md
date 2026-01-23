% CLK(1) CLK User Manual
% Guy Wilson
% January 23, 2026

# NAME

clk - cloak

# SYNOPSIS

clk [merge|extract] [*options*] file

# DESCRIPTION

Hide and extract an encrypted file within an RGB (24-bit) PNG image.

The idea is simple, a 24-bit colour PNG image uses 3 bytes for each pixel in the image, one each for the Red, Green and Blue channels, so each colour channel is represented by a value between 0 - 255. If we encode a file in the least significant bits (LSBs) of the image data, there will be no visible difference in the image when displayed. At an encoding depth of 1-bit per byte, we need 8 bytes of image data to encode 1 byte of our file.

# OPTIONS

**-h, -host [host file]**

Specifies the filename of the host. Currently, clk only supports 24-bit PNG images. That is PNG images that store their image data with 8-bits per colour channel (RGB).

**-algo [encryption algorithm]**

The algorithm used to encrypt your file prior to merging with the host file and decrypt after extracting. It can be one of the following:

- aes256 - encrypt using the AES-256 algorithm in CBC mode
- aes    - an alias for aes256
- xor    - encrypt using XOR encryption
- otp    - an alias for xor
- none   - no encryption (just hide)
- no     - an alias for none

If -algo aes256 is specified, the user will be prompted to enter a password.

If -algo xor is specified, the user must specify the key file to encrypt with using the -k or -g option.

**-sl, -security-level**

Specify the security level to use when hiding/extracting your file, it can be one of the following:

- high    - The most secure, uses 8 bytes of host data to store one byte of your file.
- medium  - Uses 4 bytes of host data to store one byte of your file.
- low     - Not recommended, uses 2 bytes of host data to store 1 byte of your file.

**-k, -key [key file]**

Supply a key file for XOR encryption, clk will abort with an error if key file is smaller than your file you want to hide/extract. If your intention is to use the key file as a One Time Pad (OTP), in order to be secure, you must follow these 3 rules:

1. The key must be truly random
2. The key must be used **only** once
3. The key must be at least as long as the file to encrypt (clk checks this)

**-g, -generate [key file]**

This will generate random data the same length as your file to encrypt with XOR encryption and save it to your key file, pfm will use the data to encrypt your file before exiting, so can be used in the same way as the -k option. See the -k option for the rules around OTP encryption.
 
**--version, -v**

Prints the version information of the clk program and exits.

**--help, -?**

Prints the supported command line options and exits.

# EXAMPLES

To 'cloak' file clk.dat using AES-256 encryption within test.png using the highest security level:

```clk merge -security-level high -algo aes -host test.png clk.dat```

To 'uncloak' file out.dat from test.png using AES-256 encryption:

```clk extract -security-level high -algo aes -host test.png out.dat```

# AUTHOR

Written by Guy Wilson.

# COPYRIGHT

Copyright (c) 2026 Guy Wilson.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.