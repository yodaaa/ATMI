#include <stdio.h>
#include <stdlib.h>

int main(void){
  printf("Content-type:text/html\n\n");
  printf("QUERY_STRING:%s", getenv("QUERY_STRING"));
  return 0;
}
