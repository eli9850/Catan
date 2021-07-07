#include "GUIClient.h"

GUIClient::GUIClient(HINSTANCE instance): m_instance(instance) {
   
    WNDCLASSEX window_class = get_initialized_window_class();

    if (!RegisterClassEx(&window_class))
    {
        MessageBox(NULL,
            L"Call to RegisterClassEx failed!",
            std::to_wstring(GetLastError()).c_str(),
            NULL);

    }

    create_main_window();

    //auto a = (HBITMAP)LoadImage(NULL, L"empty_board.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    auto a = (HBITMAP)LoadImage(NULL, IMAGE_PATH.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_bitmap = a;
    auto hLogo = CreateWindow(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 350, 60, 400, 400, m_hwnd, NULL, NULL, NULL);
    SendMessage(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) m_bitmap);

    ShowWindow(m_hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_hwnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

WNDCLASSEX GUIClient::get_initialized_window_class() {
    
    WNDCLASSEX windows_class;

    windows_class.cbSize = sizeof(WNDCLASSEX);
    windows_class.style = CS_HREDRAW | CS_VREDRAW;
    windows_class.lpfnWndProc = wnd_proc;
    windows_class.cbClsExtra = 0;
    windows_class.cbWndExtra = 0;
    windows_class.hInstance = m_instance;
    windows_class.hIcon = LoadIcon(m_instance, IDI_APPLICATION);
    windows_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    windows_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windows_class.lpszMenuName = NULL;
    windows_class.lpszClassName = APPLICATION_NAME.data();
    windows_class.hIconSm = LoadIcon(windows_class.hInstance, IDI_APPLICATION);

    return windows_class;
}

void GUIClient::create_main_window() {
    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // this: pointer to the class object in order to use it in the wndproc function
    m_hwnd = CreateWindow(
        APPLICATION_NAME.data(),
        APPLICATION_NAME.data(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1000, 600,
        NULL,
        NULL,
        m_instance,
        this
    );

    if (!m_hwnd)
    {
        MessageBox(NULL,
            L"Call to CreateWindow failed!",
            std::to_wstring(GetLastError()).c_str(),
            NULL);
        return;

    }
}

LRESULT GUIClient::wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    GUIClient* pThis;

    if (message == WM_NCCREATE)
    {
        pThis = static_cast<GUIClient*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        pThis = reinterpret_cast<GUIClient*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));       
    }

    if (pThis)
    {
        switch (message)
        {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

