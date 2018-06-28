#include <RayTracer/Integrator/PathIntegrator.h>
#include <RayTracer/Renderer.h>
#include <RayTracer/Texture/ConstantTexture.h>
#include <RayTracer/Texture/ImageTexture.h>
#include <RayTracer/Material.h>
#include <RayTracer/Camera.h>
#include <RayTracer/Scene.h>
#include <Image/Image.h>
#include <Image/Tga.h>
#include <Math/Vector.h>
#include <System/Input.h>
#include <System/Window.h>
#include <System/Dialog.h>
#include <System/Thread.h>
#include <System/SystemAllocator.h>
#include <Core/Log.h>
#include <Core/Constants.h>
#include <Core/UnitTest.h>
#include <Core/Memory.h>

namespace
{
	// Max ray depth
	//
	const int max_depth = 10;

	// Render quality
	//
	const int quality = 16;

	// Constants
	//
	ConstantTexture<float> zero = { 0.0f };
	ConstantTexture<float> one = { 1.0f };

	// Calculate the focal distance based on the current camera transform and scene
	//
	bool Autofocus(Camera& camera, const Scene& scene)
	{
		Intersection intersection;

		if (!scene.Hit(intersection, Ray(camera.position, camera.z)))
			return false;

		camera.focal_distance = intersection.t;

		return true;
	}
};

struct Application
{
	Application(int w, int h) :
		allocator(1_GiB), window("RayTracer", w, h), renderer(quality), camera({ 0.09f, 0.05f, -0.4f }, { 0, 0.1f, -0.1f })
	{
		scene.planes.push_back({ { 0, 0, 0 }, { 0, 1, 0 }, Material(wood_normal, wood_color, wood_roughness, zero) });

		scene.spheres.push_back({ { +0.00f, 0.1f, 0.0f }, 0.1f, Material(gold_normal, gold_color, gold_roughness, one) });
		scene.spheres.push_back({ { +0.3f, 0.1f, 0.3f }, 0.1f, Material(steel_normal, steel_color, steel_roughness, one) });
		scene.spheres.push_back({ { -0.3f, 0.1f, 0.3f }, 0.1f, Material(tile_normal, tile_color, tile_roughness, zero) });

		scene.lights.push_back(scene.atmosphere.sun);

		camera.ApplySettings(Lens::FL35mm, FStop::F16, Shutter::SS100, Iso::ISO100);

		Autofocus(camera, scene);
	}

	void Run()
	{
		renderer.Render(window, camera, scene, integrator);

		while (window.Update())
		{
			Message message;

			while (window.ReadMessage(message))
			{
				switch (message.type)
				{
					case Message::Type::KeyDown:
						if (message.data == 'S' && Input::KeyDown(0x11))
							SaveImage();
						if (message.data == 'O' && Input::KeyDown(0x11))
							LoadReferenceImage();
						if (message.data == 'E')
							CalculateError();
						if (message.data == 'R')
							renderer.Render(window, camera, scene, integrator);
						break;

					default:
						break;
				}
			}

			Thread::Sleep(100);
		}
	}

	void SaveImage() const
	{
		char path[max_path_length] = { 0 };

		if (!Dialog::GetSaveFilename(path, sizeof(path), ".", "Targa (*.tga)\0*.tga\0"))
			return;

		const Image<Bgra> buffer(Memory::TempAllocator(), window.w, window.h);

		if (!window.Capture(buffer.texels, buffer.w, buffer.h))
			return;

		Tga::SaveImage(path, buffer);
	}

	void LoadReferenceImage()
	{
		char path[max_path_length] = { 0 };

		if (!Dialog::GetOpenFilename(path, sizeof(path), ".", "Targa (*.tga)\0*.tga\0"))
			return;

		Tga::LoadImage(reference, allocator, path);
	}

	void CalculateError() const
	{
		if (!reference.texels)
		{
			LOG_ERROR("Can't calculate variance because there is no reference image");
			return;
		}

		if (reference.w != window.w || reference.h != window.h)
		{
			LOG_ERROR("The reference image has different dimensions to the display image");
			return;
		}

		const Image<Bgra> render(Memory::TempAllocator(), window.w, window.h);

		if (!window.Capture(render.texels, render.w, render.h))
		{
			LOG_ERROR("Failed to capture the window");
			return;
		}

		float squared_error_sum = 0;
		float value_sum = 0;

		const int count = reference.w * reference.h;

		for (int i = 0; i < count; ++i)
		{
			const float3 cref = reference[i].ToLinear();
			const float3 cdis = render[i].ToLinear();

			squared_error_sum += Square(CalculateLuminance(cdis - cref));
			value_sum += CalculateLuminance(cref);
		}

		const float mean = value_sum / count;

		const float mse = squared_error_sum / count;
		const float rms = Sqrt(mse) / mean;

		LOG_INFO("MSE: %.6f, RMS: %.2f %%", mse, rms * 100);
	}

	// System heap
	//
	SystemAllocator allocator;

	// Native window
	//
	Window window;

	// Renderer
	//
	Renderer renderer;

	// Camera
	//
	Camera camera;

	// Scene
	//
	Scene scene;

	// Integrator
	//
	PathIntegrator integrator = { max_depth };
	//DirectIntegrator integrator;
	//DepthIntegrator integrator;

	// Ground-truth reference image
	//
	Image<Bgra> reference;

	// Images
	//
	ImageTexture<Linear3> gold_normal = { allocator, "GoldNormal.tga" };
	ImageTexture<GammaColor> gold_color = { allocator, "GoldReflectance.tga" };
	ImageTexture<LinearValue> gold_roughness = { allocator, "GoldRoughness.tga" };
	ImageTexture<Linear3> tile_normal = { allocator, "TilesNormal.tga" };
	ImageTexture<GammaColor> tile_color = { allocator, "TilesColor.tga" };
	ImageTexture<LinearValue> tile_roughness = { allocator, "TilesRoughness.tga" };
	ImageTexture<Linear3> wood_normal = { allocator, "WoodNormal.tga" };
	ImageTexture<GammaColor> wood_color = { allocator, "WoodColor.tga" };
	ImageTexture<LinearValue> wood_roughness = { allocator, "WoodRoughness.tga" };
	ImageTexture<Linear3> steel_normal = { allocator, "SteelNormal.tga" };
	ImageTexture<GammaColor> steel_color = { allocator, "SteelColor.tga" };
	ImageTexture<LinearValue> steel_roughness = { allocator, "SteelRoughness.tga" };
};

void Run(int w, int h)
{
	Application application(w, h);

	application.Run();
}

int main(int argc, char** argv)
{
	if (argc == 2 && _stricmp(argv[1], "-unittest") == 0)
		return UnitTest::RunTests();

	if (!Log::Initialize(Severity::Info))
		return 1;

	const int downsize = 2;

	Run(1536 / downsize, 1024 / downsize);

	Log::Shutdown();

	return 0;
}
