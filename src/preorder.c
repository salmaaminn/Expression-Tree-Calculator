/*
Salma Amin
samin06
1281885
Oct 31 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    float value;          //stores numbers Y.YY
    char operation;       //stores operations
    char variable[3];    //stores x1,x2...
    struct node *right;
    struct node *left;
};
//malloc node
struct node *newNode(float value, char operation) {
    struct node *ptr = malloc(sizeof(struct node));
    if (!ptr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    ptr->value = value;
    ptr->operation = operation;
    ptr->variable[0] = '\0'; //?
    ptr->left = NULL;
    ptr->right = NULL;
    return ptr;
}

//store numbers from variables
void getVar(char *varString, float variables[10]) {
    // Initialize all variables to 0
    for (int i = 0; i < 10; i++) {
        variables[i] = 0.0f;
    }
    
    int i = 0;
    int varIndex = 1;  // Start with x1
    
    while (varString[i] != '\0' && varIndex < 10) {
        // Look for "x" followed by digit and "="
        if (varString[i] == 'x' && 
            varString[i+1] >= '1' && varString[i+1] <= '9' && 
            varString[i+2] == '=') {
            
            // Move past "xN="
            i += 3;
            
            // Parse the number value
            char numStr[5];
            int j = 0;
            while (varString[i] != ';' && varString[i] != '\0' && j < 5) {
                numStr[j] = varString[i];
                i++;
                j++;
            }
            numStr[j] = '\0';
            
            // Store the value
            variables[varIndex] = atof(numStr);
            varIndex++;
            
            // Skip semicolon if present
            if (varString[i] == ';') {
                i++;
            }
        } else {
            i++;  // Move to next character
        }
    }
}
//free malloced nodes
void freeTree(struct node* root) {
    if (root == NULL){
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

//check for variable error
int varError(char* variable, float variables[10]) {
    if (variable[0] == 'x' && variable[1] >= '1' && variable[1] <= '9') {
        int varInd = variable[1] - '0'; //get index of variable for array
        return variables[varInd] != 0.0f; //check for 0 value
    }        
        return 0; 
}
    
//check for 0 divisor
int divError(struct node* root, float variables[10]) {
    if (root == NULL){
        return 0;  //if reached end of tree, no error
    }
    if (root->operation == '/') { //if theres a division check for 0
        if (root->right != NULL) { //check if a right node (to check for divisor) exists
            if (root->right->operation == '0') { //if its not an operation, then it is a divisor (number)
                if (root->right->variable[0] != '\0') { //if true, the diviser is a variable
                    if (varError(root->right->variable, variables)) { //check for variable error
                        //check for zero divisor
                        int getIndex = root->right->variable[1] - '0'; //getIndex
                        if (variables[getIndex] == 0.0f) { //check for zero
                            return 1;
                        }
                    }
                  //if not a avariable check for integer 0;
                } else {
                    if (root->right->value == 0.0f) {
                        return 1;
                    }
                }
            }
        }
    }  
    //recursivley check rest of tree
    return divError(root->left, variables) || divError(root->right, variables);
}
//call for each variable
int varsError(struct node* root, float variables[10]) {
    if (root == NULL){
        return 0; //if reached the end of the tree no error
    }
    if (root->operation == '0' && root->variable[0] != '\0') {
        if (!varError(root->variable, variables)) {
            return 1;
        }
    }
    
    return varsError(root->left, variables) || varsError(root->right, variables);
}

//making expresssion tree
struct node* expressionTree(char* input, int* index, int* syntaxError);

struct node* termNode(char* input, int* index, int* syntaxError) {
    if (*index >= (int)strlen(input)) {
        *syntaxError = 1;
        return NULL;
    }
    //when input is a brakcet
    if (input[*index] == '(') {
        (*index)++; // move to next char in expression
        struct node* node = expressionTree(input, index, syntaxError); //recursivley call string expression to add nodes between the brackets
        //if the step above fails
        if (node == NULL){ 
            return NULL;  
        }
        //if the number of characters ran out or didnt find the closing bracket then there is a an error
        if (*index >= (int)strlen(input) || input[*index] != ')') { 
            *syntaxError = 1;
            freeTree(node);
            return NULL;
        }
        (*index)++;
        return node;
    }
    //when input is a variable
    else if (input[*index] == 'x' && *index + 1 < (int)strlen(input) && input[*index + 1] >= '1' && input[*index + 1] <= '9') {
        //create and store node for variable
        struct node* varNode = newNode(0, '0'); 
        varNode->variable[0] = 'x';
        varNode->variable[1] = input[*index + 1];
        varNode->variable[2] = '\0';
        *index += 2; //move to next char
        return varNode;
    }
    //when input is a number
    else if (input[*index] >= '0' && input[*index] <= '9') {
        float num = input[*index] - '0';  //convert to integer
        (*index)++; //move to decimal point
        //if string ends or the next char is not a decimal point there is a syntax error
        if (*index >= (int)strlen(input) || input[*index] != '.') {
            *syntaxError = 1;
            return NULL;
        }
        (*index)++;
        //next char -> number in the 10th place
        if (*index >= (int)strlen(input) || input[*index] < '0' || input[*index] > '9') {
            *syntaxError = 1;
            return NULL;
        }
        float fraction = (input[*index] - '0') * 0.1f; //convert into float
        (*index)++;
        //next char -> number in the 100th place
        if (*index >= (int)strlen(input) || input[*index] < '0' || input[*index] > '9') {
            *syntaxError = 1;
            return NULL;
        }
        fraction += (input[*index] - '0') * 0.01f; //convert to float
        (*index)++;
        
        num += fraction; //complete number
        return newNode(num, '0'); //make a new node for number
    }
    //if none of the above, there is an error
    else {
        *syntaxError = 1;
        return NULL;
    }
}

struct node* expressionTree(char* input, int* index, int* syntaxError) {
    //if input is empty or there are missing terms
    if (*index >= (int)strlen(input)) {
        *syntaxError = 1;
        return NULL;
    }
    
    struct node* left = termNode(input, index, syntaxError); //gets first term
    //if termNode failed
    if (left == NULL){
        return NULL;
    }
    while (*index < (int)strlen(input) && (input[*index] == '+' || input[*index] == '-' || input[*index] == '*' || input[*index] == '/')) {
        
        char op = input[*index]; //store operation
        (*index)++;
        
        struct node* right = termNode(input, index, syntaxError); //node for number after operator
        //if termNode failed
        if (right == NULL) {
            freeTree(left);
            return NULL;
        }
        //create node for operation
        struct node* opNode = newNode(0, op);
        opNode->left = left;  //first number (left of operator)
        opNode->right = right; //number right of operater
        left = opNode;  //assign the expression to the left for if its nested
    }
    
    return left; //return expression built
}


//preorder
void preorder(struct node* root) {
    //if end is reached, return
    if (root == NULL){
        return; 
    }
    //if operation, print
    if (root->operation != '0') {
        printf("%c ", root->operation);
    //if variable, print
    } else if (root->variable[0] != '\0') {
        printf("%s ", root->variable);
    //if number, print
    } else {
        printf("%.2f ", root->value);
    }
    //traverse left subtree first
    preorder(root->left);
    //then treverse4 right tree
    preorder(root->right);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    

    char *input = argv[1];  //take in expression
    float variables[10] = {0};
    
    //add definition of variables (if exists) to array
    if (argc >= 3) {
        getVar(argv[2], variables);
    }
    
    int index = 0;
    int syntaxError = 0;
    //build tree
    struct node *tree = expressionTree(input, &index, &syntaxError);
    
    //check for syntax error
    if (syntaxError || tree == NULL || index < (int)strlen(input)) {
        fprintf(stderr, "Error: syntax error.\n");
        if (tree != NULL) freeTree(tree);
        return 1;
    }
    //check for undefined argument
    if (varsError(tree, variables)) {
        fprintf(stderr, "Error: undefined argument.\n");
        freeTree(tree);
        return 1;
    }
    //check for zivision by zero
    if (divError(tree, variables)) {
        fprintf(stderr, "Error: division by zero.\n");
        freeTree(tree);
        return 1;
    }
    //traverse tree using preorder
    preorder(tree);
    printf("\n");
    
    freeTree(tree);
    return 0;
}
