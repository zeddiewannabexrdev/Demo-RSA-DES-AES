#ifndef IMGUIAPP_H
#define IMGUIAPP_H

#include <string>
#include <vector>

class ImGuiApp {
public:
    static void renderUI();

private:
    static void renderAesTab();
    static void renderDesTab();
    static void renderRsaTab();
    static void showLog();

    // UI state variables
    struct State {
        char aesIn[256] = "input.txt";
        char aesKey[256] = "aes_key.bin";
        char aesOut[256] = "aes_encrypted.bin";

        char desIn[256] = "input.txt";
        char desKey[256] = "des_key.bin";
        char desOut[256] = "des_encrypted.bin";

        char rsaIn[256] = "input.txt";
        char rsaPub[256] = "rsa_public.der";
        char rsaPriv[256] = "rsa_private.der";
        char rsaOut[256] = "rsa_encrypted.bin";

        std::vector<std::string> logs;
    };

    static State& getState();
    static void log(const std::string& msg);
};

#endif // IMGUIAPP_H
