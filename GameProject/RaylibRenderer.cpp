#include "RaylibRenderer.h"
#include <raylib.h>   // Only this translation unit pulls in raylib.

// ---------------------------------------------------------------------------
// Initialize / Shutdown
// ---------------------------------------------------------------------------

bool RaylibRenderer::Initialize(int width, int height, const std::string& title)
{
    m_Width  = width;
    m_Height = height;
    m_Title  = title;

    InitWindow(width, height, title.c_str());
    // The Application game-loop already sleeps ~16 ms per iteration (targeting
    // ~60 fps via std::this_thread::sleep_for).  Disabling raylib's own frame
    // limiter prevents the two sleep mechanisms from stacking, which would
    // otherwise halve the effective frame rate to ~30 fps.
    SetTargetFPS(0);

    m_Initialized = IsWindowReady();
    return m_Initialized;
}

void RaylibRenderer::Shutdown()
{
    if (!m_Initialized)
        return;

    // Unload every sprite sheet that was loaded through this renderer.
    for (const auto& s : m_Sheets)
    {
        // Reconstruct a minimal Texture2D; raylib's UnloadTexture only checks
        // width/height != 0 before calling rlUnloadTexture(id).
        Texture2D t{};
        t.id     = s.texId;
        t.width  = s.width;
        t.height = s.height;
        t.mipmaps = 1;
        t.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        UnloadTexture(t);
    }
    m_Sheets.clear();

    CloseWindow();
    m_Initialized = false;
}

// ---------------------------------------------------------------------------
// Frame lifecycle
// ---------------------------------------------------------------------------

void RaylibRenderer::Clear(Color c)
{
    BeginDrawing();
    ::Color rlColor{
        static_cast<unsigned char>(c.r * 255.0f),
        static_cast<unsigned char>(c.g * 255.0f),
        static_cast<unsigned char>(c.b * 255.0f),
        static_cast<unsigned char>(c.a * 255.0f)
    };
    ClearBackground(rlColor);
}

void RaylibRenderer::Present()
{
    EndDrawing();
}

// ---------------------------------------------------------------------------
// Texture management
// ---------------------------------------------------------------------------

int RaylibRenderer::LoadSpriteSheet(const std::string& filePath)
{
    if (!m_Initialized)
        return -1;

    Texture2D tex = LoadTexture(filePath.c_str());
    if (tex.id == 0)
        return -1;

    m_Sheets.push_back({ tex.id, tex.width, tex.height });
    return static_cast<int>(m_Sheets.size()) - 1;
}

// ---------------------------------------------------------------------------
// Sprite drawing
// ---------------------------------------------------------------------------

void RaylibRenderer::DrawSprite(int  sheetId,
                                int  frameW,   int frameH,
                                int  frameCol, int frameRow,
                                int  destX,    int destY,
                                int  destW,    int destH,
                                bool flipX)
{
    if (sheetId < 0 || sheetId >= static_cast<int>(m_Sheets.size()))
        return;

    const SheetEntry& s = m_Sheets[sheetId];

    // Reconstruct a minimal Texture2D for the draw call.
    Texture2D tex{};
    tex.id     = s.texId;
    tex.width  = s.width;
    tex.height = s.height;
    tex.mipmaps = 1;
    tex.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    // A negative source width tells DrawTexturePro to mirror horizontally.
    Rectangle src{
        static_cast<float>(frameCol * frameW),
        static_cast<float>(frameRow * frameH),
        flipX ? -static_cast<float>(frameW) : static_cast<float>(frameW),
        static_cast<float>(frameH)
    };
    Rectangle dst{
        static_cast<float>(destX),
        static_cast<float>(destY),
        static_cast<float>(destW),
        static_cast<float>(destH)
    };
    Vector2 origin{ 0.0f, 0.0f };

    DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
}

// ---------------------------------------------------------------------------
// Input helpers
// ---------------------------------------------------------------------------

bool RaylibRenderer::IsKeyDown(int rlKey)    const { return ::IsKeyDown(rlKey);    }
bool RaylibRenderer::IsKeyPressed(int rlKey) const { return ::IsKeyPressed(rlKey); }
bool RaylibRenderer::ShouldClose()           const { return WindowShouldClose();   }
