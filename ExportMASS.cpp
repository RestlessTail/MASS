#include "ExportMASS.h"
#include <filesystem>
#include "Symbol.h"
#include <filesystem>
#include <locale>

#pragma warning(disable:4996)

extern ActionTableType ActionTable;
//std::list<MASSAction::Action*>::iterator curAction;

extern SceneTableType SceneTable;
extern ActorTableType ActorTable;
extern SoundTableType SoundTable;

extern std::filesystem::path execPath;
extern std::filesystem::path scriptPath;

extern bool loadAssetsNow;
 
ExportMASS::ExportMASS(const char* src, const char* dest)
{
    errCode.clear();
    destPath = dest;
    destScenePath = destPath / "scene";
    destScriptPath = destPath / "script";
    destCharacterPath = destPath / "character";

    std::filesystem::create_directory(destPath, errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::create_directory(destScenePath, errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::create_directory(destScriptPath, errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::create_directory(destCharacterPath, errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    setlocale(LC_ALL, "chs");

    FILE* fp = fopen(src, "r");
    std::filesystem::path p(src);
    scriptPath = p.parent_path();
    std::filesystem::current_path(scriptPath);
    loadAssetsNow = false;
    Parse(fp);
    fclose(fp);

    projName = p.filename();
    size_t suffix = projName.rfind(L'.');
    projName = projName.substr(0, suffix);
}

void ExportMASS::Export()
{
    ExportDef();
    for (auto i : ActionTable) {
        i->Export(scriptContent);
    }

    FILE* fp = fopen((destScriptPath / "main.dat").string().c_str(), "w+");
    fprintf(fp, "%ws", scriptContent.c_str());
    fclose(fp);

    ExportEssential();
}

void ExportMASS::ExportDef()
{
    std::filesystem::current_path(scriptPath);
    for (auto &i : SceneTable) {
        std::filesystem::path path(i.second.path);

        std::wstring fileName = path.filename();

        std::filesystem::copy(path, destPath / "scene", errCode);
        if (errCode) {
            printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
            errCode.clear();
        }

        scriptContent += (
            L"DefineScene " + i.first + L" Whose\n" +
            L"\tTexture \"..\\\\scene\\\\" + fileName + L"\"\n" +
            L"EndDefine\n"
            );
    }

    for (auto &i : ActorTable) {
        std::filesystem::path path(i.second.TexturePath);

        std::wstring fileName = path.filename();

        std::filesystem::copy(path, destPath / "character", errCode);
        if (errCode) {
            printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
            errCode.clear();
        }

        scriptContent += (
            L"DefineCharacter " + i.first + L" Whose\n" +
            L"\tName \"" + i.second.Name + L"\"\n" +
            L"\tTexture \"..\\\\character\\\\" + fileName + L"\"\n" +
            L"EndDefine\n"
            );
    }

    /*for (auto i : ActorTable) {
        ScriptContent += (
            L"DefineActor " + i.first + L" Whose\n" +
            L"\tName " + i.second.Name + L"\n" +
            L"\tTexture " + i.second.TexturePath + L"\n" +
            L"EndDefine"
            );
    }*/
}

void ExportMASS::ExportEssential()
{
    std::filesystem::current_path(execPath);

    std::filesystem::copy("bin\\MASS-Export-Main.exe", destPath / (projName + L".exe"), errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::copy("assets", destPath / "assets", std::filesystem::copy_options::recursive, errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::copy("font", destPath / "font", std::filesystem::copy_options::recursive, errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::copy("shader", destPath / "shader", std::filesystem::copy_options::recursive, errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::copy("irrKlang.dll", destPath / "irrKlang.dll", errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::copy("ikpMP3.dll", destPath / "ikpMP3.dll", errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }

    std::filesystem::copy("license", destPath / "license", errCode);
    if (errCode) {
        printf("Err %d: %s\n", errCode.value(), errCode.message().c_str());
        errCode.clear();
    }
}
