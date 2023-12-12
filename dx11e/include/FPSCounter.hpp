#ifndef _DX11E_FPSCOUNTER_
#define _DX11E_FPSCOUNTER_

#pragma once

#include <dx11ewin.hpp>
#include <queue>

DX11E_START

class DX11E_API FPSCounter final {
    
    public: 
        FPSCounter();
        
        FPSCounter(std::queue<FLOAT>);
        
        void Push(FLOAT);
        FLOAT GetAverageFPS() const;
    
    public: 
        inline static constexpr FLOAT MAX_SIZE = 100u;
    
    private: 
        std::queue<FLOAT> m_queue;
    
};

DX11E_END

#endif