# :unlock: K128 Encryption Algorithm
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com) [![MIT Licence](https://badges.frapsoft.com/os/mit/mit.png?v=103)](https://opensource.org/licenses/mit-license.php) 
Implementation of K128 Encryption algorithm.

Project for subject MAC0336 (Cryptography for data security) at IME-USP.

## Usage
```
$ make

$ ./k128 [execution mode] [-i input file] [-o output file] [-p password] [-a erase input] 
```
### Execution modes
- __-c__: Encrypts the input file
- __-d__: Decrypts the input file
- __-1__: Calculates the entropy of the algorithm (mode 1)
- __-2__: Calculates the entropy of the algorithm (mode 2)

### Password
The password needs to have:
- At least 8 characters;
- At least 2 letters;
- At least 2 numbers.

### Erase input mode
Using the flag `-a`, the input file is overwritten and deleted.

## Examples
Encrypt the file test.in and erase it:
```bash
$ ./k128 -c -i test.in -o test.out -p "SeCur3P4SS" -a 
```

Decrypt the file test.out:
```bash
$ ./k128 -d -i test.out -o test.out.dec -p "SeCur3P4SS" 
```

Test the algorithm entropy (mode 1):
```bash
$ ./k128 -1 -i test.in -p "SeCur3P4SS" 
```

Test the algorithm entropy (mode 2):
```bash
$ ./k128 -2 -i test.in -p "SeCur3P4SS" 
```
## Licence
MIT
