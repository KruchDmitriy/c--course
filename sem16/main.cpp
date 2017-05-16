#include <cstdio>
#include <cassert>
#include <vector>
#include <utility>
#include <functional>
#include <cstdlib>
#include <ctime>

#undef CONFIG_DEBUG
#define CONFIG_DEBUG 1

#undef CONFIG_LOG_LEVEL
#define CONFIG_LOG_LEVEL 0

#include "myassert.h"
#include "mylog.h"
#include "instructions.h"


int main()
{
    std::srand(std::time(0));

    // Part 1
    bool assertion1_evaluated = false;
    myassert(assertion1_evaluated = true);
    assert((CONFIG_DEBUG && assertion1_evaluated)
            || (!CONFIG_DEBUG && !assertion1_evaluated));

    // Part 2
    LOG(INFO, "==============================");
    LOG(DEBUG, "%d %d %d %s", 1, 2, 3, "debug");
    LOG(INFO, "%d %d %s", 1, 2, "info");
    LOG(WARN, "%d %s", 1, "warning");
    LOG(ERROR, "%s", "error!");

    // Part 3
    LOG(INFO, "==============================");
    LOG(INFO, "%s %s", instruction_name(INST_DADD),
            instruction_description(INST_DADD));
    LOG(INFO, "%s %s", instruction_name(INST_IPRINT),
            instruction_description(INST_IPRINT));

    // Part 4
    auto inst_stream = gen_rnd_instrs(20);
    log_instrs(inst_stream);

//    // Part 5
//    LOG(INFO, "==============================");
//    std::vector<unsigned char> super_code = {
//        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
//        INST_ILOAD, 0, 0, 0xdd, 0, 0, 0, 0, 0,
//        INST_IADD,
//        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
//        INST_IADD,
//        INST_IPRINT
//    }; // 0xff + 0xdd0000 + 0xff = 0xdd01fe
//    log_instrs(super_code);
//    exec_instrs(super_code);

    return 0;
}
