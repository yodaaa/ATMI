/* cgilib.c  created by Y. Yamagishi Nov. 5, 2009 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* split_str(元々の文字列,切り分けられた文字列,デリミタ（区切り文字）, 最大変換文字数) */

int split_str(char *org_str, char *slice, char delim, int limit){
  
  char *start = org_str; /* org_str文字列の先頭アドレスを開始位置として保存 */

  while(*org_str && *org_str != delim && (org_str - start) < limit) *slice++ = *org_str++; /* デリミタが見つかるまで slice に文字をコピー */

  if(strcmp(slice, org_str) == 0){ /* slice と org_str が一致する場合は、デリミタが見つからなかったことを意味する */

    *start = '\0'; /* org_str を空にする */
    return 1; /* 見付からなかった場合は 1 を返す */
  }

  *slice = '\0';

  if(*org_str) org_str++; /* 文字列の最後でなければデリミタを飛ばす */

  while(*org_str) *start++ = *org_str++; /* 開始位置に切り取った残りをコピー */

  *start = '\0'; /* org_str の末尾に文字列終了の 0 を入れる */

  return 0; /* 正常終了 */

}

/* url_decode(デコードする文字列, デコードされた文字列, 最大変換文字数) */

int url_decode(char *input, char *output, int limit){

  char tmp[3];
  char *start = output;

  while(*input){
    if(*input == '%'){      /* エンコードされた文字の場合 */

      tmp[0] = *(input + 1);  /* 上位4ビット */
      tmp[1] = *(input + 2);  /* 下位4ビット */
      tmp[2] = '\0';        /* 文字列最後の 0 */

      if((output - start) < limit) *output++ = strtol(tmp, NULL, 16); /* 16進文字列→整数変換 */
      else return 1; /* オーバーフロー */
      *(input += 3);       /* 3バイト進める */

    }else{   /* エンコードされていない文字の場合 */

      if((output - start) < limit){
	if(*input == '+') *output++ = ' '; /* + はスペースに変換 */
	else  *output++ = *input; 

	*input++;
      }
      else return 1; /* オーバーフロー */
    }
  } 
  *output = '\0';
  return 0;
}

/* url_encode(コードする文字列, デコードされた文字列, 最大変換文字数) */

int url_encode(char *input, char *output, int limit){

  char tmp[10];
  char *start = output;

  while(*input){

    if((*input < '0' || *input > '9') &&
       (*input < 'A' || *input > 'Z') &&
       (*input < 'a' || *input > 'z') &&
       *input != '*' && 
       *input != '-' && 
       *input != '.' && 
       *input != '@' && 
       *input != ' ' && 
       *input != '_')

      {      /* エンコードされる文字の場合 */

	snprintf(tmp, sizeof(tmp)/sizeof(char),"%x", *input);

	*output = '%';

	if(strlen(tmp) > 2){
	  *(output + 1) = toupper(tmp[6]);
	  *(output + 2) = toupper(tmp[7]);
	}
	else{
	  *(output + 1) = toupper(tmp[0]);
	  *(output + 2) = toupper(tmp[1]);
	}	  

	*(output += 3);       /* 3バイト進める */

      }
    else{   /* エンコードされない文字の場合 */
      
      if((output - start) < limit){
	if(*input == ' ') *output++ = '+'; /* + はスペースに変換 */
	else  *output++ = *input; 

      }
      else return 1; /* オーバーフロー */
    }
    
    *input++;

  } 
  *output = '\0';
  return 0;
}
