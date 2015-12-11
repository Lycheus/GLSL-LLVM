	.file	"fragmentbyhand.bc"
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
	ADDI r29, r0, waterTexture@ha
	BL OpenGLES.Start
	vxor 28, 28, 28
	ADDI r29, r29, waterTexture@l
	MVR r2.1111, r28.xyzw, <4>
	MVR r3.1111, r28.xyzw, <4>
	BL OpenGLES.Dot
	mr 3, 29
	MVR r2.1111, r28.xyzw, <4>
	BL texture2D
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

	.type	facing,@object          # @facing
	.section	.bss,"aw",@nobits
	.align	2
facing:
	.long	0                       # float 0.000000e+00
	.size	facing, 4

	.type	lightDir,@object        # @lightDir
	.align	4
lightDir:
	.space	16
	.size	lightDir, 16

	.type	ecPosition,@object      # @ecPosition
	.align	4
ecPosition:
	.space	16
	.size	ecPosition, 16

	.type	surfaceColor,@object    # @surfaceColor
	.align	4
surfaceColor:
	.space	16
	.size	surfaceColor, 16

	.type	BumpDensity,@object     # @BumpDensity
	.align	2
BumpDensity:
	.long	0                       # float 0.000000e+00
	.size	BumpDensity, 4

	.type	BumpSize,@object        # @BumpSize
	.align	2
BumpSize:
	.long	0                       # float 0.000000e+00
	.size	BumpSize, 4

	.type	SpecularFactor,@object  # @SpecularFactor
	.align	2
SpecularFactor:
	.long	0                       # float 0.000000e+00
	.size	SpecularFactor, 4

	.type	waterTexture,@object    # @waterTexture
	.align	2
waterTexture:
	.long	0                       # float 0.000000e+00
	.size	waterTexture, 4

	.type	Intensity,@object       # @Intensity
	.align	2
Intensity:
	.long	0                       # float 0.000000e+00
	.size	Intensity, 4

	.type	viewVec,@object         # @viewVec
	.align	4
viewVec:
	.space	16
	.size	viewVec, 16

	.type	normal,@object          # @normal
	.align	4
normal:
	.space	16
	.size	normal, 16

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

	.weak	OpenGLES.Start
	.weak	OpenGLES.Dot
	.weak	texture2D

