#include "KrimzLib/renderer/renderer.h"


// Default vertex shader constant buffer
struct VS_CB {
	kl::mat4 w;
	kl::mat4 vp;
};

// Default pixel shader constant buffer
struct PS_CB {
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
		// Setting up the camera
		camera.nearPlane = 0.01f;
		camera.farPlane = 100.0f;
		camera.sens = 0.025f;

		// Setting up the lighing
		ambient.color = kl::colors::white;
		ambient.intensity = 0.1f;
		sun.color = kl::colors::white;
		sun.intensity = 1.0f;
		sun.direction = kl::vec3(0.0f, -1.0f, -2.0f);

		// Creating the gpu
		gpu = new kl::gpu(win.getHWND(), 4);

		// Creating the rasters
		solid_ra = gpu->newRaster(false, false, true);
		wire_ra = gpu->newRaster(true, false, true);
		solid_ra->bind();

		// Compiling shaders
		default_sh = gpu->newShaders("res/shaders/renderer.hlsl", sizeof(VS_CB), sizeof(PS_CB));

		// Sampler setup
		kl::sampler* samp = gpu->newSampler(true, true);
		samp->bind(0);
		delete samp;

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
		PS_CB pixl_data = {};
		pixl_data.ambient = ambient.getCol();
		pixl_data.dirCol = sun.getCol();
		pixl_data.dirDir = sun.getDir();
		default_sh->setPixlData(&pixl_data);

		// Clearing the frame buffer
		gpu->clear(background);

		// Rendering skybox
		if (skybox) {
			gpu->setDepthTest(false);
			skybox->render(camera.matrix());
			gpu->setDepthTest(true);
		}

		// Setting the camera data
		VS_CB vert_data = {};
		vert_data.vp = camera.matrix();

		// Rendering objects
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->visible) {
				// Setting the world matrix
				vert_data.w = entities[i]->matrix();

				// Updating the cb data
				default_sh->setVertData(&vert_data);

				// Rendering the object
				entities[i]->render();
			}
		}

		// Updating the fps display
		win.setTitle(std::to_string(int(1 / deltaT)));

		// Swapping the frame buffers
		gpu->swap(vSync);
	};

	// Window end definition
	win.end = [&]() {
		// Deleting skyboxes
		for (int i = 0; i < skyboxes.size(); i++) {
			delete skyboxes[i];
		}
		skyboxes.clear();

		// Deleting textures
		for (int i = 0; i < textures.size(); i++) {
			delete textures[i];
		}
		textures.clear();

		// Deleting meshes
		for (int i = 0; i < meshes.size(); i++) {
			delete meshes[i];
		}
		meshes.clear();

		// Deleting shaders
		delete default_sh;

		// Deleting rasters
		delete solid_ra;
		delete wire_ra;

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

// Sets the raster type
void kl::renderer::setWireframe(bool enabled) {
	if (enabled) {
		wire_ra->bind();
	}
	else {
		solid_ra->bind();
	}
}

// Mesh
kl::mesh* kl::renderer::newMesh(const std::vector<kl::vertex>& vertexData) {
	meshes.push_back(gpu->newMesh(vertexData));
	return meshes.back();
}
kl::mesh* kl::renderer::newMesh(const std::string& filePath, bool flipZ) {
	meshes.push_back(gpu->newMesh(filePath, flipZ));
	return meshes.back();
}
void kl::renderer::delMesh(kl::mesh* mesAddress) {
	for (int i = 0; i < meshes.size(); i++) {
		if (meshes[i] == mesAddress) {
			delete meshes[i];
			meshes.erase(meshes.begin() + i);
			break;
		}
	}
}

// Texture
kl::texture* kl::renderer::newTexture(const kl::image& image) {
	textures.push_back(gpu->newTexture(image));
	return textures.back();
}
void kl::renderer::delTexture(kl::texture* texAddress) {
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i] == texAddress) {
			delete textures[i];
			textures.erase(textures.begin() + i);
			break;
		}
	}
}

// Entity
kl::entity* kl::renderer::newEntity(kl::mesh* mes, kl::texture* tex) {
	entities.push_back(new kl::entity(mes, tex));
	return entities.back();
}
void kl::renderer::delEntity(kl::entity* objectAddress) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i] == objectAddress) {
			delete entities[i];
			entities.erase(entities.begin() + i);
			break;
		}
	}
}

// Skybox
kl::skybox* kl::renderer::newSkybox(const kl::image& fullBox) {
	skyboxes.push_back(new kl::skybox(gpu->getDev(), gpu->getDevCon(), fullBox));
	return skyboxes.back();
}
kl::skybox* kl::renderer::newSkybox(const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) {
	skyboxes.push_back(new kl::skybox(gpu->getDev(), gpu->getDevCon(), front, back, left, right, top, bottom));
	return skyboxes.back();
}
void kl::renderer::delSkybox(kl::skybox* skyboxAddress) {
	for (int i = 0; i < skyboxes.size(); i++) {
		if (skyboxes[i] == skyboxAddress) {
			delete skyboxes[i];
			skyboxes.erase(skyboxes.begin() + i);
			break;
		}
	}
}
