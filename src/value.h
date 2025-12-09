#ifndef VALUE_H
#define VALUE_H

#include "ast.h"

typedef enum {
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_ARRAY,
    VAL_OBJECT,
    VAL_FUNCTION,
    VAL_NULL,
    VAL_ERROR
} ValueType;

typedef struct Value Value;
typedef struct ObjectEntry ObjectEntry;

struct ObjectEntry {
    char *key;
    Value *value;
};

struct Value {
    ValueType type;
    union {
        double number;
        char *string;
        int boolean;
        struct {
            Value **elements;
            int length;
            int capacity;
        } array;
        struct {
            ObjectEntry *entries;
            int count;
            int capacity;
        } object;
        struct {
            char **params;
            int param_count;
            ASTNode *body;
        } function;
    } as;
};

Value *new_number_val(double n);
Value *new_string_val(const char *s);
Value *new_boolean_val(int b);
Value *new_array_val(void);
Value *new_object_val(void);
Value *new_function_val(char **params, int param_count, ASTNode *body);
Value *new_null_val(void);
Value *new_error_val(const char *message);

void free_value(Value *v);
Value *copy_value(Value *v);

void array_push(Value *arr, Value *val);
Value *array_get(Value *arr, int index);
void array_set(Value *arr, int index, Value *val);

void object_set(Value *obj, const char *key, Value *val);
Value *object_get(Value *obj, const char *key);

char *value_to_string(Value *v);
int value_is_truthy(Value *v);

#endif
