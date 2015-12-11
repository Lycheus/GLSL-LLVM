@rm_Vertex = attribute <4 x float> zeroinitializer
@rm_Normal = attribute <3 x float> zeroinitializer
@cube_texs = attribute <2 x float> zeroinitializer
@Color_Light = varying <4 x float> zeroinitializer
@TexCoord = varying <2 x float> zeroinitializer
@normal = varying <3 x float> zeroinitializer
@shaderr = uniform i32 zeroinitializer
@timeflag = uniform float zeroinitializer
@ProjectionMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix = uniform <4 x 4 float> zeroinitializer
@NormalMatrix = uniform <3 x 3 float> zeroinitializer
@ambient_light = uniform <4 x float> zeroinitializer
@ambient_material = uniform <4 x float> zeroinitializer
@diffuse_light = uniform <4 x float> zeroinitializer
@diffuse_material = uniform <4 x float> zeroinitializer
@eye_Pos = uniform <3 x float> zeroinitializer
@light_Pos = uniform <3 x float> zeroinitializer
@specular_light = uniform <4 x float> zeroinitializer
@specular_material = uniform <4 x float> zeroinitializer
@specularExp = uniform float zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
	%1 = shufflevector <4 x float> %0, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%2 = load i32* @shaderr.u.0.1, align 16
	%3 = icmp eq i32 %2, 1
	;br i1 %2, label %L1.T, label %L1.F
;L1.T:
	;%3 = call <4 x float> @OpenGLES.LoadAttribute2.cube_texs.0()
	;%4 = shufflevector <4 x float> %3, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%5 = call <4 x float> @OpenGLES.VaryingPut4.TexCoord.1(<4 x float> %4)
	;br label %L1.E
;L1.F:
	;%temp_vertex = call <4 x float> @OpenGLES.Normalize(<4 x float> %temp_vertex)
	;%6 = extractelement <4 x float> %temp_vertex, i32 1
	%4 = load float* @timeflag.u.0.1, align 16
	;%7 = sub float %6, @timeflag
	;%temp_vertex = insertelement <4 x float> %temp_vertex, float %7, i32 1
	;%8 = extractelement <4 x float> %temp_vertex, i32 1
	;%9 = icmp slt, float %8, 0xC00434FB60000000
	;br i1 %9, label %L2.T, label %L2.F
;L2.T:
	;%temp_vertex = insertelement <4 x float> %temp_vertex, float 0xC00434FB60000000, i32 1
	;%10 = extractelement <3 x float> %rm_Normal, i32 0
	;%temp = insertelement <3 x float> %temp, float %10, i32 0
	;%11 = extractelement <3 x float> %rm_Normal, i32 1
	;%temp = insertelement <3 x float> %temp, float %11, i32 1
	;%temp = call <3 x float> @OpenGLES.Normalize(<3 x float> %temp)
	;%12 = extractelement <3 x float> %temp, i32 0
	;%13 = mul float 0x3FD3333340000000, %12
	;%14 = extractelement <4 x float> %temp_vertex, i32 0
	;%15 = add float %14, %13
	;%temp_vertex = insertelement <4 x float> %temp_vertex, float %15, i32 0
	;%16 = extractelement <3 x float> %temp, i32 1
	;%17 = mul float 0x3FD3333340000000, %16
	;%18 = extractelement <4 x float> %temp_vertex, i32 2
	;%19 = add float %18, %17
	;%temp_vertex = insertelement <4 x float> %temp_vertex, float %19, i32 2
;L2.F:
;L2.E:
;L1.E:
	%5 = load <3 x 3 float>* @NormalMatrix.u.0.3, align 16
	%6 = load <3 x 3 float>* @NormalMatrix.u.1.3, align 16
	%7 = load <3 x 3 float>* @NormalMatrix.u.2.3, align 16
	;%23 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %22, <3 x 3 float> %21, <3 x 3 float> %20)
	;%24 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %22, <3 x 3 float> %21, <3 x 3 float> %20)
	;%25 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %22, <3 x 3 float> %21, <3 x 3 float> %20)
	;%26 = call <4 x float> @OpenGLES.LoadAttribute3.rm_Normal.0()
	;%27 = call <3 x float> @OpenGLES.CallRegister()
	;%28 = call <3 x float> @OpenGLES.VectorTimesMatrix4.5(<3 x float> %26, <3 x 3 float> %25, <3 x 3 float> %24, <3 x 3 float> %23, <3 x float> %27)
	;%normal = call <3 x float> @OpenGLES.Normalize(<3 x float> %28)
	%8 = load <3 x float>* @light_Pos.u.0.1, align 16
	;%lightDir = call <3 x float> @OpenGLES.Normalize(<3 x float> %light_Pos)
	%9 = load <3 x float>* @eye_Pos.u.0.1, align 16
	;%29 = add <3 x float> %light_Pos, @eye_Pos
	;%halfVector = call <3 x float> @OpenGLES.Normalize(<3 x float> %29)
	%10 = load <4 x float>* @diffuse_material.u.0.1, align 16
	%11 = load <4 x float>* @diffuse_light.u.0.1, align 16
	;%30 = mul <4 x float> %diffuse_material, @diffuse_light
	;%32 = shufflevector <4 x float> %31, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%12 = load <4 x float>* @ambient_material.u.0.1, align 16
	%13 = load <4 x float>* @ambient_light.u.0.1, align 16
	;%33 = mul <4 x float> %ambient_material, @ambient_light
	;%35 = shufflevector <4 x float> %34, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%37 = shufflevector <4 x float> %36, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%n = call <3 x float> @OpenGLES.Normalize(<3 x float> %normal)
	;%38 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %lightDir)
	;%NdotL = call float @OpenGLES.Max(float %38, float 0x0000000000000000)
	;%39 = icmp sgt, float %NdotL, 0x0000000000000000
	;br i1 %39, label %L3.T, label %L3.F
;L3.T:
	;%halfV = call <3 x float> @OpenGLES.Normalize(<3 x float> %halfVector)
	;%40 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %halfV)
	;%NdotHV = call float @OpenGLES.Max(float %40, float 0x0000000000000000)
	%14 = load float* @specularExp.u.0.1, align 16
	;%41 = call float @OpenGLES.Pow(float %NdotHV, float @specularExp)
	%15 = load <4 x float>* @specular_material.u.0.1, align 16
	;%42 = call <4 x float> @OpenGLES.VectorTimesScalar(float %41, <4 x float> %specular_material)
	%16 = load <4 x float>* @specular_light.u.0.1, align 16
	;%43 = mul <4 x float> %42, @specular_light
	;%44 = add <4 x float> %color_light, %43
	;%46 = shufflevector <4 x float> %45, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%47 = call <4 x float> @OpenGLES.VectorTimesScalar(float %NdotL, <4 x float> %diffuse)
	;%48 = add <4 x float> %color_light, %47
	;%50 = shufflevector <4 x float> %49, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
;L3.F:
;L3.E:
	;%52 = shufflevector <4 x float> %51, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%53 = call <4 x float> @OpenGLES.VaryingPut4.Color_Light.1(<4 x float> %52)
	%17 = load <4 x 4 float>* @ModelViewMatrix.u.0.4, align 16
	%18 = load <4 x 4 float>* @ModelViewMatrix.u.1.4, align 16
	%19 = load <4 x 4 float>* @ModelViewMatrix.u.2.4, align 16
	%20 = load <4 x 4 float>* @ModelViewMatrix.u.3.4, align 16
	;%58 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %57, <4 x 4 float> %56, <4 x 4 float> %55, <4 x 4 float> %54)
	;%59 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %57, <4 x 4 float> %56, <4 x 4 float> %55, <4 x 4 float> %54)
	;%60 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %57, <4 x 4 float> %56, <4 x 4 float> %55, <4 x 4 float> %54)
	;%61 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %57, <4 x 4 float> %56, <4 x 4 float> %55, <4 x 4 float> %54)
	;%62 = load <4 x float>* %temp_vertex., align 16
	;%63 = call <4 x float> @OpenGLES.CallRegister()
	%21 = load <4 x 4 float>* @ProjectionMatrix.u.0.4, align 16
	%22 = load <4 x 4 float>* @ProjectionMatrix.u.1.4, align 16
	%23 = load <4 x 4 float>* @ProjectionMatrix.u.2.4, align 16
	%24 = load <4 x 4 float>* @ProjectionMatrix.u.3.4, align 16
	;%68 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %67, <4 x 4 float> %66, <4 x 4 float> %65, <4 x 4 float> %64)
	;%69 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %67, <4 x 4 float> %66, <4 x 4 float> %65, <4 x 4 float> %64)
	;%70 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %67, <4 x 4 float> %66, <4 x 4 float> %65, <4 x 4 float> %64)
	;%71 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %67, <4 x 4 float> %66, <4 x 4 float> %65, <4 x 4 float> %64)
	;%72 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %62, <4 x 4 float> %61, <4 x 4 float> %60, <4 x 4 float> %59, <4 x 4 float> %58, <4 x float> %63)
	;%73 = call <4 x float> @OpenGLES.CallRegister()
	;%74 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %72, <4 x 4 float> %71, <4 x 4 float> %70, <4 x 4 float> %69, <4 x 4 float> %68, <4 x float> %73)
	;%75 = call <4 x float> @OpenGLES.CallRegister()
	;%gl_Position = call <4 x float> @OpenGLES.END.2(<4 x float> %74, <4 x float> %75)

br label %return
return:
	ret void
}



