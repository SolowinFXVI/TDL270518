#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
/**
 * Enumeration pour les types de tokens.
 */
enum type{
  PARENTHESE = 0,
  OPERATEUR = 1,
  ENTIER = 2
};

/**
*Structure decrivant un token.
**/
typedef struct token {
  int type;//type de token cf -> enum
  char op;//stocker l'OPERATEUR
  char par;//stocker la PARENTHESE
  unsigned int val;//stocker la valeur
}token;

/**
*Structure, liste de tokens.
**/
typedef struct liste_token{
  token t; //token
  struct liste_token *next;//element suivant de la liste
}liste_token;

/**
* Structure d'arbre de tokens.
**/
typedef struct arbre_token{
  token data; //token
  struct arbre_token *left; //fils gauche
  struct arbre_token *right; //fils droit
}arbre_token;

/**
 * Structure de pile.
 **/
typedef struct stack_node{
  token s; //token
  struct stack_node* prev;//element precedent de la pile
}stack_node;

/**
 * Allocation pour un nouveau token dans la liste de tokens
 * @param  t    token
 * @param  next token suivant
 * @return liste de tokens
 */
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

/**
 * Ajoute un nouveau token a la liste de tokens.
 * @param  head liste de tokens
 * @param  t token.
 * @return nouveau debut de la liste des tokens.
 */
liste_token* prepend(liste_token* head, token t){
  liste_token* new_liste_token =  create(t,head);
  head = new_liste_token;
  return head;
}

/**
 * Fonction de test pour afficher les donnees d'un token.
 * @param t token
 */
void printtoken(token t){
  printf("token : type =%d, op=%c, par=%c, val = %u \n", t.type, t.op, t.par, t.val);
}

/**
 * Fonction qui calcule la puissance.
 * @param  x.
 * @param  y.
 * @return INT (ce qui n'est pas le cas des fonctions standard).
 */
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

/**
 * Transforme la chaine de caracteres donnes en entree en liste de tokens.
 * Question 2.
 * @param  head   liste de tokens
 * @param  string chaine de caracteres donnee en entree
 * @return liste de tokens
 */
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

/**
 * Initilisation de la pile.
 * @param top [description]
 */
void init_stack(stack_node** top){
  *top = NULL;
}

/**
 * Fonction de base des pile.
 * @param  top sommet de la pile.
 * @param  s  token a mettre au sommet de la pile.
 * @return  nouveau sommet de pile.
 */
stack_node* push(stack_node* top,token s){
  stack_node* tmp = (stack_node*)malloc(sizeof(stack_node));
  if(tmp == NULL){
    perror("Impossible de creer un element de pile");
    exit(EXIT_FAILURE);
  }
  tmp->s = s;
  tmp->prev = top;
  top = tmp;
  return top;
}

/**
 * Fonction de base d'une pile (ici pop renvoie le nouveau sommet de pile et pas l'element).
 * @param  top sommet de pile.
 * @return nouveau sommet de pile.
 */
stack_node* pop(stack_node *top){
  stack_node* tmp = top;
  top = top->prev;
  free(tmp);
  return top;
}

/**
 * Fonction de pile qui renvoie le token du sommet de la pile.
 * @param  top sommet de pile.
 * @return token au sommet de la pile.
 */
token* top_of_stack(stack_node *top){
  return &top->s;
}

/**
 * Fonction de pile pour tester si la pile est vide.
 * @param  top sommet de pile
 * @return 1 si pile vide, 0 sinon.
 */
int is_empty(stack_node* top){
  return (top == NULL) ? 1 : 0;
}

/**
 * Verifie si une expression donnée en entrée est conforme arithmetiquement.
 * @param  head liste de tokens
 * @return 1(si non conforme) ou 0(si conforme)
 */
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

/**
 * Detruit un liste de tokens
 * @param head liste de tokens
 */
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

/**
 * Inverse une liste de tokens donnee en entree
 * @param  head liste de tokens a(head)->b->c
 * @return head liste de tokens c(head)->b->a
 */
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

/**
 * Creation d'une nouvelle node pour un nouvel element d'arbre
 * @param head liste de tokens
 * @return temp nouvelle node
 */
arbre_token* newNode(liste_token* head){
  arbre_token* temp = (arbre_token*)malloc(sizeof(arbre_token));
  if(temp == NULL){
    perror("impossible de creer un element de l'arbre");
    exit(0);
  }
  temp->left = NULL;
  temp->right = NULL;
  temp->data = head->t;
  return temp;
}

/* Je n'arrive pas a faire fonctionner de cette facon.
arbre_token* maketree(liste_token* head){
  stack_node *top;
  arbre_token *t, *t1, *t2;
  init_stack(&top);
  while(head != NULL){
    if(head->t.type == OPERATEUR){
      t = newNode(head);
      top = push(top,t->data);
    }
    else if(head->t.type == PARENTHESE){
      if(head->t.type.par == ')'){
        top = push(top,t->data);
      }
      else{
        while(top->s.type.par != '('){
          t = newNode()
          top = pop(top);
        }
      }
    }
    else if(head->t.type == ENTIER){
      t1 =
    }

    head=head->next;
  }
  return t;
}*/

/**
 * ###############BUG########
 * Fonction qui calcule le resultat de notre equation a partir de notre liste de tokens.
 * @param head liste de tokens
 * @return val resultat des operations.
 */
int evaluate(liste_token* head){
  token val_tok;//token pour la valeur
  val_tok.type = ENTIER;
  val_tok.op = '!';
  val_tok.par = '!';
  val_tok.val = 0;

  int val = 0;
  int nbr;
  stack_node *top;
  init_stack(&top);

  while(head != NULL){//
    if(head->t.type == PARENTHESE){
      if(head->t.par == ')'){
        top = push(top,head->t);
      }
      if(head->t.par == '('){
        while(top_of_stack(top)->par != ')'){//calcule
          printf("calcule in \n");
          printtoken(*top_of_stack(top));
          if(top_of_stack(top)->type == OPERATEUR){
            if(top_of_stack(top)->op == '+'){
              top = pop(top);
              printtoken(*top_of_stack(top));
              val = nbr + top_of_stack(top)->val;
              printf("valtmp + = %d \n",val);
            }
            else if(top_of_stack(top)->op == '-'){
              top = pop(top);
              printtoken(*top_of_stack(top));
              val = nbr - top_of_stack(top)->val;
              printf("valtmp  - = %d \n",val);
              //Incomplet (cas specifique du moins en temps qu'operateur ou signe)

            }
            else if(top_of_stack(top)->op == '*'){
              top = pop(top);
              printtoken(*top_of_stack(top));
              val = nbr * top_of_stack(top)->val;
              printf("valtmp  * = %d \n",val);
            }
            else if(top_of_stack(top)->op == '/'){
              top = pop(top);
              printtoken(*top_of_stack(top));
              val = nbr / top_of_stack(top)->val;
              printf("valtmp / = %d \n",val);
            }
          }
          else if(top_of_stack(top)->type == ENTIER){
            nbr = top_of_stack(top)->val;
          }
          top = pop(top);
        }
        top = pop(top);
        val_tok.val = val;
        top = push(top,val_tok);
      }
    }
    else{
      top= push(top,head->t);
    }
    //val = 0;
    head = head->next;
  }
  //il faut maintenant finir d'evaluer la pile :
  /*while(is_empty(top)== 0){//calcule
    printf("while calcule final\n");
    printtoken(*top_of_stack(top));
    if(top_of_stack(top)->type == OPERATEUR){
      if(top_of_stack(top)->op == '+'){
        top = pop(top);
        printtoken(*top_of_stack(top));
        val = nbr + top_of_stack(top)->val;
        printf("valtmp final + = %d\n",val);
      }
      if(top_of_stack(top)->op == '-'){
        top = pop(top);
        printtoken(*top_of_stack(top));
        val = nbr - top_of_stack(top)->val;
        printf("valtmp final - = %d\n",val);

        //Incomplet (cas specifique du moins en temps qu'operateur ou signe)

      }
      if(top_of_stack(top)->op == '*'){
        top = pop(top);
        printtoken(*top_of_stack(top));
        val = nbr * top_of_stack(top)->val;
        printf("valtmp final * = %d\n",val);
      }
      if(top_of_stack(top)->op == '/'){
        top = pop(top);
        printtoken(*top_of_stack(top));
        val = nbr / top_of_stack(top)->val;
        printf("valtmp final / = %d\n",val);
      }
    }
    else if(top_of_stack(top)->type == ENTIER){
      nbr = top_of_stack(top)->val;
      printtoken(*top_of_stack(top));
    }
    top = pop(top);
    if(is_empty(top)==0){
      tmp_tok = *top_of_stack(top);
      top = pop(top);
      if(is_empty(top)==1){
        printf("stack empty");
      }
      else{
        top = push(top,tmp_tok);
        val_tok.val = val;
        top = push(top,val_tok);
      }
    }
  }*/
  val = top_of_stack(top)->val;
  while(is_empty(top)==0){
    printtoken(*top_of_stack(top));
    printf("valfinal %d\n",val);
    if(top_of_stack(top)->type == OPERATEUR){
      if(top_of_stack(top)->op == '+'){
        top = pop(top);
        val = val + top_of_stack(top)->val;
        top = pop(top);
        val_tok.val = val;
        top = push(top,val_tok);
      }
      else if(top_of_stack(top)->op == '-'){
        top = pop(top);
        val = val - top_of_stack(top)->val;
        top = pop(top);
        val_tok.val = val;
        top = push(top,val_tok);
      }
      else if(top_of_stack(top)->op == '*'){
        top = pop(top);
        val = val * top_of_stack(top)->val;
        top = pop(top);
        val_tok.val = val;
        top = push(top,val_tok);
      }
      else if(top_of_stack(top)->op == '/'){
        top = pop(top);
        val = val / top_of_stack(top)->val;
        top = pop(top);
        val_tok.val = val;
        top = push(top,val_tok);
      }
    }
    top = pop(top);
  }
  printf("final = %d\n",val);
  return val;
}

/*
int arbre_to_int(arbre_token *at){
  return 0;
};*/


/**
 * Main
 */
int main(int argc, char *argv[]){
  liste_token* head = NULL;

  int i;
  int v = 0;
  int size = argc -1;
  char *str = (char *)malloc(v);

  int res = 0;
  if(argc == 1){
    perror("Equation manquante ?\n");
    exit(EXIT_FAILURE);
  }

  for(i=1;i<=size;i++){
    str = (char*)realloc(str,(v + strlen(argv[i])));
    strcat(str,argv[i]);
  }
  printf("Evaluation de l'equation : %s\n",str);

  head = string_to_token(head,str);
  head = revert(head);//inverse la liste pour evaluer de droite a gauche
  if(check_arithm(head) != 0){
    destroy(head);
    perror("Expression non conforme !\n");
    exit(EXIT_FAILURE);
  }
  res = evaluate(head);
  printf("RESULTAT = %d \n",res);
  destroy(head);//detruit la liste
  printf("Fin.\n");
  return 0;
}
