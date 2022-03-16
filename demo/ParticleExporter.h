void ExportPartices(FluidRenderBuffers* vBuffer, std::string vExportFilePath, int vFrameIndex, int vSolidParticleNum);
void ExportDiffusePartices(DiffuseRenderBuffers* diffuseBuffers, std::string vExportFilePath, int vFrameIndex, int vDiffuseCount);
void DeleteAllFile(std::string strPath);
void ExportMeshIndex(Mesh* mesh, std::string vExportFilePath);
void ExportMeshPos(Mesh* mesh, std::string vExportFilePath, int vFrameIndex);