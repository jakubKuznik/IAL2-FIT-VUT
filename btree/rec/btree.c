/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

  bst_node_t *temp_node = NULL;
  if (tree != NULL){

    // If empty
    if(key == tree->key){
      *value = tree->value;
      return true;
    }
    // if key is bigger go right
    else if(key > tree->key){
      temp_node = tree->right;
    } // Go left if key is smaller 
    else{
      temp_node = tree->left;
    }
    // Recusrive call 
    return bst_search(temp_node, key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  
  // go left or right and call itself 
  if ((*tree) != NULL){
    // if key is bigger go right
    if (key > (*tree)->key) {
      bst_insert(&(*tree)->right, key, value);
    }
    // if key is smaller go left
    else if (key < (*tree)->key) {
      bst_insert(&(*tree)->left, key, value);
    } // if key is alredy in table 
    else{
      (*tree)->value = value;
      return;
    }
  }
  
  // alloc space and insert 
  else{
    (*tree) = malloc(sizeof(bst_node_t));
    if((*tree) == NULL){ //Malloc failed 
      return;
    }
    
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->right = NULL;
    (*tree)->left = NULL;
    return;
  }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  bst_node_t **temp_tree;
  bst_node_t *temp_node;

  if ((*tree)->right != NULL) { // go to right
    temp_tree = &(*tree)->right;
    bst_replace_by_rightmost(target, temp_tree);
  }
  else{ //the most right node 
    target->value = (*tree)->value;
    target->key = (*tree)->key;
    
    if((*tree)->left == NULL){
      free(*tree);
      (*tree) = NULL;  
    }
    else{
      temp_node = (*tree);
      (*tree) = temp_node->left;
      free(temp_node);
    }
  }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  
  bst_node_t **temp_tree;
  bst_node_t *temp_node;

  if ((*tree) != NULL) {
    // If empty
    if(key == (*tree)->key){
      temp_node = *tree;

      if(temp_node->left == NULL){
        *tree = temp_node->right;
        free(temp_node);
      }
      else if(temp_node->right == NULL){
        *tree = temp_node->left;
        free(temp_node);
      }
      else {
        temp_tree = &(*tree)->left;
        bst_replace_by_rightmost(*tree, temp_tree);
      }
      return;  
    }
    // if key is bigger go right
    else if(key > (*tree)->key) {
      temp_tree = &(*tree)->right;
    } // Go left if key is smaller 
    else {
      temp_tree = &(*tree)->left;
    }
    // Recusrive call 
    bst_delete(temp_tree, key);
  }

}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {

  if((*tree) != NULL){
    bst_node_t **temp;
    // Recursive call left.
    temp =  &(*tree)->left;
    bst_dispose(temp);

    // Recursive call right 
    temp =  &(*tree)->right;
    bst_dispose(temp);

    free(*tree);
    // same state as after init.
    *tree = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if(tree != NULL){
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if(tree != NULL){
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if(tree != NULL){
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
