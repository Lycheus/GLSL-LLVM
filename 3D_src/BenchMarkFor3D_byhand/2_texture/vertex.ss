	.file	"vertex.bc"
	.text
	.globl	main
	.align	2
	.type	main,@function
main:                                   # @main
.Leh_func_begin0:
# BB#0:                                 # %entry
	ST r0, 4(1)
	ST r1, -48(1)
	ADDI r1, r1, -48
.Ltmp0:
	ST r23, 12(1)
	ST r24, 16(1)
	ST r25, 20(1)
	ST r26, 24(1)
	ST r27, 28(1)
	ST r28, 32(1)
	ST r29, 36(1)
	ST r30, 40(1)
	ADDI r29, r0, Mode1ViewMatrix1.u.3.4@ha
	BL OpenGLES.Start
	ADDI r28, r0, Mode1ViewMatrix1.u.3.4@l
	BL OpenGLES.LoadAttribute4.rm_color.0
	ADDI r27, r0, Mode1ViewMatrix1.u.2.4@ha
	BL OpenGLES.VaryingPut4.v_color.1
	ADDI r3, r0, Mode1ViewMatrix1.u.2.4@l
	ADDI r4, r0, Mode1ViewMatrix1.u.1.4@ha
	ADDI r5, r0, Mode1ViewMatrix1.u.1.4@l
	ADDI r6, r0, Mode1ViewMatrix1.u.0.4@ha
	ADDI r7, r0, Mode1ViewMatrix1.u.0.4@l
	ADD 2.1000, 29, 28
	LD 2.0001, 3(2)
	LD 2.0010, 2(2)
	LD 2.0100, 1(2)
	LD 2.1000, 0(2)
	ADD 3.1000, 27, 3
	LD 3.0001, 3(3)
	LD 3.0010, 2(3)
	LD 3.0100, 1(3)
	LD 3.1000, 0(3)
	ADD 4.1000, 4, 5
	LD 4.0001, 3(4)
	LD 4.0010, 2(4)
	LD 4.0100, 1(4)
	LD 4.1000, 0(4)
	ADD 5.1000, 6, 7
	LD 5.0001, 3(5)
	LD 5.0010, 2(5)
	LD 5.0100, 1(5)
	LD 5.1000, 0(5)
	ADDI r29, r0, Mode1ViewMatrix.u.3.4@ha
	ADDI r28, r0, Mode1ViewMatrix.u.3.4@l
	BL OpenGLES.ChangeMatrix4.4.1
	ADDI r27, r0, Mode1ViewMatrix.u.2.4@ha
	BL OpenGLES.ChangeMatrix4.4.2
	ADDI r26, r0, Mode1ViewMatrix.u.2.4@l
	BL OpenGLES.ChangeMatrix4.4.3
	ADDI r25, r0, Mode1ViewMatrix.u.1.4@ha
	BL OpenGLES.ChangeMatrix4.4.4
	ADDI r24, r0, Mode1ViewMatrix.u.1.4@l
	BL OpenGLES.LoadAttribute4.rm_Vertex.0
	ADDI r23, r0, Mode1ViewMatrix.u.0.4@ha
	BL OpenGLES.CallRegister
	ADDI r3, r0, Mode1ViewMatrix.u.0.4@l
	ADD 2.1000, 29, 28
	LD 2.0001, 3(2)
	LD 2.0010, 2(2)
	LD 2.0100, 1(2)
	LD 2.1000, 0(2)
	ADD 3.1000, 27, 26
	LD 3.0001, 3(3)
	LD 3.0010, 2(3)
	LD 3.0100, 1(3)
	LD 3.1000, 0(3)
	ADD 4.1000, 25, 24
	LD 4.0001, 3(4)
	LD 4.0010, 2(4)
	LD 4.0100, 1(4)
	LD 4.1000, 0(4)
	ADD 5.1000, 23, 3
	LD 5.0001, 3(5)
	LD 5.0010, 2(5)
	LD 5.0100, 1(5)
	LD 5.1000, 0(5)
	ADDI r29, r0, ProjectionMatrix2.u.3.4@ha
	ADDI r28, r0, ProjectionMatrix2.u.3.4@l
	BL OpenGLES.ChangeMatrix4.4.1
	ADDI r27, r0, ProjectionMatrix2.u.2.4@ha
	BL OpenGLES.ChangeMatrix4.4.2
	ADDI r26, r0, ProjectionMatrix2.u.2.4@l
	BL OpenGLES.ChangeMatrix4.4.3
	ADDI r25, r0, ProjectionMatrix2.u.1.4@ha
	BL OpenGLES.ChangeMatrix4.4.4
	ADDI r24, r0, ProjectionMatrix2.u.1.4@l
	MVR r3.1111, r2.xyzw, <4>
	MVR r4.1111, r2.xyzw, <4>
	MVR r5.1111, r2.xyzw, <4>
	MVR r6.1111, r2.xyzw, <4>
	MVR r7.1111, r2.xyzw, <4>
	BL OpenGLES.VectorTimesMatrix4.6
	ADDI r23, r0, ProjectionMatrix2.u.0.4@ha
	BL OpenGLES.CallRegister
	ADDI r3, r0, ProjectionMatrix2.u.0.4@l
	ADD 2.1000, 29, 28
	LD 2.0001, 3(2)
	LD 2.0010, 2(2)
	LD 2.0100, 1(2)
	LD 2.1000, 0(2)
	ADD 3.1000, 27, 26
	LD 3.0001, 3(3)
	LD 3.0010, 2(3)
	LD 3.0100, 1(3)
	LD 3.1000, 0(3)
	ADD 4.1000, 25, 24
	LD 4.0001, 3(4)
	LD 4.0010, 2(4)
	LD 4.0100, 1(4)
	LD 4.1000, 0(4)
	ADD 5.1000, 23, 3
	LD 5.0001, 3(5)
	LD 5.0010, 2(5)
	LD 5.0100, 1(5)
	LD 5.1000, 0(5)
	BL OpenGLES.ChangeMatrix4.4.1
	BL OpenGLES.ChangeMatrix4.4.2
	BL OpenGLES.ChangeMatrix4.4.3
	BL OpenGLES.ChangeMatrix4.4.4
	MVR r3.1111, r2.xyzw, <4>
	MVR r4.1111, r2.xyzw, <4>
	MVR r5.1111, r2.xyzw, <4>
	MVR r6.1111, r2.xyzw, <4>
	MVR r7.1111, r2.xyzw, <4>
	BL OpenGLES.VectorTimesMatrix4.6
	BL OpenGLES.CallRegister
	MVR r7.1111, r2.xyzw, <4>
	MVR r3.1111, r2.xyzw, <4>
	MVR r4.1111, r2.xyzw, <4>
	MVR r5.1111, r2.xyzw, <4>
	MVR r6.1111, r2.xyzw, <4>
	BL OpenGLES.VectorTimesMatrix4.6
	BL OpenGLES.CallRegister
	MVR r3.1111, r2.xyzw, <4>
	BL OpenGLES.END.2
	LD r30, 40(1)
	LD r29, 36(1)
	LD r28, 32(1)
	LD r27, 28(1)
	LD r26, 24(1)
	LD r25, 20(1)
	LD r24, 16(1)
	LD r23, 12(1)
	ADDI r1, r1, 48
	LD r0, 4(1)
	RET 
.Ltmp1:
	.size	main, .Ltmp1-main
.Leh_func_end0:

	.type	rm_Vertex,@object       # @rm_Vertex
	.section	.bss,"aw",@nobits
	.align	4
rm_Vertex:
	.space	16
	.size	rm_Vertex, 16

	.type	rm_color,@object        # @rm_color
	.align	4
rm_color:
	.space	16
	.size	rm_color, 16

	.type	Mode1ViewMatrix1,@object # @Mode1ViewMatrix1
	.align	4
Mode1ViewMatrix1:
	.space	16
	.size	Mode1ViewMatrix1, 16

	.type	Mode1ViewMatrix,@object # @Mode1ViewMatrix
	.align	4
Mode1ViewMatrix:
	.space	16
	.size	Mode1ViewMatrix, 16

	.type	ProjectionMatrix2,@object # @ProjectionMatrix2
	.align	4
ProjectionMatrix2:
	.space	16
	.size	ProjectionMatrix2, 16

	.type	v_color,@object         # @v_color
	.align	4
v_color:
	.space	16
	.size	v_color, 16

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
	.uleb128	48              # Offset
	.byte	17                      # DW_CFA_offset_extended_sf
	.uleb128	65              # Reg
	.sleb128	-1              # Offset
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

	.weak	Mode1ViewMatrix1.u.0.4
	.weak	Mode1ViewMatrix1.u.1.4
	.weak	Mode1ViewMatrix1.u.2.4
	.weak	Mode1ViewMatrix1.u.3.4
	.weak	Mode1ViewMatrix.u.0.4
	.weak	Mode1ViewMatrix.u.1.4
	.weak	Mode1ViewMatrix.u.2.4
	.weak	Mode1ViewMatrix.u.3.4
	.weak	ProjectionMatrix2.u.0.4
	.weak	ProjectionMatrix2.u.1.4
	.weak	ProjectionMatrix2.u.2.4
	.weak	ProjectionMatrix2.u.3.4
	.weak	OpenGLES.Start
	.weak	OpenGLES.LoadAttribute4.rm_color.0
	.weak	OpenGLES.VaryingPut4.v_color.1
	.weak	OpenGLES.ChangeMatrix4.4.1
	.weak	OpenGLES.ChangeMatrix4.4.2
	.weak	OpenGLES.ChangeMatrix4.4.3
	.weak	OpenGLES.ChangeMatrix4.4.4
	.weak	OpenGLES.LoadAttribute4.rm_Vertex.0
	.weak	OpenGLES.CallRegister
	.weak	OpenGLES.VectorTimesMatrix4.6
	.weak	OpenGLES.END.2

