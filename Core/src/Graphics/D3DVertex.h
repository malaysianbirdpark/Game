#pragma once

namespace Engine::Graphics::Vertex {
	class Layout {
	public:
		enum ElementType {
		    Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
		};
		template<ElementType> struct Map;
		template<> struct Map<Position2D> {
		    using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat {DXGI_FORMAT_R32G32_FLOAT};
			static constexpr char const* semantic {"Position"};
			static constexpr char const* code {"P2"};
		};
		template<> struct Map<Position3D> {
		    using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat {DXGI_FORMAT_R32G32B32_FLOAT};
			static constexpr char const* semantic {"Position"};
			static constexpr char const* code {"P3"};
		};
		template<> struct Map<Texture2D> {
		    using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat {DXGI_FORMAT_R32G32_FLOAT};
			static constexpr char const* semantic {"TexCoord"};
			static constexpr char const* code {"T2"};
		};
		template<> struct Map<Normal> {
		    using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat {DXGI_FORMAT_R32G32B32_FLOAT};
			static constexpr char const* semantic {"Normal"};
			static constexpr char const* code {"N"};
		};
		template<> struct Map<Float3Color> {
		    using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat {DXGI_FORMAT_R32G32B32_FLOAT};
			static constexpr char const* semantic {"Color"};
			static constexpr char const* code {"C3"};
		};
		template<> struct Map<Float4Color> {
		    using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat {DXGI_FORMAT_R32G32B32A32_FLOAT};
			static constexpr char const* semantic {"Color"};
			static constexpr char const* code {"C4"};
		};

		class Element {
		public:
			Element(ElementType type, size_t offset)
			    : type{type}, offset{offset} {}

			[[nodiscard]] size_t GetOffsetAfter() const noexcept {
			    return offset + Size();
			}

			[[nodiscard]] size_t GetOffset() const {
			    return offset;
			}

			[[nodiscard]] size_t Size() const noexcept {
			    return SizeOf(type);
			}

			static constexpr size_t SizeOf(ElementType const type) noexcept {
                switch (type) {
                    case Position2D: 
                        return sizeof(Map<Position2D>::SysType);
                    case Position3D: 
                        return sizeof(Map<Position3D>::SysType);
                    case Texture2D:
                        return sizeof(Map<Texture2D>::SysType);
                    case Normal: 
                        return sizeof(Map<Normal>::SysType);
                    case Float3Color: 
                        return sizeof(Map<Float3Color>::SysType);
                    case Float4Color: 
                        return sizeof(Map<Float4Color>::SysType);
                    default: 
                        CORE_ASSERT(false, "Invalid element type");
                        return 0u;
                } 
			}

			[[nodiscard]] ElementType GetType() const noexcept {
			    return type;
			}

			[[nodiscard]] D3D11_INPUT_ELEMENT_DESC GetD3D11Desc() const noexcept {
			    switch(type) {
                case Position2D: 
					return GenerateD3D11Desc<Position2D>(GetOffset());
                case Position3D: 
					return GenerateD3D11Desc<Position3D>(GetOffset());
                case Texture2D: 
					return GenerateD3D11Desc<Texture2D>(GetOffset());
                case Normal: 
					return GenerateD3D11Desc<Normal>(GetOffset());
                case Float3Color: 
					return GenerateD3D11Desc<Float3Color>(GetOffset());
                case Float4Color:
					return GenerateD3D11Desc<Float4Color>(GetOffset());
                }
				CORE_ASSERT(false, "Invalid element type!");
				return {"INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
			}

			//[[nodiscard]] D3D12_INPUT_ELEMENT_DESC GetD3D12Desc() const noexcept {
			//    switch(type) {
   //             case Position2D: 
			//		return GenerateD3D12Desc<Position2D>(GetOffset());
   //             case Position3D: 
			//		return GenerateD3D12Desc<Position3D>(GetOffset());
   //             case Texture2D: 
			//		return GenerateD3D12Desc<Texture2D>(GetOffset());
   //             case Normal: 
			//		return GenerateD3D12Desc<Normal>(GetOffset());
   //             case Float3Color: 
			//		return GenerateD3D12Desc<Float3Color>(GetOffset());
   //             case Float4Color:
			//		return GenerateD3D12Desc<Float4Color>(GetOffset());
   //             }
			//	CORE_ASSERT(false, "Invalid element type!");
			//	return {"INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
			//}


			[[nodiscard]] const char* GetCode() const noexcept {
			    switch(type) {
				case Position2D: 
					return Map<Position2D>::code;
                case Position3D: 
					return Map<Position3D>::code;
                case Texture2D: 
					return Map<Texture2D>::code;
                case Normal: 
					return Map<Normal>::code;
                case Float3Color: 
					return Map<Float3Color>::code;
                case Float4Color: 
					return Map<Float4Color>::code;
                }
				CORE_ASSERT(false, "Invalied element type!");
				return "Invalid";
			}
		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateD3D11Desc(size_t const offset) noexcept {
			    return {Map<type>::semantic, 0, Map<type>::dxgiFormat, 0, static_cast<UINT>(offset), D3D11_INPUT_PER_VERTEX_DATA, 0};
			}

			//template<ElementType type>
			//static constexpr D3D12_INPUT_ELEMENT_DESC GenerateD3D12Desc(size_t const offset) noexcept {
			//    return {Map<type>::semantic, 0, Map<type>::dxgiFormat, 0, static_cast<UINT>(offset), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
			//}
		private:
			ElementType type;
			size_t offset;
		};
	public:
		template<ElementType Type>
		[[nodiscard]] Element const& Resolve() const noexcept {
		     for (auto const& e : elements) {
		            if (e.GetType() == Type) 
						return e;
		     }
			 CORE_ASSERT(false, "Could not resolve element type!");
			 return elements.front();
		}

		[[nodiscard]] Element const& ResolveByIndex(size_t const i) const noexcept {
			return elements[i];
		}

		[[maybe_unused]] Layout& Append(ElementType type) noexcept {
		    elements.emplace_back(type, Size());
			return *this;
		}

		[[nodiscard]] size_t Size() const noexcept {
		    return elements.empty() ? 0u : elements.back().GetOffsetAfter();
		}

		[[nodiscard]] size_t GetElementCount() const noexcept {
			return elements.size();
		}

		[[nodiscard]] Engine::x_vector<D3D11_INPUT_ELEMENT_DESC> GetD3D11Layout() const noexcept {
		    Engine::x_vector<D3D11_INPUT_ELEMENT_DESC> desc;
			desc.reserve(GetElementCount());
			std::ranges::transform(elements, std::back_inserter(desc), [](auto& e) {
			    return e.GetD3D11Desc();
			});
			return desc;
		}

		//[[nodiscard]] Engine::x_vector<D3D12_INPUT_ELEMENT_DESC> GetD3D12Layout() const noexcept {
		//    Engine::x_vector<D3D12_INPUT_ELEMENT_DESC> desc;
		//	desc.reserve(GetElementCount());
		//	std::ranges::transform(elements, std::back_inserter(desc), [](auto& e) {
		//	    return e.GetD3D12Desc();
		//	});
		//	return desc;
		//}

		[[nodiscard]] Engine::x_string GetCode() const noexcept {
			Engine::x_string code{};
			for (auto const& e : elements)
				code += e.GetCode();
			return code;
		}
	private:
		Engine::x_vector<Element> elements;
	};

    class Vertex {
	    friend class Buffer;
	public:
        template<Layout::ElementType Type>
		auto& Attr() noexcept {
			auto pAttribute {pData + layout.Resolve<Type>().GetOffset()};
			return *reinterpret_cast<typename Layout::Map<Type>::SysType*>(pAttribute);
        }

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcept {
            auto const& element {layout.ResolveByIndex(i)};
			auto pAttribute {pData + element.GetOffset()};
			switch (element.GetType()) {
			case Layout::Position2D: 
				SetAttribute<Layout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Position3D: 
				SetAttribute<Layout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Texture2D: 
				SetAttribute<Layout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Normal: 
				SetAttribute<Layout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Float3Color:
				SetAttribute<Layout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case Layout::Float4Color: 
				SetAttribute<Layout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
            default:
				CORE_ASSERT(false, "Bad Element Type!");
            }
        }
	protected:
		Vertex(char* pData, Layout const& layout) noexcept
			: pData{pData}, layout{layout}
	    {
	        CORE_ASSERT(pData != nullptr, "pData is nullptr!");
		}

		// enables Parameter pack setting of multiple parameters by element index
		template<typename First, typename ...Rest>
        void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept {
		    SetAttributeByIndex(i, std::forward<First>(first));
		    SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		// helper to reduce code duplication in SetAttributeByIndex
		template<Layout::ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val) noexcept {
			using Dest = typename Layout::Map<DestLayoutType>::SysType;
		    if constexpr(std::is_assignable_v<Dest, SrcType>)
				*reinterpret_cast<Dest*>(pAttribute) = val;
			else
				CORE_ASSERT(false, "Parameter attribute type mismatch");
		}
	private:
		char* pData {nullptr};
		Layout const& layout;
	};

	class ConstVertex {
	public:
		ConstVertex(Vertex const& v) noexcept
			: vertex{v} {}

		template<Layout::ElementType Type>
		auto const& Attr() const noexcept {
		    return const_cast<Vertex&>(vertex).Attr<Type>();
		}
	private:
		Vertex vertex;
	};

	class Buffer {
	public:
		Buffer(Layout layout) noexcept
			: layout{std::move(layout)} {}

		[[nodiscard]] char const* GetData() const noexcept {
		    return buffer.data();
		}

		[[nodiscard]] Layout const& GetLayout() const noexcept {
		    return layout;
		}

		[[nodiscard]] size_t Size() const noexcept {
		    return buffer.size() / layout.Size();
		}

		[[nodiscard]] size_t SizeBytes() const noexcept {
		    return buffer.size();
		}

		template<typename ...Params>
		void EmplaceBack(Params&&... params) noexcept {
			CORE_ASSERT(sizeof...(params) == layout.GetElementCount(), "Params count doesn't match number of vertex elements!");
		    buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		[[nodiscard]] Vertex Back() noexcept {
		    CORE_ASSERT(buffer.size() != 0u, "buffer is empty!");
			return Vertex{buffer.data() + buffer.size() - layout.Size(), layout};
		}

		[[nodiscard]] Vertex Front() noexcept {
		    CORE_ASSERT(buffer.size() != 0u, "buffer is empty!");
			return Vertex{buffer.data(), layout};
		}

		[[nodiscard]] Vertex operator[](size_t i) noexcept {
		    CORE_ASSERT(i < Size(), "invalid index!");
			return Vertex{buffer.data() + layout.Size() * i, layout};
		}

		[[nodiscard]] ConstVertex Back() const noexcept {
		    return const_cast<Buffer*>(this)->Back();
		}

		[[nodiscard]] ConstVertex Front() const noexcept {
		    return const_cast<Buffer*>(this)->Front();
		}

		[[nodiscard]] ConstVertex operator[](size_t const i) const noexcept {
		    return const_cast<Buffer&>(*this)[i];
		}
	private:
		Engine::x_vector<char> buffer;
        Layout layout;
	};
}
