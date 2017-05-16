#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "instructions.h"

#define CONFIG_LOG_LEVEL 0
#include "mylog.h"

namespace {
    const char* instr_name[INST_LAST + 1] = {
        #define ARRAY_ELEM(b, d, l) #b,
        FOR_INSTRUCTIONS(ARRAY_ELEM)
        #undef ARRAY_ELEM
        "LAST"
    };

    const char* instr_desc[INST_LAST + 1] = {
        #define ARRAY_ELEM(b, d, l) d,
        FOR_INSTRUCTIONS(ARRAY_ELEM)
        #undef ARRAY_ELEM
        "LAST_DESC"
    };

    const int instr_len[INST_LAST + 1] = {
        #define ARRAY_ELEM(b, d, l) l,
        FOR_INSTRUCTIONS(ARRAY_ELEM)
        #undef ARRAY_ELEM
        -1
    };
}

const char* instruction_name(InstructionCode code) {
    return instr_name[code];
}

const char* instruction_description(InstructionCode code) {
    return instr_desc[code];
}

int instruction_length(InstructionCode code) {
    return instr_len[code] - 1;
}

std::vector<unsigned char> gen_rnd_instrs(const size_t instr_cnt) {
    std::vector<unsigned char> result;

    for (size_t i = 0; i < instr_cnt; i++) {
        int code = rand() % (INST_LAST + 1);
        result.push_back(code);
        InstructionCode instrCode = static_cast<InstructionCode>(code);

        for (int j = 0; j < instruction_length(instrCode); j++) {
            result.push_back(rand() % 256);
        }
    }

    return result;
}

void log_instrs(const std::vector<unsigned char> &instrs) {
    std::stringstream ss;

    for (size_t i = 0; i < instrs.size(); i++) {
        InstructionCode code = static_cast<InstructionCode>(instrs[i]);

        for (int j = 0; j < instruction_length(code); j++, i++) {
            ss << " " << std::hex << std::showbase << (uint32_t)instrs[i];
        }
        LOG(INFO, "Instruction name: %s. Args:%s", instruction_name(code), ss.str().c_str());
        ss.str("");
    }
}
