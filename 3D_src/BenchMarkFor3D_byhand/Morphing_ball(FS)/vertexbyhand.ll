@rm_Vertex = attribute <4 x float> zeroinitializer
@rm_Normal = attribute <3 x float> zeroinitializer
@cube_texs = attribute <2 x float> zeroinitializer
@diffuse = varying <4 x float> zeroinitializer
@ambient = varying <4 x float> zeroinitializer
@normal = varying <3 x float> zeroinitializer
@lightDir = varying <3 x float> zeroinitializer
@halfVector = varying <3 x float> zeroinitializer
@TexCoord = varying <2 x float> zeroinitializer
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

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = load i32* @shaderr.u.0.1, align 16
	%1 = icmp eq i32 %0, 1
	;br i1 %0, label %L1.T, label %L1.F
;L1.T:
	;%1 = call <4 x float> @OpenGLES.LoadAttribute2.cube_texs.0()
	;%2 = shufflevector <4 x float> %1, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%3 = call <4 x float> @OpenGLES.VaryingPut4.TexCoord.1(<4 x float> %2)
	;br label %L1.E
;L1.F:
	;%rm_Vertex = call <4 x float> @OpenGLES.Normalize(<4 x float> %rm_Vertex)
	;%4 = extractelement <4 x float> %rm_Vertex, i32 1
	%2 = load float* @timeflag.u.0.1, align 16
	;%5 = sub float %4, @timeflag
	;%rm_Vertex = insertelement <4 x float> %rm_Vertex, float %5, i32 1
	;%6 = extractelement <4 x float> %rm_Vertex, i32 1
	;%7 = icmp slt, float %6, 0xC00434FB60000000
	;br i1 %7, label %L2.T, label %L2.F
;L2.T:
	;%rm_Vertex = insertelement <4 x float> %rm_Vertex, float 0xC00434FB60000000, i32 1
	;%8 = extractelement <3 x float> %rm_Normal, i32 0
	;%temp = insertelement <3 x float> %temp, float %8, i32 0
	;%9 = extractelement <3 x float> %rm_Normal, i32 1
	;%temp = insertelement <3 x float> %temp, float %9, i32 1
	;%temp = call <3 x float> @OpenGLES.Normalize(<3 x float> %temp)
	;%10 = extractelement <3 x float> %temp, i32 0
	;%11 = mul float 0x3FD3333340000000, %10
	;%12 = extractelement <4 x float> %rm_Vertex, i32 0
	;%13 = add float %12, %11
	;%rm_Vertex = insertelement <4 x float> %rm_Vertex, float %13, i32 0
	;%14 = extractelement <3 x float> %temp, i32 1
	;%15 = mul float 0x3FD3333340000000, %14
	;%16 = extractelement <4 x float> %rm_Vertex, i32 2
	;%17 = add float %16, %15
	;%rm_Vertex = insertelement <4 x float> %rm_Vertex, float %17, i32 2
;L2.F:
;L2.E:
;L1.E:
	%3 = load <3 x 3 float>* @NormalMatrix.u.0.3, align 16
	%4 = load <3 x 3 float>* @NormalMatrix.u.1.3, align 16
	%5 = load <3 x 3 float>* @NormalMatrix.u.2.3, align 16
	;%21 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %20, <3 x 3 float> %19, <3 x 3 float> %18)
	;%22 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %20, <3 x 3 float> %19, <3 x 3 float> %18)
	;%23 = call <3 x 3 float> @OpenGLES.ChangeMatrix3.3(<3 x 3 float> %20, <3 x 3 float> %19, <3 x 3 float> %18)
	;%24 = call <4 x float> @OpenGLES.LoadAttribute3.rm_Normal.0()
	;%25 = call <3 x float> @OpenGLES.CallRegister()
	;%26 = call <3 x float> @OpenGLES.VectorTimesMatrix4.5(<3 x float> %24, <3 x 3 float> %23, <3 x 3 float> %22, <3 x 3 float> %21, <3 x float> %25)
	;%normal = call <3 x float> @OpenGLES.Normalize(<3 x float> %26)
	%6 = load <3 x float>* @light_Pos.u.0.1, align 16
	;%lightDir = call <3 x float> @OpenGLES.Normalize(<3 x float> %light_Pos)
	%7 = load <3 x float>* @eye_Pos.u.0.1, align 16
	;%27 = add <3 x float> %light_Pos, @eye_Pos
	;%halfVector = call <3 x float> @OpenGLES.Normalize(<3 x float> %27)
	%8 = load <4 x float>* @diffuse_material.u.0.1, align 16
	%9 = load <4 x float>* @diffuse_light.u.0.1, align 16
	;%28 = mul <4 x float> %diffuse_material, @diffuse_light
	;%30 = shufflevector <4 x float> %29, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%31 = call <4 x float> @OpenGLES.VaryingPut4.diffuse.1(<4 x float> %30)
	%10 = load <4 x float>* @ambient_material.u.0.1, align 16
	%11 = load <4 x float>* @ambient_light.u.0.1, align 16
	;%32 = mul <4 x float> %ambient_material, @ambient_light
	;%34 = shufflevector <4 x float> %33, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%35 = call <4 x float> @OpenGLES.VaryingPut4.ambient.1(<4 x float> %34)
	%12 = load <4 x 4 float>* @ModelViewMatrix.u.0.4, align 16
	%13 = load <4 x 4 float>* @ModelViewMatrix.u.1.4, align 16
	%14 = load <4 x 4 float>* @ModelViewMatrix.u.2.4, align 16
	%15 = load <4 x 4 float>* @ModelViewMatrix.u.3.4, align 16
	;%40 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	;%41 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	;%42 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	;%43 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %39, <4 x 4 float> %38, <4 x 4 float> %37, <4 x 4 float> %36)
	;%44 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
	;%45 = call <4 x float> @OpenGLES.CallRegister()
	%16 = load <4 x 4 float>* @ProjectionMatrix.u.0.4, align 16
	%17 = load <4 x 4 float>* @ProjectionMatrix.u.1.4, align 16
	%18 = load <4 x 4 float>* @ProjectionMatrix.u.2.4, align 16
	%19 = load <4 x 4 float>* @ProjectionMatrix.u.3.4, align 16
	;%50 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	;%51 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	;%52 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	;%53 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %49, <4 x 4 float> %48, <4 x 4 float> %47, <4 x 4 float> %46)
	;%54 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %44, <4 x 4 float> %43, <4 x 4 float> %42, <4 x 4 float> %41, <4 x 4 float> %40, <4 x float> %45)
	;%55 = call <4 x float> @OpenGLES.CallRegister()
	;%56 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %54, <4 x 4 float> %53, <4 x 4 float> %52, <4 x 4 float> %51, <4 x 4 float> %50, <4 x float> %55)
	;%57 = call <4 x float> @OpenGLES.CallRegister()
	;%gl_Position = call <4 x float> @OpenGLES.END.2(<4 x float> %56, <4 x float> %57)

br label %return
return:
	ret void
}



