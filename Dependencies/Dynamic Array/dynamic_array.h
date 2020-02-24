#pragma once
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

template <typename T>
class dynamic_array
{
public:
	// Constructor
	dynamic_array<T>()
	{
		m_data = new T[m_default_capacity];
		m_capacity = m_default_capacity;
		m_count = 0;
	}

	// Desconstructor
	~dynamic_array()
	{
		delete[] m_data;
		m_data = nullptr;

		m_capacity = 0;
		m_count = 0;
	}

	// Copy Constructor
	dynamic_array(const dynamic_array& a_other)
	{
		m_count = a_other.m_count;
		m_capacity = a_other.m_capacity;
		m_data = new T[m_capacity];

		// Need to deep copy the array data
		for (size_t i = 0; i < m_capacity; i++)
		{
			m_data[i] = a_other.m_data[i];
		}
	}

	// Copy Assignment operator
	dynamic_array& operator=(const dynamic_array& a_other)
	{
		// Prevent copying over itself
		if (&a_other == this) { return *this; }
		// Clean up any existing data
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
		// Member wise copy
		m_count = a_other.m_count;
		m_capacity = a_other.m_capacity;
		// Need to deep copy the array data
		m_data = new T[m_capacity];
		for (size_t i = 0; i < m_capacity; i++)
		{
			m_data[i] = a_other.m_data[i];
		}
		return *this;
	}

	// Access array element by index
	T& operator[] (const int index)
	{
		return m_data[index];
	}

	// Add data()
	void push(const T& a_data)
	{
		// Check capacity
		if (m_count == m_capacity)
		{
			// Increase array size
			m_capacity *= 2;					 // Double array size
			T* new_location = new T[m_capacity]; // New temp. storage
			for (size_t i = 0; i < m_count; i++) // Deep copy to new storage
			{
				new_location[i] = m_data[i];
			}
			delete[] m_data;					 // remove old storage
			m_data = new_location;				 // point at new storage
		}
		m_data[m_count] = a_data;
		// Increment item count
		++m_count;
	}

	void pop()
	{
		--m_count;
	}

	void remove(size_t a_index, size_t a_count = 1) // size_t count = 1 is a default argument
	{
		for (size_t i = a_index; i < m_count; i++)
		{
			m_data[i] = m_data[i + a_count];
		}

		m_count -= a_count;
		trim();
	}

	void grow()
	{
		m_capacity *= 2;
		T* new_location = new T[m_capacity];
		for (size_t i = 0; i < m_count; i++)
		{
			new_location[i] = m_data[i];
		}
		delete[] m_data;
		m_data = new_location;
	}

	void trim()
	{
		if (m_count < m_capacity)
		{
			m_capacity = m_count;
			T* new_location = new T[m_capacity];
			for (size_t i = 0; i < m_count; i++)
			{
				new_location[i] = m_data[i];
			}
			delete[] m_data;
			m_data = new_location;
		}
	}

	// 1 or more elements of type t
	void insert(const T& a_data, size_t a_index)
	{
		if (m_count == m_capacity)
		{
			grow();
		}

		// Copy up to insertion point
		for (size_t i = m_count; i > a_index; i--)
		{
			m_data[i] = m_data[i - 1];
		}
		m_data[a_index] = a_data;
		++m_count;
	}

	void insert(const T* a_data, size_t a_element_count, size_t a_index)
	{
		while (m_count + a_element_count >= m_capacity)
		{
			grow();
		}

		// Move data over
		for (size_t i = m_count - 1; i >= a_index; --i)
		{
			m_data[i + a_element_count] = m_data[i];
		}

		// Copy insertion from array
		size_t j = 0;
		for (size_t i = a_index; i < a_index + a_element_count; ++i, ++j)
		{
			/*m_data[i] = a_data;*/
			m_data[i] = a_data[j];
		}
		m_count += a_element_count;
	}

	void insert(const dynamic_array<T>& a_data, size_t a_index)
	{
		insert(a_data.m_data, a_data.m_count, a_index);
	}


	void clear()
	{
		delete[] m_data;
		m_data = new T[m_default_capacity];
		m_capacity = m_default_capacity;
		m_count = 0;
	}

	void print()
	{
		for (size_t i = 0; i < m_count; i++)
		{
			std::cout << m_data[i] << std::endl;
		}
		std::cout << std::endl;
	}


	// Access array element by index
	T& at(const int index)
	{
		return m_data[index];
	}
	// Remove data

	size_t count()
	{
		return m_count;
	}

	// Element Count
	size_t capacity()
	{
		return m_capacity;
	}

	// A bubble sort with a boolean switch to make it stop if nothing is swapped.
	void bubble_sort() {
		bool swapped = true;

		for (int i = 0; m_count; i++)
		{
			std::cout << "Loop: " << i << std::endl;

			swapped = false;
			for (int j = 1; j < m_count; j++)
			{
				if (m_data[j - 1] > m_data[j])
				{
					// switching the values of the two indices
					T temp = m_data[j];
					m_data[j] = m_data[j - 1];
					m_data[j - 1] = temp;
					swapped = true;
					std::cout << m_data[j - 1] << " swapped with " << m_data[j] << std::endl;
				}
			}
			if (swapped == false)
			{
				break;
			}
		}
	}

	// Shaker sort (Cocktail sort)
	void shaker_sort() {
		bool swapped = true;
		int start = 0;
		int end = m_count - 1;

		while (swapped) {

			// First inner loop going up the array
			for (int i = start; i < end; i++)
			{
				if (m_data[i] > m_data[i + 1]) {
					T temp = m_data[i];
					m_data[i] = m_data[i + 1];
					m_data[i + 1] = temp;
					swapped = true;
				}
			}
			if (swapped == false)
			{
				break;
			}
			end--;

			swapped = false;

			// Second inner loop going back down	
			for (int i = end - 1; i > start; i--)
			{
				// compare neighbouring elements and swap
				if (m_data[i] < m_data[i - 1])
				{
					T temp = m_data[i];
					m_data[i] = m_data[i - 1];
					m_data[i - 1] = temp;
					swapped = true;
				}
			}
			if (swapped == false)
			{
				break;
			}
			start++;
		}
	}

	void insert_sort() {
		T key = 0;
		// Start to the end
		for (int i = 1; i < m_count; i++) // Loops for each element as the key
		{
			key = m_data[i]; // setting the key to be the data at i
			int j = i - 1; // just the element before i

			while ((j >= 0) && (m_data[j] > key))
			{
				m_data[j + 1] = m_data[j];
				m_data[j] = key;
				j--;
			}
			// m_data[j + 1] = key;

			//print_array();
			std::cout << "\'Moved\': " << key << std::endl;
		}
	}

private:
	// Pointer to storage
	T* m_data = nullptr;

	// Store current length
	size_t m_count = 0;

	// Store current capacity
	size_t m_capacity = 0;

	const size_t m_default_capacity = 8;


};
#endif // DYNAMIC_ARRAY_H

