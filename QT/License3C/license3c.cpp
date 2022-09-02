#include "license3c.h"
#include <QDebug>

License3C::License3C()
{
}

bool License3C::checkMagicBytes(const QByteArray magicByte) {
    qDebug() << "checkMagicBytes: " << magicByte.toHex();
    return (((unsigned char)magicByte.at(0) == 0x21) &&
            ((unsigned char)magicByte.at(1) == 0xCE) &&
            ((unsigned char)magicByte.at(2) == 0x4E) &&
            ((unsigned char)magicByte.at(3) == 0x5E));
}
License* License3C::parseLicense(const QByteArray& byteArray) {
    qDebug() << "parseLicense: " << byteArray;
    License* license = NULL;
    QByteArray magicBytes = byteArray.mid(0, 4);
    if (checkMagicBytes(magicBytes)) {
        qDebug() << "magic bytes valid!";

        // Feature length 4Bytes
        // Feature type:  4 Bytes
        // Name length: 4Bytes
        // Value Length: 4Bytes optional, only for
    } else {
        qDebug() << "magic bytes invalid!";
    }
    return license;
}
