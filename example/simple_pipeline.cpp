#include <pipeline.h>
#include <iostream>
#include <chrono>
#include <thread>

struct IntType
{
	IntType() :
		m_initialized(false) {}
	IntType(int value) :
		m_value(value),
		m_initialized(true) {}

	operator bool() const { 
		    return m_initialized;
	}
	int& operator()() {
			return m_value;
	}
	private:
		bool m_initialized;
		int m_value;
};

class WriteFilter : public Filter<IntType>
{
	public:
		void process(IntType t) {
			if (!t) {
				return;
			}
			std::cout << t() << std::endl;
		}
};

class AddFilter : public Filter<IntType>
{
	public:
		void process(IntType t) {
			if (!t) {
				return;
			}
			m_output_queue.push(t()+1);
		}

};

int main(int argc, const char *argv[])
{
	Pipeline<IntType> pipe;

	AddFilter add_filter;
	WriteFilter write_filter;
	
	pipe.addFilter(add_filter);
	pipe.addFilter(write_filter);

	pipe.start();

	for (int i = 10; i < 100; i+= 10) {
		add_filter.input_queue().push(i);
	}

	while (!pipe.empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
	pipe.stop();



	return 0;
}
