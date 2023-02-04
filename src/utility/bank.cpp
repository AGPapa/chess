// TODO: Figure out the right size for this
// TODO: Figure out the best way to test this

template<typename Item>
class Bank {
    public:
        Bank() {
            for (int i = 0; i < 20000; i++) {
                _available_indicies.push(i);
            }
        }

        std::pair<Item*, int> acquire() {
            int index = _available_indicies.front();
            _available_indicies.pop();
            return std::make_pair(&(_item_array[index]), index);
        }
        
        Item* access(int index) {
            return &(_item_array[index]);
        }

        void release(int index) {
            _available_indicies.push(index);
        }

     private:
        Item _item_array[20000];
        std::queue<int> _available_indicies;
        
};