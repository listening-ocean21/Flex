#include "fluidRenderD3D11.h"
#include "shadersDemoContext.h"
#include "demoContextD3D11.h"
#include "appD3D11Ctx.h"
#include <fstream>

struct SParticle
{
	float positionX;
	float positionY;
	float positionZ;
	float density;
};

bool isOver = false;
void ExportPartices(FluidRenderBuffers* vBuffer)
{
	if (isOver) return;

	FluidRenderBuffersD3D11& buffer = *reinterpret_cast<FluidRenderBuffersD3D11*>(vBuffer);
	AppGraphCtxD3D11* context = reinterpret_cast<DemoContextD3D11*>(GetDemoContext())->m_appGraphCtxD3D11;
	ID3D11Buffer *posBuffer, *densityBuffer;
	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_positions.Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &posBuffer);
		context->m_deviceContext->CopyResource(posBuffer, buffer.m_positions.Get());
	}

	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_densities.Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &densityBuffer);
		context->m_deviceContext->CopyResource(densityBuffer, buffer.m_densities.Get());
	}

	D3D11_MAPPED_SUBRESOURCE resultResources1, resultResources2;
	ZeroMemory(&resultResources1, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources2, sizeof(D3D11_MAPPED_SUBRESOURCE));
	
	context->m_deviceContext->Map(posBuffer, 0, D3D11_MAP_READ, 0, &resultResources1);
	SParticle* p = (SParticle*)resultResources1.pData;
	context->m_deviceContext->Map(densityBuffer, 0, D3D11_MAP_READ, 0, &resultResources2);
	float* pp = (float*)resultResources2.pData;
	SParticle* data = new SParticle[buffer.m_numParticles];
	for (int i = 0; i < buffer.m_numParticles; i++)
	{
		data[i].positionX = p[i].positionX;
		data[i].positionY = p[i].positionY;
		data[i].positionZ = p[i].positionZ;
		data[i].density = *pp;
	}
	context->m_deviceContext->Unmap(posBuffer, 0);
	context->m_deviceContext->Unmap(densityBuffer, 0);

	std::string exportFileName = "C:/Users/WT/Documents/Projects/6Fluid Rendering/Large Scale Fluid/Assets/Resources/OfflineSPHData/";
	exportFileName += std::to_string(1);
	exportFileName += ".bin";
	std::ofstream* outfile = new std::ofstream(exportFileName, std::ios::binary);
	if (!outfile->is_open())
	{
		std::cout << "Cannot open a file to save VTK particles.";
		return;
	}

	for (unsigned int i = 0; i < buffer.m_numParticles; i++)
	{
		outfile->write((char*)&data[i], sizeof(SParticle));
	}

	outfile->close();
	delete outfile;
}

