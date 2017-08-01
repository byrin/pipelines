/*
 * MIT License
 * 
 * Copyright (c) 2017 Richard Spindler
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
