@rm_Vertex = attribute <4 x float> zeroinitializer
@rm_Normal = attribute <3 x float> zeroinitializer
@texcoord = attribute <2 x float> zeroinitializer
@ProjectionMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix1 = uniform <4 x 4 float> zeroinitializer
@NormalMatrix = uniform <3 x 3 float> zeroinitializer
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
@texcoord0 = varying <2 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = load <3 x 3 float>* @NormalMatrix.u.0.3, align 16
	%1 = load <3 x 3 float>* @NormalMatrix.u.1.3, align 16
	%2 = load <3 x 3 float>* @NormalMatrix.u.2.3, align 16
	%3 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %2, <3 x 3 float> %1, <3 x 3 float> %0)
	%4 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %2, <3 x 3 float> %1, <3 x 3 float> %0)
	%5 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %2, <3 x 3 float> %1, <3 x 3 float> %0)
	%6 = call <4 x float> @OpenGLES.LoadAttribute3.rm_Normal.0()
	%7 = call <3 x float> @OpenGLES.CallRegister()
	;%8 = call <3 x float> @OpenGLES.VectorTimesMatrix4.5(<3 x float> %6, <3 x 3 float> %5, <3 x 3 float> %4, <3 x 3 float> %3, <3 x float> %7)
	;%normal = call <3 x float> @OpenGLES.Normalize(<3 x float> %8)
	%8 = load <3 x float>* @light_Pos.u.0.1, align 16
	;%lightDir = call <3 x float> @OpenGLES.Normalize(<3 x float> %light_Pos)
	%9 = load <3 x float>* @eye_Pos.u.0.1, align 16
	;%9 = add <3 x float> %light_Pos, @eye_Pos
	;%halfVector = call <3 x float> @OpenGLES.Normalize(<3 x float> %9)
	%10 = load <4 x float>* @diffuse_material.u.0.1, align 16
	%11 = load <4 x float>* @diffuse_light.u.0.1, align 16
	;%10 = mul <4 x float> %diffuse_material, @diffuse_light
	;%12 = shufflevector <4 x float> %11, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%12 = load <4 x float>* @ambient_material.u.0.1, align 16
	%13 = load <4 x float>* @ambient_light.u.0.1, align 16
	%14 = load <4 x float>* @specular_material.u.0.1, align 16
	%15 = load <4 x float>* @specular_light.u.0.1, align 16
	%16 = load float* @specularExp.u.0.1, align 16
	;%13 = mul <4 x float> %ambient_material, @ambient_light
	;%15 = shufflevector <4 x float> %14, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%17 = shufflevector <4 x float> %16, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%n = call <3 x float> @OpenGLES.Normalize(<3 x float> %normal)
	;%18 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %lightDir)
	;%NdotL = call float @OpenGLES.Max(float %18, float 0x0000000000000000)
	;%19 = icmp sgt, float %NdotL, 0x0000000000000000
	;br i1 %19, label %L1.T, label %L1.F
;L1.T:
	;%halfV = call <3 x float> @OpenGLES.Normalize(<3 x float> %halfVector)
	;%20 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %halfV)
	;%NdotHV = call float @OpenGLES.Max(float %20, float 0x0000000000000000)
	;%21 = extractvalue {<4 x float>, <4 x float>, <4 x float>, <4 x float>, float
;br label %return
;return:
	;ret void
;} @gl_FrontMaterial, 4
	;%22 = extractvalue [32 x {<4 x float>, <4 x float>, <4 x float>, <4 x float>, <4 x float>, <3 x float>, float, float, float, float, float, float
;br label %return
;return:
	;ret void
;}] @gl_LightSource, 0, 2
	;%23 = call float @OpenGLES.Pow(float %NdotHV, float %21)
	;%24 = extractvalue {<4 x float>, <4 x float>, <4 x float>, <4 x float>, float
;br label %return
;return:
	;ret void
;} @gl_FrontMaterial, 3
	;%25 = call <4 x float> @OpenGLES.VectorTimesScalar(float %23, <4 x float> %22)
	;%26 = mul <4 x float> %25, %24
	;%27 = add <4 x float> %color, %26
	;%29 = shufflevector <4 x float> %28, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%30 = call <4 x float> @OpenGLES.VectorTimesScalar(float %NdotL, <4 x float> %diffuse)
	;%31 = add <4 x float> %color, %30
	;%33 = shufflevector <4 x float> %32, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
;L1.F:
;L1.E:
	;%34 = fdiv <4 x float> %color, 0x4008000000000000
	;%36 = shufflevector <4 x float> %35, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%37 = call <4 x float> @OpenGLES.VaryingPut4.Color_Light.1(<4 x float> %36)
	;%38 = call <4 x float> @OpenGLES.LoadAttribute2.texcoord.0()
	;%39 = shufflevector <4 x float> %38, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%40 = call <4 x float> @OpenGLES.VaryingPut4.texcoord0.1(<4 x float> %39)
	%17 = load <4 x 4 float>* @ModelViewMatrix1.u.0.4, align 16
	%18 = load <4 x 4 float>* @ModelViewMatrix1.u.1.4, align 16
	%19 = load <4 x 4 float>* @ModelViewMatrix1.u.2.4, align 16
	%20 = load <4 x 4 float>* @ModelViewMatrix1.u.3.4, align 16
	;%45 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %44, <4 x 4 float> %43, <4 x 4 float> %42, <4 x 4 float> %41)
	;%46 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %44, <4 x 4 float> %43, <4 x 4 float> %42, <4 x 4 float> %41)
	;%47 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %44, <4 x 4 float> %43, <4 x 4 float> %42, <4 x 4 float> %41)
	;%48 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %44, <4 x 4 float> %43, <4 x 4 float> %42, <4 x 4 float> %41)
	;%49 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
	;%50 = call <4 x float> @OpenGLES.CallRegister()
	%21 = load <4 x 4 float>* @ModelViewMatrix.u.0.4, align 16
	%22 = load <4 x 4 float>* @ModelViewMatrix.u.1.4, align 16
	%23 = load <4 x 4 float>* @ModelViewMatrix.u.2.4, align 16
	%24 = load <4 x 4 float>* @ModelViewMatrix.u.3.4, align 16
	;%55 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %54, <4 x 4 float> %53, <4 x 4 float> %52, <4 x 4 float> %51)
	;%56 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %54, <4 x 4 float> %53, <4 x 4 float> %52, <4 x 4 float> %51)
	;%57 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %54, <4 x 4 float> %53, <4 x 4 float> %52, <4 x 4 float> %51)
	;%58 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %54, <4 x 4 float> %53, <4 x 4 float> %52, <4 x 4 float> %51)
	;%59 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46, <4 x 4 float> %45, <4 x float> %50)
	;%60 = call <4 x float> @OpenGLES.CallRegister()
	%25 = load <4 x 4 float>* @ProjectionMatrix.u.0.4, align 16
	%26 = load <4 x 4 float>* @ProjectionMatrix.u.1.4, align 16
	%27 = load <4 x 4 float>* @ProjectionMatrix.u.2.4, align 16
	%28 = load <4 x 4 float>* @ProjectionMatrix.u.3.4, align 16
	;%65 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %64, <4 x 4 float> %63, <4 x 4 float> %62, <4 x 4 float> %61)
	;%66 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %64, <4 x 4 float> %63, <4 x 4 float> %62, <4 x 4 float> %61)
	;%67 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %64, <4 x 4 float> %63, <4 x 4 float> %62, <4 x 4 float> %61)
	;%68 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %64, <4 x 4 float> %63, <4 x 4 float> %62, <4 x 4 float> %61)
	;%69 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %59, <4 x 4 float> %58, <4 x 4 float> %57, <4 x 4 float> %56, <4 x 4 float> %55, <4 x float> %60)
	;%70 = call <4 x float> @OpenGLES.CallRegister()
	;%71 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %69, <4 x 4 float> %68, <4 x 4 float> %67, <4 x 4 float> %66, <4 x 4 float> %65, <4 x float> %70)
	;%72 = call <4 x float> @OpenGLES.CallRegister()
	;%gl_Position = call <4 x float> @OpenGLES.END.2(<4 x float> %71, <4 x float> %72)

br label %return
return:
	ret void
}



