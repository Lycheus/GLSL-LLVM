@rm_Vertex = attribute <4 x float> zeroinitializer
@rm_Normal = attribute <3 x float> zeroinitializer
@ProjectionMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix1 = uniform <4 x 4 float> zeroinitializer
@ambient_light = uniform <4 x float> zeroinitializer
@ambient_material = uniform <4 x float> zeroinitializer
@diffuse_light = uniform <4 x float> zeroinitializer
@diffuse_material = uniform <4 x float> zeroinitializer
@specular_light = uniform <4 x float> zeroinitializer
@specular_material = uniform <4 x float> zeroinitializer
@eye_Pos = uniform <3 x float> zeroinitializer
@light_Pos = uniform <3 x float> zeroinitializer
@specularExp = uniform float zeroinitializer
@Color_Light = varying <4 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = load <3 x 3 float>* @gl_NormalMatrix.u.0.3, align 16
	%1 = load <3 x 3 float>* @gl_NormalMatrix.u.1.3, align 16
	%2 = load <3 x 3 float>* @gl_NormalMatrix.u.2.3, align 16
	%3 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %2, <3 x 3 float> %1, <3 x 3 float> %0)
	%4 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %2, <3 x 3 float> %1, <3 x 3 float> %0)
	%5 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %2, <3 x 3 float> %1, <3 x 3 float> %0)
	%6 = call <4 x float> @OpenGLES.LoadAttribute3.rm_Normal.0()
	%7 = call <3 x float> @OpenGLES.CallRegister()
	%8 = call <3 x float> @OpenGLES.VectorTimesMatrix4.5(<3 x float> %6, <3 x 3 float> %5, <3 x 3 float> %4, <3 x 3 float> %3, <3 x float> %7)
	%normal = call <3 x float> @OpenGLES.Normalize(<3 x float> %8)
	%lightDir = call <3 x float> @OpenGLES.Normalize(<3 x float> %light_Pos)
	%9 = add <3 x float> %light_Pos, @eye_Pos
	%halfVector = call <3 x float> @OpenGLES.Normalize(<3 x float> %9)
	%10 = mul <4 x float> %diffuse_material, @diffuse_light
	%12 = shufflevector <4 x float> %11, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%13 = mul <4 x float> %ambient_material, @ambient_light
	%15 = shufflevector <4 x float> %14, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%17 = shufflevector <4 x float> %16, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%n = call <3 x float> @OpenGLES.Normalize(<3 x float> %normal)
	%18 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %lightDir)
	%NdotL = call float @OpenGLES.Max(float %18, float 0x0000000000000000)
	%19 = icmp sgt, float %NdotL, 0x0000000000000000
	br i1 %19, label %L1.T, label %L1.F
L1.T:
	%halfV = call <3 x float> @OpenGLES.Normalize(<3 x float> %halfVector)
	%20 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %halfV)
	%NdotHV = call float @OpenGLES.Max(float %20, float 0x0000000000000000)
	%21 = call float @OpenGLES.Pow(float %NdotHV, float @specularExp)
	%22 = load <4 x float>* @specular_material.u, align 16
	%23 = call <4 x float> @OpenGLES.VectorTimesScalar(float %21, <4 x float> %22)
	%24 = mul <4 x float> %23, @specular_light
	%25 = add <4 x float> %color, %24
	%27 = shufflevector <4 x float> %26, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%28 = call <4 x float> @OpenGLES.VectorTimesScalar(float %NdotL, <4 x float> %diffuse)
	%29 = add <4 x float> %color, %28
	%31 = shufflevector <4 x float> %30, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
L1.F:
L1.E:
	%32 = fdiv <4 x float> %color, 0x4008000000000000
	%34 = shufflevector <4 x float> %33, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%35 = call <4 x float> @OpenGLES.VaryingPut4.Color_Light.1(<4 x float> %34)
	%36 = load <4 x 4 float>* @ModelViewMatrix1.u.0.4, align 16
	%37 = load <4 x 4 float>* @ModelViewMatrix1.u.1.4, align 16
	%38 = load <4 x 4 float>* @ModelViewMatrix1.u.2.4, align 16
	%39 = load <4 x 4 float>* @ModelViewMatrix1.u.3.4, align 16
	%40 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	%41 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	%42 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	%43 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	%44 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
	%45 = call <4 x float> @OpenGLES.CallRegister()
	%46 = load <4 x 4 float>* @ModelViewMatrix.u.0.4, align 16
	%47 = load <4 x 4 float>* @ModelViewMatrix.u.1.4, align 16
	%48 = load <4 x 4 float>* @ModelViewMatrix.u.2.4, align 16
	%49 = load <4 x 4 float>* @ModelViewMatrix.u.3.4, align 16
	%50 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	%51 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	%52 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	%53 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	%54 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %44, <4 x 4 float> %43, <4 x 4 float> %42, <4 x 4 float> %41, <4 x 4 float> %40, <4 x float> %45)
	%55 = call <4 x float> @OpenGLES.CallRegister()
	%56 = load <4 x 4 float>* @ProjectionMatrix.u.0.4, align 16
	%57 = load <4 x 4 float>* @ProjectionMatrix.u.1.4, align 16
	%58 = load <4 x 4 float>* @ProjectionMatrix.u.2.4, align 16
	%59 = load <4 x 4 float>* @ProjectionMatrix.u.3.4, align 16
	%60 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %59, <4 x 4 float> %58, <4 x 4 float> %57, <4 x 4 float> %56)
	%61 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %59, <4 x 4 float> %58, <4 x 4 float> %57, <4 x 4 float> %56)
	%62 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %59, <4 x 4 float> %58, <4 x 4 float> %57, <4 x 4 float> %56)
	%63 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %59, <4 x 4 float> %58, <4 x 4 float> %57, <4 x 4 float> %56)
	%64 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %54, <4 x 4 float> %53, <4 x 4 float> %52, <4 x 4 float> %51, <4 x 4 float> %50, <4 x float> %55)
	%65 = call <4 x float> @OpenGLES.CallRegister()
	%66 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %64, <4 x 4 float> %63, <4 x 4 float> %62, <4 x 4 float> %61, <4 x 4 float> %60, <4 x float> %65)
	%67 = call <4 x float> @OpenGLES.CallRegister()
	%gl_Position = call <4 x float> @OpenGLES.END.2(<4 x float> %66, <4 x float> %67)

br label %return
return:
	ret void
}


