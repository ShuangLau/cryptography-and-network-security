// AddRuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fire.h"
#include "AddRuleDlg.h"
//********************************************************
#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddRuleDlg dialog


CAddRuleDlg::CAddRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddRuleDlg::IDD, pParent)
{
	m_sdadd = _T("");
	m_sdport = _T("");
	m_ssadd = _T("");
	m_ssport = _T("");
	ipFltDrv.LoadDriver("DrvFltIp", NULL, NULL, TRUE);	
}


void CAddRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddRuleDlg)
	DDX_Control(pDX, IDC_COMBO2, m_protocol);
	DDX_Control(pDX, IDC_COMBO1, m_action);
	DDX_Text(pDX, IDC_DADD, m_sdadd);
	DDV_MaxChars(pDX, m_sdadd, 15);
	DDX_Text(pDX, IDC_DPORT, m_sdport);
	DDX_Text(pDX, IDC_SADD, m_ssadd);
	DDV_MaxChars(pDX, m_ssadd, 15);
	DDX_Text(pDX, IDC_SPORT, m_ssport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddRuleDlg, CDialog)
	//{{AFX_MSG_MAP(CAddRuleDlg)
	//ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_EN_KILLFOCUS(IDC_SADD, OnKillfocusSadd)
	ON_EN_KILLFOCUS(IDC_DADD, OnKillfocusDadd)
	ON_BN_CLICKED(IDC_ADDSAVE, OnAddsave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

//**************************************************************************
//添加至IPFilter，发送指令至驱动
DWORD CAddRuleDlg::AddFilter(IPFilter pf)
{
 
	DWORD result = ipFltDrv.WriteIo(ADD_FILTER, &pf, sizeof(pf));
	if (result != DRV_SUCCESS) 
	{
		AfxMessageBox("Unable to add rule to the driver");
		return FALSE;
	}

	else
		return TRUE;
}

//*************************************************************************
//检测输入的IP地址是否非法

BOOL CAddRuleDlg::Verify(CString str)
{
	// MessageBox("此功能需要你来实现！");
	// Your code
	int pos = 0, prevpos = -1;		// Keeps track of current and previous
									// positins in the string
	CString    str1;
	
	// if string doesn't contains any . it means it is invalid IP
	if(str.Find('.') == -1)
		return FALSE;

	// if the input string contains any invalid entry like any alpahabets
	// or it may contains some symbols then report it as an error
	if(str.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
		return FALSE;
	if(str.FindOneOf("!@#$%^&*()_+|-;:'\"/?><,") != -1)
		return FALSE;

	// if string contains . but not at the right position this will 
	// return false for that
	int _pos = 0;
	_pos = str.Find('.');//the first one, start from the 0 digit
	if( (0 > _pos) || (_pos > 3))
		return FALSE;
	int newpos = _pos;
	_pos = str.Find('.',_pos+1);//the first one, start form the pos+1 digit
	if((newpos+1 >= _pos)||( _pos > newpos + 4))
		return FALSE;
	newpos = _pos;
	_pos = str.Find('.',_pos+1);
	if( (newpos+1 >= _pos) || (_pos > newpos + 4) )
		return FALSE;

	newpos = _pos;
	_pos = str.Find('.',_pos+1);
	if(_pos != -1)
		return FALSE;
	
	//if a dot is found verify that the ip address is within valid
	// range 0.0.0.0  & 255.255.255.255
	//str1 = str;
	//CString   str2;
	for(int cnt = 0 ;cnt <= 3;cnt++)
	{
		if(cnt < 3)//count the number of the value
			pos = str1.Find('.',pos+1);
		else 
			pos = str.GetLength();

		str1 = str.Left(pos);//get the string including the value
		str1 = str1.Right(pos-(prevpos+1));//get the value without a dot
		unsigned int a = atoi(LPCTSTR(str1));//Convert the string to the number
		if((0 > a)||(a > 255))//check whether the value is valid
		{
			return FALSE;
		}
		prevpos = pos;
		
		/*int length = str1.GetLength();
		if(cnt < 3)
			pos = str1.Find('.',pos);
		else
			pos = 5;
		str2 = str1.Left(pos);
		str1 = str1.Right(length-pos);
		unsigned int a = atoi(LPCTSTR(str2));//Convert the string to the number
		if((0 > a)||(a > 255))//check whether the value is valid
		{
			return FALSE;
		}
		pos = 0;*/
	}
	return TRUE;
}

//*****************************************************************

void CAddRuleDlg::OnKillfocusSadd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	BOOL bresult = Verify(m_ssadd);
	if(bresult == FALSE)
		MessageBox("Invalid IP Address");	
}

void CAddRuleDlg::OnKillfocusDadd() 
{
	// TODO: Add your control notification handler code here
	// This will check wether the IP address you had given
	// corresponds to a valid IP address or not. If not it
	// will prompt you for a valid IP address.

	UpdateData();
    BOOL bresult = Verify(m_sdadd);
	if(bresult == FALSE)
		MessageBox("Invalid IP Address");
	
}

//将新增的规则写入文件
void CAddRuleDlg::OnAddsave() 
{
	// Your code
	// TODO: Add your control notification handler code here
	UpdateData();
	BOOL setact;
	int	setproto;
	int action = m_action.GetCurSel();
	char ch[30];
	if(action == 0)
		setact = FALSE; 
	else
		setact = TRUE;

	int proto = m_protocol.GetCurSel();
	if(proto == 0)//ICMP
		setproto = 1;
	if(proto == 1)//IP
		setproto = 0;
	if(proto == 2)//TCP
		setproto = 6;
	if(proto == 3)//UDP
		setproto = 17;

	if((action == 0) && (proto == 0))
		wsprintf(ch,"Action: Allow, Protocol ICMP");
	if((action == 0) && (proto == 1))
		wsprintf(ch,"Action: Allow, Protocol IP");
	if((action == 0) && (proto == 2))
		wsprintf(ch,"Action: Allow, Protocol TCP");
	if((action == 0) && (proto == 3))
		wsprintf(ch,"Action: Allow, Protocol UDP");
	if((action == 1) && (proto == 0))
		wsprintf(ch,"Action: Deny, Protocol ICMP");
	if((action == 1) && (proto == 1))
		wsprintf(ch,"Action: Deny, Protocol IP");
	if((action == 1) && (proto == 2))
		wsprintf(ch,"Action: Deny, Protocol TCP");
	if((action == 1) && (proto == 3))
		wsprintf(ch,"Action: Deny, Protocol UDP");
	//wsprintf(ch,"Action: %d, Protocol %d",action,proto);
	MessageBox(ch);

	IPFilter   ip;
	ip.destinationIp = inet_addr((LPCTSTR)m_sdadd);
	ip.destinationMask = inet_addr("255.255.255.255");
	ip.destinationPort = htons(atoi((LPCTSTR)m_sdport));
	ip.sourceIp = inet_addr((LPCTSTR)m_ssadd);
	ip.sourceMask = inet_addr("255.255.255.255");
	ip.sourcePort = htons(atoi((LPCTSTR)m_ssport));
	ip.protocol = setproto;
	ip.drop = setact;

	CString _str;
	char   ch1[100];
	wsprintf(ch1,"%s,%s,%s,%s,%s,%s,%d,%d\n",
			 m_sdadd,
			 "255.255.255.255",
			 m_sdport,
			 m_ssadd,
			 "255.255.255.255",
			 m_ssport,
			 setproto,
			 setact);

	if( NewFile()== FALSE)
		MessageBox("unable to create file");				//create a new file

	GotoEnd();
	 

	SaveFile(ch1);

	if(CloseFile() == FALSE)
		MessageBox("Unalbe to close the file");

	DWORD result = AddFilter(ip);
}
BOOL CAddRuleDlg::NewFile(void)
{
/* This will open an existing file or open a new file if the file
   doesnot exists
*/
	_hFile = CreateFile("saved.rul",
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_ALWAYS,
						NULL,
						NULL);

	/* If unable to obtain the handle values*/
	if(_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	return TRUE;		// File has been opened succesfully
}
//******************************************************
// This will move the file pointer to the end of the file so that 
// it can be easily added to the file

DWORD CAddRuleDlg::GotoEnd(void)
{
	DWORD val;

	DWORD size = GetFileSize(_hFile,NULL);
	if(size == 0)
		return size;
	val = SetFilePointer(_hFile,0,NULL,FILE_END);

	/* If unable to set the file pointer to the end of the file */
 	if(val == 0)
	{
		MessageBox("Unable to set file pointer");
		return GetLastError();
	}
	return val;
}

/* This code will save the data into the file which is given by the parameter*/

DWORD CAddRuleDlg::SaveFile(char*  str)
{
	DWORD   bytesWritten;
	/* Try to write the string passed as parameter to the file and if any 
		error occurs return the appropriate values
	*/
	DWORD	_len =  strlen(str);
	if(WriteFile(_hFile, str, _len, &bytesWritten, NULL) == 0)
	{
		MessageBox("Unalbe to write to the file\n");
		return FALSE;
	}
	return TRUE;
}

/* This function will close the existing file */
BOOL CAddRuleDlg::CloseFile()
{
	if(!_hFile)
	{
	//	MessageBox("File handle does not exist");
		return FALSE;
	}

// if there is an appropriate handle then close it and return app values
    else{
		if(CloseHandle(_hFile) != 0)
		{
			return TRUE;
		}
		else 
			return FALSE;
	}
}