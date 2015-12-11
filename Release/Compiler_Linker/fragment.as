#### BEGIN COMPILER 0 INFO LOG ####
@FragColor_temp  = highp global <4 x float>
@color = highp varying <4 x float>

define highp float @lerp:f1;f1;f1(highp float %a, highp float %b, highp float %s) {
entry:
	%s_addr = alloca highp float
	%b_addr = alloca highp float
	%a_addr = alloca highp float
	store highp float %a, highp float* %a_addr
	store highp float %b, highp float* %b_addr
	store highp float %s, highp float* %s_addr
	%0 = load highp float* %a_addr, align 4
	%1 = load highp float* %b_addr, align 4
	%2 = fsub highp float %1, %0
	%3 = load highp float* %s_addr, align 4
	%4 = fmul highp float %3, %2
	%5 = fadd highp float %0, %4
	ret highp float %5
}


define highp void @main:void() {
	%sin_c = alloca highp float
	%weight = alloca highp float
	%arr0 = alloca highp i32
	%arrVal = alloca highp float
	%c = alloca highp float
	%sinValues = alloca highp [17 x float]
	%0 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 0
	%1 = load highp float* %0, align 4
	%2= fmul highp float %1, 0x0000000000000000
	%3= fadd highp float %2, 0x0000000000000000
	store highp float %3, highp float* %0
	%4 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 1
	%5 = load highp float* %4, align 4
	%6= fmul highp float %5, 0x0000000000000000
	%7= fadd highp float %6, 0x3FD87DE0E0000000
	store highp float %7, highp float* %4
	%8 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 2
	%9 = load highp float* %8, align 4
	%10= fmul highp float %9, 0x0000000000000000
	%11= fadd highp float %10, 0x3FE6A09EE0000000
	store highp float %11, highp float* %8
	%12 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 3
	%13 = load highp float* %12, align 4
	%14= fmul highp float %13, 0x0000000000000000
	%15= fadd highp float %14, 0x3FED906CC0000000
	store highp float %15, highp float* %12
	%16 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 4
	%17 = load highp float* %16, align 4
	%18= fmul highp float %17, 0x0000000000000000
	%19= fadd highp float %18, 0x3FF0000000000000
	store highp float %19, highp float* %16
	%20 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 5
	%21 = load highp float* %20, align 4
	%22= fmul highp float %21, 0x0000000000000000
	%23= fadd highp float %22, 0x3FED906CC0000000
	store highp float %23, highp float* %20
	%24 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 6
	%25 = load highp float* %24, align 4
	%26= fmul highp float %25, 0x0000000000000000
	%27= fadd highp float %26, 0x3FE6A09EE0000000
	store highp float %27, highp float* %24
	%28 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 7
	%29 = load highp float* %28, align 4
	%30= fmul highp float %29, 0x0000000000000000
	%31= fadd highp float %30, 0x3FD87DE0E0000000
	store highp float %31, highp float* %28
	%32 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 8
	%33 = load highp float* %32, align 4
	%34= fmul highp float %33, 0x0000000000000000
	%35= fadd highp float %34, 0x0000000000000000
	store highp float %35, highp float* %32
	%36 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 9
	%37 = load highp float* %36, align 4
	%38= fmul highp float %37, 0x0000000000000000
	%39= fadd highp float %38, 0xBFD87DE0E0000000
	store highp float %39, highp float* %36
	%40 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 10
	%41 = load highp float* %40, align 4
	%42= fmul highp float %41, 0x0000000000000000
	%43= fadd highp float %42, 0xBFE6A09EE0000000
	store highp float %43, highp float* %40
	%44 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 11
	%45 = load highp float* %44, align 4
	%46= fmul highp float %45, 0x0000000000000000
	%47= fadd highp float %46, 0xBFED906CC0000000
	store highp float %47, highp float* %44
	%48 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 12
	%49 = load highp float* %48, align 4
	%50= fmul highp float %49, 0x0000000000000000
	%51= fadd highp float %50, 0xBFF0000000000000
	store highp float %51, highp float* %48
	%52 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 13
	%53 = load highp float* %52, align 4
	%54= fmul highp float %53, 0x0000000000000000
	%55= fadd highp float %54, 0xBFED906AC0000000
	store highp float %55, highp float* %52
	%56 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 14
	%57 = load highp float* %56, align 4
	%58= fmul highp float %57, 0x0000000000000000
	%59= fadd highp float %58, 0xBFE6A09EE0000000
	store highp float %59, highp float* %56
	%60 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 15
	%61 = load highp float* %60, align 4
	%62= fmul highp float %61, 0x0000000000000000
	%63= fadd highp float %62, 0xBFD87DE0E0000000
	store highp float %63, highp float* %60
	%64 = getelementptr inboundshighp [17 x float]* %sinValues , i32 0 , i32 16
	%65 = load highp float* %64, align 4
	%66= fmul highp float %65, 0x0000000000000000
	%67= fadd highp float %66, 0x0000000000000000
	store highp float %67, highp float* %64
	%68 = call highp <4 x float> @OpenGLES.LoadVarying4.color.0()
	%69 = extractelement highp <4 x float> %68, i32 0
	%70 = fmul highp float %69, 0x400921FB60000000
	%71 = fmul highp float %70, 0x4000000000000000
	store highp float %71, highp float* %c
	%72 = load highp float* %c, align 4
	%73 = fmul highp float 0x40045F3060000000, %72
	store highp float %73, highp float* %arrVal
	%74 = load highp float* %arrVal, align 4
	%75 = call highp float @OpenGLES.Floor.f(highp float %74)
	%76 = call highp i32 @OpenGLES.Convert.fptosi(highp float %75)
	%77 = call highp float @OpenGLES.Floor.f(highp float %74)
	%78 = fsub highp float %74, %77
	store highp float %78, highp float* %weight
	store highp float 0x0000000000000000, highp float* %sin_c
	%79 = icmp eq highp i32 %76, 0
	store highp i32 %76, highp i32* %arr0
	br i1 %79, label %L1.T, label %L1.F
L1.T:
	%80 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 1
	%81 = load highp float* %80, align 4
	%82 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 0
	%83 = load highp float* %82, align 4
	%84 = load highp float* %weight, align 4
	%85 = call highp float @lerp:f1;f1;f1(highp float %83, highp float %81, highp float %84)
	store highp float %85, highp float* %sin_c
	br label %L1.E
L1.F:
	%86 = load highp i32* %arr0, align 4
	%87 = icmp eq highp i32 %86, 1
	br i1 %87, label %L2.T, label %L2.F
L2.T:
	%88 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 2
	%89 = load highp float* %88, align 4
	%90 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 1
	%91 = load highp float* %90, align 4
	%92 = load highp float* %weight, align 4
	%93 = call highp float @lerp:f1;f1;f1(highp float %91, highp float %89, highp float %92)
	store highp float %93, highp float* %sin_c
	br label %L2.E
L2.F:
	%94 = load highp i32* %arr0, align 4
	%95 = icmp eq highp i32 %94, 2
	br i1 %95, label %L3.T, label %L3.F
L3.T:
	%96 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 3
	%97 = load highp float* %96, align 4
	%98 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 2
	%99 = load highp float* %98, align 4
	%100 = load highp float* %weight, align 4
	%101 = call highp float @lerp:f1;f1;f1(highp float %99, highp float %97, highp float %100)
	store highp float %101, highp float* %sin_c
	br label %L3.E
L3.F:
	%102 = load highp i32* %arr0, align 4
	%103 = icmp eq highp i32 %102, 3
	br i1 %103, label %L4.T, label %L4.F
L4.T:
	%104 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 4
	%105 = load highp float* %104, align 4
	%106 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 3
	%107 = load highp float* %106, align 4
	%108 = load highp float* %weight, align 4
	%109 = call highp float @lerp:f1;f1;f1(highp float %107, highp float %105, highp float %108)
	store highp float %109, highp float* %sin_c
	br label %L4.E
L4.F:
	%110 = load highp i32* %arr0, align 4
	%111 = icmp eq highp i32 %110, 4
	br i1 %111, label %L5.T, label %L5.F
L5.T:
	%112 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 5
	%113 = load highp float* %112, align 4
	%114 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 4
	%115 = load highp float* %114, align 4
	%116 = load highp float* %weight, align 4
	%117 = call highp float @lerp:f1;f1;f1(highp float %115, highp float %113, highp float %116)
	store highp float %117, highp float* %sin_c
	br label %L5.E
L5.F:
	%118 = load highp i32* %arr0, align 4
	%119 = icmp eq highp i32 %118, 5
	br i1 %119, label %L6.T, label %L6.F
L6.T:
	%120 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 6
	%121 = load highp float* %120, align 4
	%122 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 5
	%123 = load highp float* %122, align 4
	%124 = load highp float* %weight, align 4
	%125 = call highp float @lerp:f1;f1;f1(highp float %123, highp float %121, highp float %124)
	store highp float %125, highp float* %sin_c
	br label %L6.E
L6.F:
	%126 = load highp i32* %arr0, align 4
	%127 = icmp eq highp i32 %126, 6
	br i1 %127, label %L7.T, label %L7.F
L7.T:
	%128 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 7
	%129 = load highp float* %128, align 4
	%130 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 6
	%131 = load highp float* %130, align 4
	%132 = load highp float* %weight, align 4
	%133 = call highp float @lerp:f1;f1;f1(highp float %131, highp float %129, highp float %132)
	store highp float %133, highp float* %sin_c
	br label %L7.E
L7.F:
	%134 = load highp i32* %arr0, align 4
	%135 = icmp eq highp i32 %134, 7
	br i1 %135, label %L8.T, label %L8.F
L8.T:
	%136 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 8
	%137 = load highp float* %136, align 4
	%138 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 7
	%139 = load highp float* %138, align 4
	%140 = load highp float* %weight, align 4
	%141 = call highp float @lerp:f1;f1;f1(highp float %139, highp float %137, highp float %140)
	store highp float %141, highp float* %sin_c
	br label %L8.E
L8.F:
	%142 = load highp i32* %arr0, align 4
	%143 = icmp eq highp i32 %142, 8
	br i1 %143, label %L9.T, label %L9.F
L9.T:
	%144 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 9
	%145 = load highp float* %144, align 4
	%146 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 8
	%147 = load highp float* %146, align 4
	%148 = load highp float* %weight, align 4
	%149 = call highp float @lerp:f1;f1;f1(highp float %147, highp float %145, highp float %148)
	store highp float %149, highp float* %sin_c
	br label %L9.E
L9.F:
	%150 = load highp i32* %arr0, align 4
	%151 = icmp eq highp i32 %150, 9
	br i1 %151, label %L10.T, label %L10.F
L10.T:
	%152 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 10
	%153 = load highp float* %152, align 4
	%154 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 9
	%155 = load highp float* %154, align 4
	%156 = load highp float* %weight, align 4
	%157 = call highp float @lerp:f1;f1;f1(highp float %155, highp float %153, highp float %156)
	store highp float %157, highp float* %sin_c
	br label %L10.E
L10.F:
	%158 = load highp i32* %arr0, align 4
	%159 = icmp eq highp i32 %158, 10
	br i1 %159, label %L11.T, label %L11.F
L11.T:
	%160 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 11
	%161 = load highp float* %160, align 4
	%162 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 10
	%163 = load highp float* %162, align 4
	%164 = load highp float* %weight, align 4
	%165 = call highp float @lerp:f1;f1;f1(highp float %163, highp float %161, highp float %164)
	store highp float %165, highp float* %sin_c
	br label %L11.E
L11.F:
	%166 = load highp i32* %arr0, align 4
	%167 = icmp eq highp i32 %166, 11
	br i1 %167, label %L12.T, label %L12.F
L12.T:
	%168 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 12
	%169 = load highp float* %168, align 4
	%170 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 11
	%171 = load highp float* %170, align 4
	%172 = load highp float* %weight, align 4
	%173 = call highp float @lerp:f1;f1;f1(highp float %171, highp float %169, highp float %172)
	store highp float %173, highp float* %sin_c
	br label %L12.E
L12.F:
	%174 = load highp i32* %arr0, align 4
	%175 = icmp eq highp i32 %174, 12
	br i1 %175, label %L13.T, label %L13.F
L13.T:
	%176 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 13
	%177 = load highp float* %176, align 4
	%178 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 12
	%179 = load highp float* %178, align 4
	%180 = load highp float* %weight, align 4
	%181 = call highp float @lerp:f1;f1;f1(highp float %179, highp float %177, highp float %180)
	store highp float %181, highp float* %sin_c
	br label %L13.E
L13.F:
	%182 = load highp i32* %arr0, align 4
	%183 = icmp eq highp i32 %182, 13
	br i1 %183, label %L14.T, label %L14.F
L14.T:
	%184 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 14
	%185 = load highp float* %184, align 4
	%186 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 13
	%187 = load highp float* %186, align 4
	%188 = load highp float* %weight, align 4
	%189 = call highp float @lerp:f1;f1;f1(highp float %187, highp float %185, highp float %188)
	store highp float %189, highp float* %sin_c
	br label %L14.E
L14.F:
	%190 = load highp i32* %arr0, align 4
	%191 = icmp eq highp i32 %190, 14
	br i1 %191, label %L15.T, label %L15.F
L15.T:
	%192 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 15
	%193 = load highp float* %192, align 4
	%194 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 14
	%195 = load highp float* %194, align 4
	%196 = load highp float* %weight, align 4
	%197 = call highp float @lerp:f1;f1;f1(highp float %195, highp float %193, highp float %196)
	store highp float %197, highp float* %sin_c
	br label %L15.E
L15.F:
	%198 = load highp i32* %arr0, align 4
	%199 = icmp eq highp i32 %198, 15
	br i1 %199, label %L16.T, label %L16.F
L16.T:
	%200 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 16
	%201 = load highp float* %200, align 4
	%202 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 15
	%203 = load highp float* %202, align 4
	%204 = load highp float* %weight, align 4
	%205 = call highp float @lerp:f1;f1;f1(highp float %203, highp float %201, highp float %204)
	store highp float %205, highp float* %sin_c
	br label %L16.E
L16.F:
	%206 = load highp i32* %arr0, align 4
	%207 = icmp eq highp i32 %206, 16
	br i1 %207, label %L17.T, label %L17.E
L17.T:
	%208 = getelementptr inbounds highp [17 x float]* %sinValues, i32 0, i32 16
	%209 = load highp float* %208, align 4
	store highp float %209, highp float* %sin_c
	br label %L17.E
L17.E:
	br label %L16.E
L16.E:
	br label %L15.E
L15.E:
	br label %L14.E
L14.E:
	br label %L13.E
L13.E:
	br label %L12.E
L12.E:
	br label %L11.E
L11.E:
	br label %L10.E
L10.E:
	br label %L9.E
L9.E:
	br label %L8.E
L8.E:
	br label %L7.E
L7.E:
	br label %L6.E
L6.E:
	br label %L5.E
L5.E:
	br label %L4.E
L4.E:
	br label %L3.E
L3.E:
	br label %L2.E
L2.E:
	br label %L1.E
L1.E:
	%210 = load highp float* %sin_c, align 4
	%211 = fmul highp float %210, 0x3FE0000000000000
	%212 = fadd highp float %211, 0x3FE0000000000000
	%213 = call highp <4 x float> @OpenGLES.ConstructVec4.f.f.f.f(highp float %212, highp float 0x0000000000000000, highp float 0x0000000000000000, highp float 0x3FF0000000000000)
	%214 = shufflevector highp <4 x float> %213, highp <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%215 = call highp <4 x float> @OpenGLES.ENDFrag.1(highp <4 x float> %214)
main function end
}
add_metadata


#### END COMPILER 0 INFO LOG ####
#### BEGIN LINKER INFO LOG ####

#### END LINKER INFO LOG ####
