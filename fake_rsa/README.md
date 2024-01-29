
# README

    This will be an implementation for the public and private key sharing, encrypted and decrypted using rsa
    loosely described in Tanebaum's "Computer Networks" at page 795.

## What the book says:
1) *CHOOSE* two large prime numbers, **p** and **q** (usually 1024 bits)
2) *FIND* **n = p x q** and **z = (p-1) x (q-1)**
3) *CHOOSE* a number "relatively prime" to *z*, that we will call **d**
4) *FIND* the value of *e* such that **e x d = 1%z**

## How I want to implement it?
### Current: v0.01
            No actual functionalities are working now.
            - Maybe I'm missing something about the block size
### Up to v1.0
            In the first versions of this project, I'll just use a text file containing a bunch of 1024-bit long prime numbers
            use it to encrypt a message, save it on a file, and decrypt it.

### Up to v2.0
            An updated version of this project will also use a server-client communication to see the real deal working
            so sending long messages will be a no go.

### Up to v3.0
            Later -and maybe last version- will also use a fake-AES.c implementation, so I'll be able to make a simple chat app
            out of this, where RSA will be used just for *key sharing* and AES will be used just for *message passing*.
