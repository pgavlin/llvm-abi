; ABI: x86_64-none-linux-gnu
; FUNCTION-TYPE: void ([1 x {double, int, long }])

declare void @callee([1 x { double, i32, i64 }]* byval align 16)

define void @caller([1 x { double, i32, i64 }]* byval align 16) {
  %indirect.arg.mem = alloca [1 x { double, i32, i64 }], align 16
  %2 = load [1 x { double, i32, i64 }]* %0
  store [1 x { double, i32, i64 }] %2, [1 x { double, i32, i64 }]* %indirect.arg.mem
  call void @callee([1 x { double, i32, i64 }]* byval align 16 %indirect.arg.mem)
  ret void
}
