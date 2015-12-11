#### BEGIN COMPILER 0 INFO LOG ####
@rm_Vertex = highp attribute <4 x float>
@rm_Normal = highp attribute <3 x float>
@ProjectionMatrix = highp uniform <4 x 4 float>
@ModelViewMatrix = highp uniform <4 x 4 float>
@ModelViewMatrix1 = highp uniform <4 x 4 float>
@ambient_light = highp uniform <4 x float>
@ambient_material = highp uniform <4 x float>
@diffuse_light = highp uniform <4 x float>
@diffuse_material = highp uniform <4 x float>
@specular_light = highp uniform <4 x float>
@specular_material = highp uniform <4 x float>
@eye_Pos = highp uniform <3 x float>
@light_Pos = highp uniform <3 x float>
@specularExp = highp uniform float
@Color_Light = highp varying <4 x float>

define highp void @main:void() {
	%0 = load highp <3 x 3 float>* @gl_NormalMatrix.u.0.3, align 16
	%1 = load highp <3 x 3 float>* @gl_NormalMatrix.u.1.3, align 16
	%2 = load highp <3 x 3 float>* @gl_NormalMatrix.u.2.3, align 16
	%3 = call highp <3 x 3 float> @OpenGLES.ChangeMatrix3.3(highp <3 x 3 float> %2, highp <3 x 3 float> %1, highp <3 x 3 float> %0)
	%4 = call highp <3 x 3 float> @OpenGLES.ChangeMatrix3.3(highp <3 x 3 float> %2, highp <3 x 3 float> %1, highp <3 x 3 float> %0)
	%5 = call highp <3 x 3 float> @OpenGLES.ChangeMatrix3.3(highp <3 x 3 float> %2, highp <3 x 3 float> %1, highp <3 x 3 float> %0)
	%6 = call <4 x float> @OpenGLES.LoadAttribute3.rm_Normal.0()
	%7 = call highp <3 x float> @OpenGLES.CallRegister()
	%8 = call highp <3 x float> @OpenGLES.VectorTimesMatrix4.5(highp <3 x float> %6, highp <3 x 3 float> %5, highp <3 x 3 float> %4, highp <3 x 3 float> %3, highp <3 x float> %7)
	%normal = call highp <3 x float> @OpenGLES.Normalize(highp <3 x float> %8)
	%lightDir = call highp <3 x float> @OpenGLES.Normalize(highp <3 x float> @light_Pos)
	%9 = add highp <3 x float> @light_Pos, @eye_Pos
	%halfVector = call highp <3 x float> @OpenGLES.Normalize(highp <3 x float> %9)
	%10 = mul highp <4 x float> @diffuse_material, @diffuse_light
	%12 = shufflevector <4 x float> %11, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%13 = mul highp <4 x float> @ambient_material, @ambient_light
	%15 = shufflevector <4 x float> %14, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%17 = shufflevector <4 x float> %16, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%n = call highp <3 x float> @OpenGLES.Normalize(highp <3 x float> %normal)
	%18 = call highp float @OpenGLES.Dot(highp <3 x float> %n, highp <3 x float> %lightDir)
	%NdotL = call highp float @OpenGLES.Max(highp float %18, highp float 0x0000000000000000)
	%19 = icmp sgt, highp float %NdotL, 0x0000000000000000
	br highp i1 %19, label %L1.T, label %L1.F
L1.T:
	%halfV = call highp <3 x float> @OpenGLES.Normalize(highp <3 x float> %halfVector)
	%20 = call highp float @OpenGLES.Dot(highp <3 x float> %n, highp <3 x float> %halfV)
	%NdotHV = call highp float @OpenGLES.Max(highp float %20, highp float 0x0000000000000000)
	%21 = call highp float @OpenGLES.Pow(highp float %NdotHV, highp float @specularExp)
	%22 = load highp <4 x float>* @specular_material.u, align 16
	%23 = call highp <4 x float> @OpenGLES.VectorTimesScalar(highp float %21, highp <4 x float> %22)
	%24 = mul highp <4 x float> %23, @specular_light
	%25 = add highp <4 x float> %color, %24
	%27 = shufflevector <4 x float> %26, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%28 = call highp <4 x float> @OpenGLES.VectorTimesScalar(highp float %NdotL, highp <4 x float> %diffuse)
	%29 = add highp <4 x float> %color, %28
	%31 = shufflevector <4 x float> %30, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
L1.F:
L1.E:
	%32 = fdiv highp <4 x float> %color, 0x4008000000000000
	%34 = shufflevector <4 x float> %33, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%35 = call <4 x float> @OpenGLES.VaryingPut4.Color_Light.1(<4 x float> %34)
	%36 = load highp <4 x 4 float>* @ModelViewMatrix1.u.0.4, align 16
	%37 = load highp <4 x 4 float>* @ModelViewMatrix1.u.1.4, align 16
	%38 = load highp <4 x 4 float>* @ModelViewMatrix1.u.2.4, align 16
	%39 = load highp <4 x 4 float>* @ModelViewMatrix1.u.3.4, align 16
	%40 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(highp <4 x 4 float> %39, highp <4 x 4 float> %38, highp <4 x 4 float> %37, highp <4 x 4 float> %36)
	%41 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(highp <4 x 4 float> %39, highp <4 x 4 float> %38, highp <4 x 4 float> %37, highp <4 x 4 float> %36)
	%42 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(highp <4 x 4 float> %39, highp <4 x 4 float> %38, highp <4 x 4 float> %37, highp <4 x 4 float> %36)
	%43 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(highp <4 x 4 float> %39, highp <4 x 4 float> %38, highp <4 x 4 float> %37, highp <4 x 4 float> %36)
	%44 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
	%45 = call highp <4 x float> @OpenGLES.CallRegister()
	%46 = load highp <4 x 4 float>* @ModelViewMatrix.u.0.4, align 16
	%47 = load highp <4 x 4 float>* @ModelViewMatrix.u.1.4, align 16
	%48 = load highp <4 x 4 float>* @ModelViewMatrix.u.2.4, align 16
	%49 = load highp <4 x 4 float>* @ModelViewMatrix.u.3.4, align 16
	%50 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(highp <4 x 4 float> %49, highp <4 x 4 float> %48, highp <4 x 4 float> %47, highp <4 x 4 float> %46)
	%51 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(highp <4 x 4 float> %49, highp <4 x 4 float> %48, highp <4 x 4 float> %47, highp <4 x 4 float> %46)
	%52 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(highp <4 x 4 float> %49, highp <4 x 4 float> %48, highp <4 x 4 float> %47, highp <4 x 4 float> %46)
	%53 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(highp <4 x 4 float> %49, highp <4 x 4 float> %48, highp <4 x 4 float> %47, highp <4 x 4 float> %46)
	%54 = call highp <4 x float> @OpenGLES.VectorTimesMatrix4.6(highp <4 x float> %44, highp <4 x 4 float> %43, highp <4 x 4 float> %42, highp <4 x 4 float> %41, highp <4 x 4 float> %40, highp <4 x float> %45)
	%55 = call highp <4 x float> @OpenGLES.CallRegister()
	%56 = load highp <4 x 4 float>* @ProjectionMatrix.u.0.4, align 16
	%57 = load highp <4 x 4 float>* @ProjectionMatrix.u.1.4, align 16
	%58 = load highp <4 x 4 float>* @ProjectionMatrix.u.2.4, align 16
	%59 = load highp <4 x 4 float>* @ProjectionMatrix.u.3.4, align 16
	%60 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(highp <4 x 4 float> %59, highp <4 x 4 float> %58, highp <4 x 4 float> %57, highp <4 x 4 float> %56)
	%61 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(highp <4 x 4 float> %59, highp <4 x 4 float> %58, highp <4 x 4 float> %57, highp <4 x 4 float> %56)
	%62 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(highp <4 x 4 float> %59, highp <4 x 4 float> %58, highp <4 x 4 float> %57, highp <4 x 4 float> %56)
	%63 = call highp <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(highp <4 x 4 float> %59, highp <4 x 4 float> %58, highp <4 x 4 float> %57, highp <4 x 4 float> %56)
	%64 = call highp <4 x float> @OpenGLES.VectorTimesMatrix4.6(highp <4 x float> %54, highp <4 x 4 float> %53, highp <4 x 4 float> %52, highp <4 x 4 float> %51, highp <4 x 4 float> %50, highp <4 x float> %55)
	%65 = call highp <4 x float> @OpenGLES.CallRegister()
	%66 = call highp <4 x float> @OpenGLES.VectorTimesMatrix4.6(highp <4 x float> %64, highp <4 x 4 float> %63, highp <4 x 4 float> %62, highp <4 x 4 float> %61, highp <4 x 4 float> %60, highp <4 x float> %65)
	%67 = call highp <4 x float> @OpenGLES.CallRegister()
	@gl_Position = call highp <4 x float> @OpenGLES.END.2(highp <4 x float> %66, highp <4 x float> %67)
}


#### END COMPILER 0 INFO LOG ####
#### BEGIN LINKER INFO LOG ####

#### END LINKER INFO LOG ####
