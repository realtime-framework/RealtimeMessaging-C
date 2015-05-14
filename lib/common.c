#include <sys/time.h>

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void _ortc_exception(ortc_context *context, char *exception){
  if(context->onException)
    context->onException(context, exception);
}

int _ortc_isValidUrl(ortc_context *context, char *url){    
  struct cap pmatch[3];
  if (0 != slre_match(&context->reValidUrl, url, (int)strlen(url), pmatch)) { 
    return 1;
  }
  return 0;
}

int _ortc_isValidInput(ortc_context *context, char *input){
  struct cap pmatch[3];
  if (0 != slre_match(&context->reValidInput, input, (int)strlen(input), pmatch)) { 
    return 1;
  }
  return 0;
}

int _ortc_initRestString(ortc_RestString *t) {
  t->len = 0;
  t->ptr = malloc(t->len+1);
  if (t->ptr == NULL) {
    return -1;
  }
  t->ptr[0] = '\0';
  return 0;
}

size_t _ortc_writeRestString(void *ptr, size_t size, size_t nmemb, ortc_RestString *s){
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

char* _ortc_replace(char *s, char *old, char *newStr){
  char *ret;
	size_t i;
  int count = 0;
  size_t newlen = strlen(newStr);
  size_t oldlen = strlen(old);

  for (i = 0; s[i] != '\0'; i++) {
    if (strstr(&s[i], old) == &s[i]) {
      count++;
      i += oldlen - 1;
    }
  }

  ret = (char*)malloc(i + count * (newlen - oldlen) + 1);
  if (ret == NULL){
    fprintf(stderr, "malloc() failed in ortc replace\n");
    exit(EXIT_FAILURE);
  }

  i = 0;
  while (*s) {
    if (strstr(s, old) == s) {
      strcpy(&ret[i], newStr);
      i += newlen;
      s += oldlen;
    } else
      ret[i++] = *s++;
  }
  ret[i] = '\0';
  
  return ret;
}

char* _ortc_remove(char* s, char *remove){
  char *ret;
  size_t i;
  int count = 0;
  size_t removelen = strlen(remove);

  for (i = 0; s[i] != '\0'; i++) {
    if (strstr(&s[i], remove) == &s[i]) {
      count++;
      i += removelen - 1;
    }
  }
  ret = (char*)malloc(i - count * removelen + 1);
  if (ret == NULL){
    fprintf(stderr, "malloc() failed in ortc remove\n");
    exit(EXIT_FAILURE);
  }

  i = 0;
  while (*s) {
    if (strstr(s, remove) == s) {
      s += removelen;
    } else
      ret[i++] = *s++;
  }
  ret[i] = '\0';
  
  return ret;
}

char* _ortc_get_from_slre(int groupIdx, struct cap *captures){
  char *ret = (char*)malloc(captures[groupIdx].len + 1);
  if(ret == NULL){
    fprintf(stderr, "malloc() failed in ortc get from slre\n");
    exit(EXIT_FAILURE);
  }
  memcpy(ret, captures[groupIdx].ptr, captures[groupIdx].len);
  ret[captures[groupIdx].len] = '\0';
  return ret;
}

char* _ortc_prepareConnectionPath(){
  int r;
  char *ret;
  char path[33], s[8];

  srand((unsigned int)time(NULL));
  r = rand() % 1000;
  _ortc_random_string(s, 8);
    snprintf(path, sizeof path, "/broadcast/%d/%s/websocket", r, s);
  ret = (char*)malloc(strlen(path)+1);
  memcpy(ret, path, strlen(path));
  ret[strlen(path)] = '\0';
  return ret;
}

unsigned int _ortc_random_string_counter = 0;
void _ortc_random_string(char * string, size_t length){
  int i, r;
  size_t num_chars;

  num_chars = length - 1;
  srand((unsigned int) time(0) + _ortc_random_string_counter);
  _ortc_random_string_counter++;
  //ASCII characters: 48-57, 65-90, 97-122
  for (i = 0; i < num_chars; ++i){
    r = rand() % 59;
    if(r < 10) {
      string[i] = r + 48; //digits
    } else if(r < 34) {
      string[i] = r + 55; //65 - 10
    } else {
      string[i] = r + 63; //97 -34
    }
  } 
  string[num_chars] = '\0';  
}

char* _ortc_escape_sequences_before(char *string){
  char *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9;
  s0 = _ortc_replace(string, "\\", "\\\\");
  s1 = _ortc_replace(s0, "\a", "\\\\a");
  free(s0);
  s2 = _ortc_replace(s1, "\b", "\\b");
  free(s1);
  s3 = _ortc_replace(s2, "\f", "\\f");
  free(s2);
  s4 = _ortc_replace(s3, "\n", "\\n");
  free(s3);
  s5 = _ortc_replace(s4, "\r", "\\r");
  free(s4);
  s6 = _ortc_replace(s5, "\t", "\\t");
  free(s5);
  s7 = _ortc_replace(s6, "\v", "\\\\v");
  free(s6);
  s8 = _ortc_replace(s7, "\'", "\\\\'");
  free(s7);
  s9 = _ortc_replace(s8, "\"", "\\\"");
  free(s8);
  return s9;
}

char* _ortc_escape_sequences_after(char *string){
  char *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9;
  s0 = _ortc_replace(string,   "\\\\\\\"", "\"");
  s1 = _ortc_replace(s0, "\\\\\\\\a", "\a");
  free(s0);
  s2 = _ortc_replace(s1, "\\\\\\\\v", "\v");
  free(s1);
  s3 = _ortc_replace(s2, "\\\\\\\\'", "\'");
  free(s2);
  s4 = _ortc_replace(s3, "\\\\\\\\", "\\");
  free(s3);
  s5 = _ortc_replace(s4, "\\\\b", "\b");
  free(s4);
  s6 = _ortc_replace(s5, "\\\\f", "\f");
  free(s5);
  s7 = _ortc_replace(s6, "\\\\n", "\n");
  free(s6);
  s8 = _ortc_replace(s7, "\\\\r", "\r");
  free(s7);
  s9 = _ortc_replace(s8, "\\\\t", "\t");
  free(s8);
  return s9;
}

char* _ortc_ch_ex_msg(char *msg, char *channel) {
    int exp_msg_size = strlen(channel) + strlen(msg) + 5;
    char *exp_msg = (char *) malloc(sizeof(char) * exp_msg_size);
    sprintf(exp_msg, "%s: %s", msg, channel);
    return exp_msg;
}
