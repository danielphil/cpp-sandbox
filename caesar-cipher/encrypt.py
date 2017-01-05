import random
import string
import sys

random.seed()

def encrypt(filename):
    characters = list(string.ascii_lowercase)
    cipher = {}

    for c in string.ascii_lowercase:
        index = random.randrange(0, len(characters))
        substitution = characters.pop(index)
        cipher[c] = substitution

    with open(filename) as textfile:
        for line in textfile:
            line = line[:-1]
            newline = ''
            for c in line:
                if c.lower() in cipher:
                    if c.isupper():
                        newline += cipher[c.lower()].upper()
                    else:
                        newline += cipher[c]
                else:
                    newline += c
            print(newline)

encrypt(sys.argv[1]);