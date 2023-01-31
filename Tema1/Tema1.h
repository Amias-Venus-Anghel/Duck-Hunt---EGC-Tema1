#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void ResetGame();
        bool Reflect();
        void Draw();

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat3 modelMatrix;
        glm::mat3 modelMatrixGen; // matrix with general operations aplicable to all parts of bird

        float translateX, translateY;
        float angularStep; // rotation for wings
        float rotationDir; // rotation direction of wings
        float angularStepBody; // rotation for the whole bird, gives angle for direction too

        glm::vec3 limits; // flying limits for the bird
        glm::vec3 initialPoz; //  random initial position of bird
        float speed, flySpeed, fallSpeed; // bird speeds
        
        const float PI = 3.1415926f; // constant used for angle values

        bool newBird; // for reseting the bird with new random initial position and angle
        int birdState; // 0 = flying, 1 = escape, 2 = dead
        float time; // time a bird has been on the screen
        int bullets; // nr of bullets left
        int lifes; // nr of lifes left
        float clickZone_R1; // radius of the circle collider zone for shoting

        float score; // actual score , increments by 0.2 per bird for a total of 50 birds
        bool lose, win; // used to show win or lose screen

        bool showCollisionBox; // used to change viewing of the collision zone
    };
}   // namespace m1
