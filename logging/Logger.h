//
// Created by christoph on 29.10.24.
//

#ifndef LEIBNITZ_LOGGER_H
#define LEIBNITZ_LOGGER_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "common/string.h"

// LOG macros initialize given parameter
// as LogObject struct type and 
// invokes logger_log 
typedef enum LOGLEVEL {
    DEBUG,
    INFO, 
    ERROR
}LOGLEVEL;

typedef enum LOGOUTPUT {
    STDERR = 0,
    STDOUT = 1,
    LOGFILE = 2
}LOGOUTPUT;

typedef struct Logger {
    int fd;
    LOGLEVEL loglevel;
}Logger;

typedef enum {
    STRING = 1,
    CHARS = 2,
    COMPLEX = 3,
    VECTOR = 4,
    VECTOR_COLLECTION = 5,
    MATRIX = 6,
    QUREG = 7,
    NDARRAY = 8, 
    NOT_IMPLEMENTED = 9
} LOGGABLE;

typedef struct {
    LOGGABLE type;
    union {
        String str;
        Complex complex;
        Vector vector;
        VectorCollection vectorCollection;
        Matrix matrix;
        QuantumRegister qureg;
        NDArray ndarray;
        char* chars;
    } object;
} LogObject;

#define LOGGABLE_TYPE(T)                     \
    _Generic((T),                            \
    String: STRING,                          \
    char *: CHARS,                           \
    Complex: COMPLEX,                        \
    Vector: VECTOR,                          \
    VectorCollection: VECTOR_COLLECTION,     \
    Matrix: MATRIX,                          \
    QuantumRegister: QUREG,                  \
    NDArray: NDARRAY,                        \
    default: NOT_IMPLEMENTED                 \
)

// declare with attribute unusded to prevent error
__attribute__((unused)) 
static LogObject logObject_create(LOGGABLE loggableType, void *element) {
    
    LogObject obj = {.type = loggableType};


    switch (loggableType)
    {
        case STRING:
            memcpy(&obj.object, element, sizeof(String));
            break;

        case COMPLEX:
            memcpy(&obj.object, element, sizeof(Complex));
            break;

        case VECTOR:
            memcpy(&obj.object, element, sizeof(Vector));
            break;

        case VECTOR_COLLECTION:
            memcpy(&obj.object, element, sizeof(VectorCollection));
            break;

        case MATRIX: 
            memcpy(&obj.object, element, sizeof(Matrix));
            break;

        case QUREG:
            memcpy(&obj.object, element, sizeof(QuantumRegister));
            break;

        case NDARRAY:
            break;

        case CHARS: {
            // Convert to own string 
            String str = string_fromCString(element);
            obj.type = STRING;
            memcpy(&obj.object, &str, sizeof(String));
            break;
        }

        default:
            return (LogObject) { 0 };
            break;
    }
    return obj;
}

#define LOGOBJ(obj) logObject_create(LOGGABLE_TYPE((obj)), &(obj)) 

#define __VA_NUM_LOGOBJS__(...)  (sizeof((LogObject[]){__VA_ARGS__})/sizeof(LogObject))

//#define LOG_DEBUG(...) logger_logAll(logger, DEBUG, __VA_NUM_LOGOBJS__(__VA_ARGS__), __VA_ARGS__)
#define LOG_DEBUG(...) logger_logAll(DEBUG, __VA_NUM_LOGOBJS__(__VA_ARGS__), __VA_ARGS__)
#define LOG_INFO(...) logger_logAll(INFO, __VA_NUM_LOGOBJS__(__VA_ARGS__), __VA_ARGS__)
#define LOG_ERROR(...) logger_logAll(ERROR, __VA_NUM_LOGOBJS__(__VA_ARGS__), __VA_ARGS__)


extern Logger* logger;

Logger* logger_init(LOGOUTPUT output);

void logger_logAll(LOGLEVEL loglevel, size_t numArgs, ...);

void logger_destroy(Logger* logger);


#endif //LEIBNITZ_LOGGER_H
