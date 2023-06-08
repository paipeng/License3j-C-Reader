#include "license3c.h"
#include <QDebug>
#include <QtEndian>
#include <qrsaencryption.h>
#include <QCryptographicHash>
#include <QDateTime>


void insertFeature(Feature* feature, Feature* nextFeature) {
    //qDebug() << "insertFeature";
    Feature* lastFeature = feature;
    while (lastFeature->next != NULL) {
        lastFeature = lastFeature->next;
    }

    lastFeature ->next = nextFeature;
}

void freeFeatures(Feature* feature) {
    Feature* nextFeature = feature->next;
    if (feature->name != NULL) {
        free(feature->name);
        feature->name = NULL;
    }
    if (feature->value != NULL) {
        free(feature->value);
        feature->value = NULL;
    }
    free(feature);
    feature = NULL;
    while (nextFeature != NULL) {
        Feature* tempFeature = nextFeature;
        if (tempFeature->name != NULL) {
            free(tempFeature->name);
            tempFeature->name = NULL;
        }
        if (tempFeature->value != NULL) {
            free(tempFeature->value);
            tempFeature->value = NULL;
        }
        free(tempFeature);
        tempFeature = NULL;
        nextFeature = nextFeature->next;
    }
}
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

Feature* License3C::parseLicense(const QByteArray& byteArray, const QByteArray& publicKeyBytes) {
    qDebug() << "parseLicense: ";// << byteArray;
    Feature* feature = NULL;
    QByteArray magicBytes = byteArray.mid(0, 4);
    if (checkMagicBytes(magicBytes)) {
        qDebug() << "magic bytes valid!";
        feature = (Feature*) malloc(sizeof (Feature));
        memset(feature, 0, sizeof(Feature));
        int pos = 4;
        do {
            Feature* newFeature = NULL;
            if (pos > 4) {
                qDebug() << "create next node";
                newFeature = (Feature*) malloc(sizeof (Feature));
                memset(newFeature, 0, sizeof(Feature));


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
            QByteArray value = byteArray.mid(pos, newFeature->value_length);
            pos += value.size();
            newFeature->value = (char*)malloc (sizeof(char) * value.size());
            memcpy(newFeature->value, value.data(), sizeof(char) * value.size());

            if (pos > 40) {
             //   break;
            }

            // insert into node list
            if (newFeature != feature) {
                insertFeature(feature, newFeature);
            }
//            nextFeature = newFeature;
//            nextFeature = nextFeature->next;
        } while (pos < byteArray.size());



    } else {
        qDebug() << "magic bytes invalid!";
    }


    verify(feature, publicKeyBytes);
    return feature;
}

QByteArray License3C::serialized(Feature* feature) {
    QByteArray serialized;
    if (feature != NULL) {
        //qDebug() << "serialized\n\n\n";
        QByteArray serialized;
        Feature* nextFeature = feature;
        do {
            if (nextFeature->type == 1 && nextFeature->name_length == 16 && strcmp(nextFeature->name, "licenseSignature") == 0) {

            } else {
                //qDebug() << "fetureType: " << nextFeature->type;
                //qDebug() << "nameLen: " << nextFeature->name_length;
                //qDebug() << "valueLen: " << nextFeature->value_length;
                //qDebug() << "name: " <<  QString::fromLocal8Bit(nextFeature->name, nextFeature->name_length);

                QByteArray featureSerialized;
                for(int i = 0; i != sizeof(nextFeature->type); ++i) {
                    featureSerialized.append((char)((nextFeature->type >> (sizeof(nextFeature->type) - 1 - i)* 8) & 0xFF));
                }

                for(int i = 0; i != sizeof(nextFeature->name_length); ++i) {
                    featureSerialized.append((char)((nextFeature->name_length >> (sizeof(nextFeature->name_length) - 1 - i)* 8) & 0xFF));
                }

                if (nextFeature->type == 1 || nextFeature->type == 2) { // VARIABLE_LENGTH
                    for(int i = 0; i != sizeof(nextFeature->value_length); ++i) {
                        featureSerialized.append((char)((nextFeature->value_length >> (sizeof(nextFeature->value_length) - 1 - i)* 8) & 0xFF));
                    }
                }

                featureSerialized.append(nextFeature->name, nextFeature->name_length);
                featureSerialized.append(nextFeature->value, nextFeature->value_length);

                int featureSerializedLength = featureSerialized.length();
                for(int i = 0; i != sizeof(featureSerializedLength); ++i) {
                    serialized.append((char)((featureSerializedLength >> (sizeof(featureSerializedLength) - 1 - i)* 8) & 0xFF));
                }
                serialized.append(featureSerialized);
            }

            //qDebug() << "\n";
            nextFeature = nextFeature->next;
        } while (nextFeature != NULL);

        return serialized;
    } else {
        return serialized;
    }
}

QByteArray License3C::parseSignature(Feature* feature) {
    QByteArray signature;
    if (feature != NULL) {
        Feature* nextFeature = feature;
        do {
            //qDebug() << "parseSignature fetureType: " << nextFeature->type;
            //qDebug() << "nameLen: " << nextFeature->name_length;
            //qDebug() << "valueLen: " << nextFeature->value_length;
            if (nextFeature->type == 1) { //binary
                if (nextFeature->name_length == 16 && strcmp(nextFeature->name, "licenseSignature") == 0) {
                    signature = QByteArray::fromRawData(nextFeature->value, nextFeature->value_length);
                    //qDebug() << "signature found: " << signature.toHex();
                    break;
                }
            }
            nextFeature = nextFeature->next;
        } while (nextFeature != NULL);

        return signature;
    } else {
        return signature;
    }
}

bool License3C::verify(Feature* feature, const QByteArray& publicKeyBytes) {
    qDebug() << "verify pub Hex: " << QString(publicKeyBytes.toHex());

    QRSAEncryption e(QRSAEncryption::Rsa::RSA_2048);

    if (feature != NULL) {
        qDebug() << "check feature\n\n\n";
        QByteArray signature = parseSignature(feature);
        qDebug() << "signature found: " << signature.toHex();
        QByteArray serializedData = serialized(feature);
        qDebug() << "serializedData: " << serializedData.toHex();

        QString MAGIC = "21CE4E5E";
        QByteArray serializedDataAll;

        serializedDataAll.append(QByteArray::fromHex(MAGIC.toUtf8()));
        serializedDataAll.append(serializedData);
        qDebug() << "serializedDataAll: " << serializedDataAll.toHex();
#if 0
        QString debugSerialized = "21ce4e5e0000002100000002000000060000000f646f6d61696e7777772e70616970656e672e636f6d000000160000000b000000066578706972650000018ff73def94000000150000000b000000056973737565000001889a6667940000002000000005000000146d61784167656e74436f6d70616e79436f756e74000000280000001e00000005000000126d617842617463685472616365436f756e740000ea600000001b000000050000000f6d617850726f64756374436f756e74000000c800000019000000050000000d6d61785472616365436f756e740007a12000000018000000050000000c6d617855736572436f756e740000003c000000190000000200000005000000086f776e65725061692050656e6700000022000000020000000f000000077369676e61747572654469676573745348412d353132000000340000000200000004000000247575696464373463366239362d373831342d343633652d383364642d643636613339656262373635";
        QByteArray debugSerializedData = QByteArray::fromHex(debugSerialized.toUtf8());
        if (serializedDataAll == debugSerializedData) {
            qDebug() << "serailized ok";
        } else {

            qDebug() << "serializedDataAll: " << debugSerializedData.toHex();
            qDebug() << "serailized failed";
        }
#endif
        if (e.checkSignMessagePKCS15(serializedDataAll, signature, publicKeyBytes, QRSAEncryption::Auto)) {
            qDebug() << " message signed success";
            return true;
        } else {
            qDebug() << " message signed failed";
            return false;
        }
    } else {
        return false;
    }
}
