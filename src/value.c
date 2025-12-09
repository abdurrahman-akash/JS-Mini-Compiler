#include "value.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Value *new_number_val(double n) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_NUMBER;
    v->as.number = n;
    return v;
}

Value *new_string_val(const char *s) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_STRING;
    v->as.string = malloc(strlen(s) + 1);
    strcpy(v->as.string, s);
    return v;
}

Value *new_boolean_val(int b) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_BOOLEAN;
    v->as.boolean = b ? 1 : 0;
    return v;
}

Value *new_array_val(void) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_ARRAY;
    v->as.array.capacity = 8;
    v->as.array.length = 0;
    v->as.array.elements = malloc(sizeof(Value*) * v->as.array.capacity);
    return v;
}

Value *new_object_val(void) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_OBJECT;
    v->as.object.capacity = 16;
    v->as.object.count = 0;
    v->as.object.entries = malloc(sizeof(ObjectEntry) * v->as.object.capacity);
    return v;
}

Value *new_function_val(char **params, int param_count, ASTNode *body) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_FUNCTION;
    v->as.function.params = params;
    v->as.function.param_count = param_count;
    v->as.function.body = body;
    return v;
}

Value *new_null_val(void) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_NULL;
    return v;
}

Value *new_error_val(const char *message) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_ERROR;
    v->as.string = malloc(strlen(message) + 1);
    strcpy(v->as.string, message);
    return v;
}

void free_value(Value *v) {
    if (!v) return;
    
    switch (v->type) {
        case VAL_STRING:
        case VAL_ERROR:
            free(v->as.string);
            break;
        case VAL_ARRAY:
            for (int i = 0; i < v->as.array.length; i++) {
                free_value(v->as.array.elements[i]);
            }
            free(v->as.array.elements);
            break;
        case VAL_OBJECT:
            for (int i = 0; i < v->as.object.count; i++) {
                free(v->as.object.entries[i].key);
                free_value(v->as.object.entries[i].value);
            }
            free(v->as.object.entries);
            break;
        case VAL_FUNCTION:
            for (int i = 0; i < v->as.function.param_count; i++) {
                // Don't free params - they're managed by AST
            }
            // Don't free params array - managed by AST
            break;
        default:
            break;
    }
    free(v);
}

Value *copy_value(Value *v) {
    if (!v) return NULL;
    
    switch (v->type) {
        case VAL_NUMBER:
            return new_number_val(v->as.number);
        case VAL_STRING:
            return new_string_val(v->as.string);
        case VAL_BOOLEAN:
            return new_boolean_val(v->as.boolean);
        case VAL_NULL:
            return new_null_val();
        case VAL_ERROR:
            return new_error_val(v->as.string);
        case VAL_ARRAY: {
            Value *arr = new_array_val();
            for (int i = 0; i < v->as.array.length; i++) {
                array_push(arr, copy_value(v->as.array.elements[i]));
            }
            return arr;
        }
        case VAL_OBJECT: {
            Value *obj = new_object_val();
            for (int i = 0; i < v->as.object.count; i++) {
                object_set(obj, v->as.object.entries[i].key, 
                          copy_value(v->as.object.entries[i].value));
            }
            return obj;
        }
        case VAL_FUNCTION:
            return new_function_val(v->as.function.params, 
                                   v->as.function.param_count, 
                                   v->as.function.body);
    }
    return NULL;
}

void array_push(Value *arr, Value *val) {
    if (arr->type != VAL_ARRAY) return;
    
    if (arr->as.array.length >= arr->as.array.capacity) {
        arr->as.array.capacity *= 2;
        arr->as.array.elements = realloc(arr->as.array.elements, 
                                        sizeof(Value*) * arr->as.array.capacity);
    }
    arr->as.array.elements[arr->as.array.length++] = val;
}

Value *array_get(Value *arr, int index) {
    if (arr->type != VAL_ARRAY) return new_null_val();
    if (index < 0 || index >= arr->as.array.length) return new_null_val();
    return arr->as.array.elements[index];
}

void array_set(Value *arr, int index, Value *val) {
    if (arr->type != VAL_ARRAY) return;
    if (index < 0) return;
    
    while (index >= arr->as.array.capacity) {
        arr->as.array.capacity *= 2;
        arr->as.array.elements = realloc(arr->as.array.elements, 
                                        sizeof(Value*) * arr->as.array.capacity);
    }
    
    while (index >= arr->as.array.length) {
        arr->as.array.elements[arr->as.array.length++] = new_null_val();
    }
    
    free_value(arr->as.array.elements[index]);
    arr->as.array.elements[index] = val;
}

void object_set(Value *obj, const char *key, Value *val) {
    if (obj->type != VAL_OBJECT) return;
    
    // Check if key exists
    for (int i = 0; i < obj->as.object.count; i++) {
        if (strcmp(obj->as.object.entries[i].key, key) == 0) {
            free_value(obj->as.object.entries[i].value);
            obj->as.object.entries[i].value = val;
            return;
        }
    }
    
    // Add new entry
    if (obj->as.object.count >= obj->as.object.capacity) {
        obj->as.object.capacity *= 2;
        obj->as.object.entries = realloc(obj->as.object.entries, 
                                        sizeof(ObjectEntry) * obj->as.object.capacity);
    }
    
    obj->as.object.entries[obj->as.object.count].key = malloc(strlen(key) + 1);
    strcpy(obj->as.object.entries[obj->as.object.count].key, key);
    obj->as.object.entries[obj->as.object.count].value = val;
    obj->as.object.count++;
}

Value *object_get(Value *obj, const char *key) {
    if (obj->type != VAL_OBJECT) return new_null_val();
    
    for (int i = 0; i < obj->as.object.count; i++) {
        if (strcmp(obj->as.object.entries[i].key, key) == 0) {
            return obj->as.object.entries[i].value;
        }
    }
    return new_null_val();
}

char *value_to_string(Value *v) {
    char *buf = malloc(1024);
    
    switch (v->type) {
        case VAL_NUMBER:
            snprintf(buf, 1024, "%g", v->as.number);
            break;
        case VAL_STRING:
            snprintf(buf, 1024, "%s", v->as.string);
            break;
        case VAL_BOOLEAN:
            snprintf(buf, 1024, "%s", v->as.boolean ? "true" : "false");
            break;
        case VAL_NULL:
            snprintf(buf, 1024, "null");
            break;
        case VAL_ARRAY:
            snprintf(buf, 1024, "[Array]");
            break;
        case VAL_OBJECT:
            snprintf(buf, 1024, "[Object]");
            break;
        case VAL_FUNCTION:
            snprintf(buf, 1024, "[Function]");
            break;
        case VAL_ERROR:
            snprintf(buf, 1024, "Error: %s", v->as.string);
            break;
    }
    return buf;
}

int value_is_truthy(Value *v) {
    switch (v->type) {
        case VAL_NULL:
            return 0;
        case VAL_BOOLEAN:
            return v->as.boolean;
        case VAL_NUMBER:
            return v->as.number != 0.0;
        case VAL_STRING:
            return strlen(v->as.string) > 0;
        default:
            return 1;
    }
}
