//
// Created by christoph on 30.10.24.
//

#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#include "Logger.h"


#define stringBuilder_appendObject(_stringBuilder, T)       \
    _Generic((T),                                           \
    Vector: stringBuilder_appendVector,                     \
    Matrix: stringBuilder_appendMatrix,                     \
    Complex: stringBuilder_appendComplex,                   \
    VectorCollection: stringBuilder_appendVectorSet,        \
    String: stringBuilder_appendString                      \
    )                                                       \
    ( (_stringBuilder), (T) )


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


static String getLoggingTimeString() {

    //get time and convert to string
    time_t currentTime;
    currentTime = time(NULL);
    struct tm* currentTimeTM;
    currentTimeTM = localtime(&currentTime);

    char* timeString;
    timeString = asctime(currentTimeTM);
    
    return string_create(timeString, strlen(timeString));
}

static String getLoglevelString(LOGLEVEL loglevel) {
    // get string for Loglevel
    char* levelString = loglevel == DEBUG ? "DEBUG" : "INFO";
    return string_create(levelString, strlen(levelString));
}


Logger* logger_create(LOGOUTPUT output) {

    Logger* logger = malloc(sizeof(Logger));

    switch (output) {

        case STDERR: {
            logger->fd = STDERR;
            break;
        }
        case STDOUT: {
            logger->fd = STDOUT;
            break;
        }
        case LOGFILE: {
                // TODO: use malloc here instead
                // initialize file
                char filePath[] = "/logs/logs.txt";
                int flags = O_CREAT | O_APPEND;
                int fd = open(filePath, flags);
                // check for error
                if (fd == -1) {
                    fprintf(stderr, "Could not open or create logfile \n");
                    return NULL;
                }
        }
    }

    return logger;
}


void logger_log(Logger* logger, LOGLEVEL loglevel, size_t numArgs, ...) {

    StringBuilder* stringBuilder = stringBuilder_create();

    // start parsing vargs
    va_list logObjects;
    va_start(logObjects, numArgs);

    for (size_t argIndex = 0; argIndex < numArgs; argIndex++) {
        
        // retrieve next logObject and parse the internal union type
        LogObject logObj = va_arg(logObjects, LogObject);

        switch (logObj.type) {
            case STRING: 
                // use memcpy to get around typecasting rules for union members
                String str;
                memcpy(&str, &logObj.object, sizeof(String));
                stringBuilder_appendString(stringBuilder, str);
                break;

            case CHARS: 
                // use string type also for the case of null-terminated char array
                String str = string_create(&logObj.object, strlen(&logObj.object));
                stringBuilder_appendString(stringBuilder, str);
                break;

            case COMPLEX: 
                Complex compl;
                memcpy(&compl, &logObj.object, sizeof(Complex));
                stringBuilder_appendComplex(stringBuilder, compl);
                break; 

            case VECTOR: 
                Vector vec;
                memcpy(&vec, &logObj.object, sizeof(Vector));
                stringBuilder_appendVector(stringBuilder, vec);
                break;

            case MATRIX:
                Matrix mat;
                memcpy(&mat, &logObj.object, sizeof(Matrix));
                stringBuilder_appendMatrix(stringBuilder, mat);
                break;

            case VECTOR_COLLECTION:
                VectorCollection vc;
                mempcy(&vc, &logObj.object, sizeof(VectorCollection));
                stringBuilder_appendVectorSet(stringBuilder, vc);
                break;
            
            case NDARRAY:
                NDArray ndarray;
                memcpy(&ndarray, &logObj.object, sizeof(ndarray));
                // todo: implement stringBuilder_appendNDarray
                //stringBuilder_append
                break;

            case QUREG:
                QuantumRegister qureg;
                memcpy(&qureg, &logObj.object, sizeof(QuantumRegister));
                stringBuilder_appendQuantumRegister(stringBuilder, qureg);
                break;           
            
            default: 
                stringBuilder_appendString(stringBuilder, STR("Encountered type not yet implemented in stringBuilder"));
                break;
        }

    }
    va_end(logObjects);

    // finally create the String and write to output 
    String outputString = stringBuilder_build(stringBuilder);
    write(logger->fd, outputString.data, outputString.length);
}


void logger_destroy(Logger* logger) {

    if (logger == NULL) {
        return;
    }

    free(logger);
}
