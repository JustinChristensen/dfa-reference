#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum state {
    START,
    COMMENT_START,
    SINGLE_COMMENT,
    MULTI_COMMENT,
    MULTI_ASTERISK,
    COMMENT_END,
    STRING_LITERAL,
    CHARACTER_LITERAL,
    STRING_ESCAPE,
    CHARACTER_ESCAPE
};

enum state transition(enum state current_state, char c)
{
    switch (current_state) {
        case START:
            if (c == '/')
                current_state = COMMENT_START;
            else if (c == '\'')
                current_state = CHARACTER_LITERAL;
            else if (c == '"')
                current_state = STRING_LITERAL;
            break;
        case COMMENT_START:
            if (c == '/')
                current_state = SINGLE_COMMENT;
            else if (c == '*')
                current_state = MULTI_COMMENT;
            else
                current_state = START;
            break;
        case SINGLE_COMMENT:
            if (c == '\n')
                current_state = COMMENT_END;
            break;
        case MULTI_COMMENT:
            if (c == '*')
                current_state = MULTI_ASTERISK;
            break;
        case MULTI_ASTERISK:
            if (c == '/')
                current_state = COMMENT_END;
            else if (c != '*')
                current_state = MULTI_COMMENT;
            break;
        case COMMENT_END:
            if (c == '/')
                current_state = COMMENT_START;
            else if (c == '"')
                current_state = STRING_LITERAL;
            else
                current_state = START;
            break;
        case STRING_LITERAL:
            if (c == '\\')
                current_state = STRING_ESCAPE;
            else if (c == '"')
                current_state = START;
            break;
        case CHARACTER_LITERAL:
            if (c == '\\')
                current_state = CHARACTER_ESCAPE;
            else if (c == '\'')
                current_state = START;
            break;
        case STRING_ESCAPE:
            current_state = STRING_LITERAL;
            break;
        case CHARACTER_ESCAPE:
            current_state = CHARACTER_LITERAL;
            break;
        default:
            break;
    }

    return current_state;
}

bool accepts(enum state current_state) {
    return current_state == SINGLE_COMMENT ||
           current_state == MULTI_COMMENT ||
           current_state == MULTI_ASTERISK ||
           current_state == COMMENT_END;
}

int main(int argc, char *argv[])
{
    char c, d;
    enum state current_state = START, previous_state;

    while ((c = getc(stdin)) != EOF) {
        current_state = transition(current_state, c);

        if (current_state == COMMENT_START) {
            previous_state = current_state;
            d = getc(stdin);
            current_state = transition(current_state, d);

            if (!accepts(current_state)) {
                ungetc(d, stdin);
                current_state = previous_state;
                putc(c, stdout);
            }
        }
        else {
            if (!accepts(current_state) || c == '\n') {
                putc(c, stdout);
            }
        }
    }
}
