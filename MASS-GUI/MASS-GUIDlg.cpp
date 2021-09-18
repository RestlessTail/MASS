
// MASS-GUIDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MASS-GUI.h"
#include "MASS-GUIDlg.h"
#include "afxdialogex.h"
#include <filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CMASSGUIDlg 对话框



CMASSGUIDlg::CMASSGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MASSGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMASSGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMASSGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &CMASSGUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CMASSGUIDlg::OnBnClickedMfcbuttonSelect)
	ON_BN_CLICKED(IDC_BUTTON1, &CMASSGUIDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CMASSGUIDlg::OnBnClickedMfcbuttonSelExportPath)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CMASSGUIDlg::OnBnClickedMfcbuttonExport)
END_MESSAGE_MAP()


// CMASSGUIDlg 消息处理程序

BOOL CMASSGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMASSGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMASSGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMASSGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMASSGUIDlg::OnBnClickedMfcbuttonSelect()
{
	CString gReadFilePathName;
	CFileDialog fileDlg(true, _T("MASS"), _T("*.mass"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("MASS Files (*.mass)|*.mass|All File (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)    //弹出对话框  
	{
		CString RunFilename = fileDlg.GetPathName();//得到完整的文件名和目录名拓展名  
		GetDlgItem(IDC_EDIT1)->SetWindowText(RunFilename);//将路径显示  
	}
}

void CMASSGUIDlg::OnBnClickedButtonRun()
{
	TCHAR exePathCstr[256] = { 0 };
	GetModuleFileName(0, exePathCstr, 255);
	std::filesystem::path execPath(exePathCstr);
	execPath = execPath.parent_path();
	std::filesystem::current_path(execPath);

	CString filename;
	GetDlgItemText(IDC_EDIT1, filename);

	CString command = CString("MASS ") + CString(filename);
	CStringA str(command);
	char* cmd = (char*)calloc((size_t)(str.GetLength() + 1), sizeof(char));
	char* srcCmd = (char*)str.GetString();
	if (cmd) {
		strcpy(cmd, srcCmd);
	}

	typedef struct {
		char* cmd;
		CWnd* text;
		CWnd* btn;
	} ThreadFunAtg;

	ThreadFunAtg* args = new ThreadFunAtg;
	args->cmd = cmd;
	args->text = GetDlgItem(IDC_EDIT2);
	args->btn = GetDlgItem(IDC_BUTTON1);

	auto ThreadFun = [](LPVOID pParam) {
		ThreadFunAtg* arg = (ThreadFunAtg*)pParam;
		FILE* p = _popen(arg->cmd, "r");
		char buf[1025] = { 0 };

		size_t len;
		len = fread(buf, sizeof(char), 1024, p);
		CString output;
		CStringA cbuf = buf;
		output += cbuf;

		while (len >= 1024) {
			memset(buf, 0, 1025);
			len = fread(buf, sizeof(char), 1024, p);
			CString output;
			CString cbuf = output;
			output += cbuf;
		}

		output.Replace(L"\n", L"\r\n");
		arg->text->SetWindowText(output);
		arg->btn->EnableWindow(true);
		free(arg->cmd);
		delete arg;
		return (UINT)_pclose(p);
	};

	::AfxBeginThread(ThreadFun, args);  //这就是创建一个线程并执行了,调用上面的函数弹出一个对话框.
	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
}


void CMASSGUIDlg::OnBnClickedMfcbuttonSelExportPath()
{
	// 配置对话框  
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	//bi.pidlRoot = rootLoation; // 文件夹对话框之根目录，不指定的话则为我的电脑  
	//bi.lpszTitle = _T("对话框抬头"); // 可以不指定  
	// bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;  

	// 打开对话框, 有点像DoModal  
	LPITEMIDLIST targetLocation = SHBrowseForFolder(&bi);
	if (targetLocation != NULL) {
		TCHAR targetPath[MAX_PATH];
		SHGetPathFromIDList(targetLocation, targetPath);
		//MessageBox( targetPath );  
		GetDlgItem(IDC_EDIT3)->SetWindowText(targetPath);//将路径显示  
	}
}


void CMASSGUIDlg::OnBnClickedMfcbuttonExport()
{
	CString filename;
	CString exportpath;
	GetDlgItemText(IDC_EDIT1, filename);
	GetDlgItemText(IDC_EDIT3, exportpath);

	CString command = CString("MASS ") + CString(filename) + CString(" -o ") + CString(exportpath);
	CStringA str(command);
	char* cmd = (char*)calloc((size_t)(str.GetLength() + 1), sizeof(char));
	char* srcCmd = (char*)str.GetString();
	if (cmd) {
		strcpy(cmd, srcCmd);
	}

	typedef struct {
		char* cmd;
		CWnd* text;
		CWnd* btn;
	} ThreadFunAtg;

	ThreadFunAtg* args = new ThreadFunAtg;
	args->cmd = cmd;
	args->text = GetDlgItem(IDC_EDIT2);
	args->btn = GetDlgItem(IDC_BUTTON1);

	auto ThreadFun = [](LPVOID pParam) {
		ThreadFunAtg* arg = (ThreadFunAtg*)pParam;
		FILE* p = _popen(arg->cmd, "r");
		char buf[1025] = { 0 };

		size_t len;
		len = fread(buf, sizeof(char), 1024, p);
		CString output;
		CStringA cbuf = buf;
		output += cbuf;

		while (len >= 1024) {
			memset(buf, 0, 1025);
			len = fread(buf, sizeof(char), 1024, p);
			CString output;
			CString cbuf = output;
			output += cbuf;
		}

		output.Replace(L"\n", L"\r\n");
		arg->text->SetWindowText(output);
		arg->btn->EnableWindow(true);
		free(arg->cmd);
		delete arg;
		return (UINT)_pclose(p);
	};

	::AfxBeginThread(ThreadFun, args);  //这就是创建一个线程并执行了,调用上面的函数弹出一个对话框.

	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
}


BOOL CMASSGUIDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
