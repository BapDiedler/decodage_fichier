CC=gcc
ARG=DIEDLER

all: run

decalageMessage: src/decalageMessage.c
	$(CC) src/decalageMessage.c -o decalageMessage -g

decrypteMessage: src/decrypteMessage.c
	$(CC) src/decrypteMessage.c -o decrypteMessage -g

parcoursFichier: src/parcoursFichier.c
	$(CC) src/parcoursFichier.c -o parcoursFichier -g

clean:
	rm -f decalageMessage decrypteMessage parcoursFichier

run: decalageMessage decrypteMessage parcoursFichier
	./parcoursFichier src/ressource/TestEtMessagesCodes/MessagesCodes/ $(ARG)

.PHONY: clean run