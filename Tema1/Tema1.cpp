#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
 


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);


    // initializations

    limits.x = resolution.x;
    limits.y = resolution.y;

    showCollisionBox = false;

    ResetGame();
    
    /* bird shapes */
    glm::vec3 corner = glm::vec3(0, 0, 0);

    Mesh* body = object2D::CreateTriangle("body", corner, 80, 50, glm::vec3(0.6, 0.4, 0.2));
    AddMeshToList(body);
    Mesh* body2 = object2D::CreateTriangle("body2", corner, 80, 100, glm::vec3(0.6, 0.4, 0.2));
    AddMeshToList(body2);
    Mesh* body3 = object2D::CreateWing("body3", corner, 80, 50, glm::vec3(0.6, 0.4, 0.2));
    AddMeshToList(body3);

    Mesh* wing1 = object2D::CreateWing("wing1", corner, 100, 50, glm::vec3(0.6, 0.4, 0));
    AddMeshToList(wing1);
    Mesh* wing2 = object2D::CreateWing("wing2", corner, 100, 50, glm::vec3(0.6, 0.4, 0));
    AddMeshToList(wing2);

    Mesh* head = object2D::CreateCircle("head", 35, glm::vec3(1, 1, 0.9));
    AddMeshToList(head);
    Mesh* eye1 = object2D::CreateCircle("eye1", 5, glm::vec3(0, 0, 0));
    AddMeshToList(eye1);
    Mesh* eye2 = object2D::CreateCircle("eye2", 5, glm::vec3(0, 0, 0));
    AddMeshToList(eye2);

    Mesh* peak = object2D::CreateTriangle("peak", corner, 10, 20, glm::vec3(1, 1, 0));
    AddMeshToList(peak);

    /* other shapes */
    Mesh* ground = object2D::CreateSquare("ground", corner, 1000, glm::vec3(0.4, 1, 0), true);
    AddMeshToList(ground);

    Mesh* life1 = object2D::CreateHeart("life1", corner, 30, glm::vec3(1, 0, 0));
    AddMeshToList(life1);
    Mesh* life2 = object2D::CreateHeart("life2", corner, 30, glm::vec3(1, 0, 0));
    AddMeshToList(life2);
    Mesh* life3 = object2D::CreateHeart("life3", corner, 30, glm::vec3(1, 0, 0));
    AddMeshToList(life3);

    Mesh* bullet1 = object2D::CreateSquare("bullet1", corner, 20, glm::vec3(0, 1, 0), true);
    AddMeshToList(bullet1);
    Mesh* bullet2 = object2D::CreateSquare("bullet2", corner, 20, glm::vec3(0, 1, 0), true);
    AddMeshToList(bullet2);
    Mesh* bullet3 = object2D::CreateSquare("bullet3", corner, 20, glm::vec3(0, 1, 0), true);
    AddMeshToList(bullet3);

    Mesh* xpBar = object2D::CreateSquare("xpBar", corner, 30, glm::vec3(1, 1, 1), false);
    AddMeshToList(xpBar);
    Mesh* xpCount = object2D::CreateSquare("xpCount", corner, 30, glm::vec3(0.4, 0, 1), true);
    AddMeshToList(xpCount);

    /* click zone radius */
    clickZone_R1 = 130;
    
    Mesh* clickZone1 = object2D::CreateCircle("clickZone1", clickZone_R1, glm::vec3(1, 0, 0));
    AddMeshToList(clickZone1);    

    /* end game screen */
    Mesh* loseScreen = object2D::CreateSquare("loseScreen", corner, 1400, glm::vec3(1, 0, 0), true);
    AddMeshToList(loseScreen);
    Mesh* winScreen = object2D::CreateSquare("winScreen", corner, 1400, glm::vec3(0, 1, 0), true);
    AddMeshToList(winScreen);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::ResetGame() {
    /* resets the game variables for a new game */
    angularStep = 0;
    birdState = 0;
    rotationDir = -1;
    newBird = true;
    lifes = 3;
    score = 0;
    lose = false;
    win = false;

    /* dificulty dependence */
    speed = 40;
    flySpeed = 100;
    fallSpeed = 400;
}

bool Tema1::Reflect() {
    /* changes angle for reflexion of bird if it s the case */
    if (translateX > limits.x - initialPoz.x || translateX < -initialPoz.x) {
        if (angularStepBody > 0) {
            angularStepBody = PI - angularStepBody;
            return true;
        }
        else {
            angularStepBody = -PI - angularStepBody;
            return true;
        }
    }
    /* for refelxion on y take into consideration the green grass so bird does not fly under it */
    else if (translateY > limits.y - initialPoz.y || translateY < -initialPoz.y + 200 ) {
        angularStepBody = -angularStepBody;
        return true;
    }
}

void Tema1::Draw() {
    /* draws everything on the screen */
    glm::ivec2 resolution = window->GetResolution();

    /* end game screens shown only on eng game */
    if (lose) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["loseScreen"], shaders["VertexColor"], modelMatrix);
    }
    if (win) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["winScreen"], shaders["VertexColor"], modelMatrix);  
    }

    /* common transformations for bird */
    modelMatrixGen = glm::mat3(1);
    modelMatrixGen *= transform2D::Translate(translateX, translateY);
    modelMatrixGen *= transform2D::Translate(initialPoz.x, initialPoz.y); // random initial position
    modelMatrixGen *= transform2D::Rotate(angularStepBody);
    modelMatrixGen *= transform2D::Rotate(-PI / 2);

    /* ground */
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Scale(1.5, 0.2);
    RenderMesh2D(meshes["ground"], shaders["VertexColor"], modelMatrix);

    /* lifes */
    if (lifes > 0) {  
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(50, resolution.y - 40);
        RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);

        if (lifes > 1) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(100, resolution.y - 40);
            RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);

            if (lifes > 2) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(150, resolution.y - 40);
                RenderMesh2D(meshes["life3"], shaders["VertexColor"], modelMatrix);
            }
        }
    }
   
    /* bullets */
    if (bullets > 0) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(40, resolution.y - 70);
        modelMatrix *= transform2D::Scale(0.5, 1);
        RenderMesh2D(meshes["bullet3"], shaders["VertexColor"], modelMatrix);

        if (bullets > 1) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(90, resolution.y - 70);
            modelMatrix *= transform2D::Scale(0.5, 1);
            RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelMatrix);

            if (bullets > 2) { 
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(140, resolution.y - 70);
                modelMatrix *= transform2D::Scale(0.5, 1);
                RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    /* xp */
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(resolution.x - 300, resolution.y - 50);
    modelMatrix *= transform2D::Scale(10, 1);
    RenderMesh2D(meshes["xpBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(resolution.x - 300, resolution.y - 50);
    modelMatrix *= transform2D::Scale(score, 1);
    RenderMesh2D(meshes["xpCount"], shaders["VertexColor"], modelMatrix);

    /* head */
    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(-10, 15);
    RenderMesh2D(meshes["eye1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(10, 15);
    RenderMesh2D(meshes["eye2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

    /* peak */
    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(-5, 35);
    RenderMesh2D(meshes["peak"], shaders["VertexColor"], modelMatrix);

    /* body */
    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(-40, -50);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(40, -50);
    modelMatrix *= transform2D::Rotate(-PI);
    RenderMesh2D(meshes["body2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(-40, -190);
    RenderMesh2D(meshes["body3"], shaders["VertexColor"], modelMatrix);


    /* wing1 */
    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(20, -50);
    /* add rotaton to the wing */
    modelMatrix *= transform2D::Rotate(angularStep);
    RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelMatrix);

    /* wing2 */
    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixGen;
    modelMatrix *= transform2D::Translate(-120, -50);
    /* adds rotation to the wing relative to the point closer to the body */
    modelMatrix *= transform2D::Translate(100, 0);
    modelMatrix *= transform2D::Rotate(-1 * angularStep);
    modelMatrix *= transform2D::Translate(-100, 0);
    RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelMatrix);

    /* click zone - shown by pressing E, showes the collision zone */ 
    if (showCollisionBox) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= modelMatrixGen;
        modelMatrix *= transform2D::Translate(0, -70);
        RenderMesh2D(meshes["clickZone1"], shaders["VertexColor"], modelMatrix);
    }
    
}

void Tema1::Update(float deltaTimeSeconds)
{
    glClearColor(0, 0.4, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (birdState == 2) {
        /* dead bird */
        angularStepBody = -PI / 2;
        speed = fallSpeed;
    }

    if (birdState == 1) {
        /* escaping bird */
        angularStepBody = PI / 2;
        speed = fallSpeed;
    }

    if (newBird) {
        /* sets the bird, giving random start position adn angle, resets bullets and time */
        newBird = false;
        birdState = 0;
        time = 0;
        bullets = 3;

        angularStep = 0;

        /* every 5 dead birds increments the speed */
        if ((int)(score * 10) % 5 == 0 && score != 0) {
            flySpeed = 100 + 30 * score;
            cout << "increment speed " << flySpeed << endl;
        }

        speed = flySpeed;

        /* new random angle and position */
        angularStepBody = std::rand() % 3;
        /* check angle is not too close to vertical or horizontal movement */
        while (angularStepBody < 0.5 || angularStepBody > 3 ||
            (angularStepBody > PI / 2 - 0.2 && angularStepBody < PI / 2 + 0.2)) {
            angularStepBody = std::rand() % 3;
        }

        glm::ivec2 resolution = window->GetResolution();
        initialPoz.x = std::rand() % (resolution.x);
      
        /* check position is not too close to the limits */
        while (initialPoz.x < 100 || initialPoz.x > 1100) {
            initialPoz.x = std::rand() % (resolution.x);
        }


        initialPoz.y = 200;
        translateX = 0;
        translateY = 0;
    }


    /* distance and cos/sin are used to keep the direction of the duck while moving */
    translateX += speed * deltaTimeSeconds * cos(angularStepBody);
    translateY += speed * deltaTimeSeconds * sin(angularStepBody);

    if (birdState == 0) {

        /* if bird ia in normal movenet, check for reflexion */
        if (Reflect()) {
            /* if reflexion has been done recalculate translation */
            translateX += speed * deltaTimeSeconds * cos(angularStepBody);
            translateY += speed * deltaTimeSeconds * sin(angularStepBody);
        }
    }

    if (birdState != 2) {
        /* if bird still alive, move wings */
        /* change rotation of wings */
        if (angularStep > PI / 6 || angularStep < -PI / 3) {
            rotationDir *= -1;
        }
        angularStep += rotationDir * 0.5 * deltaTimeSeconds;
    }

    Draw();
    
    /* check if it's time for the bird to escape */
    time += 0.2 * deltaTimeSeconds;
    if (time > 3 && birdState != 2) {
        birdState = 1;
    }

    if (birdState == 1 && translateY > limits.y) {
        /* bird escaped */ 
        lifes--;
        if (lifes == 0) {
            lose = true;
            cout << "!!LOSE!!" << endl;
        }
        newBird = true;
    }
    if (birdState == 2 && translateY < -initialPoz.y) {
        /* dead bird fell down */
        newBird = true;
        score += 0.2;
        cout << "score is " << score << endl;

        if (score >= 10) {
            win = true;
            cout << "!!WIN!!" << endl;
        }
    }
      
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event

    /* toggles the collision area view */
    if (key == GLFW_KEY_E) {
        showCollisionBox = !showCollisionBox;
    }
   
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
  
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    /* translate position with -70 to be calculated from the center of the bird */
    float x = translateX + initialPoz.x - 70 * cos(angularStepBody);
    float y = translateY + initialPoz.y - 70 * sin(angularStepBody);

    glm::ivec2 resolution = window->GetResolution();

    /* checks if mouse has clicked in the collision zone (a circle around the bird) */
    if (bullets != 0 && birdState == 0 && mouseX <= x + clickZone_R1 && mouseX >= x - clickZone_R1 
        && mouseY <= resolution.y - y + clickZone_R1 && mouseY >= resolution.y - y - clickZone_R1) {
        cout << "HIT" << endl;
        birdState = 2;
    } 

    /* no matter if the bird was hit or not, a bullet is used */
    bullets--;

    /* if there are no bullets left and the bird wasn't hit, bird escapes */
    if (bullets == 0 && birdState != 2) {
        birdState = 1;
    }

    /* reset game */
    if (win || lose) {
        ResetGame();
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
