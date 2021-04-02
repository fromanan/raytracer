#pragma once
#include "ShaderUtilities.h"
#include "graphics/GrRenderer.h"
#include "graphics/RayIntersection.h"

#include "framework.h"
#include "graphics/GrObject.h"
#include "graphics/GrPoint.h"

using namespace glm;

class CMyRaytraceRenderer : public CGrRenderer
{
public:
    CMyRaytraceRenderer() { m_window = nullptr; }
    int     m_rayimagewidth;
    int     m_rayimageheight;
    BYTE**  m_rayimage;
	
    CWnd* m_window;

    CRayIntersection m_intersection;

    std::list<CGrTransform> m_mstack;
    CGrMaterial* m_material;

    shared_ptr<CGrPoint> m_diffuse;
    shared_ptr<CGrPoint> m_specular;

    int m_aliascnt;
    int m_reflections;

public:
    void SetImage(BYTE** image, int w, int h) { m_rayimage = image; m_rayimagewidth = w;  m_rayimageheight = h; }

    void SetWindow(CWnd* p_window);
    bool RendererStart();
    bool RendererEnd();
    void RayColor(const CRay& p_ray, shared_ptr<CGrPoint> &p_color, int p_recurse, const CRayIntersection::Object* p_ignore = nullptr);
    void BlinnPhong(CGrPoint pos, CGrPoint normal, CGrPoint lightDirection, double lightIntensity, CGrMaterial* material);
	bool ShadowFeeler(CGrPoint origin, CGrPoint direction, Light light, const CRayIntersection::Object* nearest);
    BYTE RangeClamp(double value);
    void RendererMaterial(CGrMaterial* p_material);

    virtual void RendererPushMatrix();
    virtual void RendererPopMatrix();
    virtual void RendererRotate(double a, double x, double y, double z);
    virtual void RendererTranslate(double x, double y, double z);
    void RendererEndPolygon();
};

