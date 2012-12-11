#include "CmD3D11GpuProgram.h"
#include "CmD3D11Device.h"
#include "CmException.h"
#include "CmDebug.h"

namespace CamelotEngine
{
	D3D11GpuProgram::D3D11GpuProgram(GpuProgramType type, GpuProgramProfile profile) 
		: GpuProgram("", "", "", type, profile)
	{

	}

	void D3D11GpuProgram::loadImpl(void)
	{
		// Call polymorphic load
		loadFromSource();
	}

	void D3D11GpuProgram::loadFromSource(void)
	{
		CM_EXCEPT(RenderingAPIException, "DirectX 11 doesn't support assembly shaders.");
	}

	D3D11GpuVertexProgram::D3D11GpuVertexProgram(GpuProgramProfile profile) 
		: D3D11GpuProgram(GPT_VERTEX_PROGRAM, profile)
		, mVertexShader(nullptr)
	{ }

	D3D11GpuVertexProgram::~D3D11GpuVertexProgram()
	{
		// have to call this here reather than in Resource destructor
		// since calling virtual methods in base destructors causes crash
		unload(); 
	}

	void D3D11GpuVertexProgram::loadFromMicrocode(D3D11Device& device, ID3D10Blob *  microcode)
	{
		if (isSupported())
		{
			// Create the shader
			HRESULT hr = device.getD3D11Device()->CreateVertexShader( 
				static_cast<DWORD*>(microcode->GetBufferPointer()), 
				microcode->GetBufferSize(),
				NULL,
				&mVertexShader);

			if (FAILED(hr) || device.hasError())
			{
				String errorDescription = device.getErrorDescription();
				CM_EXCEPT(RenderingAPIException, 
					"Cannot create D3D11 vertex shader from microcode\nError Description:" + errorDescription);

			}
		}
		else
		{
			LOGWRN("Unsupported D3D11 vertex shader was not loaded.");
		}
	}

	void D3D11GpuVertexProgram::unloadImpl(void)
	{
		SAFE_RELEASE(mVertexShader);
	}

	ID3D11VertexShader * D3D11GpuVertexProgram::getVertexShader( void ) const
	{
		return mVertexShader;
	}

	D3D11GpuFragmentProgram::D3D11GpuFragmentProgram(GpuProgramProfile profile) 
		: D3D11GpuProgram(GPT_FRAGMENT_PROGRAM, profile)
		, mPixelShader(nullptr)
	{ }

	D3D11GpuFragmentProgram::~D3D11GpuFragmentProgram()
	{
		unload(); 
	}

	void D3D11GpuFragmentProgram::loadFromMicrocode(D3D11Device& device, ID3D10Blob* microcode)
	{
		if (isSupported())
		{
			// Create the shader
			HRESULT hr = device.getD3D11Device()->CreatePixelShader(
				static_cast<DWORD*>(microcode->GetBufferPointer()), 
				microcode->GetBufferSize(),
				NULL,
				&mPixelShader);

			if (FAILED(hr) || device.hasError())
			{
				String errorDescription = device.getErrorDescription();
				CM_EXCEPT(RenderingAPIException, 
					"Cannot create D3D11 pixel shader from microcode.\nError Description:" + errorDescription);
			}
		}
		else
		{
			LOGWRN("Unsupported D3D11 pixel shader was not loaded.");
		}
	}

	void D3D11GpuFragmentProgram::unloadImpl(void)
	{
		SAFE_RELEASE(mPixelShader);
	}

	ID3D11PixelShader * D3D11GpuFragmentProgram::getPixelShader( void ) const
	{
		return mPixelShader;
	}

	D3D11GpuGeometryProgram::D3D11GpuGeometryProgram(GpuProgramProfile profile) 
		: D3D11GpuProgram(GPT_GEOMETRY_PROGRAM, profile)
		, mGeometryShader(nullptr)
	{ }

	D3D11GpuGeometryProgram::~D3D11GpuGeometryProgram()
	{
		unload(); 
	}

	void D3D11GpuGeometryProgram::loadFromMicrocode(D3D11Device& device, ID3D10Blob* microcode)
	{
		if (isSupported())
		{
			// Create the shader
			HRESULT hr = device.getD3D11Device()->CreateGeometryShader(
				static_cast<DWORD*>(microcode->GetBufferPointer()), 
				microcode->GetBufferSize(),
				NULL,
				&mGeometryShader);

			if (FAILED(hr) || device.hasError())
			{
				String errorDescription = device.getErrorDescription();
				CM_EXCEPT(RenderingAPIException, 
					"Cannot create D3D11 geometry shader from microcode.\nError Description:" + errorDescription);
			}
		}
		else
		{
			LOGWRN("Unsupported D3D11 geometry shader was not loaded.");
		}
	}

	void D3D11GpuGeometryProgram::unloadImpl(void)
	{
		SAFE_RELEASE(mGeometryShader);
	}

	ID3D11GeometryShader * D3D11GpuGeometryProgram::getGeometryShader(void) const
	{
		return mGeometryShader;
	}

	D3D11GpuDomainProgram::D3D11GpuDomainProgram(GpuProgramProfile profile) 
		: D3D11GpuProgram(GPT_DOMAIN_PROGRAM, profile)
		, mDomainShader(nullptr)
	{ }

	D3D11GpuDomainProgram::~D3D11GpuDomainProgram()
	{
		unload(); 
	}

	void D3D11GpuDomainProgram::loadFromMicrocode(D3D11Device& device, ID3D10Blob* microcode)
	{
		if (isSupported())
		{
			// Create the shader
			HRESULT hr = device.getD3D11Device()->CreateDomainShader(
				static_cast<DWORD*>(microcode->GetBufferPointer()), 
				microcode->GetBufferSize(),
				NULL,
				&mDomainShader);

			if (FAILED(hr) || device.hasError())
			{
				String errorDescription = device.getErrorDescription();
				CM_EXCEPT(RenderingAPIException, 
					"Cannot create D3D11 domain shader from microcode.\nError Description:" + errorDescription);
			}
		}
		else
		{
			LOGWRN("Unsupported D3D11 domain shader was not loaded.");
		}
	}

	void D3D11GpuDomainProgram::unloadImpl(void)
	{
		SAFE_RELEASE(mDomainShader);
	}

	ID3D11DomainShader * D3D11GpuDomainProgram::getDomainShader(void) const
	{
		return mDomainShader;
	}

	D3D11GpuHullProgram::D3D11GpuHullProgram(GpuProgramProfile profile) 
		: D3D11GpuProgram(GPT_HULL_PROGRAM, profile)
		, mHullShader(nullptr)
	{ }

	D3D11GpuHullProgram::~D3D11GpuHullProgram()
	{
		unload(); 
	}

	void D3D11GpuHullProgram::loadFromMicrocode(D3D11Device& device, ID3D10Blob* microcode)
	{
		if (isSupported())
		{
			// Create the shader
			HRESULT hr = device.getD3D11Device()->CreateHullShader(
				static_cast<DWORD*>(microcode->GetBufferPointer()), 
				microcode->GetBufferSize(),
				NULL,
				&mHullShader);

			if (FAILED(hr) || device.hasError())
			{
				String errorDescription = device.getErrorDescription();
				CM_EXCEPT(RenderingAPIException, 
					"Cannot create D3D11 hull shader from microcode.\nError Description:" + errorDescription);
			}
		}
		else
		{
			LOGWRN("Unsupported D3D11 hull shader was not loaded.");
		}
	}

	void D3D11GpuHullProgram::unloadImpl(void)
	{
		SAFE_RELEASE(mHullShader);
	}

	ID3D11HullShader* D3D11GpuHullProgram::getHullShader(void) const
	{
		return mHullShader;
	}
}