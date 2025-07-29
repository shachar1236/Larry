#include "ECS/CPPApi/ECS.h"
#include "Utils/LarryMemory.h"
#include "Math/Math.h"
#include "World.hpp"
#include <optional>

namespace Larry {

    struct Child {
        ECS::Entity parent;
        std::optional<ECS::Entity> nextChild = std::nullopt;
    };

    struct Parent {
        std::optional<ECS::Entity> firstChild = std::nullopt;
        int numberOfChilderns = 0;
    };
    
    // adds a child to parent
    inline void AddChild(ECS::World* world, ECS::Entity parent, ECS::Entity child) {
        world->InsertOrSetComponent<Parent>(parent, [&parent, &child, &world](Parent& parentComp){
            if (parentComp.numberOfChilderns > 0) {
                world->InsertOrSetComponent<Child>(child, [&parent, &parentComp](Child& childComp){
                    childComp.parent = parent;
                    childComp.nextChild = parentComp.firstChild;
                });
                
                parentComp.firstChild = child;
            } else {
                parentComp.firstChild = child;

                world->InsertOrSetComponent<Child>(child, [&parent](Child& childComp){
                    childComp.parent = parent;
                });
            }

            parentComp.numberOfChilderns++;
        });
    }

    // separate child from parent
    inline void DetachFromParent(ECS::World* world, ECS::Entity child) {
        world->SetComponents<Child>(child, [child, &world](Child& childComp){
            world->SetComponents<Parent>(childComp.parent, [child, &world](Parent& parentComp){
                Child* lastChild = nullptr;
                bool running = true;

                world->SetComponents<Child>(child, [&parentComp, &child, &lastChild, &running](Child& childComp){
                    if (parentComp.firstChild == child) {
                        parentComp.firstChild = childComp.nextChild;
                        parentComp.numberOfChilderns--;
                        running = false;
                    }

                    lastChild = &childComp;
                });

                while (running) {
                    std::optional<ECS::Entity> currChild = lastChild->nextChild;
                    if (currChild.has_value()) {
                        auto currChildOpt = world->GetComponent<Child>(currChild.value());
                        if (currChildOpt.has_value()) {
                            if (currChild.value() == child) {
                                Child* currChildComp = currChildOpt.value();
                                lastChild->nextChild = currChildComp->nextChild;

                                world->DeleteComponent<Child>(currChild.value());
                                parentComp.numberOfChilderns--;

                                running = false;
                            } else {
                                lastChild = currChildOpt.value();
                            }
                        } else {
                            running = false;
                        }
                    } else {
                        running = false;
                    }
                }
            });
        });
    }
}
