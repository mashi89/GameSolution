#pragma once
#include "GameState.h"
#include <memory>

class RaylibRenderer;

// Movement direction used to select the correct Body_A walk sprite sheet.
enum class Direction { Down, Up, Left, Right };

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
    int m_TexDown = -1;
    int m_TexSide = -1;
    int m_TexUp   = -1;

    // Sprite-sheet constants.
    static constexpr int   FRAME_W        = 64;   // pixels per frame
    static constexpr int   FRAME_H        = 64;
    static constexpr int   FRAME_COUNT    = 6;    // frames in each walk sheet
    static constexpr float FRAME_DURATION = 0.10f; // seconds per frame (~10 fps)

    // Rendering constants.
    static constexpr int   WINDOW_W       = 800;
    static constexpr int   WINDOW_H       = 600;
    static constexpr int   DISPLAY_SIZE   = 192;  // draw sprite at 3× scale
    static constexpr float MOVE_SPEED     = 200.0f; // pixels per second
};
