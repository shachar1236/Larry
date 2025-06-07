#include "LarryApi.h"
#include "Components/Background.h"
#include "Math/Math.h"
#include "TestScript.h"

namespace Larry {

    DEFINE_SCRIPT(TestScript, "GameLayer");

    TestScript::TestScript(const Ref<ECS::World>& world_) : Scripts::Script(world_) {
        
    }

    TestScript::~TestScript() {

    }

    void TestScript::OnCreate(const ECS::Entity& entity) {
        LA_INFO("TestScript OnCreate!!!!! aaaaa");
        texture_loader = world->GetSingelton<TextureLoader>();
        face_texture = texture_loader->LoadTexture("media/textures/awesomeface.png", TextureConfig{});
        world->SetComponents<Quad>(entity, [this](Quad& quad){
            // quad.texture = face_texture;
            LA_INFO("In test script callback!!!!!!!!!!");
            quad.texture = nullptr;
            quad.color.r = 0.1;
            quad.color.g = 0.1;
            quad.color.b = 0.7;
            quad.color.a = 1.0;
        });

        LayerStack* lstack = *world->GetSingelton<LayerStack*>();
        gameLayerId = lstack->GetLayer("GameLayer")->GetId();
        LA_INFO("GameLayer id: {}", gameLayerId);

        Background* bg = world->GetSingelton<Background>();
        bg->color.r = 0.3;
        bg->color.g = 0.7;
        bg->color.b = 0.1;
        bg->color.a = 1.0;
        
        bg->color = Math::Vec4(1.0f);
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
            LA_INFO("TestScript HandleEvent2");
            Events::WindowResizedEvent* window_event = (Events::WindowResizedEvent*)e.get();
            world->System<Projection>([window_event](Projection& proj){
                proj.projection = Math::ortho(0, window_event->GetWidth(), 0, window_event->GetHeight(), 0.1f, 100.0f);
            });
        });
    }
}
