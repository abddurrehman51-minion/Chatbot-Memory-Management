#ifndef PARSER_H
#define PARSER_H

char *parser_process(const char *raw);
void parser_free(char *text);

char *parse_input(const char *input);
void parser_trim(char *text);
void parser_normalize(char *text);
int  parser_validate(const char *text);

void censor_text(char *text);
int  contains_blocked_word(const char *text);
int  parser_check_policy(const char *text);
void parser_apply_policy(char *text);
void parser_debug_dump(const char *stage, const char *text);
#endif
