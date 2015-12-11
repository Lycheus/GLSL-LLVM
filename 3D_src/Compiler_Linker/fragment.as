#### BEGIN COMPILER 0 INFO LOG ####
@Color_RGB = highp uniform <4 x float>

define highp void @main:void() {
	%0 = load highp <4 x float>* @Color_RGB.u.0.1, align 16
	%1 = shufflevector highp <4 x float> %0, highp <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	@gl_FragColor = call highp <4 x float> @OpenGLES.ENDFrag.1(highp <4 x float> %1)
}


#### END COMPILER 0 INFO LOG ####
#### BEGIN LINKER INFO LOG ####

#### END LINKER INFO LOG ####
