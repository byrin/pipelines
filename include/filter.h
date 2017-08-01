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
#ifndef FILTER_H
#define FILTER_H

#include "jobqueue.h"
#include <thread>
#include <algorithm>
#include <iostream>

template <class T>
class Filter
{
	public:
		Filter() { /*start();*/ }
		virtual ~Filter() { /*stop();*/ }
		void start() { m_thread = std::thread([this]() { exec(); }); }
		void stop()
		{
			m_run = false;
			m_thread.join();
		}
		void exec()
		{
			while (m_run) {
				T t = m_input_queue.pop_with_timeout(100);
				m_processing = true;
				process(t);
				m_processing = false;
			}
		}
		bool empty()
		{
			if (m_processing) {
				return false;
			}
			return m_input_queue.empty() && m_output_queue.empty();
		}
		virtual void process(T t) = 0;
		JobQueue<T> &input_queue() { return m_input_queue; }
		JobQueue<T> &output_queue() { return m_output_queue; }
	protected:
		bool m_run = true;
		JobQueue<T> m_input_queue;
		JobQueue<T> m_output_queue;
		std::thread m_thread;
		bool m_processing = false;
		std::vector<int> m_benchmarks;
};


#endif /* FILTER_H */
