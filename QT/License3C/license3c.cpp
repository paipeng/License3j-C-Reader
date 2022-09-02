#include "license3c.h"
#include <QDebug>
#include <QtEndian>

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

bool License3C::isTypeVariableLength(int type) {
    return (type == Type::BINARY || type == Type::STRING || type == Type::BIGDECIMAL || type == Type::BIGINTEGER);
}

int License3C::getTypeLength(int type) {
    if (type == Type::BYTE) {
        return 1;
    } else if (type == Type::SHORT) {
        return 2;
    } else if (type == Type::INT) {
        return 4;
    } else if (type == Type::LONG) {
        return 8;
    } else if (type == Type::FLOAT) {
        return 4;
    } else if (type == Type::DOUBLE) {
        return 8;
    } else if (type == Type::DATE) {
        return 8;
    } else if (type == Type::UUID) {
        return 16;
    } else {
        return 0;
    }
}

Feature* License3C::parseLicense(const QByteArray& byteArray) {
    qDebug() << "parseLicense: ";// << byteArray;
    Feature* feature = NULL;
    QByteArray magicBytes = byteArray.mid(0, 4);
    if (checkMagicBytes(magicBytes)) {
        qDebug() << "magic bytes valid!";
        feature = (Feature*) malloc(sizeof (Feature));
        memset(feature, 0, sizeof(Feature));
        int pos = 4;
        Feature* nextFeature = feature->next;
        do {
            Feature* newFeature = NULL;
            if (pos > 4) {
                qDebug() << "create next node";
                newFeature = (Feature*) malloc(sizeof (Feature));
                memset(newFeature, 0, sizeof(Feature));
                nextFeature = newFeature;
                nextFeature = nextFeature->next;
            } else {
                newFeature = feature;
            }

            qDebug() << "read binary data";
            // Feature length 4Bytes
            QByteArray featureLength = byteArray.mid(pos, 4);
            pos += 4;
            int featureLen = qFromBigEndian<qint32>(featureLength.data());
            qDebug() << "featureLen: " << featureLen;

            // Feature type:  4 Bytes
            QByteArray featureType = byteArray.mid(pos, 4);
            pos += 4;
            newFeature->type = qFromBigEndian<qint32>(featureType.data());
            qDebug() << "featureType: " << newFeature->type;

            // Name length: 4Bytes
            QByteArray nameLength = byteArray.mid(pos, 4);
            pos += 4;
            newFeature->name_length = qFromBigEndian<qint32>(nameLength.data());
            qDebug() << "nameLen: " << newFeature->name_length;


            // Value Length: 4Bytes optional
            if (isTypeVariableLength(newFeature->type)) {
                QByteArray valueLength = byteArray.mid(pos, 4);
                pos += 4;
                newFeature->value_length = qFromBigEndian<qint32>(valueLength.data());
                qDebug() << "valueLen: " << newFeature->value_length;
            } else {
                newFeature->value_length = getTypeLength(newFeature->type);
            }

            // read name
            QByteArray name = byteArray.mid(pos, newFeature->name_length);
            pos += name.size();
            newFeature->name = (char*)malloc (sizeof(char) * name.size());
            memcpy(newFeature->name, name.data(), sizeof(char) * name.size());

            // read value
            QByteArray value = byteArray.mid(pos, feature->value_length);
            pos += feature->value_length;
            newFeature->value = (char*)malloc (sizeof(char) * value.size());
            memcpy(newFeature->value, value.data(), sizeof(char) * value.size());

            if (pos > 40) {
             //   break;
            }
        } while (pos < byteArray.size());



    } else {
        qDebug() << "magic bytes invalid!";
    }
    return feature;
}
