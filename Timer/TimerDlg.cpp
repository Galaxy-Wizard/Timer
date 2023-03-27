
// TimerDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Timer.h"
#include "TimerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CTimerDlg

const UINT_PTR EventNumber = 2000;
const UINT EventTimeOneSecond = 1000;
const COLORREF RedColor = RGB(255, 0, 0);
const COLORREF GrayColor = RGB(127, 127, 127);

CTimerDlg::CTimerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIMER_DIALOG, pParent), Stop(true), CurrentColor(GrayColor)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, InitialTime);
	DDX_Control(pDX, IDC_EDIT2, CurrentTime);
}

BEGIN_MESSAGE_MAP(CTimerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTimerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTimerDlg::OnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений CTimerDlg

BOOL CTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	SetTimer(EventNumber, EventTimeOneSecond, nullptr);
	StartingEventTime = CTimeSpan();
	EventTime = StartingEventTime;

	CSingleLock SingleLock(&StopCriticalSection);
	SingleLock.Lock();

	Stop = true;
	Signal = false;

	SingleLock.Unlock();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTimerDlg::OnBnClickedButton1()
{
	CString InitialTimeString;

	InitialTime.GetWindowTextW(InitialTimeString);

	int InitialTimeInSeconds = _wtoi(InitialTimeString.GetBuffer());

	StartingEventTime = CTimeSpan(InitialTimeInSeconds);

	EventTime = StartingEventTime;

	CSingleLock SingleLock(&StopCriticalSection);
	SingleLock.Lock();

	Stop = false;

	Signal = true;

	CurrentColor = GrayColor;

	SingleLock.Unlock();
}


void CTimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == EventNumber)
	{
		CSingleLock SingleLock(&StopCriticalSection);
		SingleLock.Lock();


		if (EventTime <= CTimeSpan(0, 0, 0, 0))
		{
			CurrentColor = RedColor;

			CurrentTime.SetTextColor(CurrentColor);

			CString CurrentTimeString(L"Время закончилось");

			CurrentTime.SetWindowTextW(CurrentTimeString, Big);

			Stop = true;

			if (Signal)
			{
				MessageBeep(0xFFFFFFFF);
			}
		}
		else
		{
			if (Stop != true)
			{
				CurrentColor = GrayColor;

				CurrentTime.SetTextColor(CurrentColor);

				EventTime -= CTimeSpan(EventTimeOneSecond / 1000);

				CString CurrentTimeString;

				CurrentTimeString.Format(CString(L"Дни: %d Часы: %d Минуты: %d Секунды: %d"), EventTime.GetDays(), EventTime.GetHours(), EventTime.GetMinutes(), EventTime.GetSeconds());

				CurrentTime.SetWindowTextW(CurrentTimeString, Little);
			}
		}

		SingleLock.Unlock();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTimerDlg::OnClickedButton2()
{
	Signal = false;
}
