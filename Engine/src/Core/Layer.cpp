#include "Pch.h"
#include "Core/Layer.hpp"
#include "Input/Event.hpp"
#include "debug/Instrumentation.hpp"
namespace df
{
    LayerStack::~LayerStack()
    {
        for (auto &it : *this)
        {
            it->OnDetach();
        }
    }

    void LayerStack::OnUpdate()
    {
        CORE_PROFILE_FUNC();
        for (auto &it : *this)
        {
            it->OnUpdate();
        }
    }

    void LayerStack::OnDraw()
    {
        CORE_PROFILE_FUNC();
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
        layer->OnAttach();
        push_back(layer);
    }

    void LayerStack::PushOverlay(const Ref<Layer> &overlay)
    {
        auto it = begin() + m_overlayIndex;
        overlay->OnAttach();
        emplace(it, overlay);
        m_overlayIndex++;
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        auto target = Find(layer);
        target->get()->OnDetach();
        erase(target);
    }

    void LayerStack::PopOverlay(Layer *layer)
    {
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
} // namespace df