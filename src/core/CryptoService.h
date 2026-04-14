#ifndef CRYPTOSERVICE_H
#define CRYPTOSERVICE_H

#include <string>
#include <vector>
#include <cryptopp/aes.h>
#include <cryptopp/des.h>
#include <cryptopp/rsa.h>

class CryptoService {
public:
    // AES Methods
    static std::string encryptAES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile);
    static std::string decryptAES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile);

    // DES Methods
    static std::string encryptDES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile);
    static std::string decryptDES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile);

    // RSA Methods
    static std::string generateRSAKeys(const std::string& pubKeyFile, const std::string& privKeyFile);
    static std::string encryptRSA(const std::string& inputFile, const std::string& pubKeyFile, const std::string& outputFile);
    static std::string decryptRSA(const std::string& inputFile, const std::string& privKeyFile, const std::string& outputFile);

private:
    // Helper to save/load keys
    static void SaveKey(const std::string& filename, const CryptoPP::SecByteBlock& key);
    static CryptoPP::SecByteBlock LoadKey(const std::string& filename);
};

#endif // CRYPTOSERVICE_H
