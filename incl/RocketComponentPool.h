#ifndef COMPONENT_POOL
#define COMPONENT_POOL

#include <utility>
#include <type_traits>
#include <memory>

#include "same.h"

class Level {
  int next_id;
  struct gameObject_node {
    GameObject gameObject;
    gameObject_node *prev, *next;
    
    gameObject_node(GameObject const &obj) : gameObject(obj), prev(nullptr), next(nullptr) {}
  };
  
  gameObject_node *make_node(gameObject_node *prev, gameObject_node *next) {
    
  }
  
  gameObject_node *make_node(gameObject_node *prev, gameObject_node *next, GameObject const &copy) {
    
  }
  
  gameObject_node *first, *last;
  
  public:
  Level() : next_id(1) {}
};

class GameObject {
  
  struct component_base {
    component_base *prev, *next;
    component_base() : prev(nullptr), next(nullptr) {}
    virtual component_base *clone() const = 0;
    virtual void Load() = 0;
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Free() = 0;
    virtual void Unload() = 0;
    virtual void Serialise(Serialiser &) = 0;
    virtual void Deserialise(Serialise &) = 0;
    virtual void EditorUpdate() = 0;
    virtual ~component_base() {}
  };
  
  template <typename T>
  struct component_node : component_base {
    T component;
    component_node<T> *clone() const {
      return new component_node<T>{component};
    }
    
    void Load() {
      component.Load();
    }
    void Init() {
      component.Init();
    }
    void Update() {
      component.Update();
    }
    void Free() {
      component.Free();
    }
    void Unload() {
      component.Unload();
    }
    void Serialise(Serialiser &s) {
      component.Serialise(s);
    }
    void Deserialise(Serialser &s) {
      component.Deserialise(s);
    }
    void EditorUpdate() {
      component.EditorUpdate();
    }
    
    template <typename ... Argv, typename = typename std::enable_if<!at_least_one_same<std::is_base_of<component_base, typename std::remove_reference<Argv>::type>::value...>::value>::type>
    component_node(Argv &&... argv) : component{std::forward<Argv>(argv)...} {}
  };
  
  int id;
  component_base *first, *last;
  
  template <typename T, typename ... Argv>
  static component_node<T> *make_node(component_base *prev, component_base *next, Argv &&... argv) {
    // temporary use new to allocate memory
    component_node<T> * ret = new component_node<T>{std::forward<Argv>(argv)...};
    ret->prev = prev;
    if(prev != nullptr) ret->prev->next = ret;
    ret->next = next;
    if(next != nullptr) ret->next->prev = ret;
  }
  
  template <typename T>
  static component_node<T> *make_node(component_base *prev, component_base *next, component_node<T> const *ref) {
    // temporary use new to allocate memory
    component_node<T> * ret = ret->Clone();
    ret->prev = prev;
    if(prev != nullptr) ret->prev->next = ret;
    ret->next = next;
    if(next != nullptr) ret->next->prev = ret;
  }
  
  static component_base *copy_nodes(component_base *nodes) {
    component_base *ret = nullptr;
    while(nodes != nullptr) {
      component_base *curr = make_node(ret, nullptr, )
      nodes = nodes->next;
    }
  }
  
  static component_base *get_last(component_base *front) {
    while(front->next != nullptr) front = front->next;
    return front;
  }
  
  public:
  
  GameObject(int new_id = 0) : id(new_id), first(nullptr), last(first) {}
  GameObject(GameObject const &rhs) id(rhs.id), {
    
  }
  
  operator bool() const { return id != 0; }
  
  // nodes themselves should be low level const, so GetComponent being a const function should be fine.
  template <typename T>
  typename std::remove_reference<T>::type *GetComponent() const {
    using type = typename std::remove_reference<T>::type;
    component_base *front = first;
    while(front != nullptr) {
      component_node<type> * ret = dynamic_cast<component_node<type>*>(front);
      if(ret != nullptr) {
        return &ret->component;
      }
      front = front->next;
    }
    return nullptr;
  }
  
  template <typename T, typename ... Argv>
  typename std::remove_reference<T>::type *AddComponent(Argv &&... argv) {
    using type = typename std::remove_reference<T>::type;
    type * ret = GetComponent<T>();
    if(ret == nullptr) {
      if(first == last == nullptr) {
        first = last = ret = make_node<type>(nullptr, nullptr, std::forward<Argv>(argv)...);
      } else {
        last = ret = make_node<type>(last, nullptr, std::forward<Argv>(argv)...);
      }
      ret->component.owner_id = id;
    }
    return ret;
  }
  
  template <typename T>
  void RemoveComponent() {
    using type = typename std::remove_reference<T>::type;
    component_base *front = first;
    while(front != nullptr) {
      component_node<type> * ret = dynamic_cast<component_node<type>*>(front);
      if(ret != nullptr) {
        component_base *new_prev = ret->prev, *new_next = ret->next;
        if(new_prev != nullptr) {
          new_prev->next = ret->next;
        } else {
          first = ret->next;
        }
        
        if(new_next != nullptr) {
          new_next->prev = ret->prev;
        } else {
          last = ret->prev;
        }
        delete ret;
        return;
      }
      front = front->next;
    }
  }
}

class Component {
  friend class GameObject;
	int owner_id;
	public:
	virtual void Load() {}
	virtual void Init() {}
	virtual void Update() {}
	virtual void Free() {}
	virtual void Load() {}
	
	virtual void Serialise(Serialiser &) {}
	virtual void Deserialise(Serialiser &) {}
	virtual void EditorUpdate() {}
	GameObject *gameObject() const {
		// get game object from level based on owner_id;
	}
};

#endif