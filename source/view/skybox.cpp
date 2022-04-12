#include "view/skybox.h"

#include "utility/file.h"
#include "utility/console.h"


// Skybox box vertices
const std::vector<kl::vertex> boxVertices
{
	kl::vertex(kl::float3(1, -1,  1)), kl::vertex(kl::float3(1, -1, -1)), kl::vertex(kl::float3(1,  1, -1)),
	kl::vertex(kl::float3(1,  1, -1)), kl::vertex(kl::float3(1,  1,  1)), kl::vertex(kl::float3(1, -1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(-1,  1, -1)),
	kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(1,  1,  1)), kl::vertex(kl::float3(1,  1, -1)),
	kl::vertex(kl::float3(1,  1, -1)), kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(-1,  1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(1, -1, -1)),
	kl::vertex(kl::float3(1, -1, -1)), kl::vertex(kl::float3(1, -1,  1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(1, -1,  1)), kl::vertex(kl::float3(1,  1,  1)),
	kl::vertex(kl::float3(1,  1,  1)), kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(1,  1, -1)),
	kl::vertex(kl::float3(1,  1, -1)), kl::vertex(kl::float3(1, -1, -1)), kl::vertex(kl::float3(-1, -1, -1))
};

// Constructors/destructor
kl::skybox::skybox(kl::gpu* gpu, const std::string& name, const kl::image& fullbox)
{
	// Checking the aspect ratio
	if (fullbox.width() % 4 == 0 && fullbox.height() % 3 == 0)
	{
		// Getting the part size
		const int partWidth = fullbox.width() / 4;
		const int partHeight = fullbox.height() / 3;

		// Checking the part size
		if (partWidth == partHeight)
		{
			// Extracting the sides
			const kl::int2 partSize(partWidth, partHeight);
			const kl::image front = fullbox.rect(partSize * kl::int2(1, 1), partSize * kl::int2(2, 2));
			const kl::image back = fullbox.rect(partSize * kl::int2(3, 1), partSize * kl::int2(4, 2));
			const kl::image left = fullbox.rect(partSize * kl::int2(0, 1), partSize * kl::int2(1, 2));
			const kl::image right = fullbox.rect(partSize * kl::int2(2, 1), partSize * kl::int2(3, 2));
			const kl::image top = fullbox.rect(partSize * kl::int2(1, 0), partSize * kl::int2(2, 1));
			const kl::image bottom = fullbox.rect(partSize * kl::int2(1, 2), partSize * kl::int2(2, 3));

			// Calling the other constructor
			this->kl::skybox::skybox(gpu, name, front, back, left, right, top, bottom);
		}
		else
		{
			kl::console::show();
			std::cout << "Skybox image width and height do not match!";
			std::cin.get();
			exit(69);
		}
	}
	else
	{
		kl::console::show();
		std::cout << "Skybox image does not have the correct aspect ratio!";
		std::cin.get();
		exit(69);
	}
}
kl::skybox::skybox(kl::gpu* gpu, const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom)
{
	// Saving the gpu
	this->gpu = gpu;

	// Saving the name
	this->name = name;

	// Compiling skybox shaders
	sky_vtx = gpu->newVertexShader(kl::file::read("res/shaders/skybox.hlsl"));
	sky_pxl = gpu->newPixelShader(kl::file::read("res/shaders/skybox.hlsl"));
	vtx_cb = gpu->newConstBuffer(sizeof(kl::mat4));

	// Generating the box mesh
	box_mes = gpu->newVertBuffer(boxVertices);

	// Generating the box texture
	ID3D11Texture2D* boxTex = gpu->newTexture(front, back, left, right, top, bottom);
	box_tex = gpu->newShaderView(boxTex);
	gpu->destroy(boxTex);
}
kl::skybox::~skybox()
{
	gpu->destroy(sky_vtx);
	gpu->destroy(sky_pxl);
	gpu->destroy(vtx_cb);
	gpu->destroy(box_mes);
	gpu->destroy(box_tex);
}

// Renders the cubemap
void kl::skybox::render(const kl::mat4& vpMat) const
{
	// Shader bind
	gpu->bind(sky_pxl);
	gpu->bind(sky_vtx);
	gpu->bindVertCBuff(vtx_cb, 0);
	gpu->setBuffData(vtx_cb, (void*)&vpMat);

	// Binding the texture
	gpu->bindPixlTex(box_tex, 0);

	// Drawing the cubemap
	gpu->draw(box_mes);
}
