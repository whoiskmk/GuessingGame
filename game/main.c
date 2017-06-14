//
//  main.c
//  Guessing Game
//
//  Created by Kelly Krawczyk on 3/26/15.
//  Copyright (c) 2015 Kelly Krawczyk. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_struct {
    char* questionPtr;
    char* animalPtr;
    struct node_struct* parent;
    struct node_struct* rightPtr;
    struct node_struct* leftPtr;
}node;

const int SIZE = 255;
char question[SIZE];
char animal[SIZE];
char answer[SIZE];
char newQuestion[SIZE];
char newAnimal[SIZE];
char newAnswer[SIZE];

node* newQuestionNode(char* question, node* rightPtrNode, node* leftPtrNode){
    node* newQuestion = malloc(sizeof(node));
    newQuestion -> questionPtr = question;
    newQuestion -> rightPtr = rightPtrNode;
    newQuestion -> leftPtr = leftPtrNode;
    newQuestion -> animalPtr = NULL;
    return newQuestion;
}

node* newAnimalNode(char* animal, node* rightPtrNode, node* leftPtrNode){
    node* newAnimal = malloc(sizeof(node));
    newAnimal -> animalPtr = animal;
    newAnimal -> rightPtr = rightPtrNode;
    newAnimal -> leftPtr = leftPtrNode;
    newAnimal -> questionPtr = NULL;
    return newAnimal;
}

void cleanInput(char * mystring){
    if ( mystring[ strlen( mystring ) - 1] == '\n') {mystring[ strlen( mystring ) - 1] = '\0'; }
    if ( mystring[ strlen( mystring ) - 1] == '\r') {mystring[ strlen( mystring ) - 1] = '\0';}
}

void inOrderPrint(node* root){
    
    if (root == NULL) {
        printf("");
        return;
    }
    inOrderPrint(root -> rightPtr);
    if (root -> questionPtr == NULL) {
        printf("%s", root -> animalPtr);
    }
    if (root -> animalPtr == NULL) {
        printf("\t--\t%s\t--\t", root -> questionPtr);
        inOrderPrint(root -> leftPtr);
    }
}
void Print(node* root){
    
    if (root == NULL) {
        printf("");
        return;
    }
    Print(root -> rightPtr);
    if (root -> questionPtr == NULL) {
        while (root -> parent != NULL) {
            printf("\t");
        }
        printf("+--%s\n", root -> animalPtr);
    }
    if (root -> animalPtr == NULL) {
        printf("\t|\n");
        printf("%s\n", root -> questionPtr);
        printf("\t|\n");
    }
    Print(root -> leftPtr);
}

void inOrderLeaves(node* leaf) {
    
    if (leaf == NULL) {
        printf("");
        return;
    }
    inOrderLeaves(leaf -> rightPtr);
    if (leaf -> animalPtr == NULL) {
        printf("");
    }
    else if (leaf -> animalPtr != NULL) {
        printf("%s\n", leaf -> animalPtr);
    }
    inOrderLeaves(leaf -> leftPtr);
}

void saveLeaves(node* root, FILE* filename){
    
    filename = NULL;
    filename = fopen("myfile.txt", "w");
    
    if (root == NULL) {
        return;
    }
    if (root -> questionPtr == NULL) {
        fwrite(root -> animalPtr, sizeof(node), SIZE, filename);
    }
    fclose(filename);
}

node* play(node* root){
    
    FILE* filename = NULL;
    
    if (root -> questionPtr == NULL) {

        printf("Is it a %s?\n", root -> animalPtr);
        fgets(answer, SIZE, stdin);
        
        if (answer[0] == 'Y' || answer[0] == 'y') {
            printf("Wow!  I guess it!\n");
            return 0;
        }
        else if (answer[0] == 'N' || answer[0] == 'n') {
            printf("I guessed wrong.\n");
            printf("What animal were you thinking of?\n");
            fgets(newAnimal, SIZE, stdin);
            cleanInput(newAnimal);
            
            printf("What question should I have asked to tell your %s and my %s apart?\n", newAnimal, root -> animalPtr);
            fgets(newQuestion, SIZE, stdin);
            cleanInput(newQuestion);
            
            printf("Is the correct answer to your question yes or no?\n");
            fgets(newAnswer, SIZE, stdin);
            cleanInput(newAnswer);
            
            node* newQuestionPtr = newQuestionNode(newQuestion, NULL, NULL);
            node* newAnimalPtr = newAnimalNode(newAnimal, NULL, NULL);
            
            if (newAnswer[0] == 'Y' || newAnswer[0] == 'y') {
                newQuestionPtr -> parent = root -> parent;
                newQuestionPtr -> rightPtr = newAnimalPtr;
                newQuestionPtr -> rightPtr -> parent = newQuestionPtr;
                newQuestionPtr -> leftPtr = root;
                if (answer[0] == 'Y' || answer[0] == 'y') {
                    root -> parent -> rightPtr = newQuestionPtr;
                    newQuestionPtr -> leftPtr -> parent = newQuestionPtr;
                    saveLeaves(newAnimalPtr, filename);
                }
                if (answer[0] == 'N' || answer[0] == 'n') {
                    root -> parent -> leftPtr = newQuestionPtr;
                    newQuestionPtr -> leftPtr -> parent = newQuestionPtr;
                }
            }
            if (newAnswer[0] == 'N' || newAnswer[0] == 'n') {
                newQuestionPtr -> parent = root -> parent;
                newQuestionPtr -> leftPtr = newAnimalPtr;
                newQuestionPtr -> leftPtr -> parent = newQuestionPtr;
                newQuestionPtr -> rightPtr = root;
                if (answer[0] == 'Y' || answer[0] == 'y') {
                    root -> parent -> rightPtr = newQuestionPtr;
                    newQuestionPtr -> rightPtr -> parent = newQuestionPtr;
                    saveLeaves(newAnimalPtr, filename);
                }
                if (answer[0] == 'N' || answer[0] == 'n') {
                    root -> parent -> leftPtr = newQuestionPtr;
                    newQuestionPtr -> rightPtr -> parent = newQuestionPtr;
                }
            }
        }
    }
    else if (root -> animalPtr == NULL) {
        
        printf("%s\n", root -> questionPtr);
        fgets(answer, SIZE, stdin);
        if (answer[0] == 'Y' || answer[0] == 'y') {
            play(root -> rightPtr);
        }
        else if (answer[0] == 'N' || answer[0] == 'n') {
            play(root -> leftPtr);
        }
        
    }
    return 0;
}


int main(){
    
    char playAgain[SIZE] = "yes";
    
    char* chickenPtr = "chicken";
    cleanInput(chickenPtr);
    node* chicken = newAnimalNode(chickenPtr, NULL, NULL);
    
    char* tigerPtr = "tiger";
    cleanInput(tigerPtr);
    node* tiger = newAnimalNode(tigerPtr, NULL, NULL);
    
    char* treeQuestion = "Does it have feathers?";
    cleanInput(treeQuestion);
    node* tree = newQuestionNode(treeQuestion, chicken, tiger);
    
    chicken -> parent = tree;
    tiger -> parent = tree;

    while (playAgain[0] == 'Y' || playAgain[0] == 'y') {
        
        printf("Do you want to print the tree as you play?\n");
        fgets(answer, SIZE, stdin);
        cleanInput(answer);
        
        printf("Think of an animal.\n");
        
        if (answer[0] == 'Y' || answer[0] == 'y') {
            play(tree);
            Print(tree);
        }
        else if (answer[0] == 'N' || answer[0] == 'n') {
            play(tree);
        }
        printf("\nDo you want to play again?\n");
        fgets(playAgain, SIZE, stdin);
        cleanInput(playAgain);
    }
    return 0;
}


