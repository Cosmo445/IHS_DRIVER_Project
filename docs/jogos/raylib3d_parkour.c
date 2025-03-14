/*******************************************************************************************
*
*   raylib [core] example - 3d camera first person
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "rcamera.h"

#define MAX_COLUMNS 20

typedef struct player {
    Vector3 pos;
    BoundingBox box;
} Player;

bool hitsurface(Vector3 player, Vector3 platform, float width, float length);
void UpdatePlayer(Vector3 camera_pos, Player *player);
void MoveCamera(Camera *camera, char modif);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 
        .position = (Vector3){ 0.0f, 2.0f, 4.0f },    // Camera position
        .target = (Vector3){ 0.0f, 2.0f, 0.0f },      // Camera looking at point
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
        .fovy = 60.0f,                                // Camera field-of-view Y
        .projection = CAMERA_PERSPECTIVE              // Camera projection type
    };
    Player player;
    UpdatePlayer(camera.position, &player);
    
    int cameraMode = CAMERA_FIRST_PERSON;

    // Generates some random columns
    float heights[MAX_COLUMNS] = { 0 };
    Vector3 positions[MAX_COLUMNS] = { 0 };
    Vector3 positions2[MAX_COLUMNS];
    Color colors[MAX_COLUMNS] = { 0 };

    Vector3 platforms[MAX_COLUMNS];
    BoundingBox boxes[MAX_COLUMNS];

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
        positions2[i] = (Vector3){
            2.0f,//positions[i].x+1.0f,
            heights[i],//positions[i].y+heights[i]/2.0f,
            2.0f//positions[i].z+1.0f
        };
        platforms[i] = (Vector3){positions[i].x,positions[i].y+heights[i]/2,positions[i].z};
        boxes[i] = (BoundingBox){
            (Vector3){
                positions[i].x-1.0f,
                positions[i].y-heights[i]/2,
                positions[i].z-1.0f,
            },
            (Vector3){
                positions[i].x + 1.0f,
                positions[i].y + heights[i]/2,
                positions[i].z + 1.0f
            }
        };
        colors[i] = (Color){ GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255 };
    }
    
    Vector3 groundsrc = {};
    Vector2 groundvct = { 32.0f, 32.0f };

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    
    // move player over boxes
    //--------------------------------------------------------------------------------------
    bool hit = 1;
    while(hit) {
        // cube collisions
        camera.position.y += 0.01f;
        UpdatePlayer(camera.position, &player);
        hit = 0;
        for (int i = 0; i < MAX_COLUMNS; i++)
            if(CheckCollisionBoxes(player.box, boxes[i]))
                hit = 1;
    }
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // Basic camera and player updates
        MoveCamera(&camera, 1);
        UpdatePlayer(camera.position, &player);
        
        // Handle gravity
        float gravity = 0.05f;
        camera.position.y-=gravity;
        
        // platform collisions
        hit = 0;
        hit = hitsurface(camera.position, groundsrc, 32.0f, 32.0f );
        for (int i = 0; i < MAX_COLUMNS && !hit; i++)
            if(hitsurface(camera.position, platforms[i], 2.0f, 2.0f)) 
                hit = 1;
        
        if(hit)
        {
            camera.position.y+=gravity*1.001;
            if(IsKeyPressed(KEY_SPACE)){
                camera.position.y+=2.0f;
                camera.target.y+=2.0f;
            }
        }
        else
            camera.target.y -=gravity;
        
        // column collisions
        for (int i = 0; i < MAX_COLUMNS; i++)
            if(CheckCollisionBoxes(player.box, boxes[i]))
                MoveCamera(&camera, -1);



        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawPlane(groundsrc, groundvct, LIGHTGRAY); // Draw ground
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

                // Draw some cubes around
                for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    //DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeV(positions[i], positions2[i], colors[i]);
                    DrawCubeWiresV(positions[i], positions2[i], MAROON);
                }

                // Draw player cube
                
                DrawCube(player.pos, 0.5f, 0.5f, 0.5f, PURPLE);
                DrawCubeWires(camera.position, 0.5f, 0.5f, 0.5f, DARKPURPLE);

            EndMode3D();

            // Draw info boxes
            DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(5, 5, 330, 100, BLUE);

            DrawText("Camera controls:", 15, 15, 10, BLACK);
            DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
            DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
            DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
            DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
            DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

            DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(600, 5, 195, 100, BLUE);

            DrawText("Camera status:", 610, 15, 10, BLACK);
            DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
                                              (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
                                              (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
                                              (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
            DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
                                                    (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
            DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
            DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
            DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



bool hitsurface(Vector3 player, Vector3 platform, float width, float length) 
{
    return CheckCollisionBoxes(
        (BoundingBox){
            (Vector3){
                player.x-0.5f,
                player.y-0.5f,
                player.z-0.5f
            },
            (Vector3){
                player.x+0.5f,
                player.y+0.5f,
                player.z+0.5f
            }
        }, 
        (BoundingBox){
            (Vector3){
                platform.x-width/2,
                platform.y,
                platform.z-length/2,
            },
            (Vector3){
                platform.x+width/2,
                platform.y,
                platform.z+length/2,
            }
        }
    );
}

void UpdatePlayer(Vector3 camera_pos, Player *player)
{
    player->pos = camera_pos;
    player->box = (BoundingBox){
        (Vector3){
            camera_pos.x-0.5f,
            camera_pos.y-0.5f,
            camera_pos.z-0.5f
        }, 
        (Vector3){
            camera_pos.x+0.5f,
            camera_pos.y+0.5f,
            camera_pos.z+0.5f
        }
    };
}

void MoveCamera(Camera *camera, char modif) {
    // Camera PRO usage example (EXPERIMENTAL)
    // This new camera function allows custom movement/rotation values to be directly provided
    // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
    UpdateCameraPro(camera,
        (Vector3){
            ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -     // Move forward-backward
            (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f)*modif,    
            ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
            (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f)*modif,
            0.0f                                                // Move up-down
        },
        (Vector3){
            GetMouseDelta().x*0.05f,                            // Rotation: yaw
            GetMouseDelta().y*0.05f,                            // Rotation: pitch
            0.0f                                                // Rotation: roll
        },
         0.0f //GetMouseWheelMove()*2.0f                                // Move to target (zoom)
    );
}