/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

  //table = (ht_table_t*)malloc(sizeof(ht_table_t) + HT_SIZE * sizeof(ht_item_t));

  // Set all the h tab nodes to NULL.
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }


}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  
  int hash = get_hash(key);
  ht_item_t *item_temp = (*table)[hash];

  // Go throw whole list until end (NULL) is reached.
  while (item_temp != NULL)
  {
    if (strcmp(item_temp->key, key) == 0){
      return item_temp;
    }
    item_temp = item_temp->next;
  }
  
  // didn't find item. 
  return NULL;

}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {


  // Find if item is alredy in table 
  ht_item_t *temp_item = ht_search(table, key);
   
  // If item is alredy in table just change value. 
  if(temp_item != NULL){
    temp_item->value = value;
    return;
  }

  // Allocate space on heap for new node 
  ht_item_t *new_temp_item = (ht_item_t*) malloc(sizeof(ht_item_t));
  // If malloc fail
  if(new_temp_item == NULL){
    return;
  }

  // Init 
  new_temp_item->next = NULL;
  new_temp_item->key = key;
  new_temp_item->value = value;

  int hash = get_hash(key);

  temp_item = (*table)[hash];
  if(temp_item != NULL){ // if not on begining 
    new_temp_item->next = temp_item;
    (*table)[hash] = new_temp_item;
  }
  else{ //If begin 
    (*table)[hash] = new_temp_item;
  }
  return;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {

  ht_item_t *item = ht_search(table, key);

  // If item is in table return pointer to its value. 
  if(item != NULL){
    return &item->value;
  }

  return NULL;

}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {

  int hash = get_hash(key);

  ht_item_t * temp_item = (*table)[hash];
  ht_item_t * temp_item_next = temp_item->next;
  ht_item_t * temp_item_prev = NULL;

  if (temp_item == NULL){
    return;
  }

  // Do till reach end of list 
  while (temp_item != NULL){

    temp_item_prev = temp_item;
    temp_item = temp_item->next;
    temp_item_next = temp_item_next;

  // If key match 
    if(strcmp(key, temp_item->key) == 0){
      
      if(temp_item_prev == NULL){
        (*table)[hash] = temp_item_next;
        break;
      }
      else{
        free(temp_item);
        temp_item_prev->next = temp_item->next;      
        break;
      }
    }
  }
  return;
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {

  // If empty table. 
  if(table == NULL){
    return;
  }

  ht_item_t *temp_item = NULL;
  ht_item_t *temp_item_next = NULL;

  // For every list 
  for(int i = 0; i < HT_SIZE; i++){
    temp_item = (*table)[i];
    // Go to end of list 
    while(temp_item != NULL){

      temp_item_next = temp_item->next;
      free(temp_item);
      temp_item = temp_item_next;

    }
    // Init begin to null 
    (*table)[i] = NULL;
  }
  return;
}
