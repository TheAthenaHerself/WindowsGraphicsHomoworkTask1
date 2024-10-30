#include <d2d1_3.h>
#include <stdexcept>
#include "Direct.h"
using D2D1::RenderTargetProperties;
using D2D1::HwndRenderTargetProperties;
using D2D1::SizeU;

namespace {
    ID2D1Factory7* d2d_factory = nullptr;
    ID2D1HwndRenderTarget* d2d_render_target = nullptr;
    HRESULT error_code;
    UINT_PTR timer;
    void createRenderTarget(HWND hwnd) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        error_code = d2d_factory->CreateHwndRenderTarget(
            RenderTargetProperties(),
            HwndRenderTargetProperties(
                hwnd,
                SizeU(
                    static_cast<UINT32>(rc.right) -
                    static_cast<UINT32>(rc.left),
                    static_cast<UINT32>(rc.bottom) -
                    static_cast<UINT32>(rc.top)
                )),
            &d2d_render_target);
        if (error_code != S_OK) {
            throw std::runtime_error("Failed to create render target.");
        }
    }
}
void initD2(HWND hwnd) {
    error_code = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);
    if (error_code != S_OK) {
        throw std::runtime_error("Failed to create D2D1 Factory.");
    }
    createRenderTarget(hwnd);
}

void paintD2(HWND hwnd) {
    if (!d2d_render_target) {
        createRenderTarget(hwnd);
    }
    //setup

    d2d_render_target->BeginDraw();

    //draw

    if (d2d_render_target->EndDraw() == D2DERR_RECREATE_TARGET) {
        if (d2d_render_target) d2d_render_target->Release();
    }
    //destroy
}

void destroyD2() {
    if (d2d_render_target) d2d_render_target->Release();
    if (d2d_factory) d2d_factory->Release();
}

void InitTimer(HWND hwnd) {
    timer = SetTimer(hwnd, NULL, NULL, NULL);
}

void OnTimer() {
    //change time
}

void ReleaseTimer(HWND hwnd) {
    KillTimer(hwnd, timer);
}