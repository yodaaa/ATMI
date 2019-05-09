#include <stdio.h>
#include <stdlib.h> /* for getenv() */
int main(void){
  printf("Content-type:text/html; charaset=utf-8\n\n");
  printf("あなたのホスト:%s <br>",getenv("REMOTE_ADDR"));
  printf("あなたのブラウザ:%s",getenv("HTTP_USER_AGENT"));
  return 0;
}
