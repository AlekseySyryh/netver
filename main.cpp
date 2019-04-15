#include <iostream>
#include <windows.h>

HKEY GetBaseKey(){
    HKEY hKey;
    LONG lRes = RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Microsoft\\NET Framework Setup\\NDP",
            0,
            KEY_READ,
            &hKey);
    if (lRes == ERROR_FILE_NOT_FOUND){
        std::cout << ".NET Not found" << std::endl;
        hKey == nullptr;
    }
    return hKey;
}
void describe(HKEY &hChildKey) {
    long unsigned int len = 100;
    char ver[len];
    auto lr = RegQueryValueExA(hChildKey,
                               "Version",
                               nullptr,
                               nullptr,
                               (LPBYTE) ver,
                               &len);
    if (lr == ERROR_SUCCESS) {
        std::cout << std::string(ver);
        DWORD data, size = sizeof(DWORD);
        lr = RegQueryValueExA(hChildKey,
                              "SP",
                              nullptr,
                              nullptr,
                              (LPBYTE) &data,
                              &size);
        if (lr == ERROR_SUCCESS) {
            std::cout << " SP" << data;
        }

    }
}
void check(HKEY &hKey, const std::string &name) {
    HKEY hChildKey;

    LONG lRes = RegOpenKeyExA(
            hKey,
            name.c_str(),
            0,
            KEY_READ,
            &hChildKey);
    if (lRes == ERROR_SUCCESS){
    std::cout << name << " (";
    describe(hChildKey);
    std::cout << ")" << std::endl;
    RegCloseKey(hChildKey);
    }
}


void checkPre45(HKEY &hKey) {
    LONG lRes = 0;
    size_t i = 0;
    while (lRes == ERROR_SUCCESS) {
        long unsigned int len = 100;
        char str[len];

        lRes = RegEnumKeyExA(
                hKey,
                i,
                str,
                &len,
                nullptr,
                nullptr,
                nullptr,
                nullptr);
        if (lRes == ERROR_SUCCESS) {
            ++i;
            std::string name(str);
            if (name[0] == 'v' && name != "v4") {
                if (name == "v4.0") {
                    check(hKey, name + "\\Client");
                    check(hKey, name + "\\Full");
                } else {
                    check(hKey, name);
                }
            }
        }
    }
}

void checkProfile(HKEY &hKey, std::string profile) {
    std::string subName = "v4\\" + profile;
    HKEY hChildKey;
    LONG lRes = RegOpenKeyExA(
            hKey,
            subName.c_str(),
            0,
            KEY_READ,
            &hChildKey);
    if (lRes == EXIT_SUCCESS) {
        DWORD releaseKey, size = sizeof(DWORD);
        lRes = RegQueryValueExA(hChildKey,
                                "Release",
                                nullptr,
                                nullptr,
                                (LPBYTE) &releaseKey,
                                &size);
        if (lRes == EXIT_SUCCESS) {
            std::cout << "v";
            if (releaseKey > 461814)
                std::cout << "4.7.2 or later";
            else if (releaseKey >= 461808)
                std::cout << "4.7.2";
            else if (releaseKey >= 461308)
                std::cout << "4.7.1";
            else if (releaseKey >= 460798)
                std::cout << "4.7";
            else if (releaseKey >= 394802)
                std::cout << "4.6.2";
            else if (releaseKey >= 394254)
                std::cout << "4.6.1";
            else if (releaseKey >= 393295)
                std::cout << "4.6";
            else if (releaseKey >= 379893)
                std::cout << "4.5.2";
            else if (releaseKey >= 378675)
                std::cout << "4.5.1";
            else if (releaseKey >= 378389)
                std::cout << "4.5";
            else
                std::cout << "Unknown";
            std::cout << "\\" << profile << std::endl;
        }
        RegCloseKey(hChildKey);
    }
}

void check45(HKEY &hKey) {
    checkProfile(hKey, "Client");
    checkProfile(hKey, "Full");
}

int main() {
    HKEY hKey = GetBaseKey();
    if (hKey == nullptr){
        return EXIT_SUCCESS;
    }
    checkPre45(hKey);
    check45(hKey);
    RegCloseKey(hKey);
    return EXIT_SUCCESS;
}

