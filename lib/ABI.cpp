#include <memory>
#include <stdexcept>
#include <string>

#include <llvm-abi/ABI.hpp>

#include "win64/ABI_Win64.hpp"
#include "x86/ABI_x86.hpp"
#include "x86_64/ABI_x86_64.hpp"

namespace llvm_abi {
	
	std::unique_ptr<ABI> createABI(llvm::Module& module, const llvm::Triple& targetTriple) {
		switch (targetTriple.getArch()) {
			case llvm::Triple::x86:
				return std::unique_ptr<ABI>(new ABI_x86(&module));
			case llvm::Triple::x86_64: {
				if (targetTriple.isOSWindows()) {
					return std::unique_ptr<ABI>(new ABI_Win64(&module));
				} else {
					return std::unique_ptr<ABI>(new ABI_x86_64(&module));
				}
			}
			default:
				break;
		}
		
		std::string errorString = "No ABI available for triple: ";
		errorString += targetTriple.str();
		throw std::runtime_error(errorString);
	}
	
}

