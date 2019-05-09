#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void){
  
  struct QUERY_STR {
    char name[100];
    char value[100];
  }; // クエリ構造体

  struct QUERY_STR query[10]; // 10個までのクエリに対応
  struct QUERY_STR *q = query; // query のポインタ

  char query_string[100]; // クエリの文字列
  char buf[100], decoded_str[100]; // バッファとデコードされたクエリ
  char *qs = query_string; // クエリ文字列のポインタ

  double height, weight, bmi; // 変数

  int query_length = 0; // クエリの長さ

  printf("Content-type:text/html\n\n"); // HTTP ヘッダ

  /* 共通部分 HTML */

  printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"  \"http://www.w3.org/TR/html4/loose.dtd\">\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\">\n");
  printf("<title>BMIの計算 POST 版</title>\n");
  printf("</head>\n");


  if (getenv("CONTENT_LENGTH") != NULL)
    query_length = atol(getenv("CONTENT_LENGTH")); // クエリの長さ

  if(!query_length){ // クエリなしの場合はフォームを表示

    printf("<body bgcolor=\"#ffffff\">\n");
    printf("<form method=\"POST\" action=\"/cgi-bin/bmi-post.cgi\"><p>\n");
    printf("身長：<input type=\"text\" name=\"height\">\n");
    printf("<br>\n");
    printf("体重：<input type=\"text\" name=\"weight\">\n");
    printf("<input type=\"submit\" value=\"送信\">\n");
    printf("</p></form>\n");

  }else{ // クエリ有りの場合
    
    /* データチェック（バリデーション） */

    if(query_length > sizeof(query_string)/sizeof(char)){
      printf("Error! データが長すぎます\n");
      exit(1);
    }
    
    /* クエリの入力( POST の場合は標準入力から) */

    if(!fread(query_string, query_length, 1, stdin)){
      printf("Error! 入力がありません\n");
      exit(1);
    }

    query_string[query_length] = '\0'; // 文字列の最後は 0 にしないといけない
    
    while(*qs && (q - query) < (int)sizeof(query)/sizeof(query[0])){
      
      split_str(query_string, buf, '&', (int)sizeof(query_string)/sizeof(char)); // & で切り分け、 buf には切り分けられた文字列が入り、query_string には残りの文字列の先頭アドレスが入る
      split_str(buf, q->name, '=', (int)sizeof(buf)/sizeof(char)); // = で切り分け、q->name に変数名、buf にはデコードされる前の値が入る

      url_decode(buf, decoded_str, (int)sizeof(buf)/sizeof(char)); // buf の値をデコードして decoded_str  に入れる

      strncpy(q->value, decoded_str, (int)sizeof(q->value)/sizeof(char)-1); // q->value に値を入れる
    
      if(strcmp(q->name, "height") == 0) height = atof(q->value); //height に身長を代入
      if(strcmp(q->name, "weight") == 0) weight = atof(q->value); //weight に体重を代入

      qs = query_string; // 切り分けられた残りの文字列の先頭にポインタ qs を設定
      q++; // ポインタ q をインクリメント

    }

    bmi = weight / (height * height);

    printf("身長: %f 体重 : %f<br>\n", height, weight);
    printf("あなたの BMI は %f です。<br>\n", bmi);

  }

    printf("</body>\n");
    printf("</html>\n");

    return 0;
}
