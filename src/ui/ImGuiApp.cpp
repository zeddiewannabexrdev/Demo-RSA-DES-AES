#include "ImGuiApp.h"
#include "imgui.h"
#include "../core/CryptoService.h"
#include "portable-file-dialogs.h" // Added for real file dialogs
#include <iostream>

// Simple state management
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
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Cryptography Lab Suite", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

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
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "Symmetric Encryption (AES)");
    
    ImGui::InputText("Input File", state.aesIn, sizeof(state.aesIn));
    ImGui::SameLine();
    if (ImGui::Button("Browse##aesIn")) {
        auto selection = pfd::open_file("Select Input File", ".").result();
        if (!selection.empty()) strncpy(state.aesIn, selection[0].c_str(), sizeof(state.aesIn));
    }

    ImGui::InputText("Key File", state.aesKey, sizeof(state.aesKey));
    ImGui::SameLine();
    if (ImGui::Button("Browse##aesKey")) {
        auto selection = pfd::open_file("Select Key File", ".").result();
        if (!selection.empty()) strncpy(state.aesKey, selection[0].c_str(), sizeof(state.aesKey));
    }

    ImGui::InputText("Output File", state.aesOut, sizeof(state.aesOut));
    ImGui::SameLine();
    if (ImGui::Button("Browse##aesOut")) {
        auto selection = pfd::save_file("Select Output File", ".").result();
        if (!selection.empty()) strncpy(state.aesOut, selection.c_str(), sizeof(state.aesOut));
    }

    ImGui::Spacing();
    if (ImGui::Button("Encrypt (AES)", ImVec2(120, 30))) {
        log("AES Encrypting: " + std::string(state.aesIn));
        std::string res = CryptoService::encryptAES(state.aesIn, state.aesKey, state.aesOut);
        log(res);
    }
    ImGui::SameLine();
    if (ImGui::Button("Decrypt (AES)", ImVec2(120, 30))) {
        log("AES Decrypting: " + std::string(state.aesOut));
        std::string res = CryptoService::decryptAES(state.aesOut, state.aesKey, "aes_decrypted.txt");
        log(res);
    }
}

void ImGuiApp::renderDesTab() {
    auto& state = getState();
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "Symmetric Encryption (DES)");

    ImGui::InputText("Input File", state.desIn, sizeof(state.desIn));
    ImGui::InputText("Key File", state.desKey, sizeof(state.desKey));
    ImGui::InputText("Output File", state.desOut, sizeof(state.desOut));
    
    // We can add Browse buttons here as well, but keeping it simple for now
    if (ImGui::Button("Encrypt (DES)")) {
        log("DES Encrypting...");
        std::string res = CryptoService::encryptDES(state.desIn, state.desKey, state.desOut);
        log(res);
    }
    ImGui::SameLine();
    if (ImGui::Button("Decrypt (DES)")) {
        log("DES Decrypting...");
        std::string res = CryptoService::decryptDES(state.desOut, state.desKey, "des_decrypted.txt");
        log(res);
    }
}

void ImGuiApp::renderRsaTab() {
    auto& state = getState();
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "Asymmetric Encryption (RSA)");

    ImGui::InputText("Input File", state.rsaIn, sizeof(state.rsaIn));
    ImGui::InputText("Pub Key", state.rsaPub, sizeof(state.rsaPub));
    ImGui::InputText("Priv Key", state.rsaPriv, sizeof(state.rsaPriv));
    ImGui::InputText("Output File", state.rsaOut, sizeof(state.rsaOut));

    if (ImGui::Button("Gen RSA Keys")) {
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
        std::string res = CryptoService::decryptRSA(state.rsaOut, state.rsaPriv, "rsa_decrypted.txt");
        log(res);
    }
}

void ImGuiApp::showLog() {
    auto& state = getState();
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "System Logs:");
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& l : state.logs) {
        ImGui::TextUnformatted(l.c_str());
    }
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();
}
