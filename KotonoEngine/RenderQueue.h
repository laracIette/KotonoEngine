#pragma once
#include <vector>
template<class TSource>
class RenderQueue
{
public:
	virtual void Add(TSource* object)
	{
		_queue.push_back(object);
	}

	virtual void Sort() = 0;
	virtual void Draw() const = 0;

	void Clear()
	{
		_queue.clear();
	}

protected:
	std::vector<TSource*> _queue;
};
