# CLK

A new version of cloak, re-designed and re-engineered from the bottom up.

Hide and extract an encrypted file within an RGB (24-bit) PNG image.

The idea is simple, a 24-bit colour PNG image uses 3 bytes for each pixel in the image, one each for the Red, Green and Blue channels, so each colour channel is represented by a value between 0 - 255. If we encode a file in the least significant bits (LSBs) of the image data, there will be no visible difference in the image when displayed. At an encoding depth of 1-bit per byte, we need 8 bytes of image data to encode 1 byte of our file.

Cloak can encrypt your 'secret' data file using either the AES-256 (Rijndael) cipher (in CBC mode) or XOR encryption prior to encoding it in your chosen image. With AES encryption, you will be prompted to enter a password, the SHA-512 hash of which is used as the key for the encryption with AES-256. With XOR encryption, you must either supply a keystream file using the -k option, or specify the file to create with the -g option. The OTP generate function uses the /dev/random device on *nix systems.

The advantage with XOR encryption is you can employ a one-time-pad scheme, which providing you stick to the rules for a one-time-pad encryption scheme, is mathematically proven to be unbreakable.

The rules are simple, but break one or more of them, it becomes relatively easy to crack:

1. The key is truly random
2. The key is used once and only once
3. The key is at least as long as the file being encrypted

Of course, any encryption scheme is useless if some third party has got hold of your encryption key.

## Some tips regarding password strength

A good password is one that cannot be broken using a dictionary attack, e.g. don't use a word from the dictionary or a derivation of. Use a made-up word or phrase with symbols and numbers, better still a random string of characters. In the context of this software, an important aspect is getting the password or keystream to your intended audience securely. It is also imperative that you do not re-use a key, it may be prudent to agree a unique and random set of keys with your audience in advance.

### References:

https://en.wikipedia.org/wiki/Dictionary_attack

https://en.wikipedia.org/wiki/Password_strength

https://www.random.org/

## Building clk

Cloak is written in C++ and I have provided a makefile for Unix/Linux using the g++ compiler (tested on Mac OS). Cloak depends on the 3rd party libraries libpng (http://libpng.org) and libgcrypt (https://www.gnupg.org/software/libgcrypt/index.html) (for the encryption and hashing algorithms, part of GPG).

## Using clk

Type clk --help to get help on the command line parameters:

Using clk:
    clk --help (show this help)
    clk [options] file
    options: -h, -host [the host image]
             -k, -key [keystream file for one-time pad encryption]
             -sl, -security [high|medium|low|none]
             -algo [aes|xor|none] - the default is no encryption
             -g, -generate [keystream file to generate, use and save]

flowers_out.png

I have included a sample PNG file with this distribution - flowers_out.png which has the LICENSE encoded within it, the password used to encrypt the file is 'password', however you should use a strong password in real-world applications, see the tips on password strength above.

For example, to 'cloak' a file within flowers.png I used the following command:

```clk merge -security high -algo aes -h flowers.png LICENSE

This tells clk to use merge the file 'LICENSE' into the image 'flowers.png' using an encoding depth of 1-bit per byte.

To 'uncloak' the file from flowers.png, you can use the following command:

```clk extract -security high -algo aes -h flowers.png LICENSE.out

This tells clk to use extract mode to extract the file 'LICENSE.out' from the input image 'flowers.png', again using 1-bit per byte.

Have fun!
