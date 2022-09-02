#include "license3c.h"
#include <QDebug>

License3C::License3C()
{
}


void License3C::parseLicense(const QByteArray& byteArray) {
    qDebug() << "parseLicense: " << byteArray;
}
