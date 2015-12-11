@rm_Vertex = attribute <4 x float> zeroinitializer
@rm_Normal = attribute <4 x float> zeroinitializer
@ProjectionMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix1 = uniform <4 x 4 float> zeroinitializer
@NormalMatrix = uniform <4 x 4 float> zeroinitializer ; fix 3x3 ==> 3 x 3 ==> 4 x 4
@ambient_light = uniform <4 x float> zeroinitializer
@ambient_material = uniform <4 x float> zeroinitializer
@diffuse_light = uniform <4 x float> zeroinitializer
@diffuse_material = uniform <4 x float> zeroinitializer
@specular_light = uniform <4 x float> zeroinitializer
@specular_material = uniform <4 x float> zeroinitializer
@eye_Pos = uniform <4 x float> zeroinitializer
@light_Pos = uniform <4 x float> zeroinitializer
@specularExp = uniform float zeroinitializer
@Color_Light = varying <4 x float> zeroinitializer

define void @main() {
entry:
	;%normal
	;%lightDir
	;%halfVector
	;%diffuse
	;%ambient
	;%color
	;%NdotL

	;%start = call <4 x float> @OpenGLES.Start()
	;%color = alloca <4 x float>

	;%NormalMatrix0 = load <4 x float>* @NormalMatrix.u.0.3, align 16
	;%NormalMatrix1 = load <4 x float>* @NormalMatrix.u.1.3, align 16
	;%NormalMatrix2 = load <4 x float>* @NormalMatrix.u.2.3, align 16

	;%rm_Normal = call <4 x float> @OpenGLES.LoadAttribute3.rm_Normal.0();load <4 x float>* @rm_Normal.a.0.3, align 16
	;%0 = call <4 x float> @OpenGLES.CallRegister()
	;%1 = call <4 x float> @OpenGLES.MatrixTimesVector3.5(<4 x float> %NormalMatrix0, <4 x float> %NormalMatrix1, <4 x float> %NormalMatrix2, <4 x float> %0, <4 x float> %rm_Normal) ; fix 3x3 ==> 3 x 3


	;%normal = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %1)
	;%light_Pos = load <4 x float>* @light_Pos.u.0.3, align 16
	;%lightDir = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %light_Pos)
	;%eye_Pos = load <4 x float>* @eye_Pos.u.0.3, align 16
	;%2 = fadd <4 x float> %light_Pos, %eye_Pos
	

	;%halfVector = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %2)
	;%diffuse_light = load <4 x float>* @diffuse_light.u.0.4, align 16
	;%diffuse_material = load <4 x float>* @diffuse_material.u.0.4, align 16
	;%3 = fmul <4 x float> %diffuse_light, %diffuse_material
	;%diffuse = shufflevector <4 x float> %3, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%ambient_light = load <4 x float>* @ambient_light.u.0.4, align 16
	;%ambient_material = load <4 x float>* @ambient_material.u.0.4, align 16

	;%4 = fmul <4 x float> %ambient_light,	%ambient_material
	;%ambient = shufflevector <4 x float> %4, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%color.1 = shufflevector <4 x float> %ambient, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;;%temp_test = call float @OpenGLES.JustTest()
	;store <4 x float> %color.1, <4 x float>* %color
	;;%temp_test2 = call float @OpenGLES.JustTest()
	;%n = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %normal)
	;%5 = call float	@OpenGLES.Dot3.2(<4 x float> %n, <4 x float> %lightDir)


	%NdotL = call float @OpenGLES.Max1.2(float 0x0000000000000000, float 0x0000000000000000)
	;%branch = fcmp ugt float %NdotL, 0x0000000000000000 ; fix icmp ==> fcmp and delete the first ","
	%branch = fcmp ugt float %NdotL, %NdotL ; fix icmp ==> fcmp and delete the first ","
	br i1 %branch, label %L1.T, label %L1.F
L1.T:

br label %L1.E ;ADDED    but I am not sure
L1.F:
br label %L1.E ;ADDED    but I am not sure
L1.E:

br label %return
return:
	ret void
}

