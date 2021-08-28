#include <thread>
#include <atomic>
#include <list>
#include <map>

#include <iostream>

template<typename Key, typename Value>
class Cache {
    public:
        Cache(int max_size) {
            _max_size = max_size;
        }

        void add(Key k, std::unique_ptr<Value> v) {
            _mutex.lock();
            auto map_it = _map.find(k);
            if(map_it != _map.end()){
                _list.erase(map_it->second);
                _map.erase(map_it);
            }
            _list.push_front(std::make_pair(k, std::move(v)));
            _map.insert(std::make_pair(k, _list.begin()));

            if (_map.size() > _max_size) {
                auto last_it = _list.end();
                last_it--;
                _map.erase(last_it->first);
                _list.pop_back();
            }
            _mutex.unlock();
        }

        Value* get(Key k) {
            _mutex.lock();
            auto map_it = _map.find(k);
            if (map_it == _map.end()) {
                _mutex.unlock();
                return nullptr;
            }
            std::unique_ptr<Value> v_ptr = std::move(map_it->second->second);
            _list.erase(map_it->second);
            _map.erase(map_it);
            _list.push_front(std::make_pair(k, std::move(v_ptr)));
            _map.insert(std::make_pair(k, _list.begin()));
            Value* v = _list.front().second.get();
            _mutex.unlock();
            return v;
        }

     private:
        int _max_size;
        std::list<std::pair<Key, std::unique_ptr<Value>>> _list;
        std::map<Key, decltype(_list.begin())> _map;
        std::mutex _mutex;
};