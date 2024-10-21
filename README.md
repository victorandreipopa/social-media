Popa Victor-Andrei : 

### TASK 1 : FRIENDS

Am creat un set de functii pentru gestionarea prieteniilor intr-o
retea de utilizatori reprezentata sub forma unui graf.
Am utilizat functii precum adaugarea, eliminarea si verificarea
prieteniilor, calcularea distantei dintre doi utilizatori,
identificarea prietenilor comuni si sugerarea de noi prietenii.

1. **add_friend**: Adauga o prietenie intre doi utilizatori in ambele sensuri.

2. **check_friend**: Verifica daca doi utilizatori sunt prieteni.

3. **bfs_list_graph**: Calculeaza distanta intre doi utilizatori
folosind o cautare in latime (BFS).

4. **friends**: Afiseaza numarul de prieteni ai unui utilizator.

5. **popular**: Determina prietenul unui utilizator cu cei mai multi prieteni.

6. **common**: Gaseste si afiseaza prietenii comuni ai doi utilizatori.

7. **suggestions**: Sugereaza noi prieteni pentru un utilizator, pe
baza prietenilor prietenilor sai care nu sunt prieteni cu el.

8. **remove_friend_from_list**: Elimina un prieten din lista de
prieteni a unui utilizator.

9. **remove_friend**: Elimina o prietenie a doi utilizatori in ambele sensuri.

10. **handle_input_friends**: Prelucreaza comenzi de la input
pentru gestionarea prieteniilor, apeland functiile corespunzatoare
pe baza comenzilor introduse.

### TASK 2 : POSTS

* In cadrul acestui task am implementat o parte dintr-o retea de socializare
si anume gestionarea postarilor unui grup de oameni.
#### Structura implementata
* Pentru a gestiona mai bine postarile am retinut fiecare postare intr-o 
lista de postari, iar fiecarei postari i-am atribuit un arbore de reposturi. 
Fiecare nod din arbore are o lista de noduri reprezantand fiecare copil al 
unei repost/postari.Fiecare postare are si o lista de likeuri reprezantand 
persoanele care au dat like la o postare pentru a putea aplica si comanda 
de dislike. Pentru a putea determina cea mai apropiata postare dintre doua 
postari a trebuit sa retin in interiorul structurii si un nod la parintele 
postari
#### Descrierea comenzilor
* In cadrul acestui task am implementat diferite functii care imi creeaza 
reposturi , posturi, mi le sterge , imi calculeaza cea mai populara postare si 
multe altele dupa cum urmeaza:
    - create: imi creeaza o postare , adaugand-o in lista de postari
    - repost: imi creeaza o repostare si mi o adauga in lista de copii 
    a nodului corespunzator
    - common-repost: imi calculeaza cel mai apropiat repost de doua reposturi, 
    folosesc un algoritm care imi determina inaltimea fiecarui nod, folosind 
    parintele fiecarui nod. Dupa aceea aduc ambele postari pe cel mai mic nivel 
    si merg cu ambele pana cand gasesc cea mai comuna postare
    - like: adaug like-uri la o postare, dar si in lista de like-uri id-ul 
    persoanei care a dat like pentru a fi sigur ca nu da like de 2 ori,
    implementand astfel si functia de dislike
    - ratio: fucntie care verifica daca o repostare are mai multe like-uri 
    decat postarea originala
    - delete: functie care sterge o postare/repostare si intreaga ierarhie 
    de reposturi ale acesteia
    - get-likes: functie care afiseaza numarul de like-uri alea unuei postari
    - get-reposts: fucntie care imi afiseaza toare reposturile unei postari 

### TASK 3 : FEED

Acest task se ocupa cu gestionarea activitatilor pe un feed intr-o
retea sociala, reprezentata sub forma unui graf. Functiile folosite
sunt pentru afisarea postarilor si repostarilor utilizatorilor,
vizualizarea profilurilor si sugestii de repostari. 

1. **feed**: Afiseaza postarile din feed-ul unui utilizator
si ai prietenilor sai.

2. **print_all_repost**: Afiseaza toate repostarile unui utilizator.

3. **print_all_post**: Afiseaza toate postarile unui utilizator.

4. **view_profile**: Afiseaza toate postarile si repostarile
unui utilizator pe baza numelui.

5. **print_all_friends**: Functie ajutatoare care afiseaza toti
prietenii care au repostat o postare data.

6. **friends_repost**: Afiseaza prietenii unui utilizator care
au repostat o anumita postare.

7. **common_groups**: Functia pentru afisarea celui mai mare grup
de prieten . Pentru aceasta functie am folosit o functie ajutatorare
generare_grup_prieteni care imi genereaza toate grupurile de prieteni
posibile si mi-l pastreaza pe cel cu cea mai mare dimensiune

8. **handle_input_feed**: Prelucreaza comenzi de la input pentru
gestionarea feed-ului, apelând functiile corespunzatoare pe
baza comenzilor introduse.

