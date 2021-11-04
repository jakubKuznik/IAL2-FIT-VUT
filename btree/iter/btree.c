/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

  //
  bst_node_t *temp_node = tree;
  
  if (tree != NULL){
    // till leaf level reached or key found 
    while(temp_node != NULL){
      if(key == temp_node->key){
        *value = tree->value;
        return true;
      }
      // if key is bigger go right
      else if(key > tree->key){
        temp_node = temp_node->right;
      } // Go left if key is smaller 
      else{
        temp_node = temp_node->left;
      }
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  
  bst_node_t *temp_2 = NULL;
  bst_node_t *temp = (*tree);
  bool left;

  // If empty tree create  
  if((*tree) == NULL){
    temp_2 = malloc(sizeof(bst_node_t));
    if(temp_2 == NULL){ //Malloc failed 
      return;
    }
    temp_2->key = key;
    temp_2->value = value;
    temp_2->left = NULL;
    temp_2->right = NULL;
    
    (*tree) = temp_2;
    return;
  }

  // conditon for first node 
  // If node is same as first node 
  if((temp->key == key)){
    temp->value = value;
    return;
  }
  // Do until leaf level or until there is same key   
  while(true){
    if (key > temp->key) {
      left = false;
      if(temp->right != NULL)
        temp = temp->right;
      else
        break;
    } // if key is smaller go left
    else if (key < temp->key) {
      left = true;
      if(temp->left != NULL)
        temp = temp->left;
      else
        break;
    } // if key is alredy in table
    else{
      (*tree)->value = value;
      return;
    }
  } 

  temp_2 = malloc(sizeof(bst_node_t));
  if(temp_2 == NULL){ //Malloc failed 
    return;
  }
  temp_2->key = key;
  temp_2->value = value;
  temp_2->left = NULL;
  temp_2->right = NULL;

  if(left == true){
    temp->left = temp_2;    
  }
  else{
    temp->right = temp_2;
  }
  return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  // temporarily helping variables 
  bst_node_t *temp = NULL;
  bst_node_t *temp_prev = NULL;

  // set as first node 
  temp = (*tree);

  while(temp->right != NULL){
    temp_prev = temp;
    temp = temp->right;
  }

  target->key = temp->key;
  target->value = temp->value;

  if(temp_prev == NULL){
    free(temp);
    return;
  }


  if(temp->left == NULL) // Without this my left node could be lost 
    temp_prev->right = NULL;
  else 
    temp_prev->right = temp->left;


  free(temp);
  return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  
  if((*tree) != NULL)
  {
    // temporarily helping variables 
    bst_node_t *temp = NULL;
    bst_node_t *temp_prev = NULL;
    bool left;

    // set as first node 
    temp = (*tree);

    // Do until leaf level or until there is same key   
    while(true){

      if (key > temp->key) {
        left = false;
        if(temp->right != NULL){
          temp_prev = temp;
          temp = temp->right;
        }
        else
          return;
      } // if key is smaller go left
      else if (key < temp->key) {
        left = true;
        if(temp->left != NULL){
          temp_prev = temp;
          temp = temp->left;
        }
        else
          return;
      } // if key is alredy in table
      else{
        break;
      }
    } 

  if(temp->left != NULL && temp->right != NULL){
    bst_replace_by_rightmost(temp, &temp->left);
  }
  else if(temp->right != NULL){
    if (left == true){
      temp_prev->left = temp->right;
    }
    else{
      temp_prev->right = temp->right;
    }
  }
  else{
    if (left == true){
      temp_prev->left = temp->left;
    }
    else{
      temp_prev->right = temp->left;
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  
  // If tree is empty dont do anything
  if((*tree) != NULL){
    
    // temp variables 
    bst_node_t * node_temp = *tree;
    bst_node_t * node_delete = NULL;

    // stack alocation  
    stack_bst_t *stack_temp = calloc(1, sizeof(stack_bst_t));
    if(stack_temp == NULL) //calloc failed 
      return;

    // stack init 
    stack_bst_init(stack_temp);
    
    // store everything to stack  
    while(node_temp != NULL){

      if(node_temp->right != NULL){
        stack_bst_push(stack_temp, node_temp->right);
      }     
      node_delete = node_temp;
      node_temp = node_temp->left;
      free(node_delete);
    }
  
    //free stack 
    while(stack_bst_empty(stack_temp) != true){
      stack_bst_pop(stack_temp);
    }
    // tree is same as after init 
    *(tree) = NULL;
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {

  while(tree != NULL){
    bst_print_node(tree);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
  return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  // If node is not emtpy
  if(tree != NULL){

    //temp variable 
    bst_node_t *node_temp = NULL;

    // init stack 
    stack_bst_t stack_temp;
    stack_bst_init(&stack_temp);

    bst_leftmost_preorder(tree, &stack_temp);

    while(true){
      if(stack_bst_empty(&stack_temp) == true)
        break;
      node_temp = stack_bst_pop(&stack_temp);
      if (node_temp->right != NULL){
        bst_leftmost_preorder(node_temp->right, &stack_temp);
      }      
    }
  }
}


/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {

  // go throw left branch and store everything to stack 
  for(;tree != NULL; tree = tree->left){
    stack_bst_push(to_visit, tree);
  }  
}



/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  // if node is null dont do anything 
  if (tree != NULL){
    
    //temp variable 
    bst_node_t *node_temp = NULL;

    // init stack 
    stack_bst_t stack_temp;
    stack_bst_init(&stack_temp);

    bst_leftmost_inorder(tree, &stack_temp);

    while(true){
      if(stack_bst_empty(&stack_temp) == true)
        break;
      node_temp = stack_bst_pop(&stack_temp);
      bst_print_node(node_temp);
      if (node_temp->right != NULL){
        bst_leftmost_inorder(node_temp->right, &stack_temp);
      }      
    }
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  
  for(;tree != NULL; tree = tree->left){
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
  }

}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {

  // If empty tree don't do anything 
  if(tree != NULL){
    
    //declare stack variables 
    stack_bst_t temp_stack; 
    stack_bool_t temp_bool_stack;
  
    //init stack variables
    stack_bst_init(&temp_stack);
    stack_bool_init(&temp_bool_stack);


    bst_leftmost_postorder(tree, &temp_stack, &temp_bool_stack);


    while(!stack_bst_empty(&temp_stack)){
     
      tree = stack_bst_pop(&temp_stack);
     
      if(stack_bool_pop(&temp_bool_stack) == true){
        stack_bst_push(&temp_stack, tree);
        stack_bool_push(&temp_bool_stack, false);
        bst_leftmost_postorder(tree->right, &temp_stack, &temp_bool_stack);
      }
     
      else{
        bst_print_node(tree);
      }
    }
    return;
  }



}
