#include <windows.h>
#include <gl/gl.h>
#include "camera.h"
#include <cmath>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
int n = 20;


void ResizeWindow(int width, int height){
    glViewport(0,0,width,height);
    float aspectRatio = width / (float)height;
    float size = 0.1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-aspectRatio * size, aspectRatio * size, -size, size, size * 2, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}




void MoveTheCamera(){
    Camera_MoveDirectional(
        GetKeyState('W')< 0 ? 1 : GetKeyState('S')< 0 ? -1 : 0,
        GetKeyState('D')< 0 ? 1 : GetKeyState('A')< 0 ? -1 : 0, 0.1);

    Camera_AutoMoveByMouse(400,400,0.1);
}

float vertex[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1};
float normal[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1};
float normal_vert[]={0,0,1, 0,0,1, 0,0,1, 0,0,1};

void Draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void Init_Material() {
    glEnable(GL_COLOR_MATERIAL); // Разрешаем использование материалов
    glShadeModel(GL_SMOOTH); // Гладкое затенение

    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_shininess[] = { 50.0f }; // Блеск материала

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

void Init()
{
    glEnable(GL_BLEND);/*разрешение смешивания цветов*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);/*настройка смешивания цветов для каждой точки*/
    /*как будет братся цвет из источника, как будет братся цвет из приемника */

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
}

void RenderCoords() {
    glBegin(GL_LINES);
    // X-axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Y-axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z-axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void RenderChessboard(int n, float tileSize, float posX, float posY, float posZ){
    glEnableClientState(GL_NORMAL_ARRAY);
 glNormalPointer(GL_FLOAT,0,&normal_vert);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i + j) % 2 == 0) glColor3f(0.0f, 0.0f, 1.0f);
            else glColor3f(0.5f, 0.0f, 0.0f);
            glPushMatrix();
            glTranslatef(i * tileSize + posX, j * tileSize + posY, posZ);
            glBegin(GL_QUADS);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(tileSize, 0.0f, 0.0f);
            glVertex3f(tileSize, tileSize, 0.0f);
            glVertex3f(0.0f, tileSize, 0.0f);
            glEnd();
            glPopMatrix();
        }
    }
    glDisable(GL_NORMAL_ARRAY);
}

void DrawCube()
{
    glPushMatrix();
        glScalef(1, 1, 1);
        glTranslatef(-5, -6, 1);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 0,1,0);
        Draw();
        glRotatef(180, 0,1,0);
        Draw();
    glPopMatrix();
}
void DrawCubeSecond()
{
    glPushMatrix();
        glScalef(1, 1, 1);
        glTranslatef(4, 5, 1);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 0,1,0);
        Draw();
        glRotatef(180, 0,1,0);
        Draw();
    glPopMatrix();
}

void DrawPrism(const int n)
{
    const float pi = acos(-1);
    const int vn = 2*n; // количество вершин

    float vertieces[3*vn]; // Это же и нормали.

    for (int i = 0; i < n; i++) {
        float phi = 2 * pi * (float)i / (float)n;
        float c = cos(phi);
        float u = cos(phi);
        float s = sin(phi);

        // Вершина в верхнем основании
        vertieces[6*i + 0] = c/2;
        vertieces[6*i + 1] = s/2;
        vertieces[6*i + 2] = 1.0;

        // Вершина в нижнем основании
        vertieces[6*i + 3] = c;
        vertieces[6*i + 4] = s;
        vertieces[6*i + 5] = -1.0;
    }

    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertieces);
    glNormalPointer(GL_FLOAT, 0, vertieces);

    // Боковые стороны
    GLuint side_index[4];
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 4; k++) {
            side_index[k] = (2*i + k) % vn;
        }

        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, &side_index);
    }

    // Верхнее основание
    GLuint base_index[n];
    for (int k = 0; k < n; k++) {
        base_index[k] = 2*k;
    }
    glDrawElements(GL_POLYGON, n, GL_UNSIGNED_INT, &base_index);

    // Нижнее основание
    for (int k = 0; k < n; k++) {
        base_index[k] = 2*k + 1;
    }
    glDrawElements(GL_POLYGON, n, GL_UNSIGNED_INT, &base_index);


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
}

int WINAPI WinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine,
                      int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "NewGLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "NewGLSample",
                          "Modified Lab 5",
                          WS_POPUP,
                          0,
                          0,
                          GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

     ShowCursor(FALSE);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glEnable(GL_DEPTH_TEST);



    glEnable(GL_LIGHTING);/*допуск к освещению*/
    glEnable(GL_LIGHT0);/*режим*/
    //glEnable(GL_COLOR_MATERIAL); /*материал*/
    Init_Material();
    glEnable(GL_NORMALIZE);/*доступ к нормали*/

    RECT rct;
    GetClientRect(hwnd, &rct);
    ResizeWindow(rct.right, rct.bottom);
Init();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();

            if (GetForegroundWindow() == hwnd) MoveTheCamera();
            Camera_Apply();

            glPushMatrix();
                    glRotatef(24,0,0,1);
                    glRotatef(theta,0,1,0);
                    float position[] = {0,0,1,0};
                    glLightfv(GL_LIGHT0, GL_POSITION, position);

                    glTranslatef(0,0,40);
                    glScalef(1,1,1);
                    glColor3f(1,1,1);
                    Draw();
                glPopMatrix();


            glColor3f(0.4,0.4,0.2);
            DrawCube();
            glColor3f(1,0.5,0);
            DrawCubeSecond();

            RenderCoords();

            RenderChessboard(10, 2, -10, -10,0);

            glBegin(GL_TRIANGLES);

            glColor3f(1.0f, 0.0f, 0.0f);   glVertex3f(0.0f, 1.0f, 1.0f);
            glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(0.87f, -0.5f, 1.0f);
            glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(-0.87f, -0.5f, 1.0f);

            glEnd();

             glTranslatef(0.0, 0.0, 1.0);
                float transparency = 0.1;
                for (int i = 0; i < 10; i++)
                {
                    glPushMatrix();
                        glRotatef(36*i, 0, 0, 1);
                        glTranslatef(5, 0, 0);
                        glColor4f(0,0,1,transparency);
                        DrawPrism(n);
                    glPopMatrix();
                    transparency += 0.1;
                }
            glPopMatrix();
            theta=theta+0.5;
            SwapBuffers(hDC);

            Sleep (1);

            //lightAngle += 1.0f;
            //Move_Light(lightAngle);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
