#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  struct liste_token *prev;
  struct liste_token *next;
}liste_token;

liste_token string_to_token (char *string){
  liste_token* head = NULL;
  head = (liste_token*) malloc(sizeof(liste_token));
  int numcount = 0;
  int val = 0;
  int i;
  while(string[i] != '\0'){
    if((0 <= atoi(string[i])) && (atoi(string[i]) < 10)){
      numcount++;
      val = val + atoi(string[i]);
    }
    else{
      if(numcount > 0){
        liste_token* new_token = (liste_token*) malloc(sizeof(liste_token));
        new_token->t.type = ENTIER;
        new_token->t.val = val;
        numcount = 0;
        val = 0;
      }
      if((string[i] == '(') || (string[i] == ')')){
        liste_token* new_token = (liste_token*) malloc(sizeof(liste_token));
        new_token->t.type = PARENTHESE;
        new_token->t.par = string[i];
      }
      if((string[i] =='+')||(string[i] =='-')||(string[i] =='*')||(string[i] =='/')){
        liste_token* new_token = (liste_token*) malloc(sizeof(liste_token));
        new_token->t.type = OPERATEUR;
        new_token->t.op = string[i];
      }
    }
    i++;
  }
}

int main(int argc, char const *argv[]) {

  return 0;
}
