#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_CHARS 256

typedef struct trienode
{
    struct trienode *children[NUM_CHARS];
    bool terminal; // mark node to see if it's the end
    int digit;
} trienode;

trienode *createnode()
{
    trienode *newnode = malloc(sizeof *newnode);

    for (int i = 0; i < NUM_CHARS; i++)
    {
        (*newnode).children[i] = NULL;
        // (*newnode).children[i]
        // newnode->children[i]
    }
    (*newnode).terminal = false;
    (*newnode).digit = -1;
    return newnode;
}

bool trieinsert(trienode **root, char *signedtext, int digit)
{
    /*
    function that returns false if we try to insert a word
    that already exists
    else it inserts the word and returns true.

    Need for double pointer in case we don't have a root
    and actually need to create the pointer for the root.
    */

    if (*root == NULL)
    {
        *root = createnode();
    }

    unsigned char *text = (unsigned char *)signedtext;
    trienode *tmp = *root;
    int length = strlen(signedtext);

    for (int i = 0; i < length; i++)
    {
        if ((*tmp).children[text[i]] == NULL)
        {
            // create a new node
            (*tmp).children[text[i]] = createnode();
        }
        tmp = (*tmp).children[text[i]];
    }

    if ((*tmp).terminal)
    {
        return false;
    }
    else
    {
        (*tmp).terminal = true;
        (*tmp).digit = digit;
        return true;
    }
}

trienode *searchterminal(trienode *root, char *signedtext)
// char *signedtext points to a character within a buffer (array)

{
    unsigned char *text = (unsigned char *)signedtext;
    int length = strlen(signedtext);
    trienode *tmp = root;

    for (int i = 0; i < length; i++)
    {
        if ((*tmp).terminal)
        {
            return tmp;
        }

        if ((*tmp).children[text[i]] == NULL)
        {
            return NULL;
        }

        tmp = (*tmp).children[text[i]];
    }

    return (*tmp).terminal ? tmp : NULL;
}

bool node_has_children(trienode *node)
{
    if (node == NULL)
        return false;

    for (int i = 0; i < NUM_CHARS; i++)
    {
        if ((*node).children[i] != NULL)
        {
            // at least one child
            return true;
        }
    }
    return false;
}

trienode *deletestr_aux(trienode *node, unsigned char *text, bool *deleted)
{
    if (node == NULL)
        return node;
    if (*text == '\0')
    {
        if ((*node).terminal)
        {
            (*node).terminal = false;
            *deleted = true;

            if (node_has_children(node) == false)
            {
                free(node);
                node = NULL;
            }
        }
        return node;
    }

    (*node).children[text[0]] = deletestr_aux((*node).children[text[0]], text + 1, deleted);
    // text + 1 will move the pointer to one character to the right
    if (*deleted &&
        node_has_children(node) == false &&
        (*node).terminal == false)
    {
        free(node);
        node = NULL;
    }
    return node;
}

bool deletestr(trienode **root, char *signedtext)
{
    unsigned char *text = (unsigned char *)signedtext;
    bool result = false;
    // true if we deleted something
    // false if we didn't delete anything

    if (*root == NULL)
        return false;

    *root = deletestr_aux(*root, text, &result);
    return result;
}

int main()
{
    trienode *root = NULL;

    trieinsert(&root, "one", 1);
    trieinsert(&root, "two", 2);
    trieinsert(&root, "three", 3);
    trieinsert(&root, "four", 4);
    trieinsert(&root, "five", 5);
    trieinsert(&root, "six", 6);
    trieinsert(&root, "seven", 7);
    trieinsert(&root, "eight", 8);
    trieinsert(&root, "nine", 9);

    long flength = 0;

    FILE *file = fopen("../input.txt", "r");

    fseek(file, 0, SEEK_END);
    flength = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fcontent = malloc(sizeof(char) * flength + 1);

    for (long i = 0; i < flength; i++)
    {
        fcontent[i] = getc(file);
    }

    fcontent[flength] = '\0';

    int first_num = -1;
    int last_num = -1;
    int final_sum = 0;

    for (int i = 0; i < flength; i++)
    {
        char cur_char = fcontent[i];

        if (cur_char == '\n')
        {
            if (first_num == -1)
            {
                continue;
            }

            if (last_num == -1)
            {
                last_num = first_num;
            }

            final_sum += first_num * 10 + last_num;
            first_num = -1;
            last_num = -1;
            continue;
        }

        int cur_number = -1;

        if (cur_char < 48 || cur_char > 57)
        {
            // look up the trie
            // if found:
            //   update cur_number
            // otherwise:
            //   continue

            // trienode *node = searchterminal(root, &(fcontent[i]));
            trienode *node = searchterminal(root, fcontent + sizeof(char) * i);
            // fcontent is a 64 bit integer representing a memory address
            // fcontent points to the memory address of the first character in the buffer (array)
            // sizeof(char) returns the amount of bytes a single variable of type char takes (1 on this arch)
            // since fcontent points to the first character in the buffer, if we want to access the 10th character in that buffer,
            //     we offset it by 10 char length to the right
            // this is the same as doing &(fcontent[i])

            if (node != NULL)
            {
                cur_number = (*node).digit;
            }
            else
            {
                continue;
            }
        }
        else
        {
            cur_number = cur_char - 48;
        }

        if (first_num == -1)
        {
            first_num = cur_number;
            continue;
        }

        last_num = cur_number;
    }

    printf("%d", final_sum);

    // we need to free fcontent because we malloced it at the beginning
    free(fcontent);

    // now we need to free the contents of the trie in order to not have a memory leak
    deletestr(&root, "one");
    deletestr(&root, "two");
    deletestr(&root, "three");
    deletestr(&root, "four");
    deletestr(&root, "five");
    deletestr(&root, "six");
    deletestr(&root, "seven");
    deletestr(&root, "eight");
    deletestr(&root, "nine");
}