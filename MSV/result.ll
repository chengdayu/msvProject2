; ModuleID = '_MSVOutput.c'
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-S32"
target triple = "i686-pc-windows-msvc"

$"\01??_C@_02DPKJAMEF@?$CFd?$AA@" = comdat any

$"\01??_C@_05PDJBBECF@pause?$AA@" = comdat any

@"\01??_C@_02DPKJAMEF@?$CFd?$AA@" = linkonce_odr unnamed_addr constant [3 x i8] c"%d\00", comdat, align 1
@"\01??_C@_05PDJBBECF@pause?$AA@" = linkonce_odr unnamed_addr constant [6 x i8] c"pause\00", comdat, align 1

; Function Attrs: nounwind
define i32 @main() #0 {
  %x = alloca i32, align 4
  store i32 0, i32* %x, align 4
  %1 = load i32, i32* %x, align 4
  %2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32 %1)
  %3 = call i32 @system(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"\01??_C@_05PDJBBECF@pause?$AA@", i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

declare i32 @system(i8*) #1

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (trunk)"}
