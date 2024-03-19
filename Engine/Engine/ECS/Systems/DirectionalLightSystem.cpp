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
	for (auto [camEnt, camTransform, cam] : cameraView.each())
	{
		for (auto [ent, transform, light] : view.each())
		{
			//static glm::vec3 rotation = transform.GetRotationDeg();
			//ImGui::Begin("Directional light control");
			//if(ImGui::DragFloat3("Rotation", &rotation.x))
			//{
			//	transform.SetRotationDeg(rotation);
			//}
			//ImGui::End();
			//

			constexpr float distanceFromView = 100000.0f;
			//glm::vec3 shadowCamPos = camTransform.GetPosition() - (transform.GetForward() * -1.f * distanceFromView);
			glm::vec3 camPos = camTransform.GetPosition();
			glm::vec3 shadowCamPos = camTransform.GetPosition() - (transform.GetForward() * distanceFromView);
			glm::mat4 toView = glm::lookAt(shadowCamPos, camTransform.GetPosition(), glm::vec3(0, 1, 0));
			Camera shadowCam{};
			constexpr float renderArea = 5000.0f;
			shadowCam.CreateOrthographic({ renderArea, renderArea }, 1.0f, 200000.0f);

			light.myLightView = toView;
			light.myLightProjection = shadowCam.myProjection;

			LOG("Camera Position: %s", glm::to_string(camPos).c_str());
			LOG("Camera Position: %s", glm::to_string(shadowCamPos).c_str());
			LOG("Light view position: %s", glm::to_string(glm::vec3(toView[3])).c_str());
		}
		return;
	}
}


