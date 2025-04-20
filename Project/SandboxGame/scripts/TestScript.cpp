#include "TestScript.h"
#include "Utils/Log.h"
#include <iostream>

namespace Larry::Scripts {

    TestScript::TestScript() {

    }

    TestScript::~TestScript() {

    }

    void TestScript::OnCreate() {
        LA_INFO("TestScript OnCreate!!!!!");
    }

    void TestScript::OnUpdate(double deltaTime) {
        LA_INFO("TestScript OnUpdate");
    }

    /* void TestScript::OnDelete() {
        std::cout << "TestScript OnDelete" << std::endl;
    } */
    
    void TestScript::HandleEvent(const Ref<Event>&) {
        LA_INFO("TestScript HandleEvent");
    }
}
