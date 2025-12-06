#define TESTING "int a;"
// 6. Вывести на экран содержимое файла с программой на С, подсвечивая
//    другим цветом все идентификаторы типа int.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "colors_in_linux.h"

#define setcolor(color) printf(ANSI_BEGIN "%s" ANSI_END, color)

typedef enum {
    STATE_NONE,
    STATE_PREPROC,
    STATE_SLASH,
    STATE_SLASH_SLASH,
    STATE_SLASH_ASTERISK,
    STATE_ASTERISK,
    STATE_i,
    STATE_in,
    STATE_int,
    STATE_int_,
    STATE_int_ID,
    STATE_INT_ID_post,
    STATE_INT_ID_post_eq,
    STATE_doublequote,
    STATE_quote,
    STATE_escape,
    STATE_function,
    STATE_f_i,
    STATE_f_in,
    STATE_f_int,
    STATE_f_int_ID
} State;

struct _Str_Node;
typedef struct _Str_Node {
    struct _Str_Node *next;
    char* s;
} Str_Node;

#define BUF_LEN 256
static char buf[BUF_LEN] = "\0";
static Str_Node *list = 0;

#define isalpha_(c) (isalpha(c) || c == '_')
#define isalnum_(c) (isalnum(c) || c == '_')

int  lexing(char *filename){
    FILE* fp = fopen(filename, "r");
    if (!fp){
        fputs("Failed open file\n", stderr);
        return 1;
    }

    int c; int last_c = 0;
    int deep_statements = 0;
    int is_output = 1; int test_id = 0;
    int with_color = 0, is_ptr = 0;
    char* color = ANSI_RESET "\0\0";
    State state = STATE_NONE;
    State saved_state = STATE_NONE;
    while((c = fgetc(fp)) != EOF){
        switch(state){
            case STATE_NONE:
            proc_state_none:
                if (isalpha_(c) && (isspace(last_c) || (ispunct(last_c) && last_c != '_'))){
                    is_output = 0;
                }
                if( isalnum_(last_c) && (isspace(c) || (ispunct(c) && c != '_'))){
                    is_output = 1; test_id = 1;
                }
                switch (c){
                    case '#':
                        state = STATE_PREPROC;
                        break;
                    case '/':
                    // proc_SLASH:
                        state = STATE_SLASH;
                        break;
                    case 'i':
                    if (isspace(last_c) || (ispunct(last_c) && last_c != '_')){
                            state = STATE_i;
                        }
                        break;
                    case '\"':
                        state = STATE_doublequote;
                        break;
                    case '\'':
                        state = STATE_quote;
                        break;
                    case '(':
                        state = STATE_function;
                        break;
                    case '{':
                        deep_statements++;
                        break;
                    case '}':
                        deep_statements--;
                        break;
                }
                break;

            case STATE_PREPROC:
                if (c == '\n' && last_c != '\\'){
                    state = STATE_NONE;
                }
                break;

            case STATE_doublequote:
                switch(c){
                    case '\\':
                        saved_state = state;
                        state = STATE_escape;
                        break;
                    case '\"':
                        state = STATE_NONE;
                        break;
                }
                break;

            case STATE_quote:
                switch(c){
                    case '\\':
                        saved_state = state;
                        state = STATE_escape;
                        break;
                    case '\'':
                        state = STATE_NONE;
                        break;
                }
                break;

            case STATE_escape:
                state = saved_state;
                saved_state = STATE_NONE;
                break;

            case STATE_SLASH:
                switch (c){
                    case '/':
                        state = STATE_SLASH_SLASH;
                        break;
                    case '*':
                        state = STATE_SLASH_ASTERISK;
                        break;
                    default:
                        state = saved_state;
                        saved_state = STATE_NONE;
                        break;
                }
                break;

            case STATE_SLASH_SLASH: // int a; НЕ ЗАКРАШЕНО
                switch (c){
                    case '\n':
                        state = saved_state;
                        saved_state = STATE_NONE;
                        break;
                }
                break;

            case STATE_SLASH_ASTERISK: /* int a; НЕ ЗАКРАШЕНО */
                switch (c){
                    case '*':
                        state = STATE_ASTERISK;
                        break;
                }
                break;

            case STATE_ASTERISK:
                switch (c){
                    case '/':
                        state = saved_state;
                        saved_state = STATE_NONE;
                        break;
                }
                break;

            case STATE_i:
                switch (c){
                    case 'n':
                        state = STATE_in;
                        break;
                    case '/':
                        state = STATE_SLASH;
                        break;
                    default:
                        state = STATE_NONE;
                        break;
                }
                break;
            
            case STATE_in:
                switch (c){
                    case 't':
                        state = STATE_int;
                        break;
                    case '/':
                        state = STATE_SLASH;
                        break;
                    default:
                        state = STATE_NONE;
                        break;
                }
                break;
            
            case STATE_int:
                if (isalnum_(c)){
                    // обработка integer
                    state = STATE_NONE;
                }else{
                    state = STATE_int_;
                    is_output = 1;
                }
                break;
            
            case STATE_int_:
                switch(c){
                    case '/':
                        saved_state = state;
                        state = STATE_SLASH;
                        break;
                    case '*':
                        is_ptr = 1;
                        break;
                    default:
                        if(isalpha_(c)){
                            color = ANSI_COLOR_BLUE;
                            with_color = is_ptr ? 0 : 1; // ЕСли это int *ptr, то не выделяем
                            is_ptr = 0; // Сбрасываем, чтобы не мешать следущему
                            is_output = 0;
                            state = STATE_int_ID;
                        }
                        break;
                }
                break;

            case STATE_int_ID:
                if( ! (isalnum(c) || c == '_')){
                    state = STATE_INT_ID_post;
                    goto label_INT_ID_post;
                }
                break;

            case STATE_INT_ID_post:
            label_INT_ID_post:
                if (isspace(c)) break;
                switch (c){
                    case '[':
                        with_color = 0;
                        is_output = 0;
                        break;
                    case '=':
                        state = STATE_INT_ID_post_eq;
                        is_output = 1;
                        break;
                    case ',':
                        state = STATE_int;
                        is_output = 1;
                        break;
                    case ';':
                        is_output = 1;
                        state = STATE_NONE;
                        break;
                    case '(':
                        state = STATE_function;
                        with_color = 0;
                        is_output = 1;
                        break;
                }
                break;

            case STATE_INT_ID_post_eq:
                switch (c){
                    case ',':
                        state = STATE_int;
                        break;
                    case ';':
                        state = STATE_NONE;
                        break;
                }
                break;

            case STATE_function:
            proc_STATE_function:
                if (deep_statements){
                    state = STATE_NONE;
                    goto proc_state_none;
                }
                switch (c){
                    case 'i':
                        state = STATE_f_i;
                        break;
                    case ')':
                        state = STATE_NONE;
                        break;
                }
                break;

            case STATE_f_i:
                switch(c){
                    case 'n':
                        state = STATE_f_in;
                        break;
                    default:
                        state = STATE_function;
                        break;
                }
                break;

            case STATE_f_in:
                switch(c){
                    case 't':
                        state = STATE_f_int;
                        break;
                    default:
                        state = STATE_function;
                        break;
                }
                break;

            case STATE_f_int:
                switch(c){
                    case '/':
                        saved_state = state;
                        state = STATE_SLASH;
                        break;
                    case '*':
                        is_ptr = 1;
                        break;
                    default:
                        if(isalpha(c) || c == '_'){
                            color = ANSI_COLOR_BLUE;
                            with_color = is_ptr ? 0 : 1;
                            is_ptr = 0;
                            is_output = 0;
                            state = STATE_f_int_ID;
                        }
                        break;
                }
                break;

            case STATE_f_int_ID:
                if( ! (isalnum(c) || c == '_')){
                    state = STATE_function;
                    is_output = 1;
                    goto proc_STATE_function;
                }
                break;

        }

        if (is_output){
            if (buf[0] != '\0'){
                if (test_id){
                    Str_Node *node = list;
                    while(node != 0){
                        if (strcmp(node->s, buf) == 0){
                            color = ANSI_COLOR_RED;
                            with_color = 1;
                            break;
                        }
                        node = node->next;
                    }
                }

                if (with_color){
                    if ( ! test_id ){
                        // из-за особенностей обработки небходимо при необходимости удалять пробел в конце (перед \0)
                        // появлется при: int is_output = 0; Пробел перед равенством
                        for (size_t i = 0; i < BUF_LEN; i++){
                            if ( ! isalnum_(buf[i])) buf[i] = '\0';
                            if (buf[i] == '\0') break;
                        }

                        Str_Node *node = malloc(sizeof(Str_Node));
                        node->next = 0;
                        node->s = malloc(strlen(buf));
                        strcpy(node->s, buf);

                        static Str_Node **dest = 0;
                        if (list == 0) dest = &list;

                        *dest = node;
                        dest = &(node->next);
                    }
                    fprintf(stderr, "#%s#\n", buf);
                    fprintf(stdout, ANSI_BEGIN "%s" ANSI_END "%s" ANSI(ANSI_RESET), color, buf);
                }else{
                    fprintf(stdout, "%s", buf);
                }
                buf[0] = '\0';
                with_color = 0;
                test_id = 0;
            }
            
            fputc(c, stdout);
        }else{
            strncat(buf, (char*) &c, 1);
        }

        last_c = c;
    }

    if (c != '\n'){
        fputc('\n', stdout);
    }

    return 0;
}
