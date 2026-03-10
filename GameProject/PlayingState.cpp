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

// Grass tileset from the EnvironmentPack Decorations folder.
static constexpr const char* ASSET_TILESET =
    "VisualAssets/EnvironmentPack/EnvironmentPack/Decorations/TileSet.png";

void PlayingState::OnEnter()
{
    m_Renderer = std::make_unique<RaylibRenderer>();
    m_Renderer->Initialize(WINDOW_W, WINDOW_H, "Game Screen");

    m_TexDown  = m_Renderer->LoadSpriteSheet(ASSET_WALK_DOWN);
    m_TexSide  = m_Renderer->LoadSpriteSheet(ASSET_WALK_SIDE);
    m_TexUp    = m_Renderer->LoadSpriteSheet(ASSET_WALK_UP);
    m_TexGrass = m_Renderer->LoadSpriteSheet(ASSET_TILESET);

    // Place the player at the centre of the world map.
    m_PosX      = MAP_W / 2.0f;
    m_PosY      = MAP_H / 2.0f;
    m_Direction = Direction::Down;
    m_AnimFrame = 0;
    m_AnimTime  = 0.0f;

    // Initialise the camera so the player starts centred on screen.
    m_CamX = std::max(0.0f, std::min(static_cast<float>(MAP_W - WINDOW_W),
                                     m_PosX - WINDOW_W / 2.0f));
    m_CamY = std::max(0.0f, std::min(static_cast<float>(MAP_H - WINDOW_H),
                                     m_PosY - WINDOW_H / 2.0f));
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

    // Clamp so the sprite stays fully inside the world map.
    const float half = DISPLAY_SIZE / 2.0f;
    m_PosX = std::max(half, std::min(static_cast<float>(MAP_W) - half, m_PosX));
    m_PosY = std::max(half, std::min(static_cast<float>(MAP_H) - half, m_PosY));

    // --- Camera: follow the player, clamped to map edges ---
    m_CamX = m_PosX - WINDOW_W / 2.0f;
    m_CamY = m_PosY - WINDOW_H / 2.0f;
    m_CamX = std::max(0.0f, std::min(static_cast<float>(MAP_W - WINDOW_W), m_CamX));
    m_CamY = std::max(0.0f, std::min(static_cast<float>(MAP_H - WINDOW_H), m_CamY));

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

    // Dark-green clear colour used as the fallback when the tileset fails to load.
    m_Renderer->Clear(RenderColor(0.13f, 0.42f, 0.13f));

    // --- Draw tiled grass background ---
    if (m_TexGrass >= 0)
    {
        const int camLeft = static_cast<int>(m_CamX);
        const int camTop  = static_cast<int>(m_CamY);

        // First tile index (in map-tile coordinates) that is at least partially visible.
        const int tileStartX = camLeft / TILE_DST_SIZE;
        const int tileStartY = camTop  / TILE_DST_SIZE;

        // Number of tiles required to fill the window (add 2 for partial overlap).
        const int tilesAcross = WINDOW_W / TILE_DST_SIZE + 2;
        const int tilesDown   = WINDOW_H / TILE_DST_SIZE + 2;

        for (int ty = tileStartY; ty < tileStartY + tilesDown; ++ty)
        {
            for (int tx = tileStartX; tx < tileStartX + tilesAcross; ++tx)
            {
                // Skip tiles that start outside or extend fully beyond the map.
                const int worldX = tx * TILE_DST_SIZE;
                const int worldY = ty * TILE_DST_SIZE;
                if (worldX < 0 || worldY < 0 ||
                    worldX + TILE_DST_SIZE > MAP_W || worldY + TILE_DST_SIZE > MAP_H)
                    continue;

                // Convert world position to screen position.
                const int screenX = worldX - camLeft;
                const int screenY = worldY - camTop;

                // Alternate between two grass tile columns for a natural look.
                // Both are from the solid-green row (row 1) of the tileset.
                const int tileCol = (tx + ty) % 2;
                const int tileRow = 1;

                m_Renderer->DrawSprite(m_TexGrass,
                                       TILE_SRC_SIZE, TILE_SRC_SIZE,
                                       tileCol, tileRow,
                                       screenX, screenY,
                                       TILE_DST_SIZE, TILE_DST_SIZE);
            }
        }
    }

    // --- Draw player sprite ---
    int  texId = m_TexDown;
    bool flipX = false;

    switch (m_Direction)
    {
    case Direction::Up:    texId = m_TexUp;                break;
    case Direction::Left:  texId = m_TexSide; flipX = true; break;
    case Direction::Right: texId = m_TexSide;              break;
    default: /* Down */    texId = m_TexDown;              break;
    }

    // Convert world position to screen position.
    const int drawX = static_cast<int>(m_PosX - m_CamX) - DISPLAY_SIZE / 2;
    const int drawY = static_cast<int>(m_PosY - m_CamY) - DISPLAY_SIZE / 2;

    m_Renderer->DrawSprite(texId,
                           FRAME_W, FRAME_H,
                           m_AnimFrame, 0,
                           drawX, drawY,
                           DISPLAY_SIZE, DISPLAY_SIZE,
                           flipX);

    m_Renderer->Present();
}
