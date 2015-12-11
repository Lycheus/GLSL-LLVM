	.file	"fragmentbyhand.bc"
	.text
	.globl	main
	.align	2
	.type	main,@function
main:                                   # @main
.Leh_func_begin0:
# BB#0:                                 # %entry
	ST r0, 4(1)
	ST r1, -16(1)
	ADDI r1, r1, -16
.Ltmp0:
	ST r30, 8(1)
	BL OpenGLES.Start
	LD r30, 8(1)
	ADDI r1, r1, 16
	LD r0, 4(1)
	RET 
.Ltmp1:
	.size	main, .Ltmp1-main
.Leh_func_end0:

	.type	TexCoord,@object        # @TexCoord
	.section	.bss,"aw",@nobits
	.align	3
TexCoord:
	.space	8
	.size	TexCoord, 8

	.type	Tex,@object             # @Tex
	.align	2
Tex:
	.long	0                       # float 0.000000e+00
	.size	Tex, 4

	.type	diffuse,@object         # @diffuse
	.align	4
diffuse:
	.space	16
	.size	diffuse, 16

	.type	ambient,@object         # @ambient
	.align	4
ambient:
	.space	16
	.size	ambient, 16

	.type	normal,@object          # @normal
	.align	4
normal:
	.space	16
	.size	normal, 16

	.type	lightDir,@object        # @lightDir
	.align	4
lightDir:
	.space	16
	.size	lightDir, 16

	.type	halfVector,@object      # @halfVector
	.align	4
halfVector:
	.space	16
	.size	halfVector, 16

	.type	specular_light,@object  # @specular_light
	.align	4
specular_light:
	.space	16
	.size	specular_light, 16

	.type	specular_material,@object # @specular_material
	.align	4
specular_material:
	.space	16
	.size	specular_material, 16

	.type	specularExp,@object     # @specularExp
	.align	2
specularExp:
	.long	0                       # float 0.000000e+00
	.size	specularExp, 4

	.type	shaderr,@object         # @shaderr
	.align	2
shaderr:
	.long	0                       # 0x0
	.size	shaderr, 4

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
	.uleb128	16              # Offset
	.byte	17                      # DW_CFA_offset_extended_sf
	.uleb128	65              # Reg
	.sleb128	-1              # Offset
	.byte	158                     # DW_CFA_offset + Reg (30)
	.uleb128	2               # Offset
	.align	2
.Leh_frame_end0:

	.weak	shaderr.u.0.1
	.weak	Tex0.u.0.1
	.weak	specularExp.u.0.1
	.weak	specular_material.u.0.1
	.weak	specular_light.u.0.1
	.weak	OpenGLES.Start

