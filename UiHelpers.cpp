#include "stdafx.h"
#include "UiHelpers.h"
#include "UiStrings.h"

namespace {

HBITMAP MatToHBITMAP(const cv::Mat& src)
{
    if (src.empty())
        return nullptr;
    cv::Mat bgr;
    if (src.channels() == 1)
        cv::cvtColor(src, bgr, cv::COLOR_GRAY2BGR);
    else if (src.channels() == 4)
        cv::cvtColor(src, bgr, cv::COLOR_BGRA2BGR);
    else
        bgr = src;

    BITMAPINFOHEADER bi = {};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bgr.cols;
    bi.biHeight = -bgr.rows;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;

    std::vector<BYTE> buffer(static_cast<size_t>(bgr.cols * bgr.rows * 3));
    for (int y = 0; y < bgr.rows; ++y)
    {
        const BYTE* row = bgr.ptr<BYTE>(y);
        memcpy(&buffer[static_cast<size_t>(y) * bgr.cols * 3], row, static_cast<size_t>(bgr.cols * 3));
    }

    HDC hdc = GetDC(nullptr);
    void* bits = nullptr;
    HBITMAP hbmp = CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS, &bits, nullptr, 0);
    ReleaseDC(nullptr, hdc);
    if (hbmp && bits)
        memcpy(bits, buffer.data(), buffer.size());
    return hbmp;
}

} // namespace

namespace uihelpers {

void AppendLog(CWnd* dlg, int editId, const CString& line)
{
    if (!dlg)
        return;
    CString existing;
    dlg->GetDlgItemText(editId, existing);
    if (!existing.IsEmpty())
        existing += _T("\r\n");
    existing += line;
    dlg->SetDlgItemText(editId, existing);
    CWnd* edit = dlg->GetDlgItem(editId);
    if (edit)
    {
        edit->SendMessage(EM_SETSEL, existing.GetLength(), existing.GetLength());
        edit->SendMessage(EM_SCROLLCARET);
    }
}

void ShowMatOnStatic(CWnd* dlg, int staticId, const cv::Mat& image)
{
    if (!dlg || image.empty())
        return;
    CStatic* pic = dynamic_cast<CStatic*>(dlg->GetDlgItem(staticId));
    if (!pic)
        return;
    CRect rect;
    pic->GetClientRect(&rect);
    if (rect.Width() <= 0 || rect.Height() <= 0)
        return;
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(rect.Width(), rect.Height()));
    HBITMAP hbmp = MatToHBITMAP(dst);
    if (!hbmp)
        return;
    HBITMAP old = pic->SetBitmap(hbmp);
    if (old)
        DeleteObject(old);
}

bool PickMediaFile(CWnd* parent, bool video, std::wstring& pathOut)
{
    CString filter = video ? LoadUiStr(IDS_FILTER_VIDEO) : LoadUiStr(IDS_FILTER_IMAGE);
    CFileDialog dlg(TRUE, nullptr, nullptr,
        OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, parent);
    if (dlg.DoModal() != IDOK)
        return false;
    pathOut = dlg.GetPathName().GetString();
    return !pathOut.empty();
}

void InitTooltips(CWnd* dlg, CToolTipCtrl& tip, const std::vector<std::pair<int, CString>>& entries)
{
    if (!dlg)
        return;
    tip.Create(dlg, TTS_ALWAYSTIP);
    tip.SetMaxTipWidth(360);
    tip.Activate(TRUE);
    for (const auto& e : entries)
    {
        CWnd* w = dlg->GetDlgItem(e.first);
        if (w && w->GetSafeHwnd())
            tip.AddTool(w, e.second);
    }
    tip.SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, 15000);
}

CString FormatOpenCvPath(const std::wstring& path)
{
    return CString(path.c_str());
}

} // namespace uihelpers
