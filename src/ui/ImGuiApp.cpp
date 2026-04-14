#include "ImGuiApp.h"
#include "imgui.h"
#include "../core/CryptoService.h"
#include <iostream>

// We'll use a simple static state for the demo
ImGuiApp::State& ImGuiApp::getState() {
    static State state;
    return state;
}

void ImGuiApp::log(const std::string& msg) {
    getState().logs.push_back(msg);
    if (getState().logs.size() > 100) {
        getState().logs.erase(getState().logs.begin());
    }
}

void ImGuiApp::renderUI() {
    auto& state = getState();

    ImGui::Begin("Cryptography Lab Suite (ImGui)");

    if (ImGui::BeginTabBar("Algorithms")) {
        if (ImGui::BeginTabItem("AES-256")) {
            renderAesTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("DES-64")) {
            renderDesTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("RSA-2048")) {
            renderRsaTab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();
    showLog();

    ImGui::End();
}

void ImGuiApp::renderAesTab() {
    auto& state = getState();
    ImGui::Text("Symmetric Encryption (AES)");
    
    ImGui::InputText("Input File", state.aesIn, sizeof(state.aesIn));
    ImGui::InputText("Key File", state.aesKey, sizeof(state.aesKey));
    ImGui::InputText("Output File", state.aesOut, sizeof(state.aesOut));

    if (ImGui::Button("Encrypt (AES)")) {
        log("AES Encrypting: " + std::string(state.aesIn));
        std::string res = CryptoService::encryptAES(state.aesIn, state.aesKey, state.aesOut);
        log(res);
    }
    ImGui::SameLine();
    if (ImGui::Button("Decrypt (AES)")) {
        log("AES Decrypting: " + std::string(state.aesOut));
        std::string res = CryptoService::decryptAES(state.aesOut, state.aesKey, "aes_decrypted_imgui.txt");
        log(res);
    }
}

void ImGuiApp::renderDesTab() {
    auto& state = getState();
    ImGui::Text("Symmetric Encryption (DES)");

    ImGui::InputText("Input File", state.desIn, sizeof(state.desIn));
    ImGui::InputText("Key File", state.desKey, sizeof(state.desKey));
    ImGui::InputText("Output File", state.desOut, sizeof(state.desOut));

    if (ImGui::Button("Encrypt (DES)")) {
        log("DES Encrypting: " + std::string(state.desIn));
        std::string res = CryptoService::encryptDES(state.desIn, state.desKey, state.desOut);
        log(res);
    }
    ImGui::SameLine();
    if (ImGui::Button("Decrypt (DES)")) {
        log("DES Decrypting: " + std::string(state.desOut));
        std::string res = CryptoService::decryptDES(state.desOut, state.desKey, "des_decrypted_imgui.txt");
        log(res);
    }
}

void ImGuiApp::renderRsaTab() {
    auto& state = getState();
    ImGui::Text("Asymmetric Encryption (RSA)");

    ImGui::InputText("Input File", state.rsaIn, sizeof(state.rsaIn));
    ImGui::InputText("Public Key", state.rsaPub, sizeof(state.rsaPub));
    ImGui::InputText("Private Key", state.rsaPriv, sizeof(state.rsaPriv));
    ImGui::InputText("Output File", state.rsaOut, sizeof(state.rsaOut));

    if (ImGui::Button("Generate RSA Keys")) {
        log("Generating RSA Keys...");
        std::string res = CryptoService::generateRSAKeys(state.rsaPub, state.rsaPriv);
        log(res);
    }
    ImGui::SameLine();
    if (ImGui::Button("Encrypt (RSA)")) {
        log("RSA Encrypting...");
        std::string res = CryptoService::encryptRSA(state.rsaIn, state.rsaPub, state.rsaOut);
        log(res);
    }
    ImGui::SameLine();
    if (ImGui::Button("Decrypt (RSA)")) {
        log("RSA Decrypting...");
        std::string res = CryptoService::decryptRSA(state.rsaOut, state.rsaPriv, "rsa_decrypted_imgui.txt");
        log(res);
    }
}

void ImGuiApp::showLog() {
    auto& state = getState();
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Terminal Log:");
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& l : state.logs) {
        ImGui::TextUnformatted(l.c_str());
    }
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();
}
