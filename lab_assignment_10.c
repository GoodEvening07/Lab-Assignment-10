/*
	Gabriel Hernandez
	COP3502C - Lab 10 (Final Lab :)
	08-04-2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    int numOccurances;
    struct TrieNode *children[26];
};

struct Trie *createTrie();
void insert(struct Trie *pTrie, char *word);
struct Trie *deallocateTrie(struct Trie *pTrie);
int numberOfOccurances(struct Trie *pTrie, char *word);
int readDictionary(char *filename, char **pInWords);

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
	struct Trie* s = pTrie;
	int len = strlen(word);
	int i;

	for(i = 0; i < len; i++)
	{
		int index = word[i] - 'a';
		if(s->children[index] == NULL)
		{
			s->children[index] = createTrie();
		}
		s = s->children[index];
	}

	s->numOccurances++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
	int a;
	struct Trie* currentTrie = pTrie;
	int len = strlen(word);

	for(a = 0; a < len; a++)
	{
		int index = word[a] - 'a';
		if(currentTrie->children[index] == NULL)
		{
			return 0;
		}
		currentTrie = currentTrie->children[index];
	}

	return currentTrie->numOccurances;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
	int i;
	if(pTrie != NULL)
	{
		for(i = 0; i < 26; i++)
		{
			if(pTrie->children[i] != NULL)
			{
				deallocateTrie(pTrie->children[i]);
			}
		}
		free(pTrie);
	}
	return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
	struct Trie* pTrie = malloc(sizeof(struct Trie));
	for (int i = 0; i < 26; i++)
	{
		pTrie->children[i] = NULL;
	}
	pTrie->numOccurances = 0;
	return pTrie;

}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
	int numWords;
	int i;
	char word[256];

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File Error Opening: %s\n", filename);
        perror("fopen");
        return 0;
    }

    if (fscanf(file, "%d", &numWords) != 1)
    {
        printf("Error reading the number of words from the file.\n");
        fclose(file);
        return 0;
    }

    for (i = 0; i < numWords && fscanf(file, "%s", word) == 1; i++)
    {
        pInWords[i] = strdup(word);
    }

    fclose(file);
    return i;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}