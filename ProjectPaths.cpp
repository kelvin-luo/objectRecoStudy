#include "stdafx.h"
#include "ProjectPaths.h"

#include <fstream>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

namespace {

std::wstring ParentDir(const std::wstring& path)
{
    wchar_t buf[MAX_PATH] = {};
    wcsncpy_s(buf, path.c_str(), _TRUNCATE);
    PathRemoveFileSpecW(buf);
    return std::wstring(buf);
}

bool DirExists(const std::wstring& path)
{
    DWORD attr = GetFileAttributesW(path.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileExists(const std::wstring& path)
{
    DWORD attr = GetFileAttributesW(path.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

std::wstring ExeDir()
{
    wchar_t buf[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    return ParentDir(buf);
}

std::wstring FindProjectRootFrom(const std::wstring& start)
{
    std::wstring dir = start;
    for (int i = 0; i < 8; ++i)
    {
        const std::wstring models = dir + L"\\models";
        const std::wstring cmake = dir + L"\\code\\CMakeLists.txt";
        if (DirExists(models) && FileExists(cmake))
            return dir;
        std::wstring parent = ParentDir(dir);
        if (parent == dir)
            break;
        dir = parent;
    }
    // Fallback: parent of runtime dir (msvc_release -> project root).
    return ParentDir(start);
}

} // namespace

namespace projectpaths {

std::wstring RuntimeDir()
{
    return ExeDir();
}

std::wstring ProjectRoot()
{
    static std::wstring root = FindProjectRootFrom(ExeDir());
    return root;
}

std::wstring ModelsDir()
{
    return ProjectRoot() + L"\\models";
}

std::wstring InputDir()
{
    return RuntimeDir() + L"\\input";
}

std::wstring OutputDir()
{
    return ProjectRoot() + L"\\output";
}

std::wstring ResolveFromRoot(const std::wstring& relative)
{
    return ProjectRoot() + L"\\" + relative;
}

std::wstring ResolveModelFile(const std::wstring& relative)
{
    const std::wstring inRuntime = InputDir() + L"\\models\\" + relative;
    if (FileExists(inRuntime))
        return inRuntime;
    const std::wstring inRoot = ModelsDir() + L"\\" + relative;
    if (FileExists(inRoot))
        return inRoot;
    return inRoot;
}

std::wstring DefaultImagePath()
{
    const std::wstring p = InputDir() + L"\\people.jpeg";
    if (FileExists(p))
        return p;
    return p;
}

std::wstring DefaultVideoPath()
{
    const std::wstring p = InputDir() + L"\\demo.mp4";
    if (FileExists(p))
        return p;
    return p;
}

std::string ToUtf8(const std::wstring& ws)
{
    if (ws.empty())
        return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string out(static_cast<size_t>(len > 0 ? len - 1 : 0), '\0');
    if (len > 0)
        WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, &out[0], len, nullptr, nullptr);
    return out;
}

std::wstring FromUtf8(const std::string& s)
{
    if (s.empty())
        return {};
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring out(static_cast<size_t>(len > 0 ? len - 1 : 0), L'\0');
    if (len > 0)
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &out[0], len);
    return out;
}

} // namespace projectpaths
