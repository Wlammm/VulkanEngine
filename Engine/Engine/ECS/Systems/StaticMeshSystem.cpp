#include "EnginePch.h"
#include "StaticMeshSystem.h"

#include "Engine.h"
#include "Core/Input.h"
#include "ECS/Components/StaticMesh.h"
#include "ECS/Components/Transform.h"
#include "World/World.h"

StaticMeshSystem::StaticMeshSystem()
{
    SetDependencies<const StaticMesh, Transform>();
}

void StaticMeshSystem::Tick()
{
    auto view = Engine::GetWorld().GetRegistry().view<const StaticMesh, Transform>();

    for(auto [ent, mesh, transform]: view.each())
    {
        if(Input::IsKeyPressed(KeyCode::Left))
        {
            transform.Move(glm::right() * -1.0f);
        }
        
        if(Input::IsKeyPressed(KeyCode::Right))
        {
            transform.Move(glm::right());
        }

        if(Input::IsKeyPressed(KeyCode::Up))
        {
            transform.Move(glm::forward());
        }
        
        if(Input::IsKeyPressed(KeyCode::Down))
        {
            transform.Move(glm::forward() * -1.0f);
        }

        if(Input::IsKeyPressed(KeyCode::O))
        {
            transform.SetRotationDeg(transform.GetRotationDeg() + glm::vec3(0.1f, 0, 0));
        }
        
        if(Input::IsKeyPressed(KeyCode::L))
        {
            transform.SetRotationDeg(transform.GetRotationDeg() + glm::vec3(-0.1f, 0, 0));
        }//pedro
    }
}


