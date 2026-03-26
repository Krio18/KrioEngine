#pragma once

namespace Krio {
    using KEYCODE = int;

    class Event {
        public:
            virtual ~Event() = default;
    };

    struct WindowResizeEvent : public Event {
        int width;
        int height;
    };

    struct WindowCloseEvent : public Event {};

    struct KeyPressedEvent : public Event {
        KEYCODE keycode;
        bool repeat;
    };

    struct KeyReleasedEvent : public Event {
        KEYCODE keycode;
    };

    struct MouseMovedEvent : public Event {
        double x;
        double y;
        double deltaX;
        double deltaY;
    };

    struct MouseButtonPressedEvent : public Event {
        KEYCODE button;
    };

    struct MouseButtonReleasedEvent : public Event {
        KEYCODE button;
    };

    struct MouseScrolledEvent : public Event {
        double offsetX;
        double offsetY;
    };
}
