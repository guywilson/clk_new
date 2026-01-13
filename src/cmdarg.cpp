#include <string>
#include <vector>
#include <exception>

#include <string.h>

#include "cmdarg.h"

using namespace std;

CmdArg::CmdArg(int argc, char ** argv) {
    for (int i = 1;i < argc;i++) {
        args.push_back(argv[i]);
    }

    argPointer = 0;
}

CmdArg::~CmdArg() {
    args.clear();
}

int CmdArg::getNumArgs() {
    return args.size();
}

bool CmdArg::hasMoreArgs() {
    return argPointer < getNumArgs();
}

string CmdArg::nextArg() {
    if (argPointer >= getNumArgs()) {
        throw exception();
    }

    return args[argPointer++];
}

string CmdArg::getArg(int i) {
    if (i >= getNumArgs()) {
        throw exception();
    }

    return args[i];
}
