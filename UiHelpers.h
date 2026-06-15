#pragma once

#include <vector>
#include <utility>
#include <opencv2/core.hpp>

class CobjectRecoStudyDlg;

namespace uihelpers {

inline CString Utf8Str(const char* utf8)
{
    return CString(projectpaths::FromUtf8(utf8).c_str());
}

void AppendLog(CWnd* dlg, int editId, const CString& line);
void ShowMatOnStatic(CWnd* dlg, int staticId, const cv::Mat& image);
bool PickMediaFile(CWnd* parent, bool video, std::wstring& pathOut);
void InitTooltips(CWnd* dlg, CToolTipCtrl& tip, const std::vector<std::pair<int, CString>>& entries);
CString FormatOpenCvPath(const std::wstring& path);

} // namespace uihelpers
