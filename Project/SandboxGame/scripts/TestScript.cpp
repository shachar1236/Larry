#include "TestScript.h"
#include <iostream>

namespace Larry::Scripts {

    TestScript::TestScript() {

    }

    TestScript::~TestScript() {

    }

    void TestScript::OnCreate() {
        std::cout << "TestScript OnCreate" << std::endl;
    }

    void TestScript::OnUpdate(double deltaTime) {
        std::cout << "TestScript OnUpdate" << std::endl;
    }

    void TestScript::OnDelete() {
        std::cout << "TestScript OnDelete" << std::endl;
    }
}
