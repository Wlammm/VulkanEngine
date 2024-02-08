#include "EnginePch.h"
#include "DirectionalLightSystem.h"
#include "ECS/Components/DirectionalLight.h"
#include "ECS/Components/Transform.h"
#include "Engine.h"
#include "World/World.h"
#include "ECS/Components/Camera.h"

DirectionalLightSystem::DirectionalLightSystem()
{
	SetDependencies<Transform, DirectionalLight, const Camera>();
}

void DirectionalLightSystem::Tick()
{
	auto view = Engine::GetWorld().GetRegistry().view<Transform, DirectionalLight>();
	auto cameraView = Engine::GetWorld().GetRegistry().view<const Transform, const Camera>();
	for (auto [camEnt, camTransform, cam] : view.each())
	{
		for (auto [ent, transform, light] : view.each())
		{
			static Vec3f rotation = transform.GetRotationDeg();
			ImGui::Begin("Directional light control");
			if(ImGui::DragFloat3("Direction", &rotation.x))
			{
				transform.SetRotationDeg(rotation);
			}
			ImGui::End();

			constexpr float distanceFromView = 100000.0f;
			Vec3f shadowCamPos = camTransform.GetPosition() - (transform.GetForward() * -1.f * distanceFromView);
			Mat4f toView = Mat4f::CreateTranslation(shadowCamPos);
			toView.LookTowards(transform.GetPosition());
			Camera shadowCam{};
			constexpr float renderArea = 5000.0f;
			shadowCam.CreateOrthographic({ renderArea, renderArea });

			light.myLightView = toView.FastInverse().Transposed();
			light.myLightProjection = shadowCam.myProjection.Transposed();
			std::cout << camTransform.GetPosition() << "\n";
		}
		int a = 10;
	}
}


