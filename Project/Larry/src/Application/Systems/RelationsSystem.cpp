#include "Application/Systems/RelationsSystem.h"
#include "Components/Transform.h"
#include "Components/Relationship.h"
#include "ECS.h"
#include "Math.h"
#include <optional>

namespace Larry {

    Math::Vec3 RelationSystem::GetParentTransform(ECS::Entity currEntity, Transform& currTransform) {
        if (currTransform._realTranslationTimestemp == counter) {
            return currTransform._realTranslation;
        }
        currTransform._realTranslationTimestemp = counter;

        std::optional<Child*> childOpt = world->GetComponent<Child>(currEntity);
        if (childOpt.has_value()) {
            Child* childComp = childOpt.value();

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

    void RelationSystem::HandleEvent(const Ref<Event>& event) {
    }
}
