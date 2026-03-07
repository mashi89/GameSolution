#pragma once
#include "Renderer.h"
#include <string>
#include <vector>

// RayLib key-code constants (mirrors raylib.h KEY_* defines) so that callers
// do not need to include raylib.h directly and avoid the Color name collision.
namespace RaylibKey
{
    constexpr int Escape = 256;
    constexpr int Right  = 262;
    constexpr int Left   = 263;
    constexpr int Down   = 264;
    constexpr int Up     = 265;
}

// Concrete Renderer implementation backed by RayLib.
// A single window is created in Initialize() and destroyed in Shutdown().
class RaylibRenderer : public Renderer
{
public:
    ~RaylibRenderer() override { Shutdown(); }

    // Renderer interface -------------------------------------------------------
    bool Initialize(int width, int height, const std::string& title) override;
    void Shutdown() override;
    // Begins a new frame and clears the back-buffer to the given colour.
    void Clear(RenderColor clearColor = RenderColor::Black()) override;
    // Ends the frame and presents it to the screen.
    void Present() override;

    // Sprite-sheet support -----------------------------------------------------
    // Loads an image file (PNG, etc.) and returns a non-negative sheet index
    // that can be passed to DrawSprite.  Returns -1 on failure.
    int  LoadSpriteSheet(const std::string& filePath);

    // Draws one frame from a sprite sheet.
    //   sheetId           : value returned by LoadSpriteSheet
    //   frameW / frameH   : pixel size of a single animation frame
    //   frameCol/frameRow : zero-based column/row of the frame inside the sheet
    //   destX / destY     : top-left of the destination rectangle (window pixels)
    //   destW / destH     : size of the destination rectangle (window pixels)
    //   flipX             : mirror the frame horizontally (e.g. left-facing sprite)
    void DrawSprite(int  sheetId,
                    int  frameW,    int frameH,
                    int  frameCol,  int frameRow,
                    int  destX,     int destY,
                    int  destW,     int destH,
                    bool flipX = false);

    // Input --------------------------------------------------------------------
    bool IsKeyDown(int rlKey)    const;  // true while key is held
    bool IsKeyPressed(int rlKey) const;  // true on the first frame the key is down
    bool ShouldClose()           const;  // true when the window's X button is clicked

private:
    bool m_Initialized = false;

    // Lightweight texture handle that avoids exposing raylib types in this header
    // (raylib defines its own "Color" struct which would clash with Renderer.h).
    struct SheetEntry
    {
        unsigned int texId  = 0;
        int          width  = 0;
        int          height = 0;
    };
    std::vector<SheetEntry> m_Sheets;
};
