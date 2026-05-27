#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T* ptr;
		T* address;
	public:
		iterator(T* gptr, T* gaddress){
			ptr = gptr;
			address = gaddress;
			return;
		}
		/**
		 * return a new iterator which ptr n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			return iterator(ptr + n, address);
			//TODO
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n, address);
			//TODO
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if(address != rhs.address){
				throw sjtu::invalid_iterator();
			}
			int dif = rhs.ptr - ptr;
			return dif < 0 ? -dif : dif;
			//TODO
		}
		int operator-(const const_iterator &rhs) const
		{
			if(address != rhs.address){
				throw sjtu::invalid_iterator();
			}
			int dif = rhs.ptr - ptr;
			return dif < 0 ? -dif : dif;
			//TODO
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
			//TODO
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			*this = *this + 1;
			return *this - 1;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			*this = *this + 1;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			*this = *this - 1;
			return *this + 1;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			*this = *this - 1;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::output_iterator_tag;

	private:
		const T* ptr;
		const T* address;
	public:
		const_iterator(T* gptr, T* gaddress) : ptr(gptr), address(gaddress) {}
		const_iterator(T* gptr, const T* gaddress) : ptr(gptr), address(gaddress) {}
		const_iterator(const T* gptr, T* gaddress) : ptr(gptr), address(gaddress) {}
		const_iterator(const T* gptr, const T* gaddress) : ptr(gptr), address(gaddress) {}
		const_iterator(const iterator& other) : ptr(other.ptr), address(other.address) {}
		/**
		 * return a new iterator which ptr n-next elements
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n, address);
			//TODO
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n, address);
			//TODO
		}
		int operator-(const iterator &rhs) const
		{
			if(address != rhs.address || address == nullptr){
				throw sjtu::invalid_iterator();
			}
			int dif = rhs.ptr - ptr;
			return dif < 0 ? -dif : dif;
			//TODO
		}
		int operator-(const const_iterator &rhs) const
		{
			if(address != rhs.address || address == nullptr){
				throw sjtu::invalid_iterator();
			}
			int dif = rhs.ptr - ptr;
			return dif < 0 ? -dif : dif;
			//TODO
		}
		const_iterator operator++(int) {
			*this = *this + 1;
			return *this - 1;
		}
		const_iterator& operator++() {
			*this = *this + 1;
			return *this;
		}
		const_iterator operator--(int) {
			*this = *this - 1;
			return *this + 1;
		}
		const_iterator& operator--() {
			*this = *this - 1;
			return *this;
		}
		const T& operator*() const{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
	};
	T* address;
	T* start;
	int len, real;
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
		address = start = nullptr;
		len = real = 0;
		return;
	}
	vector(const vector &other) {
		if(other.len == 0){
			address = nullptr;
			len = real = 0;
			return;
		}
		len = other.len, real = other.real;
		T* now = static_cast<T*>(malloc(len * sizeof(T)));
		address = now;
		for(int i=0;i<real;i++){
			new (address + i) T(other.address[i]);
		}
		start = address;
		return;
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		if(address != nullptr){
			for(int i=0;i<real;i++){
				address[i].~T();
			}
			free(address);
		}
		len = real = -1;
		return;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if(this == &other){
			return *this;
		}
		if(address != nullptr){
			for(int i=0;i<real;i++){
				address[i].~T();
			}
			free(address);
		}
		if(other.len == 0){
			address = start = nullptr;
			len = real = 0;
			return *this;
		}
		len = other.len, real = other.real;
		T* now = static_cast<T*>(malloc(len * sizeof(T)));
		address = now;
		for(int i=0;i<real;i++){
			new (address + i) T(other.address[i]);
		}
		start = address;
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, len)
	 */
	T & at(const size_t &pos) {
		if(pos < 0 || pos >= real){
			throw sjtu::index_out_of_bound();
		}
		return address[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos < 0 || pos >= real){
			throw sjtu::index_out_of_bound();
		}
		return address[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, len)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos < 0 || pos >= real){
			throw sjtu::index_out_of_bound();
		}
		return address[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos < 0 || pos >= real){
			throw sjtu::index_out_of_bound();
		}
		return address[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if len == 0
	 */
	const T & front() const {
		if(real == 0){
			throw sjtu::container_is_empty();
		}
		return address[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if len == 0
	 */
	const T & back() const {
		if(real == 0){
			throw sjtu::container_is_empty();
		}
		return address[real - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(address, start);
	}
	const_iterator begin() const {
		return const_iterator(address, start);
	}
	const_iterator cbegin() const {
		return const_iterator(address, start);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(address + real, start);
	}
	const_iterator end() const {
		return const_iterator(address + real, start);
	}
	const_iterator cend() const {
		return const_iterator(address + real, start);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return real == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return real;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i=0;i<real;i++){
			address[i].~T();
		}
		real = 0;
		return;
	}
	void expand(){
		//printf("expand\n");
		len = len * 2 + 1;
		T* now = static_cast<T*>(malloc(len * sizeof(T)));
		for(int i=0;i<real;i++){
			new (now + i) T(address[i]);
		}
		for(int i=0;i<real;i++){
			address[i].~T();
		}
		free(address);
		address = now;
		return;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		int ipos = pos - begin();
		if (real == len) {
			expand();
			pos = begin() + ipos;
		}
		T* arr = address + ipos;
		real++;
		if (pos != end()) {
			for (int i = real-1;i>ipos;--i) {
				new (address + i) T(address[i - 1]);
				address[i - 1].~T();
			}
		}
		new (arr) T(value);
		return iterator(arr, start);
	}

	iterator insert(const size_t &ind, const T &value) {
		if (ind > real) {
			throw sjtu::index_out_of_bound();
		}
		return insert(begin() + ind, value);
	}

	iterator erase(iterator pos) {
		int epos = pos - begin();
		address[epos].~T();
		real--;
		for (int i = epos; i < real; ++i) {
			new (address + i) T(std::move(address[i + 1]));
			address[i + 1].~T();
		}
		return iterator(address + epos, start);
	}

	iterator erase(const size_t &ind) {
		if (ind >= static_cast<size_t>(real)) {
			throw sjtu::index_out_of_bound();
		}
		return erase(begin() + static_cast<int>(ind));
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		insert(end(), value);
		return;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if len() == 0
	 */
	void pop_back() {
		if(real == 0){
			throw sjtu::container_is_empty();
		}
		erase(real - 1);
		return;
	}
};


}

#endif
