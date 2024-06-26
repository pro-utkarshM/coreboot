/*
 *
 * Copyright 2013 Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _ARCH_EXCEPTION_H
#define _ARCH_EXCEPTION_H

#include <stddef.h>
#include <stdint.h>

void exception_init_asm(void);
void exception_dispatch(void);
void enable_interrupts(void);
void disable_interrupts(void);
/** Returns 1 if interrupts are enabled. */
int interrupts_enabled(void);

#if CONFIG(LP_ARCH_X86_64)
struct exception_state {
	/* Careful: x86/gdb.c currently relies on the size and order of regs. */
	struct {
		size_t reg_ax;
		size_t reg_bx;
		size_t reg_cx;
		size_t reg_dx;
		size_t reg_si;
		size_t reg_di;
		size_t reg_bp;
		size_t reg_sp;
		size_t reg_r8;
		size_t reg_r9;
		size_t reg_r10;
		size_t reg_r11;
		size_t reg_r12;
		size_t reg_r13;
		size_t reg_r14;
		size_t reg_r15;
		size_t reg_ip;
		size_t reg_flags;
		u32 cs;
		u32 ss;
		u32 ds;
		u32 es;
		u32 fs;
		u32 gs;
	} regs;
	size_t error_code;
	size_t vector;
} __packed;
#else
struct exception_state {
	/* Careful: x86/gdb.c currently relies on the size and order of regs. */
	struct {
		size_t reg_ax;
		size_t reg_cx;
		size_t reg_dx;
		size_t reg_bx;
		size_t reg_sp;
		size_t reg_bp;
		size_t reg_si;
		size_t reg_di;
		size_t reg_ip;
		size_t reg_flags;
		u32 cs;
		u32 ss;
		u32 ds;
		u32 es;
		u32 fs;
		u32 gs;
	} regs;
	size_t error_code;
	size_t vector;
} __packed;
#endif

extern struct exception_state *exception_state;

extern u8 exception_stack[];
extern u8 *exception_stack_end;

enum {
	EXC_DE = 0, /* Divide by zero */
	EXC_DB = 1, /* Debug */
	EXC_NMI = 2, /* Non maskable interrupt */
	EXC_BP = 3, /* Breakpoint */
	EXC_OF = 4, /* Overflow */
	EXC_BR = 5, /* Bound range */
	EXC_UD = 6, /* Invalid opcode */
	EXC_NM = 7, /* Device not available */
	EXC_DF = 8, /* Double fault */
	EXC_TS = 10, /* Invalid TSS */
	EXC_NP = 11, /* Segment not present */
	EXC_SS = 12, /* Stack */
	EXC_GP = 13, /* General protection */
	EXC_PF = 14, /* Page fault */
	EXC_MF = 16, /* x87 floating point */
	EXC_AC = 17, /* Alignment check */
	EXC_MC = 18, /* Machine check */
	EXC_XF = 19, /* SIMD floating point */
	EXC_SX = 30, /* Security */
	EXC_COUNT
};

#endif
