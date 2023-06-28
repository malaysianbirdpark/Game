#pragma once

namespace Engine::Graphics {
    class Viewport;
    class ScissorRect;
    class GraphicsDevice;
    class CommandQueue;
    class Commander;
    class Fence;
    class RenderTargetView;
    class SwapChain;
    class RootSignature;
    class VertexBuffer;
    class VertexShader;
    class PixelShader;
    class PipelineState;
    class Triangle;

    struct WindowData {
        int width;
        int height;
        HWND nativeWnd;
        bool windowed;
    };
    
    class GraphicsContext {
    public:
        GraphicsContext(int width, int height, HWND nativeWnd, bool windowed);
        ~GraphicsContext();

        [[nodiscard]] std::unique_ptr<GraphicsDevice>& GetDeviceImpl();
        [[nodiscard]] std::unique_ptr<CommandQueue>& GetCommandQueueImpl();

        // TODO: TEMP
        [[nodiscard]] std::unique_ptr<Commander>& GetCommander();
        [[nodiscard]] std::unique_ptr<RootSignature>& GetRootSignature();

        [[nodiscard]] std::unique_ptr<SwapChain>& GetSwapChainImpl();

        [[nodiscard]] WindowData& GetWindowData();

        void Render();
    private:
        void BeginRecord();
        void EndRecord();
        void ExecuteCommand();
        void ClearScreen();
    private:
        std::unique_ptr<Viewport> p_vp;
        std::unique_ptr<ScissorRect> p_rect;
    private:
        WindowData _windowData;
    private:
        using Microsoft::WRL::ComPtr;
        ComPtr<ID3D12Device> _device;

        std::unique_ptr<GraphicsDevice>    p_device;
        std::unique_ptr<CommandQueue>      p_cmdQueue;
        std::unique_ptr<Commander>         p_commander;
        std::unique_ptr<Fence>             p_fence;
        std::unique_ptr<SwapChain>         p_swapChain;
        std::unique_ptr<RenderTargetView>  p_RTV;

        std::unique_ptr<RootSignature>     p_rootSignature;

        std::unique_ptr<VertexShader>      p_vertexShader;
        std::unique_ptr<PixelShader>       p_pixelShader;
        std::unique_ptr<PipelineState>     p_pipelineState;
    private:
        std::unique_ptr<Triangle>          p_triangle;
    };
}

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        std::cout << GetLastError() << std::endl;
        throw 1;
    }
}

#define DEVICE() gfx.GetDeviceImpl()->GetDevice()
#define DXGI() gfx.GetDeviceImpl()->GetDxgi()

#define CMD_QUEUE() gfx.GetCommandQueueImpl()->GetQueue()

// TODO: TEMP
#define CMD_LIST() gfx.GetCommander()->GetCmdList()
#define ROOT_SIGNATURE() gfx.GetRootSignature()->GetSignature()

#define SWAPCHAIN() gfx.GetSwapChainImpl()->GetSwapChain()
#define SWAPCHAIN_IMPL() gfx.GetSwapChainImpl()
