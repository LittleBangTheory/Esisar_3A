all: client serveur

client: client.c calcul.h
	gcc -Wall -Wextra -g $< -o $@ -fsanitize=address

serveur: serveur.c calcul.h
	gcc -Wall -Wextra -g $< -o $@ -fsanitize=address

clean:
	rm client
	rm serveur

tar:
	tar -czvf chognot_dacosta_tp4.tar.gz *.c *.h Makefile README.md