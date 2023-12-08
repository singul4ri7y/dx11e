#include <FPSCounter.hpp>

DX11E_START

FPSCounter::FPSCounter() : 
	m_queue({ 0.0f })
{}

FPSCounter::FPSCounter(std::queue<FLOAT> queue): 
	m_queue(queue)
{
	if(m_queue.size() > FPSCounter::MAX_SIZE) {
		size_t rest = this -> m_queue.size() - FPSCounter::MAX_SIZE;
		
		while(rest--) 
			this -> m_queue.pop();
	}
}

void FPSCounter::Push(FLOAT fps) {
	if(this -> m_queue.size() < FPSCounter::MAX_SIZE) 
		this -> m_queue.push(fps);
	else {
		this -> m_queue.pop();
		this -> m_queue.push(fps);
	}
}

FLOAT FPSCounter::GetAverageFPS() const {
	FLOAT sum = 0.0f;
	
	for(register USHORT i = 0; i < this -> m_queue.size(); i++) 
		sum += this -> m_queue.front();
	
	return sum / static_cast<FLOAT>(FPSCounter::MAX_SIZE);
}

DX11E_END