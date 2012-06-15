/* 
 * This file is part of the pebil project.
 * 
 * Copyright (c) 2010, University of California Regents
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
##################################################################
DWARF Line Info
Offset	Length  Name                    Description
0	8	lr_address              the program counter of a machine instruction
8       4       lr_op_index
12	4	lr_file                 identity of the source file
16	4	lr_line                 source line number
20	4	lr_column               column number within a source line
24	4	lr_isa                  which instruction set is this instruction a part of
28      4       lr_discriminator        differentiates between multiple blocks for a single file,line,column
32	1	lr_is_stmt              is a recommended breakpoint location
33	1	lr_basic_block          is this the beginning of a basic block
34	1	lr_end_sequence         is this the first byte after the end of a sequence
35	1	lr_prologue_end         is this an address where execution should be suspended for a function entry
36	1	lr_epilogue_begin       is this an address where execution should be suspended for a function exit
##################################################################
DWARF Line Info Header
Offset	Length  Name       
0	4	li_length       
4       2       li_version
6       4       li_header_length
10      1       li_min_insn_length
11      1       li_default_is_stmt
12      1       li_line_base
16      1       li_line_range
17      1       li_opcode_base
##################################################################
*/

#define LINEINFO_MACROS_CLASS(__str) /** __str **/ \
    GET_FIELD_CLASS(uint64_t,lr_address); \
    GET_FIELD_CLASS(uint32_t,lr_op_index); \
    GET_FIELD_CLASS(uint32_t,lr_file); \
    GET_FIELD_CLASS(uint32_t,lr_line); \
    GET_FIELD_CLASS(uint32_t,lr_column); \
    GET_FIELD_CLASS(uint32_t,lr_isa); \
    GET_FIELD_CLASS(uint8_t,lr_is_stmt); \
    GET_FIELD_CLASS(uint8_t,lr_basic_block); \
    GET_FIELD_CLASS(uint8_t,lr_end_sequence); \
    GET_FIELD_CLASS(uint8_t,lr_prologue_end); \
    GET_FIELD_CLASS(uint8_t,lr_epilogue_begin); \
    GET_FIELD_CLASS(uint32_t,lr_discriminator); \
        \
    SET_FIELD_CLASS(uint64_t,lr_address); \
    SET_FIELD_CLASS(uint32_t,lr_op_index); \
    SET_FIELD_CLASS(uint32_t,lr_file); \
    SET_FIELD_CLASS(uint32_t,lr_line); \
    SET_FIELD_CLASS(uint32_t,lr_column); \
    SET_FIELD_CLASS(uint32_t,lr_isa); \
    SET_FIELD_CLASS(uint8_t,lr_is_stmt); \
    SET_FIELD_CLASS(uint8_t,lr_basic_block); \
    SET_FIELD_CLASS(uint8_t,lr_end_sequence); \
    SET_FIELD_CLASS(uint8_t,lr_prologue_end); \
    SET_FIELD_CLASS(uint8_t,lr_epilogue_begin); \
    SET_FIELD_CLASS(uint32_t,lr_discriminator); \
        \
    INCREMENT_FIELD_CLASS(uint64_t,lr_address); \
    INCREMENT_FIELD_CLASS(uint32_t,lr_op_index); \
    INCREMENT_FIELD_CLASS(uint32_t,lr_file); \
    INCREMENT_FIELD_CLASS(uint32_t,lr_line); \
    INCREMENT_FIELD_CLASS(uint32_t,lr_column); \
    INCREMENT_FIELD_CLASS(uint32_t,lr_isa); \
    INCREMENT_FIELD_CLASS(uint8_t,lr_is_stmt); \
    INCREMENT_FIELD_CLASS(uint8_t,lr_basic_block); \
    INCREMENT_FIELD_CLASS(uint8_t,lr_end_sequence); \
    INCREMENT_FIELD_CLASS(uint8_t,lr_prologue_end); \
    INCREMENT_FIELD_CLASS(uint8_t,lr_epilogue_begin); \
    INCREMENT_FIELD_CLASS(uint32_t,lr_discriminator);


#define LINEINFOTABLE_MACROS_CLASS(__str) /** __str **/ \
    GET_FIELD_CLASS(uint64_t,li_length); \
    GET_FIELD_CLASS(uint16_t,li_version); \
    GET_FIELD_CLASS(uint64_t,li_header_length); \
    GET_FIELD_CLASS(uint8_t,li_min_insn_length); \
    GET_FIELD_CLASS(uint8_t,li_default_is_stmt); \
    GET_FIELD_CLASS(uint8_t,li_line_base); \
    GET_FIELD_CLASS(uint8_t,li_line_range); \
    GET_FIELD_CLASS(uint8_t,li_opcode_base); \
        \
    SET_FIELD_CLASS(uint64_t,li_length); \
    SET_FIELD_CLASS(uint16_t,li_version); \
    SET_FIELD_CLASS(uint64_t,li_header_length); \
    SET_FIELD_CLASS(uint8_t,li_min_insn_length); \
    SET_FIELD_CLASS(uint8_t,li_default_is_stmt); \
    SET_FIELD_CLASS(uint8_t,li_line_base); \
    SET_FIELD_CLASS(uint8_t,li_line_range); \
    SET_FIELD_CLASS(uint8_t,li_opcode_base); \
        \
    INCREMENT_FIELD_CLASS(uint64_t,li_length); \
    INCREMENT_FIELD_CLASS(uint16_t,li_version); \
    INCREMENT_FIELD_CLASS(uint64_t,li_header_length); \
    INCREMENT_FIELD_CLASS(uint8_t,li_min_insn_length); \
    INCREMENT_FIELD_CLASS(uint8_t,li_default_is_stmt); \
    INCREMENT_FIELD_CLASS(uint8_t,li_line_base); \
    INCREMENT_FIELD_CLASS(uint8_t,li_line_range); \
    INCREMENT_FIELD_CLASS(uint8_t,li_opcode_base); \

