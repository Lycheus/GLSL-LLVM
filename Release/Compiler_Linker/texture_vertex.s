	.file	"vertex.bc"
	.text
	.globl	main
	.align	2
	.type	main,@function
main:                                   # @main
.Leh_func_begin0:
# BB#0:                                 # %entry
	mflr 0
	ST r0, 4(1)
	ST r1, -80(1)
	ADDI r1, r1, -80
.Ltmp0:
	ST r14, 8(1)
	ST r15, 12(1)
	ST r16, 16(1)
	ST r17, 20(1)
	ST r18, 24(1)
	ST r19, 28(1)
	ST r20, 32(1)
	ST r21, 36(1)
	ST r22, 40(1)
	ST r23, 44(1)
	ST r24, 48(1)
	ST r25, 52(1)
	ST r26, 56(1)
	ST r27, 60(1)
	ST r28, 64(1)
	ST r29, 68(1)
	ST r30, 72(1)
	mfspr 30, 256
	mtspr 256, 3
	ADDI r29, r0, ModelViewMatrix1.u.3.4@ha
	BL OpenGLES.Start
	ADDI r28, r0, ModelViewMatrix1.u.3.4@l
	BL OpenGLES.LoadAttribute2.texcoord.0
	ADDI r27, r0, ModelViewMatrix1.u.2.4@ha
	BL OpenGLES.VaryingPut4.texcoord0.1
	ADDI r3, r0, ModelViewMatrix1.u.2.4@l
	ADDI r4, r0, ModelViewMatrix1.u.1.4@ha
	ADDI r5, r0, ModelViewMatrix1.u.1.4@l
	ADDI r6, r0, ModelViewMatrix1.u.0.4@ha
	ADDI r7, r0, ModelViewMatrix1.u.0.4@l
	ADD 29.1000, 29, 28
	LD 29.0001, 3(29)
	LD 29.0010, 2(29)
	LD 29.0100, 1(29)
	LD 29.1000, 0(29)
	ADD 28.1000, 27, 3
	LD 28.0001, 3(28)
	LD 28.0010, 2(28)
	LD 28.0100, 1(28)
	LD 28.1000, 0(28)
	ADD 27.1000, 4, 5
	LD 27.0001, 3(27)
	LD 27.0010, 2(27)
	LD 27.0100, 1(27)
	LD 27.1000, 0(27)
	ADD 26.1000, 6, 7
	LD 26.0001, 3(26)
	LD 26.0010, 2(26)
	LD 26.0100, 1(26)
	LD 26.1000, 0(26)
	ADDI r25, r0, ModelViewMatrix.u.3.4@ha
	ADDI r24, r0, ModelViewMatrix.u.3.4@l
	MVR r2.1111, r29.xyzw, <4>
	MVR r3.1111, r28.xyzw, <4>
	MVR r4.1111, r27.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.1
	MVR r23.1111, r2.xyzw, <4>
	ADDI r22, r0, ModelViewMatrix.u.2.4@ha
	MVR r2.1111, r29.xyzw, <4>
	MVR r3.1111, r28.xyzw, <4>
	MVR r4.1111, r27.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.2
	MVR r21.1111, r2.xyzw, <4>
	ADDI r20, r0, ModelViewMatrix.u.2.4@l
	MVR r2.1111, r29.xyzw, <4>
	MVR r3.1111, r28.xyzw, <4>
	MVR r4.1111, r27.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.3
	MVR r19.1111, r2.xyzw, <4>
	ADDI r18, r0, ModelViewMatrix.u.1.4@ha
	MVR r2.1111, r29.xyzw, <4>
	MVR r3.1111, r28.xyzw, <4>
	MVR r4.1111, r27.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.4
	MVR r29.1111, r2.xyzw, <4>
	ADDI r28, r0, ModelViewMatrix.u.1.4@l
	BL OpenGLES.LoadAttribute4.rm_Vertex.0
	MVR r27.1111, r2.xyzw, <4>
	ADDI r26, r0, ModelViewMatrix.u.0.4@ha
	BL OpenGLES.CallRegister
	MVR r17.1111, r2.xyzw, <4>
	ADDI r3, r0, ModelViewMatrix.u.0.4@l
	ADD 25.1000, 25, 24
	LD 25.0001, 3(25)
	LD 25.0010, 2(25)
	LD 25.0100, 1(25)
	LD 25.1000, 0(25)
	ADD 24.1000, 22, 20
	LD 24.0001, 3(24)
	LD 24.0010, 2(24)
	LD 24.0100, 1(24)
	LD 24.1000, 0(24)
	ADD 28.1000, 18, 28
	LD 28.0001, 3(28)
	LD 28.0010, 2(28)
	LD 28.0100, 1(28)
	LD 28.1000, 0(28)
	ADD 26.1000, 26, 3
	LD 26.0001, 3(26)
	LD 26.0010, 2(26)
	LD 26.0100, 1(26)
	LD 26.1000, 0(26)
	MVR r2.1111, r25.xyzw, <4>
	MVR r3.1111, r24.xyzw, <4>
	MVR r4.1111, r28.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.1
	MVR r22.1111, r2.xyzw, <4>
	ADDI r20, r0, ProjectionMatrix.u.2.4@ha
	MVR r2.1111, r25.xyzw, <4>
	MVR r3.1111, r24.xyzw, <4>
	MVR r4.1111, r28.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.2
	MVR r18.1111, r2.xyzw, <4>
	ADDI r16, r0, ProjectionMatrix.u.2.4@l
	MVR r2.1111, r25.xyzw, <4>
	MVR r3.1111, r24.xyzw, <4>
	MVR r4.1111, r28.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.3
	MVR r15.1111, r2.xyzw, <4>
	ADDI r14, r0, ProjectionMatrix.u.1.4@ha
	MVR r2.1111, r25.xyzw, <4>
	MVR r3.1111, r24.xyzw, <4>
	MVR r4.1111, r28.xyzw, <4>
	MVR r5.1111, r26.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.4
	MVR r28.1111, r2.xyzw, <4>
	ADDI r26, r0, ProjectionMatrix.u.1.4@l
	MVR r2.1111, r27.xyzw, <4>
	MVR r3.1111, r29.xyzw, <4>
	MVR r4.1111, r19.xyzw, <4>
	MVR r5.1111, r21.xyzw, <4>
	MVR r6.1111, r23.xyzw, <4>
	MVR r7.1111, r17.xyzw, <4>
	BL OpenGLES.VectorTimesMatrix4.6
	MVR r29.1111, r2.xyzw, <4>
	ADDI r27, r0, ProjectionMatrix.u.0.4@ha
	BL OpenGLES.CallRegister
	MVR r25.1111, r2.xyzw, <4>
	ADDI r3, r0, ProjectionMatrix.u.0.4@l
	ADDI r4, r0, ProjectionMatrix.u.3.4@ha
	ADDI r5, r0, ProjectionMatrix.u.3.4@l
	ADD 24.1000, 4, 5
	LD 24.0001, 3(24)
	LD 24.0010, 2(24)
	LD 24.0100, 1(24)
	LD 24.1000, 0(24)
	ADD 23.1000, 20, 16
	LD 23.0001, 3(23)
	LD 23.0010, 2(23)
	LD 23.0100, 1(23)
	LD 23.1000, 0(23)
	ADD 26.1000, 14, 26
	LD 26.0001, 3(26)
	LD 26.0010, 2(26)
	LD 26.0100, 1(26)
	LD 26.1000, 0(26)
	ADD 27.1000, 27, 3
	LD 27.0001, 3(27)
	LD 27.0010, 2(27)
	LD 27.0100, 1(27)
	LD 27.1000, 0(27)
	MVR r2.1111, r24.xyzw, <4>
	MVR r3.1111, r23.xyzw, <4>
	MVR r4.1111, r26.xyzw, <4>
	MVR r5.1111, r27.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.1
	MVR r21.1111, r2.xyzw, <4>
	MVR r2.1111, r24.xyzw, <4>
	MVR r3.1111, r23.xyzw, <4>
	MVR r4.1111, r26.xyzw, <4>
	MVR r5.1111, r27.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.2
	MVR r20.1111, r2.xyzw, <4>
	MVR r2.1111, r24.xyzw, <4>
	MVR r3.1111, r23.xyzw, <4>
	MVR r4.1111, r26.xyzw, <4>
	MVR r5.1111, r27.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.3
	MVR r19.1111, r2.xyzw, <4>
	MVR r2.1111, r24.xyzw, <4>
	MVR r3.1111, r23.xyzw, <4>
	MVR r4.1111, r26.xyzw, <4>
	MVR r5.1111, r27.xyzw, <4>
	BL OpenGLES.ChangeMatrix4.4.4
	MVR r27.1111, r2.xyzw, <4>
	MVR r2.1111, r29.xyzw, <4>
	MVR r3.1111, r28.xyzw, <4>
	MVR r4.1111, r15.xyzw, <4>
	MVR r5.1111, r18.xyzw, <4>
	MVR r6.1111, r22.xyzw, <4>
	MVR r7.1111, r25.xyzw, <4>
	BL OpenGLES.VectorTimesMatrix4.6
	MVR r29.1111, r2.xyzw, <4>
	BL OpenGLES.CallRegister
	MVR r7.1111, r2.xyzw, <4>
	MVR r2.1111, r29.xyzw, <4>
	MVR r3.1111, r27.xyzw, <4>
	MVR r4.1111, r19.xyzw, <4>
	MVR r5.1111, r20.xyzw, <4>
	MVR r6.1111, r21.xyzw, <4>
	BL OpenGLES.VectorTimesMatrix4.6
	MVR r29.1111, r2.xyzw, <4>
	BL OpenGLES.CallRegister
	MVR r3.1111, r2.xyzw, <4>
	MVR r2.1111, r29.xyzw, <4>
	BL OpenGLES.END.2
	mtspr 256, 30
	LD r30, 72(1)
	LD r29, 68(1)
	LD r28, 64(1)
	LD r27, 60(1)
	LD r26, 56(1)
	LD r25, 52(1)
	LD r24, 48(1)
	LD r23, 44(1)
	LD r22, 40(1)
	LD r21, 36(1)
	LD r20, 32(1)
	LD r19, 28(1)
	LD r18, 24(1)
	LD r17, 20(1)
	LD r16, 16(1)
	LD r15, 12(1)
	LD r14, 8(1)
	ADDI r1, r1, 80
	LD r0, 4(1)
	mtlr 0
	blr 
.Ltmp1:
	.size	main, .Ltmp1-main
.Leh_func_end0:

	.type	rm_Vertex,@object       # @rm_Vertex
	.section	.bss,"aw",@nobits
	.align	4
rm_Vertex:
	.space	16
	.size	rm_Vertex, 16

	.type	rm_Normal,@object       # @rm_Normal
	.align	4
rm_Normal:
	.space	16
	.size	rm_Normal, 16

	.type	texcoord,@object        # @texcoord
	.align	3
texcoord:
	.space	8
	.size	texcoord, 8

	.type	ModelViewMatrix1,@object # @ModelViewMatrix1
	.align	4
ModelViewMatrix1:
	.space	16
	.size	ModelViewMatrix1, 16

	.type	ModelViewMatrix,@object # @ModelViewMatrix
	.align	4
ModelViewMatrix:
	.space	16
	.size	ModelViewMatrix, 16

	.type	ProjectionMatrix,@object # @ProjectionMatrix
	.align	4
ProjectionMatrix:
	.space	16
	.size	ProjectionMatrix, 16

	.type	texcoord0,@object       # @texcoord0
	.align	3
texcoord0:
	.space	8
	.size	texcoord0, 8

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
	.weak	OpenGLES.LoadAttribute2.texcoord.0
	.weak	OpenGLES.VaryingPut4.texcoord0.1
	.weak	OpenGLES.ChangeMatrix4.4.1
	.weak	OpenGLES.ChangeMatrix4.4.2
	.weak	OpenGLES.ChangeMatrix4.4.3
	.weak	OpenGLES.ChangeMatrix4.4.4
	.weak	OpenGLES.LoadAttribute4.rm_Vertex.0
	.weak	OpenGLES.CallRegister
	.weak	OpenGLES.VectorTimesMatrix4.6
	.weak	OpenGLES.END.2

