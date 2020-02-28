SD Tema 1 - Nicolau Mihnea-Andrei 311CD

createDictionary - am alocat memorie pentru un element de tip dictionar si, in
				   cazul in care aceasta operatie a reusit, am alocat memorie
				   pentru un vector de liste circulare, initializate cu NULL

urmatoarele 5 functii auxiliare si nu fac parte din scheletul temei:

getLenght - am creat aceasta functie deoarece am nevoie de numarul de elemente
			dintr-o lista in mai multe functii
		  - functia returneaza un contor, care creste cu 1 pentru fiecare 
			element parcurs, pana cand se ajunge la elementul de la care s-a 
			plecat (in cazul listelor circulare) sau pana se ajunge la NULL
			(pentru liste necirculare)

nrElements - calculeaza suma numarului de elemente a tuturor listelor din
			 dictionar 
		   - am folosit-o doar in functia de adaugare, dar la o extindere a 
		   	 programului ar putea avea mai multe folosinte

getNode - functia aloca memorie pentru un element de tip ListNode*, ii 
		  asociaza valorile corespunzatoare si il returneaza
		- secventa de alocare a unui nod apare in multe dintre functile necesare
		  temei

insertNode - functia cauta locul in lista unde ar trebui inserat nod-ul primit
			 ca parametru si il insereaza in acel loc (gaseste nodul urmator
			 pozitiei in care trebuie adaugat noul nod)
		   - este o completare a functiei de adaugare; am folosit-o pentru a nu
		   	 incarca prea mult functia

getIndex - functia calculeaza indexul listei corespunzatoare key-i primite (r)
		 - secventa inclusa in aceasta functie s-ar fi repetat de mai multe ori
		   in cod

am schimbat ordinea functiilor addElement si removeElement deoarece am apelat-o
pe ultima in cadrul primei

removeElement - parcurge lista cu indexul corespunzator key-i primite si 
				verifica daca vreunul dintre elemente are value-ul cautat
			  - daca elementul de sters este gasit si este singurul din lista,
			  	atunci lista din vector va primi valoarea NULL; daca este
			  	primul element al listei, atunci se schimba legatura cu vectorul
			  	la urmatorul element; i se taie nodului legatura cu restul 
			  	listei si memoria alocata lui se elibereaza

addElement - adauga un tuplu in lista fara a strica ordinea listei
		   - prima data verifica daca lista este NULL, caz in care adauga un
		     prim nod (conditia ca nrElemente != 2*N pare suplimentara aici
		     insa mie mi se para necesara dupa felul in care am inteles eu
		     ca ar trebui sa mearga programul)
		   - apoi cauta elementul printre cele deja existente in lista, caz in
		   	 care ii creste frecventa cu noua frecventa, ii taie legaturile cu
		   	 restul listei si apeleaza functia care cauta locul potrivit si il
		   	 adauga inapoi in lista
		   - urmeaza cele doua conditii pentru mentinerea dimensiunii reduse a
		     dictionarului: daca lista contine deja N elemente, se apeleaza 
		     functia de stergere pentru ultimul element, iar daca dictionarul
		     contine deja 2*N elemente, se apeleaza functia de stergere pentru
		     ultimele elemente din toate listele; length scade cu 1 in fiecare 
		     dintre cele doua cazuri deoarece se scoate un element din lista
		     curenta
		   - la final, se aloca un nou nod cu valorile primite si se apeleaza 
		   	 functia de inserare

printDictionary - parcurge dictionarul si scrie fiecare tuplu in fisierul cerut

get - cauta elementele cu key-a data in lista corespunzatoare key-i; daca se
	  gaseste un element, se adauga in lista nou formata
	- la final se parcurge lista pentru a creste frecventa fiecarui element
	  gasit cu 1 (prin apelarea functiei de adaugare cu key-a si value al
	  elementului si frecventa 1)

printValue + printFrequency - se parcurge dictionarul si se scriu in fisier
                              tuplurile cu value/frequency-ul primit  

unionValues - creeaza o lista necirculara (legaturile intre primul si ultimul
              nod sunt lasate NULL, asa cum sunt dupa apelarea getNode) cu
              tuplurile ce contin value-ul primit
            - se parcurge dictionarul si se gasesc elementele cu valoarea ceruta
            - aceste elemente se adauga in lista (daca lista este NULL primul
              element devine cel spre care pointeaza lista; altfel se adauga
              legaturi catre ultimul nod adaugat la lista, retinut in variabila
              prevNode) si se returneaza lista

unionMaxFrequencies - lista se creeaza in acelasi fel ca la unionValues, dar 
                      elementele listei se selecteaza diferit
                    - se gaseste frecventa maxima a fiecarei liste (care este
                      chiar frecventa primului element, datorita modului de
                      organizare a listelor dictionarului), daca lista exista,
                      si apoi se adauga toate elementele care au frecventa egala
                      cu acest maxim (cand se intalneste un element cu frecventa
                      mai mica, se iese din for si nu se mai adauga alte 
                      elemente din acea lista, trecandu-se la urmatoarea)

reverseLists - creeaza un dictionar cu listele ordonate invers fata de 
               dictionarul primit ca argument
             - se parcurge dictionarul de oglindit (dictionarul nou va avea
               acelasi numar de liste, fiecare cu aceeasi lungime); se retine
               ultimul element al listei si apoi se parcurge lista invers
               (de la ultimul element la primul), fiecare element fiind adaugat
               in lista corespunzatoare din newDictionary (primul element devine
               in plus si cel spre care pointeaza lista (j==0))
             - spre deosebire de listele necirculare create in cadrul functiilor
               anterioare, listele dictionarului sunt circulare, deci trebuie
               adaugat la sfarsitul parcurgerii (j==length sau aux==nodul spre 
               care pointeaza lista) legaturii intre primul si ultimul nod al
               listei

printList - se scriu elementele unei liste in fisierul primit ca argument

freeList - se elibereaza o lista; ea se parcurge pas cu pas (length pasi); se
		   retine nodul urmator nodului curent in variabila "next", se 
		   elibereaza memoria alocata pentru nodul curent si se trece la 
		   urmatorul nod (nodul curent devine nodul retinut in next), pana cand
		   lista devine NULL

freeDictionary - parcurge un dictionar lista cu lista, apeland functia freeList
				 pentru fiecare element al vectorului de liste; apoi se 
				 elibereaza si memoria alocata vectorului de liste si 
				 dictionarului propriu-zis