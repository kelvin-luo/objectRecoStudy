// objectRecoStudyDlg.cpp

#include "stdafx.h"
#include "objectRecoStudy.h"
#include "objectRecoStudyDlg.h"
#include "afxdialogex.h"
#include "UiStrings.h"

#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {

std::wstring OutputFile(const std::wstring& name)
{
    return projectpaths::OutputDir() + L"\\" + name;
}

} // namespace

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    CString ver;
    ver.Format(LoadUiStr(IDS_STR_VERSION_FMT), OBJECTRECOSTUDY_VERSION);
    SetDlgItemText(IDC_STATIC_ABOUT_VERSION, ver);
    SetDlgItemText(IDC_STATIC_ABOUT_COPY, LoadUiStr(IDS_STR_COPYRIGHT));
    SetDlgItemText(IDC_STATIC_ABOUT_CONTACT, LoadUiStr(IDS_STR_CONTACT));
    return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CobjectRecoStudyDlg::CobjectRecoStudyDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_OBJECTRECOSTUDY_DIALOG, pParent)
    , id_camera(0)
{
    m_hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
}

void CobjectRecoStudyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CobjectRecoStudyDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CobjectRecoStudyDlg::OnBnClickedButton1_maskRcnn_image_locate)
    ON_BN_CLICKED(IDC_BUTTON2, &CobjectRecoStudyDlg::OnBnClickedButton2_locPts4)
    ON_BN_CLICKED(IDC_BUTTON3, &CobjectRecoStudyDlg::OnBnClickedButton3_objectLocateCamera)
    ON_BN_CLICKED(IDC_BUTTON4, &CobjectRecoStudyDlg::OnBnClickedButton4_localObjMobileCamera)
    ON_BN_CLICKED(IDC_BUTTON5, &CobjectRecoStudyDlg::OnBnClickedButton5_objectMaskRcnnVideo)
    ON_BN_CLICKED(IDC_BUTTON6, &CobjectRecoStudyDlg::OnBnClickedButton6_rectangleFineMobile)
    ON_BN_CLICKED(IDC_BUTTON7, &CobjectRecoStudyDlg::OnBnClickedButton7_grabcutStudy)
    ON_BN_CLICKED(IDC_BUTTON8, &CobjectRecoStudyDlg::OnBnClickedButton8_setIdCamera)
    ON_BN_CLICKED(IDC_BUTTON9, &CobjectRecoStudyDlg::OnBnClickedButton9_caffeModel)
    ON_BN_CLICKED(IDC_BUTTON10, &CobjectRecoStudyDlg::OnBnClickedButton10_tensorflowModel)
    ON_BN_CLICKED(IDC_BUTTON11, &CobjectRecoStudyDlg::OnBnClickedButton11TorchStudy)
    ON_BN_CLICKED(IDC_BUTTON12, &CobjectRecoStudyDlg::OnBnClickedButton12ClassicalStudy)
    ON_BN_CLICKED(IDC_BUTTON13, &CobjectRecoStudyDlg::OnBnClickedButton13SSD_Study)
    ON_BN_CLICKED(IDC_BUTTON14, &CobjectRecoStudyDlg::OnBnClickedButton14_oneKeyRunAll)
END_MESSAGE_MAP()

BOOL CobjectRecoStudyDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        if (strAboutMenu.LoadString(IDS_ABOUTBOX) && !strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    SetDlgItemText(IDC_EDIT1, _T("0"));
    logLine(LoadUiStr(IDS_STR_LOG_STARTED));

    std::vector<std::pair<int, CString>> tips = {
        {IDC_BUTTON1, LoadUiStr(IDS_TIP_BTN1)},
        {IDC_BUTTON2, LoadUiStr(IDS_TIP_BTN2)},
        {IDC_BUTTON3, LoadUiStr(IDS_TIP_BTN3)},
        {IDC_BUTTON4, LoadUiStr(IDS_TIP_BTN4)},
        {IDC_BUTTON5, LoadUiStr(IDS_TIP_BTN5)},
        {IDC_BUTTON6, LoadUiStr(IDS_TIP_BTN6)},
        {IDC_BUTTON7, LoadUiStr(IDS_TIP_BTN7)},
        {IDC_BUTTON8, LoadUiStr(IDS_TIP_BTN8)},
        {IDC_BUTTON9, LoadUiStr(IDS_TIP_BTN9)},
        {IDC_BUTTON10, LoadUiStr(IDS_TIP_BTN10)},
        {IDC_BUTTON11, LoadUiStr(IDS_TIP_BTN11)},
        {IDC_BUTTON12, LoadUiStr(IDS_TIP_BTN12)},
        {IDC_BUTTON13, LoadUiStr(IDS_TIP_BTN13)},
        {IDC_BUTTON14, LoadUiStr(IDS_TIP_BTN14)},
        {IDC_EDIT1, LoadUiStr(IDS_TIP_EDIT1)},
        {IDC_EDIT2, LoadUiStr(IDS_TIP_EDIT2)},
    };
    uihelpers::InitTooltips(this, m_toolTip, tips);
    return TRUE;
}

BOOL CobjectRecoStudyDlg::PreTranslateMessage(MSG* pMsg)
{
    if (m_toolTip.GetSafeHwnd())
        m_toolTip.RelayEvent(pMsg);
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CobjectRecoStudyDlg::logLine(const CString& line)
{
    uihelpers::AppendLog(this, IDC_EDIT2, line);
}

void CobjectRecoStudyDlg::showImageFile(const std::wstring& path)
{
    Mat image = imread(projectpaths::ToUtf8(path), cv::IMREAD_COLOR);
    if (image.empty())
    {
        logLine(LoadUiStr(IDS_ERR_IMAGE_READ) + path.c_str());
        return;
    }
    uihelpers::ShowMatOnStatic(this, IDC_PIC_STATIC, image);
    logLine(LoadUiStr(IDS_LOG_IMAGE_LOADED) + path.c_str());
}

void CobjectRecoStudyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CobjectRecoStudyDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CobjectRecoStudyDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CobjectRecoStudyDlg::fetchFileName(std::string& nameOut, bool video)
{
    std::wstring path;
    if (!uihelpers::PickMediaFile(this, video, path))
        return;
    nameOut = projectpaths::ToUtf8(path);
}

int locat4pt2fFromScene_maskRcnn_image_locate(Mat inputImage, Point2f* ptf4)
{
    (void)ptf4;
    if (!inputImage.data)
        return -1;
    int res = recoImageSingle_maskRcnn_image_locate(inputImage);
    cout << "res = " << res << endl;
    return 0;
}

void CobjectRecoStudyDlg::OnBnClickedButton1_maskRcnn_image_locate()
{
    std::wstring path = projectpaths::DefaultImagePath();
    std::wstring picked;
    if (uihelpers::PickMediaFile(this, false, picked))
        path = picked;

    logLine(LoadUiStr(IDS_LOG_MASK_IMAGE) + path.c_str());
    Mat inputImage = imread(projectpaths::ToUtf8(path), cv::IMREAD_COLOR);
    if (inputImage.empty())
    {
        logLine(LoadUiStr(IDS_ERR_IMAGE_LOAD));
        return;
    }

    std::wstring outXml = OutputFile(L"inputImage.xml");
    FileStorage fs(projectpaths::ToUtf8(outXml), FileStorage::WRITE);
    fs << "inputImage" << inputImage;
    fs.release();

    clock_t t0 = clock();
    Point2f ptf4[4];
    locat4pt2fFromScene_maskRcnn_image_locate(inputImage, ptf4);
    double sec = (double)(clock() - t0) / CLK_TCK;

    uihelpers::ShowMatOnStatic(this, IDC_PIC_STATIC, inputImage);
    CString msg;
    msg.Format(LoadUiStr(IDS_FMT_MASK_DONE), sec);
    logLine(msg);
}

void CobjectRecoStudyDlg::OnBnClickedButton2_locPts4()
{
    std::wstring path;
    if (!uihelpers::PickMediaFile(this, false, path))
        path = projectpaths::DefaultImagePath();
    showImageFile(path);
}

void videoProc_maskRcnn(int opt, Net net, int idx_camera, string nameOfVideo)
{
    VideoCapture vc;
    if (opt == 0)
        vc.open(idx_camera);
    else if (opt < 0)
        vc.open(nameOfVideo);
    else
        return;

    Mat frame;
    vc >> frame;
    int id = 0;
    while (true)
    {
        if (id % 36 == 0)
        {
            clock_t start = clock();
            procImage__maskRcnn(frame, net, id);
            clock_t end = clock();
            printf("frame %d time=%f\n", id, (double)(end - start) / CLK_TCK);
        }
        vc >> frame;
        id++;
        if (frame.data)
        {
            imshow("mask_rcnn_video", frame);
            waitKey(10);
        }
        if (waitKey(10) > 0)
            break;
    }
    vc.release();
    destroyWindow("mask_rcnn_video");
}

void CobjectRecoStudyDlg::OnBnClickedButton3_objectLocateCamera()
{
    logLine(LoadUiStr(IDS_LOG_MASK_CAM_INIT));
    Net net;
    if (net_init__maskRcnn(net) < 0)
    {
        logLine(LoadUiStr(IDS_ERR_MASK_LOAD));
        return;
    }
    CString msg;
    msg.Format(LoadUiStr(IDS_FMT_CAMERA), id_camera);
    logLine(msg);
    videoProc_maskRcnn(0, net, id_camera, "");
}

void CobjectRecoStudyDlg::OnBnClickedButton4_localObjMobileCamera()
{
    std::string videoName = projectpaths::ToUtf8(projectpaths::DefaultVideoPath());
    fetchFileName(videoName, true);
    if (videoName.empty())
        return;

    logLine(LoadUiStr(IDS_LOG_MOBILE_VIDEO) + projectpaths::FromUtf8(videoName).c_str());
    Net net;
    if (net_prepare(net) < 0)
    {
        logLine(LoadUiStr(IDS_ERR_SSD_LOAD));
        return;
    }

    VideoCapture vc(videoName);
    Mat frame;
    vc >> frame;
    namedWindow("mobilenet_video");
    while (true)
    {
        if (frame.data)
        {
            Mat res_detectionMat;
            imageProc(frame, net, res_detectionMat);
            imshow("mobilenet_video", frame);
        }
        if (waitKey(10) > 0)
            break;
        vc >> frame;
    }
    destroyWindow("mobilenet_video");
    vc.release();
    logLine(LoadUiStr(IDS_LOG_MOBILE_DONE));
}

void CobjectRecoStudyDlg::OnBnClickedButton5_objectMaskRcnnVideo()
{
    std::string videoName;
    fetchFileName(videoName, true);
    if (videoName.empty())
        videoName = projectpaths::ToUtf8(projectpaths::DefaultVideoPath());

    logLine(LoadUiStr(IDS_LOG_MASK_VIDEO) + projectpaths::FromUtf8(videoName).c_str());
    Net net;
    if (net_init__maskRcnn(net) < 0)
    {
        logLine(LoadUiStr(IDS_ERR_MASK_LOAD));
        return;
    }
    videoProc_maskRcnn(-1, net, 0, videoName);
    logLine(LoadUiStr(IDS_LOG_MASK_VIDEO_DONE));
}

void approxPolyDP_app_fit_4p(int thresh, Mat srcImage, vector<Point2f>& pts4, int epsilon, int numsRef)
{
    pts4.clear();
    Mat grayImage;
    if (srcImage.channels() > 1)
        cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    else
        grayImage = srcImage;

    RNG rng(12345);
    Mat threshold_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    threshold(grayImage, threshold_output, thresh, 255, THRESH_BINARY);
    findContours(threshold_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    if (contours.empty())
        return;

    vector<vector<Point>> contours_poly(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
        approxPolyDP(Mat(contours[i]), contours_poly[i], (double)epsilon, true);

    for (size_t i = 0; i < contours_poly[0].size(); i++)
        pts4.push_back(Point2f((float)contours_poly[0][i].x, (float)contours_poly[0][i].y));

    Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point());
    }
    std::wstring outPath = OutputFile(L"contour_" + std::to_wstring(numsRef) + L".jpg");
    imwrite(projectpaths::ToUtf8(outPath), drawing);
    if (pts4.size() == 4)
    {
        namedWindow("contour_preview", WINDOW_AUTOSIZE);
        imshow("contour_preview", drawing);
        waitKey(200);
    }
}

float calc_residure_4p(Mat gray_threshold, vector<Point2f> pts4_01)
{
    if (!gray_threshold.data || pts4_01.size() < 3)
        return -1.f;
    int numsNonZero = countNonZero(gray_threshold);
    vector<Point> pts_0;
    for (const auto& p : pts4_01)
        pts_0.push_back(Point((int)p.x, (int)p.y));
    vector<vector<Point>> points = { pts_0 };
    Mat dr(gray_threshold.size(), CV_8UC1, Scalar(0));
    drawContours(dr, points, 0, Scalar(255), -1);
    int numsFetch = countNonZero(dr);
    return (float)abs(numsFetch - numsNonZero) / (float)(gray_threshold.rows * gray_threshold.cols);
}

int rect_modifier(Rect& output, int stepLen)
{
    output.x += stepLen;
    output.y += stepLen;
    output.width -= stepLen * 2;
    output.height -= stepLen * 2;
    if (output.x < 0 || output.y < 0)
        return -2;
    if (output.width <= 0 || output.height <= 0)
        return -1;
    return 0;
}

int get_right_type_and_conf_and_pts(Mat& detectionMat, int id_type, float& conf, float* ft4, int& id_line)
{
    float confidenceThreshold = 0.10f;
    float val_conf = 0.f;
    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);
        if (confidence > confidenceThreshold)
        {
            size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));
            if (fabs((double)objectClass - id_type) < 1.0)
            {
                if (val_conf < confidence)
                {
                    val_conf = confidence;
                    ft4[0] = detectionMat.at<float>(i, 3);
                    ft4[1] = detectionMat.at<float>(i, 4);
                    ft4[2] = detectionMat.at<float>(i, 5);
                    ft4[3] = detectionMat.at<float>(i, 6);
                    id_line = i;
                    conf = val_conf;
                }
            }
        }
    }
    return 0;
}

void CobjectRecoStudyDlg::OnBnClickedButton6_rectangleFineMobile()
{
    std::string imagePath = projectpaths::ToUtf8(projectpaths::DefaultImagePath());
    fetchFileName(imagePath, false);
    if (imagePath.empty())
        return;

    logLine(LoadUiStr(IDS_LOG_RECT_FINE) + projectpaths::FromUtf8(imagePath).c_str());
    Net net;
    if (net_prepare(net) < 0)
        return;

    Mat src_store = imread(imagePath, cv::IMREAD_COLOR);
    if (!src_store.data)
    {
        logLine(LoadUiStr(IDS_ERR_IMAGE_LOAD));
        return;
    }

    Mat frame_tr = src_store.clone();
    Mat res_detectionMat;
    imageProc(frame_tr, net, res_detectionMat);
    uihelpers::ShowMatOnStatic(this, IDC_PIC_STATIC, frame_tr);

    if (!res_detectionMat.data)
        return;

    int id_type = 84;
    float ft4[4] = {};
    float conf = 0.f;
    int id_line = 0;
    get_right_type_and_conf_and_pts(res_detectionMat, id_type, conf, ft4, id_line);

    int left = (int)(ft4[0] * frame_tr.cols);
    int top = (int)(ft4[1] * frame_tr.rows);
    int right = (int)(ft4[2] * frame_tr.cols);
    int bottom = (int)(ft4[3] * frame_tr.rows);

    Mat frame_post = src_store.clone();
    for (int j = -30; j < 30; j += 4)
    {
        Rect grabcutRect(left, top, right - left, bottom - top);
        if (rect_modifier(grabcutRect, j) < 0)
            continue;

        Mat grabcutResult;
        Mat bgModel, fgModel;
        grabCut(frame_post, grabcutResult, grabcutRect, bgModel, fgModel, 3, GC_INIT_WITH_RECT);

        compare(grabcutResult, GC_PR_FGD, grabcutResult, CMP_EQ);
        Mat foreground(frame_post.size(), CV_8UC3, Scalar::all(255));
        frame_post.copyTo(foreground, grabcutResult);

        std::wstring fgPath = OutputFile(L"foreground.jpg");
        imwrite(projectpaths::ToUtf8(fgPath), foreground);

        Mat gray;
        cvtColor(foreground, gray, COLOR_BGR2GRAY);
        gray = 255 - gray;
        Mat gray_threshold;
        threshold(gray, gray_threshold, 5, 255, THRESH_BINARY);
        GaussianBlur(gray_threshold, gray_threshold, Size(3, 3), 3, 3);
        threshold(gray, gray_threshold, 128, 255, THRESH_BINARY);

        vector<Point2f> pts4_01;
        for (int k = 1; k < 1000; k += 100)
        {
            approxPolyDP_app_fit_4p(128, gray_threshold, pts4_01, k, k);
            if (pts4_01.size() == 4)
            {
                Mat src_show_4p = src_store.clone();
                for (int n = 0; n < 4; n++)
                {
                    circle(src_show_4p, pts4_01[n], 3 + 3 * n, Scalar(255, 0, 255), 2);
                    putText(src_show_4p, to_string(n), pts4_01[n], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
                }
                uihelpers::ShowMatOnStatic(this, IDC_PIC_STATIC, src_show_4p);
                logLine(LoadUiStr(IDS_LOG_RECT_OK));
                break;
            }
        }
        if (waitKey(1) > 0)
            break;
    }
}

Rect selection;
Mat img, img0;
Point prePt(-1, -1);

void onMouse(int event, int x, int y, int flags, void* param)
{
    (void)param;
    if (event == EVENT_LBUTTONDOWN)
        prePt = Point(x, y);
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
    {
        img = img0.clone();
        rectangle(img, Rect(prePt.x, prePt.y, abs(x - prePt.x), abs(y - prePt.y)), Scalar(0, 0, 255), 2);
        selection = Rect(prePt.x, prePt.y, abs(x - prePt.x), abs(y - prePt.y));
    }
    else if (event == EVENT_LBUTTONUP)
        prePt = Point(-1, -1);
    imshow("grabcut_source", img);
}

int main_studyGrabcut(string nameOfImage)
{
    img = imread(nameOfImage, cv::IMREAD_COLOR);
    if (!img.data)
        return -1;
    img0 = img.clone();
    namedWindow("grabcut_source", WINDOW_AUTOSIZE);
    imshow("grabcut_source", img);
    setMouseCallback("grabcut_source", onMouse, nullptr);

    Mat result, bgModel, fgModel;
    for (int i = 0; i < 15; i += 3)
    {
        while (true)
        {
            int c = waitKey(10);
            if ((char)c == 'p')
            {
                grabCut(img0, result, selection, bgModel, fgModel, i, GC_INIT_WITH_RECT);
                compare(result, GC_PR_FGD, result, CMP_EQ);
                Mat foreground(img.size(), CV_8UC3, Scalar::all(255));
                img0.copyTo(foreground, result);
                putText(foreground, to_string(i), Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 128), 2);
                imshow("grabcut_result", foreground);
            }
            if ((char)c == 'q' || ((char)c > 0 && (char)c != 'p'))
                break;
        }
    }
    destroyWindow("grabcut_source");
    destroyWindow("grabcut_result");
    return 0;
}

void CobjectRecoStudyDlg::OnBnClickedButton7_grabcutStudy()
{
    std::string nameOfImage = projectpaths::ToUtf8(projectpaths::DefaultImagePath());
    fetchFileName(nameOfImage, false);
    if (nameOfImage.empty())
        return;
    logLine(LoadUiStr(IDS_LOG_GRABCUT_HINT));
    main_studyGrabcut(nameOfImage);
    logLine(LoadUiStr(IDS_LOG_GRABCUT_DONE));
}

void CobjectRecoStudyDlg::OnBnClickedButton8_setIdCamera()
{
    CString str2;
    GetDlgItemText(IDC_EDIT1, str2);
    id_camera = _ttoi(str2);
    CString msg;
    msg.Format(LoadUiStr(IDS_FMT_CAMERA_ID), id_camera);
    logLine(msg);
}

void CobjectRecoStudyDlg::OnBnClickedButton9_caffeModel()
{
    CaffeModelDialog cmd;
    cmd.DoModal();
}

void CobjectRecoStudyDlg::OnBnClickedButton10_tensorflowModel()
{
    TensorflowModelStudy sd;
    sd.DoModal();
}

void CobjectRecoStudyDlg::OnBnClickedButton11TorchStudy()
{
    TorchModelStudy sd;
    sd.DoModal();
}

void CobjectRecoStudyDlg::OnBnClickedButton12ClassicalStudy()
{
    ClassicalStudy sd;
    sd.DoModal();
}

void CobjectRecoStudyDlg::OnBnClickedButton13SSD_Study()
{
    ssdModelStudy sd;
    sd.DoModal();
}

void CobjectRecoStudyDlg::OnBnClickedButton14_oneKeyRunAll()
{
    logLine(LoadUiStr(IDS_LOG_RUN_ALL_START));
    OnBnClickedButton1_maskRcnn_image_locate();
    OnBnClickedButton2_locPts4();
    logLine(LoadUiStr(IDS_LOG_RUN_ALL_DONE));
}
