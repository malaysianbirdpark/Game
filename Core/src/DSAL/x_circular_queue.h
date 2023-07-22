#pragma once

// TODO: needed to be modified to use pool allocator
namespace Engine::DSAL {
	template<typename T, unsigned long CAPACITY>
	class x_circular_queue {
	public:
		explicit x_circular_queue()
			: head{0u}, tail{0u}, numElem{0u}
		{
			buf = new T[CAPACITY];
		}

		~x_circular_queue() {
			delete[] buf;
		}

		T const& front() const {
			return buf[head];
		}

		T const& back() const {
			return &buf[tail - 1];
		}

		// CAUTION : all of the type C should have the same size!!!
		template<typename C, typename... Args>
		void push(Args&&... args) {
			if (full())
				pop();
			std::construct_at(&buf[tail], std::forward<Args>(args)...);
			tail = (tail + 1) % CAPACITY;
			++numElem;
		}

        template<typename C>
        void push(C&& c) {
            if (full())
                pop();
            std::construct_at(&buf[tail], std::forward<C>(c));
            tail = (tail + 1) % CAPACITY;
            ++numElem;
        }

		void pop() {
			if (empty())
				return;
			head = (head + 1) % CAPACITY;
			--numElem;
		}

        void flush() {
            numElem = 0;
            head = tail = 0;
        }

		[[nodiscard]] bool empty() const { return numElem == 0; }
		[[nodiscard]] bool full() const { return numElem == CAPACITY; }
	private:
		unsigned short head;
		unsigned short tail;
		unsigned short numElem;
		T* buf;
	};
}
