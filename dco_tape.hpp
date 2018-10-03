#ifndef DCO_TAPE_HPP
#define DCO_TAPE_HPP

namespace dco {
namespace tapes {

template<int ADLEVEL, class DCO_PARTIALREAL>
struct dco_tape {
    int* _intstack;
    int* _tos_intstack;
    DCO_PARTIALREAL *_partialstack;
    DCO_PARTIALREAL *_tos_partialstack;
    long int _progvarcounter;
    int _edgecounter;

    dco_tape() : _intstack(nullptr) , _tos_intstack(nullptr),
        _partialstack(nullptr), _tos_partialstack(nullptr),
        _progvarcounter(0), _edgecounter(0) { }

    template<class DCO_AT>
    void register_variable(DCO_AT &x) {
        _progvarcounter ++;
        x._data_._tape_index=_progvarcounter;
    }

    void allocate(const size_t stacksize=1000) {
        _intstack = new int[stacksize];
        _tos_intstack = _intstack;
        _partialstack = new DCO_PARTIALREAL[stacksize];
        _tos_partialstack = _partialstack;
    }

    void interpret(DCO_PARTIALREAL* adjvec) {
        int *cur_intstack = _tos_intstack;
        DCO_PARTIALREAL *cur_partials = _tos_partialstack;
        int curtapeidx = _progvarcounter;

        while(cur_intstack!=_intstack) {
            const DCO_PARTIALREAL &adj = adjvec[curtapeidx];
            curtapeidx--;
            cur_intstack--;
            int edgecount = *cur_intstack;

            for(int i=0;i<edgecount;++i) {
                cur_intstack--;
                cur_partials--;
                adjvec[*cur_intstack] += adj * (*cur_partials);
            }

            std::cout << "dstack = " << cur_intstack - _intstack << std::endl;
        }
    }

    void debug_print() {
    {
        int count = _tos_intstack - _intstack;
        std::cout << "intstack={";
        for(int i=0;i<count;++i) {
            std::cout << _intstack[i] << ",";
        }
        std::cout << "}" << std::endl;
     }

     {
        int count = _tos_partialstack - _partialstack;
        std::cout << "intstack={";
        for(int i=0;i<count;++i) {
            std::cout << _partialstack[i] << ",";
        }
        std::cout << "}" << std::endl;
     }
    }
};

}
}

#endif // DCO_TAPE_HPP
