#include "../DNSS/DNSS.hpp"
namespace assas {
    class DNSS;
    typedef vector<Symbol> RegEx;

    /// \brief Interface of Reachability Solver.
    class ReachSolver {
    private:
        DNSS* dnss;
    public:
        ReachSolver() : dnss(nullptr) {}

        ReachSolver(DNSS* s) : dnss(s) {}

        virtual ~ReachSolver() {
        }

        virtual bool isReachable(const RegEx& regEx) = 0;

    };
}
