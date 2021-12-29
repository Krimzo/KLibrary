#include "KrimzLib.hpp"


// Window
kl::window engineWindow;

// Gpu
kl::dx::gpu* gpu = nullptr;

// Shaders
kl::dx::shaders* defSha = nullptr;

// Buffers
kl::dx::buffer* vertBuff = nullptr;

// Engine start function
void start() {
    // Gpu creation
    gpu = new kl::dx::gpu(engineWindow);

    // Loading and compiling shaders
    defSha = gpu->newShaders("res/shaders/dx/default.hlsl", "vShader", "pShader");

    // Binding shaders
    gpu->bindShaders(defSha);

    // Test data
    kl::vertex testVertices[3] = {
        kl::vertex(kl::vec3(-0.5, -0.5, 0), kl::vec2(), kl::vec3(1, 0, 0)),
        kl::vertex(kl::vec3(   0,  0.5, 0), kl::vec2(), kl::vec3(0, 1, 0)),
        kl::vertex(kl::vec3( 0.5, -0.5, 0), kl::vec2(), kl::vec3(0, 0, 1))
    };

    // Vertex buffer creation
    vertBuff = gpu->newBuffer(testVertices, sizeof(testVertices), D3D11_USAGE_DYNAMIC);

    // Assigning the vertex buffer
    gpu->bindVertBuff(vertBuff, sizeof(kl::vertex), 0);

    // Setting up the data parsing
    std::vector<kl::dx::layout> layouts {
        kl::dx::layout( "POSIN", 3),
        kl::dx::layout( "TEXIN", 2),
        kl::dx::layout("NORMIN", 3)
    };
    gpu->setLayouts(layouts);
}

// Engine update function
kl::time timer;
void update() {
    // Clearing the framebuffer
    gpu->clear(kl::constant::colors::gray);

    // Time calculations
    const float deltaT = timer.getElapsed();
    const float elapsedT = timer.stopwatchElapsed();

    // Rendering
    gpu->render(3);

    // Updating the title fps
    engineWindow.setTitle(std::to_string(int(1 / kl::time::staticGetElapsed())));

    // Swapping the buffers
    gpu->swap();
}

int main() {
    // Engine callback assigning
    engineWindow.start = start;
    engineWindow.update = update;

    // Engine creation
    timer.getElapsed();
    timer.stopwatchReset();
    engineWindow.startNew(kl::ivec2(1600, 900), "lol", false, true);

    // Cleanup
    delete vertBuff;
    delete defSha;
    delete gpu;

    // Exit
	return 0;
}
