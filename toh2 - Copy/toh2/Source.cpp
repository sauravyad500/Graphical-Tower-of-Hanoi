#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <process.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND hwnd,edit,click;
PAINTSTRUCT ps;
HDC hdc;
#define get_n 2
static wchar_t text[9];
int n=5,a,gx=700,gy=700,height;
BOOL in=FALSE;

RECT rc;
HFONT hfntOld;
LPPAINTSTRUCT lpPS;

void shift(int n, int barto);
void initialize();
void transfer(int n, int from, int to, int temp);
DWORD WINAPI MyThreadFunction(LPVOID lpParam);
struct toh {
	int x, y, width, bar;
}arr[9];
HANDLE h;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor= LoadCursor(NULL,IDC_ARROW);

	RegisterClass(&wc);

	// Create the window.

	hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);
	
	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = {};

	DWORD   dwThreadIdArray[3];
	h=CreateThread(
		NULL,                          // default security attributes
		0,                             // use default stack size  
		MyThreadFunction,              // thread function name
		0,                             // argument to thread function 
		CREATE_SUSPENDED,              // use default creation flags 
		&dwThreadIdArray[0]);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

HBRUSH b = CreateSolidBrush(RGB(255, 6, 45)),hold= CreateSolidBrush(RGB(255, 6, 45));
HBRUSH bar_b=CreateSolidBrush(RGB(200, 130, 20));
HBRUSH l= CreateSolidBrush(RGB(255,255,255));
HPEN p = CreatePen(PS_NULL, 0, RGB(255, 255, 255));
#define exit 103

RECT r;
HBRUSH z;
int i;

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	transfer(a, 1, 3, 2);
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		
	case WM_CREATE:
	{
		edit = CreateWindowW(TEXT("edit"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER,
			200, 10, 350, 30, hwnd, (HMENU)1, NULL, NULL);
		click = CreateWindow(TEXT("button"), TEXT("Proceed"), WS_VISIBLE | WS_CHILD,
			200, 40, 150, 50, hwnd, (HMENU)get_n, NULL, NULL);
		HMENU hMenuBar = CreateMenu();
		AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)101, L"New Game");
		AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)exit, L"Exit");
		SetMenu(hwnd, hMenuBar);


	} break;

	case WM_SIZE:
	{
		GetClientRect(hwnd, &r);
		gx = r.right - r.left;
		gy = r.bottom - r.top;
		InvalidateRect(hwnd,&r,FALSE);
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case get_n:
			{
				n = GetWindowTextLength(edit) + 1;
				GetWindowText(edit, (LPWSTR)text, n);
				a=n = _wtoi(text);

				in = TRUE;
				initialize();
				ResumeThread(h);
				return 0;
			} 
		
		case 101: 
		{
			n = gx = gy = height = 0;
			in =FALSE ;
			FillRect(hdc, &rc, (HBRUSH)COLOR_WINDOW+1);
			RECT r;
			GetClientRect(hwnd, &r);
			InvalidateRect(hwnd, &r, TRUE);
			return 0;
		}

		case exit:
			DestroyWindow(hwnd);
		}
		return 0;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		if (in)
		{
			SelectObject(hdc, p);

			FillRect(hdc, &r, b);

			SelectObject(hdc, bar_b);
			
			for (i = 1; i < 4; i++)
				Rectangle(hdc, i*gx / 4 - 5, 130, i*gx / 4 + 5, gy - 10);

			for (i = 0; i < n; i++) {

				if (i / 2 == 0) {
					z = CreateSolidBrush(RGB((i * 255 / ::n) % 255, (6 * 255 / ::n) % 255, ((::n - i) * 255 / ::n) % 255));
					SelectObject(hdc, z);
				}
				else {
					z = CreateSolidBrush(RGB((1 * 255 / ::n) % 255, (((::n - i) * 255) / ::n) % 255, ((10 * 255) / ::n)) % 255);
					SelectObject(hdc, z);
				}
				Rectangle(hdc, arr[i].x, arr[i].y, arr[i].x + arr[i].width, arr[i].y + height);
				SelectObject(hdc, hold);
				DeleteObject(z);
			}

		}
		else  FillRect(hdc, &ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void initialize()
{
	if (n < 4)
		height = 60;
	else
	height = (gy - 10 - 150) / n;

	for (i = 0; i < n; i++) {
		arr[i].x = gx / 4 - 5 - ((i + 1)*(gx / 8)) / n;
		arr[i].y = gy - 10 - (n - i)*height;
		arr[i].width = 2 * (gx / 4 - arr[i].x);
		arr[i].bar = 1;
	}
	InvalidateRect(hwnd,NULL,FALSE);
	Sleep(1);
	//in = FALSE;
}

void transfer(int n, int from, int to, int temp)
{
	if (n>0)
	{
		transfer(n - 1, from, temp, to);
		Sleep(1);
		shift(n, to);   //logic
		Sleep(1);
		transfer(n - 1, temp, to, from);
	}
}

void shift(int n, int barto)
{
	int i, b = arr[n - 1].bar, c = 0, c1 = -1;
	for (i = 0; i<n - 1; i++)
	{
		if (arr[i].bar == arr[n - 1].bar&&arr[i].y<arr[n - 1].y)
			return;
	}

	for (i = 0; i<a; i++)
	{
		if (arr[i].bar == arr[n - 1].bar)
			c1++;
	}


	while (arr[n - 1].y >= 30)
	{
		arr[n - 1].y--;
		InvalidateRect(hwnd,NULL,FALSE);
		Sleep(1);
	}


	if (barto<arr[n - 1].bar)
	{
		while (arr[n - 1].x >= (barto*gx / 4 - (arr[n - 1].width / 2)))
		{
			arr[n - 1].x--;                        //moving left
			InvalidateRect(hwnd,NULL,FALSE);
			Sleep(1);
		}


	}
	else
	{

		while (arr[n - 1].x <= (barto*gx / 4 - (arr[n - 1].width / 2)))
		{

			arr[n - 1].x++;                 //moving right
			InvalidateRect(hwnd,NULL,FALSE);
			Sleep(1);
		}
	}

	arr[n - 1].bar = barto;
	for (i = 0; i<a; i++)
	{
		if (arr[i].bar == barto)
			c++;        //counting number of disks
	}

	while (arr[n - 1].y <= (gy - 10 - c*height))
	{
		arr[n - 1].y++;
		InvalidateRect(hwnd,NULL,FALSE);
		Sleep(1);
	}

	for (i = 0; i<n - 1; i++)
	{
		if (arr[i].bar == arr[n - 1].bar&&arr[i].y == arr[n - 1].y + height)
			shift(n, b);         // reverting disk to original peg
	}
}