#include "Renderer.h"
#include "UI.h"
#include "Timer.h"
#include "Files.h"

#define IMAGE_WIDTH 1600
#define IMAGE_HEIGHT 900   

int main()
{
	Renderer render = Renderer(IMAGE_WIDTH, IMAGE_HEIGHT);
    Scene scene = Scene("assets/scenes/testScene.scene", "assets/skyboxes/container_free_Ref.hdr");
    Camera camera = Camera(glm::vec3(0, 0, -3), glm::vec3(0, 0, -1.5), glm::vec3(0, 1, 0), 90.0f, (float)IMAGE_WIDTH / (float)IMAGE_HEIGHT);
    UI ui = UI(glm::ivec2(IMAGE_WIDTH, IMAGE_HEIGHT));

    Image* image;
    float FPS, diff;
    while (ui.IsRunning())
    {
        Timer timer = Timer(&diff, &FPS);
        image = render.Render(scene, camera);
        ui.Update(render, FPS, diff);
        camera.Recalculate(image->AspectRatio());
    }
}