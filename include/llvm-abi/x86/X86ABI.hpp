#ifndef LLVMABI_X86_X86ABI_HPP
#define LLVMABI_X86_X86ABI_HPP

#include <vector>

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include <llvm-abi/ABI.hpp>
#include <llvm-abi/CallingConvention.hpp>
#include <llvm-abi/Type.hpp>

#include <llvm-abi/x86/X86ABITypeInfo.hpp>

namespace llvm_abi {
	
	namespace x86 {
		
		class X86ABI: public ABI {
		public:
			X86ABI(llvm::Module* module);
			~X86ABI();
			
			std::string name() const;
			
			const ABITypeInfo& typeInfo() const;
			
			llvm::CallingConv::ID getCallingConvention(CallingConvention callingConvention) const;
			
			llvm::FunctionType* getFunctionType(const FunctionType& functionType) const;
			
			llvm::AttributeSet getAttributes(const FunctionType& functionType,
			                                 llvm::AttributeSet existingAttributes) const;
			
			llvm::Value* createCall(Builder& builder,
			                        const FunctionType& functionType,
			                        std::function<llvm::Value* (llvm::ArrayRef<llvm::Value*>)> callBuilder,
			                        llvm::ArrayRef<TypedValue> arguments) const;
			
			std::unique_ptr<FunctionEncoder>
			createFunctionEncoder(Builder& builder,
			                      const FunctionType& functionType,
			                      llvm::ArrayRef<llvm::Value*> arguments) const;
			
		private:
			llvm::LLVMContext& llvmContext_;
			X86ABITypeInfo typeInfo_;
			
		};
		
	}
	
}

#endif
