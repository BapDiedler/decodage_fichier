CC=gcc

all: decalageMessage decrypteMessage parcoursFichier

decalageMessage: src/decalageMessage.c
	$(CC) src/decalageMessage.c -o decalageMessage

decrypteMessage: src/decrypteMessage.c
	$(CC) src/decrypteMessage.c -o decrypteMessage

parcoursFichier: src/parcoursFichier.c
	$(CC) src/parcoursFichier.c -o parcoursFichier

.PHONY: clean

clean:
	rm -f decalageMessage decrypteMessage parcoursFichier

run: decalageMessage decrypteMessage parcoursFichier
	./parcoursFichier src/ressource/TestEtMessagesCodes/MessagesCodes/ $(ARG)