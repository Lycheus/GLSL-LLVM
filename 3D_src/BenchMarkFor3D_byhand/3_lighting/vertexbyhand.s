	.file	"vertexbyhand.bc"
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
	ADDI r29, r0, NormalMatrix.u.2.3@ha
	ADDI r28, r0, NormalMatrix.u.2.3@l
	BL OpenGLES.Start
	ADDI r3, r0, NormalMatrix.u.1.3@ha
	ADDI r4, r0, NormalMatrix.u.1.3@l
	ADDI r5, r0, NormalMatrix.u.0.3@ha
	ADDI r6, r0, NormalMatrix.u.0.3@l
	ADD 2.1000, 29, 28
	LD 2.0001, 3(2)
	LD 2.0010, 2(2)
	LD 2.0100, 1(2)
	LD 2.1000, 0(2)
	ADD 3.1000, 3, 4
	LD 3.0001, 3(3)
	LD 3.0010, 2(3)
	LD 3.0100, 1(3)
	LD 3.1000, 0(3)
	ADD 4.1000, 5, 6
	LD 4.0001, 3(4)
	LD 4.0010, 2(4)
	LD 4.0100, 1(4)
	LD 4.1000, 0(4)
	BL OpenGLES.ChangeMatrix3.3
	BL OpenGLES.ChangeMatrix3.3
	BL OpenGLES.ChangeMatrix3.3
	BL OpenGLES.LoadAttribute3.rm_Normal.0
	BL OpenGLES.CallRegister
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

	.type	specularExp,@object     # @specularExp
	.align	2
specularExp:
	.long	0                       # float 0.000000e+00
	.size	specularExp, 4

	.type	Color_Light,@object     # @Color_Light
	.align	4
Color_Light:
	.space	16
	.size	Color_Light, 16

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

	.weak	NormalMatrix.u.0.3
	.weak	NormalMatrix.u.1.3
	.weak	NormalMatrix.u.2.3
	.weak	light_Pos.u.0.1
	.weak	eye_Pos.u.0.1
	.weak	diffuse_material.u.0.1
	.weak	diffuse_light.u.0.1
	.weak	ambient_material.u.0.1
	.weak	ambient_light.u.0.1
	.weak	specularExp.u.0.1
	.weak	specular_material.u.0.1
	.weak	specular_light.u.0.1
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
	.weak	OpenGLES.ChangeMatrix3.3
	.weak	OpenGLES.LoadAttribute3.rm_Normal.0
	.weak	OpenGLES.CallRegister

