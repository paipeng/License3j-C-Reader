# License3j-C-Reader
An Implementation of License3j Reader in C

This library is used for reading license file generated by https://github.com/verhas/License3j. License Format is matched to read the license features.

Magic bytes
0xCE, 0x21, 0x5E, 0x4E


Feature length 4bytes
Feature type 4bytes
Name length 4bytes
Value length 4bytes (currently only for: BINARY, STRING, BIGINTEGER and BIGDECIMAL types)


- BINARY 1, VARIABLE_LENGTH
- STRING(2, VARIABLE_LENGTH
- BYTE(3, Byte.BYTES
- SHORT(4, Short.BYTES
- INT(5, Integer.BYTES
- LONG(6, Long.BYTES
- FLOAT(7, Float.BYTES
- DOUBLE(8, Double.BYTES
- BIGINTEGER(9, VARIABLE_LENGTH
- BIGDECIMAL(10, VARIABLE_LENGTH
- DATE(11, Long.BYTES
- UUID(12, 2 * Long.BYTES



UTF-8 character set



Signature verify
PKCS#1v1.5 Padding

- RSA 2048 Public Key loading
- Serialization License
- SHA-512 HASHING
- RSA Decoding Signature with Public Key
- modified Qt-Secret Library https://github.com/paipeng/Qt-Secret