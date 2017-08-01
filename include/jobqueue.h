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
#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class JobQueue
{
	public:
		void push(T t)
		{
			type_lock lock(m_mutex);
			if (m_connected_queue) {
				m_connected_queue->push(t);
				return;
			}
			m_queue.push(t);
			m_cond_var.notify_one();
		}
		T pop()
		{
			type_lock lock(m_mutex);
			while (m_queue.empty()) {
				m_cond_var.wait(lock);
			}
			T result = m_queue.front();
			m_queue.pop();
			m_cond_var.notify_one();
			return result;
		}
		T pop_with_timeout(int timeout)
		{
			type_lock lock(m_mutex);
			if (m_queue.empty()) {
				m_cond_var.wait_for(lock, std::chrono::milliseconds(timeout));
			}
			if (m_queue.empty()) {
				return T();
			}
			T result = m_queue.front();
			m_queue.pop();
			m_cond_var.notify_one();
			return result;
		}
		int length() const
		{
			type_lock lock(m_mutex);
			return m_queue.length();
		}
		bool empty()
		{
			type_lock lock(m_mutex);
			return m_queue.empty();
		}
		void connect(JobQueue<T> *connected_queue)
		{
			type_lock lock(m_mutex);
			m_connected_queue = connected_queue;
		}
	private:
		std::queue<T> m_queue;
		std::mutex m_mutex;
		std::condition_variable m_cond_var;
		typedef std::unique_lock<decltype(m_mutex)> type_lock;
		JobQueue<T> *m_connected_queue = nullptr;
};


#endif /* JOBQUEUE_H */
