@rm_Vertex = attribute <4 x float> zeroinitializer
@rm_Normal = attribute <3 x float> zeroinitializer
@texcoord = attribute <2 x float> zeroinitializer
@ModelViewMatrix1 = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix = uniform <4 x 4 float> zeroinitializer
@ProjectionMatrix = uniform <4 x 4 float> zeroinitializer
@texcoord0 = varying <2 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = call <4 x float> @OpenGLES.LoadAttribute2.texcoord.0()
	%1 = shufflevector <4 x float> %0, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%2 = call <4 x float> @OpenGLES.VaryingPut4.texcoord0.1(<4 x float> %1)
	%3 = load <4 x 4 float>* @ModelViewMatrix1.u.0.4, align 16
	%4 = load <4 x 4 float>* @ModelViewMatrix1.u.1.4, align 16
	%5 = load <4 x 4 float>* @ModelViewMatrix1.u.2.4, align 16
	%6 = load <4 x 4 float>* @ModelViewMatrix1.u.3.4, align 16
	%7 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %6, <4 x 4 float> %5, <4 x 4 float> %4, <4 x 4 float> %3)
	%8 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %6, <4 x 4 float> %5, <4 x 4 float> %4, <4 x 4 float> %3)
	%9 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %6, <4 x 4 float> %5, <4 x 4 float> %4, <4 x 4 float> %3)
	%10 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %6, <4 x 4 float> %5, <4 x 4 float> %4, <4 x 4 float> %3)
	%11 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
	%12 = call <4 x float> @OpenGLES.CallRegister()
	%13 = load <4 x 4 float>* @ModelViewMatrix.u.0.4, align 16
	%14 = load <4 x 4 float>* @ModelViewMatrix.u.1.4, align 16
	%15 = load <4 x 4 float>* @ModelViewMatrix.u.2.4, align 16
	%16 = load <4 x 4 float>* @ModelViewMatrix.u.3.4, align 16
	%17 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %16, <4 x 4 float> %15, <4 x 4 float> %14, <4 x 4 float> %13)
	%18 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %16, <4 x 4 float> %15, <4 x 4 float> %14, <4 x 4 float> %13)
	%19 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %16, <4 x 4 float> %15, <4 x 4 float> %14, <4 x 4 float> %13)
	%20 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %16, <4 x 4 float> %15, <4 x 4 float> %14, <4 x 4 float> %13)
	%21 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %11, <4 x 4 float> %10, <4 x 4 float> %9, <4 x 4 float> %8, <4 x 4 float> %7, <4 x float> %12)
	%22 = call <4 x float> @OpenGLES.CallRegister()
	%23 = load <4 x 4 float>* @ProjectionMatrix.u.0.4, align 16
	%24 = load <4 x 4 float>* @ProjectionMatrix.u.1.4, align 16
	%25 = load <4 x 4 float>* @ProjectionMatrix.u.2.4, align 16
	%26 = load <4 x 4 float>* @ProjectionMatrix.u.3.4, align 16
	%27 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %26, <4 x 4 float> %25, <4 x 4 float> %24, <4 x 4 float> %23)
	%28 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %26, <4 x 4 float> %25, <4 x 4 float> %24, <4 x 4 float> %23)
	%29 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %26, <4 x 4 float> %25, <4 x 4 float> %24, <4 x 4 float> %23)
	%30 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %26, <4 x 4 float> %25, <4 x 4 float> %24, <4 x 4 float> %23)
	%31 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %21, <4 x 4 float> %20, <4 x 4 float> %19, <4 x 4 float> %18, <4 x 4 float> %17, <4 x float> %22)
	%32 = call <4 x float> @OpenGLES.CallRegister()
	%33 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %31, <4 x 4 float> %30, <4 x 4 float> %29, <4 x 4 float> %28, <4 x 4 float> %27, <4 x float> %32)
	%34 = call <4 x float> @OpenGLES.CallRegister()
	%gl_Position = call <4 x float> @OpenGLES.END.2(<4 x float> %33, <4 x float> %34)

br label %return
return:
	ret void
}



