#include "KrimzLib/renderer/renderer.h"


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
		//sun.color = kl::colors::white;
		//sun.intensity = 1.0f;
		//sun.direction = kl::vec3(0.0f, -1.0f, -2.0f);

		// Creating the gpu
		gpu = new kl::gpu(win.getHWND(), 4);

		// Creating the rasters
		solid_ra = gpu->newRaster(false, false, true);
		wire_ra = gpu->newRaster(true, false, true);
		solid_ra->bind();

		// Compiling shaders
		default_sh = gpu->newShaders("res/shaders/renderer.hlsl");
		default_sh->bind();

		// CBuffer creation
		default_cb = gpu->newCBuffer(sizeof(VS_CB));
		default_cb->bind(0);

		// Sampler creation
		kl::sampler* samp = gpu->newSampler(true, true);
		samp->bind(0);
		delete samp;

		// Generating sun buffers


		// Calling the user start
		setup();
	};

	// Window update definition
	win.update = [&]() {
		// Time calculations
		deltaT = timer.interval();
		elapsedT = timer.elapsed();

		// Calling the user input
		input(&win.keys, &win.mouse);

		// Calling the physics update
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->upPhys(deltaT);
		}

		// Calling the user update
		update();

		// Clearing the cb buffer
		default_cb_data = {};

		// Setting the camera data
		default_cb_data.vp = camera.matrix();

		// Calculating the light vp matrix


		// Setting the light data


		// Rendering the shadows
		//sun.render(&win, [&]() {
		//	for (int i = 0; i < objects.size(); i++) {
		//		if (objects[i]->shadows) {
		//			// Setting the world matrix
		//			sun.setWMat(objects[i]->matrix());
		//
		//			// Rendering the object
		//			objects[i]->render();
		//		}
		//	}
		//});

		// Clearing the default buffers
		gpu->clear(background);

		// Rendering skybox
		//if (skybox) {
		//	skybox->render(camera.matrix());
		//}

		// Rendering objects
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->visible) {
				// Setting the world matrix
				default_cb_data.w = objects[i]->matrix();

				// Updating the cb data
				default_cb->setData(&default_cb_data);

				// Rendering the object
				objects[i]->render();
			}
		}

		// Updating the fps display
		win.setTitle(std::to_string(int(1 / deltaT)));

		// Swapping the frame buffers
		gpu->swap(vSync);
	};

	// Window end definition
	win.end = [&]() {
		// Deleting the sun shadow buffers


		// Deleting meshes
		for (int i = 0; i < meshes.size(); i++) {
			delete meshes[i];
		}
		meshes.clear();

		// Deleting textures
		for (int i = 0; i < textures.size(); i++) {
			delete textures[i];
		}
		textures.clear();

		// Deleting cbuffer
		delete default_cb;

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
	objects.push_back(new kl::entity(mes, tex));
	return objects.back();
}
void kl::renderer::delEntity(kl::entity* objectAddress) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == objectAddress) {
			delete objects[i];
			objects.erase(objects.begin() + i);
			break;
		}
	}
}
