#include <string>
#include <vector>

#include <stdbool.h>

using namespace std;

#ifndef __INCL_CMDARG
#define __INCL_CMDARG

class CmdArg {
    private:
        int argPointer;
        vector<string> args;

    public:
        CmdArg(int argc, char ** argv);
        ~CmdArg();

        int getNumArgs();
        bool hasMoreArgs();
        string nextArg();

        string getArg(int i);
};
 
#endif
