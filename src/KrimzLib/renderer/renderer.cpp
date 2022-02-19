#include "KrimzLib/renderer/renderer.h"


// Default constant buffers
struct DEF_VS_CB {
	kl::mat4 w;
	kl::mat4 vp;
};
struct DEF_PS_CB {
	kl::vec4 ambient;
	kl::vec4 dirCol;
	kl::vec4 dirDir;
};

// Constructor
kl::renderer::renderer() {
	setup = []() {};
	input = [](kl::keys* k, kl::mouse* m) {};
	update = []() {};
}

// Creates and runs a new engine
void kl::renderer::startNew(const kl::ivec2& frameSize) {
	// Window start definition
	win.start = [&]() {
		// Setting up the lighing
		ambient.color = kl::colors::white;
		ambient.intensity = 0.1f;
		sun.color = kl::colors::white;
		sun.intensity = 1.0f;
		sun.direction = kl::vec3(0.0f, -1.0f, -2.0f);

		// Creating the gpu
		gpu = new kl::gpu(win.getHWND());

		// Creating the rasters
		kl::raster* solid_ra = gpu->newRaster(false, true);
		solid_ra->bind();

		// Compiling shaders
		default_sh = gpu->newShaders("res/shaders/renderer.hlsl", sizeof(DEF_VS_CB), sizeof(DEF_PS_CB));

		// Sampler setup
		kl::sampler* samp = gpu->newSampler(true, true);
		samp->bind(0);
		gpu->delSampler(samp);

		// Calling the user start
		setup();
	};

	// Window update definition
	win.update = [&]() {
		// Time calculations
		deltaT = timer.interval();
		elapsedT = timer.elapsed();

		// User input
		input(&win.keys, &win.mouse);

		// Physics update
		for (int i = 0; i < entities.size(); i++) {
			entities[i]->upPhys(deltaT);
		}

		// User update
		update();

		// Setting the lighting data
		DEF_PS_CB def_pixl_data = {};
		def_pixl_data.ambient = ambient.getCol();
		def_pixl_data.dirCol = sun.getCol();
		def_pixl_data.dirDir = sun.getDir();
		default_sh->setPixlData(&def_pixl_data);

		// Clearing the frame buffer
		gpu->clear(background);

		// Rendering skybox
		if (skybox) {
			gpu->setDepthTest(false);
			skybox->render(camera.matrix());
			gpu->setDepthTest(true);
		}

		// Setting the camera data
		DEF_VS_CB def_vert_data = {};
		def_vert_data.vp = camera.matrix();

		// Rendering objects
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->visible) {
				// Setting the world matrix
				def_vert_data.w = entities[i]->matrix();

				// Updating the cb data
				default_sh->setVertData(&def_vert_data);

				// Rendering the object
				entities[i]->render();
			}
		}

		// Swapping the frame buffers
		gpu->swap(vSync);

		// Updating the fps display
		win.setTitle(std::to_string(int(1 / deltaT)));
	};

	// Window end definition
	win.end = [&]() {
		// Deleting entities
		entities.clear();

		// Deleting skyboxes
		skyboxes.clear();

		// Deleting the gpu
		delete gpu;
	};

	// Timer reset
	timer.interval();
	timer.reset();

	// Window creation
	win.startNew(frameSize, kl::random::STRING(6), false, true);
}
void kl::renderer::stop() const {
	win.stop();
}

// Sets the fullscreen mode
void kl::renderer::setFullscreen(bool enable) {
	win.setFullscreen(enable);
}

// Returns the frame size
kl::ivec2 kl::renderer::frameSize() const {
	return win.getSize();
}

// Returns the frame center
kl::ivec2 kl::renderer::frameCenter() const {
	return win.getCenter();
}

// Returns the aspect ratio
float kl::renderer::getAspect() const {
	return win.getAspect();
}

// Mesh
kl::mesh* kl::renderer::newMesh(const std::vector<kl::vertex>& vertices) {
	return gpu->newMesh(vertices);
}
kl::mesh* kl::renderer::newMesh(const std::string& filePath, bool flipZ) {
	return gpu->newMesh(filePath, flipZ);
}
bool kl::renderer::delMesh(kl::mesh* mes) {
	return gpu->delMesh(mes);
}

// Texture
kl::texture* kl::renderer::newTexture(const kl::image& img) {
	return gpu->newTexture(img);
}
bool kl::renderer::delTexture(kl::texture* tex) {
	return gpu->delTexture(tex);
}

// Entity
kl::entity* kl::renderer::newEntity(const std::string& name, kl::mesh* mes, kl::texture* tex) {
	return entities.newInst(new kl::entity(name, mes, tex));
}
bool kl::renderer::delEntity(kl::entity* ent) {
	return entities.delInst(ent);
}

// Skybox
kl::skybox* kl::renderer::newSkybox(const std::string& name, const kl::image& fullBox) {
	return skyboxes.newInst(new kl::skybox(gpu->getDev(), gpu->getCon(), name, fullBox));
}
kl::skybox* kl::renderer::newSkybox(const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) {
	return skyboxes.newInst(new kl::skybox(gpu->getDev(), gpu->getCon(), name, front, back, left, right, top, bottom));
}
bool kl::renderer::delSkybox(kl::skybox* sky) {
	return skyboxes.delInst(sky);
}
