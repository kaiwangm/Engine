#pragma once

#include "../Core.h"
#include "Layer.h"

namespace Engine {
class LayerStack {
   public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> layer);
    void PopLayer(Ref<Layer> layer);
    void PopOverlay(Ref<Layer> layer);

    std::vector<Ref<Layer> >::iterator begin() { return m_LayerVector.begin(); }
    std::vector<Ref<Layer> >::iterator end() { return m_LayerVector.end(); }
    std::vector<Ref<Layer> >::reverse_iterator rbegin() {
        return m_LayerVector.rbegin();
    }
    std::vector<Ref<Layer> >::reverse_iterator rend() {
        return m_LayerVector.rend();
    }

    std::vector<Ref<Layer> >::const_iterator begin() const {
        return m_LayerVector.begin();
    }
    std::vector<Ref<Layer> >::const_iterator end() const {
        return m_LayerVector.end();
    }
    std::vector<Ref<Layer> >::const_reverse_iterator rbegin() const {
        return m_LayerVector.rbegin();
    }
    std::vector<Ref<Layer> >::const_reverse_iterator rend() const {
        return m_LayerVector.rend();
    }

   private:
    std::vector<Ref<Layer> > m_LayerVector;
    unsigned int m_LayerInsertIndex = 0;
};
}  // namespace Engine