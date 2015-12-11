@rm_Vertex = attribute <4 x float> zeroinitializer
@ProjectionMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix = uniform <4 x 4 float> zeroinitializer
@ModelViewMatrix1 = uniform <4 x 4 float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = load <4 x 4 float>* @ModelViewMatrix1.u.0.4, align 16
	%1 = load <4 x 4 float>* @ModelViewMatrix1.u.1.4, align 16
	%2 = load <4 x 4 float>* @ModelViewMatrix1.u.2.4, align 16
	%3 = load <4 x 4 float>* @ModelViewMatrix1.u.3.4, align 16
	%4 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %3, <4 x 4 float> %2, <4 x 4 float> %1, <4 x 4 float> %0)
	%5 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %3, <4 x 4 float> %2, <4 x 4 float> %1, <4 x 4 float> %0)
	%6 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %3, <4 x 4 float> %2, <4 x 4 float> %1, <4 x 4 float> %0)
	%7 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %3, <4 x 4 float> %2, <4 x 4 float> %1, <4 x 4 float> %0)
	%8 = call <4 x float> @OpenGLES.LoadAttribute4.rm_Vertex.0()
	%9 = call <4 x float> @OpenGLES.CallRegister()
	%10 = load <4 x 4 float>* @ModelViewMatrix.u.0.4, align 16
	%11 = load <4 x 4 float>* @ModelViewMatrix.u.1.4, align 16
	%12 = load <4 x 4 float>* @ModelViewMatrix.u.2.4, align 16
	%13 = load <4 x 4 float>* @ModelViewMatrix.u.3.4, align 16
	%14 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %13, <4 x 4 float> %12, <4 x 4 float> %11, <4 x 4 float> %10)
	%15 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %13, <4 x 4 float> %12, <4 x 4 float> %11, <4 x 4 float> %10)
	%16 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %13, <4 x 4 float> %12, <4 x 4 float> %11, <4 x 4 float> %10)
	%17 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %13, <4 x 4 float> %12, <4 x 4 float> %11, <4 x 4 float> %10)
	%18 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %8, <4 x 4 float> %7, <4 x 4 float> %6, <4 x 4 float> %5, <4 x 4 float> %4, <4 x float> %9)
	%19 = call <4 x float> @OpenGLES.CallRegister()
	%20 = load <4 x 4 float>* @ProjectionMatrix.u.0.4, align 16
	%21 = load <4 x 4 float>* @ProjectionMatrix.u.1.4, align 16
	%22 = load <4 x 4 float>* @ProjectionMatrix.u.2.4, align 16
	%23 = load <4 x 4 float>* @ProjectionMatrix.u.3.4, align 16
	%24 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.1(<4 x 4 float> %23, <4 x 4 float> %22, <4 x 4 float> %21, <4 x 4 float> %20)
	%25 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.2(<4 x 4 float> %23, <4 x 4 float> %22, <4 x 4 float> %21, <4 x 4 float> %20)
	%26 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.3(<4 x 4 float> %23, <4 x 4 float> %22, <4 x 4 float> %21, <4 x 4 float> %20)
	%27 = call <4 x 4 float> @OpenGLES.ChangeMatrix4.4.4(<4 x 4 float> %23, <4 x 4 float> %22, <4 x 4 float> %21, <4 x 4 float> %20)
	%28 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %18, <4 x 4 float> %17, <4 x 4 float> %16, <4 x 4 float> %15, <4 x 4 float> %14, <4 x float> %19)
	%29 = call <4 x float> @OpenGLES.CallRegister()
	%30 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %28, <4 x 4 float> %27, <4 x 4 float> %26, <4 x 4 float> %25, <4 x 4 float> %24, <4 x float> %29)
	%31 = call <4 x float> @OpenGLES.CallRegister()
	%gl_Position = call <4 x float> @OpenGLES.END.2(<4 x float> %30, <4 x float> %31)

br label %return
return:
	ret void
}



