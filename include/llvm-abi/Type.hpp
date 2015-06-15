#ifndef LLVMABI_ABITYPE_HPP
#define LLVMABI_ABITYPE_HPP

#include <string>
#include <vector>

#include <llvm/ADT/ArrayRef.h>

namespace llvm_abi {
	
	/**
	 * \brief Integer Kind
	 * 
	 * A set of possible integer types.
	 */
	enum IntegerKind {
		Bool,
		Char,
		Short,
		Int,
		Long,
		LongLong,
		Int8,
		Int16,
		Int32,
		Int64,
		Int128,
		SizeT,
		PtrDiffT
	};
	
	/**
	 * \brief Floating Point Kind
	 * 
	 * A set of possible floating point types.
	 */
	enum FloatingPointKind {
		Float,
		Double,
		LongDouble,
		Float128
	};
	
	/**
	 * \brief Type Kind
	 * 
	 * A set of possible type kinds.
	 */
	enum TypeKind {
		VoidType,
		PointerType,
		IntegerType,
		FloatingPointType,
		ComplexType,
		StructType,
		ArrayType
	};
	
	// Forward declaration.
	class StructMember;
	class TypeBuilder;
	
	/**
	 * \brief ABI Type
	 * 
	 * A representation of a C type for the purpose
	 * of ABI-compliant code generation.
	 */
	class Type {
		public:
			/**
			 * \brief Void Type
			 */
			static Type Void();
			
			/**
			 * \brief Integer Type
			 */
			static Type Pointer();
			
			/**
			 * \brief Integer Type
			 */
			static Type Integer(IntegerKind kind);
			
			/**
			 * \brief Floating Point Type
			 */
			static Type FloatingPoint(FloatingPointKind kind);
			
			/**
			 * \brief Complex Type
			 */
			static Type Complex(FloatingPointKind kind);
			
			/**
			 * \brief Struct Type
			 */
			static Type Struct(const TypeBuilder& typeBuilder, llvm::ArrayRef<StructMember> members);
			
			/**
			 * \brief Auto-aligned Struct Type
			 */
			static Type AutoStruct(const TypeBuilder& typeBuilder, llvm::ArrayRef<Type> memberTypes);
			
			/**
			 * \brief Array Type
			 */
			static Type Array(const TypeBuilder& typeBuilder, size_t elementCount, Type elementType);
			
			Type() : kind_(VoidType) { }
			
			bool operator==(const Type& type) const;
			bool operator!=(const Type& type) const;
			bool operator<(const Type& type) const;
			
			TypeKind kind() const;
			
			bool isVoid() const;
			
			bool isPointer() const;
			
			bool isInteger() const;
			
			IntegerKind integerKind() const;
			
			bool isFloatingPoint() const;
			
			FloatingPointKind floatingPointKind() const;
			
			bool isComplex() const;
			
			FloatingPointKind complexKind() const;
			
			bool isStruct() const;
			
			llvm::ArrayRef<StructMember> structMembers() const;
			
			bool isArray() const;
			
			size_t arrayElementCount() const;
			
			Type arrayElementType() const;
			
			size_t hash() const;
			
			std::string toString() const;
			
			struct TypeData;
			
		private:
			Type(TypeKind kind);
			
			TypeKind kind_;
			
			union {
				IntegerKind integerKind;
				FloatingPointKind floatingPointKind;
				FloatingPointKind complexKind;
				
				// Aggregate types are uniqued by a pointer.
				const TypeData* uniquedPointer;
			} subKind_;
			
	};
	
	/**
	 * \brief ABI Struct Member
	 */
	class StructMember {
		public:
			static StructMember AutoOffset(const Type type) {
				return StructMember(type, 0);
			}
			
			static StructMember ForceOffset(const Type type,
			                                const size_t offset) {
				return StructMember(type, offset);
			}
			
			Type type() const {
				return type_;
			}
			
			size_t offset() const {
				return offset_;
			}
			
			bool operator==(const StructMember& other) const {
				return type() == other.type() &&
				       offset() == other.offset();
			}
			
			bool operator<(const StructMember& other) const {
				if (type() != other.type()) {
					return type() < other.type();
				}
				
				if (offset() != other.offset()) {
					return offset() < other.offset();
				}
				
				return false;
			}
			
		private:
			StructMember(const Type pType,
			             const size_t pOffset)
			: type_(pType), offset_(pOffset) { }
			
			Type type_;
			size_t offset_;
			
	};
	
	struct Type::TypeData {
		struct {
			llvm::SmallVector<StructMember, 8> members;
		} structType;
		
		struct ArrayTypeData {
			size_t elementCount;
			Type elementType;
			
			ArrayTypeData()
			: elementCount(0) { }
		} arrayType;
		
		bool operator<(const TypeData& other) const {
			if (structType.members != other.structType.members) {
				return structType.members < other.structType.members;
			}
			
			if (arrayType.elementCount != other.arrayType.elementCount) {
				return arrayType.elementCount < other.arrayType.elementCount;
			}
			
			if (arrayType.elementType != other.arrayType.elementType) {
				return arrayType.elementType < other.arrayType.elementType;
			}
			
			return false;
		}
	};
	
	static const Type VoidTy = Type::Void();
	
	static const Type BoolTy = Type::Integer(Bool);
	static const Type CharTy = Type::Integer(Char);
	static const Type ShortTy = Type::Integer(Short);
	static const Type IntTy = Type::Integer(Int);
	static const Type LongTy = Type::Integer(Long);
	static const Type LongLongTy = Type::Integer(LongLong);
	
	static const Type FloatTy = Type::FloatingPoint(Float);
	static const Type DoubleTy = Type::FloatingPoint(Double);
	
}

namespace std {
	
	template <> struct hash<llvm_abi::Type> {
		size_t operator()(const llvm_abi::Type& type) const {
			return type.hash();
		}
	};
	
}

#endif
