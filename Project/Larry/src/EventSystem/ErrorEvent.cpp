#include "ErrorEvent.h"

namespace Larry {
    namespace Events {

        ErrorEvent::ErrorEvent(const std::string& errorMessage_) : errorMessage(errorMessage_) {

        }

        ErrorEvent::~ErrorEvent() {

        }

        std::string ErrorEvent::GetErrorMessage() {
            return errorMessage;
        }
    };
}
