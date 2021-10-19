#pragma once
#include <vector>
#include "Core/Core.hpp"

namespace df
{

    class Event;
    class Layer
    {
    public:
        Layer() {}
        ~Layer() {}
        friend class LayerStack;

        virtual void OnAttach() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnDraw() = 0;
        virtual void OnDetach() = 0;
        virtual void OnEvent(Event *event) = 0;

    protected:
        bool m_active = true;
    };
    using LayerStackStorageType = std::vector<Ref<Layer>>;
    class LayerStack
        : private LayerStackStorageType
    {
    public:
        LayerStack() {}
        ~LayerStack();

        inline iterator begin() { return LayerStackStorageType::begin(); }
        inline iterator end() { return LayerStackStorageType::end(); }
        inline const_iterator cbegin() { return LayerStackStorageType::cbegin(); }
        inline const_iterator cend() { return LayerStackStorageType::cend(); }

        inline reverse_iterator rbegin() { return LayerStackStorageType::rbegin(); }
        inline reverse_iterator rend() { return LayerStackStorageType::rend(); }
        inline const_reverse_iterator crbegin() { return LayerStackStorageType::crbegin(); }
        inline const_reverse_iterator crend() { return LayerStackStorageType::crend(); }

        void OnUpdate();
        void OnDraw();
        void OnEvent(Event *event);

        void PushLayer(const Ref<Layer> &layer);
        void PushOverlay(const Ref<Layer> &overlay); //todo

        void PopLayer(Layer *layer);
        void PopOverlay(Layer *layer); //todo

        iterator Find(Layer *layer);

    private:
        size_t m_overlayIndex = 0;
    };

} // namespace df
