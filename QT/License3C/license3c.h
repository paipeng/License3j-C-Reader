#ifndef LICENSE3C_H
#define LICENSE3C_H

#include "License3C_global.h"

class LICENSE3C_EXPORT License3C
{
public:
    License3C();
    static bool checkMagicBytes(const QByteArray magicByte);
    static void parseLicense(const QByteArray& byteArray);
};

#endif // LICENSE3C_H
