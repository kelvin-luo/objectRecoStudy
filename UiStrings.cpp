#include "stdafx.h"
#include "UiStrings.h"

CString LoadUiStr(UINT id)
{
    CString s;
    if (!s.LoadString(id))
        s.Format(_T("#%u"), id);
    return s;
}
