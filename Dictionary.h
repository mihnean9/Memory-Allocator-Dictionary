#ifndef TABLE_H 
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// List element for Dictionary lists.
typedef struct ListNode {
	struct ListNode *next;
	struct ListNode *prev;
	char *key;
	char *value;
	int frequency;
} ListNode;

// Dictionary structure that includes the lists of elements and their number.
typedef struct Dictionary {
	ListNode **lists;		// lists of elements.
	int N;					// number of lists.
} Dictionary;

// Initializes an empty Dictionary structure.
Dictionary* createDictionary(int N);

// Adds an element to the Dictionary structure.
void addElement(Dictionary *dictionary, char *key, char *value, int frequency);

// Removes an element from the Dictionary structure.
void removeElement(Dictionary *dictionary, char *key, char *value);

// Prints all the elements from all the lists of the Dictionary structure.
void printDictionary(FILE *f, Dictionary *dictionary);

// Gets all the elements with the specified key and increments the frequencies.
ListNode* get(Dictionary *dictionary, char *key);

// Prints all the elements with the specified value.
void printValue(FILE *f, Dictionary *dictionary , char *value);

// Prints all the elements with the specified frequency.
void printFrequency(FILE *f, Dictionary *dictionary , int frequency);

// Returns a list containing the elements with the specified value.
ListNode* unionValues(Dictionary *dictionary, char *value);

// Returns a list containing the elements with maximum frequency in each list.
ListNode* unionMaxFrequencies(Dictionary *dictionary);

// Returns a new Dictionary with reversed lists of the input structure.
Dictionary* reverseLists(Dictionary *dictionary);

// Prints a double-linked non-circular list.
void printList(FILE *f, ListNode *list);

// Frees all the memory allocated for the Dictionary.
void freeDictionary(Dictionary *dictionary);

// Frees all the memory allocated for a double-linked non-circular list.
void freeList(ListNode *list);

//------------------------------------------------------------------------------

Dictionary* createDictionary(int N) {
   
    //aloc memorie pentru un element de tip dictionar
	Dictionary *newDictionary = (Dictionary*) malloc(sizeof(Dictionary));
	if(newDictionary == NULL)
        return NULL;
    newDictionary->N = N;

    //aloc memorie pentru vectorul de liste din cadrul dictionarului
    ListNode **newList = (ListNode**) malloc(N*sizeof(ListNode*));
    if(newList == NULL) {
        freeDictionary(newDictionary);
        return NULL;
    }
    newDictionary->lists = newList;

    //initializez fiecare lista cu NULL
    int i;
    for(i = 0; i < N; i++) {
        newDictionary->lists[i] = NULL;
    }
    return newDictionary;
}

//functie auxiliara care calculeaza nr de elemente ale unei liste
int getLength(ListNode *list) {

    if(list == NULL)    return 0;
    ListNode *aux = list->next;
    int count = 1;

    //contorul creste pana cand se ajunge inapoi la primul nod al liste 
    //sau pana cand se termina lista(in cazul in care este necirculara)
    while(aux != list && aux) {
        count++;
        aux = aux->next;
    }
    return count;
}

//functie auxiliara care calculeaza nr total de tupluri dintr-un dictionar
int nrElements(Dictionary *dictionary) {
    
    int i, sum = 0;
    for (i = 0; i < dictionary->N; i++)
        sum += getLength(dictionary->lists[i]);
    return sum;
}

//functie auxiliara care aloca memorie pentru un nod si ii asociaza valorile 
ListNode* getNode(char *key, char *value, int frequency) {

    ListNode *newNode = (ListNode*) malloc(sizeof(ListNode));
    if(newNode == NULL)
        return NULL;

    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->value = value;
    newNode->key = key;
    newNode->frequency = frequency;

    return newNode;
}

//functie auxiliara care insereaza un nod in lista, pastrand ordinea
void insertNode(Dictionary *dictionary, int index, ListNode *node, int length) {

    //daca lista este goala, se insereaza nodul ca primul nod al listei
    if(length == 0) {
        node->prev = node;
        node->next = node;
        dictionary->lists[index] = node;
        return;
    }

    int i = 0;
    ListNode *list = dictionary->lists[index];

    //se cauta nodul ultimul nod cu frecventa mai mare
    //apoi cel cu valoarea si cu cheia mai mica decat nodul de inserat
    //daca s-a depasit un ciclu de parcurgere, atunci se opreste
    while(list->frequency > node->frequency && i != length) {
        list = list->next;
        i++;
    }
    while(strcmp(list->value, node->value) < 0) {
    	//daca se parcurge toata lista sau daca se ajunge la alta frecventa
    	//se opreste din cautat
    	if(i == length || list->frequency != node->frequency)
    		break;
        list = list->next;
        i++;
    }
    while(strcmp(list->key, node->key)<0 && !strcmp(list->value, node->value)) {
        if(i == length || list->frequency != node->frequency)
            break;
        list = list->next;
        i++;
    }

    //se insereaza noul nod in lista
    ListNode *prevNode = list->prev;
    node->next = list;
    node->prev = prevNode;
    prevNode->next = node;
    list->prev = node;

    //daca i = 0, atunci nodul are frecventa mai mare decat primul nod
    //deci devine noul prim nod
    if(i == 0) {
        dictionary->lists[index] = node;
    }
    return;
}

//functie auxiliara care calculeaza indexul unei liste pe baza formulei date
int getIndex(char *key, int N) {

    int i, sum = 0;
	for(i = 0; i < strlen(key); i++)
        sum += key[i];
    return sum % N;
}

void removeElement(Dictionary *dictionary, char *key, char *value) {

    //daca dictionarul nu exista nu avem ce sterge
    if(dictionary == NULL)  return;

    int index, length, i;
    index = getIndex(key, dictionary->N);
	ListNode *aux = dictionary->lists[index];

    //daca lista e goala nu avem ce sterge
    if(aux == NULL) return;

    length = getLength(aux);
    
    //se cauta elementul de sters in lista
    for(i = 0; i < length; i++) {  
      
        if(strcmp(aux->key, key) == 0 && strcmp(aux->value, value) == 0) {

            //daca lista are un singur element atunci acesta se elibereaza
            if(length == 1) {
                dictionary->lists[index] = NULL;
                free(aux);
                return;
            }

            //altfel, se elibereaza elementul pastrand legaturile intre noduri
            aux->prev->next = aux->next;
            aux->next->prev = aux->prev;

            //daca s-a sters primul nod din lista avem grija sa nu pierdem 
            //conexiunea cu vectorul de liste
            if(dictionary->lists[index] == aux) {
                dictionary->lists[index] = aux->next;
            }
            free(aux);
            return;
        }
        aux = aux->next;
	}
    //daca elementul de sters nu este gasit
    //atunci functia nu schimba nimic in cadrul dictionarului
	return;
}


void addElement(Dictionary *dictionary, char *key, char *value, int frequency) {

    //daca dictionarul este null, atunci nu avem unde sa adaugam
    if(dictionary == NULL)  return;

	int i, index;
    index = getIndex(key, dictionary->N);
	ListNode *aux = dictionary->lists[index];
    int length = getLength(aux);

    //daca lista este goala
	if(aux == NULL) {

        //daca dictionarul contine 2*N elemente deja si s-ar adauga 
        //un nou element, ar fi depasita limita impusa
        if(nrElements(dictionary) == 2*dictionary->N) {
            for(i = 0; i < dictionary->N; i++) {
                if(dictionary->lists[i] != NULL) {
                	ListNode *lastNode = dictionary->lists[i]->prev;
                    removeElement(dictionary, lastNode->key, lastNode->value);
                }
            }
        }

        //se creaza primul nod din lista si se iese din functie
        ListNode *newNode = getNode(key, value, frequency);
        if(newNode != NULL) {
        	newNode->prev = newNode;
        	newNode->next = newNode;
        	dictionary->lists[index] = newNode;
        }
        return;
    }

    //se cauta valoarea si cheia date printre nodurile deja existente in lista
    for(i = 0; i < length; i++) {

        if(!strcmp(aux->value, value) && !strcmp(aux->key, key)) {
            //daca s-a gasit, legaturile nodului cu lista sunt taiate si 
            //se re-insereaza in lista in noua pozitie
            //corespunzatoare noii frecvente
            aux->frequency += frequency;

            if(aux != dictionary->lists[index]) {
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
                insertNode(dictionary, index, aux, length-1);
            }

            return;
        }
        aux = aux->next;
    }
    
    //cele doua conditii pentru pastrarea unei dimensiuni reduse a dictionarului
    if(length == dictionary->N) {
        removeElement(dictionary, aux->prev->key, aux->prev->value);
        length--;
        //in ambele cazuri, lungimea listei scade cu 1
    }
    if(nrElements(dictionary) == 2*dictionary->N) {
        for(i = 0; i < dictionary->N; i++) {
            if(dictionary->lists[i] != NULL) {
                ListNode *lastNode = dictionary->lists[i]->prev;
                removeElement(dictionary, lastNode->key, lastNode->value);
            }
        }
        length--;
    }
    
    //se creaza un nou nod si se insereaza la pozitia corespunzatoare
    ListNode *newNode = getNode(key, value, frequency);
    if(newNode != NULL)
    	insertNode(dictionary, index, newNode, length);
    return;
}


void printDictionary(FILE *f, Dictionary *dictionary) {

	int i, j;
    ListNode *aux;

    //se parcurge dictionarul si se afiseaza fiecare tuplu
	for(i = 0; i < dictionary->N; i++) {
        fprintf(f, "List %d: ", i);
        aux = dictionary->lists[i];

        for(j = 0; j < getLength(dictionary->lists[i]); j++) {
            fprintf(f, "(%s, %s, %d) ", aux->key, aux->value, aux->frequency);
            aux = aux->next;
        }

        fprintf(f, "\n");
	}
	return;
}

ListNode* get(Dictionary *dictionary, char *key) {

	int i, index, length;
    index = getIndex(key, dictionary->N);
    ListNode *newList = NULL, *aux, *prevNode, *newNode;
    aux = dictionary->lists[index];
    length = getLength(aux);

    for(i = 0; i < length; i++) {

        //se cauta elementul cu cheia data
        if(strcmp(aux->key, key) == 0) {

            //daca se gaseste, se adauga in noua lista
            newNode = getNode(aux->key, aux->value, aux->frequency);

            if(newNode != NULL) {
            	//daca newList este goala, atunci nodul devine primul nod
            	if(newList == NULL) {
                	newList = newNode;
            	}

            	//altfel se leaga de nodul anterior
            	else {
                	newNode->prev = prevNode;
                	prevNode->next = newNode;
            	}

            	prevNode = newNode;
        	}
        }
        aux = aux->next;
    }

    //se parcurge newList si se adauga 1 la frecventa fiecarui element gasit
    aux = newList;
    for(i = 0; i < getLength(newList); i++) {
        addElement(dictionary, aux->key, aux->value, 1);
        aux = aux->next;
    }

	return newList;
}

void printValue(FILE *f, Dictionary *dictionary , char *value) {

	int i, j;
    ListNode *aux;

    //se parcurge dictionarul, lista cu lista
    for(i = 0; i < dictionary->N; i++) {

        aux = dictionary->lists[i];

        for(j = 0; j < getLength(aux); j++) {

            //se cauta valoarea primita; daca se gaseste, se scrie in fisier
            if(strcmp(value, aux->value) == 0)
                fprintf(f,"(%s, %s, %d) ",aux->key, aux->value, aux->frequency);

            aux = aux->next;
        }
    }
    fprintf(f, "\n");
    return;
}

void printFrequency(FILE *f, Dictionary *dictionary , int frequency) {

	int i, j;
    ListNode *aux;

    //se parcurge dictionarul, lista cu lista
    for(i = 0; i < dictionary->N; i++) {

        aux = dictionary->lists[i];

        for(j = 0; j < getLength(aux); j++) {

            //se cauta frecventa primita; daca se gaseste, se scrie in fisier
            if(aux->frequency == frequency)
                fprintf(f,"(%s, %s, %d) ",aux->key, aux->value, aux->frequency);

            aux = aux->next;
        }
    }
    fprintf(f, "\n");
	return;
}

ListNode* unionValues(Dictionary *dictionary, char *value) {
    
    int i, j;
    ListNode *aux, *newNode, *newList = NULL, *prevNode;

    //se parcurge dictionarul, lista cu lista
    for(i = 0; i < dictionary->N; i++) {

        aux = dictionary->lists[i];

        for(j = 0; j < getLength(aux); j++) {

            //daca valoarea e gasita intr-un tuplu, acesta este adaugat listei
            if(strcmp(value, aux->value) == 0) {
                newNode = getNode(aux->key, aux->value, aux->frequency);

                if(newNode != NULL) {
                	if(newList == NULL) {
                    	newList = newNode;
                	}
                	else {
                    	newNode->prev = prevNode;
                    	prevNode->next = newNode;
                	}

                	prevNode = newNode;
            	}
            }
            aux = aux->next;
        }
    }

    return newList;
}

ListNode* unionMaxFrequencies(Dictionary *dictionary) {

	int i, maxim, j = 0, length;
    ListNode *aux, *newNode, *newList = NULL, *prevNode;

    //se parcurge dictionarul, lista cu lista
    for(i = 0; i < dictionary->N; i++) {

        aux = dictionary->lists[i];

        //daca lista e nula trecem pestea ea
        if(aux != NULL) {

            //se ia un maxim egal cu prima frecventa intalnita in lista
            maxim = aux->frequency;
            length = getLength(aux);

            for(j = 0; j < length; j++) {  

                //atata timp cat frecventa este egala cu maximul din acea lista
                //continuam sa parcurgem si sa adaugam 
                if(aux->frequency != maxim)
                    break;           

                newNode = getNode(aux->key, aux->value, aux->frequency);

                if(newNode != NULL) {
                	if(newList == NULL) {
                    	newList = newNode;
                	}
                	else {
                    	newNode->prev = prevNode;
                    	prevNode->next = newNode;
                	}

                	prevNode = newNode;
                }
                aux = aux->next;
            }
        }
    }

	return newList;
}

Dictionary* reverseLists(Dictionary *dictionary) {

    //se aloca memorie pentru un nou dictionar
	Dictionary *newDictionary = createDictionary(dictionary->N);

	if(newDictionary == NULL)
        return NULL;

    ListNode *newNode, *prevNode, *aux;
	int i, j;
    
    //se parcurge dictionarul, lista cu lista
	for(i = 0; i < dictionary->N; i++) {

        //daca lista este nula, trecem peste
        if(dictionary->lists[i] != NULL) {

            //luam ultimul element al listei
            aux = dictionary->lists[i]->prev;
       
            //parcurgem lista si adaugam fiecare nod in lista din noul dictionar
            for(j = 0; j < getLength(dictionary->lists[i]); j++) {

                newNode = getNode(aux->key, aux->value, aux->frequency);

                if(newNode == NULL) {
                    freeDictionary(newDictionary);
                    return NULL;
                }

                //primul element al listei
                if(j == 0) {
                    newDictionary->lists[i] = newNode;
                }
                else {
                    newNode->prev = prevNode;
                    prevNode->next = newNode;
                }

                //legaturi intre primul si ultimul nod pentru circularitate
                if(aux == dictionary->lists[i]) {
                    newNode->next = newDictionary->lists[i];
                    newDictionary->lists[i]->prev = newNode;
                }
                prevNode = newNode;

                //parcurgem lista in ordine inversa
                aux = aux->prev;
            }
        }
	}

	return newDictionary;
}

void printList(FILE *f, ListNode *list) {

	int i, length = getLength(list);

    //se parcurge lista si se scrie fiecare tuplu in fisier
    for(i = 0; i < length; i++) {
        fprintf(f, "(%s, %s, %d) ", list->key, list->value, list->frequency);
        list = list->next;
    }
    fprintf(f, "\n");
	return;
}

void freeList(ListNode *list) {

    if(list == NULL)    return;

    //se parcurge lista si se elibereaza fiecare element
    int length = getLength(list);    
	while(length) {
        ListNode *next = list->next;
        free(list);
        list = next;
        length--;
	}
	return;
}

void freeDictionary(Dictionary *dictionary) {

    if(dictionary == NULL)  return;

    //se parcurge dictionarul si se elibereaza fiecare lista
    int i;
	for(i = 0; i < dictionary->N; i++) {
        freeList(dictionary->lists[i]);
	}

    //se elibereaza vectorul de liste al dictionarului si dictionarul 
	free(dictionary->lists);
	free(dictionary);
	return;
}


#endif

