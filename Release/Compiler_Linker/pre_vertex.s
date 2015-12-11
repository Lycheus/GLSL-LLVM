	.file	"vertex.bc"
	.section	.debug_frame,"",@progbits
.Lsection_debug_frame:
	.section	.debug_info,"",@progbits
.Lsection_info:
	.section	.debug_abbrev,"",@progbits
.Lsection_abbrev:
	.section	.debug_aranges,"",@progbits
	.section	.debug_macinfo,"",@progbits
	.section	.debug_line,"",@progbits
.Lsection_line:
	.section	.debug_loc,"",@progbits
	.section	.debug_pubnames,"",@progbits
	.section	.debug_pubtypes,"",@progbits
	.section	.debug_str,"",@progbits
.Lsection_str:
	.section	.debug_ranges,"",@progbits
.Ldebug_range:
	.section	.debug_loc,"",@progbits
.Lsection_debug_loc:
	.text
.Ltext_begin:
	.data
	.section	.rodata.cst4,"aM",@progbits,4
	.align	2
.LCPI0_0:                               # constant pool float
	.long	0                       # float 0.000000e+00
.LCPI0_1:                               # constant pool float
	.long	1065353216              # float 1.000000e+00
.LCPI0_2:                               # constant pool float
	.long	1092616192              # float 1.000000e+01
	.text
	.globl	main
	.align	2
	.type	main,@function
main:                                   # @main
.Leh_func_begin0:
# BB#0:                                 # %entry
	mflr 0
	NOP 9
	STV r0, 4(1), <1> set_mark_4
	ST r1, -304(1)
	ADDI r1, r1, -304
.Ltmp0:
	NOP 9
	STV r14, 272(1), <1> set_mark_4
	NOP 9
	STV r25, 276(1), <1> set_mark_4
	NOP 9
	STV r26, 280(1), <1> set_mark_4
	NOP 9
	STV r27, 284(1), <1> set_mark_4
	NOP 9
	STV r28, 288(1), <1> set_mark_4
	NOP 9
	STV r29, 292(1), <1> set_mark_4
	NOP 9
	STV r30, 296(1), <1> set_mark_4
	ADDI r0, r1, 16 set_mark
	ADD R3, R0, R0 set_mark_2
	NOP 9
	STV R20, 0(R3), <4> set_mark
	mfspr 4, 256
	NOP 9
	STV r4, 12(1), <1> set_mark_4
	mtspr 256, 4
	ADDI r30, r0, gtf_ModelViewProjectionMatrix.u.0.4@ha
	BL OpenGLES.Start               # size:4
	ADDI r29, r0, 3
	ADDI r28, r0, 2
	ADDI r27, r30, gtf_ModelViewProjectionMatrix.u.0.4@l
	BL OpenGLES.LoadAttribute4.gtf_Color.0 # size:4
	ADDI r0, r1, 13 set_mark
	ADD R3, R0, R0 set_mark_2
	NOP 9
	STV R16, 0(R3), <4> set_mark
	ADDI r4, r0, 1
	ADDI r5, r0, gtf_ModelViewProjectionMatrix.u.0.4@l
	ADD 29.1000, R27, R29
	LDV R29.1111, 0(29), <4>        # size:4
	ADD 28.1000, R27, R28
	LDV R28.1111, 0(28), <4>        # size:4
	ADD 27.1000, R27, R4
	LDV R27.1111, 0(27), <4>        # size:4
	ADD 30.1000, R30, R5
	LDV R30.1111, 0(30), <4>        # size:4
	MVR r15.1111, r29.xyzw, <4>     # size:4
	MVR r16.1111, r28.xyzw, <4>     # size:4
	MVR r17.1111, r27.xyzw, <4>     # size:4
	MVR r18.1111, r30.xyzw, <4>     # size:4
	BL OpenGLES.ChangeMatrix4.1     # size:4
	ADDI r0, r1, 11 set_mark
	ADD R3, R0, R0 set_mark_2
	NOP 9
	STV R16, 0(R3), <4> set_mark
	MVR r15.1111, r29.xyzw, <4>     # size:4
	MVR r16.1111, r28.xyzw, <4>     # size:4
	MVR r17.1111, r27.xyzw, <4>     # size:4
	MVR r18.1111, r30.xyzw, <4>     # size:4
	BL OpenGLES.ChangeMatrix4.2     # size:4
	MVR r26.1111, r16.xyzw, <4>     # size:4
	ADDI r4, r0, .LCPI0_0@ha
	LDV R3.1000, .LCPI0_0@l(4) set_mark_4
	MVR R4.1000, R3.xyzw, <4>
	NOP 9
	STV r4.1000, 10(1), <1> set_mark_4
	MVR r15.1111, r29.xyzw, <4>     # size:4
	MVR r16.1111, r28.xyzw, <4>     # size:4
	MVR r17.1111, r27.xyzw, <4>     # size:4
	MVR r18.1111, r30.xyzw, <4>     # size:4
	BL OpenGLES.ChangeMatrix4.3     # size:4
	MVR r25.1111, r16.xyzw, <4>     # size:4
	ADDI r5, r0, .LCPI0_1@ha
	LDV R3.1000, .LCPI0_1@l(5) set_mark_4
	MVR R14.1000, R3.xyzw, <4>
	MVR r15.1111, r29.xyzw, <4>     # size:4
	MVR r16.1111, r28.xyzw, <4>     # size:4
	MVR r17.1111, r27.xyzw, <4>     # size:4
	MVR r18.1111, r30.xyzw, <4>     # size:4
	BL OpenGLES.ChangeMatrix4.4     # size:4
	MVR r30.1111, r16.xyzw, <4>     # size:4
	ADDI r29, r1, 15
	BL OpenGLES.LoadAttribute4.gtf_Vertex.0 # size:4
	MVR r28.1111, r16.xyzw, <4>     # size:4
	ADDI r27, r0, .LCPI0_2@ha
	BL OpenGLES.CallRegister        # size:4
	MVR r20.1111, r16.xyzw, <4>     # size:4
	LDV R3.1000, .LCPI0_2@l(27) set_mark_4
	MVR R27.1000, R3.xyzw, <4>
	MVR r15.1111, r28.xyzw, <4>     # size:4
	MVR r16.1111, r30.xyzw, <4>     # size:4
	MVR r17.1111, r25.xyzw, <4>     # size:4
	MVR r18.1111, r26.xyzw, <4>     # size:4
	ADDI r0, r1, 11 set_mark        # size:4
	ADD R19.1000, R0, R0 set_mark_2
	LDV R19.1111, 0(19), <4>        # size:4
	BL OpenGLES.VectorTimesMatrix4.6 # size:4
	MVR r30.1111, r16.xyzw, <4>     # size:4
	ADDI r5, r0, gl_PointSize@ha
	LDV R3.1000, gl_PointSize@l(5) set_mark_4
	MVR R6.1000, R3.xyzw, <4>
	ADD R3, R0, R29
	NOP 9
	STV R30, 0(R3) set_mark_3       # size:4
	LDV R3.1000, 10(1) set_mark_4
	MVR R4.1000, R3.xyzw, <4>
	MULF r4, r6, r4
	ADDF r4, r4, r14
	NOP 9
	STV r4.1000, gl_PointSize@l(5), <1> set_mark_4
	LDV R3.0001, 15(1) set_mark_4
	MVR R29.1000, R3.wyzw, <4>      # size:4
	ADDI r28, r1, 14
	ADDI r0, r1, 13 set_mark        # size:4
	ADD R15.1000, R0, R0 set_mark_2
	LDV R15.1111, 0(15), <4>        # size:4
	BL OpenGLES.VaryingPut4.color.1 # size:4
	MULF r29, r29, r27
	BL OpenGLES.CallRegister        # size:4
	MVR r27.1111, r16.xyzw, <4>     # size:4
	NOP 9
	STV r29.1000, 14(1), <1> set_mark_4 # size:4
	ADD 29.1000, R0, R28
	LDV R29.1111, 0(29), <4>        # size:4
	BL OpenGLES.CallRegister        # size:4
	MVR r17.1111, r16.xyzw, <4>     # size:4
	vsldoi 4, 30, 30, 12            # size:4
	vsldoi 15, 4, 29, 4             # size:4
	MVR r16.1111, r27.xyzw, <4>     # size:4
	BL OpenGLES.END.3               # size:4
	LDV R4.1000, 12(1)
	mtspr 256, 4
	ADDI r0, r1, 16 set_mark
	ADD R20.1000, R0, R0 set_mark_2
	LDV R20.1111, 0(20), <4>
	LDV R30.1000, 296(1)
	LDV R29.1000, 292(1)
	LDV R28.1000, 288(1)
	LDV R27.1000, 284(1)
	LDV R26.1000, 280(1)
	LDV R25.1000, 276(1)
	LDV R14.1000, 272(1)
	ADDI r1, r1, 304
	LDV r0.1000, 4(1) set_mark_4
	mtlr 0
	blr 
.Ltmp1:
	.size	main, .Ltmp1-main
.Leh_func_end0:

	.type	Position_temp,@object   # @Position_temp
	.section	.bss,"aw",@nobits
	.globl	Position_temp
	.align	4
Position_temp:
	.space	16
	.size	Position_temp, 16

	.type	gtf_Color,@object       # @gtf_Color
	.align	4
gtf_Color:
	.space	16
	.size	gtf_Color, 16

	.type	gtf_Vertex,@object      # @gtf_Vertex
	.align	4
gtf_Vertex:
	.space	16
	.size	gtf_Vertex, 16

	.type	gtf_ModelViewProjectionMatrix,@object # @gtf_ModelViewProjectionMatrix
	.align	4
gtf_ModelViewProjectionMatrix:
	.space	64
	.size	gtf_ModelViewProjectionMatrix, 64

	.type	color,@object           # @color
	.align	4
color:
	.space	16
	.size	color, 16

	.type	temp_var0,@object       # @temp_var0
	.globl	temp_var0
	.align	4
temp_var0:
	.space	16
	.size	temp_var0, 16

	.section	.eh_frame,"aw",@progbits
.LEH_frame0:
.Lsection_eh_frame0:
.Leh_frame_common0:
.Lset0 = .Leh_frame_common_end0-.Leh_frame_common_begin0 # Length of Common Information Entry
	.long	.Lset0
.Leh_frame_common_begin0:
	.long	0                       # CIE Identifier Tag
	.byte	1                       # DW_CIE_VERSION
	.byte	0                       # CIE Augmentation
	.uleb128	1               # CIE Code Alignment Factor
	.sleb128	-4              # CIE Data Alignment Factor
	.byte	65                      # CIE Return Address Column
	.byte	12                      # DW_CFA_def_cfa
	.uleb128	1               # Register
	.uleb128	0               # Offset
	.align	2
.Leh_frame_common_end0:
.Lmain.eh:
.Lset1 = .Leh_frame_end0-.Leh_frame_begin0 # Length of Frame Information Entry
	.long	.Lset1
.Leh_frame_begin0:
.Lset2 = .Leh_frame_begin0-.Leh_frame_common0 # FDE CIE offset
	.long	.Lset2
	.long	.Leh_func_begin0        # FDE initial location
.Lset3 = .Leh_func_end0-.Leh_func_begin0 # FDE address range
	.long	.Lset3
	.uleb128	0               # Augmentation size
	.byte	4                       # DW_CFA_advance_loc4
.Lset4 = .Ltmp0-.Leh_func_begin0
	.long	.Lset4
	.byte	14                      # DW_CFA_def_cfa_offset
	.uleb128	304             # Offset
	.byte	17                      # DW_CFA_offset_extended_sf
	.uleb128	65              # Reg
	.sleb128	-1              # Offset
	.byte	142                     # DW_CFA_offset + Reg (14)
	.uleb128	8               # Offset
	.byte	153                     # DW_CFA_offset + Reg (25)
	.uleb128	7               # Offset
	.byte	154                     # DW_CFA_offset + Reg (26)
	.uleb128	6               # Offset
	.byte	155                     # DW_CFA_offset + Reg (27)
	.uleb128	5               # Offset
	.byte	156                     # DW_CFA_offset + Reg (28)
	.uleb128	4               # Offset
	.byte	157                     # DW_CFA_offset + Reg (29)
	.uleb128	3               # Offset
	.byte	158                     # DW_CFA_offset + Reg (30)
	.uleb128	2               # Offset
	.byte	5                       # DW_CFA_offset_extended
	.uleb128	97              # Reg
	.uleb128	72              # Offset
	.align	2
.Leh_frame_end0:

	.text
.Ltext_end:
	.data
.Ldata_end:
	.text
.Lsection_end1:
	.section	.debug_frame,"",@progbits
.Ldebug_frame_common:
.Lset5 = .Ldebug_frame_common_end-.Ldebug_frame_common_begin # Length of Common Information Entry
	.long	.Lset5
.Ldebug_frame_common_begin:
	.long	4294967295              # CIE Identifier Tag
	.byte	1                       # CIE Version
	.byte	0                       # CIE Augmentation
	.uleb128	1               # CIE Code Alignment Factor
	.sleb128	-4              # CIE Data Alignment Factor
	.byte	65                      # CIE RA Column
	.byte	12                      # DW_CFA_def_cfa
	.uleb128	1               # Register
	.uleb128	0               # Offset
	.align	2
.Ldebug_frame_common_end:
	.section	.debug_info,"",@progbits
.Linfo_begin1:
	.long	45                      # Length of Compilation Unit Info
	.short	2                       # DWARF version number
	.long	.Labbrev_begin          # Offset Into Abbrev. Section
	.byte	4                       # Address Size (in bytes)
	.uleb128	1               # Abbrev [1] 0xb:0x22 DW_TAG_compile_unit
	.byte	0                       # DW_AT_producer
	.byte	12                      # DW_AT_language
	.ascii	 "size"                 # DW_AT_name
	.byte	0
	.long	0                       # DW_AT_entry_pc
	.long	.Lsection_line          # DW_AT_stmt_list
	.uleb128	2               # Abbrev [2] 0x1b:0x11 DW_TAG_subprogram
	.ascii	 "vector"               # DW_AT_name
	.byte	0
	.ascii	 "vector"               # DW_AT_MIPS_linkage_name
	.byte	0
	.byte	1                       # DW_AT_prototyped
	.byte	1                       # DW_AT_external
	.byte	0                       # End Of Children Mark
	.byte	0                       # 4 extra padding bytes for GDB
	.byte	0
	.byte	0
	.byte	0
.Linfo_end1:
	.section	.debug_abbrev,"",@progbits
.Labbrev_begin:
	.uleb128	1               # Abbreviation Code
	.uleb128	17              # DW_TAG_compile_unit
	.uleb128	1               # DW_CHILDREN_yes
	.uleb128	37              # DW_AT_producer
	.uleb128	8               # DW_FORM_string
	.uleb128	19              # DW_AT_language
	.uleb128	11              # DW_FORM_data1
	.uleb128	3               # DW_AT_name
	.uleb128	8               # DW_FORM_string
	.uleb128	82              # DW_AT_entry_pc
	.uleb128	1               # DW_FORM_addr
	.uleb128	16              # DW_AT_stmt_list
	.uleb128	1               # DW_FORM_addr
	.uleb128	0               # EOM(1)
	.uleb128	0               # EOM(2)
	.uleb128	2               # Abbreviation Code
	.uleb128	46              # DW_TAG_subprogram
	.uleb128	0               # DW_CHILDREN_no
	.uleb128	3               # DW_AT_name
	.uleb128	8               # DW_FORM_string
	.uleb128	8199            # DW_AT_MIPS_linkage_name
	.uleb128	8               # DW_FORM_string
	.uleb128	39              # DW_AT_prototyped
	.uleb128	12              # DW_FORM_flag
	.uleb128	63              # DW_AT_external
	.uleb128	12              # DW_FORM_flag
	.uleb128	0               # EOM(1)
	.uleb128	0               # EOM(2)
	.uleb128	0               # EOM(3)
.Labbrev_end:
	.section	.debug_line,"",@progbits
.Lset6 = .Lline_end-.Lline_begin        # Length of Source Line Info
	.long	.Lset6
.Lline_begin:
	.short	2                       # DWARF version number
.Lset7 = .Lline_prolog_end-.Lline_prolog_begin # Prolog Length
	.long	.Lset7
.Lline_prolog_begin:
	.byte	1                       # Minimum Instruction Length
	.byte	1                       # Default is_stmt_start flag
	.byte	246                     # Line Base Value (Special Opcodes)
	.byte	245                     # Line Range Value (Special Opcodes)
	.byte	10                      # Special Opcode Base
	.byte	0                       # DW_LNS_copy arg count
	.byte	1                       # DW_LNS_advance_pc arg count
	.byte	1                       # DW_LNS_advance_line arg count
	.byte	1                       # DW_LNS_set_file arg count
	.byte	1                       # DW_LNS_set_column arg count
	.byte	0                       # DW_LNS_negate_stmt arg count
	.byte	0                       # DW_LNS_set_basic_block arg count
	.byte	0                       # DW_LNS_const_add_pc arg count
	.byte	1                       # DW_LNS_fixed_advance_pc arg count
	.byte	0                       # Directory
	.byte	0                       # End of directories
	.asciz	 "size"                 # Source
	.uleb128	1               # Directory #
	.uleb128	0               # Mod date
	.uleb128	0               # File size
	.byte	0                       # End of files
.Lline_prolog_end:
	.byte	0                       # Extended Op
	.byte	5                       # Op size
	.byte	2                       # DW_LNE_set_address
	.long	.Lsection_end1          # Section end label
	.byte	0                       # DW_LNE_end_sequence
	.byte	1
	.byte	1
.Lline_end:
	.section	.debug_pubnames,"",@progbits
.Lset8 = .Lpubnames_end1-.Lpubnames_begin1 # Length of Public Names Info
	.long	.Lset8
.Lpubnames_begin1:
	.short	2                       # DWARF Version
	.long	.Linfo_begin1           # Offset of Compilation Unit Info
.Lset9 = .Linfo_end1-.Linfo_begin1      # Compilation Unit Length
	.long	.Lset9
	.long	27                      # DIE offset
	.asciz	 "vector"               # External Name
	.long	0                       # End Mark
.Lpubnames_end1:
	.section	.debug_pubtypes,"",@progbits
.Lset10 = .Lpubtypes_end1-.Lpubtypes_begin1 # Length of Public Types Info
	.long	.Lset10
.Lpubtypes_begin1:
	.short	2                       # DWARF Version
	.long	.Linfo_begin1           # Offset of Compilation Unit Info
.Lset11 = .Linfo_end1-.Linfo_begin1     # Compilation Unit Length
	.long	.Lset11
	.long	0                       # End Mark
.Lpubtypes_end1:
	.section	.debug_aranges,"",@progbits
	.section	.debug_ranges,"",@progbits
	.section	.debug_macinfo,"",@progbits
	.weak	gtf_ModelViewProjectionMatrix.u.0.4
	.weak	gl_PointSize
	.weak	OpenGLES.Start
	.weak	OpenGLES.LoadAttribute4.gtf_Color.0
	.weak	OpenGLES.ChangeMatrix4.1
	.weak	OpenGLES.ChangeMatrix4.2
	.weak	OpenGLES.ChangeMatrix4.3
	.weak	OpenGLES.ChangeMatrix4.4
	.weak	OpenGLES.LoadAttribute4.gtf_Vertex.0
	.weak	OpenGLES.CallRegister
	.weak	OpenGLES.VectorTimesMatrix4.6
	.weak	OpenGLES.VaryingPut4.color.1
	.weak	OpenGLES.END.3

