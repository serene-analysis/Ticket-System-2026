#pragma GCC optimize(3)
#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <iostream>

namespace sjtu { 

template<class T>
class deque {
public:
	class const_iterator;
	struct block;
	const static int block_size_ = 16;
	class iterator {
		friend class deque;
		friend class const_iterator;
	private:
		int bel, pos;
		block* ptr;
		const deque* dq;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		iterator(){
			bel = pos = 0, ptr = nullptr;
		}
		iterator(int gbel, int gpos, block* gptr, const deque* gdq) : bel(gbel), pos(gpos), ptr(gptr), dq(gdq){
			//if(pos == 16)throw false;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			iterator ret = *this;
			ret.pos += n;
			ret.bel += ret.pos >> 4;
			ret.pos &= 15;
			if(ret.pos < 0){
				ret.pos += 16;
				ret.bel--;
			}
			return ret;
			//TODO
		}
		iterator operator-(const int &n) const {
			iterator ret = *this;
			ret.pos -= n;
			ret.bel += ret.pos >> 4;
			ret.pos &= 15;
			if(ret.pos < 0){
				ret.pos += 16;
				ret.bel--;
			}
			return ret;
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			if(ptr != rhs.ptr){
				//std::cout << "different pointer" << std::endl;
				//std::cout << "ptr : " << ptr << std::endl;
				//std::cout << "rhs.ptr : " << rhs.ptr << std::endl;
				throw invalid_iterator();
			}
			int ret = ((bel - rhs.bel) << 4) + (pos - rhs.pos);
			return ret;
			//TODO
		}
		iterator operator+=(const int &n) {
			*this = *this + n;
			return *this;
			//TODO
		}
		iterator operator-=(const int &n) {
			*this = *this - n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator ret = *this;
			this->pos++;
			if(this->pos == 16){
				this->pos = 0;
				this->bel++;
			}
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			this->pos++;
			if(this->pos == 16){
				this->pos = 0;
				this->bel++;
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator ret = *this;
			this->pos--;
			if(this->pos == -1){
				this->pos = 15;
				this->bel--;
			}
			return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			this->pos--;
			if(this->pos == -1){
				this->pos = 15;
				this->bel--;
			}
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
			iterator beg = dq->begin(), end = dq->end();
			if(*this < beg || !(*this < end)){
				throw invalid_iterator();
			}
			return ptr[bel].val[pos];
		}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept {
			iterator beg = dq->begin(), end = dq->end();
			if(*this < beg || !(*this < end)){
				throw invalid_iterator();
			}
			return ptr[bel].val + pos;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr && pos == rhs.pos && bel == rhs.bel;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr && pos == rhs.pos && bel == rhs.bel;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator<(const iterator &rhs) const {
			return bel < rhs.bel || (bel == rhs.bel && pos < rhs.pos);
		}
		bool operator>(const iterator &rhs) const {
			return bel > rhs.bel || (bel == rhs.bel && pos > rhs.pos);
		}
		bool operator<(const const_iterator &rhs) const {
			return bel < rhs.bel || (bel == rhs.bel && pos < rhs.pos);
		}
		bool operator>(const const_iterator &rhs) const {
			return bel > rhs.bel || (bel == rhs.bel && pos > rhs.pos);
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		friend class iterator;
		private:
			int bel, pos;
			block* ptr;
			const deque* dq;
			// data members.
		public:
			const_iterator(){
				bel = pos = 0, ptr = nullptr;
			}
			const_iterator(int gbel, int gpos, block* gptr, const deque* gdq) : bel(gbel), pos(gpos), ptr(gptr), dq(gdq){
				//if(pos == 16)throw false;
			}
			const_iterator(const iterator &other) : bel(other.bel), pos(other.pos), ptr(other.ptr), dq(other.dq){
				//if(pos == 16)throw false;
				// TODO
			}
			const_iterator operator+(const int &n) const {
				const_iterator ret = *this;
				ret.pos += n;
				ret.bel += ret.pos >> 4;
				ret.pos &= 15;
				if(ret.pos < 0){
					ret.pos += 16;
					ret.bel--;
				}
				return ret;
				//TODO
			}
			const_iterator operator-(const int &n) const {
				const_iterator ret = *this;
				ret.pos -= n;
				ret.bel += ret.pos >> 4;
				ret.pos &= 15;
				if(ret.pos < 0){
					ret.pos += 16;
					ret.bel--;
				}
				return ret;
				//TODO
			}
			// return th distance between two const_iterator,
			// if these two const_iterators points to different vectors, throw invaild_const_iterator.
			int operator-(const const_iterator &rhs) const {
				if(ptr != rhs.ptr){
					throw invalid_iterator();
				}
				int ret = ((bel - rhs.bel) << 4) + (pos - rhs.pos);
				return ret;
				//TODO
			}
			const_iterator operator+=(const int &n) {
				*this = *this + n;
				return *this;
				//TODO
			}
			const_iterator operator-=(const int &n) {
				*this = *this - n;
				return *this;
				//TODO
			}
			/**
			 * TODO iter++
			 */
			const_iterator operator++(int) {
				const_iterator ret = *this;
				this->pos++;
				if(this->pos == 16){
					this->pos = 0;
					this->bel++;
				}
				return ret;
			}
			/**
			 * TODO ++iter
			 */
			const_iterator& operator++() {
				this->pos++;
				if(this->pos == 16){
					this->pos = 0;
					this->bel++;
				}
				return *this;
			}
			/**
			 * TODO iter--
			 */
			const_iterator operator--(int) {
				const_iterator ret = *this;
				this->pos--;
				if(this->pos == -1){
					this->pos = 15;
					this->bel--;
				}
				return ret;
			}
			/**
			 * TODO --iter
			 */
			const_iterator& operator--() {
				this->pos--;
				if(this->pos == -1){
					this->pos = 15;
					this->bel--;
				}
				return *this;
			}
			/**
			 * TODO *it
			 */
			T& operator*() const {
				iterator beg = dq->begin(), end = dq->end();
				if(beg > *this || !(end > *this)){
					throw invalid_iterator();
				}
				return ptr[bel].val[pos];
			}
			/**
			 * TODO it->field
			 */
			T* operator->() const noexcept {
				iterator beg = dq->begin(), end = dq->end();
				if(beg > *this || !(end > *this)){
					throw invalid_iterator();
				}
				return ptr[bel].val + pos;
			}
			/**
			 * a operator to check whether two iterators are same (pointing to the same memory).
			 */
			bool operator==(const iterator &rhs) const {
				return ptr == rhs.ptr && pos == rhs.pos && bel == rhs.bel;
			}
			bool operator==(const const_iterator &rhs) const {
				return ptr == rhs.ptr && pos == rhs.pos && bel == rhs.bel;
			}
			/**
			 * some other operator for iterator.
			 */
			bool operator!=(const iterator &rhs) const {
				return !(*this == rhs);
			}
			bool operator!=(const const_iterator &rhs) const {
				return !(*this == rhs);
			}
			bool operator<(const iterator &rhs) const {
				return bel < rhs.bel || (bel == rhs.bel && pos < rhs.pos);
			}
			bool operator>(const iterator &rhs) const {
				return bel > rhs.bel || (bel == rhs.bel && pos > rhs.pos);
			}
			bool operator<(const const_iterator &rhs) const {
				return bel < rhs.bel || (bel == rhs.bel && pos < rhs.pos);
			}
			bool operator>(const const_iterator &rhs) const {
				return bel > rhs.bel || (bel == rhs.bel && pos > rhs.pos);
			}
	};
	class block_empty : public exception {
		/* __________________________ */
	};
	class block_full : public exception {
		/* __________________________ */
	};
	class invalid_push : public exception {
		/* __________________________ */
	};
	class invalid_pop : public exception {
		/* __________________________ */
	};
	class invalid_insert : public exception {
		/* __________________________ */
	};
	class invalid_remove : public exception {
		/* __________________________ */
	};
	struct block{
		T* val;
		int l, r;
		block(){
			val = static_cast<T*>(malloc(block_size_ * sizeof(T)));
			l = 0, r = -1;
		}
		block(const block &oth){
			val = static_cast<T*>(malloc(block_size_ * sizeof(T)));
			l = oth.l;
			r = oth.r;
			for(int i = l; i <= r; ++i){
				new (val + i) T(oth.val[i]);
			}
			return;
		}
		block(block &&other)noexcept{
			val = other.val;
			l = other.l;
			r = other.r;
			other.val = nullptr;
			other.l = 0;
			other.r = -1;
		}
		~block(){
			if(val){
				for(int i=l;i<=r;++i){
					val[i].~T();
				}
				free(val);
			}
		}
		//~block(){
		//	static int destruct_count = 0;
		//	//std::cout << "destruct times : " << ++destruct_count << ", address = " << this << std::endl;
		//	//std::cout << "l = " << l << ", r = " << r << std::endl;
		//	for(int i=l;i<=r;i++){
		//		val[i].~T();
		//	}
		//	//std::cout << "deque destructed" << std::endl;
		//	free(val);
		//	return;
		//}
		void setp(int p){
			l = p, r = p - 1;
			return;
		}
		bool empty(){
			return l > r;
		}
		bool full(){
			return l == 0 && r == block_size_ - 1;
		}
		void push_front(const T &value){
			if(full()){
				throw block_full();
			}
			if(l == 0){
				throw invalid_push();
			}
			new (val+(--l)) T(value);
			return;
		}
		void push_back(const T &value){
			//std::cout << "push_back, l = " << l << ", r = " << r << std::endl;
			if(full()){
				throw block_full();
			}
			if(r == block_size_ - 1){
				throw invalid_push();
			}
			new (val+(++r)) T(value);
			//std::cout << "push_back end" << std::endl;
			return;
		}
		void pop_front(){
			if(empty()){
				throw block_empty();
			}
			val[l++].~T();
			return;
		}
		void pop_back(){
			if(empty()){
				throw block_empty();
			}
			val[r--].~T();
			return;
		}
		void insert(int pos, const T &value){// shouldn't be used on a empty block
			if(empty()){
				l = r = pos;
				new (val+pos) T(std::move(value));
				return;
			}
			if(r == block_size_ - 1){
				throw invalid_insert();
			}
			for(int i=r;i>=pos;i--){
				new (val+i+1) T(val[i]);
				val[i].~T();
			}
			new (val+pos) T(value);
			r++;
			return;
		}
		void remove(int pos){
			if(empty()){
				throw invalid_remove();
			}
			for(int i=pos;i<r;i++){
				val[i].~T();
				new (val+i) T(std::move(val[i+1]));
			}
			val[r].~T();
			r--;
			return;
		}
	};
	block* val;
	int l, r, len, num;
	/**
	 * TODO Constructors
	 */
	// Force deque to hold at least one block
	deque() {
		l = 0, r = 0, len = 1, num = 0;
		val = static_cast<block*>(malloc(sizeof(block)));
		new (val) block();
		return;
	}
	deque(const deque &other) {
		if(this == &other){
			return;
		}
		l = other.l, r = other.r, len = other.len, num = other.num;
		val = static_cast<block*>(malloc(len * sizeof(block)));
		for(int i=l;i<=r;i++){
			new (val+i) block();
			block &now = val[i], &oth = other.val[i];
			now.l = oth.l, now.r = oth.r;
			for(int j=oth.l;j<=oth.r;j++){
				new (now.val+j) T(oth.val[j]);
			}
		}
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {
		//std::cout << "deque ~ed, l = " << l << ", r = " << r << std::endl;
		for(int i=l;i<=r;i++){
			val[i].~block();
		}
		free(val);
		return;
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
		if(this == &other){
			return *this;
		}
		for(int i=l;i<=r;i++){
			val[i].~block();
		}
		free(val);
		l = other.l, r = other.r, len = other.len, num = other.num;
		val = static_cast<block*>(malloc(len * sizeof(block)));
		for(int i=l;i<=r;i++){
			new (val+i) block(other.val[i]);
		}
		return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
		if(pos >= num){
			throw index_out_of_bound();
		}
		return *(begin() + pos);
	}
	const T & at(const size_t &pos) const {
		if(pos >= num){
			throw index_out_of_bound();
		}
		return *(begin() + pos);
	}
	T & operator[](const size_t &pos) {
		if(pos >= num){
			throw index_out_of_bound();
		}
		return *(begin() + pos);
	}
	const T & operator[](const size_t &pos) const {
		if(pos >= num){
			throw index_out_of_bound();
		}
		return *(begin() + pos);
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
		if (empty()){
			throw container_is_empty();
		}
		return val[l].val[val[l].l];
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
		if (empty()){
			throw container_is_empty();
		}
		return val[r].val[val[r].r];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() const {
		//if(val[l].l == 16){
		//	std::cout << "16?" << std::endl;
		//	throw -1;
		//}
		
		return iterator(l, val[l].l, val, this) + 0;
	}
	const_iterator cbegin() const {
		//if(val[l].l == 16){
		//	std::cout << "16?" << std::endl;
		//	throw -1;
		//}
		return const_iterator(l, val[l].l, val, this) + 0;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() const {
		iterator got = iterator(r, val[r].r, val, this);
		got++;
		//if(got.pos == 16){
		//	std::cout << "16??" << std::endl;
		//	throw -1;
		//}
		return got;
	}
	const_iterator cend() const {
		const_iterator got = const_iterator(r, val[r].r, val, this);
		got++;
		//if(got.pos == 16){
		//	std::cout << "16??" << std::endl;
		//	throw -1;
		//}
		return got;
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {
		return num == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return num;
	}

	void expand(){
		int real = r - l + 1;
		//std::cout << "l = " << l << ", r = " << r << std::endl;
		//std::cout << "expand, real = " << real << std::endl;
		if(real <= 500){
			block* now = static_cast<block*>(malloc(real * 5 * sizeof(block)));
			for(int i=2*real;i<3*real;i++){
				new (now+i) block(std::move(val[i - 2 * real + l]));
				//std::cout << "i = " << i << std::endl;
				val[i - 2 * real + l].~block();
				//std::cout << "i = " << i << std::endl;
			}
			free(val);
			val = nullptr;
			//std::cout << "free(val) end " << std::endl;
			val = now;
			l = 2 * real, r = 3 * real - 1, len = 5 * real;
			//std::cout << "expand end" << std::endl;
		}
		else{
			block* now = static_cast<block*>(malloc(real * 2 * sizeof(block)));
			for(int i=real/2;i<real/2+real;i++){
				new (now+i) block(std::move(val[i - real / 2 + l]));
				//std::cout << "i = " << i << std::endl;
				val[i - real / 2 + l].~block();
				//std::cout << "i = " << i << std::endl;
			}
			free(val);
			val = nullptr;
			//std::cout << "free(val) end " << std::endl;
			val = now;
			l = real / 2, r = real / 2 + real - 1, len = 2 * real;
		}
		return;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
		//if(num == 0){
		//	std::cout << "l = " << l << ", r = " << r << ", num = " << num << std::endl;
		//}
		if(pos.ptr != val || pos.ptr != val || pos < begin() || pos > end()){
			throw invalid_iterator();
		}
		if(!(pos < end()) && !(pos > end())){
			push_back(value);
			iterator ret = end();
			return --ret;//anyway!
		}
		num++;
		int dis = pos - begin();
		if(val[pos.bel].r != block_size_ - 1){
			val[pos.bel].insert(pos.pos, value);
			//std::cout << "straight insert, new r = " << val[pos.bel].r << std::endl;
			if(val[pos.bel].r == block_size_ - 1 && pos.bel == len - 1){
				expand();
				return begin() + dis;
			}
			return pos;
		}
		T ins = value;
		int bp = pos.bel, ip = pos.pos;
		while(bp <= r && val[bp].r == block_size_ - 1){
			T nv = std::move(val[bp].val[val[bp].r]);
			val[bp].pop_back();
			val[bp].insert(ip, ins);
			ip = 0;
			ins = std::move(nv);
			bp++;
		}
		if(bp > r){
			new (val+bp) block();
			val[bp].push_back(ins);
			r++;
		}
		else{
			val[bp].insert(ip, ins);
		}
		if(r == len - 1 && val[r].r == block_size_ - 1){
			expand();
			return begin() + dis;
		}
		return pos;
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
		if(empty()){
			throw container_is_empty();
		}
		num--;
		if(pos.ptr != val || pos.ptr != val || pos < begin() || pos > end()){
			throw invalid_iterator();
		}
		block &now = val[pos.bel];
		if(now.r != block_size_ - 1){
			now.remove(pos.pos);
			if(val[pos.bel].empty()){
				if(pos.bel != r)throw false;
				if(l != r){
					val[r--].~block();
				}
			}
			return pos;
		}
		while(val[r].empty()){
			val[r--].~block();
		}
		int bp = pos.bel, dp = pos.pos;
		//std::cout << "l = " << l  << ", r = " << r << std::endl;
		bool removed = false;
		while(bp <= r && val[bp].r == block_size_ - 1){
			//std::cout << "bp = " << bp << ", dp = " << dp << std::endl;
			//std::cout << "nl = " << val[bp].l << ", nr = " << val[bp].r << std::endl;
			val[bp].remove(dp);
			removed = true;
			if(bp == r || val[bp + 1].empty()){
				//std::cout << "end or empty" << std::endl;
				break;
			}
			val[bp].push_back(val[bp + 1].val[0]);
			dp = 0;
			bp++;
			removed = false;
		}
		if(!removed){
			val[bp].remove(0);
		}
		while(val[r].empty()){
			val[r--].~block();
		}
		return pos;
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
		//std::cout << "outer push_back, r = " << r << std::endl;
		if(r == len - 1 && val[r].r == block_size_ - 1){
			expand();
			//std::cout << "expanded, l = " << l << ", r = " << r << std::endl;
		}
		num++;
		if(val[r].r == block_size_ - 1){
			r++;
			//std::cout << "new r = " << r << std::endl;
			new (val+r) block();
			val[r].setp(0);
		}
		val[r].push_back(value);
		if(r == len - 1 && val[r].r == block_size_ - 1){
			expand();
			//std::cout << "expanded, l = " << l << ", r = " << r << std::endl;
		}
		return;
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
		if(empty()){
			throw container_is_empty();
		}
		num--;
		while(val[r].empty()){
			val[r--].~block();
		}
		val[r].pop_back();
		if(val[r].empty() && l != r){
			val[r--].~block();
		}
		return;
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
		//if(num <= 50)std::cout << "push_front" << std::endl;
		if(l == 0 && val[l].l == 0){
			//if(num <= 50)std::cout << "expand" << std::endl;
			expand();
		}
		num++;
		if(val[l].l == 0){
			//if(num <= 50)std::cout << "next block" << std::endl;
			l--;
			new (val+l) block();
			val[l].setp(block_size_);
		}
		val[l].push_front(value);
		if(r == len - 1 && val[r].r == block_size_ - 1){
			expand();
			//std::cout << "expanded, l = " << l << ", r = " << r << std::endl;
		}
		//if(num <= 50)std::cout << "l = " << val[l].l << ", r = " << val[l].r << std::endl;
		return;
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
		if(empty()){
			throw container_is_empty();
		}
		num--;
		while(l != r && val[l].empty()){
			val[l++].~block();
		}
		val[l].pop_front();
		if(val[l].empty() && l != r){
			val[l++].~block();
		}
		return;
	}

	/**
	 * clears the contents
	 */

	void clear() {
		while(!empty()){
			//pop_back();
			pop_front();
		}
		return;
	}
};

}

#endif
/*
valgrind --tool=memcheck --leak-check=full ./your_program
g++ -g test.cpp -o go
valgrind --leak-check=full --show-leak-kinds=all ./go
*/