#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

enum type{
  PARENTHESE = 0,
  OPERATEUR = 1,
  ENTIER = 2
};

typedef struct token {
  int type;
  char op;
  char par;
  unsigned int val;
}token;

typedef struct liste_token{
  token t;
  struct liste_token *next;
}liste_token;

liste_token* create(token t,liste_token* next){
  liste_token* new_liste_token = (liste_token*)malloc(sizeof(liste_token));
  if(new_liste_token == NULL){
    perror("Impossible de creer une nouvelle liste_token \n");
    exit(1);
  }
  new_liste_token->t = t;
  new_liste_token->next = next;
  return new_liste_token;
}

liste_token* prepend(liste_token* head, token t){
  liste_token* new_liste_token =  create(t,head);
  head = new_liste_token;
  return head;
}

void printtoken(token t){
  printf("token : type =%d, op=%c, par=%c, val = %u \n", t.type, t.op, t.par, t.val);
}

int power(int x, int y){
  if(y==0){
    return 1;
  }
  else if(y%2 == 0){
    return power(x, y/2)*power(x,y/2);
  }
  else{
    return x*power(x,y/2)*power(x,y/2);
  }
}

liste_token* string_to_token (liste_token* head, char *string){
  int length = strlen(string);
  int numcount = 0;
  int val = 0;
  token t;
  while(length >= 0){
    if(isdigit(string[length])){
      val = val + ((string[length] - '0') * power(10,numcount));
      numcount++;
    }
    else{
      if(numcount != 0){
        t.type = ENTIER;
        t.par ='!';
        t.op='!';
        t.val=val;
        head = prepend(head,t);
        printtoken(t);
      }
      numcount = 0;
      val = 0;
      if((string[length] == '(') || (string[length] == ')')){
        t.type = PARENTHESE;
        t.par=string[length];
        t.op='!';
        t.val= 0;
        head = prepend(head,t);
        printtoken(t);
      }
      if((string[length] == '+') || (string[length] == '-') || (string[length] == '*') || (string[length] == '/')){
        t.type = OPERATEUR;
        t.par ='!';
        t.op = string[length];
        t.val = 0;
        head = prepend(head,t);
        printtoken(t);
      }
    }
    length--;
  }
  if(numcount != 0){ //cas ou le premier char d'entree est un chiffre
    t.type = ENTIER;
    t.par ='\0';
    t.op='\0';
    t.val=val;
    head = prepend(head,t);
    printtoken(t);
  }
  return head;
}

int check_arithm(liste_token *head){
  int parenthese = 0;
  int op = 0;
  int num = 0;
  liste_token *ptr = head;
  if(head != NULL){
    while(ptr != NULL){
      if(ptr->t.type == PARENTHESE){
        if(ptr->t.par == '('){
          parenthese--;
        }
        if(ptr->t.par == ')'){
          parenthese++;
        }
      }
      if(num > 0){//test pour deux entiers a la suite
        if(ptr->t.type == ENTIER){
          printf("Deux entiers successifs\n");
          return 1;
        }
        else{
          num--;
        }
      }
      if(ptr->t.type == ENTIER){
        num++;
      }
      if(op > 0){
        if(ptr->t.type == OPERATEUR){
          if(op == 2){
            if(ptr->t.type == '-'){//evite --
              printf("Deux '-' successifs\n");
              return 1;
            }
            else{
              op = 1;
            }
          }
          else{//evite deux operateurs succesifs
            printf("Deux operateurs successifs\n");
            return 1;
          }
        }
        if(ptr->t.type == PARENTHESE){
          if(op == 2){//cas du '-'
            op = 0;
          }
          else if(ptr->t.par == '('){//evite un operateur apres une parenthese
            printf("Operateur different de - apres parenthese '(' \n");
            return 1;
          }
        }
        if(ptr->t.type == ENTIER){
          op = 0;
        }
      }
      if(ptr->t.type == OPERATEUR){
        if(ptr->t.op == '-'){
          op = 2;
        }
        else{
          op = 1;
        }
      }
      if(parenthese == -1){//test pour : commence par la mauvaise parenthese
        printf("Parentheses Incorrectes\n");
        return 1;
      }
      ptr = ptr->next;
    }
  }
  else{
    perror("expression vide");
    exit(2);
  }
  if(parenthese != 0){
    printf("retour parenthese incorrect \n");
    return 1;
  }
  if(op == 1){
    printf("retour operateur incorrect \n");
    return 1;
  }
  printf("Expression Conforme\n");
  return 0;
}

void destroy(liste_token *head){
  liste_token *ptr, *tmp;
  if(head != NULL){
    ptr = head->next;
    head->next = NULL;
    while(ptr != NULL){
      tmp= ptr->next;
      free(ptr);
      ptr =tmp;
    }
    free(head);
  }
}

liste_token* revert(liste_token* head){
  liste_token* ptr = head;
  liste_token* prev = NULL;
  liste_token* next;
  while(ptr != NULL){
    next = ptr->next;
    ptr->next = prev;
    prev = ptr;
    ptr = next;
  }
  head = prev;
  return head;
}

int main(int argc, char *argv[]) {
  liste_token* head = NULL;
  if(argc <= 1){
    perror("Equation manquante ?\n");
    exit(EXIT_FAILURE);
  }
  head = string_to_token(head,argv[1]);
  head = revert(head);//inverse la liste pour evaluer de droite a gauche
  if(check_arithm(head) != 0){
    destroy(head);
    perror("Expression non conforme !\n");
    exit(EXIT_FAILURE);
  }
  destroy(head);//detruit la liste
  return 0;
}
