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

	%start = call <4 x float> @OpenGLES.Start()
	%color = alloca <4 x float>

	%NormalMatrix0 = load <4 x float>* @NormalMatrix.u.0.3, align 16
	%NormalMatrix1 = load <4 x float>* @NormalMatrix.u.1.3, align 16
	%NormalMatrix2 = load <4 x float>* @NormalMatrix.u.2.3, align 16

	%rm_Normal = call <4 x float> @OpenGLES.LoadAttribute3.rm_Normal.0();load <4 x float>* @rm_Normal.a.0.3, align 16
	%0 = call <4 x float> @OpenGLES.CallRegister()
	%1 = call <4 x float> @OpenGLES.MatrixTimesVector3.5(<4 x float> %NormalMatrix0, <4 x float> %NormalMatrix1, <4 x float> %NormalMatrix2, <4 x float> %0, <4 x float> %rm_Normal) ; fix 3x3 ==> 3 x 3
	%normal = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %1)
	%light_Pos = load <4 x float>* @light_Pos.u.0.3, align 16
	%lightDir = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %light_Pos)
	%eye_Pos = load <4 x float>* @eye_Pos.u.0.3, align 16
	%2 = fadd <4 x float> %light_Pos, %eye_Pos
	%halfVector = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %2)
	%diffuse_light = load <4 x float>* @diffuse_light.u.0.4, align 16
	%diffuse_material = load <4 x float>* @diffuse_material.u.0.4, align 16
	%3 = fmul <4 x float> %diffuse_light, %diffuse_material
	%diffuse = shufflevector <4 x float> %3, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%ambient_light = load <4 x float>* @ambient_light.u.0.4, align 16
	%ambient_material = load <4 x float>* @ambient_material.u.0.4, align 16
	%4 = fmul <4 x float> %ambient_light,	%ambient_material
	%ambient = shufflevector <4 x float> %4, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%color.1 = shufflevector <4 x float> %ambient, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%temp_test = call float @OpenGLES.JustTest()
	store <4 x float> %color.1, <4 x float>* %color
	;%temp_test2 = call float @OpenGLES.JustTest()
	%n = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %normal)
	%5 = call float	@OpenGLES.Dot3.2(<4 x float> %n, <4 x float> %lightDir)
	%NdotL = call float @OpenGLES.Max1.2(float %5, float 0x0000000000000000)
	%6 = fcmp ugt float %NdotL, 0x0000000000000000 ; fix icmp ==> fcmp and delete the first ","
	br i1 %6, label %L1.T, label %L1.F
L1.T:
	;%halfV
	;%NdotHV
	;%color
	%halfV = call <4 x float> @OpenGLES.Normalize3.1(<4 x float> %halfVector)
	%7 = call float	@OpenGLES.Dot3.2(<4 x float> %n, <4 x float> %halfV)
	%NdotHV = call float @OpenGLES.Max1.2(float %7, float 0x0000000000000000)
	%specular_light = load <4 x float>* @specular_light.u.0.4, align 16
	%specular_material = load <4 x float>* @specular_material.u.0.4, align 16
	%8 = fmul <4 x float> %specular_light, %specular_material
	%specularExp = load float* @specularExp.u.0.1, align 4
	%9 = call float	@OpenGLES.Pow1.2(float %NdotHV, float %specularExp)
	%10 = call <4 x float> @OpenGLES.VectorTimesScalar4.2(<4 x float> %8, float %9)

	%color.2 = load <4 x float>* %color
	%11 = fadd <4 x float> %color.2, %10
	%12 = shufflevector <4 x float> %11, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>	
	store <4 x float> %12, <4 x float>* %color

	%13 = call <4 x float> @OpenGLES.VectorTimesScalar4.2(<4 x float> %diffuse, float %NdotL)
	%color.3 = load <4 x float>* %color
	%14 = fadd <4 x float> %color.3, %13
	%15 = shufflevector <4 x float> %14, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	store <4 x float> %15, <4 x float>* %color

br label %L1.E ;ADDED    but I am not sure
L1.F:
br label %L1.E ;ADDED    but I am not sure
L1.E:
	;%Color_Light
	%temp_float = call <4 x float> @OpenGLES.LoadFloatVector1.0x4008000000000000.0x4008000000000000.0x4008000000000000.0x4008000000000000()
	%color.4 = load <4 x float>* %color
	%16 = fdiv <4 x float> %color.4, %temp_float             ;<4 x float> <float 0x4008000000000000, float 0x4008000000000000, float 0x4008000000000000, float 0x4008000000000000>
	%Color_Light = shufflevector <4 x float> %16, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>

        %17 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
        %18 = load <4 x float>* @ModelViewMatrix1.u.0.4, align 16
        %19 = load <4 x float>* @ModelViewMatrix1.u.1.4, align 16
        %20 = load <4 x float>* @ModelViewMatrix1.u.2.4, align 16
        %21 = load <4 x float>* @ModelViewMatrix1.u.3.4, align 16
        %22 = call <4 x float> @OpenGLES.ChangeMatrix4.4.1 (<4 x float> %21, <4 x float> %20, <4 x float> %19, <4 x float> %18)
        %23 = call <4 x float> @OpenGLES.ChangeMatrix4.4.2 (<4 x float> %21, <4 x float> %20, <4 x float> %19, <4 x float> %18)
        %24 = call <4 x float> @OpenGLES.ChangeMatrix4.4.3 (<4 x float> %21, <4 x float> %20, <4 x float> %19, <4 x float> %18)
        %25 = call <4 x float> @OpenGLES.ChangeMatrix4.4.4 (<4 x float> %21, <4 x float> %20, <4 x float> %19, <4 x float> %18)
        %26 = call <4 x float> @OpenGLES.CallRegister()
        %27 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %17, <4 x float> %25, <4 x float> %24, <4 x float> %23, <4 x float> %22, <4 x float> %26)
        %28 = load <4 x float>* @ModelViewMatrix.u.0.4, align 16
        %29 = load <4 x float>* @ModelViewMatrix.u.1.4, align 16
        %30 = load <4 x float>* @ModelViewMatrix.u.2.4, align 16
        %31 = load <4 x float>* @ModelViewMatrix.u.3.4, align 16
        %32 = call <4 x float> @OpenGLES.ChangeMatrix4.4.1(<4 x float> %31, <4 x float> %30, <4 x float> %29, <4 x float> %28)
        %33 = call <4 x float> @OpenGLES.ChangeMatrix4.4.2(<4 x float> %31, <4 x float> %30, <4 x float> %29, <4 x float> %28)
        %34 = call <4 x float> @OpenGLES.ChangeMatrix4.4.3(<4 x float> %31, <4 x float> %30, <4 x float> %29, <4 x float> %28)
        %35 = call <4 x float> @OpenGLES.ChangeMatrix4.4.4(<4 x float> %31, <4 x float> %30, <4 x float> %29, <4 x float> %28)
        %36 = call <4 x float> @OpenGLES.CallRegister()
        %37 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %27, <4 x float> %35, <4 x float> %34, <4 x float> %33, <4 x float> %32, <4 x float> %36)
        %38 = load <4 x float>* @ProjectionMatrix.u.0.4, align 16
        %39 = load <4 x float>* @ProjectionMatrix.u.1.4, align 16
        %40 = load <4 x float>* @ProjectionMatrix.u.2.4, align 16
        %41 = load <4 x float>* @ProjectionMatrix.u.3.4, align 16
        %42 = call <4 x float> @OpenGLES.ChangeMatrix4.4.1(<4 x float> %41, <4 x float> %40, <4 x float> %39, <4 x float> %38)
        %43 = call <4 x float> @OpenGLES.ChangeMatrix4.4.2(<4 x float> %41, <4 x float> %40, <4 x float> %39, <4 x float> %38)
        %44 = call <4 x float> @OpenGLES.ChangeMatrix4.4.3(<4 x float> %41, <4 x float> %40, <4 x float> %39, <4 x float> %38)
        %45 = call <4 x float> @OpenGLES.ChangeMatrix4.4.4(<4 x float> %41, <4 x float> %40, <4 x float> %39, <4 x float> %38)
        %46 = call <4 x float> @OpenGLES.CallRegister()
        %47 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %37, <4 x float> %45, <4 x float> %44, <4 x float> %43, <4 x float> %42, <4 x float> %46)
        %48 = call <4 x float> @OpenGLES.CallRegister()
        %gl_Position = call <4 x float> @OpenGLES.END.2(<4 x float> %47, <4 x float> %48)

	;%14 = call <4 x 4 float> @OpenGLES.MatrixTimesMatrix(<4 x 4 float> %ProjectionMatrix, <4 x 4 float> %ModelViewMatrix)
	;%15 = call <4 x 4 float> @OpenGLES.MatrixTimesMatrix(<4 x 4 float> %14, <4 x 4 float> %ModelViewMatrix1)
	;%gl_Position = call <4 x float> @OpenGLES.MatrixTimesVector3(<4 x 4 float> %15, <4 x float> %rm_Vertex)
br label %return
return:
	ret void
}

