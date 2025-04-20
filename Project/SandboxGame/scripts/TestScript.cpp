#include "TestScript.h"
#include "Components/Projection.h"
#include "Components/Quad.h"
#include "Components/Transform.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Math/Math.h"
#include "Scripts/Scripts.h"
#include "TextureLoader/TextureObject.h"
#include "TextureLoader/TextureLoader.h"
#include "Utils/Log.h"
#include "ECS/World.hpp"
#include "EventSystem/WindowEvents.h"

namespace Larry {

    TestScript::TestScript(const Ref<ECS::World>& world_) : Scripts::Script(world_) {
        
    }

    TestScript::~TestScript() {

    }

    void TestScript::OnCreate(const ECS::Entity& entity) {
        LA_INFO("TestScript OnCreate!!!!!");
        texture_loader = world->GetSingelton<TextureLoader>().value();
        face_texture = texture_loader->LoadTexture("media/textures/awesomeface.png", TextureConfig{});
        world->SetComponents<Quad>(entity, [this](Quad& quad){
            quad.texture = face_texture;
        });
    }

    void TestScript::OnUpdate(const ECS::Entity& entity, double deltaTime) {
        /* LA_INFO("TestScript OnUpdate, entity: {}", entity.GetId()); */
        Math::Vec3 direction(0);
        /* LA_INFO("Delta time: {}", deltaTime); */
        if (Input::KeyPressed(KEY_W)) {
            direction.y += vel;
        } 
        if (Input::KeyPressed(KEY_S)) {
            direction.y -= vel;
        } 
        if (Input::KeyPressed(KEY_D)) {
            direction.x += vel;
        } 
        if (Input::KeyPressed(KEY_A)) {
            direction.x -= vel;
        } 
        world->SetComponents<Transform>(entity, [direction, deltaTime](Transform& transform){
            /* LA_INFO("Setting transform!"); */
            transform.translation = transform.translation + (direction * (float)deltaTime);
        });
    }

    void TestScript::HandleEvent(const ECS::Entity& entity, const Ref<Event>& event) {
        LA_INFO("TestScript HandleEvent");
        DispatchEvent<Events::WindowResizedEvent>(event, [this](const Ref<Event>& e){
            Events::WindowResizedEvent* window_event = (Events::WindowResizedEvent*)e.get();
            world->System<Projection>([window_event](Projection& proj){
                proj.projection = Math::ortho(0, window_event->GetWidth(), 0, window_event->GetHeight(), 0.1f, 100.0f);
            });
        });
    }
}
