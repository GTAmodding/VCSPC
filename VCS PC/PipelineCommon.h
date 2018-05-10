#ifndef __PIPELINECOMMON
#define __PIPELINECOMMON

extern RwTexture *&gpWhiteTexture;
inline void pipeSetTexture(RwTexture *t, int n) { RwD3D9SetTexture(t ? t : gpWhiteTexture, n); };
void RwToD3DMatrix(void *d3d, RwMatrix *rw);
void pipeGetComposedTransformMatrix(RpAtomic *atomic, float *out);
void pipeGetWorldViewMatrix(RpAtomic *atomic, float *out);
void pipeUploadMatCol(int flags, RpMaterial *m, int loc);
void pipeUploadZero(int loc);
void pipeUploadLightDirection(RpLight *light, int loc);
void D3D9Render(RxD3D9ResEntryHeader *resEntryHeader, RxD3D9InstanceData *instanceData);
void D3D9RenderDual(int dual, RxD3D9ResEntryHeader *resEntryHeader, RxD3D9InstanceData *instanceData);

#endif
