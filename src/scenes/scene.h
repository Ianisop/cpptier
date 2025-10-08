#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>

namespace ctier
{
    class Scene
    {
      public:
        virtual ~Scene()                                          = default;
        virtual ftxui::Element Render()                           = 0;
        virtual void           OnEvent(const ftxui::Event& event) = 0;

        // Optional lifecycle hooks
        virtual void on_enter() {}
        virtual void on_exit() {}
    };
}  // namespace ctier
