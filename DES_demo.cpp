#include <iostream>
#include <fstream>
#include <string>

#include <cryptopp/des.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;
using namespace std;

// --- Hàm hỗ trợ lưu/đọc khóa ---
void SaveKey(const string &filename, const SecByteBlock &key)
{
    StringSource ss(key.data(), key.size(), true, new FileSink(filename.c_str()));
}

SecByteBlock LoadKey(const string &filename)
{
    string keyStr;
    FileSource fs(filename.c_str(), true, new StringSink(keyStr));
    return SecByteBlock((const CryptoPP::byte*)keyStr.data(), keyStr.size());
}

// --- Logic DES ---
void RunDesEncrypt()
{
    string inFile = "input.txt", outFile = "des_encrypted.bin", keyFile = "des_key.bin";

    AutoSeededRandomPool prng;
    SecByteBlock key(DES::DEFAULT_KEYLENGTH); // 8 bytes (64 bits)
    prng.GenerateBlock(key, key.size());
    SaveKey(keyFile, key);

    SecByteBlock iv(DES::BLOCKSIZE); // 8 bytes
    prng.GenerateBlock(iv, iv.size());

    CBC_Mode<DES>::Encryption encryptor;
    encryptor.SetKeyWithIV(key, key.size(), iv);

    string cipher;
    FileSource(inFile.c_str(), true, new StreamTransformationFilter(encryptor, new StringSink(cipher)));

    ofstream out(outFile, ios::binary);
    out.write((const char *)iv.BytePtr(), iv.size());
    out.write(cipher.data(), cipher.size());
    out.close();

    cout << "[+] Da ma hoa DES thanh cong!\n -> Khoa: " << keyFile << "\n -> File bi mat: " << outFile << endl;
}

void RunDesDecrypt()
{
    string inFile = "des_encrypted.bin", outFile = "des_decrypted.txt", keyFile = "des_key.bin";
    SecByteBlock key = LoadKey(keyFile);

    ifstream in(inFile, ios::binary);
    SecByteBlock iv(DES::BLOCKSIZE);
    in.read((char *)iv.BytePtr(), iv.size());

    string cipher((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    string recovered;

    CBC_Mode<DES>::Decryption decryptor;
    decryptor.SetKeyWithIV(key, key.size(), iv);
    StringSource(cipher, true, new StreamTransformationFilter(decryptor, new StringSink(recovered)));

    ofstream out(outFile, ios::binary);
    out.write(recovered.data(), recovered.size());
    cout << "[+] Da giai ma DES thanh cong!\n -> Xem ket qua tai: " << outFile << endl;
}

int main()
{
    ofstream testFile("input.txt", ios::app);
    testFile << "Du lieu can ma hoa bang DES." << endl;
    testFile.close();

    int choice;
    cout << "=== CONG CU DES-64 ===" << endl;
    cout << "1. Ma hoa (Encrypt)" << endl;
    cout << "2. Giai ma (Decrypt)" << endl;
    cout << "Chon (1-2): ";
    cin >> choice;

    try
    {
        if (choice == 1)
            RunDesEncrypt();
        else if (choice == 2)
            RunDesDecrypt();
        else
            cout << "Lua chon khong hop le!" << endl;
    }
    catch (const exception &e)
    {
        cerr << "[!] Loi DES: " << e.what() << endl;
    }
    return 0;
}