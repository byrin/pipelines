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
