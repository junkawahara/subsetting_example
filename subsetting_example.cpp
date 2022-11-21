
#include "tdzdd/DdSpec.hpp"
#include "tdzdd/DdStructure.hpp"

#include "tdzdd/spec/SizeConstraint.hpp"

using namespace tdzdd;

struct SEData {
    tdzdd::NodeId node;
    bool parity;
};

bool operator==(const SEData& data1, const SEData& data2) {
    return data1.node == data2.node && data1.parity == data2.parity;
}

class SubsettingExampleSpec : public tdzdd::DdSpec<SubsettingExampleSpec,
                                                   SEData,
                                                   2> {
private:
    tdzdd::DdStructure<2> dd_;
    const int n_;
public:
    SubsettingExampleSpec(tdzdd::DdStructure<2> dd, int n) : dd_(dd), n_(n) { }

    int getRoot(SEData& data) const {
        tdzdd::NodeId node = dd_.root(); // get the root node
        int level = node.row(); // get the level of the root node

        if (level == 0 && node.col() == 0) { // 0-terminal
            return 0;
        } else {
            data.node = node;
            data.parity = false;
            return n_;
        }
    }

    int getChild(SEData& data, int level, int value) const {
        int f_level = data.node.row(); // get the level of the node
        
        while (f_level > 0 && f_level > level) {
            data.node = dd_.child(data.node, 0);
            f_level = data.node.row();
        }

        if (f_level == level) {
            data.node = dd_.child(data.node, value);
            if (data.node.row() == 0 && data.node.col() == 0) { // 0-terminal
                return 0;
            }
        }

        if (level > 0) {
            data.parity = !data.parity;
            return level - 1;
        } else {
            if (data.parity) {
                return -1; // 1-terminal
            } else {
                return 0; // 0-terminal
            }
        }
    }
};

int main(int argc, char** argv) {

    IntRange range(5, 10);
    tdzdd::SizeConstraint sc(10, &range);
    tdzdd::DdStructure<2> dd_f(sc);

    SubsettingExampleSpec subsetting_example_spec(dd_f, 10);
    tdzdd::DdStructure<2> dd_f_and_g(subsetting_example_spec);

    std::cout << "size = " << dd_f_and_g.size() << std::endl;
    std::cout << "card = " << dd_f_and_g.evaluate(tdzdd::ZddCardinality()) << std::endl;
    
    return 0;
}
