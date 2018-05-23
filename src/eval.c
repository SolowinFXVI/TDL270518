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
  liste_token *next;
}liste_token;

liste_token string_to_token (char *string){

  
}

int main(int argc, char const *argv[]) {

  return 0;
}
