#ifndef arguments_h
#define arguments_h

#include <stdbool.h>
#include <stdint.h>

#include <valor/hashtable.h>

#define argint(E) (argvalue)(int64_t)E
#define argbool(E) (argvalue)(bool)E
#define argstr(E) (argvalue)E

typedef enum {
    ARG_INT,
    ARG_STR,
    ARG_BOOL
} argtype;

typedef union {
    int64_t intValue;
    char *strValue;
    bool boolValue;
    void *nullValue;
} argvalue;

typedef struct {
    char *name;
    char *description;
    bool compulsory;
    argtype type;
    argvalue value;
    bool is_set;
    bool is_help;
    array_t *values;//Implements multiple arguments pushing something to an array
} argument_t;

extern hashtable_t *arguments;

void arguments_begin(void);

void arguments_set_usage(const char* usage);

argument_t *
argument_create(char *name, char *description, argtype type, bool compulsory, argvalue _default, bool has_default_value,
                bool is_help, bool array);

void argument_add_compulsory(char *name, char *description, argtype type);

void argument_add(char *name, char *description, argtype type, argvalue _default, bool has_default_value, bool is_help);

void arguments_parse(int argc, const char *argv[], int start);

argument_t *argument_get(char *name);

bool argument_check(char *name);

argvalue argument_value_get(char *name);

void arguments_help(const char *progname);

void _register_argument(argument_t *argument);

argvalue argument_value_get_s(char *name, argtype type);

void argument_add_array(char *name, char *description, argtype type, bool compulsory);

void arguments_finalize(void);

#endif /* arguments_h */
