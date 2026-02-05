/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * MB9BF506x memory setup.
 */
__main_stack_size__     = 0x0800;
__process_stack_size__  = 0x0800;

MEMORY
{
    /*RELEASE*/
    flash 	    : org = 0x00020000, len = 384k          /*Final config with bootloader NOT optimized**/

    ram		 	: org = 0x1FFF8000, len = 32k
    ramcode 	: org = 0x20000000, len = 32k
    xram        : org = 0x60000000, len = 128k
}

__ramcode_start__           = ORIGIN(ramcode);
__ramcode_size__            = LENGTH(ramcode);
__ramcode_end__             = __ramcode_start__ + __ramcode_size__;

__ram_start__           = ORIGIN(ram);
__ram_size__            = LENGTH(ram);
__ram_end__             = __ram_start__ + __ram_size__;

__xram_start__           = ORIGIN(xram);
__xram_size__            = LENGTH(xram);
__xram_end__             = __xram_start__ + __xram_size__;

SECTIONS
{
    . = ORIGIN(flash);
    _text = .;

    startup : ALIGN(16) SUBALIGN(16)
    {
        KEEP(*(vectors))
    } > flash

	application_info : ALIGN(4) SUBALIGN(4)
	{
		. = ORIGIN(flash) + 0x110;
		_application_info_start = .;

		. = ALIGN(4);
		KEEP(*(.application_info_section))
		_application_info_end = .;
	} > flash

	resetHandlerSection : ALIGN(4)
	{
		. = ORIGIN(flash) + 0x300; /*For StandAlone Application*/
		/*. = 0x00000000 + 0x300;*/ /*From Application to loader ??*/
		_resetHandlerSection_start = .;
		. = ALIGN(4);
		*(.resetHandlerSection)
	} > flash

    constructors : ALIGN(4) SUBALIGN(4)
    {
        PROVIDE(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(__init_array_end = .);
    } > flash

    destructors : ALIGN(4) SUBALIGN(4)
    {
        PROVIDE(__fini_array_start = .);
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        PROVIDE(__fini_array_end = .);
    } > flash

    .text : ALIGN(16) SUBALIGN(16)
    {
        *(.text.startup.*)
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
    } > flash

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > flash

    .ARM.exidx : {
        PROVIDE(__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE(__exidx_end = .);
     } > flash

    .eh_frame_hdr :
    {
        *(.eh_frame_hdr)
    } > flash

    .eh_frame : ONLY_IF_RO
    {
        *(.eh_frame)
    } > flash

    .textalign : ONLY_IF_RO
    {
        . = ALIGN(8);
    } > flash

    _textdata = .;

	datasize = _edata - _data;

	_textramcode  = . + datasize;

	ramcodesize = _ramcode_end - _ramcode_start;

	_etext = _textramcode + ramcodesize; /* END TEXT*/

	. = __ram_start__;
    .stacks :
    {
        . = ALIGN(8);
        __main_stack_base__ = .;
        . += __main_stack_size__;
        . = ALIGN(8);
        __main_stack_end__ = .;
        __process_stack_base__ = .;
        __main_thread_stack_base__ = .;
        . += __process_stack_size__;
        . = ALIGN(8);
        __process_stack_end__ = .;
        __main_thread_stack_end__ = .;
    } > ram

    .data : AT ( _textdata )
    {
        . = ALIGN(4);
        PROVIDE(_data = .);
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        PROVIDE(_edata = .);
    } > ram

    .bss :
    {
        . = ALIGN(4);
        PROVIDE(_bss_start = .);
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
    } > ram

    PROVIDE(end = .);
	_end            = .;

	__heap_base__   = _end;
	__heap_end__    = __ram_end__;

	.ramcode : AT ( _textramcode )
	{
		PROVIDE(_ramcode_start = .);
		. = ALIGN(4);
		 *(.ramcode)
		. = ALIGN(4);
		PROVIDE(_ramcode_end = .);
	} > ramcode

	.xram (NOLOAD):
    {
        . = ALIGN(4);
        PROVIDE(_xram_start = .);
        *(.xram)
        . = ALIGN(4);
        *(.xram.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_xram_used = .);
    } > xram
    PROVIDE(_xram_end = __xram_end__);
}


