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
	.text
	.globl	main
	.align	2
	.type	main,@function
main:                                   # @main
.Leh_func_begin0:
# BB#0:                                 # %entry
	ST r0, 4(1)
	NOP 9
	ST r1, -80(1)
	NOP 9
	ADDI r1, r1, -80
	NOP 9
.Ltmp0:
	ST r14, 8(1)
	NOP 9
	ST r15, 12(1)
	NOP 9
	ST r16, 16(1)
	NOP 9
	ST r17, 20(1)
	NOP 9
	ST r18, 24(1)
	NOP 9
	ST r19, 28(1)
	NOP 9
	ST r20, 32(1)
	NOP 9
	ST r21, 36(1)
	NOP 9
	ST r22, 40(1)
	NOP 9
	ST r23, 44(1)
	NOP 9
	ST r24, 48(1)
	NOP 9
	ST r25, 52(1)
	NOP 9
	ST r26, 56(1)
	NOP 9
	ST r27, 60(1)
	NOP 9
	ST r28, 64(1)
	NOP 9
	ST r29, 68(1)
	NOP 9
	ST r30, 72(1)
	NOP 9
	mfspr 30, 256
	mtspr 256, 3
	ADDI r29, r0, ModelViewMatrix1.u.3.4@ha # size:4
	NOP 9
	ADDI r28, r0, ModelViewMatrix1.u.3.4@l # size:4
	NOP 9
	BL OpenGLES.Start               # size:4
	NOP 9
	ADDI r3, r0, ModelViewMatrix1.u.2.4@ha # size:4
	NOP 9
	ADDI r4, r0, ModelViewMatrix1.u.2.4@l # size:4
	NOP 9
	ADDI r5, r0, ModelViewMatrix1.u.1.4@ha # size:4
	NOP 9
	ADDI r6, r0, ModelViewMatrix1.u.1.4@l # size:4
	NOP 9
	ADDI r7, r0, ModelViewMatrix1.u.0.4@ha # size:4
	NOP 9
	ADDI r8, r0, ModelViewMatrix1.u.0.4@l # size:4
	NOP 9
	ADD 29.1000, 29, 28
	NOP 9
	LD 29.0001, 3(29)
	NOP 9
	LD 29.0010, 2(29)
	NOP 9
	LD 29.0100, 1(29)
	NOP 9
	LD 29.1000, 0(29)               # size:4
	NOP 9
	ADD 28.1000, 3, 4
	NOP 9
	LD 28.0001, 3(28)
	NOP 9
	LD 28.0010, 2(28)
	NOP 9
	LD 28.0100, 1(28)
	NOP 9
	LD 28.1000, 0(28)               # size:4
	NOP 9
	ADD 27.1000, 5, 6
	NOP 9
	LD 27.0001, 3(27)
	NOP 9
	LD 27.0010, 2(27)
	NOP 9
	LD 27.0100, 1(27)
	NOP 9
	LD 27.1000, 0(27)               # size:4
	NOP 9
	ADD 26.1000, 7, 8
	NOP 9
	LD 26.0001, 3(26)
	NOP 9
	LD 26.0010, 2(26)
	NOP 9
	LD 26.0100, 1(26)
	NOP 9
	LD 26.1000, 0(26)               # size:4
	NOP 9
	ADDI r25, r0, ModelViewMatrix.u.3.4@ha # size:4
	NOP 9
	ADDI r24, r0, ModelViewMatrix.u.3.4@l # size:4
	NOP 9
	MVR r2.1111, r29.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r27.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.1   # size:4
	NOP 9
	MVR r23.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r22, r0, ModelViewMatrix.u.2.4@ha # size:4
	NOP 9
	MVR r2.1111, r29.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r27.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.2   # size:4
	NOP 9
	MVR r21.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r20, r0, ModelViewMatrix.u.2.4@l # size:4
	NOP 9
	MVR r2.1111, r29.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r27.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.3   # size:4
	NOP 9
	MVR r19.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r18, r0, ModelViewMatrix.u.1.4@ha # size:4
	NOP 9
	MVR r2.1111, r29.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r27.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.4   # size:4
	NOP 9
	MVR r29.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r28, r0, ModelViewMatrix.u.1.4@l # size:4
	NOP 9
	BL OpenGLES.LoadAttribute4.rm_Vertex.0 # size:4
	NOP 9
	MVR r27.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r26, r0, ModelViewMatrix.u.0.4@ha # size:4
	NOP 9
	BL OpenGLES.CallRegister        # size:4
	NOP 9
	MVR r17.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r3, r0, ModelViewMatrix.u.0.4@l # size:4
	NOP 9
	ADD 25.1000, 25, 24
	NOP 9
	LD 25.0001, 3(25)
	NOP 9
	LD 25.0010, 2(25)
	NOP 9
	LD 25.0100, 1(25)
	NOP 9
	LD 25.1000, 0(25)               # size:4
	NOP 9
	ADD 24.1000, 22, 20
	NOP 9
	LD 24.0001, 3(24)
	NOP 9
	LD 24.0010, 2(24)
	NOP 9
	LD 24.0100, 1(24)
	NOP 9
	LD 24.1000, 0(24)               # size:4
	NOP 9
	ADD 28.1000, 18, 28
	NOP 9
	LD 28.0001, 3(28)
	NOP 9
	LD 28.0010, 2(28)
	NOP 9
	LD 28.0100, 1(28)
	NOP 9
	LD 28.1000, 0(28)               # size:4
	NOP 9
	ADD 26.1000, 26, 3
	NOP 9
	LD 26.0001, 3(26)
	NOP 9
	LD 26.0010, 2(26)
	NOP 9
	LD 26.0100, 1(26)
	NOP 9
	LD 26.1000, 0(26)               # size:4
	NOP 9
	MVR r2.1111, r25.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.1   # size:4
	NOP 9
	MVR r22.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r20, r0, ProjectionMatrix.u.2.4@ha # size:4
	NOP 9
	MVR r2.1111, r25.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.2   # size:4
	NOP 9
	MVR r18.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r16, r0, ProjectionMatrix.u.2.4@l # size:4
	NOP 9
	MVR r2.1111, r25.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.3   # size:4
	NOP 9
	MVR r15.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r14, r0, ProjectionMatrix.u.1.4@ha # size:4
	NOP 9
	MVR r2.1111, r25.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r26.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.4   # size:4
	NOP 9
	MVR r28.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r26, r0, ProjectionMatrix.u.1.4@l # size:4
	NOP 9
	MVR r2.1111, r27.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r29.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r19.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r21.xyzw, <4>      # size:4
	NOP 9
	MVR r6.1111, r23.xyzw, <4>      # size:4
	NOP 9
	MVR r7.1111, r17.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.VectorTimesMatrix4.6 # size:4
	NOP 9
	MVR r29.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r27, r0, ProjectionMatrix.u.0.4@ha # size:4
	NOP 9
	BL OpenGLES.CallRegister        # size:4
	NOP 9
	MVR r25.1111, r2.xyzw, <4>      # size:4
	NOP 9
	ADDI r3, r0, ProjectionMatrix.u.0.4@l # size:4
	NOP 9
	ADDI r4, r0, ProjectionMatrix.u.3.4@ha # size:4
	NOP 9
	ADDI r5, r0, ProjectionMatrix.u.3.4@l # size:4
	NOP 9
	ADD 24.1000, 4, 5
	NOP 9
	LD 24.0001, 3(24)
	NOP 9
	LD 24.0010, 2(24)
	NOP 9
	LD 24.0100, 1(24)
	NOP 9
	LD 24.1000, 0(24)               # size:4
	NOP 9
	ADD 23.1000, 20, 16
	NOP 9
	LD 23.0001, 3(23)
	NOP 9
	LD 23.0010, 2(23)
	NOP 9
	LD 23.0100, 1(23)
	NOP 9
	LD 23.1000, 0(23)               # size:4
	NOP 9
	ADD 26.1000, 14, 26
	NOP 9
	LD 26.0001, 3(26)
	NOP 9
	LD 26.0010, 2(26)
	NOP 9
	LD 26.0100, 1(26)
	NOP 9
	LD 26.1000, 0(26)               # size:4
	NOP 9
	ADD 27.1000, 27, 3
	NOP 9
	LD 27.0001, 3(27)
	NOP 9
	LD 27.0010, 2(27)
	NOP 9
	LD 27.0100, 1(27)
	NOP 9
	LD 27.1000, 0(27)               # size:4
	NOP 9
	MVR r2.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r23.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r26.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r27.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.1   # size:4
	NOP 9
	MVR r21.1111, r2.xyzw, <4>      # size:4
	NOP 9
	MVR r2.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r23.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r26.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r27.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.2   # size:4
	NOP 9
	MVR r20.1111, r2.xyzw, <4>      # size:4
	NOP 9
	MVR r2.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r23.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r26.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r27.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.3   # size:4
	NOP 9
	MVR r19.1111, r2.xyzw, <4>      # size:4
	NOP 9
	MVR r2.1111, r24.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r23.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r26.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r27.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.ChangeMatrix4.4.4   # size:4
	NOP 9
	MVR r27.1111, r2.xyzw, <4>      # size:4
	NOP 9
	MVR r2.1111, r29.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r28.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r15.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r18.xyzw, <4>      # size:4
	NOP 9
	MVR r6.1111, r22.xyzw, <4>      # size:4
	NOP 9
	MVR r7.1111, r25.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.VectorTimesMatrix4.6 # size:4
	NOP 9
	MVR r29.1111, r2.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.CallRegister        # size:4
	NOP 9
	MVR r7.1111, r2.xyzw, <4>       # size:4
	NOP 9
	MVR r2.1111, r29.xyzw, <4>      # size:4
	NOP 9
	MVR r3.1111, r27.xyzw, <4>      # size:4
	NOP 9
	MVR r4.1111, r19.xyzw, <4>      # size:4
	NOP 9
	MVR r5.1111, r20.xyzw, <4>      # size:4
	NOP 9
	MVR r6.1111, r21.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.VectorTimesMatrix4.6 # size:4
	NOP 9
	MVR r29.1111, r2.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.CallRegister        # size:4
	NOP 9
	MVR r3.1111, r2.xyzw, <4>       # size:4
	NOP 9
	MVR r2.1111, r29.xyzw, <4>      # size:4
	NOP 9
	BL OpenGLES.END.2               # size:4
	NOP 9
	mtspr 256, 30
	LD r30, 72(1)
	NOP 9
	LD r29, 68(1)
	NOP 9
	LD r28, 64(1)
	NOP 9
	LD r27, 60(1)
	NOP 9
	LD r26, 56(1)
	NOP 9
	LD r25, 52(1)
	NOP 9
	LD r24, 48(1)
	NOP 9
	LD r23, 44(1)
	NOP 9
	LD r22, 40(1)
	NOP 9
	LD r21, 36(1)
	NOP 9
	LD r20, 32(1)
	NOP 9
	LD r19, 28(1)
	NOP 9
	LD r18, 24(1)
	NOP 9
	LD r17, 20(1)
	NOP 9
	LD r16, 16(1)
	NOP 9
	LD r15, 12(1)
	NOP 9
	LD r14, 8(1)
	NOP 9
	ADDI r1, r1, 80
	NOP 9
	LD r0, 4(1)
	NOP 9
	mtlr 0
	RET 
	NOP 9
.Ltmp1:
	.size	main, .Ltmp1-main
.Leh_func_end0:

	.type	rm_Vertex,@object       # @rm_Vertex
	.section	.bss,"aw",@nobits
	.align	4
rm_Vertex:
	.space	16
	.size	rm_Vertex, 16

	.type	ProjectionMatrix,@object # @ProjectionMatrix
	.align	4
ProjectionMatrix:
	.space	16
	.size	ProjectionMatrix, 16

	.type	ModelViewMatrix,@object # @ModelViewMatrix
	.align	4
ModelViewMatrix:
	.space	16
	.size	ModelViewMatrix, 16

	.type	ModelViewMatrix1,@object # @ModelViewMatrix1
	.align	4
ModelViewMatrix1:
	.space	16
	.size	ModelViewMatrix1, 16

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
	.uleb128	80              # Offset
	.byte	17                      # DW_CFA_offset_extended_sf
	.uleb128	65              # Reg
	.sleb128	-1              # Offset
	.byte	142                     # DW_CFA_offset + Reg (14)
	.uleb128	18              # Offset
	.byte	143                     # DW_CFA_offset + Reg (15)
	.uleb128	17              # Offset
	.byte	144                     # DW_CFA_offset + Reg (16)
	.uleb128	16              # Offset
	.byte	145                     # DW_CFA_offset + Reg (17)
	.uleb128	15              # Offset
	.byte	146                     # DW_CFA_offset + Reg (18)
	.uleb128	14              # Offset
	.byte	147                     # DW_CFA_offset + Reg (19)
	.uleb128	13              # Offset
	.byte	148                     # DW_CFA_offset + Reg (20)
	.uleb128	12              # Offset
	.byte	149                     # DW_CFA_offset + Reg (21)
	.uleb128	11              # Offset
	.byte	150                     # DW_CFA_offset + Reg (22)
	.uleb128	10              # Offset
	.byte	151                     # DW_CFA_offset + Reg (23)
	.uleb128	9               # Offset
	.byte	152                     # DW_CFA_offset + Reg (24)
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
	.weak	ModelViewMatrix1.u.0.4
	.weak	ModelViewMatrix1.u.1.4
	.weak	ModelViewMatrix1.u.2.4
	.weak	ModelViewMatrix1.u.3.4
	.weak	ModelViewMatrix.u.0.4
	.weak	ModelViewMatrix.u.1.4
	.weak	ModelViewMatrix.u.2.4
	.weak	ModelViewMatrix.u.3.4
	.weak	ProjectionMatrix.u.0.4
	.weak	ProjectionMatrix.u.1.4
	.weak	ProjectionMatrix.u.2.4
	.weak	ProjectionMatrix.u.3.4
	.weak	OpenGLES.Start
	.weak	OpenGLES.ChangeMatrix4.4.1
	.weak	OpenGLES.ChangeMatrix4.4.2
	.weak	OpenGLES.ChangeMatrix4.4.3
	.weak	OpenGLES.ChangeMatrix4.4.4
	.weak	OpenGLES.LoadAttribute4.rm_Vertex.0
	.weak	OpenGLES.CallRegister
	.weak	OpenGLES.VectorTimesMatrix4.6
	.weak	OpenGLES.END.2

