Pentru parsare am folosit biblioteca "parson", tema fiind implementata in C.

In fisierul client.c:
- comenzile de la tastatura sunt citite pe rand intr-un while, pana la primirea comenzii "exit"
- se initializeaza variabilele "connected" si "access_to_library" cu false. Aceste variabile vor fi folosite
pentru a verifica daca utilizatorul este logat si daca are acces la biblioteca
- la primirea comenzii "register" se ofera prompt  pentru introducerea credentialelor si se retin username-ul 
si parola introduse de utilizator, avand in vedere ca username-ul sa nu fi fost folosit deja
- la primirea comenzii "login" se marcheaza conectarea si obtinem credentialele 
- la primirea comenzii "logout" se marcheaza deconectarea si faptul ca utilizatorul nu mai are acces la biblioteca
- la primirea comenzii "enter_library" se verifica daca utilizatorul este logat si se afiseaza un mesaj corespunzator
- la primirea comenzii "get_books" se afiseaza cartile din biblioteca, verificand inainte accesul (prin variabila 
"acces_to_library")
- la primirea comenzii "get_books" se cauta cartea cu id-ul introdus de la tastatura, verificand inainte accesul 
la bilbioteca
- la primirea comenzii "add_book" se citesc de la tastatura datele necesare formarii unui obiect json
- la primirea comenzii "delete_book" se sterge cartea cu id-ul dat de utilizator (in cazul in care acesta corespunde 
unei carti si utilizatorul are acces la biblioteca)
- la primirea comenzii "logout" se seteaza variabilele "connected" si "access_to_library" pe false si se realizeaza 
deconectarea (verificand ca inainte sa fi fost facut un login).
