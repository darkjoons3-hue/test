#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    // --- 1. Регистрация класса окна ---
    const wchar_t CLASS_NAME[] = L"MyFirstWindowClass";
    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 🎨 ЦВЕТ ФОНА ОКНА (сейчас системный белый)
    RegisterClass(&wc);

    // --- 2. Создание окна ---
    HWND hwnd = CreateWindowEx(
        0,                              // расширенный стиль
        CLASS_NAME,                     // имя класса
        L"Моё первое окно",             // 🏷️ ЗАГОЛОВОК ОКНА (текст в title bar)
        WS_OVERLAPPEDWINDOW,            // стиль: с рамкой, кнопками свернуть/развернуть
        CW_USEDEFAULT, CW_USEDEFAULT,   // позиция (пусть Windows выберет)
        800, 600,                       // 📐 РАЗМЕР ОКНА: ширина 800, высота 600
        NULL, NULL, hInstance, NULL
    );
    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    // --- 3. Цикл сообщений ---
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

// --- 4. Оконная процедура: здесь обрабатываются события ---
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect); // получаем всю клиентскую область

        // 🎨 ЦВЕТ ТЕКСТА (сейчас чёрный)
        SetTextColor(hdc, RGB(0, 0, 0));
        // 🎨 ЦВЕТ ФОНА ПОД ТЕКСТОМ (делаем прозрачным, чтобы был виден фон окна)
        SetBkMode(hdc, TRANSPARENT);

        // ✏️ ТЕКСТ, КОТОРЫЙ РИСУЕТСЯ В ЦЕНТРЕ
        const wchar_t* text = L"Привет!";
        DrawText(hdc, text, -1, &rect,
                 DT_CENTER | DT_VCENTER | DT_SINGLELINE); // центрирование по горизонтали и вертикали

        EndPaint(hwnd, &ps);
    } return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
