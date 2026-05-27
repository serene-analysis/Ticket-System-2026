/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

#include <iostream>

namespace sjtu {

template<
    class Key,
    class T,
    class Compare = std::less <Key>
> class map {
 public:
  /**
   * the internal type of data.
   * it should have a default constructor, a copy constructor.
   * You can use sjtu::map as value_type by typedef.
   */
  typedef pair<const Key, T> value_type;
  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
  struct node{
    node *fa, *ch[2];
    node *prev, *next;
    bool red;
    value_type val;
    int size;
    node(node *gfa, node *gl, node *gr, node *gprev, node *gnext, bool gred, value_type gval, int gsize) : val(gval){
      fa = gfa, ch[0] = gl, ch[1] = gr, prev = gprev, next = gnext, red = gred, size = gsize;
      return;
    }
    node(const node& other) : val(other.val){
      fa = other.fa, ch[0] = other.ch[0], ch[1] = other.ch[1];
      prev = other.prev, next = other.next, red = other.red, size = other.size;
      return;
    }
    bool dir() const {
      return fa ? (fa->ch[1] && this == fa->ch[1]) : false;
    }
  };
  class const_iterator;
  class iterator {
    friend class const_iterator;
   private:
    /**
     * TODO add data val.secondbers
     *   just add whatever you want.
     */
   public:
    node *ptr;
    const map *from;
    iterator() {
      ptr = nullptr, from = nullptr;
      return;
      // TODO
    }

    iterator(node *gptr, const map *gfrom){
      ptr = gptr, from = gfrom;
      return;
    }

    iterator(const iterator &other) {
      ptr = other.ptr;
      from = other.from;
      return;
      // TODO
    }

    /**
     * TODO iter++
     */

   iterator operator++(int) {
      if(!this->from || !this->ptr){
        throw invalid_iterator();
      }
      if(this->ptr->size == -1){
        throw invalid_iterator();
      }
      iterator ret = *this;
      this->ptr = this->from->side(this->ptr, 1);
      return ret;
    }

    /**
     * TODO ++iter
     */
    iterator &operator++() {
      if(!this->from || !this->ptr){
        throw invalid_iterator();
      }
      if(this->ptr->size == -1){
        throw invalid_iterator();
      }
      this->ptr = this->from->side(this->ptr, 1);
      return *this;
    }

    /**
     * TODO iter--
     */
    iterator operator--(int) {
      if(!this->from){
        throw invalid_iterator();
      }
      if(this->ptr && this->ptr->size == -1){
        throw invalid_iterator();
      }
      iterator ret = *this;
      this->ptr = (this->ptr ? this->from->side(this->ptr, 0) : this->from->bottom(this->from->rt, 1));
      if(this->ptr == nullptr){
        throw invalid_iterator();
      }
      return ret;
    }

    /**
     * TODO --iter
     */
    iterator &operator--() {
      if(!this->from){
        throw invalid_iterator();
      }
      if(this->ptr && this->ptr->size == -1){
        throw invalid_iterator();
      }
      this->ptr = (this->ptr ? this->from->side(this->ptr, 0) : this->from->bottom(this->from->rt, 1));
      if(this->ptr == nullptr){
        throw invalid_iterator();
      }
      return *this;
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same val.secondory).
     */
    value_type &operator*() const {
      if(!this->ptr || !this->from || this->ptr->size == -1){
        throw invalid_iterator();
      }
      return this->ptr->val;
    }

    bool operator==(const iterator &rhs) const {
      if(from != rhs.from){
        return false;
      }
      if((ptr && ptr->size == -1) || (rhs.ptr && rhs.ptr->size == -1)){
        throw invalid_iterator();
      }
      return (ptr && from ? ptr : nullptr) == (rhs.ptr && rhs.from ? rhs.ptr : nullptr);
    }

    bool operator==(const const_iterator &rhs) const {
      if(from != rhs.from){
        return false;
      }
      if((ptr && ptr->size == -1) || (rhs.ptr && rhs.ptr->size == -1)){
        throw invalid_iterator();
      }
      return (ptr && from ? ptr : nullptr) == (rhs.ptr && rhs.from ? rhs.ptr : nullptr);
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

    /**
     * for the support of it->first.
     * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-val.secondber-access-operator-dash-greater-than-symbol-in-cpp/> for help.
     */
    value_type *operator->() const
    noexcept {
      /*if(!this->ptr || !this->from){
        throw invalid_iterator();
      }*/
      return &this->ptr->val;
    }
  };
  class const_iterator {
    friend class iterator;
    // it should has similar val.secondber method as iterator.
    //  and it should be able to construct from an iterator.
   private:
    // data val.secondbers.
   public:
    node *ptr;
    const map *from;
    const_iterator() {
      ptr = nullptr, from = nullptr;
      return;
      // TODO
    }

    const_iterator(node *gptr, const map *gfrom){
      ptr = gptr, from = gfrom;
      return;
    }

    const_iterator(const const_iterator &other) {
      ptr = other.ptr;
      from = other.from;
      return;
      // TODO
    }

    const_iterator(const iterator &other) {
      ptr = other.ptr;
      from = other.from;
      return;
      // TODO
    }

    const_iterator operator++(int) {
      if(!this->from || !this->ptr){
        throw invalid_iterator();
      }
      if(this->ptr->size == -1){
        throw invalid_iterator();
      }
      const_iterator ret = *this;
      this->ptr = this->from->side(this->ptr, 1);
      return ret;
    }

    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      if(!this->from || !this->ptr){
        throw invalid_iterator();
      }
      if(this->ptr->size == -1){
        throw invalid_iterator();
      }
      this->ptr = this->from->side(this->ptr, 1);
      return *this;
    }

    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      if(!this->from){
        throw invalid_iterator();
      }
      if(this->ptr && this->ptr->size == -1){
        throw invalid_iterator();
      }
      const_iterator ret = *this;
      this->ptr = (this->ptr ? this->from->side(this->ptr, 0) : this->from->bottom(this->from->rt, 1));
      if(this->ptr == nullptr){
        throw invalid_iterator();
      }
      return ret;
    }

    /**
     * TODO --iter
     */
    const_iterator &operator--() {
      if(!this->from){
        throw invalid_iterator();
      }
      if(this->ptr && this->ptr->size == -1){
        throw invalid_iterator();
      }
      this->ptr = (this->ptr ? this->from->side(this->ptr, 0) : this->from->bottom(this->from->rt, 1));
      if(this->ptr == nullptr){
        throw invalid_iterator();
      }
      return *this;
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same val.secondory).
     */
    value_type &operator*() const {
      if(!this->ptr || !this->from || this->ptr->size == -1){
        throw invalid_iterator();
      }
      return this->ptr->val;
    }

    bool operator==(const iterator &rhs) const {
      if(from != rhs.from){
        return false;
      }
      if((ptr && ptr->size == -1) || (rhs.ptr && rhs.ptr->size == -1)){
        throw invalid_iterator();
      }
      return (ptr && from ? ptr : nullptr) == (rhs.ptr && rhs.from ? rhs.ptr : nullptr);
    }

    bool operator==(const const_iterator &rhs) const {
      if(from != rhs.from){
        return false;
      }
      if((ptr && ptr->size == -1) || (rhs.ptr && rhs.ptr->size == -1)){
        throw invalid_iterator();
      }
      return (ptr && from ? ptr : nullptr) == (rhs.ptr && rhs.from ? rhs.ptr : nullptr);
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

    /**
     * for the support of it->first.
     * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-val.secondber-access-operator-dash-greater-than-symbol-in-cpp/> for help.
     */
    value_type *operator->() const
    noexcept {
      /*if(!this->ptr || !this->from){
        throw invalid_iterator();
      }*/
      return &this->ptr->val;
    }
    // And other methods in iterator.
    // And other methods in iterator.
    // And other methods in iterator.
  };

  /**
   * TODO two constructors
   */

  node *rt;
  Compare cmp;
  node *dbeg, *dend;
  
  map() {
    rt = dbeg = dend = nullptr;
    cmp = Compare();
    return;
  }

  void clone(node* &now, node* oth){
    if(!oth){
      now = nullptr;
      return;
    }
    now = new node(*oth);
    clone(now->ch[0], oth->ch[0]), clone(now->ch[1], oth->ch[1]);
    if(now->ch[0]){
      now->ch[0]->fa = now;
    }
    if(now->ch[1]){
      now->ch[1]->fa = now;
    }
    return;
  }

  map(const map &other) {
    clone(rt, other.rt);
    cmp = other.cmp;
    dbeg = dend = nullptr;
    return;
  }

  /**
   * TODO assignment operator
   */
  map &operator=(const map &other) {
    if(this == &other){
      return *this;
    }
    clear();
    clone(rt, other.rt), cmp = other.cmp;
    return *this;
  }

  /**
   * TODO Destructors
   */
  ~map() {
    clear();
    while(dbeg != dend){
      node *p = dbeg->next;
      delete dbeg;
      dbeg = p;
    }
    if(dbeg){
      delete dbeg;
    }
    return;
  }

  /**
   * TODO
   * access specified element with bounds checking
   * Returns a reference to the mapped value of the element with key equivalent to key.
   * If no such element exists, an exception of type `index_out_of_bound'
   */
  T &at(const Key &key) {
    iterator got = find(key);
    if(got == end()){
      throw index_out_of_bound();
    }
    return got.ptr->val.second;
  }

  const T &at(const Key &key) const {
    const_iterator got = find(key);
    if(got == cend()){
      throw index_out_of_bound();
    }
    return got.ptr->val.second;
  }

  /**
   * TODO
   * access specified element
   * Returns a reference to the value that is mapped to a key equivalent to key,
   *   performing an insertion if such key does not already exist.
   */
  T &operator[](const Key &key) {
    iterator got = find(key);
    if(got == end()){
      return insert(pair(key, T())).first.ptr->val.second;
    }
    return got.ptr->val.second;
  }

  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T &operator[](const Key &key) const {
    const_iterator got = find(key);
    if(got == cend()){
      throw index_out_of_bound();
    }
    return got.ptr->val.second;
  }

  /**
   * return a iterator to the beginning
   */
  iterator begin() {
    return iterator(bottom(rt, 0), this);
  }

  const_iterator cbegin() const {
    return const_iterator(bottom(rt, 0), this);
  }

  /**
   * return a iterator to the end
   * in fact, it returns past-the-end.
   */
  iterator end() {
    return iterator(nullptr, this);
  }

  const_iterator cend() const {
    return const_iterator(nullptr, this);
  }

  /**
   * checks whether the container is empty
   * return true if empty, otherwise false.
   */
  bool empty() const {
    return rt == nullptr;
  }

  /**
   * returns the number of elements.
   */
  size_t size() const {
    return empty() ? 0 : rt->size;
  }

  /**
   * clears the contents
   */

  void remove_push(node *p){
    p->size = -1;
    if(!dbeg){
      dbeg = dend = p;
    }
    else{
      dend->next = p, dend = p;
    }
    return;
  }

  void remove(node *p){
    if(!p){
      return;
    }
    remove(p->ch[0]), remove(p->ch[1]);
    remove_push(p);
    return;
  }

  void clear() {
    remove(rt);
    rt = nullptr;
    return;
  }

  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the insertion),
   *   the second one is true if insert successfully, or false.
   */

  void link(node *fa, node *p, bool dir){
    //std::cerr << "link : " << (fa ? fa : 0) << " , " << (p ? p : 0) << " , dir = " << dir << std::endl;
    if(fa){
      fa->ch[dir] = p;
    }
    if(p){
      p->fa = fa;
    }
    return;
  }

  void update(node *x){
    if(!x){
      return;
    }
    x->size = 1;
    if(x->ch[0]){
      x->size += x->ch[0]->size;
    }
    if(x->ch[1]){
      x->size += x->ch[1]->size;
    }
    return;
  }

  void rotate(node *x){
    //std::cerr << "rotate : " << x << std::endl;
    node *f = x->fa, *ff = f->fa;
    //std::cerr << "ok?" << std::endl;
    bool fson = x->dir(), ffson = (f->fa ? f->dir() : false);
    node *d = x->ch[!fson];
    x->fa = ff;
    if(ff){
      link(ff, x, ffson);
    }
    else{
      //std::cerr << "rotate changed rt" << std::endl;
      rt = x;
    }
    link(f, d, fson), link(x, f, !fson);
    update(f), update(x);
    return;
  }

  void put(node *p, int coe){
    while(p){
      p->size += coe, p = p->fa;
    }
    return;
  }

  bool sml(Key lv, Key rv){
    return cmp(lv, rv);
  }

  bool red(node *x){
    return x ? x->red : false;
  }
  
  void slr(node *now, int dep = 0){
    if(!now){
      return;
    }
    //if(dep >= 60){
    //  //std::cerr << "dep = " << dep << std::endl;
    //}
    slr(now->ch[0], dep + 1);
    std::cerr << now << ", is_red = " << now->red << ", l = " << (now->ch[0] ? now->ch[0] : 0)
       << ", r = " << (now->ch[1] ? now->ch[1] : 0) << ", size = " << now->size << std::endl;
    slr(now->ch[1], dep + 1);
    return;
  }
  
  pair<iterator, bool> insert(const value_type &value) {
    //slr(rt);
    //std::cerr << "insert" << std::endl;
    iterator got = find(value.first);
    if(got != end()){
      return pair(got, false);
    }
    //std::cerr << "not inserted" << std::endl;
    node *p = new node(nullptr, nullptr, nullptr, nullptr, nullptr, true, value, 1), *now = rt, *fa = rt;
    bool dir = false;
    while(now){
      fa = now;
      dir = sml(now->val.first, p->val.first), now = now->ch[dir];
    }
    if(!rt){
      //std::cerr << "empty, new root" << std::endl;
      rt = p, rt->red = false;
      return pair(iterator(p, this), true);
    }
    p->red = (fa ? true : false);
    link(fa, p, dir), put(fa, 1);
    pair<iterator, bool> ret = pair(iterator(p, this), true);
    //fix double red
    while(red(fa = p->fa)){
      //std::cerr << "double red" << std::endl;
      node *g = fa->fa, *u = g->ch[!fa->dir()];
      if(red(u)){
        //std::cerr << "u is red" << std::endl;
        fa->red = u->red = false, g->red = true;
        p = g;
      }
      else{
        //std::cerr << "u is black or empty" << std::endl;
        if(p->dir() != fa->dir()){
          rotate(p), std::swap(p, fa);
        }
        fa->red = false, g->red = true;
        rotate(fa);
      }
    }
    //std::cerr << "insert end" << std::endl;
    rt->red = false;
    return ret;
  }

  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
   */

  node* bottom(node *p, bool dir) const {
    if(!p){
      return nullptr;
    }
    while(p->ch[dir]){
      p = p->ch[dir];
    }
    return p;
  }

  node *side(node *p, bool dir) const {
    if(!p){
      return nullptr;
    }
    if(p->ch[dir]){
      return bottom(p->ch[dir], !dir);
    }
    while(p != rt && p->dir() == dir){
      p = p->fa;
    }
    return p->fa;
  }

  void erase_dw(node *p){
    //std::cerr << "erase_dw : " << p << std::endl;
    //slr(rt);
    node *fa = p->fa, *s = p->ch[0] ? p->ch[0] : p->ch[1];
    if(p == rt){
      if(s){
        s->fa = nullptr, rt = s, rt->red = false;
      }
      else{
        rt = nullptr;
      }
      return;
    }
    put(fa, -1);
    bool ndir = p->dir();
    if(s){
      link(fa, s, p->dir());
      s->red = false;
      return;
    }
    else{
      fa->ch[p->dir()] = nullptr;
    }
    //fix balance
    bool ok = false;
    while((fa = p->fa) && !red(p)){
      //std::cerr << "erase : black node" << std::endl;
      s = fa->ch[!ndir];
      //std::cerr << "p = " << p << ", fa = " << fa << ", s = " << s << std::endl;
      if(red(s)){
        rotate(s);
        std::swap(fa->red, s->red);
        continue;
      }
      node *ls = s->ch[ndir], *rs = s->ch[!ndir];
      if(!red(ls) && !red(rs)){
        if(!red(fa)){
          s->red = true;
          p = fa;
          if(p->fa){
            ndir = p->dir();
          }
          continue;
        }
        else{
          fa->red = false, s->red = true;// s must exist since p is black
          ok = true;
          //std::cerr << "ls,rs black, fa red" << std::endl;
          break;
        }
      }
      if(!red(rs)){
        rotate(ls);
        std::swap(ls->red, s->red);
        continue;
      }
      //std::cerr << "case 4: rotate(s)" << std::endl;
      rotate(s);
      rs->red = false;
      std::swap(s->red, fa->red);
      ok = true;
      break;
    }
    if(!ok){
      p->red = false;
    }
    rt->red = false;
    return;
  }

  void sub(node *op, node *np){
    //std::cerr << "sub : " << op << std::endl;
    /*std::cerr << op << ", l = " << (op->ch[0] ? op->ch[0] : 0)
       << ", r = " << (op->ch[1] ? op->ch[1] : 0) << ", fa = " << (op->fa ? op->fa : 0) << std::endl;*/
    if(op->fa){
      link(op->fa, np, op->dir());
    }
    else{
      np->fa = nullptr;
    }
    link(np, op->ch[0], 0);
    link(np, op->ch[1], 1);
    np->red = op->red;
    update(np);
    if(op == rt){
      rt = np;
    }
    op->next = np, np->prev = op;
    return;
  }

  /*node* &chain(node *p) const {
    //std::cout << "chain : " << p << ", next = " << p->next << std::endl;
    return p->next ? (p->next = chain(p->next)) : p;
  }*/

  void erase(iterator pos) {
    if(pos.from != this || pos == end()){
      throw index_out_of_bound();
    }
    iterator got = find(pos.ptr->val.first);
    if(got == end()){
      throw index_out_of_bound();
    }
    node *p = got.ptr;
    //slr(rt);
    //std::cerr << "p = " << p << std::endl;
    if(p->ch[0] && p->ch[1]){
      node *d = side(p, 1);
      //std::cerr << "d = " << d << std::endl;
      bool pdir = p->dir(), ddir = d->dir();
      if(d->fa == p){
        link(p->fa, d, pdir);
        node *l = d->ch[0], *r = d->ch[1];
        link(d, p, ddir);
        link(d, p->ch[!ddir], !ddir);
        link(p, l, 0), link(p, r, 1);
      }
      else if(p->fa == d){
        link(d->fa, p, ddir);
        node *l = p->ch[0], *r = p->ch[1];
        link(p, d, pdir);
        link(p, d->ch[!pdir], !pdir);
        link(d, l, 0), link(d, r, 1);
      }
      else{
        node *pfa = p->fa, *pl = p->ch[0], *pr = p->ch[1];
        bool pdir = p->dir();
        link(d->fa, p, ddir), link(p, d->ch[0], 0), link(p, d->ch[1], 1);
        link(pfa, d, pdir), link(d, pl, 0), link(d, pr, 1);
      }
      std::swap(p->red, d->red), std::swap(p->size, d->size);
      if(p == rt){
        rt = d;
      }
      else if(d == rt){
        rt = p;
      }
      /*node *gp = new node(*d), *gd = new node(*p);
      sub(d, gd), sub(p, gp);*/
      //std::cerr << "gd = " << gd << ", gp = " << gp << std::endl;
      /*std::swap(d->val, p->val);*/
      erase_dw(p);
    }
    else{
      erase_dw(p);
    }
    remove_push(p);
    return;
  }

  /**
   * Returns the number of elements with key
   *   that compares equivalent to the specified argument,
   *   which is either 1 or 0
   *     since this container does not allow duplicates.
   * The default method of check the equivalence is !(a < b || b > a)
   */
  size_t count(const Key &key) const {
    return find(key) == cend() ? 0 : 1;
  }

  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is returned.
   */
  iterator find(const Key &key) {
    //int cnt = 0;
    node *now = rt;
    while(now){
      //cnt++;
      //if(cnt >= 60){
      //  //std::cerr << "cnt = " << cnt << std::endl;
      //}
      //std::cerr << "now = " << now << std::endl;
      //std::cerr << "cmp(key, now->val.first) = " << cmp(key, now->val.first) << std::endl;
      //std::cerr << "cmp(now->val.first, key) = " << cmp(now->val.first, key) << std::endl;
      if(!cmp(key, now->val.first) && !cmp(now->val.first, key)){
        return iterator(now, this);
      }
      if(cmp(key, now->val.first)){
        now = now->ch[0];
      }
      else{
        now = now->ch[1];
      }
    }
    return iterator(nullptr, this);
  }

  const_iterator find(const Key &key) const {
    node *now = rt;
    while(now){
      if(!cmp(key, now->val.first) && !cmp(now->val.first, key)){
        return const_iterator(now, this);
      }
      if(cmp(key, now->val.first)){
        now = now->ch[0];
      }
      else{
        now = now->ch[1];
      }
    }
    return const_iterator(nullptr, this);
  }
};

}

#endif
