#include "CryptoService.h"
#include <fstream>
#include <iterator>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/sha.h>

using namespace CryptoPP;

// Helper function definitions
void CryptoService::SaveKey(const std::string& filename, const SecByteBlock& key) {
    StringSource ss(key.data(), key.size(), true, new FileSink(filename.c_str()));
}

SecByteBlock CryptoService::LoadKey(const std::string& filename) {
    std::string keyStr;
    FileSource fs(filename.c_str(), true, new StringSink(keyStr));
    return SecByteBlock((const CryptoPP::byte*)keyStr.data(), keyStr.size());
}

// AES Implementations
std::string CryptoService::encryptAES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
    try {
        AutoSeededRandomPool prng;
        SecByteBlock key(AES::MAX_KEYLENGTH);
        prng.GenerateBlock(key, key.size());
        SaveKey(keyFile, key);

        SecByteBlock iv(AES::BLOCKSIZE);
        prng.GenerateBlock(iv, iv.size());

        CBC_Mode<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(key, key.size(), iv);

        std::string cipher;
        FileSource(inputFile.c_str(), true, new StreamTransformationFilter(encryptor, new StringSink(cipher)));

        std::ofstream out(outputFile, std::ios::binary);
        out.write((const char*)iv.BytePtr(), iv.size());
        out.write(cipher.data(), cipher.size());
        out.close();

        return "AES Encryption Success!\nKey: " + keyFile + "\nFile: " + outputFile;
    } catch (const std::exception& e) {
        return "AES Error: " + std::string(e.what());
    }
}

std::string CryptoService::decryptAES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
    try {
        SecByteBlock key = LoadKey(keyFile);
        std::ifstream in(inputFile, std::ios::binary);
        SecByteBlock iv(AES::BLOCKSIZE);
        in.read((char*)iv.BytePtr(), iv.size());

        std::string cipher((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        std::string recovered;

        CBC_Mode<AES>::Decryption decryptor;
        decryptor.SetKeyWithIV(key, key.size(), iv);
        StringSource(cipher, true, new StreamTransformationFilter(decryptor, new StringSink(recovered)));

        std::ofstream out(outputFile, std::ios::binary);
        out.write(recovered.data(), recovered.size());

        return "AES Decryption Success!\nFile: " + outputFile;
    } catch (const std::exception& e) {
        return "AES Decryption Error: " + std::string(e.what());
    }
}

// DES Implementations
std::string CryptoService::encryptDES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
    try {
        AutoSeededRandomPool prng;
        SecByteBlock key(DES::DEFAULT_KEYLENGTH);
        prng.GenerateBlock(key, key.size());
        SaveKey(keyFile, key);

        SecByteBlock iv(DES::BLOCKSIZE);
        prng.GenerateBlock(iv, iv.size());

        CBC_Mode<DES>::Encryption encryptor;
        encryptor.SetKeyWithIV(key, key.size(), iv);

        std::string cipher;
        FileSource(inputFile.c_str(), true, new StreamTransformationFilter(encryptor, new StringSink(cipher)));

        std::ofstream out(outputFile, std::ios::binary);
        out.write((const char*)iv.BytePtr(), iv.size());
        out.write(cipher.data(), cipher.size());
        out.close();

        return "DES Encryption Success!\nKey: " + keyFile + "\nFile: " + outputFile;
    } catch (const std::exception& e) {
        return "DES Error: " + std::string(e.what());
    }
}

std::string CryptoService::decryptDES(const std::string& inputFile, const std::string& keyFile, const std::string& outputFile) {
    try {
        SecByteBlock key = LoadKey(keyFile);
        std::ifstream in(inputFile, std::ios::binary);
        SecByteBlock iv(DES::BLOCKSIZE);
        in.read((char*)iv.BytePtr(), iv.size());

        std::string cipher((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        std::string recovered;

        CBC_Mode<DES>::Decryption decryptor;
        decryptor.SetKeyWithIV(key, key.size(), iv);
        StringSource(cipher, true, new StreamTransformationFilter(decryptor, new StringSink(recovered)));

        std::ofstream out(outputFile, std::ios::binary);
        out.write(recovered.data(), recovered.size());

        return "DES Decryption Success!\nFile: " + outputFile;
    } catch (const std::exception& e) {
        return "DES Decryption Error: " + std::string(e.what());
    }
}

// RSA Implementations
std::string CryptoService::generateRSAKeys(const std::string& pubKeyFile, const std::string& privKeyFile) {
    try {
        AutoSeededRandomPool prng;
        RSA::PrivateKey priv;
        priv.GenerateRandomWithKeySize(prng, 2048);
        RSA::PublicKey pub(priv);

        {
            FileSink fs(pubKeyFile.c_str());
            pub.DEREncode(fs);
        }
        {
            FileSink fs(privKeyFile.c_str());
            priv.DEREncodePrivateKey(fs);
        }
        return "RSA Keys Generated!\nPublic: " + pubKeyFile + "\nPrivate: " + privKeyFile;
    } catch (const std::exception& e) {
        return "RSA KeyGen Error: " + std::string(e.what());
    }
}

std::string CryptoService::encryptRSA(const std::string& inputFile, const std::string& pubKeyFile, const std::string& outputFile) {
    try {
        RSA::PublicKey pub;
        {
            FileSource fs(pubKeyFile.c_str(), true);
            pub.BERDecode(fs);
        }
        AutoSeededRandomPool prng;
        RSAES_OAEP_SHA_Encryptor encryptor(pub);
        std::string cipher;
        FileSource(inputFile.c_str(), true, new PK_EncryptorFilter(prng, encryptor, new StringSink(cipher)));

        std::ofstream out(outputFile, std::ios::binary);
        out.write(cipher.data(), cipher.size());
        return "RSA Encryption Success!\nFile: " + outputFile;
    } catch (const std::exception& e) {
        return "RSA Encrypt Error: " + std::string(e.what());
    }
}

std::string CryptoService::decryptRSA(const std::string& inputFile, const std::string& privKeyFile, const std::string& outputFile) {
    try {
        RSA::PrivateKey priv;
        {
            FileSource fs(privKeyFile.c_str(), true);
            priv.BERDecodePrivateKey(fs, false, 0);
        }
        AutoSeededRandomPool prng;
        RSAES_OAEP_SHA_Decryptor decryptor(priv);
        std::string recovered;
        FileSource(inputFile.c_str(), true, new PK_DecryptorFilter(prng, decryptor, new StringSink(recovered)));

        std::ofstream out(outputFile, std::ios::binary);
        out.write(recovered.data(), recovered.size());
        return "RSA Decryption Success!\nFile: " + outputFile;
    } catch (const std::exception& e) {
        return "RSA Decrypt Error: " + std::string(e.what());
    }
}
