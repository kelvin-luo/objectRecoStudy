#pragma once

#include <string>

namespace projectpaths {

// Repository root (contains code/, models/, msvc_release/).
std::wstring ProjectRoot();

// Directory containing objectRecoStudy.exe (msvc_release/).
std::wstring RuntimeDir();

// models/ under project root.
std::wstring ModelsDir();

// msvc_release/input/
std::wstring InputDir();

// output/ under project root.
std::wstring OutputDir();

// Resolve a path relative to project root.
std::wstring ResolveFromRoot(const std::wstring& relative);

// Resolve model file: prefer runtime input/models, then project models/.
std::wstring ResolveModelFile(const std::wstring& relative);

// Default demo image (input/people.jpeg).
std::wstring DefaultImagePath();

// Default demo video (input/demo.mp4).
std::wstring DefaultVideoPath();

// UTF-8 std::string from wstring (for OpenCV imread etc.).
std::string ToUtf8(const std::wstring& ws);

// wstring from UTF-8.
std::wstring FromUtf8(const std::string& s);

} // namespace projectpaths
