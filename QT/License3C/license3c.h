#ifndef LICENSE3C_H
#define LICENSE3C_H

#include "License3C_global.h"
#include <QByteArray>

typedef struct _feature {
    int name_length;
    char* name;
    int type;
    int value_length;
    char* value;
    struct _feature* next;
} Feature;


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
LICENSE3C_EXPORT void insertFeature(Feature* feature, Feature* nextFeature);
LICENSE3C_EXPORT void freeFeatures(Feature* feature);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
class LICENSE3C_EXPORT License3C
{
public:
    License3C();
    static bool checkMagicBytes(const QByteArray magicByte);
    static Feature* parseLicense(const QByteArray& byteArray, const QByteArray& publicKeyBytes);
    static bool isTypeVariableLength(int type);
    static int getTypeLength(int type);
    static bool verify(Feature* feature, const QByteArray& publicKeyBytes);
    enum Type {
        BINARY = 1, // VARIABLE_LENGTH
        STRING = 2, // VARIABLE_LENGTH
        BYTE = 3, // 1 byte
        SHORT = 4, // 2 bytes
        INT = 5, // 4 bytes
        LONG = 6, // 8 bytes
        FLOAT = 7, // 4 bytes
        DOUBLE = 8, // 8 bytes
        BIGINTEGER = 9, // VARIABLE_LENGTH
        BIGDECIMAL = 10, // VARIABLE_LENGTH
        DATE = 11, // Long.BYTES 8
        UUID = 12 // 2 * Long.BYTES 16
    };
};

#endif // LICENSE3C_H
