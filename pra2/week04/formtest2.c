#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void){
  
  struct QUERY_STR { // フォーム変数構造体
    char name[100];  // フォーム名
    char value[100]; // 値
  };

  struct QUERY_STR query[10];
  struct QUERY_STR *q = query;

  char query_string[100]; // フォーム変数が配流文字列
  char buf[100], decoded_str[100]; //  buf: 一時処理用 decoded_str: URL デコード後の文字列
  char *qs = query_string; // クエリ文字列

  printf("Content-type:text/html\n\n"); // HTML ヘッダを出力

  if(strlen(getenv("QUERY_STRING")) > sizeof(query_string)/sizeof(char)){
    printf("Error! データが長すぎます\n");   // クエリが長すぎる
    exit(1);
  }

  strncpy(query_string, getenv("QUERY_STRING"),(int)sizeof(query_string)/sizeof(char)-1); // 環境変数 QUERY_STRING の内容を query_string にコピー

  while(*qs && (q - query) < (int)sizeof(query)/sizeof(query[0])){ // クエリを一つ一つ処理する
    
    split_str(query_string, buf, '&', (int)sizeof(query_string)/sizeof(char)); // "&" で切り分け
    split_str(buf, q->name, '=', (int)sizeof(buf)/sizeof(char)); // "=" で切り分け、フォーム名を q->name へ

    url_decode(buf, decoded_str, (int)sizeof(buf)/sizeof(char)); // 値をデコード

    strncpy(q->value, decoded_str, (int)sizeof(q->value)/sizeof(char)-1); // 値を q->value へ

    printf("%s = %s<br>\n", q->name, q->value);
    
    qs = query_string; // qs に切り分けられた残りが入った query_string の先頭アドレスを代入
    q++;

  }
  return 0;
}


