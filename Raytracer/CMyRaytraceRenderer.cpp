#include "pch.h"
#include "CMyRaytraceRenderer.h"

#include "graphics/GrTexture.h"
#include "Poisson.h"

void CMyRaytraceRenderer::SetWindow(CWnd* p_window)
{
	m_window = p_window;
}

bool CMyRaytraceRenderer::RendererStart()
{
	m_intersection.Initialize();

	m_mstack.clear();


	// We have to do all of the matrix work ourselves.
	// Set up the matrix stack.
	CGrTransform t;
	t.SetLookAt(Eye().X(), Eye().Y(), Eye().Z(),
		Center().X(), Center().Y(), Center().Z(),
		Up().X(), Up().Y(), Up().Z());

	m_mstack.push_back(t);

	m_material = nullptr;

	m_aliascnt = 4;
	m_reflections = 3;

	return true;
}

void CMyRaytraceRenderer::RendererMaterial(CGrMaterial* p_material)
{
	m_material = p_material;
}

void CMyRaytraceRenderer::RendererPushMatrix()
{
	m_mstack.push_back(m_mstack.back());
}

void CMyRaytraceRenderer::RendererPopMatrix()
{
	m_mstack.pop_back();
}

void CMyRaytraceRenderer::RendererRotate(double a, double x, double y, double z)
{
	CGrTransform r;
	r.SetRotate(a, CGrPoint(x, y, z));
	m_mstack.back() *= r;
}

void CMyRaytraceRenderer::RendererTranslate(double x, double y, double z)
{
	CGrTransform r;
	r.SetTranslate(x, y, z);
	m_mstack.back() *= r;
}

//
// Name : CMyRaytraceRenderer::RendererEndPolygon()
// Description : End definition of a polygon. The superclass has
// already collected the polygon information
//

void CMyRaytraceRenderer::RendererEndPolygon()
{
	const std::list<CGrPoint>& vertices = PolyVertices();
	const std::list<CGrPoint>& normals = PolyNormals();
	const std::list<CGrPoint>& tvertices = PolyTexVertices();

	// Allocate a new polygon in the ray intersection system
	m_intersection.PolygonBegin();
	m_intersection.Material(m_material);

	if (PolyTexture())
	{
		m_intersection.Texture(PolyTexture());
	}

	std::list<CGrPoint>::const_iterator normal = normals.begin();
	std::list<CGrPoint>::const_iterator tvertex = tvertices.begin();

	for (std::list<CGrPoint>::const_iterator i = vertices.begin(); i != vertices.end(); i++)
	{
		if (normal != normals.end())
		{
			m_intersection.Normal(m_mstack.back() * *normal);
			normal++;
		}

		if (tvertex != tvertices.end())
		{
			m_intersection.TexVertex(*tvertex);
			tvertex++;
		}

		m_intersection.Vertex(m_mstack.back() * *i);
	}

	m_intersection.PolygonEnd();
}

bool CMyRaytraceRenderer::RendererEnd()
{
	m_intersection.LoadingComplete();

	double ymin = -tan(ProjectionAngle() / 2. * GR_DTOR);
	double yhit = -ymin * 2.;

	double xmin = ymin * ProjectionAspect();
	double xwid = -xmin * 2.;

	//double minD = sqrt(double(m_rayimageheight * m_rayimagewidth)) / (sqrt(double(m_aliascnt)) * 2.);
	double minD = 1. / (sqrt(double(m_aliascnt)) * 2.);

	CPoisson poisson(minD);	//< Increasing Max => Blurrier Borders

	for (int r = 0; r < m_rayimageheight; r++)
	{
		for (int c = 0; c < m_rayimagewidth; c++)
		{
			poisson.Reset();
			shared_ptr<CGrPoint> colorTotal = make_shared<CGrPoint>( 0, 0, 0 );
			for (int a = 1; a <= m_aliascnt; a++)
			{
				shared_ptr<CGrPoint> p = poisson.Generate();

				// Anti-aliased
				double x = xmin + (c + p->X()) / m_rayimagewidth * xwid;
				double y = ymin + (r + p->Y()) / m_rayimageheight * yhit;

				// Construct a Ray
				CRay ray(CGrPoint(0, 0, 0), Normalize3(CGrPoint(x, y, -1, 0)));

				RayColor(ray, colorTotal, m_reflections);
			}
			
			*colorTotal /= double(m_aliascnt);

			m_rayimage[r][c * 3 + 0] = RangeClamp(colorTotal->X());
			m_rayimage[r][c * 3 + 1] = RangeClamp(colorTotal->Y());
			m_rayimage[r][c * 3 + 2] = RangeClamp(colorTotal->Z());
		}
		if (r % 10 == 0)
		{
			m_window->Invalidate();
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				DispatchMessage(&msg);
		}
	}

	return true;
}

void CMyRaytraceRenderer::RayColor(const CRay& p_ray, shared_ptr<CGrPoint> &p_color, int p_recurse, const CRayIntersection::Object* p_ignore)
{
	double t;                                   // Will be distance to intersection
	CGrPoint intersect;                         // Will by x,y,z location of intersection
	const CRayIntersection::Object* nearest;    // Pointer to intersecting object
	if (m_intersection.Intersect(p_ray, 1e20, nullptr, nearest, t, intersect))
	{
		// We hit something...
		// Determine information about the intersection
		CGrPoint normal;
		CGrMaterial* material;
		CGrTexture* texture;
		CGrPoint texCoord;
		m_intersection.IntersectInfo(p_ray, nearest, t, normal, material, texture, texCoord);

		if (material != nullptr)
		{
			// Base Color = Ambient
			shared_ptr<CGrPoint> color = make_shared<CGrPoint>(material->Ambient());

			// Sample the Texture, if we have one
			shared_ptr<CGrPoint> sample = texture != nullptr ? texture->Sample(texCoord) : nullptr;

			// Apply the Lighting Calculation for Every Light Source in the Scene
			for (int i = 0; i < LightCnt(); i++)
			{
				Light light = GetLight(i); //< Get the Light Source
				CGrPoint lightDirection = Normalize3(light.m_pos - intersect);
				double intensity = (CGrPoint(light.m_specular) + CGrPoint(light.m_ambient) + CGrPoint(light.m_diffuse)).Length3();

				// No Light Reaches this Surface
				if (Dot3(normal, lightDirection) < 0) continue;

				// Reverse the Light Direction, Search for Objects Between
				if (!ShadowFeeler(intersect, lightDirection, light, nearest))
				{
					// Calculate the Blinn-Phong Reflectance, Results stored to the class members
					BlinnPhong(intersect, normal, lightDirection, intensity, material);
					CGrPoint diffuse = *m_diffuse;
					CGrPoint specular = *m_specular;

					CGrPoint reflectionDir = Normalize3(Normalize3(-intersect) + Normalize3(lightDirection));
					shared_ptr<CGrPoint> reflectionColor = make_shared<CGrPoint>(0, 0, 0);
					
					if (p_recurse > 0)
					{
						RayColor(CRay(intersect, reflectionDir), reflectionColor, p_recurse - 1, nearest);
					}
					
					if (reflectionColor->Length3() <= 1e-8) reflectionColor = make_shared<CGrPoint>(1, 1, 1);

					// Use the Material Properties
					CGrPoint matDiffuse{ material->Diffuse() };
					CGrPoint matSpecular{ material->Specular() };

					// Get the Sampled Properties
					CGrPoint textureColor = sample != nullptr ? *sample : CGrPoint(1., 1., 1.);

					// Apply the Color Computation
					*color += (matDiffuse * textureColor * diffuse + matSpecular * specular * *reflectionColor) / (p_recurse > 0 ? p_recurse : 1);
				}

				// Normalize the Color using Light Intensity to Prevent Glaring
				*color /= intensity;
			}
			*p_color += *color;
		}
	}
}


void CMyRaytraceRenderer::BlinnPhong(CGrPoint pos, CGrPoint normal, CGrPoint lightDirection, double lightIntensity, CGrMaterial* material)
{
	// Light Reflectance Properties
	CGrPoint s = Normalize3(lightDirection);
	CGrPoint v = Normalize3(-pos);
	CGrPoint n = Normalize3(normal);
	CGrPoint h = Normalize3(v + s);

	// Surface Material Properties
	CGrPoint kAmbient{ material->Ambient() };
	CGrPoint kDiffuse{ material->Diffuse() };
	CGrPoint kSpecular{ material->Specular() };
	double kShininess = material->Shininess();

	// Products
	double diffuseProduct = lightIntensity * abs(Dot3(n, s));
	double specularProduct = pow(abs(Dot3(n, h)), kShininess);

	// Set the Results in the Class Members
	m_diffuse = make_shared<CGrPoint>(kAmbient + kDiffuse * diffuseProduct);
	m_specular = Dot3(s, n) < 0. ? make_shared<CGrPoint>(0., 0., 0.) : make_shared<CGrPoint>(kSpecular * specularProduct);
}


bool CMyRaytraceRenderer::ShadowFeeler(CGrPoint origin, CGrPoint direction, Light light, const CRayIntersection::Object* ignore)
{
	CRay ray(origin, direction);
	double distance = Distance(origin, light.m_pos);

	double t;                                   // Will be distance to intersection
	CGrPoint intersect;                         // Will by x,y,z location of intersection
	const CRayIntersection::Object* nearest;    // Pointer to intersecting object
	if (m_intersection.Intersect(ray, distance - 1e-8, ignore, nearest, t, intersect))
	{
		return true;
	}

	return false;
}


BYTE CMyRaytraceRenderer::RangeClamp(double value)
{
	//return BYTE(clamp(value * 255., 0., 255.));
	return BYTE(value * 255.);
}