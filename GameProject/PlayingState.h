#pragma once
#include "GameState.h"
#include <memory>
#include <vector>

class RaylibRenderer;

// Movement direction used to select the correct Body_A walk sprite sheet.
enum class Direction { Down, Up, Left, Right };

// A single pine tree instance placed in the world.
struct TreeInstance
{
    float x;           // world-space centre X
    float y;           // world-space centre Y
    int   variantIdx;  // which pine tree color variant to draw (0–4)
};

class PlayingState : public GameState
{
public:
PlayingState();
~PlayingState() override;
void OnEnter()  override;
void OnExit()   override;
void Update(float deltaTime) override;
void Render()   override;

GameStateType GetType() const override { return GameStateType::Playing; }
    std::string   GetName() const override { return "Playing"; }

private:
    // RayLib renderer – created on OnEnter, destroyed on OnExit.
    std::unique_ptr<RaylibRenderer> m_Renderer;

    // Player world position in window pixels.
    float m_PosX = 0.0f;
    float m_PosY = 0.0f;

    // Current facing direction (selects the sprite sheet + optional flip).
    Direction m_Direction = Direction::Down;

    // Walk-animation state.
    int   m_AnimFrame = 0;
    float m_AnimTime  = 0.0f;

    // Loaded sprite-sheet slot indices (set by OnEnter).
    int m_TexDown  = -1;
    int m_TexSide  = -1;
    int m_TexUp    = -1;
    int m_TexGrass = -1;  // TileSet.png grass tiles

    // Camera world-space position (top-left corner of the visible area).
    float m_CamX = 0.0f;
    float m_CamY = 0.0f;

    // Sprite-sheet constants.
    static constexpr int   FRAME_W        = 64;   // pixels per frame
    static constexpr int   FRAME_H        = 64;
    static constexpr int   FRAME_COUNT    = 6;    // frames in each walk sheet
    static constexpr float FRAME_DURATION = 0.10f; // seconds per frame (~10 fps)

    // Rendering constants.
    static constexpr int   WINDOW_W       = 800;
    static constexpr int   WINDOW_H       = 600;
    static constexpr int   DISPLAY_SIZE   = 192;  // draw sprite at 3× scale
    static constexpr float MOVE_SPEED               = 200.0f; // pixels per second
    static constexpr float PLAYER_COLLISION_RADIUS  = 30.0f;  // world-space circle radius for player
    static constexpr float TREE_COLLISION_RADIUS    = 36.0f;  // world-space circle radius for trees

    // Map / tile constants.
    static constexpr int MAP_W         = WINDOW_W * 2;  // 4× window area (2× each dimension)
    static constexpr int MAP_H         = WINDOW_H * 2;
    static constexpr int TILE_SRC_SIZE = 16;  // source pixels per tile in TileSet.png
    static constexpr int TILE_DST_SIZE = 32;  // on-screen pixels per tile (2× scale)

    // Pine tree sprite constants.
    // Each PineTree sheet is 1024×128 px → 8 frames of 128×128 across one row.
    static constexpr int   TREE_VARIANT_COUNT  = 5;    // WarmColor, CoolColor, Autumn, PreAutumn, Winter
    static constexpr int   TREE_COUNT          = 100;   // trees spawned per game-level start
    static constexpr int   TREE_FRAME_W        = 128;  // source pixels per frame (width)
    static constexpr int   TREE_FRAME_H        = 128;  // source pixels per frame (height)
    static constexpr int   TREE_FRAME_COUNT    = 8;    // 1024 / 128
    static constexpr float TREE_FRAME_DURATION = 0.15f; // seconds per frame
    static constexpr int   TREE_DISPLAY_SIZE   = 128;  // on-screen size in pixels (1:1 scale)

    // Loaded pine tree variant textures (indexed 0–4: WarmColor, CoolColor, Autumn, PreAutumn, Winter).
    int m_TexTrees[TREE_VARIANT_COUNT] = { -1, -1, -1, -1, -1 };

    // Randomly distributed pine tree instances – regenerated every OnEnter().
    std::vector<TreeInstance> m_Trees;

    // Shared pine tree animation state.
    int   m_TreeAnimFrame = 0;
    float m_TreeAnimTime  = 0.0f;
};
