#ifndef PIPELINE_H
#define PIPELINE_H

#include "filter.h"

template <class T>
class Pipeline
{
	public:
		void addFilter(Filter<T> &filter)
		{
			Filter<T> *back = nullptr;
			if (!m_filters.empty()) {
				back = m_filters.back();
			}
			m_filters.push_back(&filter);
			if (back) {
				back->output_queue().connect(&filter.input_queue());
			}
		}
		void start()
		{
			for (auto i:m_filters) { i->start(); }
		}
		void stop()
		{
			for (auto i:m_filters) { i->stop(); }
		}
		bool empty()
		{
			bool result = true;
			for (auto i: m_filters) {
				if (!i->empty()) {
					result = false;
				}
			}
			return result;
		}
	private:
		std::vector<Filter<T>*> m_filters;
};


#endif /* PIPELINE_H */
