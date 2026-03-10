#include "PlayingState.h"
#include "RaylibRenderer.h"
#include "GameManager.h"
#include <algorithm>
#include <cmath>

PlayingState::PlayingState() = default;
PlayingState::~PlayingState() = default;



// Relative path to the Body_A walk sprite sheets.
// The working directory when launched from Visual Studio is the project folder.
static constexpr const char* ASSET_WALK_DOWN =
    "VisualAssets/Pixel Crawler - Free Pack/Entities/Characters/Body_A/Animations/Walk_Base/Walk_Down-Sheet.png";
static constexpr const char* ASSET_WALK_SIDE =
    "VisualAssets/Pixel Crawler - Free Pack/Entities/Characters/Body_A/Animations/Walk_Base/Walk_Side-Sheet.png";
static constexpr const char* ASSET_WALK_UP =
    "VisualAssets/Pixel Crawler - Free Pack/Entities/Characters/Body_A/Animations/Walk_Base/Walk_Up-Sheet.png";

void PlayingState::OnEnter()
{
    m_Renderer = std::make_unique<RaylibRenderer>();
    m_Renderer->Initialize(WINDOW_W, WINDOW_H, "Game Screen");

    m_TexDown = m_Renderer->LoadSpriteSheet(ASSET_WALK_DOWN);
    m_TexSide = m_Renderer->LoadSpriteSheet(ASSET_WALK_SIDE);
    m_TexUp   = m_Renderer->LoadSpriteSheet(ASSET_WALK_UP);

    m_PosX      = WINDOW_W / 2.0f;
    m_PosY      = WINDOW_H / 2.0f;
    m_Direction = Direction::Down;
    m_AnimFrame = 0;
    m_AnimTime  = 0.0f;
}

void PlayingState::OnExit()
{
    m_Renderer.reset();
}

void PlayingState::Update(float deltaTime)
{
    if (!m_Renderer)
        return;

    // If the player closed the window, return to the main menu.
    if (m_Renderer->ShouldClose())
    {
        GameManager::GetInstance().PopState();
        return;
    }

    if (m_Renderer->IsKeyPressed(RaylibKey::Escape))
    {
        GameManager::GetInstance().PopState();
        return;
    }

    // --- Movement ---
    float dx = 0.0f, dy = 0.0f;

    if (m_Renderer->IsKeyDown(RaylibKey::Up))    dy -= 1.0f;
    if (m_Renderer->IsKeyDown(RaylibKey::Down))  dy += 1.0f;
    if (m_Renderer->IsKeyDown(RaylibKey::Left))  dx -= 1.0f;
    if (m_Renderer->IsKeyDown(RaylibKey::Right)) dx += 1.0f;

    // Normalise the movement vector so diagonal speed equals cardinal speed.
    const float len = std::sqrt(dx * dx + dy * dy);
    if (len > 0.0f)
    {
        dx /= len;
        dy /= len;
    }

    m_PosX += dx * MOVE_SPEED * deltaTime;
    m_PosY += dy * MOVE_SPEED * deltaTime;

    // Update facing direction from the final movement vector.
    // Prefer horizontal when both axes are equally pressed.
    if (len > 0.0f)
    {
        if (std::abs(dx) >= std::abs(dy))
            m_Direction = (dx > 0.0f) ? Direction::Right : Direction::Left;
        else
            m_Direction = (dy > 0.0f) ? Direction::Down : Direction::Up;
    }

    // Clamp so the sprite stays fully inside the window.
    const float half = DISPLAY_SIZE / 2.0f;
    m_PosX = std::max(half, std::min(static_cast<float>(WINDOW_W) - half, m_PosX));
    m_PosY = std::max(half, std::min(static_cast<float>(WINDOW_H) - half, m_PosY));

    // --- Animation ---
    const bool moving = (len > 0.0f);
    if (moving)
    {
        m_AnimTime += deltaTime;
        if (m_AnimTime >= FRAME_DURATION)
        {
            m_AnimTime -= FRAME_DURATION;
            m_AnimFrame = (m_AnimFrame + 1) % FRAME_COUNT;
        }
    }
    else
    {
        // Stand still at frame 0.
        m_AnimFrame = 0;
        m_AnimTime  = 0.0f;
    }
}

void PlayingState::Render()
{
    if (!m_Renderer)
        return;

    m_Renderer->Clear(RenderColor::Black());

    // Select the sprite sheet and whether to mirror it.
    int  texId = m_TexDown;
    bool flipX = false;

    switch (m_Direction)
    {
    case Direction::Up:    texId = m_TexUp;               break;
    case Direction::Left:  texId = m_TexSide; flipX = true; break;
    case Direction::Right: texId = m_TexSide;              break;
    default: /* Down */    texId = m_TexDown;             break;
    }

    // Draw the sprite centred on the player position.
    const int drawX = static_cast<int>(m_PosX) - DISPLAY_SIZE / 2;
    const int drawY = static_cast<int>(m_PosY) - DISPLAY_SIZE / 2;

    m_Renderer->DrawSprite(texId,
                           FRAME_W, FRAME_H,
                           m_AnimFrame, 0,
                           drawX, drawY,
                           DISPLAY_SIZE, DISPLAY_SIZE,
                           flipX);

    m_Renderer->Present();
}
