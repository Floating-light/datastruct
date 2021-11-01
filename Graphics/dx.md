DXGI: DirectX Graphics Infrastructure

D3D Initialize: 

IDXGIFactory   IDXGISwapChain, IDXGIDevice, IDXGIAdaper, 

## Root Signatures

定义绑定到Graphics Pipeline的资源类型.

把Shader需要的资源link到CommandList.

Graphics CommandList有一个Graphics和一个Compute RootSignatures.

Compute CommandList 仅有一个Compute RootSignature. 

RootSignature之间互相独立.

Appendix:
IA Input Assembler(装配)

Tessellation Stages 曲面细分阶段