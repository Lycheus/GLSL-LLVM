	.file	"vertexbyhand.bc"
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

	.type	cube_texs,@object       # @cube_texs
	.align	3
cube_texs:
	.space	8
	.size	cube_texs, 8

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

	.type	TexCoord,@object        # @TexCoord
	.align	3
TexCoord:
	.space	8
	.size	TexCoord, 8

	.type	shaderr,@object         # @shaderr
	.align	2
shaderr:
	.long	0                       # 0x0
	.size	shaderr, 4

	.type	timeflag,@object        # @timeflag
	.align	2
timeflag:
	.long	0                       # float 0.000000e+00
	.size	timeflag, 4

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

	.type	NormalMatrix,@object    # @NormalMatrix
	.align	4
NormalMatrix:
	.space	16
	.size	NormalMatrix, 16

	.type	ambient_light,@object   # @ambient_light
	.align	4
ambient_light:
	.space	16
	.size	ambient_light, 16

	.type	ambient_material,@object # @ambient_material
	.align	4
ambient_material:
	.space	16
	.size	ambient_material, 16

	.type	diffuse_light,@object   # @diffuse_light
	.align	4
diffuse_light:
	.space	16
	.size	diffuse_light, 16

	.type	diffuse_material,@object # @diffuse_material
	.align	4
diffuse_material:
	.space	16
	.size	diffuse_material, 16

	.type	eye_Pos,@object         # @eye_Pos
	.align	4
eye_Pos:
	.space	16
	.size	eye_Pos, 16

	.type	light_Pos,@object       # @light_Pos
	.align	4
light_Pos:
	.space	16
	.size	light_Pos, 16

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
	.weak	timeflag.u.0.1
	.weak	NormalMatrix.u.0.3
	.weak	NormalMatrix.u.1.3
	.weak	NormalMatrix.u.2.3
	.weak	light_Pos.u.0.1
	.weak	eye_Pos.u.0.1
	.weak	diffuse_material.u.0.1
	.weak	diffuse_light.u.0.1
	.weak	ambient_material.u.0.1
	.weak	ambient_light.u.0.1
	.weak	ModelViewMatrix.u.0.4
	.weak	ModelViewMatrix.u.1.4
	.weak	ModelViewMatrix.u.2.4
	.weak	ModelViewMatrix.u.3.4
	.weak	ProjectionMatrix.u.0.4
	.weak	ProjectionMatrix.u.1.4
	.weak	ProjectionMatrix.u.2.4
	.weak	ProjectionMatrix.u.3.4
	.weak	OpenGLES.Start

