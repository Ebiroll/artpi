/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2015
 * Kamil Lulko, <kamil.lulko@gmail.com>
 */


.syntax unified
.cpu cortex-m7
.fpu softvfp
.thumb


/*

.globl	reset
.type reset, %function
reset:
	W(b)	_main

.globl	c_runtime_cpu_setup
c_runtime_cpu_setup:
	mov	pc, lr
*/

.global rt_hw_interrupt_disable
.type rt_hw_interrupt_disable, %function
rt_hw_interrupt_disable:
    MRS     r0, PRIMASK
    CPSID   I
    BX      LR

/*
 * void rt_hw_interrupt_enable(rt_base_t level);
 */
.global rt_hw_interrupt_enable
.type rt_hw_interrupt_enable, %function
rt_hw_interrupt_enable:
    MSR     PRIMASK, r0
    BX      LR

