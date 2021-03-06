#include "stdafx.h"

#include "UIClass.h"
#include <Psapi.h>
UIClass* UIClassC=0;
DEFINEFUNCTION DragWindowFun(LTExecState* es)
{
	UIClassC->DragWindow();
	return JsUndefined();
}

DEFINEFUNCTION QuitAppFun(LTExecState* es)
{
	UIClassC->QuitApp();
	return JsUndefined();
}
DEFINEFUNCTION MinWindowFun(LTExecState* es)
{
	UIClassC->MinWindow();
	return JsUndefined();
}


UIClass::UIClass()
{	
	HMODULE hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = 700,wh=500;
	OleInitialize(0);


	ltskinview= new LTFrameView(this,L"LTFrame-OLEDragDrop",NULL,WS_POPUP | WS_VISIBLE,0,0,ww,wh,0);
	UIClassC =this;
	ltskinview->BindUserFunction("DragWindow",DragWindowFun,0);
	ltskinview->BindUserFunction("QuitApp",QuitAppFun,0);
	ltskinview->BindUserFunction("MinWindow",MinWindowFun,0);

	SetWindowText(ltskinview->windowHandle(),L"LTFrame-OLEDragDrop");	


	wstring path = wstring(applicationpath)+L"./template/Drag2.html";
	ltskinview->loadFile(path.c_str());
	ltskinview->EnableDragFrameChangeSize(false);
	ltskinview->MessageLoop();
}


UIClass::~UIClass(void)
{
	OleUninitialize();
	free(applicationpath);
	delete ltskinview;
}
void UIClass::DragWindow()
{
	ReleaseCapture();
	POINT DownPoint;
	GetCursorPos(&DownPoint);
	SendMessage(ltskinview->windowHandle(),WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(DownPoint.x,DownPoint.y));
}
void UIClass::QuitApp()
{
	ltskinview->CloseWindow();
}
void UIClass::MinWindow()
{
	::ShowWindow(ltskinview->windowHandle(),SW_MINIMIZE);	
}
LRESULT UIClass::HandleUserMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);
}
LPCWSTR UIClass::GetWindowClassName()
{
	return L"OLEDragDrop";
}

bool UIClass::OnDragLeave()
{
	ltskinview->RunJavaScript(L"$(\"#div1\").text('动作：DragLeave')");
	return true;
}
bool UIClass::OnDragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	ltskinview->RunJavaScript(L"$(\"#div1\").text('动作：DragEnter')");
	return true;
}

bool UIClass:: OnDragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	ltskinview->RunJavaScript(L"$(\"#div1\").text('动作：DragOver')");
	return true;
}
bool UIClass:: OnDrop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	ltskinview->RunJavaScript(L"$(\"#div1\").text('动作：Drop')");
	return true;
}