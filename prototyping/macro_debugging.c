#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>


// LOG macros initialize given parameter
// as LogObject struct type and 
// invokes logger_log 
typedef enum LOGLEVEL {
    DEBUG,
    INFO
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
        char* chars;
    } object;
} LogObject;

#define LOGGABLE_TYPE(T)                    \
    _Generic((T) ,                          \
    char*: CHARS,                           \
    default: NOT_IMPLEMENTED                \
)

#define LOGOBJ(obj) ((LogObject) {.type = LOGGABLE_TYPE((obj)), .object = (obj)})

#define __VA_NUM_LOGOBJS__(...)  (sizeof((LogObject[]){(__VA_ARGS__)})/sizeof(LogObject))

//#define LOG_DEBUG(...) logger_logAll(logger, DEBUG, __VA_NUM_LOGOBJS__(__VA_ARGS__), __VA_ARGS__)
#define LOG_DEBUG(...) logger_logAll(DEBUG, __VA_NUM_LOGOBJS__(__VA_ARGS__), __VA_ARGS__)
#define LOG_INFO(...) logger_logAll(INFO, __VA_NUM_LOGOBJS__(__VA_ARGS__), __VA_ARGS__)


void logger_logAll(LOGLEVEL loglevel, size_t numArgs, ...) {
    return;
}



int main() {

    int a = 1;

    //LOG_DEBUG(LOGOBJ("test"));

    LOGOBJ("test");

    return 0;

}