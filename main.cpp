#include "random"


#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


Vector3 genRandomDimensions();
float distanceV3D(Vector3 P1, Vector3 P2);
void anglesBoundsChecking(float * phi, float * theta);

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<int> uni(0, 100);


int main() {

    // initialise window
    int screen_width = 1000;
    int screen_height = 800;
    InitWindow(screen_width, screen_height, "boxes!");

    // Window flags
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // set up camera
    Camera camera = { 0 };
    camera.position = (Vector3){0.0f, 0.0f, 80.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;



    float distance = distanceV3D({0.0f, 0.0f, 0.0f}, camera.position);

    float phi = acos(camera.position.y / distance);
    float theta = acos(camera.position.z / distance * sin(phi));

    if (theta == 0.0f) theta += 0.000001;
    if (phi == 0.0f) theta += 0.000001;

    float change_factor = 0.005f;
    float zoom_factor = 10.0f;

    printf("original calculation of phi in degrees: %f, theta: %f\n", phi, theta);


    // set up cube
    Vector3 cube_position = { 0.0f, 0.0f, 0.0f };
    Vector3 cube_dimensions = { 30.0f, 30.0f, 30.0f };


    // button and slider spacing
    float SPACING = 10.0f;
    float WIDTH = 200.0f;
    float BUTTON_HEIGHT = 30.0f;
    float SLIDER_HEIGHT = 20.0f;



    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // update camera
        Vector2 mouse_delta = GetMouseDelta();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (mouse_delta.y) {
                phi += -(change_factor * mouse_delta.y);

                if (phi < 0.0f) phi = 0.000001f;
                if (phi > PI) phi = PI - 0.000001f;


                printf("phi: %f\n", phi);
            }

            if (mouse_delta.x) {
                theta += -(change_factor * mouse_delta.x);
            }
        }

        float mouse_wheel_move = GetMouseWheelMove();
        if (mouse_wheel_move) {
            if (mouse_wheel_move < 0.0f) {
                distance += zoom_factor;
            } else {
                distance -= zoom_factor;
            }

            // update distance and camera position

        }


        printf("mouse_wheel_move: %f\n", mouse_wheel_move);



        camera.position.x = distance * sin(theta) * sin(phi);
        camera.position.y = distance * cos(phi);
        camera.position.z = distance * cos(theta) * sin(phi);

        BeginMode3D(camera);
            DrawGrid(50, 5);

            DrawCubeV(cube_position, cube_dimensions, MAROON);
            DrawCubeWiresV(cube_position, cube_dimensions, BLACK);

        EndMode3D();


        float x_ui_anchor = (float)GetScreenWidth() - SPACING - WIDTH;
        // buttons
        Rectangle btn_reset_bounds = {x_ui_anchor, SPACING, WIDTH, BUTTON_HEIGHT};
        Rectangle btn_random_bounds = {x_ui_anchor, (2 * SPACING) + (1 * BUTTON_HEIGHT), WIDTH, BUTTON_HEIGHT};

        bool btn_reset = GuiButton(btn_reset_bounds, "RESET");
        bool btn_random = GuiButton(btn_random_bounds, "RANDOM");

        if (btn_reset) { cube_dimensions = { 30.0f, 30.0f, 30.0f }; }
        if (btn_random) { cube_dimensions = genRandomDimensions(); };

        float sliders_y_start = btn_random_bounds.y + BUTTON_HEIGHT + SPACING;

        // update sliders
        Rectangle slider_x_bounds = { x_ui_anchor, sliders_y_start, 200, 20};
        Rectangle slider_y_bounds = { x_ui_anchor, slider_x_bounds.y + SLIDER_HEIGHT + SPACING, 200, 20};
        Rectangle slider_z_bounds = { x_ui_anchor, slider_y_bounds.y + SLIDER_HEIGHT + SPACING, 200, 20};

        float slider_x_value = GuiSlider(slider_x_bounds, "x", "", cube_dimensions.x , 0.0f, 100.0f);
        float slider_y_value = GuiSlider(slider_y_bounds, "y", "", cube_dimensions.y, 0.0f, 100.0f);
        float slider_z_value = GuiSlider(slider_z_bounds, "z", "", cube_dimensions.z, 0.0f, 100.0f);

        // update cube dimensions
        cube_dimensions.x = slider_x_value;
        cube_dimensions.y = slider_y_value;
        cube_dimensions.z = slider_z_value;

        EndDrawing();
    }
}


// functions
Vector3 genRandomDimensions() {
    return {(float)uni(rng), (float)uni(rng), (float)uni(rng)};
}

float distanceV3D(Vector3 P1, Vector3 P2) {
    float distance = sqrt((
            ((P1.x - P2.x) * (P1.x - P2.x)) +
            ((P1.y - P2.y) * (P1.y - P2.y)) +
            ((P1.z - P2.z) * (P1.z - P2.z))
            ));

    return distance;
}

void anglesBoundsChecking(float * phi, float * theta) {
    printf("phi received by angle bounds check: %f\n", *phi);
    if (*phi > (PI * 2)) {
        *phi = 0.01f;
        printf("reset phi from 2pi to 0.01");
    }
    if (*phi == 0.0f) {
        *phi = 0.01f;
        printf("reset phi from 0 to 0.01");
    }

    if (*theta > (PI / 2)) {
        *theta = (PI / 2);
    }
}