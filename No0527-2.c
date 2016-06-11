/*
  与えられた数式を逆ポーランド記法に直し、
  計算をする。
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define FORMULA_SIZE 32
#define STACK_SIZE (FORMULA_SIZE / 2)
#define STACK_ERROR -1

typedef struct {
  char data[STACK_SIZE];
  int sp;
}stack_t;

/*　
  数式を逆ポーランド記法にする　
  このとき、数値や演算子の後ろに','を打つ
*/
char *to_rpn(char *formula);

/*
  ','で区切られた逆ポーランド記法の式を計算する
*/
int calc(char *formula);

 /*
  オペレータの優先度を返す
  値が大きいほど優先順位が高い
 */
int rank(char op);

void disp_stack(stack_t stack);
void push(stack_t *stack, int data);
char pop(stack_t *stack);
char peek(stack_t *stack);

int main(void){

  char formula[FORMULA_SIZE];
  int ans;

  printf("input formula :\n");
  scanf("%s", formula);
  ans = calc(formula);

  printf("ans = %d\n", ans);

  return 0;
}

char *to_rpn(char *formula){

  stack_t opstack = {{0}, 0}; /* オペレータのスタック */
  /*最大で FORMULA_SIZE*2の大きさが必要*/
  char *rpn = calloc(FORMULA_SIZE*2+1, sizeof(char*));
  int nbuf;
  char target;
  char pToken;

  nbuf = 0;
  while(*formula != '\0'){

    target = *formula++;
    if(isdigit(target)){
        rpn[nbuf++] = target;    /*　数値なら格納　*/

        if(!isdigit(*formula))
          rpn[nbuf++] = ',';     /* 数値の次の値が数値ではないなら区切る */
    }
    else if(target == '('){
      push(&opstack, target);
    }
    else if(target == ')'){
      while((pToken = pop(&opstack)) != STACK_ERROR && pToken != '('){
        rpn[nbuf++] = pToken;
        rpn[nbuf++] = ',';
      }
    }
    else if(peek(&opstack) == STACK_ERROR){
      push(&opstack, target);
    }
    else{
      while(peek(&opstack) != STACK_ERROR){
        /* 現在のオペレータの優先度がスタックの一番上の優先度より低い　*/
        if(rank(target) < rank(peek(&opstack))){
          rpn[nbuf++] = pop(&opstack);
          rpn[nbuf++] = ',';
        }
        else{
          push(&opstack, target);
          break;
        }
      }
    }
  }

  while(peek(&opstack) != STACK_ERROR){
    rpn[nbuf++] = pop(&opstack);
    rpn[nbuf++] = ',';
  }
  rpn[nbuf] = '\0';

  return rpn;
}

int calc(char *formula){

  stack_t stack = {{0}, 0};
  char buff[FORMULA_SIZE]; /* オペランドやオペレータを格納 */
  char *rpn = to_rpn(formula);
  int tar_a, tar_b, num;
  int i;

  while(*rpn != '\0'){

    i=0;
    while(*rpn != ','){
      buff[i++] = *rpn++;
    }
    buff[i] = '\0';

    switch(buff[0]){

      case '+':
        tar_a = pop(&stack);
        tar_b = pop(&stack);
        push(&stack, tar_b + tar_a);
        break;

      case '-':
        tar_a = pop(&stack);
        tar_b = pop(&stack);
        push(&stack, tar_b - tar_a);
        break;

      case '*':
        tar_a = pop(&stack);
        tar_b = pop(&stack);
        push(&stack, tar_b * tar_a);
        break;

      case '/':
        tar_a = pop(&stack);
        tar_b = pop(&stack);
        if(tar_a == 0) break;
        push(&stack, tar_b / tar_a);
        break;

      default:
        num = atoi(buff);
        push(&stack, num);
        break;
    }

    *rpn++;
  }

  return pop(&stack);
}

int rank(char op){

  if(op == '*' || op == '/') return 2;
  if(op == '+' || op == '-') return 1;
  return -1;
}

void push(stack_t *stack, int data){

  if(stack->sp < STACK_SIZE){
    stack->data[stack->sp] = data;
    stack->sp++;
  }
  else{
    printf("FULL!");
  }
}

char pop(stack_t *stack){

  if(stack->sp > 0){
    stack->sp--;
    return stack->data[stack->sp];
  }
  else{
    printf("EMPTINESS!");
    return STACK_ERROR;
  }
}

char peek(stack_t *stack){

  if(stack->sp > 0){
    return stack->data[stack->sp - 1];
  }
  else{
    return STACK_ERROR;
  }
}
