#include "Pch.h"
#include "Core/Layer.hpp"
#include "Eventing/Eventing.hpp"
#include "Utilities/InstrumentationMacros.hpp"
namespace ant
{
    LayerStack::~LayerStack()
    {
        CORE_GENERAL_PROFILE_FUNC();
        for (auto &it : *this)
        {
            it->OnDetach();
        }
    }

    void LayerStack::OnUpdate(TimeStep ts)
    {
        CORE_GENERAL_PROFILE_FUNC();
        for (auto &it : *this)
        {
            it->OnUpdate(ts);
        }
    }

    void LayerStack::OnDraw()
    {
        CORE_GENERAL_PROFILE_FUNC();
        for (auto it = rbegin(); it != rend(); it++)
        {
            if (it->get()->m_active)
            {
                it->get()->OnDraw();
            }
            else
            {
                it->get()->OnDetach(); // auto remove layer when its not active
                erase(it.base());
            }
        }
    }

    void LayerStack::OnEvent(Event *event)
    {
        for (auto &it : *this)
        {
            if (event->m_handled)
                break;

            it->OnEvent(event);
        }
    }

    void LayerStack::PushLayer(const Ref<Layer> &layer)
    {
        CORE_GENERAL_PROFILE_FUNC();
        layer->OnAttach();
        push_back(layer);
    }

    void LayerStack::PushOverlay(const Ref<Layer> &overlay)
    {
        CORE_GENERAL_PROFILE_FUNC();
        auto it = begin() + m_overlayIndex;
        overlay->OnAttach();
        emplace(it, overlay);
        m_overlayIndex++;
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        CORE_GENERAL_PROFILE_FUNC();
        auto target = Find(layer);
        target->get()->OnDetach();
        erase(target);
    }

    void LayerStack::PopOverlay(Layer *layer)
    {
        CORE_GENERAL_PROFILE_FUNC();
        auto target = Find(layer);
        target->get()->OnDetach();
        erase(target);
        m_overlayIndex--;
    }

    LayerStack::iterator LayerStack::Find(Layer *layer)
    {
        for (size_t i = 0; i < size(); i++)
        {
            if (layer == at(i).get())
                return (begin() + i);
        }
        return end();
    }
} // namespace ant