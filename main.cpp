#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include <dco_tape.hpp>

namespace dco {

int count1 = 0;
int count2 = 0;
std::vector<int> tags {0};
std::vector<std::vector<std::string>> ads;
std::vector<std::vector<std::string>> nodes;

namespace operations {
    template<class TREAL1, class TREAL2>
    struct dco_add {
        typedef decltype(TREAL1{}+TREAL2{}) VALUE_TYPE;
        typedef decltype(1) PARTIAL1_TYPE;
        typedef decltype(1) PARTIAL2_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=" + nodes[i][2] + "+" + nodes[i][3];

                    return expr;
                }
            }

            return "";
        }

        static std::string pval1str() {
            return "1";
        }

        static std::string pval2str() {
            return "1";
        }

        static VALUE_TYPE eval(const TREAL1 &x1, const  TREAL2 &x2) {
            return x1+x2;
        }
        static PARTIAL1_TYPE dx1(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return 1;
        }
        static PARTIAL2_TYPE dx2(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return 1;
        }
    };

    template<class TREAL1, class TREAL2>
    struct dco_sub {
        typedef decltype(TREAL1{}-TREAL2{}) VALUE_TYPE;
        typedef decltype(1) PARTIAL1_TYPE;
        typedef decltype(-1) PARTIAL2_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=" + nodes[i][2] + "-" + nodes[i][3];

                    return expr;
                }
            }

            return "";
        }

        static std::string pval1str() {
            return "1";
        }

        static std::string pval2str() {
            return "-1";
        }

        static VALUE_TYPE eval(const TREAL1 &x1, const  TREAL2 &x2) {
            return x1-x2;
        }
        static PARTIAL1_TYPE dx1(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return 1;
        }
        static PARTIAL2_TYPE dx2(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return -1;
        }
    };

    template<class TREAL1, class TREAL2>
    struct dco_mul {
        typedef decltype(TREAL1{}*TREAL2{}) VALUE_TYPE;
        typedef decltype(TREAL2{}) PARTIAL1_TYPE;
        typedef decltype(TREAL1{}) PARTIAL2_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=" + nodes[i][2] + "*" + nodes[i][3];

                    return expr;
                }
            }

            return "";
        }

        static std::string pval1str() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2))
                    return nodes[i][3];
            }

            return "";
        }

        static std::string pval2str() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2))
                    return nodes[i][2];
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL1 &x1, const  TREAL2 &x2) {
            return x1*x2;
        }
        static PARTIAL1_TYPE dx1(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return x2;
        }
        static PARTIAL2_TYPE dx2(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return x1;
        }
    };

    template<class TREAL1, class TREAL2>
    struct dco_div {
        typedef decltype(TREAL1{}/TREAL2{}) VALUE_TYPE;
        typedef decltype(1/TREAL2{}) PARTIAL1_TYPE;
        typedef decltype(-TREAL1{}/(TREAL2{}*TREAL2{})) PARTIAL2_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=" + nodes[i][2] + "/" + nodes[i][3];

                    return expr;
                }
            }

            return "";
        }

        static std::string pval1str() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = "1/" + nodes[i][3];

                    return expr;
                }
            }

            return "";
        }

        static std::string pval2str() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = "-" + nodes[i][2] + "/(" + nodes[i][3] + "*" + nodes[i][3] + ")";

                    return expr;
                }
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL1 &x1, const  TREAL2 &x2) {
            return x1/x2;
        }
        static PARTIAL1_TYPE dx1(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return 1/x2;
        }
        static PARTIAL2_TYPE dx2(const TREAL1 &x1, const  TREAL2 &x2) {
            (void)x1; (void)x2;
            return -x1/(x2*x2);
        }
    };

    template<class TREAL>
    struct dco_sin{
        typedef decltype(sin(TREAL{})) VALUE_TYPE;
        typedef decltype(cos(TREAL{})) PARTIAL_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=sin(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static std::string pvalstr() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = "cos(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL &x) {
            return std::sin(x);
        }

        static PARTIAL_TYPE dx(const TREAL &x) {
            return std::cos(x);
        }
    };

    template<class TREAL>
    struct dco_cos{
        typedef decltype(cos(TREAL{})) VALUE_TYPE;
        typedef decltype(-sin(TREAL{})) PARTIAL_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=cos(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static std::string pvalstr() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = "-sin(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL &x) {
            return std::cos(x);
        }

        static PARTIAL_TYPE dx(const TREAL &x) {
            return -std::sin(x);
        }
    };

    template<class TREAL>
    struct dco_tan{
        typedef decltype(tan(TREAL{})) VALUE_TYPE;
        typedef decltype(1/(cos(TREAL{})*cos(TREAL{}))) PARTIAL_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=tan(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static std::string pvalstr() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = "1/(cos(" + nodes[i][2] + ")" + "*cos(" + nodes[i][2] + "))";

                    return expr;
                }
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL &x) {
            return std::tan(x);
        }

        static PARTIAL_TYPE dx(const TREAL &x) {
            return 1/(std::cos(x)*std::cos(x));
        }
    };

    template<class TREAL>
    struct dco_exp{
        typedef decltype(exp(TREAL{})) VALUE_TYPE;
        typedef decltype(exp(TREAL{})) PARTIAL_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=exp(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static std::string pvalstr() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = "exp(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL &x) {
            return std::exp(x);
        }

        static PARTIAL_TYPE dx(const TREAL &x) {
            return std::exp(x);
        }
    };

    template<class TREAL>
    struct dco_log{
        typedef decltype(log(TREAL{})) VALUE_TYPE;
        typedef decltype(1/TREAL{}) PARTIAL_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=log(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static std::string pvalstr() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = "1/" + nodes[i][2];

                    return expr;
                }
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL &x) {
            return std::log(x);
        }

        static PARTIAL_TYPE dx(const TREAL &x) {
            return 1/x;
        }
    };

    template<class TREAL>
    struct dco_abs{
        typedef decltype(abs(TREAL{})) VALUE_TYPE;
        typedef decltype(TREAL{}/abs(TREAL{})) PARTIAL_TYPE;

        static std::string evalstr() {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][1] + "=abs(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static std::string pvalstr() {
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    std::string expr = nodes[i][2] + "/abs(" + nodes[i][2] + ")";

                    return expr;
                }
            }

            return "";
        }

        static VALUE_TYPE eval(const TREAL &x) {
            return std::abs(x);
        }

        static PARTIAL_TYPE dx(const TREAL &x) {
            return x/std::abs(x);
        }
    };
}

namespace data {

template<int ADLEVEL, class DCO_PARTIAL_TYPE,
         tapes::dco_tape<ADLEVEL, DCO_PARTIAL_TYPE> &global_tape>
struct adjoint_data {
    int _tape_index;

    adjoint_data() :  _tape_index(0) {}
    template<class TPARTIAL, class TARG>
    void _traverse_with_partials(const TPARTIAL &pval, TARG &x) const {
        *global_tape._tos_intstack = x._data_._tape_index;
        global_tape._tos_intstack++;
        *global_tape._tos_partialstack = pval;
        global_tape._tos_partialstack++;
        global_tape._edgecounter++;
    }

    template<class TLHS>
    static void finish(TLHS &lhs) {
        *global_tape._tos_intstack = global_tape._edgecounter;
        global_tape._tos_intstack++;
        global_tape._edgecounter=0;
        global_tape._progvarcounter++;
        lhs._data_._tape_index = global_tape._progvarcounter;
    }

};

}

namespace types {

template<int ADLEVEL, class DCO_AARG,class DCO_OP>
struct unary_intermediate;
template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
struct binary_intermediate_aa;

    template<int ADLEVEL, class DCO_REAL, class DCO_DATA>
    struct active_type {
        DCO_REAL _value_;
        DCO_DATA _data_;

        active_type() : _value_(0) { }
        active_type(const DCO_REAL &value) : _value_(value) { }


        long _uniqueidx() const {
            return _data_._tape_index;
        }

        template<class DCO_AARG, class DCO_OP>
        active_type& operator = (const unary_intermediate<ADLEVEL, DCO_AARG, DCO_OP> &rhs) {
            std::vector<std::string> prev;
            rhs._traverse_with_partials(1.0, prev);
            std::ofstream outfile("tmp.dot");
            outfile << "graph {" << std::endl;
            rhs._writedotfile(outfile);
            outfile << "}";
            DCO_DATA::finish(*this);
            this->_value_ = rhs._value_;
            return *this;
        }

        template<class DCO_AARG1, class DCO_AARG2, class DCO_OP>
        active_type& operator = (
                const binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP> &rhs) {
            std::vector<std::string> prev;
            rhs._traverse_with_partials(1.0, prev);
            std::ofstream outfile("tmp.dot");
            outfile << "graph {" << std::endl;
            rhs._writedotfile(outfile);
            outfile << "}";
            DCO_DATA::finish(*this);
            this->_value_ = rhs._value_;
            return *this;
        }

        template<class TPARTIAL>
        void _traverse_with_partials(const TPARTIAL &pval, std::vector<std::string> &prev) const {
            //std::cout << "insert new edge to " << _data_._tape_index << " with partial=" << pval << std::endl;
            bool exist = false;
            count1++;
            std::vector<std::string> node;
            node.push_back(std::to_string(count1));

            for (unsigned i = 0; i < ads.size(); i++) {
                if (ads[i].front() == std::to_string(_data_._tape_index)) {
                    node.push_back(ads[i].back());
                    exist = true;

                    if (!prev.empty())
                        prev.push_back(ads[i].back());
                }
            }

            if (!exist) {
                node.push_back("t"+std::to_string(count1));

                std::vector<std::string> ad;
                ad.push_back(std::to_string(_data_._tape_index));
                ad.push_back("t"+std::to_string(tags.back()+1));
                ads.push_back(ad);

                if (!prev.empty())
                    prev.push_back("t"+std::to_string(tags.back()+1));
                tags.push_back(tags.back()+1);
            }

            _data_._traverse_with_partials(pval, *this);

            nodes.push_back(node);
        }

        void _writedotfile(std::ofstream &outfile) const {
            count2++;
            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2))
                    outfile << nodes[i][1] << "[label=\"" << nodes[i][1] << "\"]" << std::endl;
            }
        }
    };

    template<int ADLEVEL, class DCO_AARG,class DCO_OP>
    struct unary_intermediate {
        typename DCO_OP::VALUE_TYPE _value_;
        const DCO_AARG& _arg_;

        unary_intermediate(const DCO_AARG &x)
            : _value_(DCO_OP::eval(x._value_)), _arg_(x) { }

        template<class TPARTIAL>
        void _traverse_with_partials(const TPARTIAL &pval, std::vector<std::string> &prev) const {
            count1++;
            std::vector<std::string> node;
            node.push_back(std::to_string(count1));
            node.push_back("t"+std::to_string(tags.back()+1));
            if (!prev.empty())
                prev.push_back("t"+std::to_string(tags.back()+1));
            tags.push_back(tags.back()+1);

            _arg_._traverse_with_partials(pval * DCO_OP::dx(_arg_._value_), node);

            nodes.push_back(node);
        }


        long _uniqueidx() const {
            return (long)(this);
        }

        void _writedotfile(std::ofstream &outfile) const {
            std::string evalstr = DCO_OP::evalstr();
            std::string pvalstr = DCO_OP::pvalstr();
            std::string head;
            std::string tail;

            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    head = nodes[i][1];
                    tail = nodes[i][2];
                }
            }

            outfile << head << "[label=\"" << evalstr << "\"]" << std::endl;
            outfile << head << "--" << tail << "[label=\"" << pvalstr << "\"]" << std::endl;
            _arg_._writedotfile(outfile);

        }
    };

    template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
    struct binary_intermediate_aa {
        typename DCO_OP::VALUE_TYPE _value_;
        const DCO_AARG1& _arg1_;
        const DCO_AARG2& _arg2_;

        binary_intermediate_aa(const DCO_AARG1 &x1, const DCO_AARG2 &x2)
            : _value_(DCO_OP::eval(x1._value_, x2._value_)), _arg1_(x1), _arg2_(x2) { }

        template<class TPARTIAL>
        void _traverse_with_partials(const TPARTIAL &pval, std::vector<std::string> &prev) const {
            count1++;
            std::vector<std::string> node;
            node.push_back(std::to_string(count1));
            node.push_back("t"+std::to_string(tags.back()+1));
            if (!prev.empty())
                prev.push_back("t"+std::to_string(tags.back()+1));
            tags.push_back(tags.back()+1);

            _arg1_._traverse_with_partials(pval * DCO_OP::dx1(_arg1_._value_, _arg2_._value_), node);
            _arg2_._traverse_with_partials(pval * DCO_OP::dx2(_arg1_._value_, _arg2_._value_), node);

            nodes.push_back(node);
        }

        long _uniqueidx() const {
            return (long)(this);
        }

        void _writedotfile(std::ofstream &outfile) const {
            std::string evalstr = DCO_OP::evalstr();
            std::string pval1str = DCO_OP::pval1str();
            std::string pval2str = DCO_OP::pval2str();
            std::string head;
            std::string tail1;
            std::string tail2;

            for (unsigned i = 0; i < nodes.size(); i++) {
                if (nodes[i].front() == std::to_string(count2)) {
                    head = nodes[i][1];
                    tail1 = nodes[i][2];
                    tail2 = nodes[i][3];
                }
            }

            outfile << head << "[label=\"" << evalstr << "\"]" << std::endl;
            outfile << head << "--" << tail1 << "[label=\"" << pval1str << "\"]" << std::endl;
            outfile << head << "--" << tail2 << "[label=\"" << pval2str << "\"]" << std::endl;
            _arg1_._writedotfile(outfile);
            _arg2_._writedotfile(outfile);
        }
    };

    // addition
    template<int ADLEVEL, class DCO_REAL1, class DCO_DATA1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator + (const active_type<ADLEVEL, DCO_REAL1, DCO_DATA1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL1, DCO_DATA1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_add<DCO_REAL1, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator + (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_add<typename DCO_A1_OP::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator + (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_add<DCO_REAL2, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator + (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_add<typename DOC_OP1::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator + (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_add<DCO_REAL2, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator + (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_add<typename DCO_A1_OP::VALUE_TYPE, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator + (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_add<typename DOC_OP1::VALUE_TYPE, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_A2_AA, class DCO_A2_OP>
    auto operator + (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP>,
                operations::dco_add<typename DCO_A1_OP::VALUE_TYPE, typename DCO_A2_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A3, class DCO_A4, class DOC_OP2>
    auto operator + (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2>,
                operations::dco_add<typename DOC_OP1::VALUE_TYPE, typename DOC_OP2::VALUE_TYPE>>(x1,x2);
    }

    // substraction
    template<int ADLEVEL, class DCO_REAL1, class DCO_DATA1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator - (const active_type<ADLEVEL, DCO_REAL1, DCO_DATA1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL1, DCO_DATA1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_sub<DCO_REAL1, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator - (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_sub<typename DCO_A1_OP::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator - (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_sub<DCO_REAL2, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator - (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_sub<typename DOC_OP1::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator - (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_sub<DCO_REAL2, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator - (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_sub<typename DCO_A1_OP::VALUE_TYPE, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator - (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_sub<typename DOC_OP1::VALUE_TYPE, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_A2_AA, class DCO_A2_OP>
    auto operator - (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP>,
                operations::dco_add<typename DCO_A1_OP::VALUE_TYPE, typename DCO_A2_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A3, class DCO_A4, class DOC_OP2>
    auto operator - (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2>,
                operations::dco_add<typename DOC_OP1::VALUE_TYPE, typename DOC_OP2::VALUE_TYPE>>(x1,x2);
    }

    // multiplication
    template<int ADLEVEL, class DCO_REAL1, class DCO_DATA1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator * (const active_type<ADLEVEL, DCO_REAL1, DCO_DATA1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL1, DCO_DATA1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_mul<DCO_REAL1, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator * (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_mul<typename DCO_A1_OP::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator * (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_mul<DCO_REAL2, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator * (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_mul<typename DOC_OP1::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator * (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_mul<DCO_REAL2, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator * (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_mul<typename DCO_A1_OP::VALUE_TYPE, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator * (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_mul<typename DOC_OP1::VALUE_TYPE, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_A2_AA, class DCO_A2_OP>
    auto operator * (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP>,
                operations::dco_add<typename DCO_A1_OP::VALUE_TYPE, typename DCO_A2_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A3, class DCO_A4, class DOC_OP2>
    auto operator * (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2>,
                operations::dco_add<typename DOC_OP1::VALUE_TYPE, typename DOC_OP2::VALUE_TYPE>>(x1,x2);
    }

    // division
    template<int ADLEVEL, class DCO_REAL1, class DCO_DATA1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator / (const active_type<ADLEVEL, DCO_REAL1, DCO_DATA1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL1, DCO_DATA1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_div<DCO_REAL1, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator / (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_div<typename DCO_A1_OP::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator / (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_div<DCO_REAL2, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator / (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                operations::dco_div<typename DOC_OP1::VALUE_TYPE, DCO_REAL2>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_REAL2, class DCO_DATA2>
    auto operator / (const active_type<ADLEVEL, DCO_REAL2, DCO_DATA2> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL2, DCO_DATA2>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_div<DCO_REAL2, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator / (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                operations::dco_div<typename DCO_A1_OP::VALUE_TYPE, typename DOC_OP1::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A1_AA, class DCO_A1_OP>
    auto operator / (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_div<typename DOC_OP1::VALUE_TYPE, typename DCO_A1_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP,
                          class DCO_A2_AA, class DCO_A2_OP>
    auto operator / (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x1,
                     const unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                unary_intermediate<ADLEVEL, DCO_A2_AA, DCO_A2_OP>,
                operations::dco_add<typename DCO_A1_OP::VALUE_TYPE, typename DCO_A2_OP::VALUE_TYPE>>(x1,x2);
    }

    template<int ADLEVEL, class DCO_A1, class DCO_A2, class DOC_OP1,
                          class DCO_A3, class DCO_A4, class DOC_OP2>
    auto operator / (const binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1> &x1,
                     const binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2> &x2) {
        return binary_intermediate_aa<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_A1, DCO_A2, DOC_OP1>,
                binary_intermediate_aa<ADLEVEL, DCO_A3, DCO_A4, DOC_OP2>,
                operations::dco_add<typename DOC_OP1::VALUE_TYPE, typename DOC_OP2::VALUE_TYPE>>(x1,x2);
    }

    //sin
    template<int ADLEVEL, class DCO_REAL, class DCO_DATA>
    auto sin (const active_type<ADLEVEL, DCO_REAL, DCO_DATA> &x) {
        return unary_intermediate<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL, DCO_DATA>,
                operations::dco_sin<DCO_REAL>>(x);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP>
    auto sin (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x) {
        return unary_intermediate<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_sin<typename DCO_A1_OP::VALUE_TYPE>>(x);
    }

    template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
    auto sin (const binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP> &x) {
        return unary_intermediate<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP>,
                operations::dco_sin<typename DCO_OP::VALUE_TYPE>>(x);
    }

    //cos
    template<int ADLEVEL, class DCO_REAL, class DCO_DATA>
    auto cos (const active_type<ADLEVEL, DCO_REAL, DCO_DATA> &x) {
        return unary_intermediate<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL, DCO_DATA>,
                operations::dco_cos<DCO_REAL>>(x);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP>
    auto cos (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x) {
        return unary_intermediate<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_cos<typename DCO_A1_OP::VALUE_TYPE>>(x);
    }

    template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
    auto cos (const binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP> &x) {
        return unary_intermediate<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP>,
                operations::dco_cos<typename DCO_OP::VALUE_TYPE>>(x);
    }

    //tan
    template<int ADLEVEL, class DCO_REAL, class DCO_DATA>
    auto tan (const active_type<ADLEVEL, DCO_REAL, DCO_DATA> &x) {
        return unary_intermediate<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL, DCO_DATA>,
                operations::dco_tan<DCO_REAL>>(x);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP>
    auto tan (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x) {
        return unary_intermediate<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_tan<typename DCO_A1_OP::VALUE_TYPE>>(x);
    }

    template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
    auto tan (const binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP> &x) {
        return unary_intermediate<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP>,
                operations::dco_tan<typename DCO_OP::VALUE_TYPE>>(x);
    }

    //exp
    template<int ADLEVEL, class DCO_REAL, class DCO_DATA>
    auto exp (const active_type<ADLEVEL, DCO_REAL, DCO_DATA> &x) {
        return unary_intermediate<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL, DCO_DATA>,
                operations::dco_exp<DCO_REAL>>(x);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP>
    auto exp (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x) {
        return unary_intermediate<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_exp<typename DCO_A1_OP::VALUE_TYPE>>(x);
    }

    template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
    auto exp (const binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP> &x) {
        return unary_intermediate<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP>,
                operations::dco_exp<typename DCO_OP::VALUE_TYPE>>(x);
    }

    //log
    template<int ADLEVEL, class DCO_REAL, class DCO_DATA>
    auto log (const active_type<ADLEVEL, DCO_REAL, DCO_DATA> &x) {
        return unary_intermediate<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL, DCO_DATA>,
                operations::dco_log<DCO_REAL>>(x);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP>
    auto log (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x) {
        return unary_intermediate<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_log<typename DCO_A1_OP::VALUE_TYPE>>(x);
    }

    template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
    auto log (const binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP> &x) {
        return unary_intermediate<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP>,
                operations::dco_log<typename DCO_OP::VALUE_TYPE>>(x);
    }

    //abs
    template<int ADLEVEL, class DCO_REAL, class DCO_DATA>
    auto abs (const active_type<ADLEVEL, DCO_REAL, DCO_DATA> &x) {
        return unary_intermediate<ADLEVEL,
                active_type<ADLEVEL, DCO_REAL, DCO_DATA>,
                operations::dco_abs<DCO_REAL>>(x);
    }

    template<int ADLEVEL, class DCO_A1_AA, class DCO_A1_OP>
    auto abs (const unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP> &x) {
        return unary_intermediate<ADLEVEL,
                unary_intermediate<ADLEVEL, DCO_A1_AA, DCO_A1_OP>,
                operations::dco_abs<typename DCO_A1_OP::VALUE_TYPE>>(x);
    }

    template<int ADLEVEL, class DCO_AARG1, class DCO_AARG2, class DCO_OP>
    auto abs (const binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP> &x) {
        return unary_intermediate<ADLEVEL,
                binary_intermediate_aa<ADLEVEL, DCO_AARG1, DCO_AARG2, DCO_OP>,
                operations::dco_abs<typename DCO_OP::VALUE_TYPE>>(x);
    }
}

namespace a1s {
    typedef tapes::dco_tape<1,double> tape_t;
    tape_t  global_tape;
    typedef dco::data::adjoint_data<1,double, global_tape> data_t;

    typedef types::active_type<1,double, data_t> dbl_type;
    typedef types::active_type<1,float, data_t> flt_type;
}
}

int main() {
    dco::a1s::dbl_type x1;
    dco::a1s::flt_type x2;
    dco::a1s::flt_type x3;
    dco::a1s::dbl_type y;

    dco::a1s::global_tape.allocate();
    dco::a1s::global_tape.register_variable(x1);
    dco::a1s::global_tape.register_variable(x2);
    dco::a1s::global_tape.register_variable(x3);

    y = log((exp(sin(x1)+x2)-cos(x3)/tan(x1))*abs(x2));
    system("xdot tmp.dot");

/*  
    dco::a1s::global_tape.debug_print();
    std::cout << "y = " << y._value_ << std::endl;

    double *adjvector = new double[1000]();
    adjvector[y._data_._tape_index] = 1.0;
    dco::a1s::global_tape.interpret(adjvector);
    std::cout << " dx1=" << adjvector[x1._data_._tape_index]  << std::endl;
    std::cout << " dx2=" << adjvector[x2._data_._tape_index]  << std::endl;
*/

    return 0;
}
