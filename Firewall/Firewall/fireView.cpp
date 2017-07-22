// fireView.cpp : implementation of the CFireView class
//

#include "stdafx.h"
#include "fire.h"
#include "fireDoc.h"
#include "fireView.h"
#include "Sockutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFireView

IMPLEMENT_DYNCREATE(CFireView, CFormView)

BEGIN_MESSAGE_MAP(CFireView, CFormView)
	ON_BN_CLICKED(IDC_ADDRULE, OnAddrule)
	ON_BN_CLICKED(IDC_BLOCKUDP, OnBlockUDP)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_BLOCKPING, OnBlockping)
	ON_BN_CLICKED(IDC_BLOCKALL, OnBlockall)
	ON_BN_CLICKED(IDC_ALLOWALL, OnAllowall)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_VIEWRULES, OnViewrules)
	ON_WM_SHOWWINDOW()
	ON_UPDATE_COMMAND_UI(ID_Start, OnUpdateStart)
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_ALLOWALL, OnUpdateAllowall)
	ON_UPDATE_COMMAND_UI(ID_BLOCKALL, OnUpdateBlockall)
	ON_COMMAND(ID_Start, OnStart)
	ON_COMMAND(ID_BLOCKALL, OnBlockall)
	ON_COMMAND(ID_ALLOWALL, OnAllowall)
	ON_COMMAND(ID_BLOCKPING, OnBlockping)
	ON_UPDATE_COMMAND_UI(ID_BLOCKPING, OnUpdateBlockping)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireView construction/destruction
//初始化界面状态
CFireView::CFireView()
	: CFormView(CFireView::IDD)
{
	//********************************************************
	m_pBrush = new CBrush;
	ASSERT(m_pBrush);
	m_clrBk = RGB(0x00,0x66,0x99);
	m_clrText = RGB(0xff,0xff,0x00);
	m_pBrush->CreateSolidBrush(m_clrBk);
	m_pColumns = new CStringList;
	ASSERT(m_pColumns);
	_rows = 1;
	start = TRUE;
	block = TRUE;
	allow = TRUE;
	ping = TRUE ;
}

CFireView::~CFireView()
{
	if (m_pBrush)
		delete m_pBrush;
}

void CFireView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_cResult);
	DDX_Control(pDX, IDC_VIEWRULES, m_cvrules);
	DDX_Control(pDX, IDC_BLOCKPING, m_cping);
	DDX_Control(pDX, IDC_BLOCKALL, m_cblockall);
	DDX_Control(pDX, IDC_START, m_cstart);
}

BOOL CFireView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//*****************************************************************
	
	m_filterDriver.LoadDriver("IpFilterDriver", "System32\\Drivers\\IpFltDrv.sys", NULL, TRUE);

	//we don't deregister the driver at destructor
	m_filterDriver.SetRemovable(FALSE);

	//we load the Filter-Hook Driver
	m_ipFltDrv.LoadDriver("DrvFltIp", NULL, NULL, TRUE);
	//****************************************************************
	return CFormView::PreCreateWindow(cs);
}

void CFireView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_parent = (CMainFrame*)GetParent();
	ShowHeaders();
}

/////////////////////////////////////////////////////////////////////////////
// CFireView diagnostics

#ifdef _DEBUG
void CFireView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFireView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFireView message handlers

void CFireView::OnAddrule() 
{
	// TODO: Add your control notification handler code here
	m_Addrule.DoModal (); 
}


void CFireView::OnStart() 
{
	CString		_text;
	m_cstart.GetWindowText(_text);
	
	//Start响应事件
	if(_text != "Stop" )
	{
		if(m_ipFltDrv.WriteIo(START_IP_HOOK, NULL, 0) != DRV_ERROR_IO)
		{
			MessageBox("Firewall Started Sucessfully");
			start = FALSE;
			m_cstart.SetWindowText("Stop");
			m_parent ->SetOnlineLed(TRUE);
			m_parent ->SetOfflineLed(FALSE);
		}
	}

	//Stop响应事件
	else
	{
		if(m_ipFltDrv.WriteIo(STOP_IP_HOOK, NULL, 0) != DRV_ERROR_IO)
		{
			MessageBox("Firewall Stopped Succesfully");
			m_cstart.SetWindowText("Start");
			start = TRUE;
			m_parent ->SetOnlineLed(FALSE);
			m_parent ->SetOfflineLed(TRUE);
		}
	}	
}

//禁用所有的ICMP包
void CFireView::OnBlockping() 
{
	// MessageBox("此功能需要你来实现！");
	// Your code
	IPFilter	 IPflt;

	IPflt.protocol        = 1;//1 for ICMP Protocol
	IPflt.destinationIp   = 0;//inet_addr("127.0.0.1");	// all destinations
	IPflt.destinationMask = 0;//inet_addr("255.255.255.255");// all destination masks
	IPflt.destinationPort = 0;//0 for all ports
	IPflt.sourceIp		  = 0;//0 means drop all packets irrespective of source
	IPflt.sourceMask	  = 0;
	IPflt.sourcePort	  = 0;//0 means from any source port
	IPflt.drop			  = TRUE;

	m_Addrule.AddFilter(IPflt);
	ping = FALSE;
	allow = TRUE;
	block = TRUE;
}

//禁用所有包
void CFireView::OnBlockall() 
{
	// MessageBox("此功能需要你来实现！");
	// Your code
	//	CAddRuleDlg     arule;
	IPFilter	 IPflt;

	IPflt.protocol        =	0;//0 for all the protocols
	IPflt.destinationIp   =	0;//0 for all destinations
	IPflt.destinationMask = 0;//0 for all destination masks
	IPflt.destinationPort = 0;//0 for all ports
	IPflt.sourceIp        =	0;//0 means drop all packets irrespective of source
	IPflt.sourceMask      =	0;
	IPflt.sourcePort      =	0;//0 means from any source port
	IPflt.drop            =	TRUE;

	m_Addrule.AddFilter(IPflt);
    
	block = FALSE;
	ping = FALSE;
	allow = TRUE;
}

//启用所有包
void CFireView::OnAllowall() 
{
	//MessageBox("此功能需要你来实现！");
	// Your code
	if(m_ipFltDrv.WriteIo(CLEAR_FILTER,NULL,0) != DRV_ERROR_IO)
	{
		allow = FALSE;
		block = TRUE;
		ping  = TRUE;
		m_cResult.DeleteAllItems();//clear the console
		_rows = 1;
	}
}

void CFireView::OnBlockUDP()
{
	IPFilter	 IPflt;

	IPflt.protocol        = 17;//1 for UDP Protocol
	IPflt.destinationIp   = 0;//inet_addr("127.0.0.1");	// all destinations
	IPflt.destinationMask = 0;//inet_addr("255.255.255.255");// all destination masks
	IPflt.destinationPort = 0;//0 for all ports
	IPflt.sourceIp		  = 0;//0 means drop all packets irrespective of source
	IPflt.sourceMask	  = 0;
	IPflt.sourcePort	  = 0;//0 means from any source port
	IPflt.drop			  = TRUE;

	m_Addrule.AddFilter(IPflt);
	ping = TRUE;
	allow = TRUE;
	block = TRUE;
}

//使用指定过滤规则
BOOL CFireView::ImplementRule(void)
{
	m_cResult.DeleteAllItems();
	// MessageBox("此功能需要你来实现！");
	// Your code	
	HANDLE	_hFile;
	DWORD	error,nbytesRead;
	char	data;
	CString	_buff = "";

	_hFile = CreateFile("saved.rul",				       // name of the file
						GENERIC_READ | GENERIC_WRITE,      // open as readable and writeable
						FILE_SHARE_READ | FILE_SHARE_WRITE,// shareaable as read only
						NULL,						
						OPEN_EXISTING,			           // open only if it exist
						NULL,
						NULL);

	//if the function fails to open the file check it
	if(_hFile == INVALID_HANDLE_VALUE)
	{
		error = GetLastError();
		MessageBox("Unable to open the file");
		return FALSE;
	}
	// if the file succeeds then interpret read the file 
	// and interprets the rule and assign them to the firewall
	else
	{  
		BOOL bResult;
		do{
			/* Read a single byte from the file as we had to look out for the 
			   endline.Though this is a bit time consuming one but it saves a lot
			   of headache that had been caused if we had used more than one byte
			*/
			bResult = ReadFile(_hFile,&data,1,&nbytesRead,NULL);

			if((data != '\n'))//end of line
			{
				_buff = _buff + data;
			}
			else if((bResult && nbytesRead) != 0)//if reading the file has no mistakes
			{
				_buff.Remove('\n');
				ParseToIp(_buff);//show the rule in the console
				_buff = "";
			}
			
		}while((bResult && nbytesRead) !=0);
		CloseHandle(_hFile);
	}
	// Close the file if all goes well	

	return TRUE;
}


//将字符串解析为filter特定格式

void CFireView:: ParseToIp(CString str)
{
	// Your code, please pay attention to the form of IP address and port!
	CString	_str[8];
	int	count = 0;
	int	_pos,_prevpos = 0;
	for(;count < 8;count++)
	{
		if(count < 7)//count the value
		{
			_pos = str.Find(',',_prevpos + 1);
			if((count > 0))
			{
				_str[count] = str.Left(_pos);//,A,B -> ,A
				_str[count].Delete(0,_prevpos + 1);//,A -> A
			}
			else
			{
				if(count == 0)//A, no need to delete
					_str[count] = str.Left(_pos);
			}
		}
		else//the last entry
		{
			_str[count] = str.Right(1);//1 owing to the rule
		}
		_prevpos  = _pos;
	}

	if(_rows == 1)
	{	
	}
	AddItem(0,0,(LPCTSTR)_str[0]);
	AddItem(0,1,(LPCTSTR)_str[1]);
	AddItem(0,2,(LPCTSTR)_str[2]);
	AddItem(0,3,(LPCTSTR)_str[3]);
	AddItem(0,4,(LPCTSTR)_str[4]);
	AddItem(0,5,(LPCTSTR)_str[5]);

	int	_proto = atoi((LPCTSTR)_str[6]);
	CString	proto;
	if(_proto == 0)
		proto = "ANY";
	if(_proto == 1)
		proto = "ICMP";
	if(_proto == 6)
		proto = "TCP";
	if(_proto == 17)
		proto = "UDP";
	AddItem(0,6,((LPCTSTR)proto));

	int _drop = atoi((LPCTSTR)_str[7]);
	if(_drop == 0)
		AddItem(0,7,"ALLOW");
	if(_drop == 1)
		AddItem(0,7,"DENY");

	_rows = _rows + 1;
	
	IPFilter ip1;
	ip1.destinationIp   = inet_addr((LPCTSTR)_str[0]);
	ip1.destinationMask = inet_addr((LPCTSTR)_str[1]);
	ip1.sourceIp		= inet_addr((LPCTSTR)_str[3]);
	ip1.sourceMask		= inet_addr((LPCTSTR)_str[4]);
	ip1.sourcePort		= htons(atoi((LPCTSTR)_str[5]));
	ip1.protocol		= atoi((LPCTSTR)_str[6]);

	int	drop;
	drop = atoi((LPCTSTR)_str[7]);
	if(drop == 0)
	{
		ip1.drop =	FALSE;
	
	}
	if(drop	== 1)
	{
		ip1.drop = TRUE;
	}
	
	m_Addrule.AddFilter(ip1);
}


//增加过滤规则表列
BOOL CFireView::AddColumn(LPCTSTR strItem,int nItem,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = m_cResult.GetStringWidth(lvc.pszText) + 25;
	if(nMask & LVCF_SUBITEM)
	{
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	return m_cResult.InsertColumn(nItem,&lvc);
}

//增加过滤规则表一个元素
BOOL CFireView::AddItem(int nItem,int nSubItem,LPCTSTR strItem ,int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;

	if(nImageIndex != -1)
	{
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage |= LVIF_IMAGE;
	}
	if(nSubItem == 0)
		return m_cResult.InsertItem(&lvItem);

	return m_cResult.SetItem(&lvItem);
}

void CFireView::AddHeader(LPTSTR hdr)
{
	if (m_pColumns)
		m_pColumns->AddTail(hdr);
}

void CFireView::ShowHeaders()
{
	int nIndex = 0;
	POSITION pos = m_pColumns->GetHeadPosition();
	while (pos)
	{
		CString hdr = (CString)m_pColumns->GetNext(pos);
		AddColumn(hdr,nIndex++);
	}
}

void CFireView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	AddHeader(_T("Dest IP"));
	AddHeader(_T("Dest MASK"));
	AddHeader(_T("Dest PORT"));
	AddHeader(_T("Source IP"));
	AddHeader(_T("Source MASK"));
	AddHeader(_T("Source PORT"));
	AddHeader(_T("PROTOCOL"));
	AddHeader(_T("ACTION"));
}

void CFireView::OnStop() 
{
	OnStart();	
}

void CFireView::OnUpdateStart(CCmdUI* pCmdUI) 
{	
	// TODO: Add your command update UI handler code here
	pCmdUI ->Enable(start);	
}

void CFireView::OnUpdateStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI ->Enable(!start);
}

void CFireView::OnUpdateAllowall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI ->Enable(allow);
}

void CFireView::OnUpdateBlockall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI ->Enable(block);
}

void CFireView::OnUpdateBlockping(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI ->Enable(ping);	
}

BOOL CFireView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
//***********************************************************************

HBRUSH CFireView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	switch(nCtlColor)
	{
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		pDC->SetBkColor(m_clrBk);
		pDC->SetTextColor(m_clrText);
	case CTLCOLOR_DLG:
		return static_cast<HBRUSH>(m_pBrush->GetSafeHandle());
	}
	return CFormView::OnCtlColor(pDC,pWnd,nCtlColor);
}

void CFireView::OnViewrules() 
{
	ImplementRule();	
}
