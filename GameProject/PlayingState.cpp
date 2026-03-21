#include "PlayingState.h"
#include "RaylibRenderer.h"
#include "GameManager.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

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

// Body_A slice animation sheets.
static constexpr const char* ASSET_SLICE_DOWN =
    "VisualAssets/Pixel Crawler - Free Pack/Entities/Characters/Body_A/Animations/Slice_Base/Slice_Down-Sheet.png";
static constexpr const char* ASSET_SLICE_SIDE =
    "VisualAssets/Pixel Crawler - Free Pack/Entities/Characters/Body_A/Animations/Slice_Base/Slice_Side-Sheet.png";
static constexpr const char* ASSET_SLICE_UP =
    "VisualAssets/Pixel Crawler - Free Pack/Entities/Characters/Body_A/Animations/Slice_Base/Slice_Up-Sheet.png";

// Grass tileset from the EnvironmentPack Decorations folder.
static constexpr const char* ASSET_TILESET =
    "VisualAssets/EnvironmentPack/EnvironmentPack/Decorations/TileSet.png";

// Animated pine tree sprite sheets (from EnvironmentPack/AnimatedTrees/PineTree).
// Each sheet is 1024×128 px containing 8 frames of 128×128.
static constexpr const char* ASSET_PINE_TREES[5] = {
    "VisualAssets/EnvironmentPack/EnvironmentPack/AnimatedTrees/PineTree/WarmColor.png",
    "VisualAssets/EnvironmentPack/EnvironmentPack/AnimatedTrees/PineTree/PineTreeCoolColor.png",
    "VisualAssets/EnvironmentPack/EnvironmentPack/AnimatedTrees/PineTree/Autumn.png",
    "VisualAssets/EnvironmentPack/EnvironmentPack/AnimatedTrees/PineTree/PreAutumn.png",
    "VisualAssets/EnvironmentPack/EnvironmentPack/AnimatedTrees/PineTree/Winter.png",
};

void PlayingState::OnEnter()
{
    m_Renderer = std::make_unique<RaylibRenderer>();
    m_Renderer->Initialize(WINDOW_W, WINDOW_H, "Game Screen");

    m_TexDown  = m_Renderer->LoadSpriteSheet(ASSET_WALK_DOWN);
    m_TexSide  = m_Renderer->LoadSpriteSheet(ASSET_WALK_SIDE);
    m_TexUp    = m_Renderer->LoadSpriteSheet(ASSET_WALK_UP);
    m_TexGrass = m_Renderer->LoadSpriteSheet(ASSET_TILESET);

    m_TexSliceDown = m_Renderer->LoadSpriteSheet(ASSET_SLICE_DOWN);
    m_TexSliceSide = m_Renderer->LoadSpriteSheet(ASSET_SLICE_SIDE);
    m_TexSliceUp   = m_Renderer->LoadSpriteSheet(ASSET_SLICE_UP);

    // Load the animated pine tree variant textures.
    for (int i = 0; i < TREE_VARIANT_COUNT; ++i)
        m_TexTrees[i] = m_Renderer->LoadSpriteSheet(ASSET_PINE_TREES[i]);

    // Generate 30 randomly distributed pine trees for this game-level start.
    // XOR a std::random_device value with a steady-clock timestamp so the seed
    // is different every entry even on platforms where random_device has low
    // entropy or returns a fixed value.
    const auto timeSeed = static_cast<std::uint32_t>(
        std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937 rng{ std::random_device{}() ^ timeSeed };
    const float margin = TREE_DISPLAY_SIZE / 2.0f;
    std::uniform_real_distribution<float> distX(margin, static_cast<float>(MAP_W) - margin);
    std::uniform_real_distribution<float> distY(margin, static_cast<float>(MAP_H) - margin);
    std::uniform_int_distribution<int>    distVariant(0, TREE_VARIANT_COUNT - 1);

    m_Trees.clear();
    m_Trees.reserve(TREE_COUNT);
    for (int i = 0; i < TREE_COUNT; ++i)
        m_Trees.push_back({ distX(rng), distY(rng), distVariant(rng) });

    m_TreeAnimFrame = 0;
    m_TreeAnimTime  = 0.0f;

    // Place the player at the centre of the world map.
    m_PosX      = MAP_W / 2.0f;
    m_PosY      = MAP_H / 2.0f;
    m_Direction  = Direction::Down;
    m_AnimFrame  = 0;
    m_AnimTime   = 0.0f;
    m_IsSlicing  = false;
    m_SliceFrame = 0;
    m_SliceTime  = 0.0f;

    // Initialise the camera so the player starts centred on screen.
    m_CamX = std::max(0.0f, std::min(static_cast<float>(MAP_W - WINDOW_W),
                                     m_PosX - WINDOW_W / 2.0f));
    m_CamY = std::max(0.0f, std::min(static_cast<float>(MAP_H - WINDOW_H),
                                     m_PosY - WINDOW_H / 2.0f));
}

void PlayingState::OnExit()
{
    m_Trees.clear();
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

    // --- Slice animation (takes priority over movement) ---
    if (m_IsSlicing)
    {
        m_SliceTime += deltaTime;
        if (m_SliceTime >= SLICE_FRAME_DURATION)
        {
            m_SliceTime -= SLICE_FRAME_DURATION;
            ++m_SliceFrame;
            if (m_SliceFrame >= SLICE_FRAME_COUNT)
            {
                // Animation complete – return to idle.
                m_IsSlicing  = false;
                m_SliceFrame = 0;
                m_SliceTime  = 0.0f;
            }
        }
        // Block movement and walk animation while slicing.
        m_AnimFrame = 0;
        m_AnimTime  = 0.0f;

        // Still advance tree animation.
        m_TreeAnimTime += deltaTime;
        if (m_TreeAnimTime >= TREE_FRAME_DURATION)
        {
            m_TreeAnimTime -= TREE_FRAME_DURATION;
            m_TreeAnimFrame = (m_TreeAnimFrame + 1) % TREE_FRAME_COUNT;
        }
        return;
    }

    // --- Trigger slice on spacebar press ---
    if (m_Renderer->IsKeyPressed(RaylibKey::Space))
    {
        m_IsSlicing  = true;
        m_SliceFrame = 0;
        m_SliceTime  = 0.0f;
        m_AnimFrame  = 0;
        m_AnimTime   = 0.0f;
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

    // --- Walk animation ---
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

    // --- Pine tree animation (runs continuously, independent of player movement) ---
    m_TreeAnimTime += deltaTime;
    if (m_TreeAnimTime >= TREE_FRAME_DURATION)
    {
        m_TreeAnimTime -= TREE_FRAME_DURATION;
        m_TreeAnimFrame = (m_TreeAnimFrame + 1) % TREE_FRAME_COUNT;
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

    // --- Y-sorted draw list (trees + player) ---
    // Each sprite is sorted by the world-Y of its base (feet/root).
    // Lower base Y = further "back" in the scene = drawn first (behind).
    struct DrawCall
    {
        float sortY;                          // world-space base Y for depth sort
        int   texId;
        int   frameW, frameH, frameCol;
        int   screenX, screenY;
        int   destW, destH;
        bool  flipX;
    };

    std::vector<DrawCall> drawList;
    drawList.reserve(static_cast<int>(m_Trees.size()) + 1);

    // Add visible trees.
    for (const auto& tree : m_Trees)
    {
        const int treeTexId = m_TexTrees[tree.variantIdx];
        if (treeTexId < 0)
            continue;

        const int sx = static_cast<int>(tree.x - m_CamX) - TREE_DISPLAY_SIZE / 2;
        const int sy = static_cast<int>(tree.y - m_CamY) - TREE_DISPLAY_SIZE / 2;

        if (sx + TREE_DISPLAY_SIZE < 0 || sx >= WINDOW_W)
            continue;
        if (sy + TREE_DISPLAY_SIZE < 0 || sy >= WINDOW_H)
            continue;

        drawList.push_back({ tree.y,
                             treeTexId,
                             TREE_FRAME_W, TREE_FRAME_H, m_TreeAnimFrame,
                             sx, sy,
                             TREE_DISPLAY_SIZE, TREE_DISPLAY_SIZE,
                             false });
    }

    // Add player.
    {
        int  texId = m_TexDown;
        bool flipX = false;
        int  frame = m_AnimFrame;

        if (m_IsSlicing)
        {
            frame = m_SliceFrame;
            switch (m_Direction)
            {
            case Direction::Up:    texId = m_TexSliceUp;                  break;
            case Direction::Left:  texId = m_TexSliceSide; flipX = true;  break;
            case Direction::Right: texId = m_TexSliceSide;                break;
            default: /* Down */    texId = m_TexSliceDown;                break;
            }
        }
        else
        {
            switch (m_Direction)
            {
            case Direction::Up:    texId = m_TexUp;                 break;
            case Direction::Left:  texId = m_TexSide; flipX = true; break;
            case Direction::Right: texId = m_TexSide;               break;
            default: /* Down */    texId = m_TexDown;               break;
            }
        }

        const int drawX = static_cast<int>(m_PosX - m_CamX) - DISPLAY_SIZE / 2;
        const int drawY = static_cast<int>(m_PosY - m_CamY) - DISPLAY_SIZE / 2;

        drawList.push_back({ m_PosY,
                             texId,
                             FRAME_W, FRAME_H, frame,
                             drawX, drawY,
                             DISPLAY_SIZE, DISPLAY_SIZE,
                             flipX });
    }

    // Sort ascending by base Y — sprites with a lower base are drawn first (behind).
    std::sort(drawList.begin(), drawList.end(),
              [](const DrawCall& a, const DrawCall& b) { return a.sortY < b.sortY; });

    for (const auto& dc : drawList)
    {
        m_Renderer->DrawSprite(dc.texId,
                               dc.frameW, dc.frameH,
                               dc.frameCol, 0,
                               dc.screenX, dc.screenY,
                               dc.destW, dc.destH,
                               dc.flipX);
    }

    m_Renderer->Present();
}
