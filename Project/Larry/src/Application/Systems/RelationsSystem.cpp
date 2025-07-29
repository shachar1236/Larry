#include "Application/Systems/RelationsSystem.h"
#include "ComponentPtr.hpp"
#include "Components/Transform.h"
#include "Components/Relationship.h"
#include "ECS/CPPApi/ECS.h"
#include "Math.h"
#include <optional>

namespace Larry {

    Math::Vec3 RelationSystem::GetParentTransform(ECS::Entity currEntity, Transform& currTransform) {
        if (currTransform._realTranslationTimestemp == counter) {
            return currTransform._realTranslation;
        }
        currTransform._realTranslationTimestemp = counter;

        ECS::ComponentPtr<Child> childComp = world->GetComponent<Child>(currEntity);
        if (childComp.has_value()) {
            world->SetComponents<Transform>(childComp->parent, [this, &currTransform, &childComp](Transform& parentTransform){
                currTransform._realTranslation = currTransform.translation + GetParentTransform(childComp->parent, parentTransform);
            });
        } else {
            currTransform._realTranslation = currTransform.translation;
        }

        return currTransform._realTranslation;
    }

    void RelationSystem::OnCreate() {
        world->System<Transform>([this](Transform& transfrom){
            transfrom._realTranslationTimestemp = counter - 1;
        });
    }

    void RelationSystem::OnUpdate(double deltaTime) {
        counter++;

        world->AdvancedSystem<Transform>([this](ECS::Entity entity, bool* stop, Transform& transfrom){
            GetParentTransform(entity, transfrom);
        });
    }

    void RelationSystem::OnDelete() {

    }

    void RelationSystem::HandleEvent(Event* event) {
    }
}
