#include <iostream>
#include <fstream>
#include <string>

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/sha.h>

using namespace CryptoPP;
using namespace std;

// --- Hàm hỗ trợ lưu/đọc khóa RSA ---
void SaveRsaPublicKey(const string &filename, const RSA::PublicKey &key)
{
    FileSink file(filename.c_str());
    key.DEREncode(file);
    file.MessageEnd();
}

void SaveRsaPrivateKey(const string &filename, const RSA::PrivateKey &key)
{
    FileSink file(filename.c_str());
    key.DEREncodePrivateKey(file);
    file.MessageEnd();
}

void LoadRsaPrivateKey(const string &filename, RSA::PrivateKey &key)
{
    FileSource file(filename.c_str(), true);
    key.BERDecodePrivateKey(file, false, 0);
}

// --- Logic RSA ---
void RunRsaEncrypt()
{
    string inFile = "input.txt", outFile = "rsa_encrypted.bin";
    string pubKeyFile = "rsa_public.der", privKeyFile = "rsa_private.der";

    AutoSeededRandomPool prng;
    cout << "[*] Generating RSA Key (2048-bit)..." << endl;
    RSA::PrivateKey rsaPrivate;
    rsaPrivate.GenerateRandomWithKeySize(prng, 2048);
    RSA::PublicKey rsaPublic(rsaPrivate);

    SaveRsaPrivateKey(privKeyFile, rsaPrivate);
    SaveRsaPublicKey(pubKeyFile, rsaPublic);

    RSAES_OAEP_SHA_Encryptor encryptor(rsaPublic);
    string cipher;
    FileSource(inFile.c_str(), true, new PK_EncryptorFilter(prng, encryptor, new StringSink(cipher)));

    ofstream out(outFile, ios::binary);
    out.write(cipher.data(), cipher.size());
    out.close();

    cout << "[+] Encrypted RSA successfully!" << endl;
    cout << " -> Public Key: " << pubKeyFile << "\n -> Private Key: " << privKeyFile << "\n -> File: " << outFile << endl;
}

void RunRsaDecrypt()
{
    string inFile = "rsa_encrypted.bin", outFile = "rsa_decrypted.txt", privKeyFile = "rsa_private.der";

    AutoSeededRandomPool prng;
    RSA::PrivateKey rsaPrivate;
    LoadRsaPrivateKey(privKeyFile, rsaPrivate);

    RSAES_OAEP_SHA_Decryptor decryptor(rsaPrivate);
    string recovered;
    FileSource(inFile.c_str(), true, new PK_DecryptorFilter(prng, decryptor, new StringSink(recovered)));

    ofstream out(outFile, ios::binary);
    out.write(recovered.data(), recovered.size());
    out.close();

    cout << "[+] Decrypted RSA successfully!\n -> Result at: " << outFile << endl;
}

int main()
{
    ofstream testFile("input.txt", ios::app);
    testFile << "Data need to encrypt by using RSA." << endl;
    testFile.close();

    int choice;
    cout << "=== RSA_TOOL-2048 ===" << endl;
    cout << "1. Encrypt" << endl;
    cout << "2. Decrypt" << endl;
    cout << "Choose (1-2): ";
    cin >> choice;

    try
    {
        if (choice == 1)
            RunRsaEncrypt();
        else if (choice == 2)
            RunRsaDecrypt();
        else
            cout << "The chosen unavailable!" << endl;
    }
    catch (const exception &e)
    {
        cerr << "[!] Error RSA: " << e.what() << endl;
    }
    return 0;
}   