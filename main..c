//#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <timeapi.h> // timeBeginPeriod
#include <immintrin.h> // _mm_pause()
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


// CONFIGURAÇÕES GLOBAIS
//#define WIN_WIDTH_LOGICAL  640
#define WIN_WIDTH_LOGICAL  640
#define WIN_HEIGHT_LOGICAL 512
// #define WIN_WIDTH_LOGICAL  54
// #define WIN_HEIGHT_LOGICAL 54
#define WIN_SCALE          1
#define WIN_TITLE          "Windows API"

typedef struct {
    HWND hwnd;
    HDC hdcWindow;
    // Backbuffer
    HDC hdcBackbuffer;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;
    // Dimensões
    int windowW, windowH;
    int viewX, viewY, viewW, viewH;
    // Configurações
    bool vsync;
    double targetFPS;
    double targetTPS;
    // Estado
    bool running;
    bool debug;
    // Debug Info
    int showFPS;
    int showTPS;
    // Timer Frequency
    int64_t perfFreq;

    int STATE;//1,2,3,4 -> start, gameover, playing, reset
} Game;
//Global game
Game game = {
    .vsync = true,
    .targetFPS = 60.0,
    .targetTPS = 60.0,
    .debug = true,
    .running = false,
    .STATE = 1
};


typedef struct {
    int playerWidth;
    int playerHeight;
    int playerSpeed;
    int playerXposition;
    int playerYposition;
    bool playerMoveUP;
    bool playerMoveDOWN;
} Player;


Player player = {
    .playerSpeed = 8,
    .playerXposition = 64,
    .playerYposition = 64+128,
    .playerWidth = 32,
    .playerHeight = 128,
    .playerMoveUP = false,
    .playerMoveDOWN = false,
};
// Player player = {
//     .playerSpeed = 1,
//     .playerXposition = 1,
//     .playerYposition = 1,
//     .playerWidth = 1,
//     .playerHeight = 1,
//     .playerMoveUP = false,
//     .playerMoveDOWN = false,
// };

typedef struct {
    int enemyWidth;
    int enemyHeight;
    int enemySpeed;
    int enemyXposition;
    int enemyYposition;
    int initialEnemyPositionX;
    int initialEnemyPositionY;
    bool enemyMoveUP;
    bool enemyMoveDOWN;
} Enemy;

Enemy enemy ={
    .enemySpeed = 8,
    .enemyXposition = 640 - 64 - 32,
    .enemyYposition = 64 + 128,
    .initialEnemyPositionX = 640 - 64 - 32,
    .initialEnemyPositionY = 64 + 128,
    .enemyWidth = 32,
    .enemyHeight = 128,
    .enemyMoveUP = false,
    .enemyMoveDOWN = false,
};


typedef struct {
    int ballWidth;
    int ballHeight;
    int ballSpeedX;
    int ballSpeedY;
    int ballX;
    int ballY;
    int initialBallPositionX;
    int initialBallPositionY;
} Ball;

Ball ball ={
    .ballX = 320 - 8,
    .ballY = 256 - 8,
    .initialBallPositionX = 320 - 8,
    .initialBallPositionY = 256 - 8,
    .ballSpeedX = 8,
    .ballSpeedY = 8,
    .ballHeight = 16,
    .ballWidth = 16,
};



typedef struct {
    int width;
    int height;
    int pitch;
    uint32_t* pixels;
    BITMAPINFO info;
} Backbuffer;

Backbuffer backbuffer;

void timer_init() {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    game.perfFreq = li.QuadPart;
    timeBeginPeriod(1);
}

void timer_cleanup() {
    timeEndPeriod(1);
}

int64_t timer_get_ns() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    int64_t seconds = li.QuadPart / game.perfFreq;
    int64_t part = li.QuadPart % game.perfFreq;

    return seconds * 1000000000LL + (part * 1000000000LL) / game.perfFreq;
}

int64_t timer_get_ms() {
    return timer_get_ns() / 1000000;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void game_init(HINSTANCE hInstance);
void game_start();
void game_run();
void game_tick();
void game_render();
void game_dispose();

void player_move();
void player_StopMove();
void player_render();
void player_tick();

void ball_tick();
void ball_render();

void enemy_render();
void enemy_tick();

// Helpers
void update_viewport(int windowWidth, int windowHeight);
void render_text(HDC hdc, int x, int y, const char* text, COLORREF color);

//Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    game_init(hInstance);
    game_start();
    return 0;
}
void game_init(HINSTANCE hInstance) {
    timer_init();

    const char CLASS_NAME[] = "EngineCatClass";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    RECT rect = {0, 0, WIN_WIDTH_LOGICAL * WIN_SCALE, WIN_HEIGHT_LOGICAL * WIN_SCALE};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    game.hwnd = CreateWindowEx(
        0, CLASS_NAME, WIN_TITLE,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, hInstance, NULL
    );

    game.hdcWindow = GetDC(game.hwnd);

    //Backbuffer
    // game.hdcBackbuffer = CreateCompatibleDC(game.hdcWindow);
    // game.hBitmap = CreateCompatibleBitmap(game.hdcWindow, WIN_WIDTH_LOGICAL, WIN_HEIGHT_LOGICAL);
    // game.hOldBitmap = (HBITMAP)SelectObject(game.hdcBackbuffer, game.hBitmap);
    game.hdcBackbuffer = CreateCompatibleDC(game.hdcWindow);

    backbuffer.width  = WIN_WIDTH_LOGICAL;
    backbuffer.height = WIN_HEIGHT_LOGICAL;
    backbuffer.pitch  = WIN_WIDTH_LOGICAL;

    ZeroMemory(&backbuffer.info, sizeof(backbuffer.info));
    backbuffer.info.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    backbuffer.info.bmiHeader.biWidth       = backbuffer.width;
    backbuffer.info.bmiHeader.biHeight      = -backbuffer.height; // top-down
    backbuffer.info.bmiHeader.biPlanes      = 1;
    backbuffer.info.bmiHeader.biBitCount    = 32;
    backbuffer.info.bmiHeader.biCompression = BI_RGB;

    game.hBitmap = CreateDIBSection(
        game.hdcWindow,
        &backbuffer.info,
        DIB_RGB_COLORS,
        (void**)&backbuffer.pixels,
        NULL,
        0
    );

    game.hOldBitmap = (HBITMAP)SelectObject(game.hdcBackbuffer, game.hBitmap);

    // Fonte
    HFONT hFont = CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, "Consolas");
    SelectObject(game.hdcBackbuffer, hFont);
    SetBkMode(game.hdcBackbuffer, TRANSPARENT);

    // Viewport
    RECT clientRect;
    GetClientRect(game.hwnd, &clientRect);
    update_viewport(clientRect.right, clientRect.bottom);

    SetFocus(game.hwnd);
}

void game_start() {
    game.running = true;
    game_run();
    game_dispose();
}

void game_run() {
    int64_t lastTime = timer_get_ns();
    double nsPerTick = 1000000000.0 / game.targetTPS;
    double delta = 0;

    int64_t lastRenderTime = timer_get_ns();

    int frames = 0;
    int ticks = 0;
    int64_t timer = timer_get_ms();

    MSG msg = {0};

    while (game.running) {
        // Input / Window Events
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) game.running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Timer Calculations
        int64_t now = timer_get_ns();
        delta += (double)(now - lastTime) / nsPerTick;
        lastTime = now;

        // Logic (Tick)
        while (delta >= 1) {
            game_tick();
            ticks++;
            delta--;
            if (delta > 100) { delta = 0; break; }
        }

        // FPS Limitado
        double effectiveFPS = game.vsync ? 60.0 : game.targetFPS;

        if (effectiveFPS > 0) {
            double nsPerFrame = 1000000000.0 / effectiveFPS;
            int64_t nextRenderTime = lastRenderTime + (int64_t)nsPerFrame;
            int64_t now_t = timer_get_ns();

            if (now_t < nextRenderTime) {
                while (now_t < nextRenderTime) {
                    int64_t remaining = nextRenderTime - now_t;

                    if (remaining > 1500000) { // > 1.5ms
                        Sleep(1);
                    } else {
                        _mm_pause();
                    }
                    now_t = timer_get_ns();
                }
            }

            game_render();
            frames++;
            lastRenderTime = now_t;

        } else {
            // FPS Ilimitado
            game_render();
            frames++;
        }

        // Debug Display (1 sec)
        if (timer_get_ms() - timer >= 1000) {
            game.showFPS = frames;
            game.showTPS = ticks;
            frames = 0;
            ticks = 0;
            timer += 1000;
        }
    }
}

void control_tick(void);

void logic_tick(void);

void game_tick() {
    // Input Handling
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) game.running = false;

    // V-Sync Toggle ('V')
    static int64_t lastVsyncTime = 0;
    if (GetAsyncKeyState('V') & 0x8000) {
        if (timer_get_ms() - lastVsyncTime > 200) {
            game.vsync = !game.vsync;
            lastVsyncTime = timer_get_ms();
        }
    }

    // F1 - Aumenta FPS
    static int64_t lastFpsTime = 0;
    if (GetAsyncKeyState(VK_F1) & 0x8000) {
        if (timer_get_ms() - lastFpsTime > 100) {
            game.targetFPS += 10;
            lastFpsTime = timer_get_ms();
        }
    }

    // F2 - Diminui FPS
    if (GetAsyncKeyState(VK_F2) & 0x8000) {
        if (timer_get_ms() - lastFpsTime > 100) {
            game.targetFPS -= 10;
            if (game.targetFPS < 10 && game.targetFPS > 0) game.targetFPS = 10;
            if (game.targetFPS <= 0) game.targetFPS = 0;
            lastFpsTime = timer_get_ms();
        }
    }


    logic_tick();



}


uint32_t BGRA(uint8_t i, uint8_t i1, uint8_t i2, uint8_t i3);

void enemy_render(void);

void ball_render(void);

void game_render() {
    // 1. Limpa o Backbuffer com a cor exata (0.2, 0.2, 0.2 ~= RGB 51, 51, 51)
    RECT logicalRect = {0, 0, WIN_WIDTH_LOGICAL, WIN_HEIGHT_LOGICAL};
    HBRUSH bgBrush = CreateSolidBrush(RGB(51, 51, 51));
    FillRect(game.hdcBackbuffer, &logicalRect, bgBrush);
    DeleteObject(bgBrush);

    // Coordenadas x=100, y=100, w=width_texture(32), h=height_texture(32)
    RECT playerRect = {10, 10, 11, 11};
    HBRUSH playerBrush = CreateSolidBrush(RGB(255, 0, 0));
    FillRect(game.hdcBackbuffer, &playerRect, playerBrush);
    DeleteObject(playerBrush);


    uint32_t* pixels = backbuffer.pixels;
    int pitch = backbuffer.width;

    for (int y = 0; y < WIN_HEIGHT_LOGICAL; ++y) {
        int red = 0;
        for (int x = 0; x < WIN_WIDTH_LOGICAL; ++x) {
                pixels[y * pitch + x] = BGRA(0,0,red,0);
                red = x * 255 / (WIN_WIDTH_LOGICAL - 1);
                if (red >= 255) {
                    red = 0;
                };
        }
    }


    player_render();

    enemy_render();

    ball_render();



    char buffer[128];

    //Infos
    snprintf(buffer, sizeof(buffer), "FPS: %d | TPS: %d | V-Sync: %s",
             game.showFPS, game.showTPS, (game.vsync ? "ON" : "OFF"));
    render_text(game.hdcBackbuffer, 10, 10, buffer, RGB(255, 255, 255));

    //Target FPS
    if (!game.vsync) {
        snprintf(buffer, sizeof(buffer), "Target: %.0f (F1/F2)", game.targetFPS);
        render_text(game.hdcBackbuffer, 10, 30, buffer, RGB(200, 200, 200));
    } else {
        render_text(game.hdcBackbuffer, 10, 30, "'V' - Toggle V-Sync", RGB(200, 200, 200));
    }

    //Anti-Flicker com ExcludeClipRect
    int savedDC = SaveDC(game.hdcWindow);

    //área do jogo não sera pintada de preto
    ExcludeClipRect(game.hdcWindow, game.viewX, game.viewY,
                    game.viewX + game.viewW, game.viewY + game.viewH);

    //bordas não usadas de preto
    HBRUSH blackBrush = GetStockObject(BLACK_BRUSH);
    RECT winRect = {0, 0, game.windowW, game.windowH};
    FillRect(game.hdcWindow, &winRect, blackBrush);

    RestoreDC(game.hdcWindow, savedDC);

    // Estica o Backbuffer para a Janela
    SetStretchBltMode(game.hdcWindow, COLORONCOLOR);
    StretchBlt(game.hdcWindow,
               game.viewX, game.viewY, game.viewW, game.viewH, // Destino
               game.hdcBackbuffer,
               0, 0, WIN_WIDTH_LOGICAL, WIN_HEIGHT_LOGICAL,    // Origem
               SRCCOPY);
}

void game_dispose() {
    timer_cleanup();
    SelectObject(game.hdcBackbuffer, game.hOldBitmap);
    DeleteObject(game.hBitmap);
    DeleteDC(game.hdcBackbuffer);
    ReleaseDC(game.hwnd, game.hdcWindow);
}

//1 ups
//2 down
void player_move(int direction) {
    switch (direction) {
        case 1:
            player.playerMoveUP = true;
            break;
        case 2:
            player.playerMoveDOWN = true;
        default:
            break;
    }
}

//1 ups
//2 down
void player_stopMove(int direction) {
    switch (direction) {
        case 1:
            player.playerMoveUP = false;
            break;
        case 2:
            player.playerMoveDOWN = false;
        default:
            break;
    }
}
void player_render() {
    int xx = (int)player.playerXposition;
    int yy = (int)player.playerYposition;
    int h  = player.playerHeight;
    int w  = player.playerWidth;

    for (int y = 0; y < h; ++y) {
        int py = yy + y;
        if (py < 0 || py >= backbuffer.height) continue;


        uint32_t* row = backbuffer.pixels + py * backbuffer.pitch;

        for (int x = 0; x < w; ++x) {
            int px = xx + x;
            if (px < 0 || px >= backbuffer.width) continue;

            row[px] = BGRA(255, 255, 255, 0);
        }
    }
}




void player_render2() {
    int xx = (int)player.playerXposition;
    int yy = (int)player.playerYposition;
    int h  = player.playerHeight;
    int w  = player.playerWidth;

    int lineY = h / 2; // linha no meio do player

    for (int y = 0; y < h; ++y) {
        int py = yy + y;
        if (py < 0 || py >= backbuffer.height) continue;

        if (y != lineY) continue; // desenha só a linha

        uint32_t* row = backbuffer.pixels + py * backbuffer.pitch;

        for (int x = 0; x < w; ++x) {
            int px = xx + x;
            if (px < 0 || px >= backbuffer.width) continue;

            uint8_t red = (uint8_t)(x * 255 / (w - 1));
            row[px] = BGRA(0, 0, red, 0);
        }
    }
}


void player_tick() {



    //move logic
    if (player.playerMoveUP && player.playerYposition > 0) {
        player.playerYposition = player.playerYposition - player.playerSpeed;
    } else if (player.playerMoveDOWN && player.playerYposition < 512 - player.playerHeight) {
        player.playerYposition = player.playerYposition + player.playerSpeed;
    }
}
void control_tick() {
    // WASD
    if (GetAsyncKeyState('W')) {
        player_move(1);
    }else {
        player_stopMove(1);
    }

    if (GetAsyncKeyState('S')) {
        player_move(2);
    }else {
        player_stopMove(2);
    }

    if (GetAsyncKeyState(VK_SPACE)) {
        if (game.STATE == 1) {
            game.STATE = 3;
        }else if (game.STATE == 3) {
            game.STATE = 1;
        }
    }

    // if (GetAsyncKeyState('A')) {
    //     player.playerXposition -= player.playerSpeed;
    // }
    // if (GetAsyncKeyState('D')) {
    //     player.playerXposition += player.playerSpeed;
    // }
}

void ball_render() {
    int xx = ball.ballX;
    int yy = ball.ballY;
    int h  = ball.ballHeight;
    int w  = ball.ballWidth;

    for (int y = 0; y < h; ++y) {
        int py = yy + y;
        if (py < 0 || py >= backbuffer.height) continue;


        uint32_t* row = backbuffer.pixels + py * backbuffer.pitch;

        for (int x = 0; x < w; ++x) {
            int px = xx + x;
            if (px < 0 || px >= backbuffer.width) continue;

            row[px] = BGRA(255, 255, 255, 0);
        }
    }
}

void ball_tick() {
}

void enemy_render() {
    int xx = enemy.enemyXposition;
    int yy = enemy.enemyYposition;
    int h  = enemy.enemyHeight;
    int w  = enemy.enemyWidth;

    for (int y = 0; y < h; ++y) {
        int py = yy + y;
        if (py < 0 || py >= backbuffer.height) continue;


        uint32_t* row = backbuffer.pixels + py * backbuffer.pitch;

        for (int x = 0; x < w; ++x) {
            int px = xx + x;
            if (px < 0 || px >= backbuffer.width) continue;

            row[px] = BGRA(255, 255, 255, 0);
        }
    }
}

void enemy_tick() {
    //random 0.7 - 0.9
    float randomSpeed = 0.7 + ((float)rand() / RAND_MAX) * 0.2;
    //printf("%f\n",randomSpeed);

    if (enemy.enemyYposition + enemy.enemyHeight / 2 <= ball.ballY) {
        enemy.enemyYposition += enemy.enemySpeed * randomSpeed;

    }
    else if (enemy.enemyYposition + enemy.enemyHeight / 2 > ball.ballY) {
        enemy.enemyYposition -= enemy.enemySpeed * randomSpeed;
    }


    //collision
    if(enemy.enemyYposition < 0){
        enemy.enemyYposition = 0;
    }
    if(enemy.enemyYposition + enemy.enemyHeight > 512){
        enemy.enemyYposition = 512 - enemy.enemyHeight;

    }

}

void logic_tick() {
    control_tick();
    if (game.STATE == 1) {
        player_tick();

        //reset ball & enemy
        enemy.enemyXposition = enemy.initialEnemyPositionX;
        enemy.enemyYposition = enemy.initialEnemyPositionY;
        ball.ballX = ball.initialBallPositionX;
        ball.ballY = ball.initialBallPositionY;
    }

    if (game.STATE == 3) {
        player_tick();
        enemy_tick();
        ball_tick();
    }

}


// UTIL WINDOW
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY: PostQuitMessage(0); return 0;
        case WM_SIZE: update_viewport(LOWORD(lParam), HIWORD(lParam)); return 0;
        case WM_ERASEBKGND: return 1;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void update_viewport(int windowWidth, int windowHeight) {
    game.windowW = windowWidth;
    game.windowH = windowHeight;

    float targetAspect = (float)WIN_WIDTH_LOGICAL / (float)WIN_HEIGHT_LOGICAL;
    float windowAspect = (float)windowWidth / (float)windowHeight;

    if (windowAspect >= targetAspect) {
        game.viewH = windowHeight;
        game.viewW = (int)(windowHeight * targetAspect);
        game.viewY = 0;
        game.viewX = (windowWidth - game.viewW) / 2;
    } else {
        game.viewW = windowWidth;
        game.viewH = (int)(windowWidth / targetAspect);
        game.viewX = 0;
        game.viewY = (windowHeight - game.viewH) / 2;
    }
}

void render_text(HDC hdc, int x, int y, const char* text, COLORREF color) {
    SetTextColor(hdc, color);
    TextOut(hdc, x, y, text, strlen(text));
}

inline uint32_t BGRA(uint8_t b, uint8_t g, uint8_t r, uint8_t a)
{
    return (b) | (g << 8) | (r << 16) | (a << 24);
}
