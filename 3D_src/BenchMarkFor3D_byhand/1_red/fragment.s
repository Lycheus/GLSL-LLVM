	.file	"fragment.bc"
	.text
	.globl	main
	.align	2
	.type	main,@function
main:                                   # @main
.Leh_func_begin0:
# BB#0:                                 # %entry
	mflr 0
	ST r0, 4(1)
	ST r1, -32(1)
	ADDI r1, r1, -32
.Ltmp0:
	ST r28, 16(1)
	ST r29, 20(1)
	ST r30, 24(1)
	mfspr 30, 256
	mtspr 256, 3
	ADDI r29, r0, Color_RGB.u.0.1@ha
	ADDI r28, r0, Color_RGB.u.0.1@l
	BL OpenGLES.Start
	ADD 2.1000, 29, 28
	LD 2.0001, 3(2)
	LD 2.0010, 2(2)
	LD 2.0100, 1(2)
	LD 2.1000, 0(2)
	BL OpenGLES.ENDFrag.1
	mtspr 256, 30
	LD r30, 24(1)
	LD r29, 20(1)
	LD r28, 16(1)
	ADDI r1, r1, 32
	LD r0, 4(1)
	mtlr 0
	blr 
.Ltmp1:
	.size	main, .Ltmp1-main
.Leh_func_end0:

	.type	Color_RGB,@object       # @Color_RGB
	.section	.bss,"aw",@nobits
	.align	4
Color_RGB:
	.space	16
	.size	Color_RGB, 16

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
	.uleb128	32              # Offset
	.byte	17                      # DW_CFA_offset_extended_sf
	.uleb128	65              # Reg
	.sleb128	-1              # Offset
	.byte	156                     # DW_CFA_offset + Reg (28)
	.uleb128	4               # Offset
	.byte	157                     # DW_CFA_offset + Reg (29)
	.uleb128	3               # Offset
	.byte	158                     # DW_CFA_offset + Reg (30)
	.uleb128	2               # Offset
	.align	2
.Leh_frame_end0:

	.weak	Color_RGB.u.0.1
	.weak	OpenGLES.Start
	.weak	OpenGLES.ENDFrag.1

